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
#include "shBone.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// TODO: Change this to a wrapper.
#include <fstream>

using std::fstream;
using std::ios;
using std::getline;

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
                                                aiProcessPreset_TargetRealtime_MaxQuality |
                                                aiProcess_FlipUVs);

  auto* node = pScene->mRootNode->mChildren[0];
  auto* mesh = pScene->mMeshes[node->mMeshes[0]];

  if (mesh->HasBones()) {
    createSkeletalMesh(pScene, fileName);
  }
  else {
    proccessStaticMeshNode(pScene->mRootNode, pScene);
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
ResourceManager::proccessStaticMeshNode(const aiNode* node, const aiScene* scene)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessStaticMesh(mesh);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessStaticMeshNode(node->mChildren[i], scene);
  }
}

void
ResourceManager::proccessStaticMesh(const aiMesh* mesh)
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
  
  currentMesh->name = mesh->mName.C_Str();
  m_loadedResources[currentMesh->name] = currentMesh;
}

void
ResourceManager::createSkeletalMesh(const aiScene* scene, const String& fileName)
{
  auto skeletalMesh = make_shared<SkeletalMeshResource>();

  proccessSkeletalMeshNode(scene->mRootNode, scene, skeletalMesh);

  SystemPath file = fileName;
  file.replace_extension("");
  skeletalMesh->name = file.filename().string();

  m_loadedResources[skeletalMesh->name] = skeletalMesh;
}

void
ResourceManager::proccessSkeletalMeshNode(const aiNode* node,
                                          const aiScene* scene,
                                          SPtr<SkeletalMeshResource>& skeletalMesh)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessSkeletalMesh(mesh, scene, skeletalMesh);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessSkeletalMeshNode(node->mChildren[i], scene, skeletalMesh);
  }
}

void
ResourceManager::proccessSkeletalMesh(const aiMesh* mesh,
                                      const aiScene* scene,
                                      SPtr<SkeletalMeshResource>& skeletalMesh)
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
}

void
ResourceManager::processSkeleton(const aiMesh* mesh, const aiScene*)
{
  UMap<String, std::pair<int, Matrix4>> boneInfo;
  Vector<uint32> boneCounts;
  //boneCounts.resize(model->vertices.size(), 0);
  //model->skeleton.boneCount = mesh->mNumBones;

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

    boneInfo[bone->mName.C_Str()] = { i, boneMatrix };

    for (uint32 j = 0; j < bone->mNumWeights; ++j)
    {
      uint32 id = bone->mWeights[j].mVertexId;
      //float weight = bone->mWeights[j].mWeight;
      boneCounts[id]++;

      /*switch (boneCounts[id])
      {
      case 1:
        model->vertices[id].boneIds.x = i;
        model->vertices[id].boneWeights.x = weight;
        break;
      case 2:
        model->vertices[id].boneIds.y = i;
        model->vertices[id].boneWeights.y = weight;
        break;
      case 3:
        model->vertices[id].boneIds.z = i;
        model->vertices[id].boneWeights.z = weight;
        break;
      case 4:
        model->vertices[id].boneIds.w = i;
        model->vertices[id].boneWeights.w = weight;
        break;
      default:

        break;
      }*/
    }
  }

  /*for (uint32 i = 0; i < model->vertices.size(); ++i)
  {
    Vector4& boneWeigths = model->vertices[i].boneWeights;
    float totalWeight = boneWeigths.x +
                        boneWeigths.y +
                        boneWeigths.z +
                        boneWeigths.w;
    if (totalWeight > 0.0f)
    {
      model->vertices[i].boneWeights = Vector4(boneWeigths.x / totalWeight,
                                               boneWeigths.y / totalWeight,
                                               boneWeigths.z / totalWeight,
                                               boneWeigths.w / totalWeight);
    }
  }*/

  //readSkeleton(model->skeleton.bones, scene->mRootNode, boneInfo);
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

// TODO : This function.
void
ResourceManager::proccessAnimation(const aiScene*)
{

}
}
