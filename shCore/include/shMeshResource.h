/*************************************************************/
/*
*  @file    shMeshResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
*  @brief   Basic mesh structure for models.
*
*  Basic mesh structure for models.
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

namespace shEngineSDK {
/**
*  @brief
*/
struct SH_CORE_EXPORT MeshResource
{
  uint32 numVertex = 0;
  uint32 numIndices = 0;
};

class SH_CORE_EXPORT StaticMeshResource : public Resource
{
 public:
  StaticMeshResource() = default;
  ~StaticMeshResource() = default;

  uint32 numVertex = 0;
  uint32 numIndices = 0;
  Vector<VertexData> vertices;
  Vector<uint32> indices;
};

class SH_CORE_EXPORT SkeletalMeshResource : public Resource
{
 public:
  SkeletalMeshResource() = default;
  ~SkeletalMeshResource() = default;

  Vector<MeshResource> meshes;
  Vector<VertexData> vertices;
  Vector<uint32> indices;
};
}
