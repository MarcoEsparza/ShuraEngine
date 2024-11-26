/*************************************************************/
/*
*  @file    shResourceManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/06
*  @brief
*
*
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
SPtr<Resource>
ResourceManager::loadResourceFromFile(const String& fileName,
                                      const RESOURCE_TYPE::E rType)
{
  if (isResourceLoaded(fileName)) {
    return m_loadedResources[fileName];
  }
  
  SPtr<Resource> resource;
  SystemPath extension = fileName;
  
  if (rType == RESOURCE_TYPE::kTexture) {
    resource = loadTextureFromFile(fileName);
  }
  else if (rType == RESOURCE_TYPE::kModel) {
    resource = loadModelFromFile(fileName);
  }

  m_loadedResources[fileName] = resource;

  return resource;
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
ResourceManager::loadModelFromFile(const String& fileName)
{
  SystemPath cacheFileName = fileName;
  cacheFileName.replace_extension(".shm");

  if (existCacheForModel(cacheFileName.string())) {
    loadModelFromCache(cacheFileName.string());
    return SPtr<Resource>();
  }
  else {
    Assimp::Importer fileImporter;

    const aiScene* pScene = fileImporter.ReadFile(fileName,
      aiProcessPreset_TargetRealtime_MaxQuality |
      aiProcess_FlipUVs);

    proccessNode(pScene->mRootNode, pScene);
  }

  return SPtr<Resource>();
}

SPtr<Resource>
ResourceManager::loadTextureFromFile(const String& fileName)
{
  auto pImage = make_shared<ImageResource>();

  pImage->texture = GraphicsManager::instance().createTextureFromFile(fileName);

  return pImage;
}

void
ResourceManager::proccessNode(const aiNode* node, const aiScene* scene)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessMesh(mesh, scene);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessNode(node->mChildren[i], scene);
  }
}

void
ResourceManager::proccessMesh(const aiMesh* mesh, const aiScene* scene)
{
  StaticMeshResource currentMesh;

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

    currentMesh.vertices.push_back(vertex);
  }

  currentMesh.numVertex = mesh->mNumVertices;

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    currentMesh.numIndices += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      currentMesh.indices.push_back(face.mIndices[j]);
    }
  }
}

void
ResourceManager::processSkeleton(const aiMesh* mesh, const aiScene* scene)
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
      float weight = bone->mWeights[j].mWeight;
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

SPtr<Resource>
ResourceManager::loadAnimations(const String& fileName)
{
  /*Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                aiProcessPreset_TargetRealtime_MaxQuality |
                                                aiProcess_FlipUVs);

  proccessAnimation(pScene);*/

  return SPtr<Resource>();
}



void
ResourceManager::proccessAnimation(const aiScene*)
{

}

bool
ResourceManager::existCacheForModel(const String& fileName)
{
  fstream file(fileName);

  bool exist = file.good();

  file.close();

  return exist;
}

SPtr<Resource>
ResourceManager::loadModelFromCache(const String& fileName)
{
  //SPtr<Model> model = make_shared<Model>();

  fstream modelFile(fileName, ios::in | ios::binary);

  if (!modelFile.is_open())
  {
    return SPtr<Resource>();
  }

  ModelCacheHeader mch;

  modelFile.read(reinterpret_cast<char*>(&mch), sizeof(mch));

  Vector<char> meshData;
  Vector<char> verticesData;
  Vector<char> indicesData;

  //int32 meshDataSize = mch.numMeshes * sizeof(Mesh);
  int32 verticesDataSize = mch.numVertices * sizeof(VertexData);
  int32 indicesDataSize = mch.numIndices * sizeof(uint32);

  //meshData.resize(meshDataSize);
  verticesData.resize(verticesDataSize);
  indicesData.resize(indicesDataSize);

  //modelFile.read(reinterpret_cast<char*>(&meshData[0]), meshDataSize);
  modelFile.read(reinterpret_cast<char*>(&verticesData[0]), verticesDataSize);
  modelFile.read(reinterpret_cast<char*>(&indicesData[0]), indicesDataSize);

 /* model->meshes.resize(mch.numMeshes);
  model->vertices.resize(mch.numVertices);
  model->indices.resize(mch.numIndices);*/

  //memcpy(model->meshes.data(), meshData.data(), meshDataSize);
  /*memcpy(model->vertices.data(), verticesData.data(), verticesDataSize);
  memcpy(model->indices.data(), indicesData.data(), indicesDataSize);*/

  modelFile.close();

  return SPtr<Resource>();
}

void
ResourceManager::createCacheForModel(const String& fileName)
{
  ModelCacheHeader mch;

 /* mch.numMeshes = static_cast<uint32>(model->meshes.size());
  mch.numVertices = static_cast<uint32>(model->vertices.size());
  mch.numIndices = static_cast<uint32>(model->indices.size());*/

  Vector<char> meshData;
  Vector<char> verticesData;
  Vector<char> indicesData;

  //int32 meshDataSize = mch.numMeshes * sizeof(Mesh);
  //int32 verticesDataSize = mch.numVertices * sizeof(VertexData);
  //int32 indicesDataSize = mch.numIndices * sizeof(uint32);

  //meshData.resize(meshDataSize);
  //verticesData.resize(verticesDataSize);
  //indicesData.resize(indicesDataSize);

  //memcpy(meshData.data(), model->meshes.data(), meshDataSize);
  //memcpy(verticesData.data(), model->vertices.data(), verticesDataSize);
  //memcpy(indicesData.data(), model->indices.data(), indicesDataSize);

  /*SystemPath cacheName = fileName;
  cacheName.replace_extension(".shm");
  fstream cacheFile(cacheName.string(), ios::out | ios::binary);

  if (!cacheFile.is_open()) {
    return;
  }

  cacheFile.write(reinterpret_cast<char*>(&mch), sizeof(mch));
  cacheFile.write(meshData.data(), meshData.size());
  cacheFile.write(verticesData.data(), verticesData.size());
  cacheFile.write(indicesData.data(), indicesData.size());

  cacheFile.close();*/
}
}
