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
#include "shMeshResource.h"
#include "shBuffers.h"

namespace shEngineSDK {
StaticMeshUnionComponent::~StaticMeshUnionComponent()
{
  m_meshesData->~StaticMeshUnionResource();
  m_meshesData.reset();

  m_vertexBuffer->~VertexBuffer();
  m_vertexBuffer.reset();

  m_indexBuffer->~IndexBuffer();
  m_indexBuffer.reset();
}

void
StaticMeshComponent::setMeshData(const SPtr<StaticMeshResource>& pMeshRes)
{
  GraphicsManager& graphMan = g_graphicsMan();

  meshData = pMeshRes;
  m_vertexBuffer = graphMan.createVertexBuffer(meshData->vertices);
  m_indexBuffer = graphMan.createIndexBuffer(meshData->indices);
}

void
SkeletalMeshComponent::setMeshData(const SPtr<SkeletalMeshResource>& meshResource)
{
  GraphicsManager& graphMan = g_graphicsMan();

  meshData = meshResource;
  Vector<VertexData> skeletalVertex;
  Vector<uint32> skeletalIndex;

  for (auto& vertex : meshData->vertices) {
    skeletalVertex.push_back(vertex);
  }

  for (auto& index : meshData->indices) {
    skeletalIndex.push_back(index);
  }

  m_vertexBuffer = graphMan.createVertexBuffer(skeletalVertex);
  m_indexBuffer = graphMan.createIndexBuffer(skeletalIndex);
}

void
StaticMeshUnionComponent::setMeshData(const SPtr<StaticMeshUnionResource>& pSMUResource)
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_meshesData = pSMUResource;
  Vector<VertexData> vertices;
  Vector<uint32> indices;

  for (auto& mesh : m_meshesData->meshes) {
    for (auto& vertex : mesh->vertices) {
      vertices.push_back(vertex);
    }

    for (auto& index : mesh->indices) {
      indices.push_back(index);
    }
  }

  m_vertexBuffer = graphMan.createVertexBuffer(vertices);
  m_indexBuffer = graphMan.createIndexBuffer(indices);
}
}
