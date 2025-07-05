/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/13
*  @brief   Render module.
*
*  Render module.
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
#include "shRenderManager.h"
#include "shGraphicsManager.h"
#include "shSceneGraph.h"
#include "shMeshComponent.h"
#include "shSkyBoxComponent.h"
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shPass.h"
#include "shMatrix4.h"
#include "shStringID.h"

#define PBRBufferSize 32 // Size of the PBR material constant buffer
#define LUMINANCE_MAP_SIZE 512.0f // Size of the luminance map
#define HISTOGRAM_MAP_SIZE 256.0f // Size of the histogram map
#define DEFAULT_THREADS 32 // Default threads for compute shaders
#define DEFAULT_NUM_MIP_LEVELS 4 // Number of mip levels for textures

#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define BLURV_THREADS_Y 4

namespace shEngineSDK {
RenderManager::~RenderManager()
{
  for (auto& pass : m_passes) {
    pass.second.reset();
  }

  m_passes.clear();
}

void
RenderManager::onStartUp()
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Blend state for alpha testing
  BlendDesc blendDesc = {};
  blendDesc.alphaToCoverageEnable = false;
  blendDesc.independentBlendEnable = false;
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kSrcAlpha;
  blendDesc.renderTarget[0].destBlend = BLEND::kInvSrcAlpha;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kInvSrcAlpha;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;
  m_pAlphaTestBS = graphMan.createBlendState(blendDesc);

  // General blend state
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;
  m_pBasicBS = graphMan.createBlendState(blendDesc);

  // Create model transform constant buffer
  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
  Matrix4 identity = Matrix4::IDENTITY;
  graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));

  auto fullUAVBindFlags = BIND_FLAGS::kRenderTarget |
                          BIND_FLAGS::kShaderResource |
                          BIND_FLAGS::kUnorderedAccess;

  m_pPBRData = graphMan.createConstantBuffer(PBRBufferSize);
  m_pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
  m_pMainBuffer = graphMan.createConstantBuffer(sizeof(MainBufferData));

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

  m_renderTargetMap[StringID("SkyBoxMap").getID()] = RenderTargetInfo("SkyBoxMap");

  m_renderTargetMap[StringID("TempMap").getID()] = RenderTargetInfo("TempMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ToneMap").getID()] = RenderTargetInfo("ToneMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("PPMap").getID()] = RenderTargetInfo("PPMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ShadowMap").getID()] = RenderTargetInfo("ShadowMap",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("ShadowTemp").getID()] = RenderTargetInfo("ShadowTemp",
    TEXTURE_FORMAT::kR32_FLOAT,
    BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("LuminanceMap").getID()] = RenderTargetInfo("LuminanceMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 0,
    LUMINANCE_MAP_SIZE, LUMINANCE_MAP_SIZE, false);

  m_renderTargetMap[StringID("BrightMap").getID()] = RenderTargetInfo("BrightMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS,
    0.5f, 0.5f);

  m_renderTargetMap[StringID("AdditiveMap").getID()] = RenderTargetInfo("AdditiveMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags);

  m_renderTargetMap[StringID("BHBlur").getID()] = RenderTargetInfo("BHBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS,
    0.5f, 0.5f);

  m_renderTargetMap[StringID("BVBlur").getID()] = RenderTargetInfo("BVBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS,
    0.5f, 0.5f);

  m_renderTargetMap[StringID("HistogramMap").getID()] = RenderTargetInfo("HistogramMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 1,
    HISTOGRAM_MAP_SIZE, 3, false);
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
                                         rtiInfo.mipLevels);
    m_renderTargetMap[StringID(rtiInfo.name).getID()].pTexture = pTex;
  }
}

void
RenderManager::createPasses()
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Init pass shaders
  // GBuffer
  auto pGbufferShader = sh_makeShared<Pass>();
  pGbufferShader->setVShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "main",
                                 "vs_5_0");
  pGbufferShader->setPShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "mainPS",
                                 "ps_5_0");
  pGbufferShader->compileShader();

  // Lightning
  auto pLightCS = sh_makeShared<Pass>();
  pLightCS->setCShaderInfo("resources/shaders/LightCShader.hlsl",
                           "CSMain",
                           "cs_5_0");
  pLightCS->compileShader();

  // AO
  auto pAOShader = sh_makeShared<Pass>();
  pAOShader->setPShaderInfo("resources/shaders/AOShader.hlsl",
                            "mainPS",
                            "ps_5_0");
  pAOShader->compileShader();

  // HBlur
  auto pHBlurShader = sh_makeShared<Pass>();
  pHBlurShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                               "HBlurCS",
                               "cs_5_0");
  pHBlurShader->compileShader();

  // VBlur
  auto pVBlurShader = sh_makeShared<Pass>();
  pVBlurShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                               "VBlurCS",
                               "cs_5_0");
  pVBlurShader->compileShader();

  // HBlur
  auto pHBlurCS = sh_makeShared<Pass>();
  pHBlurCS->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                           "HBlur_CS",
                           "cs_5_0");
  pHBlurCS->compileShader();

  // VBlur
  auto pVBlurCS = sh_makeShared<Pass>();
  pVBlurCS->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "VBlur_CS",
                            "cs_5_0");
  pVBlurCS->compileShader();

  // Shadow map
  auto pSMapShader = sh_makeShared<Pass>();
  pSMapShader->setVShaderInfo("resources/shaders/SMapShader.hlsl",
                              "main",
                              "vs_5_0");
  pSMapShader->setPShaderInfo("resources/shaders/SMapShader.hlsl",
                              "mainPS",
                              "ps_5_0");
  pSMapShader->compileShader();

  // Skybox
  auto pSkyBoxShader = sh_makeShared<Pass>();
  pSkyBoxShader->setVShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "main",
                                "vs_5_0");
  pSkyBoxShader->setPShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "mainPS",
                                "ps_5_0");
  pSkyBoxShader->compileShader();

  // Final shader
  auto pFinalShader = sh_makeShared<Pass>();
  pFinalShader->setPShaderInfo("resources/shaders/FinalShader.hlsl",
                               "mainPS",
                               "ps_5_0");
  pFinalShader->compileShader();

  // Plane Vertex shader
  auto pPlaneVS = sh_makeShared<Pass>();
  pPlaneVS->setVShaderInfo("resources/shaders/PlaneVertexShader.hlsl",
                           "main",
                           "vs_5_0");
  pPlaneVS->compileShader();

  // Histogram shader
  auto pHistogramShader = sh_makeShared<Pass>();
  pHistogramShader->setCShaderInfo("resources/shaders/HistogramShader.hlsl",
                                   "CSMain",
                                   "cs_5_0");
  pHistogramShader->compileShader();

  // Add skybox shader
  auto pASBShader = sh_makeShared<Pass>();
  pASBShader->setCShaderInfo("resources/shaders/AddSkyboxShader.hlsl",
                             "CSMain",
                             "cs_5_0");
  pASBShader->compileShader();

  // Luminance shader
  auto pLuminanceShader = sh_makeShared<Pass>();
  pLuminanceShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                   "LuminanceCS",
                                   "cs_5_0");
  pLuminanceShader->compileShader();

  // Bright shader
  auto pBrightShader = sh_makeShared<Pass>();
  pBrightShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "BrightCS",
                                "cs_5_0");
  pBrightShader->compileShader();

  // ToneMap shader
  auto pToneMapShader = sh_makeShared<Pass>();
  pToneMapShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                                 "ToneMapCS",
                                 "cs_5_0");
  pToneMapShader->compileShader();

  // AddMix shader
  auto pAddMixShader = sh_makeShared<Pass>();
  pAddMixShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "AddMixCS",
                                "cs_5_0");
  pAddMixShader->compileShader();

  // PostProcess shader
  auto pPPShader = sh_makeShared<Pass>();
  pPPShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "PostProcessCS",
                            "cs_5_0");
  pPPShader->compileShader();

  // Raster state
  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kSolid;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  // Blend state
  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  // Basic depth stencil state
  DepthStencilDesc depthSDesc = {};
  depthSDesc.depthEnable = true;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kLess;
  depthSDesc.stencilEnable = true;
  depthSDesc.stencilReadMask = 0xFF;
  depthSDesc.stencilWriteMask = 0xFF;
  depthSDesc.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  depthSDesc.frontFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.backFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  depthSDesc.backFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Depth stencil state for sky box
  DepthStencilDesc skyBoxDepth = {};
  skyBoxDepth.depthEnable = false;
  skyBoxDepth.depthWriteMask = DEPTH_WRITE_MASK::kZero;
  skyBoxDepth.depthFunc = COMPARISON_FUNC::kLessEqual;
  skyBoxDepth.stencilEnable = true;
  skyBoxDepth.stencilReadMask = 0xFF;
  skyBoxDepth.stencilWriteMask = 0xFF;
  skyBoxDepth.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  skyBoxDepth.frontFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  skyBoxDepth.backFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  skyBoxDepth.backFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Dpeth stencil state for planes
  DepthStencilDesc planeDepthSDesc = depthSDesc;
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.stencilEnable = false;

  // States creation
  auto pSamplerLinear = graphMan.createSamplerState();
  m_pSamplerClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipLinear,
                                                TEXTURE_ADDRESS_MODE::kClamp);
  auto pRasterState = graphMan.createRasterizerState(rasterDesc);
  auto pBlendState = graphMan.createBlendState(blendDesc);
  auto pDepthStencil = graphMan.createDepthStencilState(depthSDesc);

  // Fill pass info
  // GBuffer
  pGbufferShader->generateInputLayout();
  pGbufferShader->setSamplerState(pSamplerLinear);
  pGbufferShader->setRasterizerState(pRasterState);
  pGbufferShader->setBlendState(pBlendState);
  pGbufferShader->setDepthStencilState(pDepthStencil);

  // Lightning
  pLightCS->setSamplerState(pSamplerLinear);

  // AO
  pAOShader->setSamplerState(pSamplerLinear);

  // Shadow Map
  pSMapShader->generateInputLayout();
  pSMapShader->setSamplerState(pSamplerLinear);
  pSMapShader->setRasterizerState(pRasterState);
  pSMapShader->setBlendState(pBlendState);
  pSMapShader->setDepthStencilState(pDepthStencil);

  // SkyBox Map
  pSkyBoxShader->generateInputLayout();
  pSkyBoxShader->setSamplerState(pSamplerLinear);
  pSkyBoxShader->setRasterizerState(pRasterState);
  pSkyBoxShader->setBlendState(pBlendState);
  pSkyBoxShader->setDepthStencilStateFromDesc(skyBoxDepth);

  // Final
  pFinalShader->setSamplerState(pSamplerLinear);

  // Plane vs
  pPlaneVS->generateInputLayout();
  pPlaneVS->setDepthStencilStateFromDesc(planeDepthSDesc);

  // ToneMap
  pToneMapShader->setSamplerState(pSamplerLinear);

  // Bright
  pBrightShader->setSamplerState(pSamplerLinear);

  // Save passes
  m_passes[StringID("GBufferShader").getID()] = pGbufferShader;
  m_passes[StringID("AOShader").getID()] = pAOShader;
  m_passes[StringID("HBlurShader").getID()] = pHBlurShader;
  m_passes[StringID("VBlurShader").getID()] = pVBlurShader;
  m_passes[StringID("HBlurCS").getID()] = pVBlurShader;
  m_passes[StringID("VBlurCS").getID()] = pVBlurShader;
  m_passes[StringID("LightCS").getID()] = pLightCS;
  m_passes[StringID("SMapShader").getID()] = pSMapShader;
  m_passes[StringID("SkyBoxShader").getID()] = pSkyBoxShader;
  m_passes[StringID("FinalShader").getID()] = pFinalShader;
  m_passes[StringID("PlaneShader").getID()] = pPlaneVS;
  m_passes[StringID("HistogramShader").getID()] = pHistogramShader;
  m_passes[StringID("ASBShader").getID()] = pASBShader;
  m_passes[StringID("LuminanceShader").getID()] = pLuminanceShader;
  m_passes[StringID("BrightShader").getID()] = pBrightShader;
  m_passes[StringID("ToneMapShader").getID()] = pToneMapShader;
  m_passes[StringID("PPShader").getID()] = pPPShader;
  m_passes[StringID("AddMixShader").getID()] = pAddMixShader;
}

SPtr<Pass>
RenderManager::getPass(const String& passName)
{
  StringID strID(passName);
  auto pPass = m_passes.find(strID.getID());

  if (pPass != m_passes.end()) {
    return (*pPass).second;
  }

  return nullptr;
}

void RenderManager::recompileShaders()
{
  for(auto& pPass : m_passes) {
    pPass.second->compileShader();
  }
}

void
RenderManager::drawStaticMeshOnScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

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
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
        auto meshComponent = sh_reinterpretPCast<StaticMeshComponent>(component);

        graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
        graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

        Transform modelT = gameObject->transform.getTransform();
        graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

        uint32 indexCount = 0;
        uint32 vertexCount = 0;
        auto& meshResource = meshComponent->m_mesh;
        for (auto& mesh : meshResource->m_meshes) {
          if (!meshResource->m_materials[mesh.materialIndex]->m_properties.bHasAlphaTest) {
            setResourceViewFromPBRMaterial(meshResource->m_materials[mesh.materialIndex]);
            graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
          }
          indexCount += mesh.numIndices;
          vertexCount += mesh.numVertices;
        }
      }
    }
  }
  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
        auto meshComponent = sh_reinterpretPCast<StaticMeshComponent>(component);

        graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
        graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

        Transform modelT = gameObject->transform.getTransform();
        graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

        uint32 indexCount = 0;
        uint32 vertexCount = 0;
        auto& meshResource = meshComponent->m_mesh;
        for (auto& mesh : meshResource->m_meshes) {
          if (meshResource->m_materials[mesh.materialIndex]->m_properties.bHasAlphaTest) {
            setResourceViewFromPBRMaterial(meshResource->m_materials[mesh.materialIndex]);
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
RenderManager::setResourceViewFromPBRMaterial(const SPtr<Material>& pMat)
{
  GraphicsManager& graphMan = g_graphicsMan();

  if (pMat->m_type != MATERIAL_TYPE::kPBR) {
    return;
  }

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

  if (pMat->m_properties.bHasAlphaTest) {
    graphMan.setBlendState(m_pAlphaTestBS);
  }
  else {
    graphMan.setBlendState(m_pBasicBS);
  }
}

static uint32
threadGroups(uint32 size, uint32 threads)
{
  return (size + threads - 1) / threads;
}

void
RenderManager::renderScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

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
  auto& pHistogramMap = m_renderTargetMap[StringID("HistogramMap").getID()];

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
  graphMan.setRenderTargets({ pShadowTemp.pTexture }, pOutput);
  m_passes[StringID("SMapShader").getID()]->setPass();
  drawStaticMeshOnScene();
  cleanShaderObjects();

  /*************************************/
  /*              GBuffer              */
  /*************************************/
  graphMan.setViewport(normalVP);
  graphMan.clearRenderTarget(pDepthMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pNormalMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pColorMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pPropMap.pTexture, LinearColor::BLACK);
  graphMan.clearDepthStencil(pDepthSV);
  graphMan.setRenderTargets({ pDepthMap.pTexture,
                              pNormalMap.pTexture,
                              pColorMap.pTexture,
                              pPropMap.pTexture }, pDepthSV);
  m_passes[StringID("GBufferShader").getID()]->setPass();
  drawStaticMeshOnScene();
  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  pOutput = pAOMap.pTexture;
  graphMan.clearRenderTarget(pOutput, LinearColor::BLACK);
  graphMan.setRenderTargets({ pOutput }, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("AOShader").getID()]->setPass();
  graphMan.psSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.psSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.draw(3, 0);
  cleanShaderObjects();

  /*************************************/
  /*        AO Horizontal Blur         */
  /*************************************/
  m_shaderData.mipLevel0 = 1;
  graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));
  pInput = pAOMap.pTexture;
  pOutput = pHBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HBlurShader").getID()]->setPass();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView(pOutput, 0);
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
  m_passes[StringID("VBlurShader").getID()]->setPass();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView(pOutput, 0);
  /*graphMan.dispatch(threadGroups(screenWidth, BLURV_THREADS_X),
                    threadGroups(screenHeight, BLURV_THREADS_Y),
                    1);*/
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*             Lightning             */
  /*************************************/
  pOutput = pLightCMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LightCS").getID()]->setPass();
  graphMan.csSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.csSetShaderResourceView(pColorMap.pTexture, 2);
  graphMan.csSetShaderResourceView(pPropMap.pTexture, 3);
  graphMan.csSetShaderResourceView(pVBlurMap.pTexture, 4);
  graphMan.csSetShaderResourceView(pShadowTemp.pTexture, 5);
  //graphMan.csSetShaderResourceView(pSkyBoxMap.pTexture, 6);
  graphMan.setUnorderedAccessView(pOutput, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  graphMan.clearRenderTarget(pSkyBoxMap.pTexture, LinearColor::BLACK);
  graphMan.setRenderTargets({ pSkyBoxMap.pTexture }, pDepthSV);
  m_passes[StringID("SkyBoxShader").getID()]->setPass();

  WPtr<Texture2D> pSbTex;
  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkyBox) {
        auto pSkyBox = sh_reinterpretPCast<SkyBoxComponent>(component);
        graphMan.setVertexBuffers(pSkyBox->getVertexBuffer());
        graphMan.setIndexBuffers(pSkyBox->getIndexBuffer());
        pSbTex = pSkyBox->getMaterial()->baseColor;
        graphMan.psSetShaderResourceView(pSbTex, 0);

        uint32 numIndices = static_cast<uint32>(pSkyBox->getIndices().size());
        graphMan.drawIndexed(numIndices, 0, 0);
      }
    }
  }

  cleanShaderObjects();

  /*************************************/
  /*            Add Sky Box            */
  /*************************************/
  pOutput = pTempMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ASBShader").getID()]->setPass();
  graphMan.csSetShaderResourceView(pLightCMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.csSetShaderResourceView(pSkyBoxMap.pTexture, 2);
  graphMan.setUnorderedAccessView(pOutput, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*             Luminance             */
  /*************************************/
  pInput = pTempMap.pTexture;
  pOutput = pLuminance.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LuminanceShader").getID()]->setPass();
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.setUnorderedAccessView(pOutput, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*               Bright              */
  /*************************************/
  //m_mainBufferData.screenSize.x = static_cast<float>(screenWidth * 0.5f);
  //m_mainBufferData.screenSize.y = static_cast<float>(screenHeight * 0.5f);
  //graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));
  pInput = pLuminance.pTexture;
  pOutput = pBrightMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("BrightShader").getID()]->setPass();
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pInput, 1);
  graphMan.setUnorderedAccessView(pOutput, 0);
  graphMan.dispatch(threadGroups(screenWidth * 0.5f, DEFAULT_THREADS),
                    threadGroups(screenHeight * 0.5f, DEFAULT_THREADS),
                    1);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*   Blur mips and do additive mix   */
  /*************************************/
  int32 startMip = 3;
  int32 mipCount = 4;
  for (int32 i = 0; i < mipCount; ++i) {
    int32 mipLevel0 = startMip + i;
    int32 mipLevel1 = mipLevel0 + 1;
    
    int32 width = static_cast<int32>(static_cast<int32>(screenWidth) >> (mipLevel0 + 1));
    int32 height = static_cast<int32>(static_cast<int32>(screenHeight) >> (mipLevel0 + 1));

    //Viewport blurVP = {};
    //blurVP.width = static_cast<float>(width);
    //blurVP.height = static_cast<float>(height);
    //blurVP.minDepth = 0.0f;
    //blurVP.maxDepth = 1.0f;
    //blurVP.topLeftX = 0.0f;
    //blurVP.topLeftY = 0.0f;
    //graphMan.setViewport(blurVP);

    //uint32 blurDispatchX = threadGroups(width, DEFAULT_THREADS);
    //uint32 blurDispatchY = threadGroups(height, DEFAULT_THREADS);

    // Update buffers
    m_mainBufferData.screenSize.x = static_cast<float>(width);
    m_mainBufferData.screenSize.y = static_cast<float>(height);
    m_shaderData.mipLevel0 = mipLevel0;
    m_shaderData.mipLevel1 = mipLevel1;
    graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));
    graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));

    /*************************************/
    /*            Additive Mix           */
    /*************************************/
    pInput = pBrightMap.pTexture;
    if (i > 0) {
      pInput = pAdditiveMap.pTexture;
      graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
      m_passes[StringID("AddMixShader").getID()]->setPass();
      graphMan.setCSSamplerState(m_pSamplerClamp, 1);
      graphMan.csSetShaderResourceView(pBrightMap.pTexture, 0);
      graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
      graphMan.setUnorderedAccessView(pAdditiveMap.pTexture, 0);
      graphMan.dispatch(threadGroups(width, DEFAULT_THREADS),
                        threadGroups(height, DEFAULT_THREADS),
                        1);
      cleanShaderObjects();
      graphMan.generateMips(pAdditiveMap.pTexture);
    }

    /*************************************/
    /*          Horizontal Blur          */
    /*************************************/
    graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    //m_passes[StringID("HBlurShader").getID()]->setPass();
    //m_passes[StringID("HBlurCS").getID()]->setPass();
    graphMan.csSetShaderResourceView(pInput, 0);
    graphMan.setUnorderedAccessView(pBHBlur.pTexture, 0);
    graphMan.dispatch(threadGroups(screenWidth, BLURH_THREADS_X),
                      threadGroups(screenHeight, BLURH_THREADS_Y),
                      1);
    /*graphMan.dispatch(threadGroups(screenWidth * 0.5f, DEFAULT_THREADS),
                      threadGroups(screenHeight * 0.5f, DEFAULT_THREADS),
                      1);*/
    cleanShaderObjects();
    graphMan.generateMips(pBHBlur.pTexture);

    /*************************************/
    /*            Vetical Blur           */
    /*************************************/
    graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    //m_passes[StringID("VBlurShader").getID()]->setPass();
    m_passes[StringID("VBlurCS").getID()]->setPass();
    graphMan.csSetShaderResourceView(pBHBlur.pTexture, 0);
    graphMan.setUnorderedAccessView(pBVBlur.pTexture, 0);
    graphMan.dispatch(threadGroups(screenWidth, BLURH_THREADS_X),
                      threadGroups(screenHeight, BLURH_THREADS_Y),
                      1);
    /*graphMan.dispatch(threadGroups(screenWidth * 0.5f, DEFAULT_THREADS),
                      threadGroups(screenHeight * 0.5f, DEFAULT_THREADS),
                      1);*/
    cleanShaderObjects();
    graphMan.generateMips(pBVBlur.pTexture);
  }

  /*************************************/
  /*             Tone Map              */
  /*************************************/
  m_mainBufferData.screenSize.x = m_screenDimension.x;
  m_mainBufferData.screenSize.y = m_screenDimension.y;
  graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));
  //pInput = pVBlurMap.pTexture;
  //pOutput = pToneMap.pTexture;
  graphMan.setViewport(normalVP);

  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ToneMapShader").getID()]->setPass();
  graphMan.setCSSamplerState(m_pSamplerClamp, 1);
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
  graphMan.csSetShaderResourceView(pLuminance.pTexture, 2);
  graphMan.setUnorderedAccessView(pToneMap.pTexture, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*            PostProcess            */
  /*************************************/
  //pInput = pTempMap.pTexture;
  //pOutput = pPPMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("PPShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pToneMap.pTexture, 0);
  graphMan.setUnorderedAccessView(pPPMap.pTexture, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*         Add to backbuffer         */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("FinalShader").getID()]->setPass();

  graphMan.psSetShaderResourceView(pPPMap.pTexture, 0);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*             Histogram             */
  /*************************************/
  /*graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HistogramShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pPPMap.pTexture, 0);
  graphMan.setUnorderedAccessView(pHistogramMap.pTexture, 0);

  uint32 dx = static_cast<uint32>((HISTOGRAM_MAP_SIZE + DEFAULT_THREADS) / DEFAULT_THREADS);
  uint32 dy = 3;
  uint32 dz = 1;

  graphMan.dispatch(dx, dy, dz);

  cleanShaderObjects();*/
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
    graphMan.setUnorderedAccessView(pSRV, i);
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
