/*************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
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

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

namespace shEngineSDK {
void
GraphicsManager::initManager(const SPtr<Screen> screen,
                             const bool bAntiliasing,
                             const SampleDesc& sample)
{
  internalInit(screen, bAntiliasing, sample);
}

void
GraphicsManager::clearRenderTarget(const SPtr<RenderTargetView>& pTarget,
                                   const LinearColor& color)
{
  internalClearRenderTarget(pTarget, color);
}

void
GraphicsManager::clearDepthStencil(const SPtr<Texture2D>& pDepthSV,
                                   uint32 flags,
                                   float depth,
                                   uint8 stencil)
{
  internalClearDepthStencil(pDepthSV, flags, depth, stencil);
}

void
GraphicsManager::present(uint32 syncInterval, uint32 flags)
{
  internalPresent(syncInterval, flags);
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
GraphicsManager::createInputLayout(const Vector<InputDesc>& desc,
                                   const SPtr<ProgramShader>& pShader)
{
  return internalCreateInputLayout(desc, pShader);
}

SPtr<InputLayout>
GraphicsManager::createInputLayoutFromShader(const SPtr<ProgramShader>& pShader)
{
  return internalCreateInputLayoutFromShader(pShader);
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
  int32 width, height, bpp;

  uint8* data = stbi_load(fileName.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

  auto pTexture = internalCreateTextureFromFile(data, width, height, bpp);

  stbi_image_free(data);

  return pTexture;
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

SPtr<BlendState>
GraphicsManager::createBlendState(const BlendDesc& blendDesc, const LinearColor& blendFactor)
{
  return internalCreateBlendState(blendDesc, blendFactor);
}

SPtr<RasterizerState>
GraphicsManager::createRasterizerState(const RasterizerDesc& rasterDesc)
{
  return internalCreateRasterizerState(rasterDesc);
}

SPtr<DepthStencilState>
GraphicsManager::createDepthStencilState(const DepthStencilDesc& depthSDesc)
{
    return internalCreateDepthStencilState(depthSDesc);
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
GraphicsManager::setBlendState(const SPtr<BlendState>& pBlendState)
{
  internalSetBlendState(pBlendState);
}

void
GraphicsManager::setRasterizerState(const SPtr<RasterizerState>& pRasterizerState)
{
  internalSetRasterizerState(pRasterizerState);
}

void
GraphicsManager::setDepthStencilState(const SPtr<DepthStencilState>& pDepthStencilState,
                                      const uint8 stencilRef)
{
  internalSetDepthStencilState(pDepthStencilState, stencilRef);
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

GraphicsManager& g_graphicsMan()
{
  return GraphicsManager::instance();
}
}
