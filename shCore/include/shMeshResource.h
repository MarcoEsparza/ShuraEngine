/*************************************************************/
/*
*  @file    shMeshResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Meshes classes.
*
*  Meshes classes.
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

namespace shEngineSDK {
/**
*  @brief Mesh resource base class.
*/
class SH_CORE_EXPORT MeshResource : public Resource
{
public:
  /**
  *  @brief Default constructor.
  */
  MeshResource() = default;

  /**
  *  @brief Default destructor.
  */
  ~MeshResource() = default;

  /**
  *  @brief Mesh VertexData
  */
  Vector<VertexData> vertices;

  /**
  *  @brief Mesh indices
  */
  Vector<uint32> indices;
};

/**
*  @brief Static mesh data.
*/
class SH_CORE_EXPORT StaticMeshResource : public MeshResource
{
 public:
  /**
  *  @brief Default constructor.
  */
  StaticMeshResource() { type = RESOURCE_TYPE::kStaticMesh; }

  /**
  *  @brief Default destructor.
  */
  ~StaticMeshResource() = default;

  /**
  *  @brief Number of vertex.
  */
  uint32 numVertex = 0;

  /**
  *  @brief Number of index.
  */
  uint32 numIndex = 0;
};

/**
*  @brief Skeletal mesh data.
*/
class SH_CORE_EXPORT SkeletalMeshResource : public MeshResource
{
 public:
  /**
  *  @brief Default constructor.
  */
  SkeletalMeshResource() { type = RESOURCE_TYPE::kSkeletalMesh; }

  /**
  *  @brief Default destructor.
  */
  ~SkeletalMeshResource() = default;

  /**
  *  @brief Number of vertices.
  */
  Vector<uint32> numVertices;

  /**
  *  @brief Number of indices.
  */
  Vector<uint32> numIndices;

  /**
  *  @brief Mesh names.
  */
  Vector<String> meshNames;

  /**
  *  @brief Number of meshes.
  */
  uint32 numMeshes = 0;
};
}
