/*****************************************************************************/
/*
*  @file    shMeshComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
*  @brief   Mesh component classes.
*
*  Mesh component classes.
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
#include "shPrerequisitesCore.h"
#include "shComponent.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward declarations
*/
/*****************************************************************************/
class Material;
class StaticMeshResource;
class SkeletalMeshResource;
class SkeletonResource;
class Matrix4;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

/**
*  @brief Static Mesh Component class.
*/
class SH_CORE_EXPORT StaticMeshComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  StaticMeshComponent() : Component(COMPONENT_TYPE::kStaticMesh) {}

  /**
  *  @brief Default destructor.
  */
  ~StaticMeshComponent() = default;

  /**
  *  @brief Sets the mesh data.
  *
  *  @param SPtr<StaticMeshResource>& pMeshRes
  */
  void
  setMeshData(const SPtr<StaticMeshResource>& pMeshRes);

  /**
  *  @brief Returns the static mesh resource.
  * 
  *  @return SPtr<StaticMeshResource>&
  */
  FORCEINLINE const SPtr<StaticMeshResource>&
  getMeshResource() const;

  /**
  *  @brief Returns the vertex buffer.
  *
  *  @return SPtr<VertexBuffer>&
  */
  FORCEINLINE const SPtr<VertexBuffer>&
  getVertexBuffer() const;

  /**
  *  @brief Returns the index buffer.
  * 
  *  @return SPtr<IndexBuffer>&
  */
  FORCEINLINE const SPtr<IndexBuffer>&
  getIndexBuffer() const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Resource with mesh data.
  */
  SPtr<StaticMeshResource> m_mesh;

  /**
  *  @brief Vertex Buffer.
  */
  SPtr<VertexBuffer> m_vertexBuffer;

  /**
  *  @brief Index Buffer.
  */
  SPtr<IndexBuffer> m_indexBuffer;
};

/**
*  @brief Skeletal Mesh Component class.
*/
class SH_CORE_EXPORT SkeletalMeshComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  SkeletalMeshComponent() : Component(COMPONENT_TYPE::kSkeletalMesh) {}

  /**
  *  @brief Default destructor.
  */
  ~SkeletalMeshComponent() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Set the mesh data and creates the vertex and index buffer.
  * 
  *  @param SPtr<SkeletalMeshResource>& meshResource
  */
  void
  setMeshData(const SPtr<SkeletalMeshResource>& meshResource);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Resource with mesh data.
  */
  SPtr<SkeletalMeshResource> m_mesh;

  /**
  *  @brief Resource with skeleton data.
  */
  SPtr<SkeletonResource> m_skeletonData;

  /**
  *  @brief Vertex Buffer.
  */
  SPtr<VertexBuffer> m_vertexBuffer;

  /**
  *  @brief Index Buffer.
  */
  SPtr<IndexBuffer> m_indexBuffer;

  SPtr<ConstantBuffer> m_bonesBuffer;

  Vector<Matrix4> m_currentPose;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE const SPtr<StaticMeshResource>&
StaticMeshComponent::getMeshResource() const
{
  return m_mesh;
}

FORCEINLINE const SPtr<VertexBuffer>&
StaticMeshComponent::getVertexBuffer() const
{
  return m_vertexBuffer;
}

FORCEINLINE const SPtr<IndexBuffer>&
StaticMeshComponent::getIndexBuffer() const
{
  return m_indexBuffer;
}
}
