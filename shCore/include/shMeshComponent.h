/*************************************************************/
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
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shComponent.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Forward declarations
*/
/*************************************************************/
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

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Mesh component Material.
  */
  //SPtr<Material> material;

  /**
  *  @brief Resource with mesh data.
  */
  SPtr<StaticMeshResource> meshData;
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

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Set the mesh data and creates the vertex and index buffer.
  * 
  *  @param SPtr<SkeletalMeshResource>& meshResource
  */
  void
  setMeshData(const SPtr<SkeletalMeshResource>& meshResource);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
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
}
