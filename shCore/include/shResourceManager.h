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
#include "shGraphicsManager.h"
#include "shModel.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace shEngineSDK {
struct SH_CORE_EXPORT ModelCacheHeader
{
  int32 numMeshes;
  int32 numVertices;
  int32 numIndices;
  int32 numPaths;
};

class SH_CORE_EXPORT ResourceManager
{
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

 public:
  void
  loadModelFromFile(const String& fileName);

  SPtr<Texture2D>
  loadTextureFromFile(const String& fileName);

  SPtr<Material>
  loadMaterial(const String& name);

  void
  loadAnimations(const String& fileName, const SPtr<Model>& model);

 private:
  SPtr<Model>
  isModelLoaded(const String& fileName);

  SPtr<Texture2D>
  isTextureLoaded(const String& fileName);

  SPtr<Material>
  isMaterialLoaded(const String& name);

  void
  proccessNode(const aiNode* node, const aiScene* scene, SPtr<Model>& model);

  void
  proccessMesh(const aiMesh* mesh, const aiScene* scene, SPtr<Model>& model);

  bool
  readSkeleton(Bone& boneOutput,
               aiNode* node,
               UMap<String, Pair<int32, Matrix4>> boneInfoTable);

  void
  proccessAnimation(const aiScene* scene);

  bool
  existCacheForModel(const String& fileName);

  void
  loadModelFromCache(const String& fileName);

  void
  createCacheForModel(const String& fileName, SPtr<Model>& model);

 public:
  UMap<String, SPtr<Texture2D>> m_loadedTextures;
  UMap<String, SPtr<Model>> m_loadedModels;
  UMap<String, SPtr<Material>> m_loadedMaterials;
};
}
