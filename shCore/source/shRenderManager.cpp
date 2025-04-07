/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shPass.h"
#include "shMatrix4.h"

using std::reinterpret_pointer_cast;

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

  /*BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = false;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;*/

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
  m_targets[name] = pRTV;
}

void
RenderManager::createPass(const String& passName)
{
  auto pPass = make_shared<Pass>();
  m_passes[passName] = pPass;
}

SPtr<Pass>
RenderManager::getPass(const String& passName)
{
  auto pPass = m_passes.find(passName);

  if (pPass != m_passes.end()) {
      return (*pPass).second;
  }

  return nullptr;
}

void
RenderManager::setPass(const SPtr<Pass>& pPass, const String& passName)
{
  m_passes[passName] = pPass;
}

void
RenderManager::makePass(const String& passName)
{
  auto& pPass = m_passes[passName];
  pPass->setPass();
}

void RenderManager::recompileShaders()
{
  for(auto& pPass : m_passes) {
    pPass.second->compileShader();
  }
}

void
RenderManager::drawSMUInScene(const Vector<SPtr<StaticMeshUnionComponent>>& meshList)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (auto& meshUnion : meshList) {
    graphMan.setVertexBuffers(meshUnion->getVertexBuffer());
    graphMan.setIndexBuffers(meshUnion->getIndexBuffer());

    uint32 indexCount = 0;
    uint32 vertexCount = 0;
    for (auto& mesh : meshUnion->getMeshResource()->meshes) {
      auto mat = reinterpret_pointer_cast<Material>(mesh->material);
      setResourceViewFromPBRMaterial(mat);
      graphMan.drawIndexed(mesh->numIndex, indexCount, vertexCount);
      indexCount += mesh->numIndex;
      vertexCount += mesh->numVertex;
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
    graphMan.setShaderResourceView(pMat->baseColor);
  }

  if (pMat->normal) {
    graphMan.setShaderResourceView(pMat->normal, 1);
  }

  if (pMat->metallic) {
    graphMan.setShaderResourceView(pMat->metallic, 2);
  }

  if (pMat->roughness) {
    graphMan.setShaderResourceView(pMat->roughness, 3);
  }

  if (pMat->ao) {
    graphMan.setShaderResourceView(pMat->ao, 4);
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

  /*************************************/
  /*          Shadow Mapping           */
  /*************************************/
  // Shadow Mapping
  Viewport shadowVP = {};
  shadowVP.width = m_sMapSize;
  shadowVP.height = m_sMapSize;
  shadowVP.minDepth = 0.0f;
  shadowVP.maxDepth = 1.0f;
  shadowVP.topLeftX = 0.0f;
  shadowVP.topLeftY = 0.0f;

  graphMan.setViewport(shadowVP);

  graphMan.clearDepthStencil(m_targets["ShadowMap"]);
  graphMan.setRenderTargets({ nullptr }, m_targets["ShadowMap"]);
  makePass("SMapShader");

  auto& pistolGO = scene.getGameObjectList()[0];
  auto& sponzaGO = scene.getGameObjectList()[1];
  SPtr<StaticMeshUnionComponent> pistolMesh;
  SPtr<StaticMeshUnionComponent> sponzaMesh;
  for (auto& component : pistolGO->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
      pistolMesh = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
    }
  }
  for (auto& component : sponzaGO->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
      sponzaMesh = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
    }
  }
  graphMan.vsSetConstantBuffers(m_pModelTransform, 1);

  Transform modelT = pistolGO->transform.getTransform();
  graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));
  drawSMUInScene({ pistolMesh });

  modelT = sponzaGO->transform.getTransform();
  graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));
  //graphMan.vsSetConstantBuffers(m_pModelTransform, 1);
  drawSMUInScene({ sponzaMesh });

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

  graphMan.clearRenderTarget(m_targets["DepthMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_targets["NormalMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_targets["ColorMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_targets["PropMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearDepthStencil(pDepthSV);

  graphMan.setRenderTargets({ m_targets["DepthMap"],
                              m_targets["NormalMap"],
                              m_targets["ColorMap"],
                              m_targets["PropMap"] },
                            pDepthSV);
  makePass("BasicShader");

  modelT = pistolGO->transform.getTransform();
  graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));
  graphMan.vsSetConstantBuffers(m_pModelTransform, 1);
  drawSMUInScene({ pistolMesh });

  modelT = sponzaGO->transform.getTransform();
  graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));
  graphMan.vsSetConstantBuffers(m_pModelTransform, 1);
  drawSMUInScene({ sponzaMesh });

  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  graphMan.clearRenderTarget(m_targets["AOMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_targets["AOMap"] }, pDepthSV);
  makePass("AOShader");

  graphMan.setShaderResourceView(m_targets["DepthMap"], 0);
  graphMan.setShaderResourceView(m_targets["NormalMap"], 1);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*          Horizontal Blur          */
  /*************************************/
  graphMan.clearRenderTarget(m_targets["HBlurMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_targets["HBlurMap"] }, pDepthSV);
  makePass("HBlurShader");

  graphMan.setShaderResourceView(m_targets["AOMap"], 0);

  graphMan.draw(3, 0);

  cleanShaderResourceView();

  /*************************************/
  /*            Vetical Blur           */
  /*************************************/
  graphMan.clearRenderTarget(m_targets["VBlurMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_targets["VBlurMap"] }, pDepthSV);
  makePass("VBlurShader");

  graphMan.setShaderResourceView({ m_targets["HBlurMap"] }, 0);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*             Lightning             */
  /*************************************/
  graphMan.setRenderTargets({ m_targets["MainTarget"] }, pDepthSV);
  makePass("LightningShader");

  graphMan.setShaderResourceView(m_targets["DepthMap"], 0);
  graphMan.setShaderResourceView(m_targets["NormalMap"], 1);
  graphMan.setShaderResourceView(m_targets["ColorMap"], 2);
  graphMan.setShaderResourceView(m_targets["PropMap"], 3);
  graphMan.setShaderResourceView(m_targets["VBlurMap"], 4);
  graphMan.setShaderResourceView(m_targets["ShadowMap"], 5);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  auto& skyBoxGO = scene.getGameObjectList()[2];
  graphMan.clearRenderTarget(m_targets["SkyBoxMap"], LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_targets["SkyBoxMap"] }, nullptr);
  makePass("SkyBoxShader");

  for (auto& component : skyBoxGO->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
      auto pMesh = reinterpret_pointer_cast<StaticMeshComponent>(component);
      graphMan.setVertexBuffers(pMesh->m_vertexBuffer);
      graphMan.setIndexBuffers(pMesh->m_indexBuffer);

      auto& pMat = pMesh->meshData->material;
      graphMan.setShaderResourceView(pMat->baseColor, 0);

      graphMan.drawIndexed(pMesh->meshData->numIndex, 0, 0);
    }
  }

  cleanShaderObjects();

  graphMan.setRenderTargets({ m_targets["MainTarget"] }, pDepthSV);
  makePass("FinalShader");

  graphMan.setShaderResourceView(m_targets["NormalMap"], 0);
  graphMan.setShaderResourceView(m_targets["SkyBoxMap"], 1);

  graphMan.draw(3, 0);
}

void
RenderManager::cleanShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    graphMan.setShaderResourceView(nullptr, i);
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
RenderManager::cleanShaderObjects()
{
  cleanShaderResourceView();
  cleanVSConstantBuffers();
  cleanPSConstantBuffers();
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
