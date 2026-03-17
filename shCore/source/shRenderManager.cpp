/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/11/14
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
#include "shRect.h"

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

#define MAX_SAMPLERS 16 // Maximum number of samplers

namespace shEngineSDK {
const uint32 RenderManager::SHADOWMAP_TEX_ID = StringID("ShadowMap").getID();
const uint32 RenderManager::SHADOWTEMP_TEX_ID = StringID("ShadowTemp").getID();
const uint32 RenderManager::DEPTH_TEX_ID = StringID("DepthMap").getID();
const uint32 RenderManager::NORMAL_TEX_ID = StringID("NormalMap").getID();
const uint32 RenderManager::COLOR_TEX_ID = StringID("ColorMap").getID();
const uint32 RenderManager::PROPS_TEX_ID = StringID("PropMap").getID();
const uint32 RenderManager::EMMISIVE_TEX_ID = StringID("EmmisiveMap").getID();
const uint32 RenderManager::SSAO_TEX_ID = StringID("SSAOMap").getID();
const uint32 RenderManager::HBLUR_TEX_ID = StringID("HBlurMap").getID();
const uint32 RenderManager::VBLUR_TEX_ID = StringID("VBlurMap").getID();
const uint32 RenderManager::SKYBOX_TEX_ID = StringID("SkyBoxMap").getID();
const uint32 RenderManager::LIGHTC_TEX_ID = StringID("LightCMap").getID();
const uint32 RenderManager::TONEMAP_TEX_ID = StringID("ToneMap").getID();
const uint32 RenderManager::TEMP_TEX_ID = StringID("TempMap").getID();
const uint32 RenderManager::LUMINANCE_TEX_ID = StringID("LuminanceMap").getID();
const uint32 RenderManager::BRIGHT_TEX_ID = StringID("BrightMap").getID();
const uint32 RenderManager::POSTPROCESS_TEX_ID = StringID("PPMap").getID();
const uint32 RenderManager::ADDITIVE_TEX_ID = StringID("AdditiveMap").getID();
const uint32 RenderManager::BHBLUR_TEX_ID = StringID("BHBlur").getID();
const uint32 RenderManager::BVBLUR_TEX_ID = StringID("BVBlur").getID();
const uint32 RenderManager::GBUFFER_DEPTH_TEX_ID = StringID("GBufferDepth").getID();
const uint32 RenderManager::HISTOGRAM_TEX_ID = StringID("HistogramMap").getID();
const uint32 RenderManager::EMM_PROCESS_TEX_ID = StringID("EmmProMap").getID();
const uint32 RenderManager::PLANE_DEPTH_TEX_ID = StringID("PlaneDepth").getID();

RenderManager::~RenderManager()
{
  cleanShaderObjects();
  m_pLutTexture.reset();
  m_pLutLBK.reset();
  m_pBRDF.reset();
  m_pDiffIrr.reset();
  m_pSpecularPreMap.reset();
}

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

  auto fullUAVBindFlags = BIND_FLAGS::kRenderTarget |
                          BIND_FLAGS::kShaderResource |
                          BIND_FLAGS::kUnorderedAccess;

  // Textures info
  m_renderTargetMap[DEPTH_TEX_ID] = RenderTargetInfo("DepthMap",
                                    TEXTURE_FORMAT::kR32G32B32A32_FLOAT);
  m_renderTargetMap[NORMAL_TEX_ID] = RenderTargetInfo("NormalMap");
  m_renderTargetMap[COLOR_TEX_ID] = RenderTargetInfo("ColorMap");
  m_renderTargetMap[PROPS_TEX_ID] = RenderTargetInfo("PropMap");
  m_renderTargetMap[EMMISIVE_TEX_ID] = RenderTargetInfo("EmmisiveMap");
  m_renderTargetMap[SSAO_TEX_ID] = RenderTargetInfo("SSAOMap", TEXTURE_FORMAT::kR16_FLOAT);

  m_renderTargetMap[HBLUR_TEX_ID] = RenderTargetInfo("HBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[VBLUR_TEX_ID] = RenderTargetInfo("VBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[EMM_PROCESS_TEX_ID] = RenderTargetInfo("EmmProMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT, fullUAVBindFlags, USAGE::kDefault);

  m_renderTargetMap[LIGHTC_TEX_ID] = RenderTargetInfo("LightCMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[SKYBOX_TEX_ID] = RenderTargetInfo("SkyBoxMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT, fullUAVBindFlags);

  m_renderTargetMap[TEMP_TEX_ID] = RenderTargetInfo("TempMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT, fullUAVBindFlags);

  m_renderTargetMap[TONEMAP_TEX_ID] = RenderTargetInfo("ToneMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[POSTPROCESS_TEX_ID] = RenderTargetInfo("PPMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[SHADOWMAP_TEX_ID] = RenderTargetInfo("ShadowMap",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[SHADOWTEMP_TEX_ID] = RenderTargetInfo("ShadowTemp",
    TEXTURE_FORMAT::kR32_FLOAT,
    BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[LUMINANCE_TEX_ID] = RenderTargetInfo("LuminanceMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 0, 1,
    LUMINANCE_MAP_SIZE, LUMINANCE_MAP_SIZE, false);

  m_renderTargetMap[BRIGHT_TEX_ID] = RenderTargetInfo("BrightMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[ADDITIVE_TEX_ID] = RenderTargetInfo("AdditiveMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[BHBLUR_TEX_ID] = RenderTargetInfo("BHBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[BVBLUR_TEX_ID] = RenderTargetInfo("BVBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[HISTOGRAM_TEX_ID] = RenderTargetInfo("HistogramMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 1, 1,
    HISTOGRAM_MAP_SIZE, 3, false);

  m_renderTargetMap[GBUFFER_DEPTH_TEX_ID] = RenderTargetInfo("GBufferDepth",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1);

  m_renderTargetMap[PLANE_DEPTH_TEX_ID] = RenderTargetInfo("PlaneDepth",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1);

  m_pDiffIrr = graphMan.createTexture2D(256, 128,
                        TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
                        USAGE::kDefault,
                        BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                        0, 1);

  m_pDiffIrr->setDebugName("DiffuseIrradianceMap");

  auto pLut = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/Guardians-LogC4.cube"));
  if (pLut) {
    m_pLutTexture = cast::re_ptr<CubeMap>(pLut);
  }

  auto pLut1 = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/LBK-K-Tone_33.cube"));
  if (pLut1) {
    m_pLutLBK = cast::re_ptr<CubeMap>(pLut1);
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
    pTex->setDebugName(rtiInfo.name);
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

  graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                &Matrix4::IDENTITY,
                                sizeof(Matrix4));

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

  for (auto& component : pGameObject->m_componentList) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
        pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      if (meshComponent->m_mesh.expired()) {
        return;
      }

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());
      Matrix4 modelT = pGameObject->m_transform.getTransformMatrix();
      graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                    &modelT, sizeof(Matrix4));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto meshResource = meshComponent->m_mesh.lock();
      for (auto& mesh : meshResource->m_meshes) {
        auto material = meshResource->m_materials[mesh.materialIndex].lock();
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        bool bHasAlphaTest = material->m_properties.properties.flags.bHasAlphaTest;
        if (!bHasAlphaTest && mesh.bVisible) {
          setShaderResourceFromMaterial(material);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->m_childList) {
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

  for (auto& component : pGameObject->m_componentList) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
      pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      if (meshComponent->m_mesh.expired()) {
        return;
      }

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

      Matrix4 modelT = pGameObject->m_transform.getTransformMatrix();
      graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                    &modelT, sizeof(Matrix4));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto meshResource = meshComponent->m_mesh.lock();
      for (auto& mesh : meshResource->m_meshes) {
        auto material = meshResource->m_materials[mesh.materialIndex].lock();
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        bool bHasAlphaTest = material->m_properties.properties.flags.bHasAlphaTest;
        if (bHasAlphaTest && mesh.bVisible) {
          setShaderResourceFromMaterial(material);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->m_childList) {
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

  for (auto& component : pGameObject->m_componentList) {
    if (component->getType() == COMPONENT_TYPE::kSkeletalMesh &&
        pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<SkeletalMeshComponent>(component);

      if (!meshComponent->m_mesh) {
        return;
      }

      graphMan.setVertexBuffers(meshComponent->m_vertexBuffer);
      graphMan.setIndexBuffers(meshComponent->m_indexBuffer);

      Matrix4 modelT = pGameObject->m_transform.getTransformMatrix();
      graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                    &modelT, sizeof(Matrix4));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        auto& material = meshResource->m_materials[mesh.materialIndex];
        shaderMan.getPassFromMaterial(material->m_properties)->setPass();
        setShaderResourceFromMaterial(material);
        graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }
}

void
RenderManager::drawShadowMap()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();
  ShaderManager& shaderMan = g_shaderMan();

  graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                &Matrix4::IDENTITY,
                                sizeof(Matrix4));

  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->m_componentList) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
          gameObject->m_bActive) {
        auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);
        if (meshComponent->m_mesh.expired()) {
          return;
        }

        graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
        graphMan.setIndexBuffers(meshComponent->getIndexBuffer());
        Matrix4 modelT = gameObject->m_transform.getTransformMatrix();
        graphMan.updateConstantBuffer(shaderMan.m_pModelTransformBuffer,
                                      &modelT, sizeof(Matrix4));
        
        uint32 indexCount = 0;
        uint32 vertexCount = 0;
        auto meshResource = meshComponent->m_mesh.lock();
        for (auto& mesh : meshResource->m_meshes) {
          if (mesh.bVisible) {
            graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
          }
          indexCount += mesh.numIndices;
          vertexCount += mesh.numVertices;
        }
      }
    }
  }
}

void
RenderManager::setShaderResourceFromMaterial(const SPtr<Material>& pMat)
{
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();
  //ResourceManager& resMan = g_resourceMan();

  shaderMan.m_materialData.baseColorFactor = pMat->baseColorFactor;
  shaderMan.m_materialData.metallicRoughnessFactor = pMat->metallicRoughnessFactor;
  shaderMan.m_materialData.emissiveFactor = pMat->emissiveFactor;
  shaderMan.m_materialData.emmisiveIntensity = pMat->emmisiveIntensity;
  shaderMan.m_materialData.alphaCutoff = pMat->alphaCutoff;
  shaderMan.m_materialData.properties = pMat->m_properties;
  shaderMan.updateMaterialCB();

  if (!pMat->m_baseColor.expired()) {
    auto pBaseColorImg = pMat->m_baseColor.lock();
    graphMan.psSetShaderResourceView(pBaseColorImg->texture, 0);
  }
  if (!pMat->m_normal.expired()) {
    auto pNormal = pMat->m_normal.lock();
    graphMan.psSetShaderResourceView(pNormal->texture, 1);
  }
  if (!pMat->m_metalness.expired()) {
    auto pMetalness = pMat->m_metalness.lock();
    graphMan.psSetShaderResourceView(pMetalness->texture, 2);
  }
  if (!pMat->m_roughness.expired()) {
    auto pRoughness = pMat->m_roughness.lock();
    graphMan.psSetShaderResourceView(pRoughness->texture, 3);
  }
  if (!pMat->m_ao.expired()) {
    auto pAO = pMat->m_ao.lock();
    graphMan.psSetShaderResourceView(pAO->texture, 4);
  }
  if (!pMat->m_emissive.expired()) {
    auto pEmissive = pMat->m_emissive.lock();
    graphMan.psSetShaderResourceView(pEmissive->texture, 5);
  }
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
  ShaderManager& shaderMan = g_shaderMan();

  SPtr<Texture2D> pOutput;
  SPtr<Texture2D> pInput;

  auto pDepthSV = graphMan.getMainDepthStencil();

  // Get render targets
  auto pMainTarget = graphMan.getMainRenderTargetView();
  auto& pShadowMap = m_renderTargetMap[SHADOWMAP_TEX_ID];
  auto& pShadowTemp = m_renderTargetMap[SHADOWTEMP_TEX_ID];
  auto& pDepthMap = m_renderTargetMap[DEPTH_TEX_ID];
  auto& pNormalMap = m_renderTargetMap[NORMAL_TEX_ID];
  auto& pColorMap = m_renderTargetMap[COLOR_TEX_ID];
  auto& pPropMap = m_renderTargetMap[PROPS_TEX_ID];
  auto& pEmmisiveMap = m_renderTargetMap[EMMISIVE_TEX_ID];
  auto& pAOMap = m_renderTargetMap[SSAO_TEX_ID];
  auto& pHBlurMap = m_renderTargetMap[HBLUR_TEX_ID];
  auto& pVBlurMap = m_renderTargetMap[VBLUR_TEX_ID];
  auto& pSkyBoxMap = m_renderTargetMap[SKYBOX_TEX_ID];
  auto& pLightCMap = m_renderTargetMap[LIGHTC_TEX_ID];
  auto& pToneMap = m_renderTargetMap[TONEMAP_TEX_ID];
  auto& pTempMap = m_renderTargetMap[TEMP_TEX_ID];
  auto& pLuminance = m_renderTargetMap[LUMINANCE_TEX_ID];
  auto& pBrightMap = m_renderTargetMap[BRIGHT_TEX_ID];
  auto& pPPMap = m_renderTargetMap[POSTPROCESS_TEX_ID];
  auto& pAdditiveMap = m_renderTargetMap[ADDITIVE_TEX_ID];
  auto& pBHBlur = m_renderTargetMap[BHBLUR_TEX_ID];
  auto& pBVBlur = m_renderTargetMap[BVBLUR_TEX_ID];
  auto& pGbufferDepth = m_renderTargetMap[GBUFFER_DEPTH_TEX_ID];
  auto& pEmmProcessMap = m_renderTargetMap[EMM_PROCESS_TEX_ID];
  auto& pPlaneDepth = m_renderTargetMap[PLANE_DEPTH_TEX_ID];
  //auto& pHistogramMap = m_renderTargetMap[HISTOGRAM_TEX_ID];

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

  if (pTempMap.width != m_screenDimension.x ||
      pTempMap.height != m_screenDimension.y) {
    createRenderTextures();
  }

  /*************************************/
  /*          Shadow Mapping           */
  /*************************************/
  //pOutput = pShadowMap.pTexture;
  graphMan.setViewport(shadowVP);
  graphMan.clearDepthStencil(pShadowMap.pTexture);
  graphMan.clearRenderTarget(pShadowTemp.pTexture, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pShadowTemp.pTexture }}, pShadowMap.pTexture);
  shaderMan.m_passes[shaderMan.SHADOWMAP_SHADER_ID]->setPass();
  drawShadowMap();
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
  graphMan.clearRenderTarget(pEmmisiveMap.pTexture, LinearColor::BLACK);
  graphMan.clearDepthStencil(pDepthSV);
  graphMan.setRenderTargets({{pDepthMap.pTexture},
                             {pNormalMap.pTexture},
                             {pColorMap.pTexture},
                             {pPropMap.pTexture },
                             {pEmmisiveMap.pTexture }}, pGbufferDepth.pTexture);
  setSamplers();
  drawMeshesOnScene();
  cleanShaderObjects();

  // Reset buffer miplevels
  shaderMan.m_shaderData.mipLevel0 = 0.0f;
  shaderMan.m_shaderData.mipLevel1 = 0.0f;
  shaderMan.updateShaderDataCB();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  pOutput = pAOMap.pTexture;
  graphMan.clearRenderTarget(pOutput, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pOutput }}, pPlaneDepth.pTexture);
  shaderMan.m_passes[shaderMan.PLANE_SHADER_ID]->setPass();
  shaderMan.m_passes[shaderMan.SSAO_SHADER_ID]->setPass();
  setSamplers();
  graphMan.psSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.psSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.draw(3, 0);
  cleanShaderObjects();

  /*************************************/
  /*        SSAO Horizontal Blur       */
  /*************************************/
  pInput = pAOMap.pTexture;
  pOutput = pHBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[shaderMan.HBLUR_SHADER_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(threadGroups(screenWidth, BLURH_THREADS_X),
                    threadGroups(screenHeight, BLURH_THREADS_Y),
                    1);
  cleanShaderObjects();

  /*************************************/
  /*          SSAO Vetical Blur        */
  /*************************************/
  pInput = pHBlurMap.pTexture;
  pOutput = pVBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[shaderMan.VBLUR_SHADER_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(threadGroups(screenWidth, BLURV_THREADS_X),
                    threadGroups(screenHeight, BLURV_THREADS_Y),
                    1);
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

  shaderMan.m_passes[shaderMan.SKYBOX_SHADER_ID]->setPass();
  setSamplers();
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
  shaderMan.m_passes[shaderMan.LIGHT_CS_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.csSetShaderResourceView(pColorMap.pTexture, 2);
  graphMan.csSetShaderResourceView(pPropMap.pTexture, 3);
  graphMan.csSetShaderResourceView(pVBlurMap.pTexture, 5);
  graphMan.csSetShaderResourceView(pShadowTemp.pTexture, 6);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 7);
  graphMan.csSetShaderResourceView(m_pBRDF, 8);
  graphMan.csSetShaderResourceView(m_pDiffIrr, 9);
  graphMan.csSetShaderResourceView(m_pSpecularPreMap, 10);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*            Add Sky Box            */
  /*************************************/
  pOutput = pTempMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  shaderMan.m_passes[shaderMan.ADDSKYBOX_SHADER_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pLightCMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 1);
  graphMan.csSetShaderResourceView(pSkyBoxMap.pTexture, 2);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*              Emmisive             */
  /*************************************/
  shaderMan.m_passes[shaderMan.EMMISIVE_SHADER_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pEmmisiveMap.pTexture, 1);
  graphMan.setUnorderedAccessView({ pEmmProcessMap.pTexture }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*             Luminance             */
  /*************************************/
  pInput = pEmmProcessMap.pTexture;
  pOutput = pLuminance.pTexture;
  shaderMan.m_passes[shaderMan.LUMINANCE_SHADER_ID]->setPass();
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
  shaderMan.m_passes[shaderMan.BRIGHT_SHADER_ID]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pEmmProcessMap.pTexture, 0);
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

    // Update buffer
    shaderMan.m_shaderData.mipLevel0 = static_cast<float>(mipLevel0);
    shaderMan.m_shaderData.mipLevel1 = static_cast<float>(mipLevel1);
    shaderMan.updateShaderDataCB();

    /*************************************/
    /*            Additive Mix           */
    /*************************************/
    pInput = pBrightMap.pTexture;
    if (i > 0) {
      pInput = pAdditiveMap.pTexture;
      shaderMan.m_passes[shaderMan.ADDITIVE_SHADER_ID]->setPass();
      setSamplers();
      graphMan.csSetShaderResourceView(pBrightMap.pTexture, 0);
      graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
      graphMan.setUnorderedAccessView({ pAdditiveMap.pTexture, mipLevel0 }, 0);
      graphMan.dispatch(threadGroups(width, DEFAULT_THREADS),
                        threadGroups(height, DEFAULT_THREADS),
                        1);
      cleanShaderObjects();
    }

    /*************************************/
    /*          Horizontal Blur          */
    /*************************************/
    shaderMan.m_passes[shaderMan.HBLUR_SHADER_ID]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pInput, 0);
    graphMan.setUnorderedAccessView({ pBHBlur.pTexture, mipLevel0 }, 0);
    graphMan.dispatch(threadGroups(width, BLURH_THREADS_X),
                      threadGroups(height, BLURH_THREADS_Y),
                      1);
    cleanShaderObjects();

    /*************************************/
    /*            Vetical Blur           */
    /*************************************/
    shaderMan.m_passes[shaderMan.VBLUR_SHADER_ID]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pBHBlur.pTexture, 0);
    graphMan.setUnorderedAccessView({ pBVBlur.pTexture, mipLevel0 }, 0);
    graphMan.dispatch(threadGroups(width, BLURV_THREADS_X),
                      threadGroups(height, BLURV_THREADS_Y),
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
  graphMan.setViewport(normalVP);

  shaderMan.m_passes[shaderMan.TONEMAP_SHADER_ID]->setPass();
  setSamplers();
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
  shaderMan.m_passes[shaderMan.POSTPROCESS_SHADER_ID]->setPass();
  setSamplers();

  graphMan.csSetShaderResourceView(pToneMap.pTexture, 0);
  graphMan.setUnorderedAccessView({ pPPMap.pTexture }, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ); 
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
    for (auto& component : gameObject->m_componentList) {
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

  m_pSpecularPreMap->setDebugName("SpecularPrefilteredMap");

  shaderMan.m_prefilteredData.width = 256;
  shaderMan.m_prefilteredData.height = 128;
  shaderMan.m_prefilteredData.samples = TEXTURE4K_WIDTH;
  shaderMan.m_prefilteredData.roughness = 0.0f;
  shaderMan.m_prefilteredData.mipmapLevels = 0.0f;
  shaderMan.updatePrefilterShaderCB();

  /*************************************/
  /*           Diff Irradiance         */
  /*************************************/
  shaderMan.m_passes[shaderMan.DIFFUSE_IRR_SHADER_ID]->setPass();
  setSamplers();
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

    shaderMan.m_passes[shaderMan.PREFILTERED_IRR_SHADER_ID]->setPass();
    setSamplers();
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
  
  m_pBRDF->setDebugName("BRDF_Texture");

  uint32 lutDispatch = threadGroups(256, 16);

  shaderMan.m_passes[shaderMan.BRDF_SHADER_ID]->setPass();

  setSamplers();
  graphMan.setUnorderedAccessView({ m_pBRDF }, 0);
  graphMan.dispatch(lutDispatch, lutDispatch, 1);

  cleanShaderObjects();
}

SPtr<Texture2D>&
RenderManager::createSceneTexture(const Vector2& winSize)
{
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();
  auto& pPPMap = m_renderTargetMap[POSTPROCESS_TEX_ID];
  //auto& pSceneTex = m_renderTargetMap[SCENE_TEX_ID];
  if (m_sceneTarget.pTexture != nullptr ||
      m_sceneTarget.width != winSize.x ||
      m_sceneTarget.height != winSize.y) {
    m_sceneTarget.pTexture = graphMan.createTexture2D(cast::st<uint32>(winSize.x),
                                                      cast::st<uint32>(winSize.y),
                                                      TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                                                      USAGE::kDefault,
                                                      BIND_FLAGS::kShaderResource |
                                                      BIND_FLAGS::kRenderTarget,
                                                      1, 1);
  }

  cleanShaderObjects();
  graphMan.clearRenderTarget(m_sceneTarget.pTexture, LinearColor::BLUE);
  Rect scissorRect = {};
  scissorRect.min = { 0, 0 };
  scissorRect.max = { cast::st<float>(m_screenDimension.x),
                      cast::st<float>(m_screenDimension.y) };
  graphMan.setScissorRects(scissorRect);

  graphMan.setPrimitiveTopology();
  WPtr<Texture2D> pSceneDepthStencil;
  graphMan.setRenderTargets({{m_sceneTarget.pTexture}},
                             pSceneDepthStencil);
  shaderMan.m_passes[shaderMan.PLANE_SHADER_ID]->setPass();
  shaderMan.m_passes[shaderMan.FINAL_SHADER_ID]->setPass();
  setSamplers();
  graphMan.psSetShaderResourceView(pPPMap.pTexture, 0);
  graphMan.draw(3, 0);

  cleanShaderObjects();

  return m_sceneTarget.pTexture;
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
