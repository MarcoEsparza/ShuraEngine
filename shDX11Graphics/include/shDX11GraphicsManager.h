/*****************************************************************************/
/*
*  @file    shDX11GraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/18
*  @brief   Graphics Manager for DirectX 11.
*
*  Graphics Manager for DirectX 11.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesDX11Graphics.h"
#include <shGraphicsManager.h>

#include "shDX11Buffers.h"
#include "shDX11InputLayout.h"
#include "shDX11Shader.h"
#include "shDX11Texture.h"
#include "shDX11SamplerState.h"
#include "shDX11RasterizerState.h"
#include "shDX11BlendState.h"
#include "shDX11DepthStencilState.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
/**
*  @brief Graphics Manager for DirectX 11.
*/
class DX11GraphicsManager : public GraphicsManager
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11GraphicsManager() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~DX11GraphicsManager();

  /***************************************************************************/
  /*
  *  Functions: Init and clears
  */
  /***************************************************************************/
 public:
  /**
  *  @copydoc shEngineSDK::GraphicsManager::initManager()
  */
  void
  initManager(const WPtr<Screen> pScreen,
              const bool bAntiliasing,
              const SampleDesc& sample) override;

  /**
  *  @brief Clear the render target with given LinearColor.
  * 
  *  @param SPtr<RenderTargetView> pTarget
  *  @param LinearColor& color
  */
  void
  clearRenderTarget(const WPtr<Texture2D> pTarget, const LinearColor& color) override;

  /**
  *  @brief Clear the depth stencil.
  * 
  *  @param WPtr<Texture2D> pDepthSV
  */
  void
  clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                    const uint32 flags,
                    const float depth,
                    const uint8 stencil) override;

  /**
  *  @brief Present the swapchain.
  */
  void
  present(uint32 syncInterval, uint32 flags) override;

  void
  unbindAll() override;

  /********************
  *  Getters
  ********************/

  /**
  *  @brief Returns the Render Target View.
  * 
  *  @return SPtr<RenderTargetView>
  */
  WPtr<Texture2D>
  getMainRenderTargetView() const override;

  /**
  *  @brief Returns the Depth Stencil View.
  * 
  *  @return SPtr<DepthStencilView>
  */
  WPtr<Texture2D>
  getMainDepthStencil() const override;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Creates Input Layout with given descriptor and Vertex Shader.
  * 
  *  @param Vector<shInputLayoutTypes::E>& types
  *  @param SPtr<ProgramShader> pVShader
  * 
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& desc,
                    const WPtr<VertexShader> pVShader) override;

  /**
  *  @brief Creates Input Layout from VertexShader.
  *
  *  @param SPtr<ProgramShader> pVShader
  *
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayoutFromShader(const WPtr<VertexShader> pPShader) override;

  /**
  *  @brief Creates a DX11 Vertex Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<VertexShader>
  */
  SPtr<VertexShader>
  createVertexShader(const String& fileName,
                     const String& entryPoint,
                     const String& shaderModel,
                     const Vector<ShaderMacro>& macros) override;

  /**
  *  @brief Creates a DX11 Pixel Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<PixelShader>
  */
  SPtr<PixelShader>
  createPixelShader(const String& fileName,
                    const String& entryPoint,
                    const String& shaderModel,
                    const Vector<ShaderMacro>& macros) override;

  /**
  *  @brief Creates a DX11 Geometry Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<GeometryShader>
  */
  SPtr<GeometryShader>
  createGeometryShader(const String& fileName,
                       const String& entryPoint,
                       const String& shaderModel,
                       const Vector<ShaderMacro>& macros) override;

  /**
  *  @brief Creates a DX11 Compute Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<ComputeShader>
  */
  SPtr<ComputeShader>
  createComputeShader(const String& fileName,
                      const String& entryPoint,
                      const String& shaderModel,
                      const Vector<ShaderMacro>& macros) override;

  /**
  *  @brief Creates a Vertex Buffer with given vertices.
  * 
  *  @param Vector<VertexData>& vertices
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<VertexBuffer>
  */
  SPtr<VertexBuffer>
  internalCreateVertexBuffer(const void* pData,
                             const uint32 bufferSize,
                             const uint32 stride,
                             const uint32 usage) override;

  /**
  *  @brief Creates a Index Buffer with given indices.
  * 
  *  @param Vector<int32>& indices
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<IndexBuffer>
  */
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& indices,
                    const uint32 usage) override;

  /**
  *  @brief Creates a constant buffer with given data.
  * 
  *  @param uint32 bufferSize
  *  @param void* pData = nullptr
  *  @param uint32 usage = D3D11_USAGE_DEFAULT
  * 
  *  @return SPtr<ConstantBuffer>
  */
  SPtr<ConstantBuffer>
  createConstantBuffer(const uint32 bufferSize,
                       const uint32 usage,
                       const void* pData) override;

  /**
  *  @brief Creates a Sampler State.
  * 
  *  @param uint32 filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR
  *  @param uint32 textAddress = D3D11_TEXTURE_ADDRESS_WRAP
  * 
  *  @return SPtr<SamplerState>
  */
  SPtr<SamplerState>
  createSamplerState(const uint32 filter, const uint32 textAddress) override;

  /**
  *  @brief Creates a Texture2D from file with given route.
  * 
  *  @param String& fileName
  * 
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTextureFromFile(const String& fileName,
                        const void* pData,
                        const uint32 width,
                        const uint32 height,
                        const uint32 bpp) override;

  /**
  *  @brief Creates a Texture2D from a dds file.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTextureFromDDS(const String& fileName) override;

  /**
  *  @brief Creates a Texture2D.
  * 
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = DXGI_FORMAT_B8G8R8A8_UNORM 87
  *  @param uint32 usage = D3D11_USAGE_DEFAULT 0
  *  @param uint32 bindFlags = D3D11_BIND_SHADER_RESOURCE 8
  * 
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTexture2D(const uint32 width,
                  const uint32 height,
                  const uint32 format = TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                  const uint32 usage = USAGE::kDefault,
                  const uint32 bindFlags = BIND_FLAGS::kShaderResource,
                  const uint32 mipLevels = 1,
                  const uint32 arraySize = 1) override;

  SPtr<Texture3D>
  createTexture3D(const Vector3 size,
                  const uint32 format = TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                  const Vector<LinearColor>* data = nullptr,
                  const uint32 usage = USAGE::kDefault) override;

  /*SPtr<Texture2D>
  createTexture2DArray(const uint32 width,
                       const uint32 height,
                       const uint32 arraySize = 6,
                       const uint32 format = TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                       const uint32 usage = USAGE::kDefault) override;*/

  /**
  *  @brief Creates an error Texture2D.
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createErrorTexture() override;

  SPtr<Texture2D>
  createDefaultNormalTexture() override;

  SPtr<Texture2D>
  createBlackTexture() override;

  /**
  *  @brief Creates a blend state.
  *
  *  @param BlendDesc& blendDesc
  * 
  *  @return SPtr<BlendState>
  */
  SPtr<BlendState>
  createBlendState(const BlendDesc& blendDesc, const LinearColor& blendFactor) override;

  /**
  *  @brief Creates a rasterizer state.
  *
  *  @param RasterizerDesc& RasterizerDesc
  * 
  *  @return SPtr<RasterizerState>
  */
  SPtr<RasterizerState>
  createRasterizerState(const RasterizerDesc& rasterizerDesc) override;

  /**
  *  @brief Creates a depth stencil state.
  *
  *  @param DepthStencilDesc& depthSDesc
  * 
  *  @return SPtr<DepthStencilState>
  */
  SPtr<DepthStencilState>
  createDepthStencilState(const DepthStencilDesc& depthSDesc) override;

  /**
  *  @brief GenerateMips for a texture.
  *
  *  @param SPtr<Texture2D>& pTexture
  */
  void
  generateMips(const WPtr<Texture2D> pTexture) override;

  /********************
  *  Update
  ********************/

  /**
  *  @brief Update Constant Buffer subresource with given data.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param void* pData
  *  @param uint32 dataSize
  */
  void
  updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                       const void* pData,
                       const uint32 dataSize) override;

  /**
  *  @brief Update a given Texture2D with given data.
  *
  *  @param SPtr<Texture2D>& pTexture
  *  @param uint8* pData
  *  @param uint32 width
  *  @param uint32 bpp
  */
  void
  updateTexture2D(WPtr<Texture2D> pTexture,
                  uint8* pData,
                  uint32 width,
                  uint32 bpp) override;

  /**
  *  @brief Update the screen size.
  *
  *  @param SPtr<Screen>& pScreen
  */
  virtual void
  updateScreenSize(const Vector2& size) override;

  void
  updateVertexBuffer(const WPtr<VertexBuffer> pVBuffer,
                     const void* pData,
                     const uint32 dataSize) override;

  void
  updateIndexBuffer(const WPtr<IndexBuffer> pIBuffer,
                    const void* pData,
                    const uint32 dataSize) override;

  /**
  *  @brief Saves a Texture2D to a dds file.
  *
  *  @param SPtr<Texture2D>& pTexture
  *  @param String&  filePath
  */
  void
  saveTextureToDDS(const WPtr<Texture2D> pTexture, const String& filePath) override;

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Set the viewport.
  *
  *  @param Viewport& vp
  */
  void
  setViewport(const Viewport& vp) override;

  /**
  *  @brief Set the render targets with number of views.
  * 
  *  @param Vector<SPtr<RenderTargetView>>& pRenderTVs
  *  @param SPtr<Texture2D> pDepthSV
  *  @param uint32 numViews
  */
  void
  setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                   const WPtr<Texture2D> pDepthSV) override;

  /**
  *  @brief Sets the Input Layout.
  * 
  *  @param SPtr<InputLayout> pInput
  */
  void
  setInputLayout(const WPtr<InputLayout> pInput) override;

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  * 
  *  @param SPtr<VertexBuffer> pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  void
  setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                   const uint32 startSlot,
                   const uint32 numBuffers,
                   const uint32 offset) override;

  /**
  *  @brief Sets a Index Buffer with given offset.
  * 
  *  @param SPtr<IndexBuffer> pIBuffer
  *  @param uint32 offset = 0
  */
  void
  setIndexBuffers(const WPtr<IndexBuffer> pIBuffer, const uint32 offset) override;

  /**
  *  @brief Sets a Constant Buffer for the vertex shader with given start slot
  *         and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot,
                       const uint32 numBuffers) override;

  /**
  *  @brief Sets a Constant Buffer for the pixel shader with given start slot
  *         and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot,
                       const uint32 numBuffers) override;

  /**
  *  @brief Sets a Constant Buffer for the geometry shader with given start slot
  *         and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot,
                       const uint32 numBuffers) override;

  /**
  *  @brief Sets a Constant Buffer for the compute shader with given start slot
  *         and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot,
                       const uint32 numBuffers) override;

  /**
  *  @brief Sets the primitive topology.
  * 
  *  @param uint32 primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST 4
  */
  void
  setPrimitiveTopology(const uint32 primitive) override;

  /**
  *  @brief Sets the Vertex Shader.
  * 
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances
  *  @param uint32 numClassInstances
  */
  virtual void
  setVertexShader(const WPtr<VertexShader> pVShader,
                  const void* ppClassInstances,
                  const uint32 numClassInstances) override;

  /**
  *  @brief Sets the Pixel Shader.
  *
  *  @param SPtr<PixelShader>& pPShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  setPixelShader(const WPtr<PixelShader> pPShader,
                 const void* ppClassInstances = nullptr,
                 const uint32 numClassInstances = 0) override;

  /**
  *  @brief Sets the Geometry Shader.
  *
  *  @param SPtr<GeometryShader>& pGShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  setGeometryShader(const WPtr<GeometryShader> pGShader,
                    const void* ppClassInstances = nullptr,
                    const uint32 numClassInstances = 0) override;

  /**
  *  @brief Sets the Compute Shader.
  *
  *  @param SPtr<ComputeShader>& pCShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  setComputeShader(const WPtr<ComputeShader> pCShader,
                   const void* ppClassInstances = nullptr,
                   const uint32 numClassInstances = 0) override;

  /**
  *  @brief Sets a shader resource to the pixel shader.
  * 
  *  @param SPtr<Texture> pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot,
                          const uint32 numViews) override;

  /**
  *  @brief Sets a shader resource to the compute shader.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  */
  void
  csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot,
                          const uint32 numViews) override;

  /**
  *  @brief Sets a 3D shader resource to the pixel shader.
  *
  *  @param SPtr<Texture3D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  psSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) override;

  /**
  *  @brief Sets a 3D shader resource to the compute shader.
  *
  *  @param SPtr<Texture3D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  csSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) override;

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  *  @param uint32* count
  */
  void
  setUnorderedAccessView(const UnorderedAccess& pUAVs, const uint32 startSlot) override;

  /**
  *  @brief Sets the Sampler State to the pixel shader.
  * 
  *  @param SPtr<SamplerState> pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  psSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                    const uint32 startSlot,
                    const uint32 numSamplers) override;

  /**
  *  @brief Sets the Sampler State to the compute shader.
  * 
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot
  *  @param uint32 numSamplers
  */
  void
  csSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                            const uint32 startSlot,
                            const uint32 numSamplers) override;

  /**
  *  @brief Sets the blend state to the device context.
  *
  *  @param SPtr<BlendState>& pBlendState
  *  @param Vector4& blendFactor
  */
  void
  setBlendState(const WPtr<BlendState> pBlendState) override;

  /**
  *  @brief Sets the rasterizer state to the device context.
  *
  *  @param SPtr<RasterizerState>& pRasterizerState
  */
  void
  setRasterizerState(const WPtr<RasterizerState> pRasterizerState) override;

  /**
  *  @brief Sets the depth stencil state to the device context.
  *
  *  @param SPtr<DepthStencilState>& pDepthSState
  *  @param uint8 stencilRef
  */
  void
  setDepthStencilState(const WPtr<DepthStencilState> pDepthSState,
                               const uint8 stencilRef) override;

  /**
  *  @brief Sets the scissor rect.
  *
  *  @param Rect& scissorClip
  */
  void
  setScissorRects(const Rect& scissorClip) override;

  /**
  *  @brief Draw with vertices info.
  * 
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  void
  draw(const uint32 vertexCount, const uint32 startVertexLocation) override;

  /**
  *  @brief Draw with indices and vertices info.
  * 
  *  @param uint32 indexCount
  *  @param uint32 StartVertexLocation
  *  @param uint32 baseVertexLocation
  */
  void
  drawIndexed(const uint32 indexCount,
              const uint32 startIndexLocation,
              const uint32 baseVertexLocation) override;

  /**
  *  @brief Dispatch compute shader.
  *
  *  @param uint32 threadGroupCountX
  *  @param uint32 threadGroupCountY
  *  @param uint32 threadGroupCountZ
  */
  void
  dispatch(const uint32 threadGroupCountX,
           const uint32 threadGroupCountY,
           const uint32 threadGroupCountZ) override;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Descriptor for sample configuration.
  */
  SampleDesc m_multiSampleConfig;

  /**
  *  @brief Is fullscreen?
  */
  bool m_bFullScreen = false;

  /**
  *  @brief GPU Device.
  */
  //SPtr<DX11Device> m_pDevice;
  ID3D11Device* m_pDevice = nullptr;

  /**
  *  @brief GPU Device Context.
  */
  //SPtr<DX11DeviceContext> m_pDeviceContext;
  ID3D11DeviceContext* m_pDeviceContext = nullptr;

  /**
  *  @brief The SwapChain for the front and back buffers.
  */
  IDXGISwapChain* m_pSwapChain = nullptr;

  /**
  *  @brief The back buffer.
  */
  SPtr<Texture2D> m_pBackbuffer;

  /**
  *  @brief The main Depth Stencil.
  */
  SPtr<Texture2D> m_pDepthStencil;

#if defined(SH_DEBUG_MODE)
  ID3D11Debug* m_pDebug = nullptr;
#endif
};
}

#endif
