/*****************************************************************************/
/*
*  @file    shGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/18
*  @brief   Graphics Manager module that uses function from loaded API.
*
*  Graphics Manager module that uses function from loaded API.
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
#include "shPrerequisitesCore.h"
#include "shGraphicTypes.h"
#include <shModule.h>
#include <shLinearColor.h>
#include <shRect.h>
#include <shVector3.h>
#include <shVector4.h>
#include <shException.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward declarations
*/
/*****************************************************************************/

class Screen;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class InputLayout;
class SamplerState;
class VertexShader;
class PixelShader;
class GeometryShader;
class ComputeShader;
class Texture2D;
class Texture3D;
class BlendState;
class RasterizerState;
class DepthStencilState;

/**
*  @brief Module Graphis Manager.
*/
class SH_CORE_EXPORT GraphicsManager : public Module<GraphicsManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  GraphicsManager() = default;

  /**
  *  @brief Default destructor.
  */
  ~GraphicsManager() = default;

  /***************************************************************************/
  /*
  *  Functions: Init and clears
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Initialize the graphics manager.
  *
  *  @param const WPtr<Screen> screen: Pointer to the screen.
  *  @param const bool bAntiliasing: Is antialiasing activated?
  *  @param const SAMPLE_DESC& sample: Sample description
  */
  virtual void
  initManager(const WPtr<Screen> screen,
              const bool bAntiliasing,
              const SampleDesc& sample) = 0;

  /**
  *  @brief Clear the render target with given LinearColor.
  *
  *  @param const WPtr<Texture2D> pTarget: Texture to clean.
  *  @param const LinearColor& color: Color to clean texture with.
  */
  virtual void
  clearRenderTarget(const WPtr<Texture2D> pTarget,
                    const LinearColor& color) = 0;

  /**
  *  @brief Clear the depth stencil.
  *
  *  @param const WPtr<Texture2D>& pDepthSV
  *  @param const uint32 flags
  */
  virtual void
  clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                    const uint32 flags = CLEAR_FLAGS::kDepth,
                    const float depth = 1.0f,
                    const uint8 stencil = 0) = 0;

  /**
  *  @brief Present the swapchain.
  */
  virtual void
  present(uint32 syncInterval = 0, uint32 flags = 0) = 0;

  virtual void
  unbindAll() = 0;

  virtual void
  deleteProgram(uint32 programID) {
    SH_UNREFERENCED_PARAMETER(programID);
  }

  /***************************************************************************/
  /*
  *  Functions: Getters
  */
  /***************************************************************************/

  /**
  *  @brief Returns the Render Target View.
  *
  *  @return SPtr<RenderTargetView>
  */
  virtual WPtr<Texture2D>
  getMainRenderTargetView() const = 0;

  /**
  *  @brief Returns the Depth Stencil View.
  *
  *  @return SPtr<DepthStencilView>
  */
  virtual WPtr<Texture2D>
  getMainDepthStencil() const = 0;

  GRAPHIC_API::E
  getAPI() const;

  /***************************************************************************/
  /*
  *  Functions: Creates
  */
  /***************************************************************************/

  /**
  *  @brief Creates Input Layout with given types and Vertex Shader.
  *
  *  @param Vector<InputDesc>& desc
  *  @param SPtr<ProgramShader>& pShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& desc,
                    const WPtr<VertexShader> pShader) = 0;

  /**
  *  @brief Creates Input Layout from a VertexShader.
  *
  *  @param SPtr<ProgramShader>& pShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  createInputLayoutFromShader(const WPtr<VertexShader> pShader) = 0;

  /**
  *  @brief Creates a Vertex Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<VertexShader>
  */
  virtual SPtr<VertexShader>
  createVertexShader(const String& fileName,
                     const String& entryPoint,
                     const String& shaderModel,
                     const Vector<ShaderMacro>& macros = {}) = 0;

  /**
  *  @brief Creates a Pixel Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<PixelShader>
  */
  virtual SPtr<PixelShader>
  createPixelShader(const String& fileName,
                    const String& entryPoint,
                    const String& shaderModel,
                    const Vector<ShaderMacro>& macros = {}) = 0;

  /**
  *  @brief Creates a Geometry Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<GeometryShader>
  */
  virtual SPtr<GeometryShader>
  createGeometryShader(const String& fileName,
                       const String& entryPoint,
                       const String& shaderModel,
                       const Vector<ShaderMacro>& macros = {}) = 0;

  /**
  *  @brief Creates a Compute Shader.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<ComputeShader>
  */
  virtual SPtr<ComputeShader>
  createComputeShader(const String& fileName,
                      const String& entryPoint,
                      const String& shaderModel,
                      const Vector<ShaderMacro>& macros = {}) = 0;

  /**
  *  @brief Creates a Vertex Buffer with given vertices.
  *
  *  @param const Vector<T>& vertices: Template vector.
  *  @param const uint32 usage = USAGE::kDefault
  *
  *  @return SPtr<VertexBuffer>: The vertex buffer.
  */
  template<typename T>
  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<T>& vertices,
                     const uint32 usage = USAGE::kDefault)
  {
    return internalCreateVertexBuffer(&vertices[0],
                                      static_cast<uint32>(vertices.size()),
                                      sizeof(T),
                                      usage);
  }

  /**
  *  @brief Creates a Index Buffer with given indices.
  *
  *  @param Vector<int32>& indices
  *  @param uint32 usage = 0
  *
  *  @return SPtr<IndexBuffer>
  */
  virtual SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& indices,
                    const uint32 usage = USAGE::kDefault) = 0;

  /**
  *  @brief Creates a constant buffer with given data.
  *
  *  @param uint32 bufferSize
  *  @param uint32 usage = 0
  *  @param void* pData = nullptr
  *
  *  @return SPtr<ConstantBuffer>
  */
  virtual SPtr<ConstantBuffer>
  createConstantBuffer(const uint32 bufferSize,
                       const uint32 usage = USAGE::kDefault,
                       const void* pData = nullptr) = 0;

  /**
  *  @brief Creates a Sampler State.
  *
  *  @param uint32 filter = SAMPLER_FILTER::kFilterMinMagMipLinear
  *  @param uint32 textAddress = TEXTURE_ADDRESS_MODE::kWrap
  *
  *  @return SPtr<SamplerState>
  */
  virtual SPtr<SamplerState>
  createSamplerState(const uint32 filter = SAMPLER_FILTER::kFilterMinMagMipLinear,
                     const uint32 textAddress = TEXTURE_ADDRESS_MODE::kWrap) = 0;

  /**
  *  @brief Creates a Texture2D from file with given route.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  createTextureFromFile(const String& fileName,
                        const void* pData,
                        const uint32 width,
                        const uint32 height,
                        const uint32 bpp) = 0;

  /**
  *  @brief Creates a Texture2D from a dds file.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  createTextureFromDDS(const String& fileName) = 0;

  /**
  *  @brief Creates a Texture2D.
  *
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = TEXTURE_FORMAT::kR8G8B8A8_UNORM
  *  @param uint32 usage = USAGE::kDefault
  *  @param uint32 bindFlags = BIND_FLAGS::kShaderResource
  *  @param uint32 mipLevels = 1
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  createTexture2D(const uint32 width,
                  const uint32 height,
                  const uint32 format = TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                  const uint32 usage = USAGE::kDefault,
                  const uint32 bindFlags = BIND_FLAGS::kShaderResource,
                  const uint32 mipLevels = 1,
                  const uint32 arraySize = 1)  = 0;

  virtual SPtr<Texture3D>
  createTexture3D(const Vector3 size,
                  const uint32 format = TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                  const Vector<LinearColor>* data = nullptr,
                  const uint32 usage = USAGE::kDefault) = 0;

  /*virtual SPtr<Texture2D>
  createTexture2DArray(const uint32 width,
                       const uint32 height,
                       const uint32 arraySize = 6,
                       const uint32 format = TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                       const uint32 usage = USAGE::kDefault) = 0;*/

  /**
  *  @brief Creates an error Texture2D.
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  createErrorTexture() = 0;

  virtual SPtr<Texture2D>
  createDefaultNormalTexture() = 0;

  virtual SPtr<Texture2D>
  createBlackTexture() = 0;

  /**
  *  @brief Creates a blend state.
  *
  *  @param BlendDesc& blendDesc
  * 
  *  @return SPtr<BlendState>
  */
  virtual SPtr<BlendState>
  createBlendState(const BlendDesc& blendDesc,
                   const LinearColor& blendFactor = LinearColor(0.0f, 0.0f, 0.0f, 0.0f)) = 0;

  /**
  *  @brief Creates a Rasterizer state.
  *
  *  @param RasterizerDesc& blendDesc
  * 
  *  @return SPtr<RasterizerState>
  */
  virtual SPtr<RasterizerState>
  createRasterizerState(const RasterizerDesc& rasterDesc) = 0;

  /**
  *  @brief Creates a Depth Stencil state.
  *
  *  @param DepthStencilDesc& blendDesc
  * 
  *  @return SPtr<DepthStencilState>
  */
  virtual SPtr<DepthStencilState>
  createDepthStencilState(const DepthStencilDesc& depthSDesc) = 0;

  virtual uint32
  createProgram(const WPtr<VertexShader>& pVShader,
                const WPtr<PixelShader>& pPShader,
                const WPtr<GeometryShader>& pGShader) {
    SH_UNREFERENCED_PARAMETER(pVShader);
    SH_UNREFERENCED_PARAMETER(pPShader);
    SH_UNREFERENCED_PARAMETER(pGShader);

    return 0;
  }

  virtual uint32
  createProgram(const WPtr<VertexShader>& pVShader,
                const WPtr<PixelShader>& pPShader) {
    SH_UNREFERENCED_PARAMETER(pVShader);
    SH_UNREFERENCED_PARAMETER(pPShader);

    return 0;
  }

  virtual uint32
  createProgram(const WPtr<ComputeShader>& pCShader) {
    SH_UNREFERENCED_PARAMETER(pCShader);

    return 0;
  }

  /**
  *  @brief GenerateMips for a texture.
  *
  *  @param SPtr<Texture2D>& pTexture
  */
  virtual void
  generateMips(const WPtr<Texture2D> pTexture) = 0;

  /********************
  *  Update
  ********************/

  /**
  *  @brief Update Constant Buffer subresource with given data.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param void* pData
  *  @param uint32 dataSize
  */
  virtual void
  updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                       const void* pData,
                       const uint32 dataSize) = 0;

  /**
  *  @brief Update a given Texture2D with given data.
  *
  *  @param SPtr<Texture2D>& pTexture
  *  @param uint8* pData
  *  @param uint32 width
  *  @param uint32 bpp
  */
  virtual void
  updateTexture2D(WPtr<Texture2D> pTexture,
                  uint8* pData,
                  uint32 width,
                  uint32 bpp) = 0;

  /**
  *  @brief Update the screen size.
  *
  *  @param const Vector2& size
  */
  virtual void
  updateScreenSize(const Vector2& size) = 0;

  virtual void
  updateVertexBuffer(const WPtr<VertexBuffer> pVBuffer,
                     const void* pData,
                     const uint32 dataSize) = 0;

  virtual void
  updateIndexBuffer(const WPtr<IndexBuffer> pIBuffer,
                    const void* pData,
                    const uint32 index) = 0;

  /**
  *  @brief Saves a Texture2D to a dds file.
  * 
  *  @param SPtr<Texture2D>& pTexture
  *  @param String&  filePath
  */
  virtual void
  saveTextureToDDS(const WPtr<Texture2D> pTexture, const String& filePath) = 0;

  /***************************************************************************/
  /*
  *  Functions: Setters
  */
  /***************************************************************************/

  /**
  *  @brief Set the viewport.
  *
  *  @param Viewport& vp
  */
  virtual void
  setViewport(const Viewport& vp) = 0;

  /**
  *  @brief Set the render targets with number of views.
  *
  *  @param Vector<SPtr<RenderTargetView>>& pRenderTVs
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  virtual void
  setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                   const WPtr<Texture2D> pDepthSV) = 0;

  /**
  *  @brief Sets the Input Layout.
  *
  *  @param SPtr<InputLayout>& pInput
  */
  virtual void
  setInputLayout(const WPtr<InputLayout> pInput) = 0;

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  *
  *  @param SPtr<VertexBuffer>& pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  virtual void
  setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                   const uint32 startSlot = 0,
                   const uint32 numBuffers = 1,
                   const uint32 offset = 0) = 0;

  /**
  *  @brief Sets a Index Buffer with given offset.
  *
  *  @param SPtr<IndexBuffer>& pIBuffer
  *  @param uint32 offset = 0
  */
  virtual void
  setIndexBuffers(const WPtr<IndexBuffer> pIBuffer,
                  const uint32 offset = 0) = 0;

  /**
  *  @brief Sets a Constant Buffer for the vertex shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  virtual void
  vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1) = 0;

  /**
  *  @brief Sets a Constant Buffer for the pixel shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  virtual void
  psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1) = 0;

  /**
  *  @brief Sets a Constant Buffer for the geometry shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  virtual void
  gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1) = 0;

  /**
  *  @brief Sets a Constant Buffer for the compute shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  virtual void
  csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1) = 0;

  /**
  *  @brief Sets the primitive topology.
  *
  *  @param uint32 primitive = PRIMITIVE_TOPOLOGY::kTrianglelist
  */
  virtual void
  setPrimitiveTopology(const uint32 primitive = PRIMITIVE_TOPOLOGY::kTrianglelist) = 0;

  /**
  *  @brief Sets the Vertex Shader.
  *
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  setVertexShader(const WPtr<VertexShader> pVShader,
                  const void* ppClassInstances = nullptr,
                  const uint32 numClassInstances = 0) = 0;

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
                 const uint32 numClassInstances = 0) = 0;

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
                    const uint32 numClassInstances = 0) = 0;

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
                   const uint32 numClassInstances = 0) = 0;

  /**
  *  @brief Sets a 2D shader resource to the pixel shader.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  virtual void
  psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) = 0;

  /**
  *  @brief Sets a 2D shader resource to the compute shader.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  virtual void
  csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) = 0;

  /**
  *  @brief Sets a 3D shader resource to the pixel shader.
  *
  *  @param SPtr<Texture3D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  virtual void
  psSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) = 0;

  /**
  *  @brief Sets a 3D shader resource to the compute shader.
  *
  *  @param SPtr<Texture3D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  virtual void
  csSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1) = 0;

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  *  @param uint32* count = nullptr
  */
  virtual void
  setUnorderedAccessView(const UnorderedAccess& pUAVs,
                         const uint32 startSlot = 0) = 0;

  /**
  *  @brief Sets the Sampler State to the pixel shader.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  virtual void
  psSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                    const uint32 startSlot = 0,
                    const uint32 numSamplers = 1) = 0;

  /**
  *  @brief Sets the Sampler State to the compute shader.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  virtual void
  csSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                    const uint32 startSlot = 0,
                    const uint32 numSamplers = 1) = 0;

  /**
  *  @brief Sets the blend state to the device context.
  *
  *  @param SPtr<BlendState>& pBlendState
  */
  virtual void
  setBlendState(const WPtr<BlendState> pBlendState) = 0;

  /**
  *  @brief Sets the rasterizer state to the device context.
  *
  *  @param SPtr<RasterizerState>& pRasterizerState
  */
  virtual void
  setRasterizerState(const WPtr<RasterizerState> pRasterizerState) = 0;

  /**
  *  @brief Sets the depth stencil state to the device context.
  *
  *  @param SPtr<DepthStencilState>& pDepthStencilState
  *  @param uint8 stencilRef = 0
  */
  virtual void
  setDepthStencilState(const WPtr<DepthStencilState> pDepthStencilState,
                       const uint8 stencilRef = 0) = 0;

  /**
  *  @brief Sets the scissor rect.
  *
  *  @param Rect& scissorClip
  */
  virtual void
  setScissorRects(const Rect& scissorClip) = 0;

  virtual void
  useProgram(uint32 programID) {
    SH_UNREFERENCED_PARAMETER(programID);
  }

  /***************************************************************************/
  /*
  *  Functions: Draws
  */
  /***************************************************************************/

  /**
  *  @brief Draw with vertices info.
  *
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  virtual void
  draw(const uint32 vertexCount, const uint32 startVertexLocation) = 0;

  /**
  *  @brief Draw with indices and vertices info.
  *
  *  @param uint32 indexCount
  *  @param uint32 StartVertexLocation
  *  @param uint32 baseVertexLocation
  */
  virtual void
  drawIndexed(const uint32 indexCount,
              const uint32 startIndexLocation,
              const uint32 baseVertexLocation) = 0;

  /**
  *  @brief Dispatch compute shader.
  *
  *  @param uint32 threadGroupCountX
  *  @param uint32 threadGroupCountY
  *  @param uint32 threadGroupCountZ
  */
  virtual void
  dispatch(const uint32 threadGroupCountX,
           const uint32 threadGroupCountY,
           const uint32 threadGroupCountZ) = 0;

  /***************************************************************************/
  /*
  *  Internal functions
  */
  /***************************************************************************/
 protected:
  /**
  *  @copydoc GraphicsManager::createVertexBuffer(type1,type2)
  */
  virtual SPtr<VertexBuffer>
  internalCreateVertexBuffer(const void* pData,
                             const uint32 bufferSize,
                             const uint32 stride,
                             const uint32 usage) = 0;

 protected:
  GRAPHIC_API::E m_graphicAPI = GRAPHIC_API::kDX11;
};

/**
*  @brief Easier way to access the GraphicsManager module.
*/
SH_CORE_EXPORT GraphicsManager&
g_graphicsMan();
}
