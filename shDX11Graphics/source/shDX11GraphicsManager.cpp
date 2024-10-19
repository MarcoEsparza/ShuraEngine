/*************************************************************/
/*
*  @file    shDX11GraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   Graphics Manager for DirectX 11.
*
*  Graphics Manager for DirectX 11.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shDX11GraphicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3dcompiler.h>

// TODO: Delete this, wrapper will be added later.
using std::make_shared;

namespace shEngineSDK {
FORCEINLINE void
throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    SH_ASSERT(false && "Something went horribly wrong!!!");
  }
}

bool
compileShaderFromFile(const String& fileName,
                      const String& entryPoint,
                      const String& shaderModel,
                      ID3DBlob** pBlob)
{
  HRESULT hr = S_OK;
  int32 shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if SH_DEBUG_MODE
  shaderFlags |= D3DCOMPILE_DEBUG;
#endif

  auto beg = (String::const_iterator)fileName.begin();
  auto end = (String::const_iterator)fileName.end();
  WString wFileName(beg, end);

  ID3DBlob* pErrorBlob = nullptr;

  hr = D3DCompileFromFile(wFileName.c_str(),
                          nullptr,
                          nullptr,
                          entryPoint.c_str(),
                          shaderModel.c_str(),
                          shaderFlags,
                          0,
                          pBlob,
                          &pErrorBlob);

  if (FAILED(hr))
  {
    if (nullptr != pErrorBlob)
    {
      String errStr(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
      SafeRelease(pErrorBlob);
    }

    return false;
  }

  return true;
}

void
DX11GraphicsManager::init(void* srcHandle,
                          bool bFullScreen,
                          bool bAntiliasing,
                          uint32 samplesPerPixel,
                          uint32 sampleQuality)
{
  m_bFullScreen = bFullScreen;

  auto hWnd = reinterpret_cast<HWND>(srcHandle);
  RECT rc;
  GetClientRect(hWnd, &rc);

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
    scDesc.BufferDesc.Width = rc.right;
    scDesc.BufferDesc.Height = rc.bottom;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  }

  scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

  m_multiSampleConfig.count = 1;
  m_multiSampleConfig.quality = 0;
  if (bAntiliasing)
  {
    m_multiSampleConfig.count = samplesPerPixel;
    m_multiSampleConfig.quality = sampleQuality;
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
  
  throwIfFailed(dxgiFactory->CreateSwapChain(m_pDevice->m_pDevice,
                                             &scDesc,
                                             &m_pSwapChain->m_pSwapChain));

  //Get Backbuffer Interface
  //Create a render target view

  throwIfFailed(m_pSwapChain->m_pSwapChain->GetBuffer(0,
                __uuidof(ID3D11Texture2D),
                reinterpret_cast<LPVOID*>(&m_pBackbuffer->m_pTexture2D)));

  throwIfFailed(m_pDevice->m_pDevice->CreateRenderTargetView(m_pBackbuffer->m_pTexture2D,
                nullptr,
                &m_pRenderTargetView->m_pRenderTV));

  auto pTDSV = createTexture2D(scDesc.BufferDesc.Width,
                               scDesc.BufferDesc.Height,
                               DXGI_FORMAT_D24_UNORM_S8_UINT,
                               D3D11_USAGE_DEFAULT,
                               D3D11_BIND_DEPTH_STENCIL);

  m_pDepthStencil->m_pTexture2D = pTDSV->m_pTexture2D;
  m_pDepthStencil->m_pDepthSV = pTDSV->m_pDepthSV;

  //Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = static_cast<FLOAT>(scDesc.BufferDesc.Width);
  vp.Height = static_cast<FLOAT>(scDesc.BufferDesc.Height);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pDeviceContext->m_pDeviceContext->RSSetViewports(1, &vp);

  //TODO : REMOVE
  m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(1,
                                                         &m_pRenderTargetView->m_pRenderTV,
                                                         m_pDepthStencil->m_pDepthSV);

  //Release all objects
  SafeRelease(pFactory);
  SafeRelease(pAdapter);
  SafeRelease(pAdapter1);
  SafeRelease(dxgiFactory);
  SafeRelease(dxgiAdapter);
  SafeRelease(dxgiDevice);

  for (auto adap : vecAdapters)
  {
    SafeRelease(adap);
  }
}

void
DX11GraphicsManager::clearRenderTarget(SPtr<DX11RenderTargetView> pTarget, LinearColor& color)
{
  m_pDeviceContext->m_pDeviceContext->ClearRenderTargetView(pTarget->m_pRenderTV,
                                                            reinterpret_cast<FLOAT*>(&color));
}

void
DX11GraphicsManager::clearDepthStencil(SPtr<DX11DepthStencilView> pDepthSV)
{
  m_pDeviceContext->m_pDeviceContext->ClearDepthStencilView(pDepthSV->m_pDepthSV,
                                                            D3D11_CLEAR_DEPTH,
                                                            1.0f,
                                                            0);
}

void
DX11GraphicsManager::present()
{
  m_pSwapChain->m_pSwapChain->Present(0, 0);
}

SPtr<DX11RenderTargetView>
DX11GraphicsManager::getBackBufferRenderTargetView() const
{
  return m_pRenderTargetView;
}

SPtr<DX11DepthStencilView>
DX11GraphicsManager::getMainDepthStencil() const
{
  return m_pDepthStencil;
}

SPtr<DX11DeviceContext>
DX11GraphicsManager::getDeviceContext() const
{
  return m_pDeviceContext;
}

SPtr<DX11InputLayout>
DX11GraphicsManager::createInputLayout(const Vector<InputLayoutDesc>& ilDesc,
                                       SPtr<DX11VertexShader> pVShader)
{
  auto pInputLayout = make_shared<DX11InputLayout>();

  Vector<D3D11_INPUT_ELEMENT_DESC> d3d11ILDesc;
  d3d11ILDesc.resize(ilDesc.size());

  for (uint32 i = 0; i < ilDesc.size(); ++i) {
    auto& posElement = d3d11ILDesc[i];
    auto& desc = ilDesc[i];

    memset(&posElement, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));

    posElement.SemanticName = static_cast<LPCSTR>(desc.semanticName.c_str());
    posElement.SemanticIndex = static_cast<UINT>(desc.semanticIndex);
    posElement.Format = static_cast<DXGI_FORMAT>(desc.format);
    posElement.InputSlot = static_cast<UINT>(desc.inputSlot);
    posElement.AlignedByteOffset = static_cast<UINT>(desc.aligenedByteOffset);
    posElement.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(desc.inputSlotClass);
    posElement.InstanceDataStepRate = static_cast<UINT>(desc.instanceDataStepRate);
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateInputLayout(&d3d11ILDesc[0],
                                                        static_cast<UINT>(d3d11ILDesc.size()),
                                                        pVShader->m_pBlob->GetBufferPointer(),
                                                        pVShader->m_pBlob->GetBufferSize(),
                                                        &pInputLayout->m_pLayout));

  return pInputLayout;
}

SPtr<DX11VertexShader>
DX11GraphicsManager::createVertexShader(const String& fileName,
                                        const String& entryPoint,
                                        const String& shaderModel)
{
  auto pVertexShader = make_shared<DX11VertexShader>();

  if (!compileShaderFromFile(fileName, entryPoint, shaderModel, &pVertexShader->m_pBlob)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateVertexShader(
                                      pVertexShader->m_pBlob->GetBufferPointer(),
                                      pVertexShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pVertexShader->m_pShader));

  return pVertexShader;
}

SPtr<DX11PixelShader>
DX11GraphicsManager::createPixelShader(const String& fileName,
                                       const String& entryPoint,
                                       const String& shaderModel)
{
  auto pPixelShader = make_shared<DX11PixelShader>();

  if (!compileShaderFromFile(fileName, entryPoint, shaderModel, &pPixelShader->m_pBlob)) {
    return nullptr;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreatePixelShader(
                                      pPixelShader->m_pBlob->GetBufferPointer(),
                                      pPixelShader->m_pBlob->GetBufferSize(),
                                      nullptr,
                                      &pPixelShader->m_pShader));

  return pPixelShader;
}

SPtr<DX11ConstantBuffer>
DX11GraphicsManager::createConstantBuffer(uint32 bufferSize, void* pData, uint32 usage)
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
  if (pData)
  {
    initData.pSysMem = pData;
    initData.SysMemPitch = bufferSize;
    initData.SysMemSlicePitch = 0;
  }

  throwIfFailed(m_pDevice->m_pDevice->CreateBuffer(&desc, pData ? &initData : nullptr,
                                                   &pCBuffer->m_pBuffer));

  return pCBuffer;
}

SPtr<DX11SamplerState>
DX11GraphicsManager::createSamplerState(uint32 filter, uint32 textAddress)
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

SPtr<DX11DepthStencilView>
DX11GraphicsManager::createDepthSV(SPtr<DX11Texture2D> pText)
{
  auto pDepthSV = std::make_shared<DX11DepthStencilView>();

  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  memset(&descDSV, 0, sizeof(descDSV));
  descDSV.Format = DXGI_FORMAT_R32G32B32A32_UINT;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;

  throwIfFailed(m_pDevice->m_pDevice->CreateDepthStencilView(pText->m_pTexture2D,
                                                             &descDSV,
                                                             &pDepthSV->m_pDepthSV));

  return pDepthSV;
}

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

SPtr<DX11Texture2D>
DX11GraphicsManager::createTextureFromFile(const String& fileName)
{
  int32 width, height, bpp;

  unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

  int32 pitch = width * bpp;

  auto pTexture = createTexture2D(width, height);

  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pTexture->m_pTexture2D,
                                                        0,
                                                        nullptr,
                                                        data,
                                                        pitch,
                                                        0);

  stbi_image_free(data);
  return pTexture;
}

SPtr<DX11Texture2D>
DX11GraphicsManager::createTexture2D(uint32 width,
                                     uint32 height,
                                     uint32 format,
                                     uint32 usage,
                                     uint32 bindFlags)
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

  return pTexture;
}

void
DX11GraphicsManager::updateConstantBuffer(SPtr<DX11ConstantBuffer> pCBuffer,
                                          void* pData,
                                          uint32 dataSize)
{
  m_pDeviceContext->m_pDeviceContext->UpdateSubresource(pCBuffer->m_pBuffer,
                                                        0,
                                                        nullptr,
                                                        pData,
                                                        dataSize,
                                                        0);
}

void
DX11GraphicsManager::setRenderTargets(SPtr<DX11RenderTargetView> pRenderTV,
                                      SPtr<DX11DepthStencilView> pDepthSV,
                                      uint32 numViews)
{
  m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(numViews,
                                                         &pRenderTV->m_pRenderTV,
                                                         pDepthSV->m_pDepthSV);
}

void
DX11GraphicsManager::setInputLayout(SPtr<DX11InputLayout> pInput)
{
  m_pDeviceContext->m_pDeviceContext->IASetInputLayout(pInput->m_pLayout);
}
void
DX11GraphicsManager::setVertexBuffers(SPtr<DX11VertexBuffer> pVBuffer,
                                      uint32 startSlot,
                                      uint32 numBuffers,
                                      uint32 offset)
{
  m_pDeviceContext->m_pDeviceContext->IASetVertexBuffers(startSlot,
                                                         numBuffers,
                                                         &pVBuffer->m_pBuffer,
                                                         &pVBuffer->m_stride,
                                                         &offset);
}

void
DX11GraphicsManager::setIndexBuffers(SPtr<DX11IndexBuffer> pIBuffer, uint32 offset)
{
  m_pDeviceContext->m_pDeviceContext->IASetIndexBuffer(pIBuffer->m_pBuffer,
                                      static_cast<DXGI_FORMAT>(pIBuffer->m_dataFormat),
                                      offset);
}

void
DX11GraphicsManager::vsSetConstantBuffers(SPtr<DX11ConstantBuffer> pCBuffer,
                                          uint32 startSlot,
                                          uint32 numBuffers)
{
  m_pDeviceContext->m_pDeviceContext->VSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pCBuffer->m_pBuffer);
}

void
DX11GraphicsManager::psSetConstantBuffers(SPtr<DX11ConstantBuffer> pCBuffer,
                                          uint32 startSlot,
                                          uint32 numBuffers)
{
  m_pDeviceContext->m_pDeviceContext->PSSetConstantBuffers(startSlot,
                                                           numBuffers,
                                                           &pCBuffer->m_pBuffer);
}

void
DX11GraphicsManager::setPrimitiveTopology(uint32 primitive)
{
  m_pDeviceContext->m_pDeviceContext->IASetPrimitiveTopology(
                                      static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitive));
}

void
DX11GraphicsManager::setVertexShader(SPtr<DX11VertexShader> pVShader,
                                     ID3D11ClassInstance* const* ppClassInstances,
                                     uint32 numClassInstances)
{
  m_pDeviceContext->m_pDeviceContext->VSSetShader(pVShader->m_pShader,
                                                  ppClassInstances,
                                                  numClassInstances);
}

void
DX11GraphicsManager::setPixelShader(SPtr<DX11PixelShader> pPShader,
                                    ID3D11ClassInstance* const* ppClassInstances,
                                    uint32 numClassInstances)
{
  m_pDeviceContext->m_pDeviceContext->PSSetShader(pPShader->m_pShader,
                                                  ppClassInstances,
                                                  numClassInstances);
}

void
DX11GraphicsManager::setShaderResourceView(SPtr<DX11Texture> pShaderRV,
                                           uint32 startSlot,
                                           uint32 numViews)
{
  m_pDeviceContext->m_pDeviceContext->PSSetShaderResources(startSlot,
                                                           numViews,
                                                           &pShaderRV->m_pShaderRV);
}

void
DX11GraphicsManager::setSamplerState(SPtr<DX11SamplerState> pSamplerLinear,
                                     uint32 startSlot,
                                     uint32 numSamplers)
{
  m_pDeviceContext->m_pDeviceContext->PSSetSamplers(startSlot,
                                                    numSamplers,
                                                    &pSamplerLinear->m_pSamplerLinear);
}

void
DX11GraphicsManager::draw(uint32 vertexCount, uint32 startVertexLocation)
{
  m_pDeviceContext->m_pDeviceContext->Draw(vertexCount, startVertexLocation);
}

void
DX11GraphicsManager::drawIndexed(uint32 indexCount,
                                 uint32 startIndexLocation,
                                 uint32 baseVertexLocation)
{
  m_pDeviceContext->m_pDeviceContext->DrawIndexed(indexCount,
                                                  startIndexLocation,
                                                  baseVertexLocation);
}
}
