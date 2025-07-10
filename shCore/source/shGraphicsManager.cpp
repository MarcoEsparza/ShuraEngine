/*************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
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
GraphicsManager::initManager(const WPtr<Screen> screen,
                             const bool bAntiliasing,
                             const SampleDesc& sample)
{
  internalInit(screen, bAntiliasing, sample);
}

void
GraphicsManager::clearRenderTarget(const WPtr<Texture2D> pTarget,
                                   const LinearColor& color)
{
  internalClearRenderTarget(pTarget, color);
}

void
GraphicsManager::clearDepthStencil(const WPtr<Texture2D> pDepthSV,
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

WPtr<Texture2D>
GraphicsManager::getMainRenderTargetView() const
{
  return internalGetMainRenderTargetView();
}

WPtr<Texture2D>
GraphicsManager::getMainDepthStencil() const
{
  return internalGetMainDepthStencil();
}

SPtr<InputLayout>
GraphicsManager::createInputLayout(const Vector<InputDesc>& desc,
                                   const WPtr<VertexShader> pShader)
{
  return internalCreateInputLayout(desc, pShader);
}

SPtr<InputLayout>
GraphicsManager::createInputLayoutFromShader(const WPtr<VertexShader> pShader)
{
  return internalCreateInputLayoutFromShader(pShader);
}

SPtr<VertexShader>
GraphicsManager::createVertexShader(const String& fileName,
                                    const String& entryPoint,
                                    const String& shaderModel,
                                    const Vector<ShaderMacro>& macros)
{
  return internalCreateVertexShader(fileName, entryPoint, shaderModel, macros);
}

SPtr<PixelShader>
GraphicsManager::createPixelShader(const String& fileName,
                                   const String& entryPoint,
                                   const String& shaderModel,
                                   const Vector<ShaderMacro>& macros)
{
  return internalCreatePixelShader(fileName, entryPoint, shaderModel, macros);
}

SPtr<GeometryShader>
GraphicsManager::createGeometryShader(const String& fileName,
                                      const String& entryPoint,
                                      const String& shaderModel,
                                      const Vector<ShaderMacro>& macros)
{
  return internalCreateGeometryShader(fileName, entryPoint, shaderModel, macros);
}

SPtr<ComputeShader>
GraphicsManager::createComputeShader(const String& fileName,
                                     const String& entryPoint,
                                     const String& shaderModel,
                                     const Vector<ShaderMacro>& macros)
{
  return internalCreateComputeShader(fileName, entryPoint, shaderModel, macros);
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

  SystemPath path = fileName;
  SPtr<Texture2D> pTexture;

  if (path.extension() == ".hdr") {
    void* data = stbi_loadf(fileName.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    pTexture = internalCreateTextureFromFile(data, width, height, bpp);
    stbi_image_free(data);
  }
  else {
    void* data = stbi_load(fileName.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    pTexture = internalCreateTextureFromFile(data, width, height, bpp);
    stbi_image_free(data);
  }

  return pTexture;
}

SPtr<Texture2D>
GraphicsManager::createTextureFromDDS(const String& fileName)
{
  return internalCreateTextureFromDDS(fileName);
}

SPtr<Texture2D>
GraphicsManager::createTexture2D(const uint32 width,
                                 const uint32 height,
                                 const uint32 format,
                                 const uint32 usage,
                                 const uint32 bindFlags,
                                 const uint32 mipLevels)
{
  return internalCreateTexture2D(width, height, format, usage, bindFlags, mipLevels);
}

SPtr<Texture2D> GraphicsManager::createErrorTexturre()
{
  return internalCreateErrorTexture();
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
GraphicsManager::generateMips(const WPtr<Texture2D> pTexture)
{
  internalGenerateMips(pTexture);
}

void
GraphicsManager::updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                                      const void* pData,
                                      const uint32 dataSize)
{
  internalUpdateConstantBuffer(pCBuffer, pData, dataSize);
}

void
GraphicsManager::updateTexture2D(WPtr<Texture2D> pTexture,
                                 uint8* pData,
                                 uint32 width,
                                 uint32 bpp)
{
  internalUpdateTexture2D(pTexture,
                          pData,
                          width,
                          bpp);
}

void
GraphicsManager::updateScreenSize(const Vector2& size)
{
  internalUpdateScreenSize(size);
}

void
GraphicsManager::saveTextureToDDS(const WPtr<Texture2D> pTexture,
                                  const String& filePath)
{
  internalSaveTextureToDDS(pTexture, filePath);
}

void
GraphicsManager::setViewport(const Viewport& vp)
{
  internalSetViewport(vp);
}

void
GraphicsManager::setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                                  const WPtr<Texture2D> pDepthSV)
{
  internalSetRenderTargets(pRenderTVs, pDepthSV);
}

void
GraphicsManager::setInputLayout(const WPtr<InputLayout> pInput)
{
  internalSetInputLayout(pInput);
}

void
GraphicsManager::setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                                  const uint32 startSlot,
                                  const uint32 numBuffers,
                                  const uint32 offset)
{
  internalSetVertexBuffers(pVBuffer, startSlot, numBuffers, offset);
}

void
GraphicsManager::setIndexBuffers(const WPtr<IndexBuffer> pIBuffer, const uint32 offset)
{
  internalSetIndexBuffers(pIBuffer, offset);
}

void
GraphicsManager::vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalVSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalPSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalGSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                      const uint32 startSlot,
                                      const uint32 numBuffers)
{
  internalCSSetConstantBuffers(pCBuffer, startSlot, numBuffers);
}

void
GraphicsManager::setPrimitiveTopology(const uint32 primitive)
{
  internalSetPrimitiveTopology(primitive);
}

//void
//GraphicsManager::setProgramShader(const SPtr<ProgramShader>& pVShader,
//                                 const void* ppClassInstances,
//                                 const uint32 numClassInstances)
//{
//  internalSetProgramShader(pVShader, ppClassInstances, numClassInstances);
//}

void
GraphicsManager::setVertexShader(const WPtr<VertexShader> pVShader,
                                 const void* ppClassInstances,
                                 const uint32 numClassInstances)
{
  internalSetVertexShader(pVShader, ppClassInstances, numClassInstances);
}

void
GraphicsManager::setPixelShader(const WPtr<PixelShader> pPShader,
                                const void* ppClassInstances,
                                const uint32 numClassInstances)
{
  internalSetPixelShader(pPShader, ppClassInstances, numClassInstances);
}

void
GraphicsManager::setGeometryShader(const WPtr<GeometryShader> pGShader,
                                   const void* ppClassInstances,
                                   const uint32 numClassInstances)
{
  internalSetGeometryShader(pGShader, ppClassInstances, numClassInstances);
}

void
GraphicsManager::setComputeShader(const WPtr<ComputeShader> pCShader,
                                  const void* ppClassInstances,
                                  const uint32 numClassInstances)
{
  internalSetComputeShader(pCShader, ppClassInstances, numClassInstances);
}

void
GraphicsManager::psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                         const uint32 startSlot,
                                         const uint32 numViews)
{
  internalPSSetShaderResourceView(pShaderRV, startSlot, numViews);
}

void
GraphicsManager::csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                         const uint32 startSlot,
                                         const uint32 numViews)
{
  internalCSSetShaderResourceView(pShaderRV, startSlot, numViews);
}

void
GraphicsManager::setUnorderedAccessView(const UnorderedAccess& pUAVs,
                                        const uint32 startSlot)
{
  internalSetUnorderedAccessView(pUAVs, startSlot);
}

void
GraphicsManager::setPSSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                 const uint32 startSlot,
                                 const uint32 numSamplers)
{
  internalPSSetSamplerState(pSamplerLinear, startSlot, numSamplers);
}

void
GraphicsManager::setCSSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                   const uint32 startSlot,
                                   const uint32 numSamplers)
{
  internalCSSetSamplerState(pSamplerLinear, startSlot, numSamplers);
}

void
GraphicsManager::setBlendState(const WPtr<BlendState> pBlendState)
{
  internalSetBlendState(pBlendState);
}

void
GraphicsManager::setRasterizerState(const WPtr<RasterizerState> pRasterizerState)
{
  internalSetRasterizerState(pRasterizerState);
}

void
GraphicsManager::setDepthStencilState(const WPtr<DepthStencilState> pDepthStencilState,
                                      const uint8 stencilRef)
{
  internalSetDepthStencilState(pDepthStencilState, stencilRef);
}

void
GraphicsManager::setScissorRects(const Rect& scissorClip)
{
  internalSetScissorRects(scissorClip);
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

void 
GraphicsManager::dispatch(const uint32 threadGroupCountX,
                          const uint32 threadGroupCountY,
                          const uint32 threadGroupCountZ)
{
  internalDispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

GraphicsManager& g_graphicsMan()
{
  return GraphicsManager::instance();
}
}
