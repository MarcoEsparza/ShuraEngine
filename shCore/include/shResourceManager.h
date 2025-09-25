/*************************************************************/
/*
*  @file    shResourceManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
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
#include "shPath.h"

/*************************************************************/
/*
*  External forward declarations
*/
/*************************************************************/

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiAnimation;

namespace shEngineSDK {
 /*************************************************************/
/*
*  Internal forward declarations
*/
/*************************************************************/

struct Bone;
class Material;
class StaticMeshResource;
class SkeletalMeshResource;
class SkeletonResource;
class AnimationResource;
struct BoneHierarchy;

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
  ~ResourceManager();


  void
  onStartUp() override;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Load resource from desired file, clasify it and create a cache
  *         for optimizing the next time the resource will be loaded.
  *
  *  @param Path& filePath
  */
  SPtr<Resource>
  loadResourceFromFile(const Path& filePath);

  /**
  *  @brief Get the desired resource by its name.
  * 
  *  @param String& resourceName
  */
  SPtr<Resource>
  getResource(const String& resourceName);

  /**
  *  @brief Saves the given resource to an asset for better loading.
  *
  *  @param SPtr<Resource> pRes
  * 
  *  TODO: This function is not completed yet.
  */
  bool
  saveResourceToAsset(const SPtr<Resource> pRes);

  /**
  *  @brief Load a model from cache.
  *
  *  @param String& fileName
  */
  SPtr<Resource>
  loadModelFromCache(const String& fileName);

 private:
  /**
  *  @brief Checks if the resource is already loaded.
  * 
  *  @param String& fileName
  */
  SPtr<Resource>
  isResourceLoaded(const String& fileName);

  /**
  *  @brief Checks if the resource is already loaded.
  *
  *  @param Path& fileName
  */
  SPtr<Resource>
  isResourceLoaded(const Path& fileName);

  /**
  *  @brief Checks if the resource is already loaded on memory.
  *
  *  @param Path& fileName
  *  @param SPtr<Resource>& pRes
  */
  bool
  isResourceOnMemory(const Path& filePath, SPtr<Resource>& pRes);

  /**
  *  @brief Checks if there is a cache for resource.
  *
  *  @param Path& fileName
  *  @param SPtr<Resource>& pRes
  */
  bool
  isCacheForResource(const Path& filePath, SPtr<Resource>& pRes);

  /*************************************************************/
  /*
  *  Texture
  */
  /*************************************************************/

  /**
  *  @brief Load a image and creates a texture.
  * 
  *  @param String& fileName
  */
  SPtr<Resource>
  loadTextureFromFile(const String& fileName);

  /**
  *  @brief Load a dds image and creates a texture.
  *
  *  @param String& fileName
  */
  SPtr<Resource>
  loadTextureFromDDS(const String& filename);

  SPtr<Resource>
  loadCubeMapFromFile(const String& fileName);

  /*************************************************************/
  /*
  *  Model loading
  */
  /*************************************************************/

  /**
  *  @brief Load a model from file and create a Static or Skeletal mesh.
  * 
  *  @param String& fileName
  */
  SPtr<Resource>
  loadModelFromFile(const String& fileName);

  /**
  *  @brief Creates an engine material from an aiMaterial.
  *
  *  @param aiMaterial* pMat
  */
  SPtr<Material>
  createMaterialFromFile(const aiMaterial* pMat);

  SPtr<Material>
  isMaterialLoaded(const String& materialName);

  /*************************************************************/
  /*
  *  Static Mesh
  */
  /*************************************************************/

  /**
  *  @brief Creates the static mesh.
  *
  *  @param String& fileName
  *  @param aiNode* node
  *  @param aiScene* scene
  */
  SPtr<Resource>
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
                         SPtr<StaticMeshResource>& currentMesh);

  /**
  *  @brief If the model file is for static meshes, this function process and
  *         creates all static meshes on the file.
  * 
  *  @param aiMesh* mesh
  */
  void
  proccessStaticMesh(const aiMesh* mesh,
                     const aiScene* scene,
                     SPtr<StaticMeshResource>& currentMesh);

  /*************************************************************/
  /*
  *  Skeletal mesh and skeleton
  */
  /*************************************************************/

  /**
  *  @brief Creates the skeletal mesh.
  * 
  *  @param aiScene* scene
  *  @param String& fileName
  */
  SPtr<Resource>
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
                           SPtr<BoneHierarchy> boneNode,
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

  /*************************************************************/
  /*
  *  Animation
  */
  /*************************************************************/

  void
  checkModelAnimations(const aiScene* scene, SPtr<SkeletonResource>& skeleton);

  /**
  *  @brief Process the animation.
  * 
  *  @param aiScene* scene
  *  @param SPtr<AnimationResource>& animation
  *  @param uint32 index
  */
  void
  proccessAnimation(const aiAnimation* anim,
                    SPtr<AnimationResource>& outAnimation);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief All the resources are storaged here.
  */
  UMap<String, SPtr<Resource>> m_loadedResources;

  /*************************************************************/
  /*
  *  Static Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief All supported model extensions.
  */
  static const Vector<String> MODEL_EXTENSIONS;

  /**
  *  @brief All supported image extensions.
  */
  static const Vector<String> IMAGE_EXTENSIONS;
};

/**
*  @brief Easier way to access the Time module.
*/
SH_CORE_EXPORT ResourceManager&
g_resourceMan();
}
