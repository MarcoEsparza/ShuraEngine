/*************************************************************/
/*
*  @file    shDX11GraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   Graphics Manager for DirectX 11.
*
*  Graphics Manager for DirectX 11.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesDX11Graphics.h"
#include "shModule.h"

#include "shDX11Buffers.h"
#include "shDX11Device.h"
#include "shDX11DepthStencilView.h"
#include "shDX11InputLayout.h"
#include "shDX11RenderTargetView.h"
#include "shDX11SamplerState.h"
#include "shDX11Shader.h"
#include "shDX11SwapChain.h"
#include "shDX11Texture.h"

#include "shLinearColor.h"

#include "d3d11.h"

namespace shEngineSDK {
/**
*  @brief Sample descriptor.
*/
typedef struct SAMPLE_DESC
{
  UINT count = 0;
  UINT quality = 0;
} SAMPLE_DESC;

/**
*  @brief InputLayout descriptor.
*/
struct InputLayoutDesc
{
  String semanticName;
  uint32 semanticIndex;
  uint32 format;
  uint32 inputSlot;
  uint32 aligenedByteOffset;
  uint32 inputSlotClass;
  uint32 instanceDataStepRate;
};

/**
*  @brief Graphics Manager for DirectX 11.
*/
class DX11GraphicsManager : public Module<DX11GraphicsManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11GraphicsManager() = default;

  /**
  *  @brief Default destructor.
  */
  ~DX11GraphicsManager() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Initialize the graphics manager.
  * 
  *  @param void* srcHandle
  *  @param bool bFullScreen
  *  @param bool bAntiliasing
  *  @param uint32 samplesPerPixel
  *  @param uint32 sampleQuality
  */
  void
  init(void* srcHandle,
       bool bFullScreen,
       bool bAntiliasing,
       uint32 samplesPerPixel,
       uint32 sampleQuality);

  /**
  *  @brief Clear the render target with given LinearColor.
  * 
  *  @param SPtr<RenderTargetView> pTarget
  *  @param LinearColor& color
  */
  void
  clearRenderTarget(SPtr<DX11RenderTargetView> pTarget, LinearColor& color);

  /**
  *  @brief Clear the depth stencil.
  * 
  *  @param SPtr<DepthStencilView> pDepthSV
  */
  void
  clearDepthStencil(SPtr<DX11DepthStencilView> pDepthSV);

  /**
  *  @brief Present the swapchain.
  */
  void
  present();

  /**
  *  @brief Returns the Render Target View.
  * 
  *  @return SPtr<RenderTargetView>
  */
  SPtr<DX11RenderTargetView>
  getBackBufferRenderTargetView() const;

  /**
  *  @brief Returns the Depth Stencil View.
  * 
  *  @return SPtr<DepthStencilView>
  */
  SPtr<DX11DepthStencilView>
  getMainDepthStencil() const;

  /**
  *  @brief Returns the Device Context.
  * 
  *  @return SPtr<DeviceContext>
  */
  SPtr<DX11DeviceContext>
  getDeviceContext() const;

  /**
  *  @brief Creates Input Layout with given descriptor and Vertex Shader.
  * 
  *  @param Vector<InputLayoutDesc>& desc
  *  @param SPtr<VertexShader> pVShader
  * 
  *  @return SPtr<InputLayout>
  */
  SPtr<DX11InputLayout>
  createInputLayout(const Vector<InputLayoutDesc>& desc,
                    SPtr<DX11VertexShader> pVShader);

  /**
  *  @brief Creates a Vertex Shader.
  * 
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  * 
  *  @return SPtr<VertexShader>
  */
  SPtr<DX11VertexShader>
  createVertexShader(const String& fileName,
                     const String& entryPoint,
                     const String& shaderModel);

  /**
  *  @brief Creates a Pixel Shader.
  * 
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  * 
  *  @return SPtr<PixelShader>
  */
  SPtr<DX11PixelShader>
  createPixelShader(const String& fileName,
                    const String& entryPoint,
                    const String& shaderModel);

  /**
  *  @brief Creates a Vertex Buffer with given vertices.
  *  @brief Vertices is a typename with information user needs.
  * 
  *  @param Vector<T>& vertices
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<VertexBuffer>
  */
  template<typename T>
  FORCEINLINE SPtr<DX11VertexBuffer>
  createVertexBuffer(const Vector<T>& vertices,
                     uint32 usage = D3D11_USAGE_DEFAULT);

  /**
  *  @brief Creates a Index Buffer with given indices.
  *  @brief Indices is a typename with information user needs.
  * 
  *  @param Vector<T>& indices
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<IndexBuffer>
  */
  template<typename T>
  FORCEINLINE SPtr<DX11IndexBuffer>
  createIndexBuffer(const Vector<T>& indices,
                    uint32 usage = D3D11_USAGE_DEFAULT);

  /**
  *  @brief Creates a constant buffer with given data.
  * 
  *  @param uint32 bufferSize
  *  @param void* pData = nullptr
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<ConstantBuffer>
  */
  SPtr<DX11ConstantBuffer>
  createConstantBuffer(uint32 bufferSize,
                       void* pData = nullptr,
                       uint32 usage = D3D11_USAGE_DEFAULT);

  /**
  *  @brief Creates a Sampler State.
  * 
  *  @param uint32 filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR
  *  @param uint32 textAddress = D3D11_TEXTURE_ADDRESS_WRAP
  * 
  *  @return SPtr<SamplerState>
  */
  SPtr<DX11SamplerState>
  createSamplerState(uint32 filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
                     uint32 textAddress = D3D11_TEXTURE_ADDRESS_WRAP);

  /**
  *  @brief Creates a Depth Stencil View with given Texture2D.
  * 
  *  @param SPtr<Texture2D> pText
  * 
  *  @return SPtr<DepthStencilView>
  */
  SPtr<DX11DepthStencilView>
  createDepthSV(SPtr<DX11Texture2D> pText);

  /**
  *  @brief Creates a Texture2D from file with given route.
  * 
  *  @param String& fileName
  * 
  *  @return SPtr<Texture2D>
  */
  SPtr<DX11Texture2D>
  createTextureFromFile(const String& fileName);

  /**
  *  @brief Creates a Texture2D.
  * 
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = DXGI_FORMAT_B8G8R8A8_UNORM
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  *  @param uint32 bindFlags = D3D11_BIND_SHADER_RESOURCE
  * 
  *  @return SPtr<Texture2D>
  */
  SPtr<DX11Texture2D>
  createTexture2D(uint32 width,
                  uint32 height,
                  uint32 format = DXGI_FORMAT_B8G8R8A8_UNORM,
                  uint32 usage = D3D11_USAGE_DEFAULT,
                  uint32 bindFlags = D3D11_BIND_SHADER_RESOURCE);

  /**
  *  @brief Update Constant Buffer subresource with given data.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param void* pData
  *  @param uint32 dataSize
  */
  void
  updateConstantBuffer(SPtr<DX11ConstantBuffer> pCBuffer,
                       void* pData,
                       uint32 dataSize);

  /**
  *  @brief Set the render targets with number of views.
  * 
  *  @param SPtr<RenderTargetView> pRenderTV
  *  @param SPtr<DepthStencilView> pDepthSV
  *  @param uint32 numViews
  */
  void
  setRenderTargets(SPtr<DX11RenderTargetView> pRenderTV,
                   SPtr<DX11DepthStencilView> pDepthSV,
                   uint32 numViews);

  /**
  *  @brief Sets the Input Layout.
  * 
  *  @param SPtr<InputLayout> pInput
  */
  void
  setInputLayout(SPtr<DX11InputLayout> pInput);

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  * 
  *  @param SPtr<VertexBuffer> pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  void
  setVertexBuffers(SPtr<DX11VertexBuffer> pVBuffer,
                   uint32 startSlot = 0,
                   uint32 numBuffers = 1,
                   uint32 offset = 0);

  /**
  *  @brief Sets a Index Buffer with given offset.
  * 
  *  @param SPtr<IndexBuffer> pIBuffer
  *  @param uint32 offset = 0
  */
  void
  setIndexBuffers(SPtr<DX11IndexBuffer> pIBuffer, uint32 offset = 0);

  /**
  *  @brief Sets a Constant Buffer for  the vertex shader with given start slot
            and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  vsSetConstantBuffers(SPtr<DX11ConstantBuffer> pCBuffer,
                       uint32 startSlot = 0,
                       uint32 numBuffers = 1);

  /**
  *  @brief Sets a Constant Buffer for  the pixel shader with given start slot
            and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  psSetConstantBuffers(SPtr<DX11ConstantBuffer> pCBuffer,
                       uint32 startSlot = 0,
                       uint32 numBuffers = 1);

  /**
  *  @brief Sets the primitive topology.
  * 
  *  @param uint32 primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  */
  void
  setPrimitiveTopology(uint32 primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  
  /**
  *  @brief Sets the Vertex Shader.
  * 
  *  @param SPtr<VertexShader> pVShader
  *  @param ID3D11ClassInstance* const* ppClassInstances = nullptr
  *  @param uint32 numClassInstances = 0
  */
  void
  setVertexShader(SPtr<DX11VertexShader> pVShader,
                  ID3D11ClassInstance* const* ppClassInstances = nullptr,
                  uint32 numClassInstances = 0);

  /**
  *  @brief Sets the Pixel Shader.
  * 
  *  @param SPtr<PixelShader> pPShader
  *  @param ID3D11ClassInstance* const* ppClassInstances = nullptr
  *  @param uint32 numClassInstances = 0
  */
  void
  setPixelShader(SPtr<DX11PixelShader> pPShader,
                 ID3D11ClassInstance* const* ppClassInstances = nullptr,
                 uint32 numClassInstances = 0);

  /**
  *  @brief Sets a shader resource.
  * 
  *  @param SPtr<Texture> pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  setShaderResourceView(SPtr<DX11Texture> pShaderRV, uint32 startSlot = 0, uint32 numViews = 1);

  /**
  *  @brief Sets the Sampler State.
  * 
  *  @param SPtr<SamplerState> pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  setSamplerState(SPtr<DX11SamplerState> pSamplerLinear,
                  uint32 startSlot = 0,
                  uint32 numSamplers = 1);

  /**
  *  @brief Draw with vertices info.
  * 
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  void
  draw(uint32 vertexCount, uint32 startVertexLocation);

  /**
  *  @brief Draw with indices and vertices info.
  * 
  *  @param uint32 indexCount
  *  @param uint32 StartVertexLocation
  *  @param uint32 baseVertexLocation
  */
  void
  drawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief Descriptor for sample configuration.
  */
  SAMPLE_DESC m_multiSampleConfig;

  /**
  *  @brief Is fullscreen?
  */
  bool m_bFullScreen = false;

  /**
  *  @brief GPU Device.
  */
  SPtr<DX11Device> m_pDevice = std::make_shared<DX11Device>();

  /**
  *  @brief GPU Device Context.
  */
  SPtr<DX11DeviceContext> m_pDeviceContext = std::make_shared<DX11DeviceContext>();

  /**
  *  @brief The SwapChain for the front and back buffers.
  */
  SPtr<DX11SwapChain> m_pSwapChain = std::make_shared<DX11SwapChain>();

  /**
  *  @brief The back buffer.
  */
  SPtr<DX11Texture2D> m_pBackbuffer = std::make_shared<DX11Texture2D>();

  /**
  *  @brief The main Depth Stencil.
  */
  SPtr<DX11DepthStencilView> m_pDepthStencil = std::make_shared<DX11DepthStencilView>();

  /**
  *  @brief The main Render Target View.
  */
  SPtr<DX11RenderTargetView> m_pRenderTargetView = std::make_shared<DX11RenderTargetView>();
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

/**
*  @brief Creates a Vertex Buffer with given vertices.
*  @brief Vertices is a typename with information user needs.
*
*  @param Vector<T>& vertices
*  @param uint32 usage = D3D11_USAGE_DEFAULT
*
*  @return SPtr<VertexBuffer>
*/
template<typename T>
FORCEINLINE SPtr<DX11VertexBuffer>
DX11GraphicsManager::createVertexBuffer(const Vector<T>& vertices, uint32 usage)
{
  auto pVBuffer = std::make_shared<DX11VertexBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(T));
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ?
                                 D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = &vertices[0];
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  m_pDevice->m_pDevice->CreateBuffer(&desc, &initData, &pVBuffer->m_pBuffer);
  pVBuffer->m_stride = sizeof(T);

  return pVBuffer;
}

/**
*  @brief Creates a Index Buffer with given indices.
*  @brief Indices is a typename with information user needs.
*
*  @param Vector<T>& indices
*  @param uint32 usage = D3D11_USAGE_DEFAULT
*
*  @return SPtr<IndexBuffer>
*/
template<typename T>
FORCEINLINE SPtr<DX11IndexBuffer>
DX11GraphicsManager::createIndexBuffer(const Vector<T>& indices, uint32 usage)
{
  auto pIBuffer = std::make_shared<DX11IndexBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = static_cast<D3D11_USAGE>(usage);
  desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(T));
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  desc.CPUAccessFlags = usage == D3D10_USAGE_DYNAMIC ?
                                 D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = &indices[0];
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  m_pDevice->m_pDevice->CreateBuffer(&desc, &initData, &pIBuffer->m_pBuffer);
  if (sizeof(T) == 2)
  {
    pIBuffer->m_dataFormat = DXGI_FORMAT_R16_UINT;
  }

  return pIBuffer;
}
}
