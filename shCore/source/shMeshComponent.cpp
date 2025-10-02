/*****************************************************************************/
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
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
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

  uint32 totalVertices = 0;
  uint32 totalIndices = 0;
#pragma omp parallel for
  for (auto& mesh : pMeshRes->m_meshes) {
    totalVertices += static_cast<uint32>(mesh.numVertices);
    totalIndices += static_cast<uint32>(mesh.numIndices);
  }

  Vector<VertexData> vertices;
  vertices.reserve(totalVertices);
  Vector<uint32> indices;
  indices.reserve(totalIndices);

#pragma omp parallel for
  for (auto& mesh : pMeshRes->m_meshes) {
    vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
    indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
  }

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

void
SkeletalMeshComponent::onUpdate()
{
}
}
