/*****************************************************************************/
/*
*  @file    shDX11GraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
*  @brief   Graphics Manager for DirectX 11.
*
*  Graphics Manager for DirectX 11.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11GraphicsManager.h"
#include "shLogger.h"
#include "shScreen.h"
#include "shLinearColor.h"
#include "shException.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3dcompiler.h>
#include "DDSTextureLoader11.h"

using namespace DirectX;

namespace shEngineSDK {
class ShaderInclude : public ID3DInclude
{
 public:
  HRESULT __stdcall Open(D3D_INCLUDE_TYPE, LPCSTR pFileName,
    LPCVOID, LPCVOID* ppData, UINT* pBytes) override {
    std::ifstream file(pFileName, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return E_FAIL;

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    file.read(buffer, size);

    *ppData = buffer;
    *pBytes = static_cast<UINT>(size);
    return S_OK;
  }

  HRESULT __stdcall Close(LPCVOID pData) override {
    delete[] static_cast<const char*>(pData);
    return S_OK;
  }
};

static void
throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    SH_ASSERT(false && "Something went horribly wrong!!!");
  }
}

static bool
compileShaderFromFile(const String& fileName,
                      const String& entryPoint,
                      const String& shaderModel,
                      ID3DBlob** pBlob,
                      const Vector<ShaderMacro>& macros)
{
  //Logger& log = g_logger();

  HRESULT hr = S_OK;
  int32 shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if SH_DEBUG_MODE
  shaderFlags |= D3DCOMPILE_DEBUG;
#endif

  auto beg = (String::const_iterator)fileName.begin();
  auto end = (String::const_iterator)fileName.end();
  WString wFileName(beg, end);

  static ShaderInclude shaderInclude;
  ID3DBlob* pErrorBlob = nullptr;

  Vector<D3D_SHADER_MACRO> d3dMacros;

  if (!macros.empty()) {
    for (auto& currentMacro : macros) {
      D3D_SHADER_MACRO d3dM = {};
      d3dM.Name = currentMacro.name.c_str();
      d3dM.Definition = currentMacro.definition.c_str();
    }

    hr = D3DCompileFromFile(wFileName.c_str(),
                            d3dMacros.data(),
                            &shaderInclude,
                            entryPoint.c_str() ,
                            shaderModel.c_str(),
                            shaderFlags,
                            0,
                            pBlob,
                            &pErrorBlob);
  }
  else {
    hr = D3DCompileFromFile(wFileName.c_str(),
                            nullptr,
                            &shaderInclude,
                            entryPoint.c_str() ,
                            shaderModel.c_str(),
                            shaderFlags,
                            0,
                            pBlob,
                            &pErrorBlob);
  }

  if (FAILED(hr)) {
    if (nullptr != pErrorBlob) {
      String errStr(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
      //log.Log(errStr);
      SafeRelease(pErrorBlob);
    }

    return false;
  }

  SafeRelease(pErrorBlob);
  return true;
}

DX11GraphicsManager::~DX11GraphicsManager()
{

}

void
DX11GraphicsManager::internalInit(const WPtr<Screen> pScreen,
                                  const bool bAntiliasing,
                                  const SampleDesc& sample)
{
  if (pScreen.expired()) {
    SH_ASSERT(false && "Screen expired!");
    return;
  }
  auto screen = pScreen.lock();

  m_bFullScreen = screen->isFullscreen();
  
  auto hWnd = reinterpret_cast<HWND>(screen->getPlatformHandler());

  Vector<IDXGIAdapter*> vecAdapters;

  IDXGIAdapter* pAdapter = nullptr;
  IDXGIAdapter1* pAdapter1 = nullptr;

  IDXGIFactory* pFactory = nullptr;

  //Enumerate all graphics adapter
  CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory);

  uint32 iAdapter = 0;
  while (DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters(iAdapter, &pAdapter)) {
    DXGI_ADAPTER_DESC1 aDesc1;

    pAdapter1 = nullptr;
    pAdapter->QueryInterface(__uuidof(IDXGIAdapter1), (void**)&pAdapter1);
    pAdapter1->GetDesc1(&aDesc1);

    vecAdapters.push_back(pAdapter);
    ++iAdapter;
    SafeRelease(pAdapter1);
  }

  //Initialize D3D Device
  Vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_11_0,
                                              D3D_FEATURE_LEVEL_10_1,
                                              D3D_FEATURE_LEVEL_10_0,
                                              D3D_FEATURE_LEVEL_9_3 };
  D3D_FEATURE_LEVEL selectedFeatureLevel;
  
  uint32 deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if SH_DEBUG_MODE == 1
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  m_pDevice = sh_makeShared<DX11Device>();
  m_pDeviceContext = sh_makeShared<DX11DeviceContext>();

  //Create a device and immediate device context
  throwIfFailed(D3D11CreateDevice(vecAdapters[0],
                                  D3D_DRIVER_TYPE_UNKNOWN,
                                  nullptr,
                                  deviceFlags,
                                  &featureLevels[0],
                                  static_cast<UINT>(featureLevels.size()),
                                  D3D11_SDK_VERSION,
                                  &m_pDevice->m_pDevice,
                                  &selectedFeatureLevel,
                                  &m_pDeviceContext->m_pDeviceContext));

  //Create a swap chain
  DXGI_SWAP_CHAIN_DESC scDesc;
  memset(&scDesc, 0, sizeof(scDesc));

  scDesc.OutputWindow = hWnd;
  scDesc.Windowed = !m_bFullScreen;

  if (!m_bFullScreen) {
    scDesc.BufferDesc.Width = screen->getWidth();
    scDesc.BufferDesc.Height = screen->getHeight();
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  }

  scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

  m_multiSampleConfig.count = 1;
  m_multiSampleConfig.quality = 0;
  if (bAntiliasing)
  {
    m_multiSampleConfig = sample;
  }

  DXGI_SAMPLE_DESC multiSample;
  multiSample.Count = m_multiSampleConfig.count;
  multiSample.Quality = m_multiSampleConfig.quality;

  scDesc.SampleDesc = multiSample;
  scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
  scDesc.BufferCount = 2;
  scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  IDXGIDevice* dxgiDevice = nullptr;
  throwIfFailed(m_pDevice->m_pDevice->QueryInterface(__uuidof(IDXGIDevice),
                                                     reinterpret_cast<void**>(&dxgiDevice)));

  IDXGIAdapter* dxgiAdapter = nullptr;
  dxgiDevice->GetAdapter(&dxgiAdapter);

  IDXGIFactory* dxgiFactory;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
                         reinterpret_cast<void**>(&dxgiFactory));
  
  //m_pSwapChain = sh_makeUnique<IDXGISwapChain>();

  throwIfFailed(dxgiFactory->CreateSwapChain(m_pDevice->m_pDevice,
                                             &scDesc,
                                             &m_pSwapChain));

  //Get Backbuffer Interface
  //Create a render target view

  auto pBackbuffer = sh_makeShared<DX11Texture2D>();

  throwIfFailed(m_pSwapChain->GetBuffer(0,
                              __uuidof(ID3D11Texture2D),
                              reinterpret_cast<LPVOID*>(&pBackbuffer->m_pTexture2D)));

  throwIfFailed(m_pDevice->m_pDevice->CreateRenderTargetView(pBackbuffer->m_pTexture2D,
                                      nullptr,
                                      &pBackbuffer->m_pRenderTV));

  m_pDepthStencil = internalCreateTexture2D(scDesc.BufferDesc.Width,
                                            scDesc.BufferDesc.Height,
                                            DXGI_FORMAT_D24_UNORM_S8_UINT,
                                            D3D11_USAGE_DEFAULT,
                                            D3D11_BIND_DEPTH_STENCIL,
                                            1);
  
  m_pBackbuffer = pBackbuffer;

  //Setup the viewport
  Viewport viewPort;
  viewPort.width = static_cast<float>(scDesc.BufferDesc.Width);
  viewPort.height = static_cast<float>(scDesc.BufferDesc.Height);
  viewPort.minDepth = 0.0f;
  viewPort.maxDepth = 1.0f;
  viewPort.topLeftX = 0.0f;
  viewPort.topLeftY = 0.0f;
  internalSetViewport(viewPort);

  //Release all objects
  SafeRelease(pFactory);
  SafeRelease(dxgiFactory);
  SafeRelease(dxgiAdapter);
  SafeRelease(dxgiDevice);

  for (auto adap : vecAdapters)
  {
    SafeRelease(adap);
  }
}

void
DX11GraphicsManager::internalClearRenderTarget(const WPtr<Texture2D> pTarget,
                                               const LinearColor& color)
{
  if (pTarget.expired()) {
    return;
  }
  auto pRTV = sh_reinterpretPCast<DX11Texture2D>(pTarget.lock());
  
  FLOAT colorRGBA[4] = { color.r, color.g, color.b, color.a };

  m_pDeviceContext->m_pDeviceContext->ClearRenderTargetView(pRTV->m_pRenderTV,
                                                            colorRGBA);
}

void
DX11GraphicsManager::internalClearDepthStencil(const WPtr<Texture2D> pDepthSV,
                                               uint32 flags,
                                               float depth,
                                               uint8 stencil)
{
  if (pDepthSV.expired()) {
    return;
  }
  auto pDTV = sh_reinterpretPCast<DX11Texture2D>(pDepthSV.lock());
  
  m_pDeviceContext->m_pDeviceContext->ClearDepthStencilView(pDTV->m_pDepthSV,
                                                            flags,
                                                            depth,
                                                            stencil);
}

void
DX11GraphicsManager::internalPresent(uint32 syncInterval, uint32 flags)
{
  SH_ASSERT(m_pSwapChain);
  //DX11SwapChain* obj = reinterpret_cast<DX11SwapChain*>(m_pSwapChain.get());
  //IDXGISwapChain* pSwapChain = obj->m_pSwapChain;
  m_pSwapChain->Present(syncInterval, flags);
}

SPtr<Texture2D>
DX11GraphicsManager::internalGetMainRenderTargetView() const
{
  return m_pBackbuffer;
}

SPtr<Texture2D>
DX11GraphicsManager::internalGetMainDepthStencil() const
{
  return m_pDepthStencil;
}

SPtr<InputLayout>
DX11GraphicsManager::internalCreateInputLayout(const Vector<InputDesc>& desc,
                                               const WPtr<VertexShader> pPShader)
{
  if (pPShader.expired()) {
    return nullptr;
  }
  auto pInputLayout = sh_makeShared<DX11InputLayout>();
  auto pVertexShader = sh_reinterpretPCast<DX11VertexShader>(pPShader.lock());

  Vector<D3D11_INPUT_ELEMENT_DESC> dxInputDesc;
  dxInputDesc.resize(desc.size());

  UINT byteOffset = 0;
  for (uint32 i = 0; i < desc.size(); ++i) {
    auto& element = dxInputDesc[i];
    memset(&element, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));

    element.Format = static_cast<DXGI_FORMAT>(desc[i].format);
    element.SemanticIndex = 0;
    element.InputSlot = 0;
    element.AlignedByteOffset = byteOffset;
    element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    element.InstanceDataStepRate = 0;

    byteOffset += desc[i].size;

    if (desc[i].type == INPUT_LAYOUT_TYPES::kPosition) {
      element.SemanticName = "POSITION";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kNormal) {
      element.SemanticName = "NORMAL";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kTexcoord) {
      element.SemanticName = "TEXCOORD";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kTangents) {
      element.SemanticName = "TANGENT";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kBitangents) {
      element.SemanticName = "BINORMAL";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kBoneIndices) {
      element.SemanticName = "BLENDINDICES";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kBoneWieghts) {
      element.SemanticName = "BLENDWEIGHT";
    }
    else if (desc[i].type == INPUT_LAYOUT_TYPES::kColor) {
      element.SemanticName = "COLOR";
    }
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateInputLayout(&dxInputDesc[0],
                                      static_cast<UINT>(dxInputDesc.size()),
                                      pVertexShader->m_pBlob->GetBufferPointer(),
                                      pVertexShader->m_pBlob->GetBufferSize(),
                                      &pInputLayout->m_pLayout));

  return pInputLayout;
}

SPtr<InputLayout>
DX11GraphicsManager::internalCreateInputLayoutFromShader(const WPtr<VertexShader> pPShader)
{
  if (pPShader.expired()) {
    return nullptr;
  }
  auto pInputLayout = sh_makeShared<DX11InputLayout>();
  auto pVertexShader = sh_reinterpretPCast<DX11VertexShader>(pPShader.lock());

  ID3D11ShaderReflection* pReflector = nullptr;
  throwIfFailed((D3DReflect(pVertexShader->m_pBlob->GetBufferPointer(),
                            pVertexShader->m_pBlob->GetBufferSize(),
                            __uuidof(ID3D11ShaderReflection),
                            reinterpret_cast<void**>(&pReflector))));

  D3D11_SHADER_DESC shaderDesc;
  pReflector->GetDesc(&shaderDesc);

  Vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;

  UINT byteOffset = 0;
  for (uint32 i = 0; i < shaderDesc.InputParameters; ++i) {
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    pReflector->GetInputParameterDesc(i, &paramDesc);

    D3D11_INPUT_ELEMENT_DESC element;
    element.SemanticName = paramDesc.SemanticName;
    element.SemanticIndex = paramDesc.SemanticIndex;
    element.InputSlot = 0;
    element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    //element.AlignedByteOffset = byteOffset;
    element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    element.InstanceDataStepRate = 0;

    if (paramDesc.Mask == 1) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element.Format = DXGI_FORMAT_R32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element.Format = DXGI_FORMAT_R32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element.Format = DXGI_FORMAT_R32_FLOAT;
      }
      byteOffset += 1;
    }
    else if (paramDesc.Mask <= 3) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element.Format = DXGI_FORMAT_R32G32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element.Format = DXGI_FORMAT_R32G32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element.Format = DXGI_FORMAT_R32G32_FLOAT;
      }
      byteOffset += 2;
    }
    else if (paramDesc.Mask <= 7) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element.Format = DXGI_FORMAT_R32G32B32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element.Format = DXGI_FORMAT_R32G32B32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      }
      byteOffset += 3;
    }
    else if (paramDesc.Mask <= 15) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element.Format = DXGI_FORMAT_R32G32B32A32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element.Format = DXGI_FORMAT_R32G32B32A32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
      byteOffset += 4;
    }

    ilDesc.push_back(element);
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateInputLayout(&ilDesc[0],
                                      static_cast<UINT>(ilDesc.size()),
                                      pVertexShader->m_pBlob->GetBufferPointer(),
                                      pVertexShader->m_pBlob->GetBufferSize(),
                                      &pInputLayout->m_pLayout));

  SafeRelease(pReflector);

  return pInputLayout;
}

SPtr<VertexShader>
DX11GraphicsManager::internalCreateVertexShader(const String& fileName,
                                                const String& entryPoint,
                                                const String& shaderModel,
                                                const Vector<ShaderMacro>& macros)
{
  auto pVertexShader = sh_makeShared<DX11VertexShader>();

  if (!compileShaderFromFile(fileName,
                             entryPoint,
                             shaderModel,
                             &pVertexShader->m_pBlob,
                             macros)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateVertexShader(
                                      pVertexShader->m_pBlob->GetBufferPointer(),
                                      pVertexShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pVertexShader->m_pVertexShader));

  return pVertexShader;
}

SPtr<PixelShader>
DX11GraphicsManager::internalCreatePixelShader(const String& fileName,
                                               const String& entryPoint,
                                               const String& shaderModel,
                                               const Vector<ShaderMacro>& macros)
{
  auto pPixelShader = sh_makeShared<DX11PixelShader>();

  if (!compileShaderFromFile(fileName,
                             entryPoint,
                             shaderModel,
                             &pPixelShader->m_pBlob,
                             macros)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreatePixelShader(
                                      pPixelShader->m_pBlob->GetBufferPointer(),
                                      pPixelShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pPixelShader->m_pPixelShader));

  return pPixelShader;
}

SPtr<GeometryShader>
DX11GraphicsManager::internalCreateGeometryShader(const String& fileName,
                                                  const String& entryPoint,
                                                  const String& shaderModel,
                                                  const Vector<ShaderMacro>& macros)
{
  auto pGeometryShader = sh_makeShared<DX11GeometryShader>();

  if (!compileShaderFromFile(fileName,
                             entryPoint,
                             shaderModel,
                             &pGeometryShader->m_pBlob,
                             macros)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateGeometryShader(
                                      pGeometryShader->m_pBlob->GetBufferPointer(),
                                      pGeometryShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pGeometryShader->m_pGeometryShader));

  return pGeometryShader;
}

SPtr<ComputeShader>
DX11GraphicsManager::internalCreateComputeShader(const String& fileName,
                                                 const String& entryPoint,
                                                 const String& shaderModel,
                                                 const Vector<ShaderMacro>& macros)
{
  auto pComputeShader = sh_makeShared<DX11ComputeShader>();

  if (!compileShaderFromFile(fileName,
                             entryPoint,
                             shaderModel,
                             &pComputeShader->m_pBlob,
                             macros)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateComputeShader(
                                      pComputeShader->m_pBlob->GetBufferPointer(),
                                      pComputeShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pComputeShader->m_pComputeShader));

  return pComputeShader;
}

SPtr<VertexBuffer>
DX11GraphicsManager::internalCreateVertexBuffer(const void* pData,
                                                const uint32 bufferSize,
                                                const uint32 stride,
                                                const uint32 usage)
{
  auto pVBuffer = std::make_shared<DX11VertexBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = bufferSize * stride;
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = pData;
  initData.SysMemPitch = bufferSize;
  initData.SysMemSlicePitch = 0;

  m_pDevice->m_pDevice->CreateBuffer(&desc, &initData, &pVBuffer->m_pBuffer);
  pVBuffer->m_stride = stride;

  return pVBuffer;
}

SPtr<IndexBuffer>
DX11GraphicsManager::internalCreateIndexBuffer(const Vector<uint32>& indices,
                                               const uint32 usage)
{
  auto pIBuffer = std::make_shared<DX11IndexBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(uint32));
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = &indices[0];
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  m_pDevice->m_pDevice->CreateBuffer(&desc, &initData, &pIBuffer->m_pBuffer);
  pIBuffer->m_dataFormat = DXGI_FORMAT_R32_UINT;

  return pIBuffer;
}

SPtr<ConstantBuffer>
DX11GraphicsManager::internalCreateConstantBuffer(const uint32 bufferSize,
                                                  const uint32 usage,
                                                  const void* pData)
{
  auto pCBuffer = sh_makeShared<DX11ConstantBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = bufferSize;
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ?
                                 D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  if (pData) {
    initData.pSysMem = pData;
    initData.SysMemPitch = bufferSize;
    initData.SysMemSlicePitch = 0;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateBuffer(&desc,
                                                   pData ? &initData : nullptr,
                                                   &pCBuffer->m_pBuffer));

  return pCBuffer;
}

SPtr<SamplerState>
DX11GraphicsManager::internalCreateSamplerState(const uint32 filter, const uint32 textAddress)
{
  auto pSampleLinear = sh_makeShared<DX11SamplerState>();

  D3D11_SAMPLER_DESC sampDesc;
  memset(&sampDesc, 0, sizeof(sampDesc));
  sampDesc.Filter = static_cast<D3D11_FILTER>(filter);
  sampDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(textAddress);
  sampDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(textAddress);
  sampDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(textAddress);
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  throwIfFailed(m_pDevice->m_pDevice->CreateSamplerState(&sampDesc,
                                                         &pSampleLinear->m_pSamplerLinear));

  return pSampleLinear;
}

SPtr<Texture2D>
DX11GraphicsManager::internalCreateTextureFromFile(const void* pData,
                                                   const int32 width,
                                                   const int32 height,
                                                   const int32 bpp)
{
  int32 pitch = width * bpp;

  auto pTexture = sh_reinterpretPCast<DX11Texture2D>(internalCreateTexture2D(
                                                     width,
                                                     height,
                                                     DXGI_FORMAT_R8G8B8A8_UNORM,
                                                     D3D11_USAGE_DEFAULT,
                                                     D3D11_BIND_SHADER_RESOURCE,
                                                     1));

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pTexture->m_pTexture2D,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        pitch,
                                                        0);
  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::internalCreateTextureFromDDS(const String& fileName)
{
  auto pTexture = sh_makeShared<DX11Texture2D>();

  SystemPath path = fileName;
  
  throwIfFailed(CreateDDSTextureFromFile(m_pDevice->m_pDevice,
                          path.wstring().c_str(),
                          reinterpret_cast<ID3D11Resource**>(&pTexture->m_pTexture2D),
                          &pTexture->m_pShaderRV));

  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::internalCreateTexture2D(const uint32 width,
                                             const uint32 height,
                                             const uint32 format,
                                             const uint32 usage,
                                             const uint32 bindFlags,
                                             const uint32 mipLevels)
{
  auto pTexture = std::make_shared<DX11Texture2D>();

  D3D11_TEXTURE2D_DESC textureDesc;
  memset(&textureDesc, 0, sizeof(textureDesc));
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = mipLevels;
  textureDesc.ArraySize = 1;
  textureDesc.Format = static_cast<DXGI_FORMAT>(format);
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
  textureDesc.BindFlags = bindFlags;
  textureDesc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ?  D3D11_CPU_ACCESS_WRITE : 0;
  textureDesc.MiscFlags = 0;

  throwIfFailed(m_pDevice->m_pDevice->CreateTexture2D(&textureDesc,
                                                      nullptr,
                                                      &pTexture->m_pTexture2D));

  if ((bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderRVDesc;
    memset(&shaderRVDesc, 0, sizeof(shaderRVDesc));
    if (textureDesc.Format == DXGI_FORMAT_R32_TYPELESS ||
        textureDesc.Format == DXGI_FORMAT_D32_FLOAT) {
      textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
      shaderRVDesc.Format = textureDesc.Format;
    }
    else {
      shaderRVDesc.Format = textureDesc.Format;
    }
    shaderRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderRVDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    shaderRVDesc.Texture2D.MostDetailedMip = 0;
    throwIfFailed(m_pDevice->m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                                                                 &shaderRVDesc,
                                                                 &pTexture->m_pShaderRV));
  }

  if ((bindFlags & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL)
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    memset(&descDSV, 0, sizeof(descDSV));
    if (textureDesc.Format == DXGI_FORMAT_R32_TYPELESS ||
        textureDesc.Format == DXGI_FORMAT_R32_FLOAT) {
      textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
      descDSV.Format = textureDesc.Format;
    }
    else {
      descDSV.Format = textureDesc.Format;
    }
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    throwIfFailed(m_pDevice->m_pDevice->CreateDepthStencilView(pTexture->m_pTexture2D,
                                                               &descDSV,
                                                               &pTexture->m_pDepthSV));
  }

  if ((bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET)
  {
    D3D11_RENDER_TARGET_VIEW_DESC descRTV;
    memset(&descRTV, 0, sizeof(descRTV));
    descRTV.Format = textureDesc.Format;
    descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    descRTV.Texture2D.MipSlice = 0;
    throwIfFailed(m_pDevice->m_pDevice->CreateRenderTargetView(pTexture->m_pTexture2D,
                                                               &descRTV,
                                                               &pTexture->m_pRenderTV));
  }

  if ((bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS)
  {
    D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
    memset(&descUAV, 0, sizeof(descUAV));
    descUAV.Format = textureDesc.Format;
    descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    descUAV.Texture2D.MipSlice = 0;
    throwIfFailed(m_pDevice->m_pDevice->CreateUnorderedAccessView(pTexture->m_pTexture2D,
                                                                  &descUAV,
                                                                  &pTexture->m_pUnorderedAV));
  }

  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::internalCreateErrorTexture()
{
  auto pTexture = sh_makeShared<DX11Texture2D>();
  uint32 errorSize = 128;
  Vector<uint32> pixels;
  pixels.resize(errorSize * errorSize);

  for (uint32 y = 0; y < errorSize; ++y) {
    for (uint32 x = 0; x < errorSize; ++x) {
      bool isPink = ((x / 16) % 2) == ((y / 16) % 2);
      pixels[y * errorSize + x] = isPink ? 0xFFFF00FF : 0xFF000000;
    }
  }

  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = errorSize;
  desc.Height = errorSize;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  D3D11_SUBRESOURCE_DATA initData = {};
  initData.pSysMem = pixels.data();
  initData.SysMemPitch = errorSize * sizeof(uint32);

  throwIfFailed(m_pDevice->m_pDevice->CreateTexture2D(&desc,
                                                      &initData,
                                                      &pTexture->m_pTexture2D));

  throwIfFailed(m_pDevice->m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                                                               nullptr,
                                                               &pTexture->m_pShaderRV));

  return pTexture;
}

SPtr<BlendState>
DX11GraphicsManager::internalCreateBlendState(const BlendDesc& blendDesc,
                                              const LinearColor& blendFactor)
{
  auto pBlendState = sh_makeShared<DX11BlendState>();

  D3D11_BLEND_DESC d3d11BlendDesc = {};
  d3d11BlendDesc.RenderTarget[0].BlendEnable = blendDesc.renderTarget[0].blendEnable;
  d3d11BlendDesc.RenderTarget[0].SrcBlend =
    static_cast<D3D11_BLEND>(blendDesc.renderTarget[0].srcBlend);
  d3d11BlendDesc.RenderTarget[0].DestBlend =
    static_cast<D3D11_BLEND>(blendDesc.renderTarget[0].destBlend);
  d3d11BlendDesc.RenderTarget[0].BlendOp =
    static_cast<D3D11_BLEND_OP>(blendDesc.renderTarget[0].blendOp);
  d3d11BlendDesc.RenderTarget[0].SrcBlendAlpha =
    static_cast<D3D11_BLEND>(blendDesc.renderTarget[0].srcBlendAlpha);
  d3d11BlendDesc.RenderTarget[0].DestBlendAlpha =
    static_cast<D3D11_BLEND>(blendDesc.renderTarget[0].destBlendAlpha);
  d3d11BlendDesc.RenderTarget[0].BlendOpAlpha =
    static_cast<D3D11_BLEND_OP>(blendDesc.renderTarget[0].blendOpAlpha);
  d3d11BlendDesc.RenderTarget[0].RenderTargetWriteMask =
    blendDesc.renderTarget[0].renderTargetWriteMask;

  m_pDevice->m_pDevice->CreateBlendState(&d3d11BlendDesc, &pBlendState->m_pBlendS);

  pBlendState->m_blendFactor = blendFactor;

  return pBlendState;
}

SPtr<RasterizerState>
DX11GraphicsManager::internalCreateRasterizerState(const RasterizerDesc& rasterizerDesc)
{
  auto pRasterizerState = sh_makeShared<DX11RasterizerState>();

  D3D11_RASTERIZER_DESC rasterDesc = {};
  rasterDesc.FillMode = static_cast<D3D11_FILL_MODE>(rasterizerDesc.fillMode);
  rasterDesc.CullMode = static_cast<D3D11_CULL_MODE>(rasterizerDesc.cullMode);
  rasterDesc.FrontCounterClockwise = rasterizerDesc.frontCounterClockwise;
  rasterDesc.DepthBias = rasterizerDesc.depthBias;
  rasterDesc.DepthBiasClamp = rasterizerDesc.depthBiasClamp;
  rasterDesc.SlopeScaledDepthBias = rasterizerDesc.slopeScaledDepthBias;
  rasterDesc.DepthClipEnable = rasterizerDesc.depthClipEnable;
  rasterDesc.ScissorEnable = rasterizerDesc.scissorEnable;
  rasterDesc.MultisampleEnable = rasterizerDesc.multisampleEnable;
  rasterDesc.AntialiasedLineEnable = rasterizerDesc.antialiasedLineEnable;

  m_pDevice->m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState->m_pRasterS);

  return pRasterizerState;
}

SPtr<DepthStencilState>
DX11GraphicsManager::internalCreateDepthStencilState(const DepthStencilDesc& depthSDesc)
{
  auto pDepthSS = sh_makeShared<DX11DepthStencilState>();

  D3D11_DEPTH_STENCIL_DESC d3d11DepthDesc = {};
  d3d11DepthDesc.DepthEnable = depthSDesc.depthEnable;
  d3d11DepthDesc.DepthWriteMask =
    static_cast<D3D11_DEPTH_WRITE_MASK>(depthSDesc.depthWriteMask);
  d3d11DepthDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(depthSDesc.depthFunc);
  d3d11DepthDesc.StencilEnable = depthSDesc.stencilEnable;
  d3d11DepthDesc.StencilReadMask = depthSDesc.stencilReadMask;
  d3d11DepthDesc.StencilWriteMask = depthSDesc.stencilWriteMask;

  D3D11_DEPTH_STENCILOP_DESC frontFace = {};
  frontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilFailOp);
  frontFace.StencilDepthFailOp =
    static_cast<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilDepthFailOp);
  frontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilPassOp);
  frontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(depthSDesc.frontFace.stencilFunc);

  D3D11_DEPTH_STENCILOP_DESC backFace = {};
  backFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilFailOp);
  backFace.StencilDepthFailOp =
    static_cast<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilDepthFailOp);
  backFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilPassOp);
  backFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(depthSDesc.backFace.stencilFunc);

  d3d11DepthDesc.FrontFace = frontFace;
  d3d11DepthDesc.BackFace = backFace;

  m_pDevice->m_pDevice->CreateDepthStencilState(&d3d11DepthDesc, &pDepthSS->m_pDepthSS);

  return pDepthSS;
}

void
DX11GraphicsManager::internalGenerateMips(const WPtr<Texture2D> pTexture)
{
  if (pTexture.expired())
  {
    return;
  }
  auto pTex = sh_reinterpretPCast<DX11Texture2D>(pTexture.lock());

  if (pTex->m_pShaderRV) {
    m_pDeviceContext->m_pDeviceContext->GenerateMips(pTex->m_pShaderRV);
  }
}

void
DX11GraphicsManager::internalUpdateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                                                  const void* pData,
                                                  const uint32 dataSize)
{
  if (pCBuffer.expired()) {
    return;
  }
  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pConstantBuffer->m_pBuffer,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        dataSize,
                                                        0);
}

void
DX11GraphicsManager::internalUpdateTexture2D(WPtr<Texture2D> pTexture,
                                             uint8* pData,
                                             uint32 width,
                                             uint32 bpp)
{
  if (pTexture.expired() || pData == nullptr) {
    return;
  }
  auto pTex2D = sh_reinterpretPCast<DX11Texture2D>(pTexture.lock());
  int32 pitch = width * bpp;

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pTex2D->m_pTexture2D,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        pitch,
                                                        0);
}

void
DX11GraphicsManager::internalUpdateScreenSize(const Vector2& size)
{
  uint32 width = static_cast<uint32>(size.x);
  uint32 height = static_cast<uint32>(size.y);

  if (m_pDepthStencil) {
    m_pDepthStencil = nullptr;
  }
  if (m_pBackbuffer) {
    m_pBackbuffer = nullptr;
  }

  //DX11SwapChain* obj = reinterpret_cast<DX11SwapChain*>(m_pSwapChain.get());
  //IDXGISwapChain* pSwapChain = obj->m_pSwapChain;
  m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  auto pBackbuffer = sh_makeShared<DX11Texture2D>();
  throwIfFailed(m_pSwapChain->GetBuffer(0,
                __uuidof(ID3D11Texture2D),
                reinterpret_cast<LPVOID*>(&pBackbuffer->m_pTexture2D)));

  throwIfFailed(m_pDevice->m_pDevice->CreateRenderTargetView(pBackbuffer->m_pTexture2D,
                                      nullptr,
                                      &pBackbuffer->m_pRenderTV));
  m_pBackbuffer = pBackbuffer;

  m_pDepthStencil = internalCreateTexture2D(width,
                                            height,
                                            DXGI_FORMAT_D24_UNORM_S8_UINT,
                                            D3D11_USAGE_DEFAULT,
                                            D3D11_BIND_DEPTH_STENCIL,
                                            1);

  /*m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(1,
                                                         &m_pRenderTargetView->m_pRenderTV,
                                                         m_pDepthStencil->m_pDepthSV);*/

  //Setup the viewport
  Viewport viewPort;
  viewPort.width = static_cast<float>(width);
  viewPort.height = static_cast<float>(height);
  viewPort.minDepth = 0.0f;
  viewPort.maxDepth = 1.0f;
  viewPort.topLeftX = 0.0f;
  viewPort.topLeftY = 0.0f;
  internalSetViewport(viewPort);
}

void
DX11GraphicsManager::internalSaveTextureToDDS(const WPtr<Texture2D> pTexture,
                                              const String& filePath)
{
  SH_UNREFERENCED_PARAMETER(filePath);
  if (pTexture.expired()) {
    return;
  }
  /*
  auto pResTex = sh_reinterpretPCast<DX11Texture2D>(pTexture);

  ScratchImage image;

  throwIfFailed(CaptureTexture(m_pDevice->m_pDevice,
                               m_pDeviceContext->m_pDeviceContext,
                               pResTex->m_pTexture2D,
                               image));

  SystemPath sFileName = filePath;

  throwIfFailed(SaveToDDSFile(image.GetImages(),
                              image.GetImageCount(),
                              image.GetMetadata(),
                              DDS_FLAGS_NONE,
                              sFileName.wstring().c_str()));
  */
}

void
DX11GraphicsManager::internalSetViewport(const Viewport& vp)
{
  D3D11_VIEWPORT viewPort;
  viewPort.Width = vp.width;
  viewPort.Height = vp.height;
  viewPort.MinDepth = vp.minDepth;
  viewPort.MaxDepth = vp.maxDepth;
  viewPort.TopLeftX = vp.topLeftX;
  viewPort.TopLeftY = vp.topLeftY;
  m_pDeviceContext->m_pDeviceContext->RSSetViewports(1, &viewPort);
}

void
DX11GraphicsManager::internalSetRenderTargets(const Vector<WPtr<Texture2D>>& pRenderTVs,
                                              const WPtr<Texture2D> pDepthSV)
{
  Vector<ID3D11RenderTargetView*> pRTVs;

  for (auto& pRenderTarget : pRenderTVs) {
    ID3D11RenderTargetView* pD3D11RTV = nullptr;

    if (!pRenderTarget.expired()) {
      auto pRT = pRenderTarget.lock();
      pD3D11RTV = reinterpret_cast<DX11Texture2D*>(pRT.get())->m_pRenderTV;
    }
    
    pRTVs.push_back(pD3D11RTV);
  }

  for (uint32 i = pRTVs.size(); i < 8; ++i) {
    pRTVs.push_back(nullptr);
  }

  ID3D11DepthStencilView* pDSV = nullptr;
  if (!pDepthSV.expired()) {
    auto pDepthStencil = sh_reinterpretPCast<DX11Texture2D>(pDepthSV.lock());
    pDSV = pDepthStencil->m_pDepthSV;
  }

  m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(static_cast<UINT>(pRTVs.size()),
                                                         pRTVs.data(),
                                                         pDSV);
}

void
DX11GraphicsManager::internalSetInputLayout(const WPtr<InputLayout> pInput)
{
  if (pInput.expired()) {
    return;
  }
  auto pInputLayout = sh_reinterpretPCast<DX11InputLayout>(pInput.lock());
  m_pDeviceContext->m_pDeviceContext->IASetInputLayout(pInputLayout->m_pLayout);
}

void
DX11GraphicsManager::internalSetVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                                              const uint32 startSlot,
                                              const uint32 numBuffers,
                                              const uint32 offset)
{
  SH_ASSERT(m_pDeviceContext && m_pDeviceContext->m_pDeviceContext);
  auto& pDC = m_pDeviceContext->m_pDeviceContext;

  if (!pVBuffer.expired()) {
    auto pVertexBuffer = sh_reinterpretPCast<DX11VertexBuffer>(pVBuffer.lock());

    pDC->IASetVertexBuffers(startSlot,
                            numBuffers,
                            &pVertexBuffer->m_pBuffer,
                            &pVertexBuffer->m_stride,
                            &offset);
  }
  else {
    ID3D11Buffer* pVB = nullptr;
    pDC->IASetVertexBuffers(startSlot,
                            numBuffers,
                            &pVB,
                            0,
                            &offset);
  }
}

void
DX11GraphicsManager::internalSetIndexBuffers(const WPtr<IndexBuffer> pIBuffer,
                                             const uint32 offset)
{
  SH_ASSERT(m_pDeviceContext && m_pDeviceContext->m_pDeviceContext);
  auto& pDC = m_pDeviceContext->m_pDeviceContext;

  if(!pIBuffer.expired()){
    auto pIndexBuffer = sh_reinterpretPCast<DX11IndexBuffer>(pIBuffer.lock());
    
    pDC->IASetIndexBuffer(pIndexBuffer->m_pBuffer,
                          static_cast<DXGI_FORMAT>(pIndexBuffer->m_dataFormat),
                          offset);
  }
  else {
    ID3D11Buffer* pIB = nullptr;
    pDC->IASetIndexBuffer(pIB, DXGI_FORMAT_UNKNOWN, offset);
  }
}

void
DX11GraphicsManager::internalVSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->m_pDeviceContext->VSSetConstantBuffers(startSlot,
                                                             numBuffers,
                                                             &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->m_pDeviceContext->VSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::internalPSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->m_pDeviceContext->PSSetConstantBuffers(startSlot,
                                                             numBuffers,
                                                             &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->m_pDeviceContext->PSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::internalGSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->m_pDeviceContext->GSSetConstantBuffers(startSlot,
                                                             numBuffers,
                                                             &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->m_pDeviceContext->GSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::internalCSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->m_pDeviceContext->CSSetConstantBuffers(startSlot,
                                                             numBuffers,
                                                             &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->m_pDeviceContext->CSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::internalSetPrimitiveTopology(uint32 primitive)
{
  m_pDeviceContext->m_pDeviceContext->IASetPrimitiveTopology(
                                      static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitive));
}

void
DX11GraphicsManager::internalSetVertexShader(const WPtr<VertexShader> pVShader,
                                             const void* ppClassInstances,
                                             const uint32 numClassInstances)
{
  if (pVShader.expired()) {
    return;
  }
  auto pVS = sh_reinterpretPCast<DX11VertexShader>(pVShader.lock());

  m_pDeviceContext->m_pDeviceContext->VSSetShader(
                    pVS->m_pVertexShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::internalSetPixelShader(const WPtr<PixelShader> pPShader,
                                            const void* ppClassInstances,
                                            const uint32 numClassInstances)
{
  if (pPShader.expired()) {
    return;
  }
  auto pPS = sh_reinterpretPCast<DX11PixelShader>(pPShader.lock());

  m_pDeviceContext->m_pDeviceContext->PSSetShader(
                    pPS->m_pPixelShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::internalSetGeometryShader(const WPtr<GeometryShader> pGShader,
                                               const void* ppClassInstances,
                                               const uint32 numClassInstances)
{
  if (pGShader.expired()) {
    return;
  }
  auto pGS = sh_reinterpretPCast<DX11GeometryShader>(pGShader.lock());

  m_pDeviceContext->m_pDeviceContext->GSSetShader(
                    pGS->m_pGeometryShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::internalSetComputeShader(const WPtr<ComputeShader> pCShader,
                                              const void* ppClassInstances,
                                              const uint32 numClassInstances)
{
  if (pCShader.expired()) {
    return;
  }
  auto pCS = sh_reinterpretPCast<DX11ComputeShader>(pCShader.lock());

  m_pDeviceContext->m_pDeviceContext->CSSetShader(
                    pCS->m_pComputeShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::internalPSSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                                     const uint32 startSlot,
                                                     const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->m_pDeviceContext->PSSetShaderResources(startSlot,
                                                             numViews,
                                                             &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture2D>(pShaderRV.lock());

  m_pDeviceContext->m_pDeviceContext->PSSetShaderResources(startSlot,
                                                           numViews,
                                                           &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::internalCSSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                                     const uint32 startSlot,
                                                     const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->m_pDeviceContext->CSSetShaderResources(startSlot,
                                                             numViews,
                                                             &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture2D>(pShaderRV.lock());

  m_pDeviceContext->m_pDeviceContext->CSSetShaderResources(startSlot,
                                                           numViews,
                                                           &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::internalSetUnorderedAccessView(const WPtr<Texture2D> pUAV,
                                                    const uint32 startSlot,
                                                    const uint32 numViews,
                                                    const uint32* count)
{
  SH_UNREFERENCED_PARAMETER(count);
  if (pUAV.expired()) {
    ID3D11UnorderedAccessView* dx11UAV = nullptr;
    m_pDeviceContext->m_pDeviceContext->CSSetUnorderedAccessViews(startSlot,
                                                                  numViews,
                                                                  &dx11UAV,
                                                                  nullptr);
    return;
  }

  auto pUAVTexture = sh_reinterpretPCast<DX11Texture2D>(pUAV.lock());
  m_pDeviceContext->m_pDeviceContext->CSSetUnorderedAccessViews(startSlot,
                                                                numViews,
                                                                &pUAVTexture->m_pUnorderedAV,
                                                                nullptr);
}

void
DX11GraphicsManager::internalPSSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                               const uint32 startSlot,
                                               const uint32 numSamplers)
{
  if (!pSamplerLinear.expired()) {
    auto pSampler = sh_reinterpretPCast<DX11SamplerState>(pSamplerLinear.lock());

    m_pDeviceContext->m_pDeviceContext->PSSetSamplers(startSlot,
                                                      numSamplers,
                                                      &pSampler->m_pSamplerLinear);
  }
  else {
    ID3D11SamplerState* pSampler = nullptr;
    m_pDeviceContext->m_pDeviceContext->PSSetSamplers(0, 1, &pSampler);
  }
}

void
DX11GraphicsManager::internalCSSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                               const uint32 startSlot,
                                               const uint32 numSamplers)
{
  if (!pSamplerLinear.expired()) {
    auto pSampler = sh_reinterpretPCast<DX11SamplerState>(pSamplerLinear.lock());

    m_pDeviceContext->m_pDeviceContext->CSSetSamplers(startSlot,
                                                      numSamplers,
                                                      &pSampler->m_pSamplerLinear);
  }
  else {
    ID3D11SamplerState* pSampler = nullptr;
    m_pDeviceContext->m_pDeviceContext->CSSetSamplers(0, 1, &pSampler);
  }
}

void
DX11GraphicsManager::internalSetBlendState(const WPtr<BlendState> pBlendState)
{
  if (pBlendState.expired()) {
    return;
  }
  auto pBS = sh_reinterpretPCast<DX11BlendState>(pBlendState.lock());
  FLOAT bf[4] = { pBS->m_blendFactor.r,
                  pBS->m_blendFactor.g,
                  pBS->m_blendFactor.b,
                  pBS->m_blendFactor.a };
  m_pDeviceContext->m_pDeviceContext->OMSetBlendState(pBS->m_pBlendS, bf, 0xFFFFFFFF);
}

void
DX11GraphicsManager::internalSetRasterizerState(const WPtr<RasterizerState> pRasterizerState)
{
  if (pRasterizerState.expired()) {
    return;
  }
  auto pRS = sh_reinterpretPCast<DX11RasterizerState>(pRasterizerState.lock());
  m_pDeviceContext->m_pDeviceContext->RSSetState(pRS->m_pRasterS);
}

void
DX11GraphicsManager::internalSetDepthStencilState(const WPtr<DepthStencilState> pDepthSState,
                                                  const uint8 stencilRef)
{
  if (pDepthSState.expired()) {
    return;
  }
  auto pDepthSS = sh_reinterpretPCast<DX11DepthStencilState>(pDepthSState.lock());
  m_pDeviceContext->m_pDeviceContext->OMSetDepthStencilState(pDepthSS->m_pDepthSS,
                                                             stencilRef);
}

void
DX11GraphicsManager::internalSetScissorRects(const Rect& scissorClip)
{
  const D3D11_RECT r = { static_cast<LONG>(scissorClip.min.x),
                         static_cast<LONG>(scissorClip.min.y),
                         static_cast<LONG>(scissorClip.max.x),
                         static_cast<LONG>(scissorClip.max.y) };

  m_pDeviceContext->m_pDeviceContext->RSSetScissorRects(1, &r);
}

void
DX11GraphicsManager::internalDraw(const uint32 vertexCount, const uint32 startVertexLocation)
{
  m_pDeviceContext->m_pDeviceContext->Draw(vertexCount, startVertexLocation);
}

void
DX11GraphicsManager::internalDrawIndexed(const uint32 indexCount,
                                         const uint32 startIndexLocation,
                                         const uint32 baseVertexLocation)
{
  m_pDeviceContext->m_pDeviceContext->DrawIndexed(indexCount,
                                                  startIndexLocation,
                                                  baseVertexLocation);
}

void
DX11GraphicsManager::internalDispatch(const uint32 threadGroupCountX,
                                      const uint32 threadGroupCountY,
                                      const uint32 threadGroupCountZ)
{
  m_pDeviceContext->m_pDeviceContext->Dispatch(threadGroupCountX,
                                               threadGroupCountY,
                                               threadGroupCountZ);
}
}
