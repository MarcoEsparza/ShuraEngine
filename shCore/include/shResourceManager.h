/*************************************************************/
/*
*  @file    shResourceManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/06
*  @brief   
*
*  
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
#include "shModule.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace shEngineSDK {
struct Bone;

struct SH_CORE_EXPORT ResourceInfoHeader
{

};

struct SH_CORE_EXPORT ModelCacheHeader
{
  int32 numMeshes;
  int32 numVertices;
  int32 numIndices;
  int32 numPaths;
};

class SH_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

 public:
  SPtr<Resource>
  loadResourceFromFile(const String& fileName, const RESOURCE_TYPE::E rType);

 private:
  SPtr<Resource>
  isResourceLoaded(const String& fileName);

  SPtr<Resource>
  loadModelFromFile(const String& fileName);

  SPtr<Resource>
  loadTextureFromFile(const String& fileName);

  SPtr<Resource>
  loadAnimations(const String& fileName);

  void
  proccessNode(const aiNode* node, const aiScene* scene);

  void
  proccessMesh(const aiMesh* mesh, const aiScene* scene);

  void
  processSkeleton(const aiMesh* mesh, const aiScene* scene);

  void
  proccessAnimation(const aiScene* scene);

  bool
  existCacheForModel(const String& fileName);

  SPtr<Resource>
  loadModelFromCache(const String& fileName);

  void
  createCacheForModel(const String& fileName);

 public:
  UMap<String, SPtr<Resource>> m_loadedResources;
};
}
