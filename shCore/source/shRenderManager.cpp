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
#include "shMeshComponent.h"
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shPass.h"

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
  for (auto& meshUnion : meshList) {
    g_graphicsMan().setVertexBuffers(meshUnion->getVertexBuffer());
    g_graphicsMan().setIndexBuffers(meshUnion->getIndexBuffer());

    uint32 indexCount = 0;
    uint32 vertexCount = 0;
    for (auto& mesh : meshUnion->getMeshResource()->meshes) {
      auto mat = reinterpret_pointer_cast<PBRMaterial>(mesh->material);
      setResourceViewFromPBRMaterial(mat);
      g_graphicsMan().drawIndexed(mesh->numIndex, indexCount, vertexCount);
      indexCount += mesh->numIndex;
      vertexCount += mesh->numVertex;
    }
  }
}

void
RenderManager::setResourceViewFromPBRMaterial(const SPtr<PBRMaterial>& pMat)
{
  if (pMat->m_type != MATERIAL_TYPE::kPBR) {
    return;
  }

  if (pMat->baseColor) {
    g_graphicsMan().setShaderResourceView(pMat->baseColor);
  }

  if (pMat->normal) {
    g_graphicsMan().setShaderResourceView(pMat->normal, 1);
  }

  if (pMat->metallic) {
    g_graphicsMan().setShaderResourceView(pMat->metallic, 2);
  }

  if (pMat->roughness) {
    g_graphicsMan().setShaderResourceView(pMat->roughness, 3);
  }

  if (pMat->ao) {
    g_graphicsMan().setShaderResourceView(pMat->ao, 4);
  }
}

RenderManager& g_renderMan()
{
  return RenderManager::instance();
}
}
