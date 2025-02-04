/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/04
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

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
RenderManager::drawStaticMeshUnionInScene(const Vector<SPtr<StaticMeshUnionComponent>>&
                                          meshList)
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

  g_graphicsMan().setShaderResourceView(pMat->baseColor);
  g_graphicsMan().setShaderResourceView(pMat->normal, 1);
  g_graphicsMan().setShaderResourceView(pMat->metallic, 2);
  g_graphicsMan().setShaderResourceView(pMat->roughness, 3);
  g_graphicsMan().setShaderResourceView(pMat->ao, 4);
}

RenderManager& g_renderMan()
{
  return RenderManager::instance();
}
}
