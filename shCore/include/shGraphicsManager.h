/*************************************************************/
/*
*  @file    shGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/09
*  @brief   Graphics Manager module that uses function from loaded API.
*
*  Graphics Manager module that uses function from loaded API.
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
#include "shPrerequisitesCore.h"
#include "shGraphicTypes.h"
#include "shModule.h"
#include "shLinearColor.h"
#include "shRect.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Forward declarations
*/
/*************************************************************/

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

  /*************************************************************/
  /*
  *  Public functions
  */
  /*************************************************************/
 public:
  /********************
  *  Init and clears
  ********************/

  /**
  *  @brief Initialize the graphics manager.
  *
  *  @param SPtr<Screen> screen
  *  @param bool bAntiliasing
  *  @param SAMPLE_DESC& sample
  */
  void
  initManager(const WPtr<Screen> screen,
              const bool bAntiliasing,
              const SampleDesc& sample);

  /**
  *  @brief Clear the render target with given LinearColor.
  *
  *  @param SPtr<RenderTargetView>& pTarget
  *  @param LinearColor& color
  */
  void
  clearRenderTarget(const WPtr<Texture2D> pTarget,
                    const LinearColor& color);

  /**
  *  @brief Clear the depth stencil.
  *
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  void
  clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                    uint32 flags = CLEAR_FLAGS::kDepth,
                    float depth = 1.0f,
                    uint8 stencil = 0);

  /**
  *  @brief Present the swapchain.
  */
  void
  present(uint32 syncInterval = 0, uint32 flags = 0);

  /********************
  *  Getters
  ********************/

  /**
  *  @brief Returns the Render Target View.
  *
  *  @return SPtr<RenderTargetView>
  */
  WPtr<Texture2D>
  getMainRenderTargetView() const;

  /**
  *  @brief Returns the Depth Stencil View.
  *
  *  @return SPtr<DepthStencilView>
  */
  WPtr<Texture2D>
  getMainDepthStencil() const;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Creates Input Layout with given types and Vertex Shader.
  *
  *  @param Vector<InputDesc>& desc
  *  @param SPtr<ProgramShader>& pShader
  *
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayout(const Vector<InputDesc>& desc,
                    const WPtr<VertexShader> pShader);

  /**
  *  @brief Creates Input Layout from a VertexShader.
  *
  *  @param SPtr<ProgramShader>& pShader
  *
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayoutFromShader(const WPtr<VertexShader> pShader);

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
  SPtr<VertexShader>
  createVertexShader(const String& fileName,
                     const String& entryPoint,
                     const String& shaderModel,
                     const Vector<ShaderMacro>& macros = {});

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
  SPtr<PixelShader>
  createPixelShader(const String& fileName,
                    const String& entryPoint,
                    const String& shaderModel,
                    const Vector<ShaderMacro>& macros = {});

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
  SPtr<GeometryShader>
  createGeometryShader(const String& fileName,
                       const String& entryPoint,
                       const String& shaderModel,
                       const Vector<ShaderMacro>& macros = {});

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
  SPtr<ComputeShader>
  createComputeShader(const String& fileName,
                      const String& entryPoint,
                      const String& shaderModel,
                      const Vector<ShaderMacro>& macros = {});

  /**
  *  @brief Creates a Vertex Buffer with given vertices.
  *
  *  @param Vector<T>& vertices : Template vector.
  *  @param uint32 usage = 0
  *
  *  @return SPtr<VertexBuffer>
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
  SPtr<IndexBuffer>
  createIndexBuffer(const Vector<uint32>& indices,
                    const uint32 usage = USAGE::kDefault);

  /**
  *  @brief Creates a constant buffer with given data.
  *
  *  @param uint32 bufferSize
  *  @param uint32 usage = 0
  *  @param void* pData = nullptr
  *
  *  @return SPtr<ConstantBuffer>
  */
  SPtr<ConstantBuffer>
  createConstantBuffer(const uint32 bufferSize,
                       const uint32 usage = USAGE::kDefault,
                       const void* pData = nullptr);

  /**
  *  @brief Creates a Sampler State.
  *
  *  @param uint32 filter = SAMPLER_FILTER::kFilterMinMagMipLinear
  *  @param uint32 textAddress = TEXTURE_ADDRESS_MODE::kWrap
  *
  *  @return SPtr<SamplerState>
  */
  SPtr<SamplerState>
  createSamplerState(const uint32 filter = SAMPLER_FILTER::kFilterMinMagMipLinear,
                     const uint32 textAddress = TEXTURE_ADDRESS_MODE::kWrap);

  /**
  *  @brief Creates a Texture2D from file with given route.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTextureFromFile(const String& fileName);

  /**
  *  @brief Creates a Texture2D from a dds file.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTextureFromDDS(const String& fileName);

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
  SPtr<Texture2D>
  createTexture2D(const uint32 width,
                  const uint32 height,
                  const uint32 format = TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                  const uint32 usage = USAGE::kDefault,
                  const uint32 bindFlags = BIND_FLAGS::kShaderResource,
                  const uint32 mipLevels = 1);

  /**
  *  @brief Creates an error Texture2D.
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createErrorTexturre();

  /**
  *  @brief Creates a blend state.
  *
  *  @param BlendDesc& blendDesc
  * 
  *  @return SPtr<BlendState>
  */
  SPtr<BlendState>
  createBlendState(const BlendDesc& blendDesc,
                   const LinearColor& blendFactor = LinearColor(0.0f, 0.0f, 0.0f, 0.0f));

  /**
  *  @brief Creates a Rasterizer state.
  *
  *  @param RasterizerDesc& blendDesc
  * 
  *  @return SPtr<RasterizerState>
  */
  SPtr<RasterizerState>
  createRasterizerState(const RasterizerDesc& rasterDesc);

  /**
  *  @brief Creates a Depth Stencil state.
  *
  *  @param DepthStencilDesc& blendDesc
  * 
  *  @return SPtr<DepthStencilState>
  */
  SPtr<DepthStencilState>
  createDepthStencilState(const DepthStencilDesc& depthSDesc);

  /**
  *  @brief GenerateMips for a texture.
  *
  *  @param SPtr<Texture2D>& pTexture
  */
  void
  generateMips(const WPtr<Texture2D> pTexture);

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
  void
  updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                       const void* pData,
                       const uint32 dataSize);

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
                  uint32 bpp);

  /**
  *  @brief Update the screen size.
  *
  *  @param const Vector2& size
  */
  void
  updateScreenSize(const Vector2& size);

  /**
  *  @brief Saves a Texture2D to a dds file.
  * 
  *  @param SPtr<Texture2D>& pTexture
  *  @param String&  filePath
  */
  void
  saveTextureToDDS(const WPtr<Texture2D> pTexture, const String& filePath);

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Set the viewport.
  *
  *  @param Viewport& vp
  */
  void
  setViewport(const Viewport& vp);

  /**
  *  @brief Set the render targets with number of views.
  *
  *  @param Vector<SPtr<RenderTargetView>>& pRenderTVs
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  void
  setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                   const WPtr<Texture2D> pDepthSV);

  /**
  *  @brief Sets the Input Layout.
  *
  *  @param SPtr<InputLayout>& pInput
  */
  void
  setInputLayout(const WPtr<InputLayout> pInput);

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  *
  *  @param SPtr<VertexBuffer>& pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  void
  setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                   const uint32 startSlot = 0,
                   const uint32 numBuffers = 1,
                   const uint32 offset = 0);

  /**
  *  @brief Sets a Index Buffer with given offset.
  *
  *  @param SPtr<IndexBuffer>& pIBuffer
  *  @param uint32 offset = 0
  */
  void
  setIndexBuffers(const WPtr<IndexBuffer> pIBuffer,
                  const uint32 offset = 0);

  /**
  *  @brief Sets a Constant Buffer for the vertex shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets a Constant Buffer for the pixel shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets a Constant Buffer for the geometry shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets a Constant Buffer for the compute shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets the primitive topology.
  *
  *  @param uint32 primitive = PRIMITIVE_TOPOLOGY::kTrianglelist
  */
  void
  setPrimitiveTopology(const uint32 primitive = PRIMITIVE_TOPOLOGY::kTrianglelist);

  /**
  *  @brief Sets the Vertex Shader.
  *
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  void
  setVertexShader(const WPtr<VertexShader> pVShader,
                  const void* ppClassInstances = nullptr,
                  const uint32 numClassInstances = 0);

  /**
  *  @brief Sets the Pixel Shader.
  *
  *  @param SPtr<PixelShader>& pPShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  void
  setPixelShader(const WPtr<PixelShader> pPShader,
                 const void* ppClassInstances = nullptr,
                 const uint32 numClassInstances = 0);

  /**
  *  @brief Sets the Geometry Shader.
  *
  *  @param SPtr<GeometryShader>& pGShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  void
  setGeometryShader(const WPtr<GeometryShader> pGShader,
                    const void* ppClassInstances = nullptr,
                    const uint32 numClassInstances = 0);

  /**
  *  @brief Sets the Compute Shader.
  *
  *  @param SPtr<ComputeShader>& pCShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  void
  setComputeShader(const WPtr<ComputeShader> pCShader,
                   const void* ppClassInstances = nullptr,
                   const uint32 numClassInstances = 0);

  /**
  *  @brief Sets a shader resource to the pixel shader.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1);

  /**
  *  @brief Sets a shader resource to the compute shader.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                          const uint32 startSlot = 0,
                          const uint32 numViews = 1);

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  *  @param uint32* count = nullptr
  */
  void
  setUnorderedAccessView(const UnorderedAccess& pUAVs,
                         const uint32 startSlot = 0);

  /**
  *  @brief Sets the Sampler State to the pixel shader.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  setPSSamplerState(const WPtr<SamplerState> pSamplerLinear,
                    const uint32 startSlot = 0,
                    const uint32 numSamplers = 1);

  /**
  *  @brief Sets the Sampler State to the compute shader.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  setCSSamplerState(const WPtr<SamplerState> pSamplerLinear,
                    const uint32 startSlot = 0,
                    const uint32 numSamplers = 1);

  /**
  *  @brief Sets the blend state to the device context.
  *
  *  @param SPtr<BlendState>& pBlendState
  */
  void
  setBlendState(const WPtr<BlendState> pBlendState);

  /**
  *  @brief Sets the rasterizer state to the device context.
  *
  *  @param SPtr<RasterizerState>& pRasterizerState
  */
  void
  setRasterizerState(const WPtr<RasterizerState> pRasterizerState);

  /**
  *  @brief Sets the depth stencil state to the device context.
  *
  *  @param SPtr<DepthStencilState>& pDepthStencilState
  *  @param uint8 stencilRef = 0
  */
  void
  setDepthStencilState(const WPtr<DepthStencilState> pDepthStencilState,
                       const uint8 stencilRef = 0);

  /**
  *  @brief Sets the scissor rect.
  *
  *  @param Rect& scissorClip
  */
  void
  setScissorRects(const Rect& scissorClip);

  /**
  *  @brief Draw with vertices info.
  *
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  void
  draw(const uint32 vertexCount, const uint32 startVertexLocation) ;

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
              const uint32 baseVertexLocation);

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
           const uint32 threadGroupCountZ);

  /*************************************************************/
  /*
  *  Internal functions
  */
  /*************************************************************/
 protected:
  /********************
  *  Init and clears
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param WPtr<Screen> screen
  *  @param bool bAntiliasing
  *  @param SAMPLE_DESC& sample
  */
  virtual void
  internalInit(const WPtr<Screen> screen,
               const bool bAntiliasing,
               const SampleDesc& sample) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param WPtr<Texture2D> pTarget
  *  @param LinearColor& color
  */
  virtual void
  internalClearRenderTarget(const WPtr<Texture2D> pTarget,
                            const LinearColor& color) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<Texture2D>& pDepthSV
  */
  virtual void
  internalClearDepthStencil(const WPtr<Texture2D> pDepthSV,
                            uint32 flags,
                            float depth,
                            uint8 stencil) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  */
  virtual void
  internalPresent(uint32 syncInterval, uint32 flags) = 0;

  /********************
  *  Getters
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @return SPtr<RenderTargetView>
  */
  virtual WPtr<Texture2D>
  internalGetMainRenderTargetView() const = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @return SPtr<DepthStencilView>
  */
  virtual WPtr<Texture2D>
  internalGetMainDepthStencil() const = 0;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<InputDesc>& desc
  *  @param SPtr<ProgramShader>& pPShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  internalCreateInputLayout(const Vector<InputDesc>& desc,
                            const WPtr<VertexShader> pPShader) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ProgramShader>& pPShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  internalCreateInputLayoutFromShader(const WPtr<VertexShader> pPShader) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<VertexShader>
  */
  virtual SPtr<VertexShader>
  internalCreateVertexShader(const String& fileName,
                             const String& entryPoint,
                             const String& shaderModel,
                             const Vector<ShaderMacro>& macros) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<PixelShader>
  */
  virtual SPtr<PixelShader>
  internalCreatePixelShader(const String& fileName,
                            const String& entryPoint,
                            const String& shaderModel,
                            const Vector<ShaderMacro>& macros) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<GeometryShader>
  */
  virtual SPtr<GeometryShader>
  internalCreateGeometryShader(const String& fileName,
                               const String& entryPoint,
                               const String& shaderModel,
                               const Vector<ShaderMacro>& macros) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param String& fileName
  *  @param String& entryPoint
  *  @param String& shaderModel
  *  @param Vector<ShaderMacro>& macros
  *
  *  @return SPtr<ComputeShader>
  */
  virtual SPtr<ComputeShader>
  internalCreateComputeShader(const String& fileName,
                              const String& entryPoint,
                              const String& shaderModel,
                              const Vector<ShaderMacro>& macros) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<VertexData>& vertices
  *  @param uint32 usage
  *
  *  @return SPtr<VertexBuffer>
  */
  virtual SPtr<VertexBuffer>
  internalCreateVertexBuffer(const void* pData,
                             const uint32 bufferSize,
                             const uint32 stride,
                             const uint32 usage) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<int32>& indices
  *  @param uint32 usage
  *
  *  @return SPtr<IndexBuffer>
  */
  virtual SPtr<IndexBuffer>
  internalCreateIndexBuffer(const Vector<uint32>& indices,
                            const uint32 usage) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 bufferSize
  *  @param uint32 usage
  *  @param void* pData
  *
  *  @return SPtr<ConstantBuffer>
  */
  virtual SPtr<ConstantBuffer>
  internalCreateConstantBuffer(const uint32 bufferSize,
                               const uint32 usage,
                               const void* pData) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 filter
  *  @param uint32 textAddress
  *
  *  @return SPtr<SamplerState>
  */
  virtual SPtr<SamplerState>
  internalCreateSamplerState(const uint32 filter, const uint32 textAddress) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateTextureFromFile(const void* pData,
                                const int32 width,
                                const int32 height,
                                const int32 bpp) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateTextureFromDDS(const String& fileName) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = 87
  *  @param uint32 usage = 0
  *  @param uint32 bindFlags = 8
  *  @param uint32 mipLevels = 1
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateTexture2D(const uint32 width,
                          const uint32 height,
                          const uint32 format,
                          const uint32 usage,
                          const uint32 bindFlags,
                          const uint32 mipLevels) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateErrorTexture() = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param BlendDesc& blendDesc
  * 
  *  @return SPtr<BlendState>
  */
  virtual SPtr<BlendState>
  internalCreateBlendState(const BlendDesc& blendDesc, const LinearColor& blendFactor) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param RasterizerDesc& RasterizerDesc
  * 
  *  @return SPtr<RasterizerState>
  */
  virtual SPtr<RasterizerState>
  internalCreateRasterizerState(const RasterizerDesc& rasterizerDesc) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param DepthStencilDesc& rasterizerDesc
  * 
  *  @return SPtr<DepthStencilState>
  */
  virtual SPtr<DepthStencilState>
  internalCreateDepthStencilState(const DepthStencilDesc& rasterizerDesc) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<Texture2D>& pTexture
  */
  virtual void
  internalGenerateMips(const WPtr<Texture2D> pTexture) = 0;

  /********************
  *  Update
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param void* pData
  *  @param uint32 dataSize
  */
  virtual void
  internalUpdateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                               const void* pData,
                               const uint32 dataSize) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<Texture2D>& pTexture
  *  @param uint8* pData
  *  @param uint32 width
  *  @param uint32 bpp
  */
  virtual void
  internalUpdateTexture2D(WPtr<Texture2D> pTexture,
                          uint8* pData,
                          uint32 width,
                          uint32 bpp) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<Screen>& pScreen
  */
  virtual void
  internalUpdateScreenSize(const Vector2& size) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<Texture2D>& pTexture
  *  @param String& filePath
  */
  virtual void
  internalSaveTextureToDDS(const WPtr<Texture2D> pTexture, const String& filePath) = 0;

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param Viewport& vp
  */
  virtual void
  internalSetViewport(const Viewport& vp) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<SPtr<RenderTargetView>>& pRenderTVs
  *  @param SPtr<Texture2D>& pDepthSV
  *  @param uint32 numViews
  */
  virtual void
  internalSetRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                           const WPtr<Texture2D> pDepthSV) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<InputLayout>& pInput
  */
  virtual void
  internalSetInputLayout(const WPtr<InputLayout> pInput) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<VertexBuffer>& pVBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  *  @param uint32 offset
  */
  virtual void
  internalSetVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                           const uint32 startSlot,
                           const uint32 numBuffers,
                           const uint32 offset) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<IndexBuffer>& pIBuffer
  *  @param uint32 offset
  */
  virtual void
  internalSetIndexBuffers(const WPtr<IndexBuffer> pIBuffer,
                          const uint32 offset) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  */
  virtual void
  internalVSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  */
  virtual void
  internalPSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  */
  virtual void
  internalGSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  */
  virtual void
  internalCSSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 primitive = 4
  */
  virtual void
  internalSetPrimitiveTopology(const uint32 primitive) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances
  *  @param uint32 numClassInstances
  */
  virtual void
  internalSetVertexShader(const WPtr<VertexShader> pVShader,
                           const void* ppClassInstances,
                           const uint32 numClassInstances) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<PixelShader>& pPShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  internalSetPixelShader(const WPtr<PixelShader> pPShader,
                         const void* ppClassInstances = nullptr,
                         const uint32 numClassInstances = 0) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<GeometryShader>& pGShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  internalSetGeometryShader(const WPtr<GeometryShader> pGShader,
                            const void* ppClassInstances = nullptr,
                            const uint32 numClassInstances = 0) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<ComputeShader>& pCShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  virtual void
  internalSetComputeShader(const WPtr<ComputeShader> pCShader,
                           const void* ppClassInstances = nullptr,
                           const uint32 numClassInstances = 0) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  */
  virtual void
  internalPSSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                  const uint32 startSlot,
                                  const uint32 numViews) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  */
  virtual void
  internalCSSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                  const uint32 startSlot,
                                  const uint32 numViews) = 0;

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  *  @param uint32* count
  */
  virtual void
  internalSetUnorderedAccessView(const UnorderedAccess& pUAVs,
                                 const uint32 startSlot) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot
  *  @param uint32 numSamplers
  */
  virtual void
  internalPSSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                            const uint32 startSlot,
                            const uint32 numSamplers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot
  *  @param uint32 numSamplers
  */
  virtual void
  internalCSSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                            const uint32 startSlot,
                            const uint32 numSamplers) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<BlendState>& pBlendState
  *  @param Vector4& blendFactor
  */
  virtual void
  internalSetBlendState(const WPtr<BlendState> pBlendState) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<RasterizerState>& pRasterizerState
  */
  virtual void
  internalSetRasterizerState(const WPtr<RasterizerState> pRasterizerState) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param SPtr<DepthStencilState>& pDepthStencilState
  *  @param uint8 stencilRef
  */
  virtual void
  internalSetDepthStencilState(const WPtr<DepthStencilState> pDepthStencilState,
                               const uint8 stencilRef) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param Rect& scissorClip
  */
  virtual void
  internalSetScissorRects(const Rect& scissorClip) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 vertexCount
  *  @param uint32 startVertexLocation
  */
  virtual void
  internalDraw(const uint32 vertexCount, const uint32 startVertexLocation) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 indexCount
  *  @param uint32 StartVertexLocation
  *  @param uint32 baseVertexLocation
  */
  virtual void
  internalDrawIndexed(const uint32 indexCount,
                      const uint32 startIndexLocation,
                      const uint32 baseVertexLocation) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  *
  *  @param uint32 threadGroupCountX
  *  @param uint32 threadGroupCountY
  *  @param uint32 threadGroupCountZ
  */
  virtual void
  internalDispatch(const uint32 threadGroupCountX,
                   const uint32 threadGroupCountY,
                   const uint32 threadGroupCountZ) = 0;
};

/**
*  @brief Easier way to access the GraphicsManager module.
*/
SH_CORE_EXPORT GraphicsManager&
g_graphicsMan();
}
