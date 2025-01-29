/*************************************************************/
/*
*  @file    shResourceManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief   Resource Manager module for loading all desired resources
*           from files.
*
*  Resource Manager module for loading all desired resources from files.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shResourceManager.h"

#include "shGraphicsManager.h"
#include "shResource.h"
#include "shMeshResource.h"
#include "shImageResource.h"
#include "shSkeletonResource.h"
#include "shAnimationResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// TODO: Change this to a wrapper.
#include <fstream>

using std::fstream;
using std::ios;
using std::getline;

using std::reinterpret_pointer_cast;

/**
*  @brief Custom flags for assimp importer
*/
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
                                    aiProcess_FlipUVs               | \
                                    0)

namespace shEngineSDK {
/*************************************************************/
/*
*  Static Variables
*/
/*************************************************************/

const Vector<String> ResourceManager::MODEL_EXTENSIONS = { ".fbx", ".obj" };
const Vector<String> ResourceManager::IMAGE_EXTENSIONS = { ".png",
                                                           ".jpeg",
                                                           ".bmp",
                                                           ".tga",
                                                           ".hdr" };

/*************************************************************/
/*
*  Non class functions
*/
/*************************************************************/

/**
*  @brief Tranforms an assimp aiMatrix4x4 to an engine Matrix4.
*
*  @param aiMatrix4x4& aiMatrix
*
*  @return Matrix4
*/
Matrix4
aiMatrixToMatrix4(const aiMatrix4x4& aiMatrix)
{
  return Matrix4(static_cast<float>(aiMatrix.a1),
                 static_cast<float>(aiMatrix.b1),
                 static_cast<float>(aiMatrix.c1),
                 static_cast<float>(aiMatrix.d1),

                 static_cast<float>(aiMatrix.a2),
                 static_cast<float>(aiMatrix.b2),
                 static_cast<float>(aiMatrix.c2),
                 static_cast<float>(aiMatrix.d2),

                 static_cast<float>(aiMatrix.a3),
                 static_cast<float>(aiMatrix.b3),
                 static_cast<float>(aiMatrix.c3),
                 static_cast<float>(aiMatrix.d3),

                 static_cast<float>(aiMatrix.a4),
                 static_cast<float>(aiMatrix.b4),
                 static_cast<float>(aiMatrix.c4),
                 static_cast<float>(aiMatrix.d4));
}

/**
*  @brief Tranforms an assimp aiVector3D to an engine Vector3.
*
*  @param aiVector3D& aiVec
*
*  @return Vector3
*/
Vector3
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
Quaternion
aiQuatToQuaternion(const aiQuaternion& aiQuat)
{
  return Quaternion(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
}

/**
*  @brief Sets the bone ids and weights to a vertex struct.
*
*  @param VertexData& vertex
*  @param int32 boneID
*  @param float weight
*/
void
setVertexBoneData(VertexData& vertex, int32 boneID, float weight)
{
  if (vertex.boneIds.x < 0) {
    vertex.boneIds.x = static_cast<float>(boneID);
    vertex.boneWeights.x = weight;
  }
  if (vertex.boneIds.y < 0) {
    vertex.boneIds.y = static_cast<float>(boneID);
    vertex.boneWeights.y = weight;
  }
  if (vertex.boneIds.z < 0) {
    vertex.boneIds.z = static_cast<float>(boneID);
    vertex.boneWeights.z = weight;
  }
  if (vertex.boneIds.w < 0) {
    vertex.boneIds.w = static_cast<float>(boneID);
    vertex.boneWeights.w = weight;
  }
}

/**
*  @brief Extract the bone id and weight yo set to a vertex.
*
*  @param Vector<VertexData>& vertices
*  @param const aiMesh* mesh
*  @param SPtr<SkeletonResource>& skeleton
*/
void
ExtractBoneWeightForVertex(Vector<VertexData>& vertices,
                           const aiMesh* mesh,
                           SPtr<SkeletonResource>& skeleton)
{
  for (uint32 boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
    int32 boneID = -1;
    String boneName = mesh->mBones[boneIndex]->mName.C_Str();
    if (skeleton->boneInfoMap.find(boneName) == skeleton->boneInfoMap.end()) {
      BoneInfo newBoneInfo;
      newBoneInfo.id = static_cast<int32>(skeleton->boneCount);
      newBoneInfo.offset = aiMatrixToMatrix4(mesh->mBones[boneIndex]->mOffsetMatrix);
      skeleton->boneInfoMap[boneName] = newBoneInfo;
      boneID = static_cast<int32>(skeleton->boneCount);
      ++skeleton->boneCount;
    }
    else {
      boneID = skeleton->boneInfoMap[boneName].id;
    }

    SH_ASSERT(boneID != -1);
    aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
    uint32 numWeights = mesh->mBones[boneIndex]->mNumWeights;

    for (uint32 weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
      uint32 vertexID = weights[weightIndex].mVertexId;
      float weight = static_cast<float>(weights[weightIndex].mWeight);
      SH_ASSERT(vertexID <= vertices.size());
      setVertexBoneData(vertices[vertexID], boneID, weight);
    }
  }
}

/**
*  @brief Gets the keys of the animation.
*
*  @param String& boneName
*  @param int32 ID
*  @param aiNodeAnim* channel
*
*  @return BoneTransformTrack
*/
BoneTransformTrack
getBTTrack(const String& boneName, int32 ID, const aiNodeAnim* channel)
{
  BoneTransformTrack btt;

  btt.name = boneName;
  btt.ID = ID;
  btt.localTransform = Matrix4::IDENTITY;

  btt.numPositions = channel->mNumPositionKeys;

  for (uint32 posIndex = 0; posIndex < btt.numPositions; ++posIndex) {
    KeyPosition data;
    data.position = aiVec3ToVector3(channel->mPositionKeys[posIndex].mValue);
    data.timeStamp = static_cast<float>(channel->mPositionKeys[posIndex].mTime);
    btt.positions.push_back(data);
  }

  btt.numRotations = channel->mNumRotationKeys;

  for (uint32 rotIndex = 0; rotIndex < btt.numRotations; ++rotIndex) {
    KeyRotation data;
    data.orientation = aiQuatToQuaternion(channel->mRotationKeys[rotIndex].mValue);
    data.timeStamp = static_cast<float>(channel->mPositionKeys[rotIndex].mTime);
    btt.rotations.push_back(data);
  }

  btt.numScalings = channel->mNumScalingKeys;

  for (uint32 scaleIndex = 0; scaleIndex < btt.numScalings; ++scaleIndex) {
    KeyScale data;
    data.scale = aiVec3ToVector3(channel->mPositionKeys[scaleIndex].mValue);
    data.timeStamp = static_cast<float>(channel->mPositionKeys[scaleIndex].mTime);
    btt.scales.push_back(data);
  }

  return btt;
}

void
ReadHierarchyData(AnimationNodeData& dest, const aiNode* src)
{
  SH_ASSERT(src);

  dest.name = src->mName.C_Str();
  dest.transformation = aiMatrixToMatrix4(src->mTransformation);
  dest.childrenCount = src->mNumChildren;

  for (uint32 i = 0; i < src->mNumChildren; ++i) {
    AnimationNodeData newData;
    ReadHierarchyData(newData, src->mChildren[i]);
    dest.children.push_back(newData);
  }
}

void
ReadMissingBoneTracks(const aiAnimation* aiAnim,
  SPtr<SkeletonResource>& skeleton,
  SPtr<AnimationResource>& animation)
{
  uint32 size = aiAnim->mNumChannels;

  UMap<String, BoneInfo>& boneInfoMap = skeleton->boneInfoMap;
  uint32& boneCount = skeleton->boneCount;

  for (uint32 i = 0; i < size; ++i) {
    auto channel = aiAnim->mChannels[i];
    String boneName = channel->mNodeName.data;

    if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
      boneInfoMap[boneName].id = boneCount;
      ++boneCount;
    }

    animation->boneTracks.push_back(getBTTrack(boneName,
      boneInfoMap[boneName].id,
      channel));
  }
}

/*************************************************************/
/*
*  Resource Manager functions
*/
/*************************************************************/

SPtr<Resource>
ResourceManager::loadResourceFromFile(const Path& filePath)
{
  SPtr<Resource> resource;

  if (filePath.compareExtensions(IMAGE_EXTENSIONS)) {
    resource = loadTextureFromFile(filePath.toString());
  }
  else if (filePath.compareExtensions(MODEL_EXTENSIONS)) {
    resource = loadModelFromFile(filePath.toString());
  }
  else {
    return nullptr;
  }

  return resource;
}

SPtr<Resource>
ResourceManager::getResource(const String& resourceName)
{
  return isResourceLoaded(resourceName);
}

SPtr<Resource>
ResourceManager::isResourceLoaded(const String& fileName)
{
  auto resObj = m_loadedResources.find(fileName);

  if (resObj != m_loadedResources.end()) {
    return (*resObj).second;
  }

  return nullptr;
}

SPtr<Resource>
ResourceManager::isResourceLoaded(const Path& fileName)
{
  auto resObj = m_loadedResources.find(fileName.toString());

  if (resObj != m_loadedResources.end()) {
    return (*resObj).second;
  }

  return nullptr;
}

SPtr<Resource>
ResourceManager::loadTextureFromFile(const String& fileName)
{
  auto pImage = make_shared<ImageResource>();

  pImage->texture = GraphicsManager::instance().createTextureFromFile(fileName);

  SystemPath file = fileName;
  pImage->setName(file.filename().string());

  m_loadedResources[pImage->getName()] = pImage;

  return pImage;
}

SPtr<Resource>
ResourceManager::loadModelFromFile(const String& fileName)
{
  Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                CUSTOM_AI_MAX_QUALITY_FLAG);

  aiNode* node = pScene->mRootNode->mChildren[0];

  if (node == nullptr) {
    node = pScene->mRootNode->mChildren[1];
  }
  aiMesh* mesh = pScene->mMeshes[node->mMeshes[0]];

  if (mesh->HasBones()) {
    return createSkeletalMesh(pScene, fileName);
  }
  else {
    if (pScene->mNumMeshes == 1) {
      return createStaticMesh(fileName, pScene->mRootNode, pScene);
    }
    else {
      return createStaticMeshUnion(fileName, pScene->mRootNode, pScene);
    }
  }
}

SPtr<Resource>
ResourceManager::createStaticMesh(const String&,
                                  const aiNode* node,
                                  const aiScene* scene)
{
  auto currentMesh = make_shared<StaticMeshResource>();

  proccessStaticMeshNode(node, scene, currentMesh);

  return currentMesh;
}

void
ResourceManager::proccessStaticMeshNode(const aiNode* node,
                                        const aiScene* scene,
                                        SPtr<StaticMeshResource>& currentMesh)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessStaticMesh(mesh, scene, currentMesh);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessStaticMeshNode(node->mChildren[i], scene, currentMesh);
  }
}

void
ResourceManager::proccessStaticMesh(const aiMesh* mesh,
                                    const aiScene* scene,
                                    SPtr<StaticMeshResource>& currentMesh)
{
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

    currentMesh->vertices.push_back(vertex);
  }

  currentMesh->numVertex = mesh->mNumVertices;

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    currentMesh->numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      currentMesh->indices.push_back(face.mIndices[j]);
    }
  }

  auto* mat = scene->mMaterials[mesh->mMaterialIndex];

  auto meshMaterial = make_shared<PBRMaterial>();

  auto& imgRes = m_loadedResources["White.png"];
  auto img = reinterpret_pointer_cast<ImageResource>(imgRes);
  meshMaterial->baseColor = img->texture;
  meshMaterial->name = mat->GetName().C_Str();
  currentMesh->material = meshMaterial;

  currentMesh->setName(mesh->mName.C_Str());
  m_loadedResources[currentMesh->getName()] = currentMesh;
}

SPtr<Resource>
ResourceManager::createStaticMeshUnion(const String& fileName,
                                       const aiNode* node,
                                       const aiScene* scene)
{
  auto meshUnion = make_shared<StaticMeshUnionResource>();

  proccessStaticMeshUnionNode(node, scene, meshUnion);

  if(meshUnion->meshes.size() > 1){
    SystemPath name = fileName;
    meshUnion->setName(name.filename().string());
    
    m_loadedResources[meshUnion->getName()] = meshUnion;
  }

  return meshUnion;
}

void
ResourceManager::proccessStaticMeshUnionNode(const aiNode* node,
                                             const aiScene* scene,
                                             SPtr<StaticMeshUnionResource> meshUnion)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessStaticUnionMesh(mesh, scene, meshUnion);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessStaticMeshUnionNode(node->mChildren[i], scene, meshUnion);
  }
}

void
ResourceManager::proccessStaticUnionMesh(const aiMesh* mesh,
                                         const aiScene* scene,
                                         SPtr<StaticMeshUnionResource> meshUnion)
{
  auto currentMesh = make_shared<StaticMeshResource>();

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

    currentMesh->vertices.push_back(vertex);
  }

  currentMesh->numVertex = mesh->mNumVertices;

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    currentMesh->numIndex += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      currentMesh->indices.push_back(face.mIndices[j]);
    }
  }

  auto* mat = scene->mMaterials[mesh->mMaterialIndex];
  
  auto meshMaterial = make_shared<PBRMaterial>();

  auto imgRes = m_loadedResources["White.png"];
  auto img = reinterpret_pointer_cast<ImageResource>(imgRes);
  meshMaterial->baseColor = img->texture;
  meshMaterial->name = mat->GetName().C_Str();
  currentMesh->material = meshMaterial;
  
  if (meshUnion->materials.size() == 0) {
    meshUnion->materials.push_back(currentMesh->material);
  }
  else {
    for (uint32 i = 0; i < meshUnion->materials.size(); ++i) {
      auto unionMat = meshUnion->materials[i];
      if (currentMesh->material->name == unionMat->name) {
        currentMesh->material = unionMat;
        break;
      }
      else {
        if (i == (meshUnion->materials.size() - 1)) {
          meshUnion->materials.push_back(currentMesh->material);
          break;
        }
      }
    }
  }

  currentMesh->setName(mesh->mName.C_Str());
  m_loadedResources[currentMesh->getName()] = currentMesh;

  meshUnion->meshes.push_back(currentMesh);
}

SPtr<Resource>
ResourceManager::createSkeletalMesh(const aiScene* scene, const String& fileName)
{
  auto skeletalMesh = make_shared<SkeletalMeshResource>();
  auto skeleton = make_shared<SkeletonResource>();

  skeletalMesh->materials.resize(scene->mNumMaterials);

  proccessSkeletalMeshNode(scene->mRootNode, scene, skeletalMesh, skeleton);

  SystemPath file = fileName;
  file.replace_extension("");
  skeletalMesh->setName(file.filename().string());
  skeleton->setName(file.filename().string() + "Skeleton");

  m_loadedResources[skeletalMesh->getName()] = skeletalMesh;
  m_loadedResources[skeleton->getName()] = skeleton;

  if (scene->HasAnimations()) {
    auto animation = make_shared<AnimationResource>();
    proccessAnimation(scene, animation, skeleton, 0);
    animation->setName(file.filename().string() + "Animation");
    m_loadedResources[animation->getName()] = animation;
  }

  return skeletalMesh;
}

void
ResourceManager::proccessSkeletalMeshNode(const aiNode* node,
                                          const aiScene* scene,
                                          SPtr<SkeletalMeshResource>& skeletalMesh,
                                          SPtr<SkeletonResource>& skeleton)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessSkeletalMesh(mesh, scene, skeletalMesh, skeleton);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessSkeletalMeshNode(node->mChildren[i], scene, skeletalMesh, skeleton);
  }
}

void
ResourceManager::proccessSkeletalMesh(const aiMesh* mesh,
                                      const aiScene* scene,
                                      SPtr<SkeletalMeshResource>& skeletalMesh,
                                      SPtr<SkeletonResource>& skeleton)
{
  SkeletalMeshInfo currentMeshInfo;
  Vector<VertexData> currentMeshVertices;

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

    currentMeshVertices.push_back(vertex);
  }

  currentMeshInfo.numVertices = static_cast<uint32>(currentMeshVertices.size());

  ExtractBoneWeightForVertex(currentMeshVertices, mesh, skeleton);

  for (auto& vertex : currentMeshVertices) {
    skeletalMesh->vertices.push_back(vertex);
  }

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    currentMeshInfo.numIndices += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      skeletalMesh->indices.push_back(face.mIndices[j]);
    }
  }

  currentMeshInfo.name = mesh->mName.C_Str();
  currentMeshInfo.materialIndex = mesh->mMaterialIndex;

  if (skeletalMesh->materials[currentMeshInfo.materialIndex] == nullptr) {
    auto meshMat = make_shared<PBRMaterial>();
    auto& imgRes = m_loadedResources["White.png"];
    auto img = reinterpret_pointer_cast<ImageResource>(imgRes);
    meshMat->baseColor = img->texture;
    auto mat = scene->mMaterials[mesh->mMaterialIndex];
    meshMat->name = mat->GetName().C_Str();
    skeletalMesh->materials[currentMeshInfo.materialIndex] = meshMat;
  }

  skeletalMesh->meshes.push_back(currentMeshInfo);
}

void
ResourceManager::proccessAnimation(const aiScene* scene,
                                   SPtr<AnimationResource>& animation,
                                   SPtr<SkeletonResource>& skeleton,
                                   uint32 index)
{
  animation->skeletonData = skeleton;
  auto anim = scene->mAnimations[index];

  if (anim->mTicksPerSecond != 0.0f)
  {
    animation->ticksPerSecond = static_cast<float>(anim->mTicksPerSecond);
  }
  else
  {
    animation->ticksPerSecond = 1;
  }

  animation->hasLoop = true;
  animation->duration = static_cast<float>(anim->mDuration);

  ReadHierarchyData(animation->rootNode, scene->mRootNode);
  ReadMissingBoneTracks(anim, skeleton, animation);
}

ResourceManager& g_resourceMan()
{
  return ResourceManager::instance();
}
}
