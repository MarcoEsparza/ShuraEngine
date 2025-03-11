/*****************************************************************************/
/*
*  @file    shDX11GraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/07
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
#include "shGraphicsManager.h"

#include "shDX11Buffers.h"
#include "shDX11Device.h"
#include "shDX11InputLayout.h"
#include "shDX11Shader.h"
#include "shDX11SwapChain.h"
#include "shDX11Texture.h"
#include "shDX11SamplerState.h"
#include "shDX11RasterizerState.h"
#include "shDX11BlendState.h"
#include "shDX11DepthStencilState.h"

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
  ~DX11GraphicsManager() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /********************
  *  Init and clears
  ********************/

  /**
  *  @brief Initialize the graphics manager.
  * 
  *  @param Screen& screen
  *  @param bool bAntiliasing
  *  @param SAMPLE_DESC& sample
  */
  void
  internalInit(const SPtr<Screen> screen,
               const bool bAntiliasing,
               const SampleDesc& sample) override;

  /**
  *  @brief Clear the render target with given LinearColor.
  * 
  *  @param SPtr<RenderTargetView> pTarget
  *  @param LinearColor& color
  */
  void
  internalClearRenderTarget(const SPtr<Texture2D>& pTarget,
                            const LinearColor& color) override;

  /**
  *  @brief Clear the depth stencil.
  * 
  *  @param SPtr<DepthStencilView> pDepthSV
  */
  void
  internalClearDepthStencil(const SPtr<Texture2D>& pDepthSV,
                            uint32 flags,
                            float depth,
                            uint8 stencil) override;

  /**
  *  @brief Present the swapchain.
  */
  void
  internalPresent(uint32 syncInterval, uint32 flags) override;

  /********************
  *  Getters
  ********************/

  /**
  *  @brief Returns the Render Target View.
  * 
  *  @return SPtr<RenderTargetView>
  */
  SPtr<Texture2D>
  internalGetMainRenderTargetView() const override;

  /**
  *  @brief Returns the Depth Stencil View.
  * 
  *  @return SPtr<DepthStencilView>
  */
  SPtr<Texture2D>
  internalGetMainDepthStencil() const override;

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
  virtual SPtr<InputLayout>
  internalCreateInputLayout(const Vector<InputDesc>& desc,
                            const SPtr<ProgramShader>& pVShader) override;

  /**
  *  @brief Creates Input Layout from VertexShader.
  *
  *  @param SPtr<ProgramShader> pVShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  internalCreateInputLayoutFromShader(const SPtr<ProgramShader>& pPShader) override;

  /**
  *  @brief Creates a Vertex Shader.
  * 
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  * 
  *  @return SPtr<VertexShader>
  */
  virtual SPtr<ProgramShader>
  internalCreateProgramShader(const String& fileName,
                              const String& vsEntryPoint,
                              const String& psEntryPoint,
                              const String& vsShaderModel,
                              const String& psShaderModel) override;

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
  internalCreateIndexBuffer(const Vector<uint32>& indices,
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
  internalCreateConstantBuffer(const uint32 bufferSize,
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
  internalCreateSamplerState(const uint32 filter, const uint32 textAddress) override;

  /**
  *  @brief Creates a Texture2D from file with given route.
  * 
  *  @param String& fileName
  * 
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  internalCreateTextureFromFile(const uint8* pData,
                                const int32 width,
                                const int32 height,
                                const int32 bpp) override;

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
  internalCreateTexture2D(const uint32 width,
                          const uint32 height,
                          const uint32 format,
                          const uint32 usage,
                          const uint32 bindFlags) override;

  /**
  *  @brief Creates a blend state.
  *
  *  @param BlendDesc& blendDesc
  * 
  *  @return SPtr<BlendState>
  */
  SPtr<BlendState>
  internalCreateBlendState(const BlendDesc& blendDesc,
                           const LinearColor& blendFactor) override;

  /**
  *  @brief Creates a rasterizer state.
  *
  *  @param RasterizerDesc& RasterizerDesc
  * 
  *  @return SPtr<RasterizerState>
  */
  SPtr<RasterizerState>
  internalCreateRasterizerState(const RasterizerDesc& rasterizerDesc) override;

  /**
  *  @brief Creates a depth stencil state.
  *
  *  @param DepthStencilDesc& depthSDesc
  * 
  *  @return SPtr<DepthStencilState>
  */
  SPtr<DepthStencilState>
  internalCreateDepthStencilState(const DepthStencilDesc& depthSDesc) override;

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
  internalUpdateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
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
  internalUpdateTexture2D(SPtr<Texture2D>& pTexture,
                          uint8* pData,
                          uint32 width,
                          uint32 bpp) override;

  /**
  *  @brief Update the screen size.
  *
  *  @param SPtr<Screen>& pScreen
  */
  virtual void
  internalUpdateScreenSize(const SPtr<Screen>& pScreen) override;

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Set the viewport.
  *
  *  @param Viewport& vp
  */
  void
  internalSetViewport(const Viewport& vp) override;

  /**
  *  @brief Set the render targets with number of views.
  * 
  *  @param Vector<SPtr<RenderTargetView>>& pRenderTVs
  *  @param SPtr<Texture2D> pDepthSV
  *  @param uint32 numViews
  */
  void
  internalSetRenderTargets(const Vector<SPtr<Texture2D>>& pRenderTVs,
                           const SPtr<Texture2D>& pDepthSV) override;

  /**
  *  @brief Sets the Input Layout.
  * 
  *  @param SPtr<InputLayout> pInput
  */
  void
  internalSetInputLayout(const SPtr<InputLayout>& pInput) override;

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  * 
  *  @param SPtr<VertexBuffer> pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  void
  internalSetVertexBuffers(const SPtr<VertexBuffer>& pVBuffer,
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
  internalSetIndexBuffers(const SPtr<IndexBuffer>& pIBuffer,
                          const uint32 offset) override;

  /**
  *  @brief Sets a Constant Buffer for  the vertex shader with given start slot
  *         and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  internalVSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) override;

  /**
  *  @brief Sets a Constant Buffer for  the pixel shader with given start slot
            and number of buffers.
  * 
  *  @param SPtr<ConstantBuffer> pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  internalPSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) override;

  /**
  *  @brief Sets the primitive topology.
  * 
  *  @param uint32 primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST 4
  */
  void
  internalSetPrimitiveTopology(const uint32 primitive) override;
   
  /**
  *  @brief Sets the Program Shader.
  * 
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances = nullptr
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  internalSetProgramShader(const SPtr<ProgramShader>& pPShader,
                           const void* ppClassInstances,
                           const uint32 numClassInstances) override;

  /**
  *  @brief Sets a shader resource.
  * 
  *  @param SPtr<Texture> pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  internalSetShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                                const uint32 startSlot,
                                const uint32 numViews) override;

  /**
  *  @brief Sets the Sampler State.
  * 
  *  @param SPtr<SamplerState> pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  internalSetSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                          const uint32 startSlot,
                          const uint32 numSamplers) override;

  /**
  *  @brief Sets the blend state to the device context.
  *
  *  @param SPtr<BlendState>& pBlendState
  *  @param Vector4& blendFactor
  */
  void
  internalSetBlendState(const SPtr<BlendState>& pBlendState) override;

  /**
  *  @brief Sets the rasterizer state to the device context.
  *
  *  @param SPtr<RasterizerState>& pRasterizerState
  */
  void
  internalSetRasterizerState(const SPtr<RasterizerState>& pRasterizerState) override;

  /**
  *  @brief Sets the depth stencil state to the device context.
  *
  *  @param SPtr<DepthStencilState>& pDepthSState
  *  @param uint8 stencilRef
  */
  void
  internalSetDepthStencilState(const SPtr<DepthStencilState>& pDepthSState,
                               const uint8 stencilRef) override;

  /**
  *  @brief Sets the scissor rect.
  *
  *  @param Rect& scissorClip
  */
  void
  internalSetScissorRects(const Rect& scissorClip) override;

  /**
  *  @brief Draw with vertices info.
  * 
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  void
  internalDraw(const uint32 vertexCount, const uint32 startVertexLocation) override;

  /**
  *  @brief Draw with indices and vertices info.
  * 
  *  @param uint32 indexCount
  *  @param uint32 StartVertexLocation
  *  @param uint32 baseVertexLocation
  */
  void
  internalDrawIndexed(const uint32 indexCount,
                      const uint32 startIndexLocation,
                      const uint32 baseVertexLocation) override;

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
  SPtr<DX11Device> m_pDevice;

  /**
  *  @brief GPU Device Context.
  */
  SPtr<DX11DeviceContext> m_pDeviceContext;

  /**
  *  @brief The SwapChain for the front and back buffers.
  */
  SPtr<DX11SwapChain> m_pSwapChain;

  /**
  *  @brief The back buffer.
  */
  SPtr<DX11Texture2D> m_pBackbuffer;

  /**
  *  @brief The main Depth Stencil.
  */
  SPtr<DX11Texture2D> m_pDepthStencil;

  /**
  *  @brief The main Render Target View.
  */
  SPtr<DX11Texture2D> m_pRenderTargetView;
};
}
