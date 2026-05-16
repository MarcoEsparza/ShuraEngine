/*****************************************************************************/
/*
*  @file    shMeshCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   Mesh Codec for decoding and encoding mesh files.
*
*  Mesh Codec for decoding and encoding mesh files.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shMeshCodec.h"
#include <shResourceManager.h>
#include <shLogger.h>
#include <shFileExplorer.h>
#include <shStringID.h>
#include <shException.h>

#include <shResource.h>
#include <shMeshResource.h>
#include <shImageResource.h>
#include <shSkeletonResource.h>
#include <shAnimationResource.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Custom flags for assimp importer
#define CUSTOM_AI_MAX_QUALITY_FLAG (aiProcess_CalcTangentSpace      | \
                                    aiProcess_GenSmoothNormals      | \
                                    aiProcess_JoinIdenticalVertices | \
                                    aiProcess_ImproveCacheLocality  | \
                                    aiProcess_LimitBoneWeights      | \
                                    aiProcess_SplitLargeMeshes      | \
                                    aiProcess_Triangulate           | \
                                    aiProcess_GenUVCoords           | \
                                    aiProcess_SortByPType           | \
                                    aiProcess_FindDegenerates       | \
                                    aiProcess_FindInvalidData       | \
                                    aiProcess_FindInstances         | \
                                    aiProcess_ValidateDataStructure | \
                                    aiProcess_OptimizeMeshes        | \
                                    aiProcess_ConvertToLeftHanded | \
                                    0)

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Helper functions
*/
/*****************************************************************************/

/**
*  @brief Tranforms an assimp aiMatrix4x4 to an engine Matrix4.
*
*  @param aiMatrix4x4& aiMatrix
*
*  @return Matrix4
*/
static Matrix4
aiMatrixToMatrix4(const aiMatrix4x4& aiMatrix)
{
  return Matrix4(cast::st<float>(aiMatrix.a1),
                 cast::st<float>(aiMatrix.b1),
                 cast::st<float>(aiMatrix.c1),
                 cast::st<float>(aiMatrix.d1),

                 cast::st<float>(aiMatrix.a2),
                 cast::st<float>(aiMatrix.b2),
                 cast::st<float>(aiMatrix.c2),
                 cast::st<float>(aiMatrix.d2),

                 cast::st<float>(aiMatrix.a3),
                 cast::st<float>(aiMatrix.b3),
                 cast::st<float>(aiMatrix.c3),
                 cast::st<float>(aiMatrix.d3),

                 cast::st<float>(aiMatrix.a4),
                 cast::st<float>(aiMatrix.b4),
                 cast::st<float>(aiMatrix.c4),
                 cast::st<float>(aiMatrix.d4));
}

/**
*  @brief Tranforms an assimp aiVector3D to an engine Vector3.
*
*  @param aiVector3D& aiVec
*
*  @return Vector3
*/
static Vector3
aiVec3ToVector3(const aiVector3D& aiVec)
{
  return Vector3(aiVec.x, aiVec.y, aiVec.z);
}

/**
*  @brief Tranforms an assimp aiQuaternion to an engine Quaternion.
*
*  @param aiQuaternion& aiQuat
*
*  @return Quaternion
*/
static Quaternion
aiQuatToQuaternion(const aiQuaternion& aiQuat)
{
  return Quaternion(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
}

static Vector<VertexData>
getVertexDataFromMesh(const aiMesh* mesh)
{
  Vector<VertexData> vertices;

  for (uint32 i = 0; i < mesh->mNumVertices; ++i) {
    VertexData vertex;

    vertex.position = Vector3(mesh->mVertices[i].x,
                              mesh->mVertices[i].y,
                              mesh->mVertices[i].z);

    if (mesh->HasNormals()) {
      vertex.normal = Vector3(mesh->mNormals[i].x,
                              mesh->mNormals[i].y,
                              mesh->mNormals[i].z);
    }
    else {
      vertex.normal = Vector3(0.0f, 0.0f, 0.0f);
    }

    if (mesh->mTextureCoords[0]) {
      vertex.tex = Vector2(mesh->mTextureCoords[0][i].x,
        mesh->mTextureCoords[0][i].y);
    }

    if (mesh->HasTangentsAndBitangents()) {
      vertex.tangents.x = mesh->mTangents[i].x;
      vertex.tangents.y = mesh->mTangents[i].y;
      vertex.tangents.z = mesh->mTangents[i].z;

      vertex.bitangents.x = mesh->mBitangents[i].x;
      vertex.bitangents.y = mesh->mBitangents[i].y;
      vertex.bitangents.z = mesh->mBitangents[i].z;
    }
    else {
      vertex.tangents = Vector3(0.0f, 0.0f, 0.0f);
      vertex.bitangents = Vector3(0.0f, 0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }
  return vertices;
}

static Vector<uint32>
getIndicesFromMesh(const aiMesh* mesh, uint32& numIndex)
{
  Vector<uint32> indices;

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return indices;
}

static SPtr<ImageResource>
getTextureFromMaterial(const aiTextureType type,
                       const aiMaterial* pMat,
                       const Path& resPath)
{
  ResourceManager& resMan = g_resourceMan();

  uint32 count = pMat->GetTextureCount(type);
  if (count > 0) {
    aiString str;
    pMat->GetTexture(type, 0, &str);
    Path texturePath(resPath.directory() + "/" + String(str.C_Str()));
    auto pImgRes = cast::re_ptr<ImageResource>(resMan.getResource(texturePath.filename()));
    if (pImgRes) {
      pImgRes = cast::re_ptr<ImageResource>(resMan.loadResourceFromFile(texturePath));
    }
    return pImgRes;
  }
  return nullptr;
}

static SPtr<Material>
getErrorMaterial()
{
  ResourceManager& resMan = g_resourceMan();
  auto pErrorMat = cast::re_ptr<Material>(resMan.getResource("ErrorMaterial"));

  if (pErrorMat) {
    return pErrorMat;
  }

  pErrorMat = sh_makeShared<Material>();

  auto errorTex = cast::re_ptr<ImageResource>(resMan.getResource("ErrorTexture"));
  auto defaultNormal = cast::re_ptr<ImageResource>(resMan.getResource("DefaultNormal"));
  auto blackTex = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  auto whiteTex = cast::re_ptr<ImageResource>(resMan.getResource("White.png"));

  pErrorMat->m_baseColor = errorTex;
  pErrorMat->m_normal = defaultNormal;
  pErrorMat->m_metalness = blackTex;
  pErrorMat->m_roughness = whiteTex;
  pErrorMat->m_ao = blackTex;
  pErrorMat->m_emissive = blackTex;
  pErrorMat->m_opacityMask = blackTex;

  pErrorMat->m_properties.properties.flags.bHasDiffuseMap = true;
  pErrorMat->m_properties.properties.flags.bHasNormalMap = true;
  pErrorMat->m_properties.properties.flags.bHasMetalnessMap = false;
  pErrorMat->m_properties.properties.flags.bHasRoughnessMap = false;
  pErrorMat->m_properties.properties.flags.bHasAmbientOcclusionMap = false;
  pErrorMat->m_properties.properties.flags.bHasEmissiveMap = false;
  pErrorMat->m_properties.properties.flags.bHasOpacityMask = false;

  pErrorMat->setName("ErrorMaterial");
  resMan.addResource(pErrorMat);

  return pErrorMat;
}

static void
setVertexBoneData(VertexData& vertex, int32 boneID, float weight)
{
  if (vertex.boneWeights.x == 0.0f) {
    vertex.boneIds.x = cast::st<float>(boneID);
    vertex.boneWeights.x = weight;
  }
  if (vertex.boneWeights.y == 0.0f) {
    vertex.boneIds.y = cast::st<float>(boneID);
    vertex.boneWeights.y = weight;
  }
  if (vertex.boneWeights.z == 0.0f) {
    vertex.boneIds.z = cast::st<float>(boneID);
    vertex.boneWeights.z = weight;
  }
  if (vertex.boneWeights.w == 0.0f) {
    vertex.boneIds.w = cast::st<float>(boneID);
    vertex.boneWeights.w = weight;
  }
}

/*****************************************************************************/
/*
*  Class functions
*/
/*****************************************************************************/

UID
MeshCodec::getUID() const
{
  return StringID("MeshCodec").getUID();
}

Vector<String>
MeshCodec::getSupportedExtensions() const
{
  return { ".fbx", ".obj", ".gltf", ".glb" };
}

bool
MeshCodec::decode(const Path& filePath) const
{
  ResourceManager& resMan = g_resourceMan();

  Assimp::Importer fileImporter;
  const aiScene* pScene = fileImporter.ReadFile(filePath.string(),
                                                CUSTOM_AI_MAX_QUALITY_FLAG);

  aiNode* pRootNode = pScene->mRootNode;
  if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pRootNode) {
    SH_LOG_ERROR("Assimp error: " + String(fileImporter.GetErrorString()));
    return false;
  }
  if (pScene->mNumMeshes == 0) {
    SH_LOG_ERROR("Assimp error: No meshes found in the scene.");
    return false;
  }

  SPtr<Resource> pMeshRes;
  aiMesh* pMesh = pScene->mMeshes[0];
  if (pMesh->HasBones()) {
    pMeshRes = decodeSkeletalMesh(pScene, filePath);
  }
  else {
    pMeshRes = decodeStaticMesh(pScene, filePath);
  }

  if(pMeshRes) {
    return resMan.addResource(pMeshRes);
  }

  return false;
}

bool
MeshCodec::encode(const Path& filePath) const
{
  SH_UNREFERENCED_PARAMETER(filePath);
  return false;
}

SPtr<Resource>
MeshCodec::decodeStaticMesh(const aiScene* pScene, const Path& filePath) const
{
  auto pCurrentMesh = sh_makeShared<StaticMeshResource>();
  pCurrentMesh->setName(filePath.filename());
  pCurrentMesh->setPath(filePath);

  aiNode* pRootNode = pScene->mRootNode;
  processStaticMeshNode(pRootNode, pScene, pCurrentMesh);

  return pCurrentMesh;
}

SPtr<Resource>
MeshCodec::decodeSkeletalMesh(const aiScene* pScene, const Path& filePath) const
{
  ResourceManager& resMan = g_resourceMan();
  auto skeletalMesh = sh_makeShared<SkeletalMeshResource>();
  auto skeleton = sh_makeShared<SkeletonResource>();

  auto rootBone = sh_makeShared<BoneHierarchy>();
  skeletalMesh->setPath(filePath);
  skeletalMesh->setName(filePath.filename());
  Path skeletonPath = filePath;
  skeletonPath.replaceExtension("");
  skeleton->setName(skeletonPath.filename() + "_Skeleton");
  processSkeletalMeshNode(pScene->mRootNode, pScene, rootBone, skeletalMesh, skeleton);

  if (skeletalMesh->m_vertices.empty()) {
    SH_LOG_ERROR("Assimp error: No vertices found in the skeletal mesh.");
    return nullptr;
  }
  if (skeleton->m_boneCount == 0) {
    SH_LOG_ERROR("Assimp error: No bones found in the skeletal mesh.");
    return nullptr;
  }

  resMan.addResource(skeleton);
  checkModelAnimations(pScene, skeleton);
  skeletalMesh->m_skeleton = skeleton;

  return skeletalMesh;
}

SPtr<Material>
MeshCodec::getMaterialFromScene(const aiMaterial* pMat, const Path& meshPath) const
{
  ResourceManager& resMan = g_resourceMan();

  String matName = pMat->GetName().C_Str();
  auto pMeshMat = cast::re_ptr<Material>(resMan.getResource(matName));

  if (pMeshMat) {
    return pMeshMat;
  }

  String directory = meshPath.directory() + "/";
  pMeshMat = sh_makeShared<Material>();
  pMeshMat->setName(matName);

  // Diffuse
  pMeshMat->m_baseColor = getTextureFromMaterial(aiTextureType_DIFFUSE, pMat, meshPath);
  if (pMeshMat->m_baseColor.expired()) {
    auto pErrMat = getErrorMaterial();
    pMeshMat->m_properties.properties.flags = pErrMat->m_properties.properties.flags;
    pMeshMat->m_baseColor = pErrMat->m_baseColor;
    pMeshMat->m_normal = pErrMat->m_normal;
    pMeshMat->m_metalness = pErrMat->m_metalness;
    pMeshMat->m_roughness = pErrMat->m_roughness;
    pMeshMat->m_ao = pErrMat->m_ao;
    pMeshMat->m_emissive = pErrMat->m_emissive;
    pMeshMat->m_opacityMask = pErrMat->m_opacityMask;
    resMan.addResource(pMeshMat);
    return pMeshMat;
  }

  // Normal
  pMeshMat->m_normal = getTextureFromMaterial(aiTextureType_NORMALS, pMat, meshPath);
  if (pMeshMat->m_normal.expired()) {
    pMeshMat->m_properties.properties.flags.bHasNormalMap = true;
    pMeshMat->m_normal = cast::re_ptr<ImageResource>(resMan.getResource("DefaultNormal"));
  }

  // Metalness
  pMeshMat->m_metalness = getTextureFromMaterial(aiTextureType_METALNESS, pMat, meshPath);
  if (pMeshMat->m_metalness.expired()) {
    pMeshMat->m_properties.properties.flags.bHasMetalnessMap = false;
    pMeshMat->m_metalness = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  // Roughness
  pMeshMat->m_roughness = getTextureFromMaterial(aiTextureType_SHININESS, pMat, meshPath);
  if (pMeshMat->m_roughness.expired()) {
    pMeshMat->m_properties.properties.flags.bHasRoughnessMap = false;
    pMeshMat->m_roughness = cast::re_ptr<ImageResource>(resMan.getResource("White.png"));
  }

  // Ambient Occlusion
  pMeshMat->m_ao = getTextureFromMaterial(aiTextureType_LIGHTMAP, pMat, meshPath);
  if(pMeshMat->m_ao.expired()) {
    pMeshMat->m_properties.properties.flags.bHasAmbientOcclusionMap = false;
    pMeshMat->m_ao = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  // Emissive
  pMeshMat->m_emissive = getTextureFromMaterial(aiTextureType_EMISSIVE, pMat, meshPath);
  if (pMeshMat->m_emissive.expired()) {
    pMeshMat->m_properties.properties.flags.bHasEmissiveMap = false;
    pMeshMat->m_emissive = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  // Opacity Mask
  pMeshMat->m_opacityMask = getTextureFromMaterial(aiTextureType_OPACITY, pMat, meshPath);
  if (pMeshMat->m_opacityMask.expired()) {
    pMeshMat->m_properties.properties.flags.bHasOpacityMask = false;
    pMeshMat->m_opacityMask = cast::re_ptr<ImageResource>(resMan.getResource("White.png"));
  }

  resMan.addResource(pMeshMat);

  return pMeshMat;
}

void
MeshCodec::processStaticMeshNode(const aiNode* pNode,
                                 const aiScene* pScene,
                                 SPtr<StaticMeshResource>& pCurrentMesh) const
{
  for (uint32 i = 0; i < pNode->mNumMeshes; ++i) {
    aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
    processCurrentStaticMesh(mesh, pScene, pCurrentMesh);
  }

  for (uint32 i = 0; i < pNode->mNumChildren; ++i) {
    processStaticMeshNode(pNode->mChildren[i], pScene, pCurrentMesh);
  }
}

void
MeshCodec::processCurrentStaticMesh(const aiMesh* pMesh,
                                     const aiScene* pScene,
                                     SPtr<StaticMeshResource>& pCurrentMesh) const
{
  MeshData currentData;
  currentData.name = pMesh->mName.C_Str();
  currentData.vertices = getVertexDataFromMesh(pMesh);
  currentData.numVertices = pMesh->mNumVertices;
  currentData.indices = getIndicesFromMesh(pMesh, currentData.numIndices);

  auto* aiMat = pScene->mMaterials[pMesh->mMaterialIndex];
  currentData.materialIndex = pMesh->mMaterialIndex;
  auto pCurrentMat = getMaterialFromScene(aiMat, pCurrentMesh->getPath());

  if (!pCurrentMat) {
    pCurrentMat = getErrorMaterial();
  }

  if (pCurrentMesh->m_materials.empty()) {
    pCurrentMesh->m_materials.push_back(pCurrentMat);
  }
  else {
    for (uint32 i = 0; i < pCurrentMesh->m_materials.size(); ++i) {
      auto mat = pCurrentMesh->m_materials[i].lock();
      if (pCurrentMat->getName() == mat->getName()) {
        currentData.materialIndex = i;
        break;
      }
      else {
        if (i == pCurrentMesh->m_materials.size() - 1) {
          pCurrentMesh->m_materials.push_back(pCurrentMat);
          currentData.materialIndex = i + 1;
        }
      }
    }
  }

  pCurrentMesh->m_meshes.push_back(currentData);
}

void
MeshCodec::processSkeletalMeshNode(const aiNode* pNode,
                                   const aiScene* pScene,
                                   SPtr<BoneHierarchy> pBoneNode,
                                   SPtr<SkeletalMeshResource>& pSkeletalMesh,
                                   SPtr<SkeletonResource>& pSkeleton)  const
{
  for (uint32 i = 0; i < pNode->mNumMeshes; ++i) {
    aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
    processCurrentSkeletalMesh(mesh, pScene, pSkeletalMesh, pSkeleton);
  }
  pBoneNode->name = pNode->mName.C_Str();
  pBoneNode->transformation = aiMatrixToMatrix4(pNode->mTransformation);

  for (uint32 i = 0; i < pNode->mNumChildren; ++i) {
    auto childBone = sh_makeShared<BoneHierarchy>();
    pBoneNode->children.push_back(childBone);
    processSkeletalMeshNode(pNode->mChildren[i], pScene, childBone, pSkeletalMesh, pSkeleton);
  }
}

void
MeshCodec::processCurrentSkeletalMesh(const aiMesh* pMesh,
                                      const aiScene* pScene,
                                      SPtr<SkeletalMeshResource>& pSkeletalMesh,
                                      SPtr<SkeletonResource>& pSkeleton) const
{
  // Skeletal mesh info
  SkeletalMeshData currentMeshInfo;

  Vector<VertexData> currentMeshVertices = getVertexDataFromMesh(pMesh);
  currentMeshInfo.numVertices = pMesh->mNumVertices;
  for (auto& vertex : currentMeshVertices) {
    pSkeletalMesh->m_vertices.push_back(vertex);
  }
  pSkeletalMesh->m_indices = getIndicesFromMesh(pMesh, currentMeshInfo.numIndices);

  currentMeshInfo.name = pMesh->mName.C_Str();

  auto pCurrentMat = getMaterialFromScene(pScene->mMaterials[pMesh->mMaterialIndex],
                                          pSkeletalMesh->getPath());

  if (!pCurrentMat) {
    pCurrentMat = getErrorMaterial();
  }

  if (pSkeletalMesh->m_materials.empty()) {
    pSkeletalMesh->m_materials.push_back(pCurrentMat);
  }
  else {
    for (uint32 i = 0; i < pSkeletalMesh->m_materials.size(); ++i) {
      auto& mat = pSkeletalMesh->m_materials[i];
      if (pCurrentMat->getName() == mat->getName()) {
        currentMeshInfo.materialIndex = i;
        break;
      }
      else {
        if (i == pSkeletalMesh->m_materials.size() - 1) {
          pSkeletalMesh->m_materials.push_back(pCurrentMat);
          currentMeshInfo.materialIndex = i + 1;
        }
      }
    }
  }

  pSkeletalMesh->m_meshes.push_back(currentMeshInfo);

  // Bones info
  for (uint32 b = 0; b < pMesh->mNumBones; ++b) {
    aiBone* bone = pMesh->mBones[b];
    String boneName = bone->mName.C_Str();

    int32 boneIndex = 0;
    if (pSkeleton->m_boneMapping.find(boneName) == pSkeleton->m_boneMapping.end()) {
      boneIndex = pSkeleton->m_boneCount++;
      pSkeleton->m_boneMapping[boneName] = boneIndex;

      BoneInfo bi;
      bi.name = boneName;
      bi.offset = aiMatrixToMatrix4(bone->mOffsetMatrix);
      pSkeleton->m_bones.push_back(bi);
    }
    else {
      boneIndex = pSkeleton->m_boneMapping[boneName];
    }

    // Assign weights to vertices
    for (uint32 w = 0; w < bone->mNumWeights; ++w) {
      uint32 vertexID = bone->mWeights[w].mVertexId;
      float weight = bone->mWeights[w].mWeight;
      setVertexBoneData(pSkeletalMesh->m_vertices[vertexID], boneIndex, weight);
    }
  }
}

void
MeshCodec::checkModelAnimations(const aiScene* pScene,
                                SPtr<SkeletonResource>& pSkeleton) const
{
  ResourceManager& resMan = g_resourceMan();

  if (pScene->HasAnimations()) {
    for (uint32 i = 0; i < pScene->mNumAnimations; ++i) {
      auto animation = sh_makeShared<AnimationResource>();
      aiAnimation* anim = pScene->mAnimations[i];
      proccessAnimation(anim, animation);
      String name = pScene->mAnimations[i]->mName.C_Str();
      resMan.addResource(animation);
      pSkeleton->m_animations.push_back(animation);
    }
  }
}

void
MeshCodec::proccessAnimation(const aiAnimation* pAnim,
                             SPtr<AnimationResource>& pOutAnimation)  const
{
  pOutAnimation->m_duration = cast::st<float>(pAnim->mDuration);
  pOutAnimation->m_ticksPerSecond = cast::st<float>(pAnim->mTicksPerSecond) != 0.0f ?
                                    cast::st<float>(pAnim->mTicksPerSecond) : 25.0f;

  for (uint32 c = 0; c < pAnim->mNumChannels; ++c) {
    aiNodeAnim* channel = pAnim->mChannels[c];
    BoneAnimationChannel outChannel;
    outChannel.name = channel->mNodeName.C_Str();
    for (uint32 p = 0; p < channel->mNumPositionKeys; ++p) {
      KeyPosition posKey;
      posKey.position = aiVec3ToVector3(channel->mPositionKeys[p].mValue);
      posKey.timeStamp = cast::st<float>(channel->mPositionKeys[p].mTime);
      outChannel.positions.push_back(posKey);
    }

    for (uint32 r = 0; r < channel->mNumRotationKeys; ++r) {
      KeyRotation rotKey;
      rotKey.orientation = aiQuatToQuaternion(channel->mRotationKeys[r].mValue);
      rotKey.timeStamp = cast::st<float>(channel->mRotationKeys[r].mTime);
      outChannel.rotations.push_back(rotKey);
    }

    for (uint32 s = 0; s < channel->mNumScalingKeys; ++s) {
      KeyScale scaleKey;
      scaleKey.scale = aiVec3ToVector3(channel->mScalingKeys[s].mValue);
      scaleKey.timeStamp = cast::st<float>(channel->mScalingKeys[s].mTime);
      outChannel.scales.push_back(scaleKey);
    }

    pOutAnimation->m_channels[outChannel.name] = outChannel;
  }
}
}