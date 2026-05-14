/*************************************************************/
/*
*  @file    shOGLGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
*  @brief   Graphics Manager for Open GL.
*
*  Graphics Manager for Open GL.
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
#include "shPrerequisitesOGLGraphics.h"
#include "shGraphicsManager.h"

#include "shOGLBuffers.h"
#include "shOGLInputLayout.h"
#include "shOGLShader.h"
#include "shOGLTexture.h"
#include "shOGLSamplerState.h"
#include "shOGLRasterizerState.h"
#include "shOGLBlendState.h"
#include "shOGLDepthStencilState.h"

namespace shEngineSDK {
class OGLGraphicsManager : public GraphicsManager
{
 public:
  OGLGraphicsManager() = default;

  virtual ~OGLGraphicsManager();

  /***************************************************************************/
  /*
  *  Functions: Init and clears
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Initialize the graphics manager.
  * 
  *  @param Screen& screen
  *  @param bool bAntiliasing
  *  @param SAMPLE_DESC& sample
  */
  void
  initManager(const WPtr<Screen> screen,
              const bool bAntiliasing,
              const SampleDesc& sample) override;

  void
  clearRenderTarget(const WPtr<Texture2D> pTarget,
                    const LinearColor& color) override;

  void
  clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                    const uint32 flags = CLEAR_FLAGS::kDepth,
                    const float depth = 1.0f,
                    const uint8 stencil = 0) override;

  void
  present(uint32 syncInterval = 0, uint32 flags = 0) override;

  void
  unbindAll() override;

  void
  deleteProgram(uint32 programID) override;

  /********************
  *  Getters
  ********************/

  WPtr<Texture2D>
  getMainRenderTargetView() const override;

  WPtr<Texture2D>
  getMainDepthStencil() const override;

  /********************
  *  Creates
  ********************/

  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& desc,
                    const WPtr<VertexShader> pVShader) override;

  SPtr<InputLayout>
  createInputLayoutFromShader(const WPtr<VertexShader> pPShader) override;

  SPtr<InputLayout>
  createInputLayoutFromVertexBuffer(const Vector<InputDesc>& desc,
                                    const WPtr<VertexBuffer> pVBuffer) override;

  SPtr<VertexShader>
  createVertexShader(const Path& filePath,
                     const String& entryPoint,
                     const String& shaderModel,
                     const Vector<ShaderMacro>& macros) override;

  SPtr<PixelShader>
  createPixelShader(const Path& filePath,
                    const String& entryPoint,
                    const String& shaderModel,
                    const Vector<ShaderMacro>& macros) override;

  SPtr<GeometryShader>
  createGeometryShader(const Path& filePath,
                       const String& entryPoint,
                       const String& shaderModel,
                       const Vector<ShaderMacro>& macros) override;

  SPtr<ComputeShader>
  createComputeShader(const Path& filePath,
                      const String& entryPoint,
                      const String& shaderModel,
                      const Vector<ShaderMacro>& macros) override;

  SPtr<VertexBuffer>
  internalCreateVertexBuffer(const void* pData,
                             const uint32 bufferSize,
                             const uint32 stride,
                             const uint32 usage) override;

  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& indices,
                    const uint32 usage) override;

  SPtr<ConstantBuffer>
  createConstantBuffer(const uint32 bufferSize,
                       const uint32 usage,
                       const void* pData) override;

  SPtr<SamplerState>
  createSamplerState(const uint32 filter, const uint32 textAddress) override;

  SPtr<Texture2D>
  createTextureFromFile(const Path& filePath,
                        const void* pData,
                        const uint32 width,
                        const uint32 height,
                        const uint32 bpp) override;

  SPtr<Texture2D>
  createTextureFromDDS(const Path& filePath) override;

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

  uint32
  createProgram(const WPtr<VertexShader>& pVShader,
                const WPtr<PixelShader>& pPShader,
                const WPtr<GeometryShader>& pGShader) override;

  uint32
  createProgram(const WPtr<VertexShader>& pVShader,
                const WPtr<PixelShader>& pPShader) override;

  uint32
  createProgram(const WPtr<ComputeShader>& pCShader) override;

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

  void
  useProgram(uint32 programID) override;

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

  void
  setUniformBuffers(WPtr<ConstantBuffer> pCBuffer, uint32 startSlot);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  SampleDesc m_multiSampleConfig;
  bool m_bFullScreen = false;
  uint32 m_defaultFBO = 0;
  uint32 m_backBufferFBO = 0;
  uint32 m_depthStencilRBO = 0;
  SPtr<Texture2D> m_pBackBuffer;
  SPtr<Texture2D> m_pDepthStencil;
  uint32 m_currentTopology = 0;
  uint32 m_currentIBO = 0;
  uint32 m_frameBufferHeight = 0;
  Viewport m_currentViewport;

#if defined(SH_DEBUG_MODE)
  bool m_debugEnabled = false;
#endif
};
}