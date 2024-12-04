/*************************************************************/
/*
*  @file    shMeshComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
*  @brief   Mesh component classes.
*
*  Mesh component classes.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shMeshComponent.h"
#include "shGraphicsManager.h"

namespace shEngineSDK {
StaticMeshComponent::StaticMeshComponent()
{
  type = COMPONENT_TYPE::kStaticMesh;
  name = "StaticMeshComponent";
}

SkeletalMeshComponent::SkeletalMeshComponent()
{
  type = COMPONENT_TYPE::kSkeletalMesh;
  name = "SkeletalMeshComponent";
}

void
SkeletalMeshComponent::setMeshData(const SPtr<SkeletalMeshResource>& meshResource)
{
  meshData = meshResource;
  Vector<VertexData> skeletalVertex;
  Vector<uint32> skeletalIndex;

  for (auto vertex : meshData->vertices) {
    skeletalVertex.push_back(vertex);
  }

  for (auto index : meshData->indices) {
    skeletalIndex.push_back(index);
  }

  m_vertexBuffer = GraphicsManager::instance().createVertexBuffer(skeletalVertex);
  m_indexBuffer = GraphicsManager::instance().createIndexBuffer(skeletalIndex);
}

}
