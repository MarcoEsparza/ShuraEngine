/*************************************************************/
/*
*  @file    shResourceManager.cpp
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

namespace shEngineSDK {
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

Vector3
aiVec3ToVector3(const aiVector3D& aiVec)
{
  return Vector3(aiVec.x, aiVec.y, aiVec.z);
}

Quaternion
aiQuatToQuaternion(const aiQuaternion& aiQuat)
{
  return Quaternion(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
}

void
ResourceManager::loadResourceFromFile(const String& fileName)
{
  if (isResourceLoaded(fileName)) {
    return;
  }
  
  SPtr<Resource> resource;
  SystemPath filePath = fileName;
  
  if (filePath.extension() == ".png" ||
      filePath.extension() == ".jpeg" ||
      filePath.extension() == ".bmp" ||
      filePath.extension() == ".tga" ||
      filePath.extension() == ".hdr") {
    loadTextureFromFile(fileName);
  }
  else if (filePath.extension() == ".fbx" ||
           filePath.extension() == ".obj") {
    loadModelFromFile(fileName);
  }
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

void
ResourceManager::loadModelFromFile(const String& fileName)
{
  Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                aiProcess_CalcTangentSpace | 
                                                aiProcess_GenSmoothNormals | 
                                                aiProcess_JoinIdenticalVertices | 
                                                aiProcess_ImproveCacheLocality | 
                                                aiProcess_LimitBoneWeights | 
                                                aiProcess_SplitLargeMeshes | 
                                                aiProcess_Triangulate | 
                                                aiProcess_GenUVCoords | 
                                                aiProcess_SortByPType | 
                                                aiProcess_FindDegenerates | 
                                                aiProcess_FindInvalidData |
                                                aiProcess_FindInstances | 
                                                aiProcess_ValidateDataStructure | 
                                                aiProcess_OptimizeMeshes | 
                                                aiProcess_FlipUVs |
                                                0);

  auto* node = pScene->mRootNode->mChildren[0];

  if (node->mChildren == nullptr) {
    node = pScene->mRootNode->mChildren[1];
  }
  auto* mesh = pScene->mMeshes[node->mMeshes[0]];

  if (mesh->HasBones()) {
    createSkeletalMesh(pScene, fileName);
  }
  else {
    createStaticMesh(fileName, pScene->mRootNode, pScene);
  }
}

void
ResourceManager::loadTextureFromFile(const String& fileName)
{
  auto pImage = make_shared<ImageResource>();

  pImage->texture = GraphicsManager::instance().createTextureFromFile(fileName);

  SystemPath file = fileName;
  pImage->name = file.filename().string();

  m_loadedResources[pImage->name] = pImage;
}

void
ResourceManager::createStaticMesh(const String& fileName,
  const aiNode* node,
  const aiScene* scene)
{
  auto meshUnion = make_shared<StaticMeshUnionResource>();

  proccessStaticMeshNode(node, scene, meshUnion);

  if(meshUnion->meshes.size() > 1){
    SystemPath name = fileName;
    meshUnion->name = name.filename().string();
    
    m_loadedResources[meshUnion->name] = meshUnion;
  }
}

void
ResourceManager::proccessStaticMeshNode(const aiNode* node,
  const aiScene* scene,
  SPtr<StaticMeshUnionResource> meshUnion)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessStaticMesh(mesh, scene, meshUnion);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessStaticMeshNode(node->mChildren[i], scene, meshUnion);
  }
}

void
ResourceManager::proccessStaticMesh(const aiMesh* mesh,
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

  currentMesh->name = mesh->mName.C_Str();
  m_loadedResources[currentMesh->name] = currentMesh;

  meshUnion->meshes.push_back(currentMesh);
}

void
ResourceManager::createSkeletalMesh(const aiScene* scene, const String& fileName)
{
  auto skeletalMesh = make_shared<SkeletalMeshResource>();
  auto skeleton = make_shared<SkeletonResource>();

  proccessSkeletalMeshNode(scene->mRootNode, scene, skeletalMesh, skeleton);

  SystemPath file = fileName;
  file.replace_extension("");
  skeletalMesh->name = file.filename().string();
  skeleton->name = file.filename().string() + "Skeleton";

  //for (uint32 i = 0; i < skeletalMesh->vertices.size(); ++i)
  //{
  //  Vector4& boneWeigths = skeletalMesh->vertices[i].boneWeights;
  //  float totalWeight = boneWeigths.x +
  //                      boneWeigths.y +
  //                      boneWeigths.z +
  //                      boneWeigths.w;
  //  if (totalWeight > 0.0f)
  //  {
  //    skeletalMesh->vertices[i].boneWeights = Vector4(boneWeigths.x / totalWeight,
  //                                                    boneWeigths.y / totalWeight,
  //                                                    boneWeigths.z / totalWeight,
  //                                                    boneWeigths.w / totalWeight);
  //  }
  //}

  readSkeleton(skeleton->bones, scene->mRootNode, skeleton->boneInfo);

  Matrix4 skTransform = aiMatrixToMatrix4(scene->mRootNode->mTransformation);
  skeleton->inverseTransform = skTransform.getInversed();

  m_loadedResources[skeletalMesh->name] = skeletalMesh;
  m_loadedResources[skeleton->name] = skeleton;

  if (scene->HasAnimations()) {
    auto animation = make_shared<AnimationResource>();
    proccessAnimation(scene, animation, skeleton, 1);
    animation->name = file.filename().string() + "Animation";
    m_loadedResources[animation->name] = animation;
  }
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

    skeletalMesh->vertices.push_back(vertex);
  }

  skeletalMesh->numVertices.push_back(mesh->mNumVertices);

  uint32 indexCount = 0;
  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    indexCount += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      skeletalMesh->indices.push_back(face.mIndices[j]);
    }
  }
  skeletalMesh->numIndices.push_back(indexCount);

  auto mat = scene->mMaterials[mesh->mMaterialIndex];

  skeletalMesh->meshNames.push_back(mat->GetName().C_Str());
  ++skeletalMesh->numMeshes;

  for (uint32 i = 0; i < mesh->mNumBones; ++i) {
    aiBone* bone = mesh->mBones[i];
  }

  processSkeleton(mesh, scene, skeletalMesh, skeleton);
}

void
setVertexBoneData(VertexData& vertex, int32 boneID, float weight)
{
  if (vertex.boneIds.x < 0) {
    vertex.boneIds.x = boneID;
    vertex.boneWeights.x = weight;
  }
  if (vertex.boneIds.y < 0) {
    vertex.boneIds.y = boneID;
    vertex.boneWeights.y = weight;
  }
  if (vertex.boneIds.z < 0) {
    vertex.boneIds.z = boneID;
    vertex.boneWeights.z = weight;
  }
  if (vertex.boneIds.w < 0) {
    vertex.boneIds.w = boneID;
    vertex.boneWeights.w = weight;
  }
}

void
ResourceManager::processSkeleton(const aiMesh* mesh,
                                 const aiScene* scene,
                                 SPtr<SkeletalMeshResource>& skeletalMesh,
                                 SPtr<SkeletonResource>& skeleton)
{
  Vector<uint32> boneCounts;
  boneCounts.resize(mesh->mNumVertices, 0);
  uint32 baseVertex = 0;

  if (skeletalMesh->numMeshes > 1) {
    for (uint32 i = 0; i < skeletalMesh->numMeshes - 1; ++i) {
      baseVertex += skeletalMesh->numVertices[i];
    }
    --baseVertex;
  }

  for (uint32 i = 0; i < mesh->mNumBones; ++i) {
    aiBone* bone = mesh->mBones[i];

    int32 boneId = -1;

    Matrix4 boneMatrix = aiMatrixToMatrix4(bone->mOffsetMatrix);

    String boneName = bone->mName.C_Str();

    if (skeleton->boneInfo.find(boneName) == skeleton->boneInfo.end()) {
      skeleton->boneInfo[boneName] = { skeleton->boneCount, boneMatrix };
      boneId = skeleton->boneCount;
      ++skeleton->boneCount;
    }
    else {
      boneId = skeleton->boneInfo[boneName].first;
    }

    auto weights = mesh->mBones[i]->mWeights;
    int numWeights = mesh->mBones[i]->mNumWeights;

    for (uint32 weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
      int32 vertexID = baseVertex + weights[weightIndex].mVertexId;
      float weight = weights[weightIndex].mWeight;

      if (vertexID <= skeletalMesh->vertices.size()) {
        setVertexBoneData(skeletalMesh->vertices[vertexID], boneId, weight);
      }
    }
  }
}

bool
ResourceManager::readSkeleton(Bone& boneOutput,
                              aiNode* node,
                              UMap<String, std::pair<int32, Matrix4>>& boneInfoTable)
{
  if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end())
  {
    boneOutput.name = node->mName.C_Str();
    boneOutput.id = boneInfoTable[boneOutput.name].first;
    boneOutput.offset = boneInfoTable[boneOutput.name].second;
    boneOutput.transformation = aiMatrixToMatrix4(node->mTransformation);

    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
      Bone child;
      readSkeleton(child, node->mChildren[i], boneInfoTable);
      boneOutput.children.push_back(child);
    }

    return true;
  }
  else
  {
    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
      if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable))
      {
        return true;
      }
    }
  }

  return false;
}

BoneTransformTrack
getBTTrack(const String& boneName, int32 ID, const aiNodeAnim* channel) {
  BoneTransformTrack btt;

  btt.name = boneName;
  btt.ID = ID;
  btt.localTransform = Matrix4::IDENTITY;

  btt.numPositions = channel->mNumPositionKeys;

  for (uint32 posIndex = 0; posIndex < btt.numPositions; ++posIndex) {
    KeyPosition data;
    data.position = aiVec3ToVector3(channel->mPositionKeys[posIndex].mValue);
    data.timeStamp = channel->mPositionKeys[posIndex].mTime;
    btt.positions.push_back(data);
  }

  btt.numRotations = channel->mNumRotationKeys;

  for (uint32 rotIndex = 0; rotIndex < btt.numRotations; ++rotIndex) {
    KeyRotation data;
    data.orientation = aiQuatToQuaternion(channel->mRotationKeys[rotIndex].mValue);
    data.timeStamp = channel->mPositionKeys[rotIndex].mTime;
    btt.rotations.push_back(data);
  }

  btt.numScalings = channel->mNumScalingKeys;

  for (uint32 scaleIndex = 0; scaleIndex < btt.numScalings; ++scaleIndex) {
    KeyScale data;
    data.scale = aiVec3ToVector3(channel->mPositionKeys[scaleIndex].mValue);
    data.timeStamp = channel->mPositionKeys[scaleIndex].mTime;
    btt.scales.push_back(data);
  }

  return btt;
}

void
ResourceManager::proccessAnimation(const aiScene* scene,
                                   SPtr<AnimationResource>& animation,
                                   SPtr<SkeletonResource>& skeleton,
                                   uint32 index)
{
  animation->skeletonData = skeleton;
  aiAnimation* anim = scene->mAnimations[index];

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

  auto& bInfo = skeleton->boneInfo;
  
  for (uint32 i = 0; i < anim->mNumChannels; ++i) {
    auto channel = anim->mChannels[i];
    String boneName = channel->mNodeName.C_Str();

    if (bInfo.find(boneName) != bInfo.end()) {
      animation->boneTTracks.push_back(getBTTrack(boneName,
                                                 bInfo[boneName].first,
                                                 channel));
    }
  }

  // Load positions, rotations and scales for each bone
  /*for (uint32 i = 0; i < anim->mNumChannels; ++i)
  {
    aiNodeAnim* channel = anim->mChannels[i];
    BoneTransformTrack track;

    for (uint32 j = 0; j < channel->mNumPositionKeys; ++j)
    {
      track.posTimestamps.push_back(static_cast<float>(channel->mPositionKeys[j].mTime));
      track.positions.push_back(Vector3(channel->mPositionKeys[j].mValue.x,
        channel->mPositionKeys[j].mValue.y,
        channel->mPositionKeys[j].mValue.z));
    }
    for (uint32 j = 0; j < channel->mNumRotationKeys; ++j)
    {
      track.rotTimestamps.push_back(static_cast<float>(channel->mRotationKeys[j].mTime));
      track.rotations.push_back(Vector4(channel->mRotationKeys[j].mValue.x,
        channel->mRotationKeys[j].mValue.y,
        channel->mRotationKeys[j].mValue.z,
        channel->mRotationKeys[j].mValue.w));
    }
    for (uint32 j = 0; j < channel->mNumScalingKeys; ++j)
    {
      track.scaleTimestamps.push_back(static_cast<float>(channel->mScalingKeys[j].mTime));
      track.scales.push_back(Vector3(channel->mScalingKeys[j].mValue.x,
        channel->mScalingKeys[j].mValue.y,
        channel->mScalingKeys[j].mValue.z));
    }
    animation->boneTransform[channel->mNodeName.C_Str()] = track;
  }*/
}
}
