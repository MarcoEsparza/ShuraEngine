/*************************************************************/
/*
*  @file    shMeshResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
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
  *  @brief Mesh VertexData
  */
  Vector<VertexData> vertices;

  /**
  *  @brief Mesh indices
  */
  Vector<uint32> indices;

  /**
  *  @brief Number of vertex.
  */
  uint32 numVertex = 0;

  /**
  *  @brief Number of index.
  */
  uint32 numIndex = 0;

  /**
  *  @brief Material info.
  */
  SPtr<Material> material;
};

struct SkeletalMeshInfo
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 materialIndex = 0;
  String name;
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
  Vector<SkeletalMeshInfo> meshes;

  Vector<SPtr<Material>> materials;

  /**
  *  @brief Mesh VertexData
  */
  Vector<VertexData> vertices;

  /**
  *  @brief Mesh indices
  */
  Vector<uint32> indices;
};

class StaticMeshUnionResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  StaticMeshUnionResource() : Resource(RESOURCE_TYPE::kMeshUnion) {}

  /**
  *  @brief Default destructor.
  */
  ~StaticMeshUnionResource() = default;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Meshes.
  */
  Vector<SPtr<StaticMeshResource>> meshes;

  /**
  *  @brief Materials.
  */
  Vector<SPtr<Material>> materials;
};
}
