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

  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  m_pBasicBS = graphMan.createBlendState(blendDesc);

  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Transform));
}

void
RenderManager::addRenderTarget(const SPtr<Texture2D>& pRTV, const String& name)
{
  StringID strID(name);
  m_targets[strID.getID()] = pRTV;
}

SPtr<Texture2D>
RenderManager::getRenderTargetByName(const String& name)
{
  StringID strID(name);
  auto pRTV = m_targets.find(strID.getID());

  if (pRTV != m_targets.end()) {
    return (*pRTV).second;
  }

  return nullptr;
}

void
RenderManager::clearRenderTargetByName(const String& name, const LinearColor& color)
{
  GraphicsManager& graphMan = g_graphicsMan();
  auto pRTV = getRenderTargetByName(name);

  if (pRTV) {
    graphMan.clearRenderTarget(pRTV, color);
  }
}

void
RenderManager::setRenderTargetsByName(const Vector<String>& names,
                                      const SPtr<Texture2D>& pDepthS)
{
  GraphicsManager& graphMan = g_graphicsMan();
  Vector<SPtr<Texture2D>> pRTVs;

  for (auto& name : names) {
    auto pTarget = getRenderTargetByName(name);
    if (pTarget) {
      pRTVs.push_back(pTarget);
    }
  }

  graphMan.setRenderTargets(pRTVs, pDepthS);
}

void
RenderManager::createPass(const String& passName)
{
  auto pPass = sh_makeShared<Pass>();
  StringID strID(passName);
  m_passes[strID.getID()] = pPass;
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

void
RenderManager::addPass(const SPtr<Pass>& pPass, const String& passName)
{
  StringID strID(passName);
  m_passes[strID.getID()] = pPass;
}

void
RenderManager::setPassByName(const String& passName)
{
  StringID strID(passName);
  m_passes[strID.getID()]->setPass();
}

void RenderManager::recompileShaders()
{
  for(auto& pPass : m_passes) {
    pPass.second->compileShader();
  }
}

void
RenderManager::drawStaticMeshOnScene(const SceneGraph& scene)
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.vsSetConstantBuffers(m_pModelTransform, 1);

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
  auto pShadowMap = getRenderTargetByName("ShadowMap");
  auto pDepthMap = getRenderTargetByName("DepthMap");
  auto pNormalMap = getRenderTargetByName("NormalMap");
  auto pColorMap = getRenderTargetByName("ColorMap");
  auto pPropMap = getRenderTargetByName("PropMap");
  auto pAOMap = getRenderTargetByName("AOMap");
  auto pHBlurMap = getRenderTargetByName("HBlurMap");
  auto pVBlurMap = getRenderTargetByName("VBlurMap");
  auto pSkyBoxMap = getRenderTargetByName("SkyBoxMap");
  //auto pCHBlur = getRenderTargetByName("CHBlurMap");
  auto pLightCMap = getRenderTargetByName("LightCMap");

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
  setRenderTargetsByName({ "ShadowTemp" }, pShadowMap);
  setPassByName("SMapShader");
  drawStaticMeshOnScene(scene);

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

  clearRenderTargetByName("DepthMap");
  clearRenderTargetByName("NormalMap");
  clearRenderTargetByName("ColorMap");
  clearRenderTargetByName("PropMap");
  graphMan.clearDepthStencil(pDepthSV);

  setRenderTargetsByName({ "DepthMap", "NormalMap", "ColorMap", "PropMap" }, pDepthSV);
  setPassByName("GBufferShader");
  drawStaticMeshOnScene(scene);

  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  //clearRenderTargetByName("AOMap");
  //setRenderTargetsByName({ "AOMap" }, pDepthSV);
  //setPassByName("PlaneShader");

  setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("AOShader");

  graphMan.csSetShaderResourceView(pDepthMap, 0);
  graphMan.csSetShaderResourceView(pNormalMap, 1);
  graphMan.setUnorderedAccessView(pAOMap, 0);

  graphMan.dispatch(static_cast<uint32>(m_screenDimension.x / 16.0f),
                    static_cast<uint32>(m_screenDimension.y / 16.0f),
                    1);
  //graphMan.draw(3, 0);

  cleanShaderObjects();
  graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*          Horizontal Blur          */
  /*************************************/
  //clearRenderTargetByName("HBlurMap");
  //setRenderTargetsByName({ "HBlurMap" }, pDepthSV);
  //setPassByName("PlaneShader");

  setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("HBlurShader");

  //graphMan.psSetShaderResourceView(pAOMap, 0);
  graphMan.csSetShaderResourceView(pAOMap, 0);
  graphMan.setUnorderedAccessView(pHBlurMap, 0);

  //graphMan.draw(3, 0);
  graphMan.dispatch(static_cast<uint32>(m_screenDimension.x / 16.0f),
                    static_cast<uint32>(m_screenDimension.y / 16.0f),
                    1);

  cleanShaderObjects();
  graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*            Vetical Blur           */
  /*************************************/
  //clearRenderTargetByName("VBlurMap");
  //setRenderTargetsByName({ "VBlurMap" }, pDepthSV);
  //setPassByName("PlaneShader");

  setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("VBlurShader");

  //graphMan.psSetShaderResourceView({ pHBlurMap }, 0);
  graphMan.csSetShaderResourceView(pHBlurMap, 0);
  graphMan.setUnorderedAccessView(pVBlurMap, 0);

  //graphMan.draw(3, 0);
  graphMan.dispatch(static_cast<uint32>(m_screenDimension.x / 16.0f),
                    static_cast<uint32>(m_screenDimension.y / 16.0f),
                    1);

  cleanShaderObjects();
  graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*             Lightning             */
  /*************************************/
  /*setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("PlaneShader");
  setPassByName("LightningShader");

  graphMan.psSetShaderResourceView(pDepthMap, 0);
  graphMan.psSetShaderResourceView(pNormalMap, 1);
  graphMan.psSetShaderResourceView(pColorMap, 2);
  graphMan.psSetShaderResourceView(pPropMap, 3);
  graphMan.psSetShaderResourceView(pVBlurMap, 4);
  graphMan.psSetShaderResourceView(pShadowMap, 5);

  graphMan.draw(3, 0);

  cleanShaderObjects();*/

  setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("LightCS");

  graphMan.csSetShaderResourceView(pDepthMap, 0);
  graphMan.csSetShaderResourceView(pNormalMap, 1);
  graphMan.csSetShaderResourceView(pColorMap, 2);
  graphMan.csSetShaderResourceView(pPropMap, 3);
  graphMan.csSetShaderResourceView(pVBlurMap, 4);
  graphMan.csSetShaderResourceView(pShadowMap, 5);
  graphMan.setUnorderedAccessView(pLightCMap, 0);

  graphMan.dispatch(static_cast<uint32>(m_screenDimension.x / 16.0f),
                    static_cast<uint32>(m_screenDimension.y / 16.0f),
                    1);

  cleanShaderObjects();
  graphMan.setUnorderedAccessView(nullptr, 0);

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  clearRenderTargetByName("SkyBoxMap");
  setRenderTargetsByName({ "SkyBoxMap" }, pDepthSV);
  setPassByName("SkyBoxShader");

  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkyBox) {
        auto pSkyBox = sh_reinterpretPCast<SkyBoxComponent>(component);
        graphMan.setVertexBuffers(pSkyBox->getVertexBuffer());
        graphMan.setIndexBuffers(pSkyBox->getIndexBuffer());
        graphMan.psSetShaderResourceView(pSkyBox->getMaterial()->baseColor, 0);

        uint32 numIndices = static_cast<uint32>(pSkyBox->getIndices().size());
        graphMan.drawIndexed(numIndices, 0, 0);
      }
    }
  }

  cleanShaderObjects();

  /*************************************/
  /*            Add Sky Box            */
  /*************************************/
  setRenderTargetsByName({ "MainTarget" }, pDepthSV);
  setPassByName("PlaneShader");
  setPassByName("FinalShader");

  graphMan.psSetShaderResourceView(pNormalMap, 0);
  graphMan.psSetShaderResourceView(pSkyBoxMap, 1);
  graphMan.psSetShaderResourceView(pLightCMap, 2);

  graphMan.draw(3, 0);

  cleanShaderObjects();
  graphMan.setUnorderedAccessView(nullptr, 0);
}

void
RenderManager::cleanPSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    graphMan.psSetShaderResourceView(nullptr, i);
  }
}

void
RenderManager::cleanCSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    graphMan.csSetShaderResourceView(nullptr, i);
  }
}

void
RenderManager::cleanCSUAView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    graphMan.setUnorderedAccessView(nullptr, i);
  }
}

void
RenderManager::cleanVSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    graphMan.vsSetConstantBuffers(nullptr, i);
  }
}

void
RenderManager::cleanPSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    graphMan.psSetConstantBuffers(nullptr, i);
  }
}

void
RenderManager::cleanGSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    graphMan.gsSetConstantBuffers(nullptr, i);
  }
}

void
RenderManager::cleanCSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    graphMan.csSetConstantBuffers(nullptr, i);
  }
}

void
RenderManager::cleanShaderObjects()
{
  cleanPSShaderResourceView();
  cleanCSShaderResourceView();
  //cleanCSUAView();
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
RenderManager::setScreenDimensions(const Vector2& screenD)
{
  m_screenDimension = screenD;
}

RenderManager& g_renderMan()
{
  return RenderManager::instance();
}
}
