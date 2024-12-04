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
#include "shMaterial.h"
#include "shMeshResource.h"
#include "shBuffers.h"

namespace shEngineSDK {
/**
*  @brief Static Mesh Component class.
*/
class SH_CORE_EXPORT StaticMeshComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  StaticMeshComponent();

  /**
  *  @brief Default destructor.
  */
  ~StaticMeshComponent() = default;

  /**
  *  @brief Mesh component Material.
  */
  SPtr<Material> material;

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
  SkeletalMeshComponent();

  /**
  *  @brief Default destructor.
  */
  ~SkeletalMeshComponent() = default;

  /**
  *  @brief Set the mesh data and creates the vertex and index buffer.
  * 
  *  @param SPtr<SkeletalMeshResource>& meshResource
  */
  void
  setMeshData(const SPtr<SkeletalMeshResource>& meshResource);

  /**
  *  @brief Meshes materials.
  */
  Vector<SPtr<Material>> materials;

  /**
  *  @brief Resource with mesh data.
  */
  SPtr<SkeletalMeshResource> meshData;

  /**
  *  @brief Vertex Buffer.
  */
  SPtr<VertexBuffer> m_vertexBuffer;

  /**
  *  @brief Index Buffer.
  */
  SPtr<IndexBuffer> m_indexBuffer;
};
}
