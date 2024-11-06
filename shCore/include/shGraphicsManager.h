/*************************************************************/
/*
*  @file    shGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/02
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

namespace shEngineSDK {
/*************************************************************/
/*
*  Forward declarations
*/
/*************************************************************/

class LinearColor;
class Screen;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class InputLayout;
class RenderTargetView;
class SamplerState;
class ProgramShader;
class Texture2D;

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
  initManager(const SPtr<Screen> screen,
              const bool bAntiliasing,
              const SampleDesc& sample);

  /**
  *  @brief Clear the render target with given LinearColor.
  *
  *  @param SPtr<RenderTargetView>& pTarget
  *  @param LinearColor& color
  */
  void
  clearRenderTarget(const SPtr<RenderTargetView>& pTarget,
                    const LinearColor& color);

  /**
  *  @brief Clear the depth stencil.
  *
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  void
  clearDepthStencil(const SPtr<Texture2D>& pDepthSV,
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
  SPtr<RenderTargetView>
  getMainRenderTargetView() const;

  /**
  *  @brief Returns the Depth Stencil View.
  *
  *  @return SPtr<DepthStencilView>
  */
  SPtr<Texture2D>
  getMainDepthStencil() const;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Creates Input Layout with given types and Vertex Shader.
  *
  *  @param Vector<shInputLayoutTypes::E>& types
  *  @param SPtr<ProgramShader>& pShader
  *
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayout(const Vector<INPUT_LAYOUT_TYPES::E>& types,
                    const SPtr<ProgramShader>& pShader);

  /**
  *  @brief Creates a Program Shader.
  *
  *  @param const String& fileName
  *  @param const String& vsEntryPoint
  *  @param const String& psEntryPoint
  *  @param const String& vsShaderModel
  *  @param const String& psShaderModel
  *
  *  @return SPtr<ProgramShader>
  */
  SPtr<ProgramShader>
  createProgramShader(const String& fileName,
                      const String& vsEntryPoint,
                      const String& psEntryPoint,
                      const String& vsShaderModel,
                      const String& psShaderModel);

  /**
  *  @brief Creates a Vertex Buffer with given vertices.
  *
  *  @param Vector<VertexData>& vertices
  *  @param uint32 usage = 0
  *
  *  @return SPtr<VertexBuffer>
  */
  SPtr<VertexBuffer>
  createVertexBuffer(const Vector<VertexData>& vertices,
                     const uint32 usage = USAGE::kDefault);

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
  *  @param uint32 filter = 21
  *  @param uint32 textAddress = 1
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
  *  @brief Creates a Texture2D.
  *
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = 87
  *  @param uint32 usage = 0
  *  @param uint32 bindFlags = 8
  *
  *  @return SPtr<Texture2D>
  */
  SPtr<Texture2D>
  createTexture2D(const uint32 width,
                  const uint32 height,
                  const uint32 format = TEXTURE_FORMAT::kR8G8B8A8_unorm,
                  const uint32 usage = USAGE::kDefault,
                  const uint32 bindFlags = BIND_FLAGS::kShaderResource);

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
  updateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
                       const void* pData,
                       const uint32 dataSize);

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Set the render targets with number of views.
  *
  *  @param SPtr<RenderTargetView>& pRenderTV
  *  @param SPtr<DepthStencilView>& pDepthSV
  *  @param uint32 numViews
  */
  void
  setRenderTargets(const SPtr<RenderTargetView>& pRenderTV,
                   const SPtr<Texture2D>& pDepthSV,
                   const uint32 numViews);

  /**
  *  @brief Sets the Input Layout.
  *
  *  @param SPtr<InputLayout>& pInput
  */
  void
  setInputLayout(const SPtr<InputLayout>& pInput);

  /**
  *  @brief Sets a Vertex Buffer with given start slot, number of buffers and its offset.
  *
  *  @param SPtr<VertexBuffer>& pVBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  *  @param uint32 offset = 0
  */
  void
  setVertexBuffers(const SPtr<VertexBuffer>& pVBuffer,
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
  setIndexBuffers(const SPtr<IndexBuffer>& pIBuffer,
                  const uint32 offset = 0);

  /**
  *  @brief Sets a Constant Buffer for  the vertex shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  vsSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets a Constant Buffer for  the pixel shader with given start slot
  *         and number of buffers.
  *
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot = 0
  *  @param uint32 numBuffers = 1
  */
  void
  psSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                       const uint32 startSlot = 0,
                       const uint32 numBuffers = 1);

  /**
  *  @brief Sets the primitive topology.
  *
  *  @param uint32 primitive = 4
  */
  void
  setPrimitiveTopology(const uint32 primitive = PRIMITIVE_TOPOLOGY::kTrianglelist);

  /**
  *  @brief Sets the Vertex Shader.
  *
  *  @param SPtr<ProgramShader>& pVShader
  *  @param void* ppClassInstances = nullptr This interface encapsulates an HLSL class.
  *  @param uint32 numClassInstances = 0
  */
  void
  setProgramShader(const SPtr<ProgramShader>& pVShader,
                  const void* ppClassInstances = nullptr,
                  const uint32 numClassInstances = 0);

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot = 0
  *  @param uint32 numViews = 1
  */
  void
  setShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                        const uint32 startSlot = 0,
                        const uint32 numViews = 1);

  /**
  *  @brief Sets the Sampler State.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot = 0
  *  @param uint32 numSamplers = 1
  */
  void
  setSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                  const uint32 startSlot = 0,
                  const uint32 numSamplers = 1);

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
  *  @param SPtr<Screen> screen
  *  @param bool bAntiliasing
  *  @param SAMPLE_DESC& sample
  */
  virtual void
  internalInit(const SPtr<Screen> screen,
               const bool bAntiliasing,
               const SampleDesc& sample) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<RenderTargetView>& pTarget
  *  @param LinearColor& color
  */
  virtual void
  internalClearRenderTarget(const SPtr<RenderTargetView>& pTarget,
                            const LinearColor& color) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<Texture2D>& pDepthSV
  */
  virtual void
  internalClearDepthStencil(const SPtr<Texture2D>& pDepthSV,
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
  virtual SPtr<RenderTargetView>
  internalGetMainRenderTargetView() const = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @return SPtr<DepthStencilView>
  */
  virtual SPtr<Texture2D>
  internalGetMainDepthStencil() const = 0;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<shInputLayoutTypes::E>& types
  *  @param SPtr<ProgramShader>& pVShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  internalCreateInputLayout(const Vector<INPUT_LAYOUT_TYPES::E>& types,
                            const SPtr<ProgramShader>& pPShader) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param const String& fileName
  *  @param const String& vsEntryPoint
  *  @param const String& psEntryPoint
  *  @param const String& vsShaderModel
  *  @param const String& psShaderModel
  *
  *  @return SPtr<ProgramShader>
  */
  virtual SPtr<ProgramShader>
  internalCreateProgramShader(const String& fileName,
                              const String& vsEntryPoint,
                              const String& psEntryPoint,
                              const String& vsShaderModel,
                              const String& psShaderModel) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<VertexData>& vertices
  *  @param uint32 usage
  *
  *  @return SPtr<VertexBuffer>
  */
  virtual SPtr<VertexBuffer>
  internalCreateVertexBuffer(const Vector<VertexData>& vertices,
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
  internalCreateTextureFromFile(const uint8* pData,
                                const int32 width,
                                const int32 height,
                                const int32 bpp) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param uint32 width
  *  @param uint32 height
  *  @param uint32 format = 87
  *  @param uint32 usage = 0
  *  @param uint32 bindFlags = 8
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateTexture2D(const uint32 width,
                          const uint32 height,
                          const uint32 format,
                          const uint32 usage,
                          const uint32 bindFlags) = 0;

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
  internalUpdateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
                               const void* pData,
                               const uint32 dataSize) = 0;

  /********************
  *  Setters
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<RenderTargetView>& pRenderTV
  *  @param SPtr<Texture2D>& pDepthSV
  *  @param uint32 numViews
  */
  virtual void
  internalSetRenderTargets(const SPtr<RenderTargetView>& pRenderTV,
                           const SPtr<Texture2D>& pDepthSV,
                           const uint32 numViews) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<InputLayout>& pInput
  */
  virtual void
  internalSetInputLayout(const SPtr<InputLayout>& pInput) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<VertexBuffer>& pVBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  *  @param uint32 offset
  */
  virtual void
  internalSetVertexBuffers(const SPtr<VertexBuffer>& pVBuffer,
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
  internalSetIndexBuffers(const SPtr<IndexBuffer>& pIBuffer,
                          const uint32 offset) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<ConstantBuffer>& pCBuffer
  *  @param uint32 startSlot
  *  @param uint32 numBuffers
  */
  virtual void
  internalVSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
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
  internalPSSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
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
  internalSetProgramShader(const SPtr<ProgramShader>& pVShader,
                           const void* ppClassInstances,
                           const uint32 numClassInstances) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<Texture2D>& pShaderRV
  *  @param uint32 startSlot
  *  @param uint32 numViews
  */
  virtual void
  internalSetShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                                const uint32 startSlot,
                                const uint32 numViews) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<SamplerState>& pSamplerLinear
  *  @param uint32 startSlot
  *  @param uint32 numSamplers
  */
  virtual void
  internalSetSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                          const uint32 startSlot,
                          const uint32 numSamplers) = 0;

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
};
}
