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

  auto trans = scene->mRootNode->mTransformation;
  Matrix4 skTransform(static_cast<float>(trans.a1),
                      static_cast<float>(trans.b1),
                      static_cast<float>(trans.c1),
                      static_cast<float>(trans.d1),
                      
                      static_cast<float>(trans.a2),
                      static_cast<float>(trans.b2),
                      static_cast<float>(trans.c2),
                      static_cast<float>(trans.d2),
                      
                      static_cast<float>(trans.a3),
                      static_cast<float>(trans.b3),
                      static_cast<float>(trans.c3),
                      static_cast<float>(trans.d3),
                      
                      static_cast<float>(trans.a4),
                      static_cast<float>(trans.b4),
                      static_cast<float>(trans.c4),
                      static_cast<float>(trans.d4));
  skeleton->inverseTransform = skTransform.getInversed();

  m_loadedResources[skeletalMesh->name] = skeletalMesh;
  m_loadedResources[skeleton->name] = skeleton;

  if (scene->HasAnimations()) {
    auto animation = make_shared<AnimationResource>();
    proccessAnimation(scene, animation, 1);
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
ResourceManager::processSkeleton(const aiMesh* mesh,
                                 const aiScene* scene,
                                 SPtr<SkeletalMeshResource>& skeletalMesh,
                                 SPtr<SkeletonResource>& skeleton)
{
  //UMap<String, std::pair<int32, Matrix4>> boneInfo;
  Vector<uint32> boneCounts;
  boneCounts.resize(mesh->mNumVertices, 0);
  //skeleton->boneCount = mesh->mNumBones;

  for (uint32 i = 0; i < mesh->mNumBones; ++i) {
    aiBone* bone = mesh->mBones[i];
    Matrix4 boneMatrix(static_cast<float>(bone->mOffsetMatrix.a1),
                       static_cast<float>(bone->mOffsetMatrix.a2),
                       static_cast<float>(bone->mOffsetMatrix.a3),
                       static_cast<float>(bone->mOffsetMatrix.a4),

                       static_cast<float>(bone->mOffsetMatrix.b1),
                       static_cast<float>(bone->mOffsetMatrix.b2),
                       static_cast<float>(bone->mOffsetMatrix.b3),
                       static_cast<float>(bone->mOffsetMatrix.b4),

                       static_cast<float>(bone->mOffsetMatrix.c1),
                       static_cast<float>(bone->mOffsetMatrix.c2),
                       static_cast<float>(bone->mOffsetMatrix.c3),
                       static_cast<float>(bone->mOffsetMatrix.c4),

                       static_cast<float>(bone->mOffsetMatrix.d1),
                       static_cast<float>(bone->mOffsetMatrix.d2),
                       static_cast<float>(bone->mOffsetMatrix.d3),
                       static_cast<float>(bone->mOffsetMatrix.d4));

    String boneName = bone->mName.C_Str();

    if (skeleton->boneInfo.find(boneName) == skeleton->boneInfo.end()) {
      skeleton->boneInfo[boneName] = { i, boneMatrix };
      ++skeleton->boneCount;

      for (uint32 j = 0; j < bone->mNumWeights; ++j)
      {
        uint32 id = bone->mWeights[j].mVertexId;
        float weight = bone->mWeights[j].mWeight;
        boneCounts[id]++;

        switch (boneCounts[id])
        {
        case 1:
          skeletalMesh->vertices[id].boneIds.x = static_cast<float>(i);
          skeletalMesh->vertices[id].boneWeights.x = weight;
          break;
        case 2:
          skeletalMesh->vertices[id].boneIds.y = static_cast<float>(i);
          skeletalMesh->vertices[id].boneWeights.y = weight;
          break;
        case 3:
          skeletalMesh->vertices[id].boneIds.z = static_cast<float>(i);
          skeletalMesh->vertices[id].boneWeights.z = weight;
          break;
        case 4:
          skeletalMesh->vertices[id].boneIds.w = static_cast<float>(i);
          skeletalMesh->vertices[id].boneWeights.w = weight;
          break;
        default:

          break;
        }
      }
    }

    //if(str == "pelvis") {
    //  int32 a = 10;
    //}
    //else {
    //  
    //}
  }

  for (uint32 i = 0; i < skeletalMesh->vertices.size(); ++i)
  {
    Vector4& boneWeigths = skeletalMesh->vertices[i].boneWeights;
    float totalWeight = boneWeigths.x +
                        boneWeigths.y +
                        boneWeigths.z +
                        boneWeigths.w;
    if (totalWeight > 0.0f)
    {
      skeletalMesh->vertices[i].boneWeights = Vector4(boneWeigths.x / totalWeight,
                                                      boneWeigths.y / totalWeight,
                                                      boneWeigths.z / totalWeight,
                                                      boneWeigths.w / totalWeight);
    }
  }

  readSkeleton(skeleton->bones, scene->mRootNode, skeleton->boneInfo);
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

void
ResourceManager::proccessAnimation(const aiScene* scene,
                                   SPtr<AnimationResource>& animation,
                                   uint32 index)
{
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
  animation->duration = static_cast<float>(anim->mDuration) *
                        static_cast<float>(anim->mTicksPerSecond);
  animation->boneTransform = {};

  // Load positions, rotations and scales for each bone
  for (uint32 i = 0; i < anim->mNumChannels; ++i)
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
  }
}

// TODO : This function.
SPtr<Resource>
ResourceManager::loadAnimations(const String&)
{
  /*Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                aiProcessPreset_TargetRealtime_MaxQuality |
                                                aiProcess_FlipUVs);

  proccessAnimation(pScene);*/

  return SPtr<Resource>();
}
}
