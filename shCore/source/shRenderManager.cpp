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

  // Textures info
  m_targetInfoVec.push_back({ "DepthMap",
                              TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource});
  m_targetInfoVec.push_back({ "NormalMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource });
  m_targetInfoVec.push_back({ "ColorMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource });
  m_targetInfoVec.push_back({ "PropMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource });
  m_targetInfoVec.push_back({ "AOMap",
                              TEXTURE_FORMAT::kR16_FLOAT,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource });
  m_targetInfoVec.push_back({ "HBlurMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "VBlurMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "LightCMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "SkyBoxMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget |
                              BIND_FLAGS::kShaderResource });
  m_targetInfoVec.push_back({ "BrightMap",
                              TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "TempMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "ToneMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });
  m_targetInfoVec.push_back({ "PPMap",
                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                              USAGE::kDefault,
                              BIND_FLAGS::kShaderResource |
                              BIND_FLAGS::kUnorderedAccess });

}

void
RenderManager::onShutDown()
{
  m_passes.clear();
  m_targets.clear();
  cleanShaderObjects();
}

void
RenderManager::addRenderTarget(const SPtr<Texture2D>& pRTV, const String& name)
{
  StringID strID(name);
  m_targets[strID.getID()] = pRTV;
}

void
RenderManager::createRenderTextures()
{
  GraphicsManager& graphMan = g_graphicsMan();

  if (!m_targets.empty()) {
    m_targets.clear();
  }

  for (auto& rti : m_targetInfoVec) {
    auto pTex = graphMan.createTexture2D(static_cast<uint32>(m_screenDimension.x),
                                         static_cast<uint32>(m_screenDimension.y),
                                         rti.format,
                                         rti.usage,
                                         rti.bFlags);
    m_targets[StringID(rti.name).getID()] = pTex;
  }

  auto pSMapTarget = graphMan.createTexture2D(static_cast<uint32>(m_sMapSize),
                                              static_cast<uint32>(m_sMapSize),
                                              TEXTURE_FORMAT::kR32_TYPELESS,
                                              USAGE::kDefault,
                                              BIND_FLAGS::kDepthStencil |
                                              BIND_FLAGS::kShaderResource);

  auto pShadowTempTarget = graphMan.createTexture2D(static_cast<uint32>(m_sMapSize),
                                                    static_cast<uint32>(m_sMapSize),
                                                    TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                                                    USAGE::kDefault,
                                                    BIND_FLAGS::kRenderTarget |
                                                    BIND_FLAGS::kShaderResource);

  auto pLuminance = graphMan.createTexture2D(512,
                                             512,
                                             TEXTURE_FORMAT::kR32_FLOAT,
                                             USAGE::kDefault,
                                             BIND_FLAGS::kShaderResource |
                                             BIND_FLAGS::kUnorderedAccess);

  auto pHistogramMap = graphMan.createTexture2D(256,
                                                3,
                                                TEXTURE_FORMAT::kR32_UINT,
                                                USAGE::kDefault,
                                                BIND_FLAGS::kShaderResource |
                                                BIND_FLAGS::kUnorderedAccess);

  m_targets[StringID("ShadowMap").getID()] = pSMapTarget;
  m_targets[StringID("ShadowTemp").getID()] = pShadowTempTarget;
  m_targets[StringID("LuminanceMap").getID()] = pLuminance;
  m_targets[StringID("HistogramMap").getID()] = pHistogramMap;
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
  pHBlurShader->setCShaderInfo("resources/shaders/HBlurShader.hlsl",
                               "CSMain",
                               "cs_5_0");
  pHBlurShader->compileShader();

  // VBlur
  auto pVBlurShader = sh_makeShared<Pass>();
  pVBlurShader->setCShaderInfo("resources/shaders/VBlurShader.hlsl",
                               "CSMain",
                               "cs_5_0");
  pVBlurShader->compileShader();

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

  // Luminance shader
  auto pBrightShader = sh_makeShared<Pass>();
  pBrightShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "BrightCS",
                                "cs_5_0");
  pBrightShader->compileShader();

  // ToneMap shader
  auto pToneMapShader = sh_makeShared<Pass>();
  pToneMapShader->setCShaderInfo("resources/shaders/ToneMappingShader.hlsl",
                                 "CSMain",
                                 "cs_5_0");
  pToneMapShader->compileShader();

  // PostProcess shader
  auto pAddMixShader = sh_makeShared<Pass>();
  pAddMixShader->setCShaderInfo("resources/shaders/AdditiveMixShader.hlsl",
                                "CSMain",
                                "cs_5_0");
  pAddMixShader->compileShader();

  // PostProcess shader
  auto pPPShader = sh_makeShared<Pass>();
  pPPShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "CSMain",
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

  // Save passes
  m_passes[StringID("GBufferShader").getID()] = pGbufferShader;
  m_passes[StringID("AOShader").getID()] = pAOShader;
  m_passes[StringID("HBlurShader").getID()] = pHBlurShader;
  m_passes[StringID("VBlurShader").getID()] = pVBlurShader;
  m_passes[StringID("LightCS").getID()] = pLightCS;
  m_passes[StringID("SMapShader").getID()] = pSMapShader;
  m_passes[StringID("SkyBoxShader").getID()] = pSkyBoxShader;
  m_passes[StringID("FinalShader").getID()] = pFinalShader;
  m_passes[StringID("PlaneShader").getID()] = pPlaneVS;
  m_passes[StringID("HistogramShader").getID()] = pHistogramShader;
  m_passes[StringID("ASBShader").getID()] = pASBShader;
  m_passes[StringID("LuminanceShader").getID()] = pLuminanceShader;
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
    graphMan.vsSetConstantBuffers(m_pModelTransform, 1);
  }
  else {
    m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
    Matrix4 identity = Matrix4::IDENTITY;
    graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));
    graphMan.vsSetConstantBuffers(m_pModelTransform, 1);
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
          setResourceViewFromPBRMaterial(meshResource->m_materials[mesh.materialIndex]);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
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

void
RenderManager::renderScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

  auto pDepthSV = graphMan.getMainDepthStencil();

  // Get render targets
  auto pMainTarget = graphMan.getMainRenderTargetView();
  auto& pShadowMap = m_targets[StringID("ShadowMap").getID()];
  auto& pShadowTemp = m_targets[StringID("ShadowTemp").getID()];
  auto& pDepthMap = m_targets[StringID("DepthMap").getID()];
  auto& pNormalMap = m_targets[StringID("NormalMap").getID()];
  auto& pColorMap = m_targets[StringID("ColorMap").getID()];
  auto& pPropMap = m_targets[StringID("PropMap").getID()];
  auto& pAOMap = m_targets[StringID("AOMap").getID()];
  auto& pHBlurMap = m_targets[StringID("HBlurMap").getID()];
  auto& pVBlurMap = m_targets[StringID("VBlurMap").getID()];
  auto& pSkyBoxMap = m_targets[StringID("SkyBoxMap").getID()];
  auto& pLightCMap = m_targets[StringID("LightCMap").getID()];
  auto& pHistogramMap = m_targets[StringID("HistogramMap").getID()];
  auto& pToneMap = m_targets[StringID("ToneMap").getID()];
  auto& pTempMap = m_targets[StringID("TempMap").getID()];
  auto& pLuminance = m_targets[StringID("LuminanceMap").getID()];
  auto& pPPMap = m_targets[StringID("PPMap").getID()];

  uint32 dispatchX = static_cast<uint32>((m_screenDimension.x + 32.0f) / 32.0f);
  uint32 dispatchY = static_cast<uint32>((m_screenDimension.y + 32.0f) / 32.0f);
  uint32 dispatchZ = 1;

  /*************************************/
  /*          Shadow Mapping           */
  /*************************************/
  Viewport shadowVP = {};
  shadowVP.width = m_sMapSize;
  shadowVP.height = m_sMapSize;
  shadowVP.minDepth = 0.0f;
  shadowVP.maxDepth = 1.0f;
  shadowVP.topLeftX = 0.0f;
  shadowVP.topLeftY = 0.0f;

  graphMan.setViewport(shadowVP);

  graphMan.clearDepthStencil(pShadowMap);
  graphMan.clearRenderTarget(pShadowTemp, LinearColor::BLACK);
  graphMan.setRenderTargets({ pShadowTemp }, pShadowMap);
  m_passes[StringID("SMapShader").getID()]->setPass();
  drawStaticMeshOnScene();

  cleanShaderObjects();

  /*************************************/
  /*              GBuffer              */
  /*************************************/
  Viewport normalVP = {};
  normalVP.width = m_screenDimension.x;
  normalVP.height = m_screenDimension.y;
  normalVP.minDepth = 0.0f;
  normalVP.maxDepth = 1.0f;
  normalVP.topLeftX = 0.0f;
  normalVP.topLeftY = 0.0f;

  graphMan.setViewport(normalVP);

  graphMan.clearRenderTarget(pDepthMap, LinearColor::BLACK);
  graphMan.clearRenderTarget(pNormalMap, LinearColor::BLACK);
  graphMan.clearRenderTarget(pColorMap, LinearColor::BLACK);
  graphMan.clearRenderTarget(pPropMap, LinearColor::BLACK);
  graphMan.clearDepthStencil(pDepthSV);

  graphMan.setRenderTargets({ pDepthMap, pNormalMap, pColorMap, pPropMap }, pDepthSV);
  m_passes[StringID("GBufferShader").getID()]->setPass();
  drawStaticMeshOnScene();

  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  graphMan.clearRenderTarget(pAOMap, LinearColor::BLACK);
  graphMan.setRenderTargets({ pAOMap }, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("AOShader").getID()]->setPass();

  graphMan.psSetShaderResourceView(pDepthMap, 0);
  graphMan.psSetShaderResourceView(pNormalMap, 1);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*          Horizontal Blur          */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HBlurShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pAOMap, 0);
  graphMan.setUnorderedAccessView(pHBlurMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*            Vetical Blur           */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("VBlurShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pHBlurMap, 0);
  graphMan.setUnorderedAccessView(pVBlurMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*             Lightning             */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LightCS").getID()]->setPass();

  graphMan.csSetShaderResourceView(pDepthMap, 0);
  graphMan.csSetShaderResourceView(pNormalMap, 1);
  graphMan.csSetShaderResourceView(pColorMap, 2);
  graphMan.csSetShaderResourceView(pPropMap, 3);
  graphMan.csSetShaderResourceView(pVBlurMap, 4);
  graphMan.csSetShaderResourceView(pShadowMap, 5);
  graphMan.setUnorderedAccessView(pLightCMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  graphMan.clearRenderTarget(pSkyBoxMap, LinearColor::BLACK);
  graphMan.setRenderTargets({ pSkyBoxMap }, pDepthSV);
  m_passes[StringID("SkyBoxShader").getID()]->setPass();

  SPtr<Texture2D> pSbTex = nullptr;

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
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ASBShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pLightCMap, 0);
  graphMan.csSetShaderResourceView(pNormalMap, 1);
  graphMan.csSetShaderResourceView(pSkyBoxMap, 2);
  graphMan.setUnorderedAccessView(pTempMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();
  //graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*             Luminance             */
  /*************************************/
  Viewport lumVP = {};
  lumVP.width = 512.0f;
  lumVP.height = 512.0f;
  lumVP.minDepth = 0.0f;
  lumVP.maxDepth = 1.0f;
  lumVP.topLeftX = 0.0f;
  lumVP.topLeftY = 0.0f;

  graphMan.setViewport(lumVP);

  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LuminanceShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pTempMap, 0);
  graphMan.setUnorderedAccessView(pLuminance, 0);

  uint32 dSize = static_cast<uint32>(512.0f / 32.0f);

  graphMan.dispatch(dSize, dSize, 1);

  cleanShaderObjects();

  /*************************************/
  /*               Bright              */
  /*************************************/


  ///////////////////////////////////////


  /*************************************/
  /*             Tone Map              */
  /*************************************/
  graphMan.setViewport(normalVP);

  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ToneMapShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pTempMap, 0);
  graphMan.setUnorderedAccessView(pToneMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*          Horizontal Blur          */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HBlurShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pLuminance, 0);
  graphMan.setUnorderedAccessView(pHBlurMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*            Vetical Blur           */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("VBlurShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pHBlurMap, 0);
  graphMan.setUnorderedAccessView(pVBlurMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*            Additive Mix           */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("AddMixShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pToneMap, 0);
  graphMan.csSetShaderResourceView(pVBlurMap, 1);
  graphMan.setUnorderedAccessView(pTempMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();
  //graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*            PostProcess            */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("PPShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pTempMap, 0);
  graphMan.setUnorderedAccessView(pPPMap, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*         Add to backbuffer         */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("FinalShader").getID()]->setPass();

  graphMan.psSetShaderResourceView(pPPMap, 0);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*             Histogram             */
  /*************************************/
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HistogramShader").getID()]->setPass();

  graphMan.csSetShaderResourceView(pPPMap, 0);
  graphMan.setUnorderedAccessView(pHistogramMap, 0);

  uint32 dx = static_cast<uint32>((256.0f + 32.0f) / 32.0f);
  uint32 dy = 3;
  uint32 dz = 1;

  graphMan.dispatch(dx, dy, dz);

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
  m_sMapSize = size;
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
