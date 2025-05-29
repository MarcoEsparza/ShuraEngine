/*************************************************************/
/*
*  @file    shMeshResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/22
*  @brief   Mesh resource classes.
*
*  Mesh resource classes.
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
#include "shResource.h"
#include "shGraphicTypes.h"
#include "shMaterial.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT MeshData
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 materialIndex = 0;
  String name;
  Vector<VertexData> vertices;
  Vector<uint32> indices;
};

struct SH_CORE_EXPORT SkeletalMeshData
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 materialIndex = 0;
  String name;
};

/**
*  @brief Static mesh data.
*/
class SH_CORE_EXPORT StaticMeshResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  StaticMeshResource() : Resource(RESOURCE_TYPE::kStaticMesh) {}

  /**
  *  @brief Default destructor.
  */
  ~StaticMeshResource() = default;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Meshes info.
  */
  Vector<MeshData> m_meshes;

  /**
  *  @brief Materials info.
  */
  Vector<SPtr<Material>> m_materials;
};

/**
*  @brief Skeletal mesh data.
*/
class SH_CORE_EXPORT SkeletalMeshResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  SkeletalMeshResource() : Resource(RESOURCE_TYPE::kSkeletalMesh) {}

  /**
  *  @brief Default destructor.
  */
  ~SkeletalMeshResource() = default;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Meshes info.
  */
  Vector<SkeletalMeshData> m_meshes;

  /**
  *  @brief Materials info.
  */
  Vector<SPtr<Material>> m_materials;

  /**
  *  @brief Mesh VertexData
  */
  Vector<VertexData> m_vertices;

  /**
  *  @brief Mesh indices
  */
  Vector<uint32> m_indices;
};
}
