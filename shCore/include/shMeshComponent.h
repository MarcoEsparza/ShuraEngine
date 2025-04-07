/*****************************************************************************/
/*
*  @file    shMeshComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
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
class StaticMeshUnionResource;
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

  void
  setMeshData(const SPtr<StaticMeshResource>& pMeshRes);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Mesh component Material.
  */
  //SPtr<Material> material;

  /**
  *  @brief Resource with mesh data.
  */
  SPtr<StaticMeshResource> meshData;

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
  *  @brief Meshes materials.
  */
  Vector<SPtr<Material>> materials;

  /**
  *  @brief Resource with mesh data.
  */
  SPtr<SkeletalMeshResource> meshData;

  /**
  *  @brief Resource with skeleton data.
  */
  SPtr<SkeletonResource> skeletonData;

  /**
  *  @brief Vertex Buffer.
  */
  SPtr<VertexBuffer> m_vertexBuffer;

  /**
  *  @brief Index Buffer.
  */
  SPtr<IndexBuffer> m_indexBuffer;

  SPtr<ConstantBuffer> m_bonesBuffer;

  Vector<Matrix4> currentPose;
};

class SH_CORE_EXPORT StaticMeshUnionComponent : public Component
{
public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE StaticMeshUnionComponent() : Component(COMPONENT_TYPE::kStaticMeshUnion) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~StaticMeshUnionComponent();

  /**
  *  @brief Sets the mesh data.
  * 
  *  @param SPtr<StaticMeshUnionResource>& pSMUResource
  */
  void
  setMeshData(const SPtr<StaticMeshUnionResource>& pSMUResource);

  /**
  *  @brief Returns the static mesh union resource.
  * 
  *  @return SPtr<StaticMeshUnionResource>&
  */
  FORCEINLINE const SPtr<StaticMeshUnionResource>&
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
 private:
  /**
  *  @brief Resource with mesh data.
  */
  SPtr<StaticMeshUnionResource> m_meshesData;

  /**
  *  @brief Vertex Buffer.
  */
  SPtr<VertexBuffer> m_vertexBuffer;

  /**
  *  @brief Index Buffer.
  */
  SPtr<IndexBuffer> m_indexBuffer;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE const SPtr<StaticMeshUnionResource>&
StaticMeshUnionComponent::getMeshResource() const
{
  return m_meshesData;
}

FORCEINLINE const SPtr<VertexBuffer>&
StaticMeshUnionComponent::getVertexBuffer() const
{
  return m_vertexBuffer;
}

FORCEINLINE const SPtr<IndexBuffer>&
StaticMeshUnionComponent::getIndexBuffer() const
{
  return m_indexBuffer;
}
}
