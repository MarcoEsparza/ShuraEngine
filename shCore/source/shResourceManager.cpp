/*************************************************************/
/*
*  @file    shResourceManager.cpp
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
#include "shAsset.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// TODO: Change this to a wrapper.
#include <fstream>

using std::fstream;
using std::ios;
using std::getline;

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
                                                           ".jpg",
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
static Matrix4
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

/**
*  @brief Sets the bone ids and weights to a vertex struct.
*
*  @param VertexData& vertex
*  @param int32 boneID
*  @param float weight
*/
static void
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
static void
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
static BoneTransformTrack
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

static void
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

static void
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

ResourceManager::~ResourceManager()
{
  for (auto& resource : m_loadedResources) {
    resource.second.reset();
  }

  m_loadedResources.clear();
}

SPtr<Resource>
ResourceManager::loadResourceFromFile(const Path& filePath)
{
  SPtr<Resource> resource;

  // Check if resource is already on memory
  if (isResourceOnMemory(filePath, resource)) {
    return resource;
  }

  // Check if there is a cache for resource
  if (isCacheForResource(filePath, resource)) {
    return resource;
  }

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

bool
ResourceManager::saveResourceToAsset(const SPtr<Resource> pRes)
{
  Asset resAsset;
  return resAsset.saveResourceToAsset(pRes);
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

bool
ResourceManager::isResourceOnMemory(const Path& filePath, SPtr<Resource>& pRes)
{
  auto resObj = m_loadedResources.find(filePath.toString());

  if (resObj != m_loadedResources.end()) {
    pRes = (*resObj).second;
    return true;
  }

  pRes = nullptr;
  return false;
}

bool
ResourceManager::isCacheForResource(const Path& filePath, SPtr<Resource>& pRes)
{
  if (filePath.compareExtensions(IMAGE_EXTENSIONS)) {
    SystemPath path = filePath.toString();
    path.replace_extension(".dds");
    SystemPath fullPath = "resources/assets/textures/" + path.filename().string();

    if (std::filesystem::exists(fullPath)) {
      pRes = loadTextureFromDDS(fullPath.string());
      return true;
    }
  }
  else if (filePath.compareExtensions({ ".dds" })) {
    pRes = loadTextureFromDDS(filePath.toString());
    return true;
  }
  else if (filePath.compareExtensions({ ".sha" })) {
    SystemPath path = filePath.toString();
    path.replace_extension(".sha");
    SystemPath fullPath = "resources/assets/textures/" + path.filename().string();

    if (std::filesystem::exists(fullPath)) {
      
      return true;
    }
  }

  return false;
}

SPtr<Resource>
ResourceManager::loadTextureFromFile(const String& fileName)
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pImage = sh_makeShared<ImageResource>();

  pImage->texture = graphMan.createTextureFromFile(fileName);

  SystemPath file = fileName;
  pImage->setName(file.filename().string());

  m_loadedResources[pImage->getName()] = pImage;

  SystemPath path = file.filename();
  path.replace_extension(".dds");
  String saveTex = "resources/assets/textures/" + path.string();

  graphMan.saveTextureToDDS(pImage->texture, saveTex);

  Path texPath(saveTex);
  pImage->setPath(texPath);

  return pImage;
}

SPtr<Resource>
ResourceManager::loadTextureFromDDS(const String& filename)
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pImage = sh_makeShared<ImageResource>();

  pImage->texture = graphMan.createTextureFromDDS(filename);

  SystemPath file = filename;
  pImage->setName(file.filename().string());
  m_loadedResources[pImage->getName()] = pImage;
  Path texPath(filename);
  pImage->setPath(texPath);

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
    return createStaticMesh(fileName, pScene->mRootNode, pScene);
  }
}

SPtr<Material>
ResourceManager::createMaterialFromFile(const aiMaterial* pMat)
{
  GraphicsManager& graphMan = g_graphicsMan();
  auto pMeshMat = sh_makeShared<Material>();

  uint32 diffCount = pMat->GetTextureCount(aiTextureType_DIFFUSE);
  uint32 normCount = pMat->GetTextureCount(aiTextureType_NORMALS);
  uint32 metalCount = pMat->GetTextureCount(aiTextureType_METALNESS);
  uint32 roughCount = pMat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
  uint32 aoCount = pMat->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);

  if (diffCount == 0) {
    // Create error texture
    pMeshMat->m_properties.bHasDiffuseMap = true;
    pMeshMat->baseColor = graphMan.createErrorTexturre();
  }
  else {
    pMeshMat->m_properties.bHasDiffuseMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
    SystemPath filename = aiPath.C_Str();
    String directory = "resources/textures/";
    Path filePath(directory + filename.filename().string());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    pMeshMat->baseColor = pImage->texture;
    pMeshMat->baseColorPath = pImage->getPath().toString();
  }

  if (normCount == 0) {
    pMeshMat->m_properties.bHasNormalMap = false;
  }
  else {
    pMeshMat->m_properties.bHasNormalMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_NORMALS, 0, &aiPath);
    SystemPath filename = aiPath.C_Str();
    String directory = "resources/textures/";
    Path filePath(directory + filename.filename().string());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    pMeshMat->normal = pImage->texture;
    pMeshMat->normalPath = pImage->getPath().toString();
  }

  if (metalCount == 0) {
    pMeshMat->m_properties.bHasMetalnessMap = false;
  }
  else {
    pMeshMat->m_properties.bHasMetalnessMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_METALNESS, 0, &aiPath);
    SystemPath filename = aiPath.C_Str();
    String directory = "resources/textures/";
    Path filePath(directory + filename.filename().string());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    pMeshMat->metallic = pImage->texture;
    pMeshMat->metallicPath = pImage->getPath().toString();
  }

  if (roughCount == 0) {
    pMeshMat->m_properties.bHasRoughnessMap = false;
  }
  else {
    pMeshMat->m_properties.bHasRoughnessMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &aiPath);
    SystemPath filename = aiPath.C_Str();
    String directory = "resources/textures/";
    Path filePath(directory + filename.filename().string());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    pMeshMat->roughness = pImage->texture;
    pMeshMat->roughnessPath = pImage->getPath().toString();
  }

  if (aoCount == 0) {
    pMeshMat->m_properties.bHasAmbientOcclusionMap = false;
  }
  else {
    pMeshMat->m_properties.bHasAmbientOcclusionMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &aiPath);
    SystemPath filename = aiPath.C_Str();
    String directory = "resources/textures/";
    Path filePath(directory + filename.filename().string());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    pMeshMat->ao = pImage->texture;
    pMeshMat->aoPath = pImage->getPath().toString();
  }
  
  pMeshMat->m_type = MATERIAL_TYPE::kPBR;
  pMeshMat->name = pMat->GetName().C_Str();

  return pMeshMat;
}

SPtr<Resource>
ResourceManager::loadModelFromCache(const String& fileName)
{
  Asset model;
  model.loadResourceFromAsset(Path(fileName));
  auto& pRes = model.m_res;

  SystemPath path = fileName;
  pRes->setName(path.filename().string());
  m_loadedResources[pRes->getName()] = pRes;

  return pRes;
}

SPtr<Resource>
ResourceManager::createStaticMesh(const String& fileName,
                                  const aiNode* node,
                                  const aiScene* scene)
{
  auto currentMesh = sh_makeShared<StaticMeshResource>();

  proccessStaticMeshNode(node, scene, currentMesh);
  SystemPath path = fileName;
  currentMesh->setName(path.filename().string());
  m_loadedResources[currentMesh->getName()] = currentMesh;

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
  MeshData currentData;
  currentData.name = mesh->mName.C_Str();
  currentData.vertices = getVertexDataFromMesh(mesh);
  currentData.numVertices = mesh->mNumVertices;
  currentData.indices = getIndicesFromMesh(mesh, currentData.numIndices);

  auto* aiMat = scene->mMaterials[mesh->mMaterialIndex];
  currentData.materialIndex = mesh->mMaterialIndex;
  auto currentMat = createMaterialFromFile(aiMat);

  if (currentMesh->m_materials.empty()) {
    currentMesh->m_materials.push_back(currentMat);
  }
  else {
    for (uint32 i = 0; i < currentMesh->m_materials.size(); ++i) {
      auto& mat = currentMesh->m_materials[i];
      if (currentMat->name == mat->name) {
        currentData.materialIndex = i;
        break;
      }
      else {
        if (i == currentMesh->m_materials.size() - 1) {
          currentMesh->m_materials.push_back(currentMat);
          currentData.materialIndex = i + 1;
        }
      }
    }
  }

  currentMesh->m_meshes.push_back(currentData);
}

SPtr<Resource>
ResourceManager::createSkeletalMesh(const aiScene* scene, const String& fileName)
{
  auto skeletalMesh = sh_makeShared<SkeletalMeshResource>();
  auto skeleton = sh_makeShared<SkeletonResource>();

  skeletalMesh->m_materials.resize(scene->mNumMaterials);

  proccessSkeletalMeshNode(scene->mRootNode, scene, skeletalMesh, skeleton);

  SystemPath file = fileName;
  file.replace_extension("");
  skeletalMesh->setName(file.filename().string());
  skeleton->setName(file.filename().string() + "Skeleton");

  m_loadedResources[skeletalMesh->getName()] = skeletalMesh;
  m_loadedResources[skeleton->getName()] = skeleton;

  if (scene->HasAnimations()) {
    auto animation = sh_makeShared<AnimationResource>();
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
  SkeletalMeshData currentMeshInfo;

  Vector<VertexData> currentMeshVertices = getVertexDataFromMesh(mesh);

  currentMeshInfo.numVertices = mesh->mNumVertices;
  ExtractBoneWeightForVertex(currentMeshVertices, mesh, skeleton);
  for (auto& vertex : currentMeshVertices) {
    skeletalMesh->m_vertices.push_back(vertex);
  }
  skeletalMesh->m_indices = getIndicesFromMesh(mesh, currentMeshInfo.numIndices);

  currentMeshInfo.name = mesh->mName.C_Str();
  currentMeshInfo.materialIndex = mesh->mMaterialIndex;

  if (skeletalMesh->m_materials[currentMeshInfo.materialIndex] == nullptr) {
    auto meshMat = sh_makeShared<Material>();
    auto& imgRes = m_loadedResources["White.png"];
    auto img = sh_reinterpretPCast<ImageResource>(imgRes);
    meshMat->baseColor = img->texture;
    auto mat = scene->mMaterials[mesh->mMaterialIndex];
    meshMat->name = mat->GetName().C_Str();
    skeletalMesh->m_materials[currentMeshInfo.materialIndex] = meshMat;
  }

  skeletalMesh->m_meshes.push_back(currentMeshInfo);
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
