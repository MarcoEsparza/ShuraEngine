/*************************************************************/
/*
*  @file    shGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/23
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
#include "shModule.h"
#include "shScreen.h"
#include "shLinearColor.h"

#include "shBuffers.h"
#include "shDepthStencilView.h"
#include "shDevice.h"
#include "shInputLayout.h"
#include "shRenderTargetView.h"
#include "shSamplerState.h"
#include "shShader.h"
#include "shSwapChain.h"
#include "shTexture.h"

#include "shVector2.h"
#include "shVector3.h"
#include "shVector4.h"
#include "shMatrix4.h"

namespace shEngineSDK {
/**
*  @brief Sample descriptor.
*/
typedef struct SAMPLE_DESC
{
  uint32 count = 0;
  uint32 quality = 0;
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
*  @brief Input Layout types enumerator namespace.
*/
namespace shInputLayoutTypes {
enum E
{
  kPOSITION = 0,
  kNORMAL,
  kTEXCOORD,
  kBONEINDICES,
  kBONEWEIGHTS
};
}

/**
*  @brief Vertex struct.
*/
struct VertexData
{
  Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 tex = Vector2(0.0f, 0.0f);
  Vector4 boneIds = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  Vector4 boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

/**
*  @brief View struct.
*/
struct WorldViewProjection
{
  ViewMatrix view;
  ProjectionMatrix proj;
};

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
  *  @param PlatformScreen& srcHandle
  *  @param bool bFullScreen
  *  @param bool bAntiliasing
  *  @param uint32 samplesPerPixel
  *  @param uint32 sampleQuality
  */
  void
  initManager(const PlatformScreen& srcHandle,
              const bool bFullScreen,
              const bool bAntiliasing,
              const uint32 samplesPerPixel,
              const uint32 sampleQuality);

  /**
  *  @brief Clear the render target with given LinearColor.
  *
  *  @param SPtr<RenderTargetView>& pTarget
  *  @param LinearColor& color
  */
  void
  clearRenderTarget(const SPtr<RenderTargetView>& pTarget, LinearColor& color);

  /**
  *  @brief Clear the depth stencil.
  *
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  void
  clearDepthStencil(const SPtr<DepthStencilView>& pDepthSV);

  /**
  *  @brief Present the swapchain.
  */
  void
  present();

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
  SPtr<DepthStencilView>
  getMainDepthStencil() const;

  /**
  *  @brief Returns the Device Context.
  *
  *  @return SPtr<DeviceContext>
  */
  SPtr<DeviceContext>
  getDeviceContext() const;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Creates Input Layout with given types and Vertex Shader.
  *
  *  @param Vector<shInputLayoutTypes::E>& types
  *  @param SPtr<VertexShader>& pVShader
  *
  *  @return SPtr<InputLayout>
  */
  SPtr<InputLayout>
  createInputLayout(const Vector<shInputLayoutTypes::E>& types,
                    const SPtr<VertexShader>& pVShader);

  /**
  *  @brief Creates a Vertex Shader.
  *
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  *
  *  @return SPtr<VertexShader>
  */
  SPtr<VertexShader>
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
  SPtr<PixelShader>
  createPixelShader(const String& fileName,
                    const String& entryPoint,
                    const String& shaderModel);

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
                     const uint32 usage = 0);

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
                    const uint32 usage = 0);

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
                       const uint32 usage = 0,
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
  createSamplerState(const uint32 filter = 21, const uint32 textAddress = 1);

  /**
  *  @brief Creates a Depth Stencil View with given Texture2D.
  *
  *  @param SPtr<Texture2D> pText
  *
  *  @return SPtr<DepthStencilView>
  */
  SPtr<DepthStencilView>
  createDepthSV(const SPtr<Texture2D>& pText);

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
                  const uint32 format = 87,
                  const uint32 usage = 0,
                  const uint32 bindFlags = 8);

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
                   const SPtr<DepthStencilView>& pDepthSV,
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
  setPrimitiveTopology(const uint32 primitive = 4);

  /**
  *  @brief Sets the Vertex Shader.
  *
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances = nullptr
  *  @param uint32 numClassInstances = 0
  */
  void
  setVertexShader(const SPtr<VertexShader>& pVShader,
                  const void* ppClassInstances = nullptr,
                  const uint32 numClassInstances = 0);

  /**
  *  @brief Sets the Pixel Shader.
  *
  *  @param SPtr<PixelShader>& pPShader
  *  @param void* ppClassInstances = nullptr
  *  @param uint32 numClassInstances = 0
  */
  void
  setPixelShader(const SPtr<PixelShader>& pPShader,
                 const void* ppClassInstances = nullptr,
                 const uint32 numClassInstances = 0);

  /**
  *  @brief Sets a shader resource.
  *
  *  @param SPtr<Texture>& pShaderRV
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
  *  @param PlatformScreen& srcHandle
  *  @param bool bFullScreen
  *  @param bool bAntiliasing
  *  @param uint32 samplesPerPixel
  *  @param uint32 sampleQuality
  */
  virtual void
  internalInit(const PlatformScreen& srcHandle,
               const bool bFullScreen,
               const bool bAntiliasing,
               const uint32 samplesPerPixel,
               const uint32 sampleQuality) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<RenderTargetView>& pTarget
  *  @param LinearColor& color
  */
  virtual void
  internalClearRenderTarget(const SPtr<RenderTargetView>& pTarget, LinearColor& color) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<DepthStencilView>& pDepthSV
  */
  virtual void
  internalClearDepthStencil(const SPtr<DepthStencilView>& pDepthSV) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  */
  virtual void
  internalPresent() = 0;

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
  virtual SPtr<DepthStencilView>
  internalGetMainDepthStencil() const = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @return SPtr<DeviceContext>
  */
  virtual SPtr<DeviceContext>
  internalGetDeviceContext() const = 0;

  /********************
  *  Creates
  ********************/

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param Vector<shInputLayoutTypes::E>& types
  *  @param SPtr<VertexShader>& pVShader
  *
  *  @return SPtr<InputLayout>
  */
  virtual SPtr<InputLayout>
  internalCreateInputLayout(const Vector<shInputLayoutTypes::E>& types,
                            const SPtr<VertexShader>& pVShader) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  *
  *  @return SPtr<VertexShader>
  */
  virtual SPtr<VertexShader>
  internalCreateVertexShader(const String& fileName,
                             const String& entryPoint,
                             const String& shaderModel) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param const String& fileName
  *  @param const String& entryPoint
  *  @param const String& shaderModel
  *
  *  @return SPtr<PixelShader>
  */
  virtual SPtr<PixelShader>
  internalCreatePixelShader(const String& fileName,
                            const String& entryPoint,
                            const String& shaderModel) = 0;

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
  *  @param SPtr<Texture2D> pText
  *
  *  @return SPtr<DepthStencilView>
  */
  virtual SPtr<DepthStencilView>
  internalCreateDepthSV(const SPtr<Texture2D>& pText) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param String& fileName
  *
  *  @return SPtr<Texture2D>
  */
  virtual SPtr<Texture2D>
  internalCreateTextureFromFile(const String& fileName) = 0;

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
  *  @param SPtr<DepthStencilView>& pDepthSV
  *  @param uint32 numViews
  */
  virtual void
  internalSetRenderTargets(const SPtr<RenderTargetView>& pRenderTV,
                           const SPtr<DepthStencilView>& pDepthSV,
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
  internalSetVertexShader(const SPtr<VertexShader>& pVShader,
                          const void* ppClassInstances,
                          const uint32 numClassInstances) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<VertexShader>& pVShader
  *  @param void* ppClassInstances
  *  @param uint32 numClassInstances
  */
  virtual void
  internalSetPixelShader(const SPtr<PixelShader>& pPShader,
                         const void* ppClassInstances,
                         const uint32 numClassInstances) = 0;

  /**
  *  @brief Calls the selected API overrided function.
  * 
  *  @param SPtr<Texture>& pShaderRV
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
