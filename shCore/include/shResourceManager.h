/*************************************************************/
/*
*  @file    shResourceManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Resource Manager module for loading all desired resources
*           from files.
*
*  Resource Manager module for loading all desired resources from files.
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
#include "shMatrix4.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace shEngineSDK {
struct Bone;
class SkeletalMeshResource;
class SkeletonResource;
class AnimationResource;
class StaticMeshUnionResource;

// TODO : Finish this for cache creation.
struct SH_CORE_EXPORT ResourceInfoHeader
{

};

// TODO : Finish this for cache creation.
struct SH_CORE_EXPORT ModelCacheHeader
{
  int32 numMeshes;
  int32 numVertices;
  int32 numIndices;
  int32 numPaths;
};

/**
*  @brief Resource Manager module for loading all desired resources from files.
*/
class SH_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  ResourceManager() = default;

  /**
  *  @brief Default destructor.
  */
  ~ResourceManager() = default;

 public:
  /**
  *  @brief Load resource from desired file, clasify it and create a cache
  *         for optimizing the next time the resource will be loaded.
  * 
  *  @param String& fileName.
  */
  void
  loadResourceFromFile(const String& fileName);

  /**
  *  @brief Get the desired resource by its name.
  * 
  *  @param String& resourceName
  */
  SPtr<Resource>
  getResource(const String& resourceName);

 private:
  /**
  *  @brief Checks if the resource is already loaded.
  * 
  *  @param String& fileName
  */
  SPtr<Resource>
  isResourceLoaded(const String& fileName);

  /**
  *  @brief Load a model from file and create a Static or Skeletal mesh.
  * 
  *  @param String& fileName
  */
  void
  loadModelFromFile(const String& fileName);

  /**
  *  @brief Load a image and creates a texture.
  * 
  *  @param String& fileName
  */
  void
  loadTextureFromFile(const String& fileName);

  /**
  *  @brief Load an animation.
  * 
  *  @note This function is unfinished.
  * 
  *  @param String& fileName
  */
  SPtr<Resource>
  loadAnimations(const String& fileName);

  void
  createStaticMesh(const String& fileName,
    const aiNode* node,
    const aiScene* scene);

  /**
  *  @brief If the model file is for static meshes, this function process all
  *         nodes on the loaded file scene.
  * 
  *  @param aiNode* node
  *  @param aiScene* scene
  */
  void
  proccessStaticMeshNode(const aiNode* node,
                         const aiScene* scene,
                         SPtr<StaticMeshUnionResource> meshUnion);

  /**
  *  @brief If the model file is for static meshes, this function process and
  *         creates all static meshes on the file.
  * 
  *  @param aiMesh* mesh
  */
  void
  proccessStaticMesh(const aiMesh* mesh,
    const aiScene* scene,
    SPtr<StaticMeshUnionResource> meshUnion);

  /**
  *  @brief Creates the skeletal mesh.
  * 
  *  @param aiScene* scene
  *  @param String& fileName
  */
  void
  createSkeletalMesh(const aiScene* scene, const String& fileName);

  /**
  *  @brief Process all nodes on the file scene for the skeletal mesh.
  * 
  *  @param const aiNode* node
  *  @param const aiScene* scene
  *  @param SPtr<SkeletalMeshResource>& skeletalMesh
  */
  void
  proccessSkeletalMeshNode(const aiNode* node,
                           const aiScene* scene,
                           SPtr<SkeletalMeshResource>& skeletalMesh,
                           SPtr<SkeletonResource>& skeleton);

  /**
  *  @brief Process the mesh on the file scene for the skeletal mesh.
  * 
  *  @param aiMesh* mesh
  *  @param aiScene* scene
  *  @param SPtr<SkeletalMeshResource>& skeletalMesh
  */
  void
  proccessSkeletalMesh(const aiMesh* mesh,
                       const aiScene* scene,
                       SPtr<SkeletalMeshResource>& skeletalMesh,
                       SPtr<SkeletonResource>& skeleton);

  /**
  *  @brief Process the Skeleton.
  * 
  *  @param const aiMesh* mesh
  *  @param aiScene* scene
  */
  void
  processSkeleton(const aiMesh* mesh,
                  const aiScene* scene,
                  SPtr<SkeletalMeshResource>& skeletalMesh,
                  SPtr<SkeletonResource>& skeleton);

  bool
  readSkeleton(Bone& boneOutput,
               aiNode* node,
               UMap<String, std::pair<int32, Matrix4>>& boneInfoTable);

  /**
  *  @brief Process the animation.
  * 
  *  @param aiScene* scene
  *  @param SPtr<AnimationResource>& animation
  *  @param uint32 index
  */
  void
  proccessAnimation(const aiScene* scene,
                    SPtr<AnimationResource>& animation,
                    uint32 index);

 private:
  /**
  *  @brief All the resources are storaged here.
  */
  UMap<String, SPtr<Resource>> m_loadedResources;
};
}
