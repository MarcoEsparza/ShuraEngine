/*****************************************************************************/
/*
*  @file    shMeshCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
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

  UID
  getUID() const override;

  Vector<String>
  getSupportedExtensions() const override;

  bool
  decode(const Path& filePath) const override;

  bool
  encode(const Path& filePath) const override;

 private:
  _NODISCARD SPtr<Resource>
  decodeStaticMesh(const aiScene* pScene, const Path& filePath) const;

  _NODISCARD SPtr<Resource>
  decodeSkeletalMesh(const aiScene* pScene, const Path& filePath) const;

  SPtr<Material>
  getMaterialFromScene(const aiMaterial* pMat, const Path& meshPath) const;

  /***************************************************************************/
  /*
  *  Static Mesh
  */
  /***************************************************************************/

  void
  processStaticMeshNode(const aiNode* pNode,
                        const aiScene* pScene,
                        SPtr<StaticMeshResource>& pCurrentMesh) const;

  void
  processCurrentStaticMesh(const aiMesh* pMesh,
                            const aiScene* pScene,
                            SPtr<StaticMeshResource>& pCurrentMesh) const;

  /***************************************************************************/
  /*
  *  Skeletal mesh and skeleton
  */
  /***************************************************************************/

  void
  processSkeletalMeshNode(const aiNode* pNode,
                          const aiScene* pScene,
                          SPtr<BoneHierarchy> pBoneNode,
                          SPtr<SkeletalMeshResource>& pSkeletalMesh,
                          SPtr<SkeletonResource>& pSkeleton) const;

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
  *  @brief Process the animation.
  * 
  *  @param aiScene* scene
  *  @param SPtr<AnimationResource>& animation
  *  @param uint32 index
  */
  void
  proccessAnimation(const aiAnimation* pAnim,
                    SPtr<AnimationResource>& pOutAnimation) const;
};
}
