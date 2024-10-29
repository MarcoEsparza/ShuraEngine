/*************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
*  @brief   Graphics Manager module that uses function from loaded API.
*
*  Graphics Manager module that uses function from loaded API.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shGraphicsManager.h"

namespace shEngineSDK {
void
GraphicsManager::initManager(const Screen& screen,
                             const bool bAntiliasing,
                             const SAMPLE_DESC& sample)
{
  internalInit(screen, bAntiliasing, sample);
}

void
GraphicsManager::clearRenderTarget(const SPtr<RenderTargetView>& pTarget, LinearColor& color)
{
  internalClearRenderTarget(pTarget, color);
}

void
GraphicsManager::clearDepthStencil(const SPtr<Texture2D>& pDepthSV)
{
  internalClearDepthStencil(pDepthSV);
}

void
GraphicsManager::present()
{
  internalPresent();
}

SPtr<RenderTargetView>
GraphicsManager::getMainRenderTargetView() const
{
  return internalGetMainRenderTargetView();
}

SPtr<Texture2D>
GraphicsManager::getMainDepthStencil() const
{
  return internalGetMainDepthStencil();
}

SPtr<InputLayout>
GraphicsManager::createInputLayout(const Vector<shINPUT_LAYOUT_TYPES::E>& types,
                                   const SPtr<ProgramShader>& pShader)
{
  return internalCreateInputLayout(types, pShader);
}

SPtr<ProgramShader>
GraphicsManager::createProgramShader(const String& fileName,
                                     const String& vsEntryPoint,
                                     const String& psEntryPoint,
                                     const String& vsShaderModel,
                                     const String& psShaderModel)
{
  return internalCreateProgramShader(fileName,
                                     vsEntryPoint,
                                     psEntryPoint,
                                     vsShaderModel,
                                     psShaderModel);
}

SPtr<VertexBuffer>
GraphicsManager::createVertexBuffer(const Vector<VertexData>& vertices, const uint32 usage)
{
  return internalCreateVertexBuffer(vertices, usage);
}

SPtr<IndexBuffer>
GraphicsManager::createIndexBuffer(const Vector<uint32>& indices, const uint32 usage)
{
  return internalCreateIndexBuffer(indices, usage);
}

SPtr<ConstantBuffer>
GraphicsManager::createConstantBuffer(const uint32 bufferSize,
                                      const uint32 usage,
                                      const void* pData)
{
  return internalCreateConstantBuffer(bufferSize, usage, pData);
}

SPtr<SamplerState>
GraphicsManager::createSamplerState(const uint32 filter, const uint32 textAddress)
{
  return internalCreateSamplerState(filter, textAddress);
}

SPtr<Texture2D>
GraphicsManager::createTextureFromFile(const String& fileName)
{
  return internalCreateTextureFromFile(fileName);
}

SPtr<Texture2D>
GraphicsManager::createTexture2D(const uint32 width,
                                 const uint32 height,
                                 const uint32 format,
                                 const uint32 usage,
                                 const uint32 bindFlags)
{
  return internalCreateTexture2D(width, height, format, usage, bindFlags);
}

void
GraphicsManager::updateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
                                      const void* pData,
                                      const uint32 dataSize)
{
  internalUpdateConstantBuffer(pCBuffer, pData, dataSize);
}

void
GraphicsManager::setRenderTargets(const SPtr<RenderTargetView>& pRenderTV,
                                  const SPtr<Texture2D>& pDepthSV,
                                  const uint32 numViews)
{
  internalSetRenderTargets(pRenderTV, pDepthSV, numViews);
}

void
GraphicsManager::setInputLayout(const SPtr<InputLayout>& pInput)
{
  internalSetInputLayout(pInput);
}

void
GraphicsManager::setVertexBuffers(const SPtr<VertexBuffer>& pVBuffer,
                                  const uint32 startSlot,
                                  const uint32 numBuffers,
                                  const uint32 offset)
{
  internalSetVertexBuffers(pVBuffer, startSlot, numBuffers, offset);
}

void
GraphicsManager::setIndexBuffers(const SPtr<IndexBuffer>& pIBuffer, const uint32 offset)
{
  internalSetIndexBuffers(pIBuffer, offset);
}

void
GraphicsManager::vsSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalVSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::psSetConstantBuffers(const SPtr<ConstantBuffer>& pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalPSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::setPrimitiveTopology(const uint32 primitive)
{
  internalSetPrimitiveTopology(primitive);
}

void
GraphicsManager::setProgramShader(const SPtr<ProgramShader>& pVShader,
                                 const void* ppClassInstances,
                                 const uint32 numClassInstances)
{
  internalSetProgramShader(pVShader, ppClassInstances, numClassInstances);
}

void
GraphicsManager::setShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                                       const uint32 startSlot,
                                       const uint32 numViews)
{
  internalSetShaderResourceView(pShaderRV, startSlot, numViews);
}

void
GraphicsManager::setSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                                 const uint32 startSlot,
                                 const uint32 numSamplers)
{
  internalSetSamplerState(pSamplerLinear, startSlot, numSamplers);
}

void
GraphicsManager::draw(const uint32 vertexCount, const uint32 startVertexLocation)
{
  internalDraw(vertexCount, startVertexLocation);
}

void
GraphicsManager::drawIndexed(const uint32 indexCount,
                             const uint32 startIndexLocation,
                             const uint32 baseVertexLocation)
{
  internalDrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
}
