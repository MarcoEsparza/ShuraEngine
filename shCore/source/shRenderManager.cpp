/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/24
*  @brief   Render module.
*
*  Render module.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRenderManager.h"
#include "shGraphicsManager.h"
#include "shSceneGraph.h"
#include "shResourceManager.h"
#include "shShaderManager.h"

#include "shMeshComponent.h"
#include "shSkyBoxComponent.h"
#include "shGameObject.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shPass.h"
#include "shMatrix4.h"
#include "shStringID.h"
#include "shPath.h"
#include "shTexture.h"
#include "shCubeMap.h"
#include "shMath.h"

#define PBRBufferSize 32 // Size of the PBR material constant buffer
#define LUMINANCE_MAP_SIZE 512.0f // Size of the luminance map
#define HISTOGRAM_MAP_SIZE 256.0f // Size of the histogram map
#define DEFAULT_THREADS 32 // Default threads for compute shaders
#define DEFAULT_NUM_MIP_LEVELS 4 // Number of mip levels for textures

#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define BLURV_THREADS_Y 4

#define TEXTURE8K_WIDTH 8192
#define TEXTURE4K_WIDTH 4096

#define CUBE_MAP_SIZE 1024 // Size of the cube map texture
#define NUM_CUBE_MAP_FACES 6 // Number of faces in a cube map

namespace shEngineSDK {
void
RenderManager::onStartUp()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  // Smplertates creation
  m_pSamplerLinearWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipLinear,
                                                     TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerPointWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipPoint,
                                                    TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerAnisotropicWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterAnisotropic,
                                                          TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerLinearClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipLinear,
                                                      TEXTURE_ADDRESS_MODE::kClamp);
  m_pSamplerPointClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipPoint,
                                                     TEXTURE_ADDRESS_MODE::kClamp);
  m_pSamplerAnisotropicClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterAnisotropic,
                                                           TEXTURE_ADDRESS_MODE::kClamp);

  // Create model transform constant buffer
  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
  Matrix4 identity = Matrix4::IDENTITY;
  graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));

  auto fullUAVBindFlags = BIND_FLAGS::kRenderTarget |
                          BIND_FLAGS::kShaderResource |
                          BIND_FLAGS::kUnorderedAccess;

  m_pPBRData = graphMan.createConstantBuffer(PBRBufferSize);
  

  // Textures info
  m_renderTargetMap[StringID("DepthMap").getID()] = RenderTargetInfo("DepthMap",
                                                    TEXTURE_FORMAT::kR32G32B32A32_FLOAT);
  m_renderTargetMap[StringID("NormalMap").getID()] = RenderTargetInfo("NormalMap");
  m_renderTargetMap[StringID("ColorMap").getID()] = RenderTargetInfo("ColorMap");
  m_renderTargetMap[StringID("PropMap").getID()] = RenderTargetInfo("PropMap");
  m_renderTargetMap[StringID("AOMap").getID()] = RenderTargetInfo("AOMap",
                                                                  TEXTURE_FORMAT::kR16_FLOAT);

  m_renderTargetMap[StringID("HBlurMap").getID()] = RenderTargetInfo("HBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("VBlurMap").getID()] = RenderTargetInfo("VBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("LightCMap").getID()] = RenderTargetInfo("LightCMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("SkyBoxMap").getID()] = RenderTargetInfo("SkyBoxMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT, fullUAVBindFlags);

  m_renderTargetMap[StringID("TempMap").getID()] = RenderTargetInfo("TempMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ToneMap").getID()] = RenderTargetInfo("ToneMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("PPMap").getID()] = RenderTargetInfo("PPMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ShadowMap").getID()] = RenderTargetInfo("ShadowMap",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("ShadowTemp").getID()] = RenderTargetInfo("ShadowTemp",
    TEXTURE_FORMAT::kR32_FLOAT,
    BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("LuminanceMap").getID()] = RenderTargetInfo("LuminanceMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 0, 1,
    LUMINANCE_MAP_SIZE, LUMINANCE_MAP_SIZE, false);

  m_renderTargetMap[StringID("BrightMap").getID()] = RenderTargetInfo("BrightMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[StringID("AdditiveMap").getID()] = RenderTargetInfo("AdditiveMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("BHBlur").getID()] = RenderTargetInfo("BHBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("BVBlur").getID()] = RenderTargetInfo("BVBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("HistogramMap").getID()] = RenderTargetInfo("HistogramMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 1, 1,
    HISTOGRAM_MAP_SIZE, 3, false);

  m_renderTargetMap[StringID("GbufferDepth").getID()] = RenderTargetInfo("GbufferDepth",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1);

  /*m_renderTargetMap[StringID("CubeMap").getID()] = RenderTargetInfo("CubeMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 1, NUM_CUBE_MAP_FACES, CUBE_MAP_SIZE, CUBE_MAP_SIZE, false);

  m_renderTargetMap[StringID("DiffIrrMap").getID()] = RenderTargetInfo("DiffIrrMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 0, 1, 256.0f, 128.0f, false);

  m_renderTargetMap[StringID("BRDFLut").getID()] = RenderTargetInfo("BRDFLut",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 1, 1, 256.0f, 256.0f, false);

  m_renderTargetMap[StringID("SpecPreMap").getID()] = RenderTargetInfo("SpecPreMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 0, 1);*/

  m_pDiffIrr = graphMan.createTexture2D(256, 128,
                        TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
                        USAGE::kDefault,
                        BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                        0, 1);

  auto pLut = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/Guardians-LogC4.cube"));
  if (pLut) {
    m_pLutTexture = cast::re_ptr<CubeMap>(pLut);
    //m_shaderData.lutSize = static_cast<float>(m_pLutTexture->getLutSize());
  }

  auto pLut1 = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/LBK-K-Tone_33.cube"));
  if (pLut1) {
    m_pLutLBK = cast::re_ptr<CubeMap>(pLut1);
    //m_shaderData.lutSize = static_cast<float>(m_pLutLBK->getLutSize());
  }
}

void
RenderManager::onShutDown()
{
  cleanShaderObjects();
}

void
RenderManager::createRenderTextures()
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (auto& rti : m_renderTargetMap) {
    RenderTargetInfo& rtiInfo = rti.second;

    auto widthSize = rtiInfo.width;
    auto heightSize = rtiInfo.height;
    if (rtiInfo.bUseScaledSize)
    {
      widthSize *= m_screenDimension.x;
      heightSize *= m_screenDimension.y;
    }

    auto pTex = graphMan.createTexture2D(static_cast<uint32>(widthSize),
                                         static_cast<uint32>(heightSize),
                                         rtiInfo.format,
                                         rtiInfo.usage,
                                         rtiInfo.bFlags,
                                         rtiInfo.mipLevels,
                                         rtiInfo.arraySize);
    m_renderTargetMap[StringID(rtiInfo.name).getID()].pTexture = pTex;
  }
}

SPtr<Texture2D>
RenderManager::getTexture(const String& texName)
{
  StringID strID(texName);
  auto pTex = m_renderTargetMap.find(strID.getID());

  if (pTex != m_renderTargetMap.end()) {
    return (*pTex).second.pTexture;
  }

  return nullptr;
}

void
RenderManager::drawMeshesOnScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();
  ShaderManager& shaderMan = g_shaderMan();

  graphMan.vsSetConstantBuffers(shaderMan.m_pMainBuffer, 0);
  graphMan.vsSetConstantBuffers(shaderMan.m_pShaderDataBuffer, 1);

  if (m_pModelTransform) {
    graphMan.vsSetConstantBuffers(m_pModelTransform, 2);
  }
  else {
    m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
    Matrix4 identity = Matrix4::IDENTITY;
    graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));
    graphMan.vsSetConstantBuffers(m_pModelTransform, 2);
  }

  for (auto& gameObject : scene.getGameObjectList()) {
    drawStaticMesh(gameObject);
  }
  for (auto& gameObject : scene.getGameObjectList()) {
    drawTransparentStaticMesh(gameObject);
  }
  for(auto& gameObject : scene.getGameObjectList()) {
    drawSkeletalMesh(gameObject);
  }
}

void
RenderManager::drawStaticMesh(const WPtr<GameObject> pGO)
{
  if(pGO.expired()) {
    return;
  }
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();

  auto pGameObject = pGO.lock();

  for (auto& component : pGameObject->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
        pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

      Transform modelT = pGameObject->transform.getTransform();
      graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        auto& material = meshResource->m_materials[mesh.materialIndex];
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        bool bHasAlphaTest = material->m_properties.properties.flags.bHasAlphaTest;
        if (!bHasAlphaTest && mesh.bVisible) {
          setResourceViewFromPBRMaterial(material);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->childs) {
    drawStaticMesh(pChild);
  }
}

void
RenderManager::drawTransparentStaticMesh(const WPtr<GameObject> pGO)
{
  if (pGO.expired()) {
    return;
  }
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();

  auto pGameObject = pGO.lock();

  for (auto& component : pGameObject->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
      pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

      Transform modelT = pGameObject->transform.getTransform();
      graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        auto& material = meshResource->m_materials[mesh.materialIndex];
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        bool bHasAlphaTest = material->m_properties.properties.flags.bHasAlphaTest;
        if (bHasAlphaTest && mesh.bVisible) {
          setResourceViewFromPBRMaterial(material);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->childs) {
    drawTransparentStaticMesh(pChild);
  }
}

void
RenderManager::drawSkeletalMesh(const WPtr<GameObject> pGO)
{
  if (pGO.expired()) {
    return;
  }
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();

  auto pGameObject = pGO.lock();

  for (auto& component : pGameObject->components) {
    if (component->getType() == COMPONENT_TYPE::kSkeletalMesh &&
        pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<SkeletalMeshComponent>(component);

      graphMan.setVertexBuffers(meshComponent->m_vertexBuffer);
      graphMan.setIndexBuffers(meshComponent->m_indexBuffer);

      Transform modelT = pGameObject->transform.getTransform();
      graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        auto& material = meshResource->m_materials[mesh.materialIndex];
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        setResourceViewFromPBRMaterial(material);
        graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }
}

void
RenderManager::setResourceViewFromPBRMaterial(const SPtr<Material>& pMat)
{
  GraphicsManager& graphMan = g_graphicsMan();

  /*if (pMat->m_type != MATERIAL_TYPE::kPBR) {
    return;
  }*/

  struct PBRMaterialData {
    Vector3 baseColorFactor;
    float unused0; // Padding to align to 16 bytes
    Vector2 metallicRoughnessFactor;
    MaterialProperties properties;
    float unused1; // Padding to align to 16 bytes
  };

  PBRMaterialData pbrData = {};
  pbrData.baseColorFactor = pMat->baseColorFactor;
  pbrData.metallicRoughnessFactor = pMat->metallicRoughnessFactor;
  pbrData.properties = pMat->m_properties;

  /*if (m_pPBRData == nullptr) {
    graphMan.createConstantBuffer(sizeof(PBRMaterialData));
  }*/
  graphMan.updateConstantBuffer(m_pPBRData, &pbrData, sizeof(PBRMaterialData));
  graphMan.psSetConstantBuffers(m_pPBRData, 3);

  if (pMat->baseColor) {
    graphMan.psSetShaderResourceView(pMat->baseColor);
  }

  if (pMat->normal) {
    graphMan.psSetShaderResourceView(pMat->normal, 1);
  }

  if (pMat->metallic) {
    graphMan.psSetShaderResourceView(pMat->metallic, 2);
  }

  if (pMat->roughness) {
    graphMan.psSetShaderResourceView(pMat->roughness, 3);
  }

  if (pMat->ao) {
    graphMan.psSetShaderResourceView(pMat->ao, 4);
  }

  /*if (pMat->m_properties.properties.flags.bHasAlphaTest) {
    graphMan.setBlendState(m_pAlphaTestBS);
  }
  else {
    graphMan.setBlendState(m_pBasicBS);
  }*/
}

static uint32
threadGroups(uint32 size, uint32 threads)
{
  uint32 groups = size / threads;
  if (size % threads > 0) {
    groups++;
  }
  return groups;
}

void
RenderManager::renderScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  //SceneGraph& scene = g_sceneGraph();
  ShaderManager& shaderMan = g_shaderMan();

  SPtr<Texture2D> pOutput;
  SPtr<Texture2D> pInput;

  auto pDepthSV = graphMan.getMainDepthStencil();

  // Get render targets
  auto pMainTarget = graphMan.getMainRenderTargetView();
  auto& pShadowMap = m_renderTargetMap[StringID("ShadowMap").getID()];
  auto& pShadowTemp = m_renderTargetMap[StringID("ShadowTemp").getID()];
  auto& pDepthMap = m_renderTargetMap[StringID("DepthMap").getID()];
  auto& pNormalMap = m_renderTargetMap[StringID("NormalMap").getID()];
  auto& pColorMap = m_renderTargetMap[StringID("ColorMap").getID()];
  auto& pPropMap = m_renderTargetMap[StringID("PropMap").getID()];
  auto& pAOMap = m_renderTargetMap[StringID("AOMap").getID()];
  auto& pHBlurMap = m_renderTargetMap[StringID("HBlurMap").getID()];
  auto& pVBlurMap = m_renderTargetMap[StringID("VBlurMap").getID()];
  auto& pSkyBoxMap = m_renderTargetMap[StringID("SkyBoxMap").getID()];
  auto& pLightCMap = m_renderTargetMap[StringID("LightCMap").getID()];
  auto& pToneMap = m_renderTargetMap[StringID("ToneMap").getID()];
  auto& pTempMap = m_renderTargetMap[StringID("TempMap").getID()];
  auto& pLuminance = m_renderTargetMap[StringID("LuminanceMap").getID()];
  auto& pBrightMap = m_renderTargetMap[StringID("BrightMap").getID()];
  auto& pPPMap = m_renderTargetMap[StringID("PPMap").getID()];
  auto& pAdditiveMap = m_renderTargetMap[StringID("AdditiveMap").getID()];
  auto& pBHBlur = m_renderTargetMap[StringID("BHBlur").getID()];
  auto& pBVBlur = m_renderTargetMap[StringID("BVBlur").getID()];
  auto& pGbufferDepth = m_renderTargetMap[StringID("GbufferDepth").getID()];
  //auto& pHistogramMap = m_renderTargetMap[StringID("HistogramMap").getID()];

  uint32 screenWidth = static_cast<uint32>(m_screenDimension.x);
  uint32 screenHeight = static_cast<uint32>(m_screenDimension.y);

  uint32 dispatchX = threadGroups(screenWidth, DEFAULT_THREADS);
  uint32 dispatchY = threadGroups(screenHeight, DEFAULT_THREADS);
  uint32 dispatchZ = 1;

  Viewport normalVP = {};
  normalVP.width = m_screenDimension.x;
  normalVP.height = m_screenDimension.y;
  normalVP.minDepth = 0.0f;
  normalVP.maxDepth = 1.0f;
  normalVP.topLeftX = 0.0f;
  normalVP.topLeftY = 0.0f;

  Viewport shadowVP = {};
  shadowVP.width = m_shadowMapSize;
  shadowVP.height = m_shadowMapSize;
  shadowVP.minDepth = 0.0f;
  shadowVP.maxDepth = 1.0f;
  shadowVP.topLeftX = 0.0f;
  shadowVP.topLeftY = 0.0f;

  Viewport lumVP = {};
  lumVP.width = LUMINANCE_MAP_SIZE;
  lumVP.height = LUMINANCE_MAP_SIZE;
  lumVP.minDepth = 0.0f;
  lumVP.maxDepth = 1.0f;
  lumVP.topLeftX = 0.0f;
  lumVP.topLeftY = 0.0f;

  Viewport quadVP = {};
  quadVP.width = m_screenDimension.x * 0.5f;
  quadVP.height = m_screenDimension.y * 0.5f;
  quadVP.minDepth = 0.0f;
  quadVP.maxDepth = 1.0f;
  quadVP.topLeftX = 0.0f;
  quadVP.topLeftY = 0.0f;

  /*************************************/
  /*          Shadow Mapping           */
  /*************************************/
  pOutput = pShadowMap.pTexture;
  graphMan.setViewport(shadowVP);
  graphMan.clearDepthStencil(pOutput);
  graphMan.clearRenderTarget(pShadowTemp.pTexture, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pShadowTemp.pTexture }}, pOutput);
  shaderMan.m_passes[StringID("SMapShader").getID()]->setPass();
  drawMeshesOnScene();
  cleanShaderObjects();

  /*************************************/
  /*              GBuffer              */
  /*************************************/
  graphMan.setViewport(normalVP);
  graphMan.clearDepthStencil(pGbufferDepth.pTexture);
  graphMan.clearRenderTarget(pDepthMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pNormalMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pColorMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pPropMap.pTexture, LinearColor::BLACK);
  graphMan.clearDepthStencil(pDepthSV);
  graphMan.setRenderTargets({{pDepthMap.pTexture},
                             {pNormalMap.pTexture},
                             {pColorMap.pTexture},
                             {pPropMap.pTexture }}, pGbufferDepth.pTexture);
  //m_passes[StringID("GBufferShader").getID()]->setPass();
  setSamplers();
  drawMeshesOnScene();
  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  pOutput = pAOMap.pTexture;
  graphMan.clearRenderTarget(pOutput, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pOutput }}, pDepthSV);
  shaderMan.m_passes[StringID("PlaneShader").getID()]->setPass();
  shaderMan.m_passes[StringID("AOShader").getID()]->setPass();
  setSamplers();
  graphMan.psSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.psSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.draw(3, 0);
  cleanShaderObjects();

  /*************************************/
  /*        AO Horizontal Blur         */
  /*************************************/
  shaderMan.m_shaderData.mipLevel0 = 1;
  graphMan.updateConstantBuffer(shaderMan.m_pShaderDataBuffer,
                                &shaderMan.m_shaderData,
                                sizeof(ShaderData));
  pInput = pAOMap.pTexture;
  pOutput = pHBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("HBlurShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  /*graphMan.dispatch(threadGroups(screenWidth, BLURH_THREADS_X),
                    threadGroups(screenHeight, BLURH_THREADS_Y),
                    1);*/
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*          AO Vetical Blur          */
  /*************************************/
  pInput = pHBlurMap.pTexture;
  pOutput = pVBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("VBlurShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  /*graphMan.dispatch(threadGroups(screenWidth, BLURV_THREADS_X),
                    threadGroups(screenHeight, BLURV_THREADS_Y),
                    1);*/
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  uint32 numMipLevels = 0;
  uint32 maxSize = cast::st<uint32>(Math::max(cast::st<float>(m_skyboxDimension.x),
                                             cast::st<float>(m_skyboxDimension.y)));
  for (uint32 i = 0; i < maxSize; ++i) {
    if (maxSize >> i < 32) {
      break;
    }
    ++numMipLevels;
  }
  float normMip = Math::clamp(shaderMan.m_prefilteredData.roughness, 0.0f, 1.0f);
  shaderMan.m_prefilteredData.mipmapLevels = normMip * (numMipLevels - 1) + 0.5f;
  shaderMan.updatePrefilterShaderCB();

  shaderMan.m_passes[StringID("SkyBoxShader").getID()]->setPass();
  setSamplers();
  //graphMan.csSetConstantBuffers(shaderMan.m_pPreCB, 2);
  graphMan.csSetShaderResourceView(m_pSpecularPreMap, 0);
  graphMan.setUnorderedAccessView({ pSkyBoxMap.pTexture }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  float tempRoughness = shaderMan.m_prefilteredData.roughness;
  shaderMan.m_prefilteredData.roughness = 0.0f;
  shaderMan.updatePrefilterShaderCB();
  shaderMan.m_prefilteredData.roughness = tempRoughness;

  /*************************************/
  /*             Lightning             */
  /*************************************/
  pOutput = pLightCMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("LightCS").getID()]->setPass();
  setSamplers();
  //graphMan.csSetConstantBuffers(shaderMan.m_pPreCB, 4);
  graphMan.csSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.csSetShaderResourceView(pColorMap.pTexture, 2);
  graphMan.csSetShaderResourceView(pPropMap.pTexture, 3);
  graphMan.csSetShaderResourceView(pVBlurMap.pTexture, 4);
  graphMan.csSetShaderResourceView(pShadowTemp.pTexture, 5);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 6);
  //graphMan.csSetShaderResourceView(m_pEnvTexture, 7);
  graphMan.csSetShaderResourceView(m_pBRDF, 7);
  graphMan.csSetShaderResourceView(m_pDiffIrr, 8);
  graphMan.csSetShaderResourceView(m_pSpecularPreMap, 9);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*            Add Sky Box            */
  /*************************************/
  pOutput = pTempMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("ASBShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pLightCMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 1);
  graphMan.csSetShaderResourceView(pSkyBoxMap.pTexture, 2);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*             Luminance             */
  /*************************************/
  pInput = pTempMap.pTexture;
  pOutput = pLuminance.pTexture;
  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("LuminanceShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*               Bright              */
  /*************************************/
  pInput = pLuminance.pTexture;
  pOutput = pBrightMap.pTexture;
  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("BrightShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pInput, 1);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(threadGroups(static_cast<uint32>(screenWidth * 0.5f), DEFAULT_THREADS),
                    threadGroups(static_cast<uint32>(screenHeight * 0.5f), DEFAULT_THREADS),
                    1);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*   Blur mips and do additive mix   */
  /*************************************/
  int32 startMip = 3;
  int32 mipCount = 4;
  for (int32 i = 0; i < mipCount; ++i) {
    int32 mipLevel0 = startMip - i;
    int32 mipLevel1 = mipLevel0 + 1;
    
    int32 width = static_cast<int32>(static_cast<int32>(screenWidth) >> (mipLevel0 + 1));
    int32 height = static_cast<int32>(static_cast<int32>(screenHeight) >> (mipLevel0 + 1));

    // Update buffers
    shaderMan.m_mainBufferData.screenSize.x = static_cast<float>(width);
    shaderMan.m_mainBufferData.screenSize.y = static_cast<float>(height);
    shaderMan.m_shaderData.mipLevel0 = static_cast<float>(mipLevel0);
    shaderMan.m_shaderData.mipLevel1 = static_cast<float>(mipLevel1);
    shaderMan.updateMainCB();
    shaderMan.updateShaderDataCB();

    /*************************************/
    /*            Additive Mix           */
    /*************************************/
    pInput = pBrightMap.pTexture;
    if (i > 0) {
      pInput = pAdditiveMap.pTexture;
      //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
      shaderMan.m_passes[StringID("AddMixShader").getID()]->setPass();
      setSamplers();
      //graphMan.csSetSamplerState(m_pSamplerLinearClamp, 1);
      graphMan.csSetShaderResourceView(pBrightMap.pTexture, 0);
      graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
      graphMan.setUnorderedAccessView({ pAdditiveMap.pTexture, mipLevel0 }, 0);
      graphMan.dispatch(threadGroups(width, DEFAULT_THREADS),
                        threadGroups(height, DEFAULT_THREADS),
                        1);
      cleanShaderObjects();
      //graphMan.generateMips(pAdditiveMap.pTexture);
    }

    /*************************************/
    /*          Horizontal Blur          */
    /*************************************/
    //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    shaderMan.m_passes[StringID("HBlurShader").getID()]->setPass();
    //m_passes[StringID("HBlurCS").getID()]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pInput, 0);
    graphMan.setUnorderedAccessView({ pBHBlur.pTexture, mipLevel0 }, 0);
    /*graphMan.dispatch(threadGroups(width, BLURH_THREADS_X),
                      threadGroups(height, BLURH_THREADS_Y),
                      1);*/
    graphMan.dispatch(threadGroups(screenWidth, DEFAULT_THREADS),
                      threadGroups(screenHeight, DEFAULT_THREADS),
                      1);
    cleanShaderObjects();

    /*************************************/
    /*            Vetical Blur           */
    /*************************************/
    //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    shaderMan.m_passes[StringID("VBlurShader").getID()]->setPass();
    //m_passes[StringID("VBlurCS").getID()]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pBHBlur.pTexture, 0);
    graphMan.setUnorderedAccessView({ pBVBlur.pTexture, mipLevel0 }, 0);
    /*graphMan.dispatch(threadGroups(width, BLURH_THREADS_X),
                      threadGroups(height, BLURH_THREADS_Y),
                      1);*/
    graphMan.dispatch(threadGroups(screenWidth, DEFAULT_THREADS),
                      threadGroups(screenHeight, DEFAULT_THREADS),
                      1);
    cleanShaderObjects();
  }

  /*************************************/
  /*             Tone Map              */
  /*************************************/
  shaderMan.m_mainBufferData.screenSize.x = m_screenDimension.x;
  shaderMan.m_mainBufferData.screenSize.y = m_screenDimension.y;
  shaderMan.updateMainCB();

  SPtr<Texture3D> pLutTex;
  int32 index = static_cast<int32>(shaderMan.m_shaderData.toneMappingIndex);
  if (index == 4) {
    shaderMan.m_shaderData.lutSize = cast::st<float>(m_pLutTexture->getLutSize());
    pLutTex = m_pLutTexture->getTexture();
  }
  else if (index == 5) {
    shaderMan.m_shaderData.lutSize = cast::st<float>(m_pLutLBK->getLutSize());
    pLutTex = m_pLutLBK->getTexture();
    shaderMan.m_shaderData.toneMappingIndex = 4.0f;
  }
  else {
    shaderMan.m_shaderData.lutSize = 0;
    pLutTex = nullptr;
  }
  shaderMan.updateShaderDataCB();

  //pInput = pVBlurMap.pTexture;
  //pOutput = pToneMap.pTexture;
  graphMan.setViewport(normalVP);

  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[StringID("ToneMapShader").getID()]->setPass();
  setSamplers();
  //graphMan.csSetSamplerState(m_pSamplerLinearClamp, 1);
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
  graphMan.csSetShaderResourceView(pLuminance.pTexture, 2);
  graphMan.csSetShaderResourceView(pLutTex, 3);
  graphMan.setUnorderedAccessView({ pToneMap.pTexture }, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  shaderMan.m_shaderData.toneMappingIndex = cast::st<float>(index);
  shaderMan.updateShaderDataCB();

  /*************************************/
  /*            PostProcess            */
  /*************************************/
  shaderMan.m_passes[StringID("PPShader").getID()]->setPass();
  setSamplers();

  graphMan.csSetShaderResourceView(pToneMap.pTexture, 0);
  graphMan.setUnorderedAccessView({ pPPMap.pTexture }, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*         Add to backbuffer         */
  /*************************************/
  graphMan.setRenderTargets({{ pMainTarget }}, pDepthSV);
  shaderMan.m_passes[StringID("PlaneShader").getID()]->setPass();
  shaderMan.m_passes[StringID("FinalShader").getID()]->setPass();
  setSamplers();

  graphMan.psSetShaderResourceView(pPPMap.pTexture, 0);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*             Histogram             */
  /*************************************/
  /*graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HistogramShader").getID()]->setPass();
  setSamplers();

  graphMan.csSetShaderResourceView(pPPMap.pTexture, 0);
  graphMan.setUnorderedAccessView(pHistogramMap.pTexture, 0);

  uint32 dx = static_cast<uint32>((HISTOGRAM_MAP_SIZE + DEFAULT_THREADS) / DEFAULT_THREADS);
  uint32 dy = 3;
  uint32 dz = 1;

  graphMan.dispatch(dx, dy, dz);

  cleanShaderObjects();*/
}

void
RenderManager::computeIBL()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();
  ShaderManager& shaderMan = g_shaderMan();

  m_pSpecularPreMap = nullptr;
  m_skyboxDimension.x = TEXTURE8K_WIDTH;
  m_skyboxDimension.y = TEXTURE4K_WIDTH;

  SPtr<ImageResource> pSbImg;
  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkyBox) {
        auto pSkyBox = sh_reinterpretPCast<SkyBoxComponent>(component);
        pSbImg = pSkyBox->getSkyBoxResource();
        break;
      }
    }
  }

  m_skyboxDimension.x = cast::st<int32>(pSbImg->width);
  m_skyboxDimension.y = cast::st<int32>(pSbImg->height);
  uint32 numMipLevels = 0;
  uint32 maxSize = cast::st<uint32>(Math::max(cast::st<float>(m_skyboxDimension.x),
                                             cast::st<float>(m_skyboxDimension.y)));
  for (uint32 i = 0; i < maxSize; ++i) {
    if (maxSize >> i < 32) {
      break;
    }
    ++numMipLevels;
  }
  
  m_pSpecularPreMap = graphMan.createTexture2D(cast::st<uint32>(m_skyboxDimension.x),
                               cast::st<uint32>(m_skyboxDimension.y),
                               TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                               USAGE::kDefault,
                               BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                               numMipLevels);

  shaderMan.m_prefilteredData.width = 256;
  shaderMan.m_prefilteredData.height = 128;
  shaderMan.m_prefilteredData.samples = TEXTURE4K_WIDTH;
  shaderMan.m_prefilteredData.roughness = 0.0f;
  shaderMan.m_prefilteredData.mipmapLevels = 0.0f;
  shaderMan.updatePrefilterShaderCB();

  /*************************************/
  /*           Diff Irradiance         */
  /*************************************/
  shaderMan.m_passes[StringID("DiffIrrShader").getID()]->setPass();
  setSamplers();
  //graphMan.csSetConstantBuffers(shaderMan.m_pPrefilteredCB, 2);
  graphMan.csSetShaderResourceView(pSbImg->texture, 0);
  graphMan.setUnorderedAccessView({ m_pDiffIrr }, 0);
  graphMan.dispatch(threadGroups(256, 16),
                    threadGroups(128, 16),
                    1);

  cleanShaderObjects();

  graphMan.generateMips(m_pDiffIrr);

  /*************************************/
  /*      Specular Prefiltered Map     */
  /*************************************/
  PrefilteredCB specPreCB = shaderMan.m_prefilteredData;

  for (int32 mip = 0; mip < cast::st<int32>(numMipLevels); ++mip) {
    Vector2i preSize = Vector2i(m_skyboxDimension.x >> mip, m_skyboxDimension.y >> mip);
    Vector2i dispatchSize = Vector2i(threadGroups(preSize.x, 8),
                                     threadGroups(preSize.y, 8));

    shaderMan.m_prefilteredData.width = preSize.x;
    shaderMan.m_prefilteredData.height = preSize.y;
    shaderMan.m_prefilteredData.samples = TEXTURE4K_WIDTH;
    shaderMan.m_prefilteredData.mipmapLevels = cast::st<float>(mip);
    shaderMan.m_prefilteredData.roughness = (mip == 0) ? 0.0f :
      Math::clamp(cast::st<float>(mip) / cast::st<float>(numMipLevels - 1), 0.0f, 1.0f);

    shaderMan.updatePrefilterShaderCB();

    shaderMan.m_passes[StringID("SpecularPreMapShader").getID()]->setPass();
    setSamplers();
    //graphMan.csSetConstantBuffers(shaderMan.m_pPreCB, 2);
    graphMan.csSetShaderResourceView(pSbImg->texture, 0);
    graphMan.setUnorderedAccessView({ m_pSpecularPreMap, mip }, 0);
    graphMan.dispatch(dispatchSize.x, dispatchSize.y, 1);

    cleanShaderObjects();
  }

  shaderMan.m_prefilteredData = specPreCB;
  shaderMan.updatePrefilterShaderCB();
}

void
RenderManager::computeBRDF()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();
  
  m_pBRDF = graphMan.createTexture2D(256, 256,
                                     TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                                     USAGE::kDefault,
                                     BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                                     1, 1);
  
  uint32 lutDispatch = threadGroups(256, 16);

  shaderMan.m_passes[StringID("BRDFShader").getID()]->setPass();

  setSamplers();
  graphMan.setUnorderedAccessView({ m_pBRDF }, 0);
  graphMan.dispatch(lutDispatch, lutDispatch, 1);

  cleanShaderObjects();
}

void
RenderManager::cleanPSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.psSetShaderResourceView(pSRV, i);
  }
}

void
RenderManager::cleanCSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.csSetShaderResourceView(pSRV, i);
  }
}

void
RenderManager::cleanCSUAView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.setUnorderedAccessView({pSRV}, i);
  }
}

void
RenderManager::cleanVSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.vsSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanPSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.psSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanGSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.gsSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanCSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.csSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanShaderObjects()
{
  cleanPSShaderResourceView();
  cleanCSShaderResourceView();
  cleanCSUAView(1);
  cleanVSConstantBuffers();
  cleanPSConstantBuffers();
  cleanGSConstantBuffers();
  cleanCSConstantBuffers();
}

//void
//RenderManager::updateShaderDataBuffer()
//{
//  GraphicsManager& graphMan = g_graphicsMan();
//  if (m_pShaderDataBuffer == nullptr) {
//    m_pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
//  }
//
//  graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));
//}
//
//void
//RenderManager::updatePrefilteredIBLBuffer()
//{
//  GraphicsManager& graphMan = g_graphicsMan();
//  graphMan.updateConstantBuffer(m_pPreCB, &m_prefilteredCB, sizeof(PrefilteredCB));
//}

void
RenderManager::setSamplers()
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.psSetSamplerState(m_pSamplerLinearWrap, 0);
  graphMan.psSetSamplerState(m_pSamplerPointWrap, 1);
  graphMan.psSetSamplerState(m_pSamplerAnisotropicWrap, 2);
  graphMan.psSetSamplerState(m_pSamplerLinearClamp, 3);
  graphMan.psSetSamplerState(m_pSamplerPointClamp, 4);
  graphMan.psSetSamplerState(m_pSamplerAnisotropicClamp, 5);

  graphMan.csSetSamplerState(m_pSamplerLinearWrap, 0);
  graphMan.csSetSamplerState(m_pSamplerPointWrap, 1);
  graphMan.csSetSamplerState(m_pSamplerAnisotropicWrap, 2);
  graphMan.csSetSamplerState(m_pSamplerLinearClamp, 3);
  graphMan.csSetSamplerState(m_pSamplerPointClamp, 4);
  graphMan.csSetSamplerState(m_pSamplerAnisotropicClamp, 5);
}

void
RenderManager::setShadowMapSize(const float size)
{
  m_shadowMapSize = size;
}

void
RenderManager::setScreenSize(const Vector2& screenD)
{
  m_screenDimension = screenD;
}

RenderManager& g_renderMan()
{
  return RenderManager::instance();
}
}
