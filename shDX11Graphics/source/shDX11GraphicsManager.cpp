/*****************************************************************************/
/*
*  @file    shDX11GraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/07
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
#include "shScreen.h"
#include "shLinearColor.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3dcompiler.h>

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
FORCEINLINE void
throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    SH_ASSERT(false && "Something went horribly wrong!!!");
  }
}

bool
compileShaderFromFile(const String& fileName,
                      const String& vsEntryPoint,
                      const String& psEntryPoint,
                      const String& vsShaderModel,
                      const String& psShaderModel,
                      ID3DBlob** pVertexBlob,
                      ID3DBlob** pPixelBlob)
{
  HRESULT hrVS = S_OK;
  HRESULT hrPS = S_OK;
  int32 shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if SH_DEBUG_MODE
  shaderFlags |= D3DCOMPILE_DEBUG;
#endif

  auto beg = (String::const_iterator)fileName.begin();
  auto end = (String::const_iterator)fileName.end();
  WString wFileName(beg, end);

  ID3DBlob* pErrorBlob = nullptr;

  hrVS = D3DCompileFromFile(wFileName.c_str(),
                            nullptr,
                            nullptr,
                            vsEntryPoint.c_str() ,
                            vsShaderModel.c_str(),
                            shaderFlags,
                            0,
                            pVertexBlob,
                            &pErrorBlob);

  if (FAILED(hrVS)) {
    if (nullptr != pErrorBlob) {
      String errStr(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
      SafeRelease(pErrorBlob);
    }

    return false;
  }

  SafeRelease(pErrorBlob);

  hrPS = D3DCompileFromFile(wFileName.c_str(),
                            nullptr,
                            nullptr,
                            psEntryPoint.c_str(),
                            psShaderModel.c_str(),
                            shaderFlags,
                            0,
                            pPixelBlob,
                            &pErrorBlob);

  if (FAILED(hrPS)) {
    if (nullptr != pErrorBlob) {
      String errStr(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
      SafeRelease(pErrorBlob);
    }

    return false;
  }

  return true;
}

void
DX11GraphicsManager::internalInit(const SPtr<Screen> screen,
                                  const bool bAntiliasing,
                                  const SampleDesc& sample)
{
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
#if SH_DEBUG_MODE
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  m_pDevice = make_shared<DX11Device>();
  m_pDeviceContext = make_shared<DX11DeviceContext>();

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
  scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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
  
  m_pSwapChain = make_shared<DX11SwapChain>();

  throwIfFailed(dxgiFactory->CreateSwapChain(m_pDevice->m_pDevice,
                                             &scDesc,
                                             &m_pSwapChain->m_pSwapChain));

  //Get Backbuffer Interface
  //Create a render target view

  m_pBackbuffer = make_shared<DX11Texture2D>();

  throwIfFailed(m_pSwapChain->m_pSwapChain->GetBuffer(0,
                              __uuidof(ID3D11Texture2D),
                              reinterpret_cast<LPVOID*>(&m_pBackbuffer->m_pTexture2D)));

  m_pRenderTargetView = make_shared<DX11Texture2D>();

  throwIfFailed(m_pDevice->m_pDevice->CreateRenderTargetView(m_pBackbuffer->m_pTexture2D,
                                      nullptr,
                                      &m_pRenderTargetView->m_pRenderTV));

  m_pDepthStencil = make_shared<DX11Texture2D>();

  m_pDepthStencil = reinterpret_pointer_cast<DX11Texture2D>(internalCreateTexture2D(
                                                            scDesc.BufferDesc.Width,
                                                            scDesc.BufferDesc.Height,
                                                            DXGI_FORMAT_D24_UNORM_S8_UINT,
                                                            D3D11_USAGE_DEFAULT,
                                                            D3D11_BIND_DEPTH_STENCIL));

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
DX11GraphicsManager::internalClearRenderTarget(const SPtr<Texture2D>& pTarget,
                                               const LinearColor& color)
{
  auto pRTV = reinterpret_pointer_cast<DX11Texture2D>(pTarget);
  
  FLOAT colorRGBA[4] = { color.r, color.g, color.b, color.a };

  m_pDeviceContext->m_pDeviceContext->ClearRenderTargetView(pRTV->m_pRenderTV,
                                                            colorRGBA);
}

void
DX11GraphicsManager::internalClearDepthStencil(const SPtr<Texture2D>& pDepthSV,
                                               uint32 flags,
                                               float depth,
                                               uint8 stencil)
{
  auto pDTV = reinterpret_pointer_cast<DX11Texture2D>(pDepthSV);
  
  m_pDeviceContext->m_pDeviceContext->ClearDepthStencilView(pDTV->m_pDepthSV,
                                                            flags,
                                                            depth,
                                                            stencil);
}

void
DX11GraphicsManager::internalPresent(uint32 syncInterval, uint32 flags)
{
  m_pSwapChain->m_pSwapChain->Present(syncInterval, flags);
}

SPtr<Texture2D>
DX11GraphicsManager::internalGetMainRenderTargetView() const
{
  return m_pRenderTargetView;
}

SPtr<Texture2D>
DX11GraphicsManager::internalGetMainDepthStencil() const
{
  return m_pDepthStencil;
}

SPtr<InputLayout>
DX11GraphicsManager::internalCreateInputLayout(const Vector<InputDesc>& desc,
                                               const SPtr<ProgramShader>& pPShader)
{
  auto pInputLayout = make_shared<DX11InputLayout>();
  auto pProgramShader = reinterpret_pointer_cast<DX11ProgramShader>(pPShader);

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
                                      pProgramShader->m_pVertexBlob->GetBufferPointer(),
                                      pProgramShader->m_pVertexBlob->GetBufferSize(),
                                      &pInputLayout->m_pLayout));

  return pInputLayout;
}

SPtr<InputLayout>
DX11GraphicsManager::internalCreateInputLayoutFromShader(const SPtr<ProgramShader>& pPShader)
{
  auto pInputLayout = make_shared<DX11InputLayout>();
  auto pProgramShader = reinterpret_pointer_cast<DX11ProgramShader>(pPShader);

  ID3D11ShaderReflection* pReflector = nullptr;
  throwIfFailed((D3DReflect(pProgramShader->m_pVertexBlob->GetBufferPointer(),
                            pProgramShader->m_pVertexBlob->GetBufferSize(),
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
                                      pProgramShader->m_pVertexBlob->GetBufferPointer(),
                                      pProgramShader->m_pVertexBlob->GetBufferSize(),
                                      &pInputLayout->m_pLayout));

  SafeRelease(pReflector);

  return pInputLayout;
}

SPtr<ProgramShader>
DX11GraphicsManager::internalCreateProgramShader(const String& fileName,
                                                 const String& vsEntryPoint,
                                                 const String& psEntryPoint,
                                                 const String& vsShaderModel,
                                                 const String& psShaderModel)
{
  auto pProgramShader = make_shared<DX11ProgramShader>();

  if (!compileShaderFromFile(fileName,
                             vsEntryPoint,
                             psEntryPoint,
                             vsShaderModel,
                             psShaderModel,
                             &pProgramShader->m_pVertexBlob,
                             &pProgramShader->m_pPixelBlob)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateVertexShader(
                pProgramShader->m_pVertexBlob->GetBufferPointer(),
                pProgramShader->m_pVertexBlob->GetBufferSize(),
                nullptr,
                &pProgramShader->m_pVertexShader));

  throwIfFailed(m_pDevice->m_pDevice->CreatePixelShader(
                pProgramShader->m_pPixelBlob->GetBufferPointer(),
                pProgramShader->m_pPixelBlob->GetBufferSize(),
                nullptr,
                &pProgramShader->m_pPixelShader));

  return pProgramShader;
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
  auto pCBuffer = make_shared<DX11ConstantBuffer>();

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
  auto pSampleLinear = make_shared<DX11SamplerState>();

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
DX11GraphicsManager::internalCreateTextureFromFile(const uint8* pData,
                                                   const int32 width,
                                                   const int32 height,
                                                   const int32 bpp)
{
  int32 pitch = width * bpp;

  auto pTexture = reinterpret_pointer_cast<DX11Texture2D>(internalCreateTexture2D(
                                                          width,
                                                          height,
                                                          DXGI_FORMAT_R8G8B8A8_UNORM,
                                                          D3D11_USAGE_DEFAULT,
                                                          D3D11_BIND_SHADER_RESOURCE));

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pTexture->m_pTexture2D,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        pitch,
                                                        0);
  return pTexture;
}

SPtr<Texture2D>
DX11GraphicsManager::internalCreateTexture2D(const uint32 width,
                                             const uint32 height,
                                             const uint32 format,
                                             const uint32 usage,
                                             const uint32 bindFlags)
{
  auto pTexture = std::make_shared<DX11Texture2D>();

  D3D11_TEXTURE2D_DESC textureDesc;
  memset(&textureDesc, 0, sizeof(textureDesc));
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
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
    shaderRVDesc.Format = textureDesc.Format;
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
    descDSV.Format = textureDesc.Format;
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

  return pTexture;
}

SPtr<BlendState>
DX11GraphicsManager::internalCreateBlendState(const BlendDesc& blendDesc,
                                              const LinearColor& blendFactor)
{
  auto pBlendState = make_shared<DX11BlendState>();

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
  auto pRasterizerState = make_shared<DX11RasterizerState>();

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
  auto pDepthSS = make_shared<DX11DepthStencilState>();

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
DX11GraphicsManager::internalUpdateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
                                                  const void* pData,
                                                  const uint32 dataSize)
{
  auto pConstantBuffer = reinterpret_pointer_cast<DX11ConstantBuffer>(pCBuffer);

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pConstantBuffer->m_pBuffer,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        dataSize,
                                                        0);
}

void
DX11GraphicsManager::internalUpdateTexture2D(SPtr<Texture2D>& pTexture,
                                             uint8* pData,
                                             uint32 width,
                                             uint32 bpp)
{
  auto pTex2D = reinterpret_pointer_cast<DX11Texture2D>(pTexture);
  int32 pitch = width * bpp;

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pTex2D->m_pTexture2D,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        pitch,
                                                        0);
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
DX11GraphicsManager::internalSetRenderTargets(const Vector<SPtr<Texture2D>>& pRenderTVs,
                                              const SPtr<Texture2D>& pDepthSV)
{
  auto pDepthStencil = reinterpret_pointer_cast<DX11Texture2D>(pDepthSV);
  Vector<ID3D11RenderTargetView*> pRTVs;

  for (auto& pRenderTarget : pRenderTVs) {
    auto pRTV = reinterpret_pointer_cast<DX11Texture2D>(pRenderTarget);
  
    pRTVs.push_back(pRTV->m_pRenderTV);
  }
  m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(static_cast<UINT>(pRTVs.size()),
                                                         pRTVs.data(),
                                                         pDepthStencil->m_pDepthSV);

  /*for (auto& d3d11RTV : pRTVs) {
    SafeRelease(d3d11RTV);
  }*/
}

void
DX11GraphicsManager::internalSetInputLayout(const SPtr<InputLayout>& pInput)
{
  auto pInputLayout = reinterpret_pointer_cast<DX11InputLayout>(pInput);

  m_pDeviceContext->m_pDeviceContext->IASetInputLayout(pInputLayout->m_pLayout);
}

void
DX11GraphicsManager::internalSetVertexBuffers(const SPtr<VertexBuffer>& pVBuffer,
                                              const uint32 startSlot,
                                              const uint32 numBuffers,
                                              const uint32 offset)
{
  auto pVertexBuffer = reinterpret_pointer_cast<DX11VertexBuffer>(pVBuffer);

  m_pDeviceContext->m_pDeviceContext->IASetVertexBuffers(startSlot,
                                                         numBuffers,
                                                         &pVertexBuffer->m_pBuffer,
                                                         &pVertexBuffer->m_stride,
                                                         &offset);
}

void
DX11GraphicsManager::internalSetIndexBuffers(const SPtr<IndexBuffer>& pIBuffer,
                                             const uint32 offset)
{
  auto pIndexBuffer = reinterpret_pointer_cast<DX11IndexBuffer>(pIBuffer);

  m_pDeviceContext->m_pDeviceContext->IASetIndexBuffer(pIndexBuffer->m_pBuffer,
                                      static_cast<DXGI_FORMAT>(pIndexBuffer->m_dataFormat),
                                      offset);
}

void
DX11GraphicsManager::internalVSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  auto pConstantBuffer = reinterpret_pointer_cast<DX11ConstantBuffer>(pCBuffer);

  m_pDeviceContext->m_pDeviceContext->VSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pConstantBuffer->m_pBuffer);
}

void
DX11GraphicsManager::internalPSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                                                  const uint32 startSlot,
                                                  const uint32 numBuffers)
{
  auto pConstantBuffer = reinterpret_pointer_cast<DX11ConstantBuffer>(pCBuffer);

  m_pDeviceContext->m_pDeviceContext->PSSetConstantBuffers(startSlot,
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
DX11GraphicsManager::internalSetProgramShader(const SPtr<ProgramShader>& pPShader,
                                              const void* ppClassInstances,
                                              const uint32 numClassInstances)
{
  auto pProgramShader = reinterpret_pointer_cast<DX11ProgramShader>(pPShader);

  m_pDeviceContext->m_pDeviceContext->VSSetShader(
    pProgramShader->m_pVertexShader,
    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
    numClassInstances);

  m_pDeviceContext->m_pDeviceContext->PSSetShader(
    pProgramShader->m_pPixelShader,
    reinterpret_cast<ID3D11ClassInstance* const*>(ppClassInstances),
    numClassInstances);
}

void
DX11GraphicsManager::internalSetShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                                                   const uint32 startSlot,
                                                   const uint32 numViews)
{
  auto pShaderTexture = reinterpret_pointer_cast<DX11Texture2D>(pShaderRV);

  m_pDeviceContext->m_pDeviceContext->PSSetShaderResources(startSlot,
                                                           numViews,
                                                           &pShaderTexture->m_pShaderRV);
}

void
DX11GraphicsManager::internalSetSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                                             const uint32 startSlot,
                                             const uint32 numSamplers)
{
  auto pSampler = reinterpret_pointer_cast<DX11SamplerState>(pSamplerLinear);

  m_pDeviceContext->m_pDeviceContext->PSSetSamplers(startSlot,
                                                    numSamplers,
                                                    &pSampler->m_pSamplerLinear);
}

void
DX11GraphicsManager::internalSetBlendState(const SPtr<BlendState>& pBlendState)
{
  auto pBS = reinterpret_pointer_cast<DX11BlendState>(pBlendState);

  FLOAT bf[4] = { pBS->m_blendFactor.r,
                  pBS->m_blendFactor.g,
                  pBS->m_blendFactor.b,
                  pBS->m_blendFactor.a };
  
  m_pDeviceContext->m_pDeviceContext->OMSetBlendState(pBS->m_pBlendS, bf, 0xFFFFFFFF);
}

void
DX11GraphicsManager::internalSetRasterizerState(const SPtr<RasterizerState>& pRasterizerState)
{
  auto pRS = reinterpret_pointer_cast<DX11RasterizerState>(pRasterizerState);

  m_pDeviceContext->m_pDeviceContext->RSSetState(pRS->m_pRasterS);
}

void
DX11GraphicsManager::internalSetDepthStencilState(const SPtr<DepthStencilState>& pDepthSState,
                                                  const uint8 stencilRef)
{
  auto pDepthSS = reinterpret_pointer_cast<DX11DepthStencilState>(pDepthSState);

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
}
