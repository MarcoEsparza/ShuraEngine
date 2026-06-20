/*****************************************************************************/
/*
*  @file    shMeshCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief   Mesh Codec for decoding and encoding mesh files.
*
*  Mesh Codec for decoding and encoding mesh files.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include <shPrerequisitesCore.h>
#include <shCodec.h>

/*****************************************************************************/
/*
*  External forward declarations
*/
/*****************************************************************************/

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiAnimation;

namespace shEngineSDK {
/****************************************************************************/
/*
*  Internal forward declarations
*/
/*****************************************************************************/
class Resource;
class Material;
class StaticMeshResource;
class SkeletalMeshResource;
class SkeletonResource;
class AnimationResource;
struct BoneHierarchy;

/**
*  @brief Mesh Codec for decoding and encoding mesh files.
*/
class MeshCodec : public Codec
{
 public:
  MeshCodec() = default;
  ~MeshCodec() = default;

  /**
  *  @brief Gets the unique identifier (UID) of the codec.
  * 
  *  @return UID: Unique identifier of the codec.
  */
  UID
  getUID() const override;

  /**
  *  @brief Gets the list of supported file extensions by the codec.
  * 
  *  @return Vector<String>: List of supported file extensions by the codec,
  *                          including the dot.
  */
  Vector<String>
  getSupportedExtensions() const override;

  /**
  *  @brief Decodes the mesh file at the given file path and creates the resource.
  * 
  *  @param const Path& filePath: File path of the mesh file to decode.
  * 
  *  @return bool: True if the decoding was successful, false if it failed.
  */
  bool
  decode(const Path& filePath) const override;

  /**
  *  @brief Encodes the mesh resource with the given name and saves it to the given file path.
  * 
  *  @param const String& objName: Name of the mesh resource to encode.
  *  @param const Path& saveFilePath: File path to save the encoded mesh resource to.
  * 
  *  @return bool: True if the encoding was successful, false if it failed.
  */
  bool
  encode(const String& objName, const Path& saveFilePath) const override;

 private:
  /**
  *  @brief Decodes the mesh file at the given file path and creates the resource.
  * 
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param const Path& filePath: File path of the mesh file to decode,
  *                               used for material loading.
  *  @return SPtr<Resource>: Shared pointer to the created mesh resource,
  *                          or nullptr if the decoding failed.
  */
  _NODISCARD SPtr<Resource>
  decodeStaticMesh(const aiScene* pScene, const Path& filePath) const;

  /**
  *  @brief Decodes the mesh file at the given file path and creates the resource.
  * 
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param const Path& filePath: File path of the mesh file to decode,
  *                               used for material loading.
  * 
  *  @return SPtr<Resource>: Shared pointer to the created mesh resource,
  *                          or nullptr if the decoding failed.
  */
  _NODISCARD SPtr<Resource>
  decodeSkeletalMesh(const aiScene* pScene, const Path& filePath) const;

  /**
  *  @brief Gets the material from the given aiMaterial and creates the resource.
  * 
  *  @param const aiMaterial* pMat: Pointer to the aiMaterial containing the
  *                                 material data to decode.
  *  @param const Path& meshPath: File path of the mesh file being decoded,
  *                               used for texture loading.
  * 
  *  @return SPtr<Material>: Shared pointer to the created material resource,
  *                          or a default material if the decoding failed.
  */
  SPtr<Material>
  getMaterialFromScene(const aiMaterial* pMat, const Path& meshPath) const;

  /***************************************************************************/
  /*
  *  Static Mesh
  */
  /***************************************************************************/

  /**
  *  @brief Processes the given aiNode and its children to extract static mesh
  *         data and create the resource.
  * 
  *  @param const aiNode* pNode: Pointer to the aiNode containing the static mesh.
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param SPtr<StaticMeshResource>& pCurrentMesh: Reference to the shared pointer to the
  *                                                 current static mesh resource being created.
  */
  void
  processStaticMeshNode(const aiNode* pNode,
                        const aiScene* pScene,
                        SPtr<StaticMeshResource>& pCurrentMesh,
                        Vector<String>& meshNames) const;

  /**
  *  @brief Processes the given aiMesh to extract static mesh data and fill the
  *         vertex, index and other relevant data of the current static mesh resource.
  * 
  *  @param const aiMesh* pMesh: Pointer to the aiMesh containing the static
  *                              mesh data to process.
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param SPtr<StaticMeshResource>& pCurrentMesh: Reference to the shared pointer to the
  *                                                 currently processed static mesh resource
  *                                                 being filled with data.
  */
  void
  processCurrentStaticMesh(const aiMesh* pMesh,
                           const aiScene* pScene,
                           SPtr<StaticMeshResource>& pCurrentMesh,
                           Vector<String>& meshNames) const;

  /***************************************************************************/
  /*
  *  Skeletal mesh and skeleton
  */
  /***************************************************************************/

  /**
  *  @brief Processes the given aiNode and its children to extract skeletal mesh
  *         and skeleton data and create the resources.
  * 
  *  @param const aiNode* pNode: Pointer to the aiNode containing the skeletal mesh and skeleton.
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param SPtr<BoneHierarchy> pBoneNode: Shared pointer to the current bone
  *                                        hierarchy node being processed.
  */
  void
  processSkeletalMeshNode(const aiNode* pNode,
                          const aiScene* pScene,
                          SPtr<BoneHierarchy> pBoneNode,
                          SPtr<SkeletalMeshResource>& pSkeletalMesh,
                          SPtr<SkeletonResource>& pSkeleton) const;

  /**
  *  @brief Processes the given aiMesh to extract skeletal mesh data and fill the
  *         vertex, index, bone weights and other relevant data of the current
  *         skeletal mesh resource, as well as the bone hierarchy and other relevant
  *         data of the current skeleton resource.
  * 
  *  @param const aiMesh* pMesh: Pointer to the aiMesh containing the skeletal
  *  @param const aiScene* pScene: Pointer to the aiScene containing the mesh data to decode.
  *  @param SPtr<SkeletalMeshResource>& pCurrentMesh: Reference to the shared pointer to the
  *                                                   currently processed skeletal mesh
  *                                                   resource being filled with data.
  *  @param SPtr<SkeletonResource>& pCurrentSkeleton: Reference to the shared pointer to the
  *                                                   currently processed skeleton resource.
  */
  void
  processCurrentSkeletalMesh(const aiMesh* pMesh,
                             const aiScene* pScene,
                             SPtr<SkeletalMeshResource>& pSkeletalMesh,
                             SPtr<SkeletonResource>& pSkeleton) const;

  /***************************************************************************/
  /*
  *  Animation
  */
  /***************************************************************************/

  void
  checkModelAnimations(const aiScene* pScene, SPtr<SkeletonResource>& pSkeleton) const;

  /**
  *  @brief Process the data of the given aiAnimation to extract the animation data.
  * 
  *  @param const aiAnimation* pAnim: Pointer to the aiAnimation containing the
  *                                   relevant animation data to process.
  *  @param SPtr<AnimationResource>& pOutAnimation: Reference to the shared pointer to the
  *                                                 current animation resource being filled with data.
  */
  void
  proccessAnimation(const aiAnimation* pAnim,
                    SPtr<AnimationResource>& pOutAnimation) const;
};
}
