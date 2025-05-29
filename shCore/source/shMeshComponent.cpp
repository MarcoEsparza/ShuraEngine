/*************************************************************/
/*
*  @file    shMeshComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
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
#include "shMeshResource.h"
#include "shBuffers.h"

namespace shEngineSDK {
void
StaticMeshComponent::setMeshData(const SPtr<StaticMeshResource>& pMeshRes)
{
  GraphicsManager& graphMan = g_graphicsMan();
  m_mesh = pMeshRes;

  // Get all vertices
  Vector<VertexData> vertices;
  for (auto& mesh : m_mesh->m_meshes) {
    for (auto& vertex : mesh.vertices) {
      vertices.push_back(vertex);
    }
  }

  // Get all indices
  Vector<uint32> indices;
  for (auto& mesh : m_mesh->m_meshes) {
    for (auto& index : mesh.indices) {
      indices.push_back(index);
    }
  }

  // Create certex and index buffers
  m_vertexBuffer = graphMan.createVertexBuffer(vertices);
  m_indexBuffer = graphMan.createIndexBuffer(indices);
}

void
SkeletalMeshComponent::setMeshData(const SPtr<SkeletalMeshResource>& meshResource)
{
  GraphicsManager& graphMan = g_graphicsMan();
  m_mesh = meshResource;

  // Get all vertices
  Vector<VertexData> skeletalVertex;
  for (auto& vertex : m_mesh->m_vertices) {
    skeletalVertex.push_back(vertex);
  }

  // Get all indices
  Vector<uint32> skeletalIndex;
  for (auto& index : m_mesh->m_indices) {
    skeletalIndex.push_back(index);
  }

  // Create certex and index buffers
  m_vertexBuffer = graphMan.createVertexBuffer(skeletalVertex);
  m_indexBuffer = graphMan.createIndexBuffer(skeletalIndex);
}
}
