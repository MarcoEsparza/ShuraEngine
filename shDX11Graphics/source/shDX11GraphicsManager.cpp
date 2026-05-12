/*****************************************************************************/
/*
*  @file    shDX11GraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/18
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
#include <shLogger.h>
#include <shFileExplorer.h>

#include <shScreen.h>
#include <shLinearColor.h>
#include <shException.h>
#include "shMath.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3dcompiler.h>
#pragma comment(lib, "dxguid.lib")
#include "DDSTextureLoader11.h"

#if SH_DEBUG_MODE
#include <dxgidebug.h>
#endif

#define MAX_CONSTANT_BUFFER_SLOTS                             14
#define MAX_SHADER_RESOURCE_VIEW_SLOTS                        128
#define MAX_SAMPLERS                                          16
#define MAX_UNORDERED_ACCESS_VIEW_SLOTS                       7

using namespace DirectX;

namespace shEngineSDK {
class ShaderInclude : public ID3DInclude
{
 public:
  HRESULT __stdcall Open(D3D_INCLUDE_TYPE includeType, LPCSTR pFileName,  
                         LPCVOID lpcVoid, LPCVOID* ppData, UINT* pBytes) noexcept override {  
    SH_UNREFERENCED_PARAMETER(includeType);  
    SH_UNREFERENCED_PARAMETER(lpcVoid);  
    String workingDirectory;  
    workingDirectory.resize(MAX_PATH);  
    GetCurrentDirectoryA(MAX_PATH, &workingDirectory[0]);  
    workingDirectory.resize(strlen(workingDirectory.c_str()));  
    if (workingDirectory.back() != '\\' && workingDirectory.back() != '/') {  
      workingDirectory.append("\\");  
    }  
    workingDirectory.append(FileExplorer::SHADERS_FOLDER + "DX11/");  
    workingDirectory.append(pFileName);  

    std::ifstream file(workingDirectory.c_str(), std::ios::binary | std::ios::ate);  
    if (!file.is_open()) {  
      return E_FAIL;  
    }  

    size_t size = file.tellg();  
    file.seekg(0, std::ios::beg);  

    char* buffer = new char[size];  
    file.read(buffer, size);  

    *ppData = buffer;  
    *pBytes = static_cast<UINT>(size);  
    return S_OK;  
  }  

  HRESULT __stdcall Close(LPCVOID pData) noexcept override {  
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

  SystemPath path = fileName;
  path.replace_extension(".hlsl");
  String newFileName = path.string();
  auto beg = (String::const_iterator)newFileName.begin();
  auto end = (String::const_iterator)newFileName.end();
  WString wFileName(beg, end);

  static ShaderInclude shaderInclude;
  ID3DBlob* pErrorBlob = nullptr;

  Vector<D3D_SHADER_MACRO> d3dMacros;

  if (!macros.empty()) {
    for (auto& currentMacro : macros) {
      D3D_SHADER_MACRO d3dM = {};
      d3dM.Name = currentMacro.name.c_str();
      d3dM.Definition = currentMacro.definition.c_str();
      d3dMacros.push_back(d3dM);
    }
    d3dMacros.push_back({ nullptr, nullptr });

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
  unbindAll();

  m_pBackbuffer.reset();
  m_pDepthStencil.reset();
  SafeRelease(m_pSwapChain);

  ID3D11SamplerState* nullSamplers[16] = {};
  m_pDeviceContext->CSSetSamplers(0, 16, nullSamplers);
  m_pDeviceContext->PSSetSamplers(0, 16, nullSamplers);
  m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
  m_pDeviceContext->OMSetDepthStencilState(nullptr, 0);
  m_pDeviceContext->RSSetState(nullptr);

  m_pDeviceContext->ClearState();
  m_pDeviceContext->Flush();

#if defined(SH_DEBUG_MODE)
  m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
  SafeRelease(m_pDebug);
#endif

  SafeRelease(m_pDeviceContext);
  SafeRelease(m_pDevice);
}

void
DX11GraphicsManager::initManager(const WPtr<Screen> pScreen,
                                 const bool bAntiliasing,
                                 const SampleDesc& sample)
{
  if (pScreen.expired()) {
    SH_ASSERT(false && "Screen expired!");
    return;
  }
  auto screen = pScreen.lock();
  m_graphicAPI = GRAPHIC_API::kDX11;
  m_bFullScreen = screen->isFullscreen();
  
  auto hWnd = cast::re<HWND>(screen->getPlatformHandler());

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

  //Create a device and immediate device context
  throwIfFailed(D3D11CreateDevice(vecAdapters[0],
                                  D3D_DRIVER_TYPE_UNKNOWN,
                                  nullptr,
                                  deviceFlags,
                                  &featureLevels[0],
                                  static_cast<UINT>(featureLevels.size()),
                                  D3D11_SDK_VERSION,
                                  &m_pDevice,
                                  &selectedFeatureLevel,
                                  &m_pDeviceContext));

#if defined(SH_DEBUG_MODE)
  throwIfFailed(m_pDevice->QueryInterface(__uuidof(ID3D11Debug),
                                          cast::re<void**>(&m_pDebug)));
#endif

  //Create a swap chain
  DXGI_SWAP_CHAIN_DESC scDesc;
  memset(&scDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

  scDesc.OutputWindow = hWnd;
  scDesc.Windowed = !m_bFullScreen;

  if (!m_bFullScreen) {
    scDesc.BufferDesc.Width = cast::st<uint32>(screen->getClientSize().x);
    scDesc.BufferDesc.Height = cast::st<uint32>(screen->getClientSize().y);
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
  throwIfFailed(m_pDevice->QueryInterface(__uuidof(IDXGIDevice),
                                          reinterpret_cast<void**>(&dxgiDevice)));

  IDXGIAdapter* dxgiAdapter = nullptr;
  dxgiDevice->GetAdapter(&dxgiAdapter);

  IDXGIFactory* dxgiFactory;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
                         reinterpret_cast<void**>(&dxgiFactory));

  throwIfFailed(dxgiFactory->CreateSwapChain(m_pDevice,
                                             &scDesc,
                                             &m_pSwapChain));

  //Get Backbuffer Interface
  //Create a render target view

  auto pBackbuffer = sh_makeShared<DX11Texture2D>();

  throwIfFailed(m_pSwapChain->GetBuffer(0,
                              __uuidof(ID3D11Texture2D),
                              reinterpret_cast<LPVOID*>(&pBackbuffer->m_pTexture2D)));

  pBackbuffer->m_pRenderTV.resize(1);
  throwIfFailed(m_pDevice->CreateRenderTargetView(pBackbuffer->m_pTexture2D,
                                                  nullptr,
                                                  &pBackbuffer->m_pRenderTV[0]));

  m_pDepthStencil = createTexture2D(scDesc.BufferDesc.Width,
                                    scDesc.BufferDesc.Height,
                                    DXGI_FORMAT_D24_UNORM_S8_UINT,
                                    D3D11_USAGE_DEFAULT,
                                    D3D11_BIND_DEPTH_STENCIL,
                                    1);

  //Setup the viewport
  Viewport viewPort;
  viewPort.width = static_cast<float>(scDesc.BufferDesc.Width);
  viewPort.height = static_cast<float>(scDesc.BufferDesc.Height);
  viewPort.minDepth = 0.0f;
  viewPort.maxDepth = 1.0f;
  viewPort.topLeftX = 0.0f;
  viewPort.topLeftY = 0.0f;
  setViewport(viewPort);

  m_pBackbuffer = pBackbuffer;
  pBackbuffer.reset();

#if defined(SH_DEBUG_MODE)
  String DeviceName = "Main Device";
  m_pDevice->SetPrivateData(WKPDID_D3DDebugObjectName,
                            static_cast<uint32>(DeviceName.size()),
                            DeviceName.c_str());
  String DeviceCtxName = "Main Device Ctx";
  m_pDeviceContext->SetPrivateData(WKPDID_D3DDebugObjectName,
                                   static_cast<uint32>(DeviceCtxName.size()),
                                   DeviceCtxName.c_str());
  String SwapChainName = "My SwapChain";
  m_pSwapChain->SetPrivateData(WKPDID_D3DDebugObjectName,
                               static_cast<uint32>(SwapChainName.size()),
                               SwapChainName.c_str());
  m_pBackbuffer->setDebugName("My BackBuffer");
#endif

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
DX11GraphicsManager::clearRenderTarget(const WPtr<Texture2D> pTarget,
                                       const LinearColor& color)
{
  if (pTarget.expired()) {
    return;
  }
  auto pRTV = sh_reinterpretPCast<DX11Texture2D>(pTarget.lock());
  
  FLOAT colorRGBA[4] = { color.r, color.g, color.b, color.a };

  m_pDeviceContext->ClearRenderTargetView(pRTV->m_pRenderTV[0], colorRGBA);
}

void
DX11GraphicsManager::clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                                       const uint32 flags,
                                       const float depth,
                                       const uint8 stencil)
{
  if (pDepthSV.expired()) {
    return;
  }
  auto pDTV = sh_reinterpretPCast<DX11Texture2D>(pDepthSV.lock());
  
  m_pDeviceContext->ClearDepthStencilView(pDTV->m_pDepthSV, flags, depth, stencil);
}

void
DX11GraphicsManager::present(uint32 syncInterval, uint32 flags)
{
  SH_ASSERT(m_pSwapChain);
  m_pSwapChain->Present(syncInterval, flags);
}

void
DX11GraphicsManager::unbindAll()
{
  ID3D11ShaderResourceView* nullSRVs[MAX_SHADER_RESOURCE_VIEW_SLOTS] = {};
  m_pDeviceContext->VSSetShaderResources(0, MAX_SHADER_RESOURCE_VIEW_SLOTS, nullSRVs);
  m_pDeviceContext->PSSetShaderResources(0, MAX_SHADER_RESOURCE_VIEW_SLOTS, nullSRVs);
  m_pDeviceContext->CSSetShaderResources(0, MAX_SHADER_RESOURCE_VIEW_SLOTS, nullSRVs);
  ID3D11UnorderedAccessView* nullUAVs[MAX_UNORDERED_ACCESS_VIEW_SLOTS] = {};
  m_pDeviceContext->CSSetUnorderedAccessViews(0, MAX_UNORDERED_ACCESS_VIEW_SLOTS,
                                              nullUAVs, nullptr);
  ID3D11Buffer* nullCBs[MAX_CONSTANT_BUFFER_SLOTS] = {};
  m_pDeviceContext->VSSetConstantBuffers(0, MAX_CONSTANT_BUFFER_SLOTS, nullCBs);
  m_pDeviceContext->PSSetConstantBuffers(0, MAX_CONSTANT_BUFFER_SLOTS, nullCBs);
  m_pDeviceContext->CSSetConstantBuffers(0, MAX_CONSTANT_BUFFER_SLOTS, nullCBs);
  //ID3D11SamplerState* nullSamplers[MAX_SAMPLERS] = {};
  //m_pDeviceContext->VSSetSamplers(0, MAX_SAMPLERS, nullSamplers);
  //m_pDeviceContext->PSSetSamplers(0, MAX_SAMPLERS, nullSamplers);
  //m_pDeviceContext->CSSetSamplers(0, MAX_SAMPLERS, nullSamplers);
  //m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
  //m_pDeviceContext->OMSetDepthStencilState(nullptr, 0);
  //m_pDeviceContext->RSSetState(nullptr);
}

WPtr<Texture2D>
DX11GraphicsManager::getMainRenderTargetView() const
{
  return m_pBackbuffer;
}

WPtr<Texture2D>
DX11GraphicsManager::getMainDepthStencil() const
{
  return m_pDepthStencil;
}

SPtr<InputLayout>
DX11GraphicsManager::createInputLayout(const Vector<InputDesc>& desc,
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

  throwIfFailed(m_pDevice->CreateInputLayout(&dxInputDesc[0],
                           static_cast<UINT>(dxInputDesc.size()),
                           pVertexShader->m_pBlob->GetBufferPointer(),
                           pVertexShader->m_pBlob->GetBufferSize(),
                           &pInputLayout->m_pLayout));

  return pInputLayout;
}

SPtr<InputLayout>
DX11GraphicsManager::createInputLayoutFromShader(const WPtr<VertexShader> pPShader)
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

  for (uint32 i = 0; i < shaderDesc.InputParameters; ++i) {
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    pReflector->GetInputParameterDesc(i, &paramDesc);

    D3D11_INPUT_ELEMENT_DESC element;
    element.SemanticName = paramDesc.SemanticName;
    element.SemanticIndex = paramDesc.SemanticIndex;
    element.InputSlot = 0;
    element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
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
    }

    ilDesc.push_back(element);
  }

  throwIfFailed(m_pDevice->CreateInputLayout(&ilDesc[0],
                           static_cast<UINT>(ilDesc.size()),
                           pVertexShader->m_pBlob->GetBufferPointer(),
                           pVertexShader->m_pBlob->GetBufferSize(),
                           &pInputLayout->m_pLayout));

  SafeRelease(pReflector);

  return pInputLayout;
}

SPtr<VertexShader>
DX11GraphicsManager::createVertexShader(const String& fileName,
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

  throwIfFailed(m_pDevice->CreateVertexShader(pVertexShader->m_pBlob->GetBufferPointer(),
                                              pVertexShader->m_pBlob->GetBufferSize(),
                                              nullptr,
                                              &pVertexShader->m_pVertexShader));

  SystemPath path(fileName);
  path.extension() = ""; // Remove the extension for the name
  String pathName = path.filename().string() + "_" + entryPoint;
  pVertexShader->m_pVertexShader->SetPrivateData(WKPDID_D3DDebugObjectName,
                                                 static_cast<uint32>(pathName.size()),
                                                 pathName.c_str());

  return pVertexShader;
}

SPtr<PixelShader>
DX11GraphicsManager::createPixelShader(const String& fileName,
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

  throwIfFailed(m_pDevice->CreatePixelShader(pPixelShader->m_pBlob->GetBufferPointer(),
                                             pPixelShader->m_pBlob->GetBufferSize(),
                                             nullptr,
                                             &pPixelShader->m_pPixelShader));

  SystemPath path(fileName);
  path.extension() = ""; // Remove the extension for the name
  String pathName = path.filename().string() + "_" + entryPoint;
  pPixelShader->m_pPixelShader->SetPrivateData(WKPDID_D3DDebugObjectName,
                                               static_cast<uint32>(pathName.size()),
                                               pathName.c_str());

  return pPixelShader;
}

SPtr<GeometryShader>
DX11GraphicsManager::createGeometryShader(const String& fileName,
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

  throwIfFailed(m_pDevice->CreateGeometryShader(pGeometryShader->m_pBlob->GetBufferPointer(),
                                                pGeometryShader->m_pBlob->GetBufferSize(),
                                                nullptr,
                                                &pGeometryShader->m_pGeometryShader));

  SystemPath path(fileName);
  path.extension() = ""; // Remove the extension for the name
  String pathName = path.filename().string() + "_" + entryPoint;
  pGeometryShader->m_pGeometryShader->SetPrivateData(WKPDID_D3DDebugObjectName,
                                                     static_cast<uint32>(pathName.size()),
                                                     pathName.c_str());

  return pGeometryShader;
}

SPtr<ComputeShader>
DX11GraphicsManager::createComputeShader(const String& fileName,
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

  throwIfFailed(m_pDevice->CreateComputeShader(pComputeShader->m_pBlob->GetBufferPointer(),
                                               pComputeShader->m_pBlob->GetBufferSize(),
                                               nullptr,
                                               &pComputeShader->m_pComputeShader));

  SystemPath path(fileName);
  path.extension() = ""; // Remove the extension for the name
  String pathName = path.filename().string() + "_" + entryPoint;
  pComputeShader->m_pComputeShader->SetPrivateData(WKPDID_D3DDebugObjectName,
                                                   static_cast<uint32>(pathName.size()),
                                                   pathName.c_str());

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
  desc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = pData;
  initData.SysMemPitch = bufferSize;
  initData.SysMemSlicePitch = 0;

  m_pDevice->CreateBuffer(&desc, &initData, &pVBuffer->m_pBuffer);
  pVBuffer->m_stride = stride;

  return pVBuffer;
}

SPtr<IndexBuffer>
DX11GraphicsManager::createIndexBuffer(const Vector<uint32>& indices, const uint32 usage)
{
  auto pIBuffer = std::make_shared<DX11IndexBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(uint32));
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  desc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = &indices[0];
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  m_pDevice->CreateBuffer(&desc, &initData, &pIBuffer->m_pBuffer);
  pIBuffer->m_dataFormat = DXGI_FORMAT_R32_UINT;

  return pIBuffer;
}

SPtr<ConstantBuffer>
DX11GraphicsManager::createConstantBuffer(const uint32 bufferSize,
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

  throwIfFailed(m_pDevice->CreateBuffer(&desc, pData ? &initData : nullptr,
                                        &pCBuffer->m_pBuffer));

  return pCBuffer;
}

SPtr<SamplerState>
DX11GraphicsManager::createSamplerState(const uint32 filter, const uint32 textAddress)
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

  throwIfFailed(m_pDevice->CreateSamplerState(&sampDesc, &pSampleLinear->m_pSamplerLinear));

  return pSampleLinear;
}

SPtr<Texture2D>
DX11GraphicsManager::createTextureFromFile(const String& fileName,
                                           const void* pData,
                                           const uint32 width,
                                           const uint32 height,
                                           const uint32 bpp)
{
  if (pData == nullptr) {
    return nullptr;
  }

  int32 pitch = width * bpp;

  SystemPath path(fileName);
  uint32 format = 0;

  uint8* pDst = new uint8[width * height * 4]; // Allocate memory for RGBA format
  memcpy(cast::ct<void*>(pData), &pDst, sizeof(pDst));

  //if(path.extension() == ".dds") {
  //  // If the file is a DDS, we will use the DDS loader
  //  return createTextureFromDDS(fileName);
  //}
  if (path.extension() == ".hdr") {
    format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    pitch = width * 16;
  }
  else {
    if (bpp == 3) {
      const uint8* pSrc = cast::re<const uint8*>(pData);
      for (uint32 i = 0; i < width * height; ++i) {
        pDst[i * 4 + 0] = pSrc[i * 3 + 0]; // R
        pDst[i * 4 + 1] = pSrc[i * 3 + 1]; // G
        pDst[i * 4 + 2] = pSrc[i * 3 + 2]; // B
        pDst[i * 4 + 3] = 255;     // A
      }
      //pData = data.data();
      //memcpy(cast::ct<void*>(pData), data.data(), data.size());
      pitch = width * 4; // Update pitch for RGBA format
    }
    format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;
  }
  auto pTexture = cast::re_ptr<DX11Texture2D>(createTexture2D(width,
                                                              height,
                                                              format,
                                                              D3D11_USAGE_DEFAULT,
                                                              D3D11_BIND_SHADER_RESOURCE,
                                                              1));
  
  if (path.extension() == ".hdr") {
    m_pDeviceContext->UpdateSubresource(pTexture->m_pTexture2D, 0, nullptr, pData, pitch, 0);
  }
  else {
    m_pDeviceContext->UpdateSubresource(pTexture->m_pTexture2D, 0, nullptr, pData, pitch, 0);
  }
  
  String texName = "t_" + fileName;
  String ShaderRes = "sr_" + fileName;

  pTexture->m_pTexture2D->SetPrivateData(WKPDID_D3DDebugObjectName,
                                         static_cast<uint32>(texName.size()),
                                         texName.c_str());
  pTexture->m_pShaderRV->SetPrivateData(WKPDID_D3DDebugObjectName,
                                        static_cast<uint32>(ShaderRes.size()),
                                        ShaderRes.c_str());

  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::createTextureFromDDS(const String& fileName)
{
  auto pTexture = sh_makeShared<DX11Texture2D>();

  SystemPath path = fileName;
  
  HRESULT hr =  CreateDDSTextureFromFile(m_pDevice,
                path.wstring().c_str(),
                reinterpret_cast<ID3D11Resource**>(&pTexture->m_pTexture2D),
                &pTexture->m_pShaderRV);

  if(hr != S_OK) {
    return nullptr;
  }

  String texName = "t_" + fileName;
  String ShaderRes = "sr_" + fileName;

  pTexture->m_pTexture2D->SetPrivateData(WKPDID_D3DDebugObjectName,
                                         static_cast<uint32>(texName.size()),
                                         texName.c_str());
  pTexture->m_pShaderRV->SetPrivateData(WKPDID_D3DDebugObjectName,
                                        static_cast<uint32>(ShaderRes.size()),
                                        ShaderRes.c_str());

  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::createTexture2D(const uint32 width,
                                     const uint32 height,
                                     const uint32 format,
                                     const uint32 usage,
                                     const uint32 bindFlags,
                                     const uint32 mipLevels,
                                     const uint32 arraySize)
{
  auto pTexture = sh_makeShared<DX11Texture2D>();
  uint32 texMipLevels = mipLevels;

  D3D11_TEXTURE2D_DESC textureDesc;
  memset(&textureDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = texMipLevels;
  textureDesc.ArraySize = arraySize;
  textureDesc.Format = static_cast<DXGI_FORMAT>(format);
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
  textureDesc.BindFlags = bindFlags;
  textureDesc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ?  D3D11_CPU_ACCESS_WRITE : 0;
  textureDesc.MiscFlags = 0;

  if (arraySize > 1) {
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  }

  bool autoGenMipMaps = false;
  if (texMipLevels != 1 && usage != D3D11_USAGE_STAGING) {
    // Check if the format supports mipmaps
    uint32 fmtSupport = 0;
    HRESULT hr = m_pDevice->CheckFormatSupport(static_cast<DXGI_FORMAT>(format), &fmtSupport);
    if(SUCCEEDED(hr) && (fmtSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN)) {
      // If the format supports mipmaps, we need to make sure it is binded as render target
      textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
      textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
      if(texMipLevels == 0) {
        // If mipLevels is 0, we will auto-generate mipmaps
        texMipLevels = static_cast<uint32>(Math::log2(Math::max(static_cast<float>(width),
                                                      static_cast<float>(height)))) + 1;
        autoGenMipMaps = true;
      }
    }
  }

  throwIfFailed(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture->m_pTexture2D));

  if ((bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET)
  {
    D3D11_RENDER_TARGET_VIEW_DESC descRTV;
    memset(&descRTV, 0, sizeof(descRTV));
    descRTV.Format = textureDesc.Format;
    descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    pTexture->m_pRenderTV.resize(texMipLevels);
    for (uint32 i = 0; i < texMipLevels; ++i) {
      descRTV.Texture2D.MipSlice = i;
      throwIfFailed(m_pDevice->CreateRenderTargetView(pTexture->m_pTexture2D,
                                                      &descRTV,
                                                      &pTexture->m_pRenderTV[i]));
    }
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
    throwIfFailed(m_pDevice->CreateDepthStencilView(pTexture->m_pTexture2D,
                                                    &descDSV,
                                                    &pTexture->m_pDepthSV));
  }

  if ((bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS)
  {
    D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
    memset(&descUAV, 0, sizeof(descUAV));
    descUAV.Format = textureDesc.Format;
    descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

    if(arraySize > 1) {
      descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
      descUAV.Texture2DArray.ArraySize = arraySize;
      descUAV.Texture2DArray.FirstArraySlice = 0;
    }

    pTexture->m_pUnorderedAV.resize(texMipLevels);
    for (uint32 i = 0; i < texMipLevels; ++i) {
      descUAV.Texture2D.MipSlice = i;
      throwIfFailed(m_pDevice->CreateUnorderedAccessView(pTexture->m_pTexture2D,
                                                         &descUAV,
                                                         &pTexture->m_pUnorderedAV[i]));
    }
  }

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
    shaderRVDesc.Texture2D.MostDetailedMip = 0;
    shaderRVDesc.Texture2D.MipLevels = texMipLevels == 1 ? 1 : -1; // -1 means all mip levels
    throwIfFailed(m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                                                      &shaderRVDesc,
                                                      &pTexture->m_pShaderRV));
  }

  if(autoGenMipMaps) {
    generateMips(pTexture);
  }

  return pTexture;
}

SPtr<Texture3D>
DX11GraphicsManager::createTexture3D(const Vector3 size,
                                     const uint32 format,
                                     const Vector<LinearColor>* data,
                                     const uint32 usage)
{
  auto pTexture = std::make_shared<DX11Texture3D>();

  D3D11_TEXTURE3D_DESC textureDesc;
  memset(&textureDesc, 0, sizeof(textureDesc));
  textureDesc.Width = static_cast<uint32>(size.x);
  textureDesc.Height = static_cast<uint32>(size.y);
  textureDesc.Depth = static_cast<uint32>(size.z);
  textureDesc.MipLevels = 1;
  textureDesc.Format = static_cast<DXGI_FORMAT>(format);
  textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
  textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  textureDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData = {};
  if (data) {
    initData.pSysMem = data->data();
    initData.SysMemPitch = cast::st<UINT>(size.x * sizeof(LinearColor));
    initData.SysMemSlicePitch = cast::st<UINT>(size.y * initData.SysMemPitch);
  }

  throwIfFailed(m_pDevice->CreateTexture3D(&textureDesc,
                                           data ? &initData : nullptr,
                                           &pTexture->m_pTexture3D));

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
  srvDesc.Texture3D.MipLevels = 1;

  throwIfFailed(m_pDevice->CreateShaderResourceView(pTexture->m_pTexture3D,
                                                    &srvDesc,
                                                    &pTexture->m_pShaderRV));

  return pTexture;
}

//SPtr<Texture2D>
//DX11GraphicsManager::createTexture2DArray(const uint32 width,
//                                          const uint32 height,
//                                          const uint32 arraySize,
//                                          const uint32 format,
//                                          const uint32 usage)
//{
//  auto pTexture = std::make_shared<DX11Texture2D>();
//
//  D3D11_TEXTURE2D_DESC textureDesc;
//  memset(&textureDesc, 0, sizeof(textureDesc));
//  textureDesc.Width = width;
//  textureDesc.Height = height;
//  textureDesc.MipLevels = 1;
//  textureDesc.ArraySize = arraySize;
//  textureDesc.Format = static_cast<DXGI_FORMAT>(format);
//  textureDesc.SampleDesc.Count = 1;
//  textureDesc.SampleDesc.Quality = 0;
//  textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
//  textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
//  textureDesc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
//  textureDesc.MiscFlags = 0;
//
//  throwIfFailed(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture->m_pTexture2D));
//
//  D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV = {};
//  descUAV.Format = textureDesc.Format;
//  descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
//  descUAV.Texture2DArray.ArraySize = arraySize;
//  descUAV.Texture2DArray.FirstArraySlice = 0;
//  descUAV.Texture2DArray.MipSlice = 0;
//
//  throwIfFailed(m_pDevice->CreateUnorderedAccessView(pTexture->m_pTexture2D,
//                                                     &descUAV,
//                                                     &pTexture->m_pUnorderedAV[0]));
//
//  return pTexture;
//}

SPtr<Texture2D>
DX11GraphicsManager::createErrorTexture()
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

  throwIfFailed(m_pDevice->CreateTexture2D(&desc, &initData, &pTexture->m_pTexture2D));

  throwIfFailed(m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                                                    nullptr,
                                                    &pTexture->m_pShaderRV));

  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::createDefaultNormalTexture()
{
  auto pTexture = sh_makeShared<DX11Texture2D>();
  uint32 normalSize = 128;
  Vector<uint32> pixels;
  //Vector<LinearColor> pixels;
  pixels.resize(normalSize * normalSize);
  for (uint32 y = 0; y < normalSize; ++y) {
    for (uint32 x = 0; x < normalSize; ++x) {
      //float u = cast::st<float>(x / (normalSize - 1));
      //float v = cast::st<float>(y / (normalSize - 1));
      // Generate a simple blue normal map
      pixels[y * normalSize + x] = 0xFFFF8080; // RGB: (128, 128, 255)
    }
  }

  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = normalSize;
  desc.Height = normalSize;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  D3D11_SUBRESOURCE_DATA initData = {};
  initData.pSysMem = pixels.data();
  initData.SysMemPitch = normalSize * sizeof(uint32);

  throwIfFailed(m_pDevice->CreateTexture2D(&desc, &initData, &pTexture->m_pTexture2D));

  throwIfFailed(m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                                                    nullptr,
                                                    &pTexture->m_pShaderRV));

  return pTexture;
}

SPtr<Texture2D> DX11GraphicsManager::createBlackTexture()
{
  auto pTexture = sh_makeShared<DX11Texture2D>();
  uint32 size = 8;
  Vector<uint32> pixels;
  //Vector<LinearColor> pixels;
  pixels.resize(size * size);
  for (uint32 y = 0; y < size; ++y) {
    for (uint32 x = 0; x < size; ++x) {
      //float u = cast::st<float>(x / (size - 1));
      //float v = cast::st<float>(y / (size - 1));
      // Generate a simple blue normal map
      pixels[y * size + x] = 0xFF000000; // RGB: (128, 128, 255)
    }
  }

  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = size;
  desc.Height = size;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  D3D11_SUBRESOURCE_DATA initData = {};
  initData.pSysMem = pixels.data();
  initData.SysMemPitch = size * sizeof(uint32);

  throwIfFailed(m_pDevice->CreateTexture2D(&desc, &initData, &pTexture->m_pTexture2D));

  throwIfFailed(m_pDevice->CreateShaderResourceView(pTexture->m_pTexture2D,
                nullptr,
                &pTexture->m_pShaderRV));

  return pTexture;
}

SPtr<BlendState>
DX11GraphicsManager::createBlendState(const BlendDesc& blendDesc,
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

  m_pDevice->CreateBlendState(&d3d11BlendDesc, &pBlendState->m_pBlendS);

  pBlendState->m_blendFactor = blendFactor;

  return pBlendState;
}

SPtr<RasterizerState>
DX11GraphicsManager::createRasterizerState(const RasterizerDesc& rasterizerDesc)
{
  auto pRasterizerState = sh_makeShared<DX11RasterizerState>();

  D3D11_RASTERIZER_DESC rasterDesc = {};
  rasterDesc.FillMode = cast::st<D3D11_FILL_MODE>(rasterizerDesc.fillMode);
  rasterDesc.CullMode = cast::st<D3D11_CULL_MODE>(rasterizerDesc.cullMode);
  rasterDesc.FrontCounterClockwise = rasterizerDesc.frontCounterClockwise;
  rasterDesc.DepthBias = rasterizerDesc.depthBias;
  rasterDesc.DepthBiasClamp = rasterizerDesc.depthBiasClamp;
  rasterDesc.SlopeScaledDepthBias = rasterizerDesc.slopeScaledDepthBias;
  rasterDesc.DepthClipEnable = rasterizerDesc.depthClipEnable;
  rasterDesc.ScissorEnable = rasterizerDesc.scissorEnable;
  rasterDesc.MultisampleEnable = rasterizerDesc.multisampleEnable;
  rasterDesc.AntialiasedLineEnable = rasterizerDesc.antialiasedLineEnable;

  m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState->m_pRasterS);

  return pRasterizerState;
}

SPtr<DepthStencilState>
DX11GraphicsManager::createDepthStencilState(const DepthStencilDesc& depthSDesc)
{
  auto pDepthSS = sh_makeShared<DX11DepthStencilState>();

  D3D11_DEPTH_STENCIL_DESC d3d11DepthDesc = {};
  d3d11DepthDesc.DepthEnable = depthSDesc.depthEnable;
  d3d11DepthDesc.DepthWriteMask = cast::st<D3D11_DEPTH_WRITE_MASK>(depthSDesc.depthWriteMask);
  d3d11DepthDesc.DepthFunc = cast::st<D3D11_COMPARISON_FUNC>(depthSDesc.depthFunc);
  d3d11DepthDesc.StencilEnable = depthSDesc.stencilEnable;
  d3d11DepthDesc.StencilReadMask = depthSDesc.stencilReadMask;
  d3d11DepthDesc.StencilWriteMask = depthSDesc.stencilWriteMask;

  D3D11_DEPTH_STENCILOP_DESC frontFace = {};
  frontFace.StencilFailOp = cast::st<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilFailOp);
  frontFace.StencilDepthFailOp =
    cast::st<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilDepthFailOp);
  frontFace.StencilPassOp = cast::st<D3D11_STENCIL_OP>(depthSDesc.frontFace.stencilPassOp);
  frontFace.StencilFunc = cast::st<D3D11_COMPARISON_FUNC>(depthSDesc.frontFace.stencilFunc);

  D3D11_DEPTH_STENCILOP_DESC backFace = {};
  backFace.StencilFailOp = cast::st<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilFailOp);
  backFace.StencilDepthFailOp =
    cast::st<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilDepthFailOp);
  backFace.StencilPassOp = cast::st<D3D11_STENCIL_OP>(depthSDesc.backFace.stencilPassOp);
  backFace.StencilFunc = cast::st<D3D11_COMPARISON_FUNC>(depthSDesc.backFace.stencilFunc);

  d3d11DepthDesc.FrontFace = frontFace;
  d3d11DepthDesc.BackFace = backFace;

  m_pDevice->CreateDepthStencilState(&d3d11DepthDesc, &pDepthSS->m_pDepthSS);

  return pDepthSS;
}

void
DX11GraphicsManager::generateMips(const WPtr<Texture2D> pTexture)
{
  if (pTexture.expired())
  {
    return;
  }
  auto pTex = sh_reinterpretPCast<DX11Texture2D>(pTexture.lock());

  if (pTex->m_pShaderRV) {
    m_pDeviceContext->GenerateMips(pTex->m_pShaderRV);
  }
}

void
DX11GraphicsManager::updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                                          const void* pData,
                                          const uint32 dataSize)
{
  if (pCBuffer.expired()) {
    return;
  }
  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->UpdateSubresource(pConstantBuffer->m_pBuffer,
                                      0,
                                      nullptr,
                                      pData,
                                      dataSize,
                                      0);
}

void
DX11GraphicsManager::updateTexture2D(WPtr<Texture2D> pTexture,
                                     uint8* pData,
                                     uint32 width,
                                     uint32 bpp)
{
  if (pTexture.expired() || pData == nullptr) {
    return;
  }
  auto pTex2D = sh_reinterpretPCast<DX11Texture2D>(pTexture.lock());
  int32 pitch = width * bpp;

  m_pDeviceContext->UpdateSubresource(pTex2D->m_pTexture2D, 0, nullptr, pData, pitch, 0);
}

void
DX11GraphicsManager::updateScreenSize(const Vector2& size)
{
  uint32 width = static_cast<uint32>(size.x);
  uint32 height = static_cast<uint32>(size.y);

  if (m_pDepthStencil) {
    m_pDepthStencil = nullptr;
  }
  if (m_pBackbuffer) {
    m_pBackbuffer = nullptr;
  }

  m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  auto pBackbuffer = sh_makeShared<DX11Texture2D>();
  throwIfFailed(m_pSwapChain->GetBuffer(0,
                __uuidof(ID3D11Texture2D),
                reinterpret_cast<LPVOID*>(&pBackbuffer->m_pTexture2D)));

  pBackbuffer->m_pRenderTV.resize(1);
  throwIfFailed(m_pDevice->CreateRenderTargetView(pBackbuffer->m_pTexture2D,
                                                  nullptr,
                                                  &pBackbuffer->m_pRenderTV[0]));
  m_pBackbuffer = pBackbuffer;

  m_pDepthStencil = createTexture2D(width,
                                    height,
                                    DXGI_FORMAT_D24_UNORM_S8_UINT,
                                    D3D11_USAGE_DEFAULT,
                                    D3D11_BIND_DEPTH_STENCIL,
                                    1);

  //Setup the viewport
  Viewport viewPort;
  viewPort.width = static_cast<float>(width);
  viewPort.height = static_cast<float>(height);
  viewPort.minDepth = 0.0f;
  viewPort.maxDepth = 1.0f;
  viewPort.topLeftX = 0.0f;
  viewPort.topLeftY = 0.0f;
  setViewport(viewPort);
}

void
DX11GraphicsManager::updateVertexBuffer(const WPtr<VertexBuffer> pVBuffer,
                                        const void* pData,
                                        const uint32 dataSize)
{
  if (pVBuffer.expired() || pData == nullptr) {
    return;
  }
  auto pVertexBuffer = cast::re_ptr<DX11VertexBuffer>(pVBuffer.lock());
  /*m_pDeviceContext->UpdateSubresource(pVertexBuffer->m_pBuffer,
                                      0,
                                      nullptr,
                                      pData,
                                      0,
                                      0);*/

  D3D11_MAPPED_SUBRESOURCE mappedResource;
  throwIfFailed(m_pDeviceContext->Map(pVertexBuffer->m_pBuffer,
                                      0,
                                      D3D11_MAP_WRITE_DISCARD,
                                      0,
                                      &mappedResource));

  memcpy(mappedResource.pData, pData, dataSize * pVertexBuffer->m_stride);
  m_pDeviceContext->Unmap(pVertexBuffer->m_pBuffer, 0);
}

void
DX11GraphicsManager::updateIndexBuffer(const WPtr<IndexBuffer> pIBuffer,
                                       const void* pData,
                                       const uint32 dataSize)
{
  if (pIBuffer.expired() || pData == nullptr) {
    return;
  }
  auto pIndexBuffer = sh_reinterpretPCast<DX11IndexBuffer>(pIBuffer.lock());
  /*m_pDeviceContext->UpdateSubresource(pIndexBuffer->m_pBuffer,
                                      0,
                                      nullptr,
                                      pData,
                                      0,
                                      0);*/

  D3D11_MAPPED_SUBRESOURCE mappedResource = {};
  throwIfFailed(m_pDeviceContext->Map(pIndexBuffer->m_pBuffer,
                                      0,
                                      D3D11_MAP_WRITE_DISCARD,
                                      0,
                                      &mappedResource));
  memcpy(mappedResource.pData, pData, dataSize);
  m_pDeviceContext->Unmap(pIndexBuffer->m_pBuffer, 0);
}

void
DX11GraphicsManager::saveTextureToDDS(const WPtr<Texture2D> pTexture,
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
DX11GraphicsManager::setViewport(const Viewport& vp)
{
  D3D11_VIEWPORT viewPort;
  viewPort.Width = vp.width;
  viewPort.Height = vp.height;
  viewPort.MinDepth = vp.minDepth;
  viewPort.MaxDepth = vp.maxDepth;
  viewPort.TopLeftX = vp.topLeftX;
  viewPort.TopLeftY = vp.topLeftY;
  m_pDeviceContext->RSSetViewports(1, &viewPort);
}

void
DX11GraphicsManager::setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                                      const WPtr<Texture2D> pDepthSV)
{
  Vector<ID3D11RenderTargetView*> pRTVs;

  for (auto& pRenderTarget : pRenderTVs) {
    ID3D11RenderTargetView* pD3D11RTV = nullptr;

    if (!pRenderTarget.pRenderTarget.expired()) {
      auto pRT = pRenderTarget.pRenderTarget.lock();
      pD3D11RTV = reinterpret_cast<DX11Texture2D*>(pRT.get())->m_pRenderTV[pRenderTarget.mipLevel];
    }
    
    pRTVs.push_back(pD3D11RTV);
  }

  for (uint32 i = static_cast<uint32>(pRTVs.size()); i < 8; ++i) {
    pRTVs.push_back(nullptr);
  }

  ID3D11DepthStencilView* pDSV = nullptr;
  if (!pDepthSV.expired()) {
    auto pDepthStencil = sh_reinterpretPCast<DX11Texture2D>(pDepthSV.lock());
    pDSV = pDepthStencil->m_pDepthSV;
  }

  m_pDeviceContext->OMSetRenderTargets(static_cast<UINT>(pRTVs.size()), pRTVs.data(), pDSV);
}

void
DX11GraphicsManager::setInputLayout(const WPtr<InputLayout> pInput)
{
  if (pInput.expired()) {
    return;
  }
  auto pInputLayout = sh_reinterpretPCast<DX11InputLayout>(pInput.lock());
  m_pDeviceContext->IASetInputLayout(pInputLayout->m_pLayout);
}

void
DX11GraphicsManager::setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                                              const uint32 startSlot,
                                              const uint32 numBuffers,
                                              const uint32 offset)
{
  SH_ASSERT(m_pDeviceContext);

  if (!pVBuffer.expired()) {
    auto pVertexBuffer = sh_reinterpretPCast<DX11VertexBuffer>(pVBuffer.lock());

    m_pDeviceContext->IASetVertexBuffers(startSlot,
                                         numBuffers,
                                         &pVertexBuffer->m_pBuffer,
                                         &pVertexBuffer->m_stride,
                                         &offset);
  }
  else {
    ID3D11Buffer* pVB = nullptr;
    m_pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, &pVB, 0, &offset);
  }
}

void
DX11GraphicsManager::setIndexBuffers(const WPtr<IndexBuffer> pIBuffer,
                                             const uint32 offset)
{
  SH_ASSERT(m_pDeviceContext);

  if(!pIBuffer.expired()){
    auto pIndexBuffer = sh_reinterpretPCast<DX11IndexBuffer>(pIBuffer.lock());
    
    m_pDeviceContext->IASetIndexBuffer(pIndexBuffer->m_pBuffer,
                                       static_cast<DXGI_FORMAT>(pIndexBuffer->m_dataFormat),
                                       offset);
  }
  else {
    ID3D11Buffer* pIB = nullptr;
    m_pDeviceContext->IASetIndexBuffer(pIB, DXGI_FORMAT_UNKNOWN, offset);
  }
}

void
DX11GraphicsManager::vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  if (pCBuffer.expired()) {
    ID3D11Buffer* pBuff = nullptr;

    m_pDeviceContext->CSSetConstantBuffers(startSlot, numBuffers, &pBuff);

    SafeRelease(pBuff);
    return;
  }

  auto pConstantBuffer = sh_reinterpretPCast<DX11ConstantBuffer>(pCBuffer.lock());

  m_pDeviceContext->CSSetConstantBuffers(startSlot, numBuffers, &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::setPrimitiveTopology(uint32 primitive)
{
  m_pDeviceContext->IASetPrimitiveTopology(cast::st<D3D_PRIMITIVE_TOPOLOGY>(primitive));
}

void
DX11GraphicsManager::setVertexShader(const WPtr<VertexShader> pVShader,
                                             const void* ppClassInstances,
                                             const uint32 numClassInstances)
{
  if (pVShader.expired()) {
    return;
  }
  auto pVS = sh_reinterpretPCast<DX11VertexShader>(pVShader.lock());

  m_pDeviceContext->VSSetShader(pVS->m_pVertexShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::setPixelShader(const WPtr<PixelShader> pPShader,
                                            const void* ppClassInstances,
                                            const uint32 numClassInstances)
{
  if (pPShader.expired()) {
    return;
  }
  auto pPS = sh_reinterpretPCast<DX11PixelShader>(pPShader.lock());

  m_pDeviceContext->PSSetShader(pPS->m_pPixelShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::setGeometryShader(const WPtr<GeometryShader> pGShader,
                                               const void* ppClassInstances,
                                               const uint32 numClassInstances)
{
  if (pGShader.expired()) {
    return;
  }
  auto pGS = sh_reinterpretPCast<DX11GeometryShader>(pGShader.lock());

  m_pDeviceContext->GSSetShader(pGS->m_pGeometryShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::setComputeShader(const WPtr<ComputeShader> pCShader,
                                              const void* ppClassInstances,
                                              const uint32 numClassInstances)
{
  if (pCShader.expired()) {
    return;
  }
  auto pCS = sh_reinterpretPCast<DX11ComputeShader>(pCShader.lock());

  m_pDeviceContext->CSSetShader(pCS->m_pComputeShader,
                    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
                    numClassInstances);
}

void
DX11GraphicsManager::psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                                     const uint32 startSlot,
                                                     const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->PSSetShaderResources(startSlot, numViews, &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture2D>(pShaderRV.lock());

  m_pDeviceContext->PSSetShaderResources(startSlot, numViews, &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                                     const uint32 startSlot,
                                                     const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->CSSetShaderResources(startSlot, numViews, &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture2D>(pShaderRV.lock());

  m_pDeviceContext->CSSetShaderResources(startSlot, numViews, &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::psSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                                             const uint32 startSlot,
                                             const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->PSSetShaderResources(startSlot, numViews, &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture3D>(pShaderRV.lock());

  m_pDeviceContext->PSSetShaderResources(startSlot, numViews, &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::csSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                                             const uint32 startSlot,
                                             const uint32 numViews)
{
  if (pShaderRV.expired()) {
    ID3D11ShaderResourceView* dx11SRV = nullptr;
    m_pDeviceContext->CSSetShaderResources(startSlot, numViews, &dx11SRV);
    return;
  }

  auto pShaderTexture = sh_reinterpretPCast<DX11Texture3D>(pShaderRV.lock());

  m_pDeviceContext->CSSetShaderResources(startSlot, numViews, &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::setUnorderedAccessView(const UnorderedAccess& pUAVs,
                                                    const uint32 startSlot)
{
  if (pUAVs.pUAccess.expired()) {
    ID3D11UnorderedAccessView* dx11UAV = nullptr;
    m_pDeviceContext->CSSetUnorderedAccessViews(startSlot, 1, &dx11UAV, nullptr);
    return;
  }

  auto pUAVTexture = sh_reinterpretPCast<DX11Texture2D>(pUAVs.pUAccess.lock());
  m_pDeviceContext->CSSetUnorderedAccessViews(startSlot,
                                              1,
                                              &pUAVTexture->m_pUnorderedAV[pUAVs.mipLevel],
                                              nullptr);
}

void
DX11GraphicsManager::psSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                               const uint32 startSlot,
                                               const uint32 numSamplers)
{
  if (!pSamplerLinear.expired()) {
    auto pSampler = sh_reinterpretPCast<DX11SamplerState>(pSamplerLinear.lock());

    m_pDeviceContext->PSSetSamplers(startSlot, numSamplers, &pSampler->m_pSamplerLinear);
  }
  else {
    ID3D11SamplerState* pSampler = nullptr;
    m_pDeviceContext->PSSetSamplers(0, 1, &pSampler);
  }
}

void
DX11GraphicsManager::csSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                               const uint32 startSlot,
                                               const uint32 numSamplers)
{
  if (!pSamplerLinear.expired()) {
    auto pSampler = sh_reinterpretPCast<DX11SamplerState>(pSamplerLinear.lock());

    m_pDeviceContext->CSSetSamplers(startSlot, numSamplers, &pSampler->m_pSamplerLinear);
  }
  else {
    ID3D11SamplerState* pSampler = nullptr;
    m_pDeviceContext->CSSetSamplers(0, 1, &pSampler);
  }
}

void
DX11GraphicsManager::setBlendState(const WPtr<BlendState> pBlendState)
{
  if (pBlendState.expired()) {
    return;
  }
  auto pBS = sh_reinterpretPCast<DX11BlendState>(pBlendState.lock());
  FLOAT bf[4] = { pBS->m_blendFactor.r,
                  pBS->m_blendFactor.g,
                  pBS->m_blendFactor.b,
                  pBS->m_blendFactor.a };
  m_pDeviceContext->OMSetBlendState(pBS->m_pBlendS, bf, 0xFFFFFFFF);
}

void
DX11GraphicsManager::setRasterizerState(const WPtr<RasterizerState> pRasterizerState)
{
  if (pRasterizerState.expired()) {
    return;
  }
  auto pRS = sh_reinterpretPCast<DX11RasterizerState>(pRasterizerState.lock());
  m_pDeviceContext->RSSetState(pRS->m_pRasterS);
}

void
DX11GraphicsManager::setDepthStencilState(const WPtr<DepthStencilState> pDepthSState,
                                                  const uint8 stencilRef)
{
  if (pDepthSState.expired()) {
    return;
  }
  auto pDepthSS = sh_reinterpretPCast<DX11DepthStencilState>(pDepthSState.lock());
  m_pDeviceContext->OMSetDepthStencilState(pDepthSS->m_pDepthSS, stencilRef);
}

void
DX11GraphicsManager::setScissorRects(const Rect& scissorClip)
{
  const D3D11_RECT r = { static_cast<LONG>(scissorClip.min.x),
                         static_cast<LONG>(scissorClip.min.y),
                         static_cast<LONG>(scissorClip.max.x),
                         static_cast<LONG>(scissorClip.max.y) };

  m_pDeviceContext->RSSetScissorRects(1, &r);
}

void
DX11GraphicsManager::draw(const uint32 vertexCount, const uint32 startVertexLocation)
{
  m_pDeviceContext->Draw(vertexCount, startVertexLocation);
}

void
DX11GraphicsManager::drawIndexed(const uint32 indexCount,
                                         const uint32 startIndexLocation,
                                         const uint32 baseVertexLocation)
{
  m_pDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void
DX11GraphicsManager::dispatch(const uint32 threadGroupCountX,
                                      const uint32 threadGroupCountY,
                                      const uint32 threadGroupCountZ)
{
  m_pDeviceContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}
}

#endif
