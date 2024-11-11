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
ResourceManager::loadModelFromFile(const String& fileName)
{
  if (isModelLoaded(fileName) != nullptr)
  {
    return;
  }

  SystemPath cacheFileName = fileName;
  cacheFileName.replace_extension(".shm");

  if (existCacheForModel(cacheFileName.string())) {
    loadModelFromCache(cacheFileName.string());
    return;
  }
  else {
    Assimp::Importer fileImporter;

    const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                  aiProcessPreset_TargetRealtime_MaxQuality |
                                                  aiProcess_FlipUVs);

    SPtr<Model> model = make_shared<Model>();

    proccessNode(pScene->mRootNode, pScene, model);

    m_loadedModels[fileName] = model;

    //createCacheForModel(fileName, model);
  }

  return;
}

SPtr<Texture2D>
ResourceManager::loadTextureFromFile(const String& fileName)
{
  if (isTextureLoaded(fileName) != nullptr) {
    return m_loadedTextures[fileName];
  }

  auto pTexture = GraphicsManager::instance().createTextureFromFile(fileName);

  m_loadedTextures[fileName] = pTexture;

  return m_loadedTextures[fileName];
}

SPtr<Model>
ResourceManager::isModelLoaded(const String& fileName)
{
  auto modelObj = m_loadedModels.find(fileName);

  if (modelObj != m_loadedModels.end()) {
    return (*modelObj).second;
  }

  return nullptr;
}

SPtr<Texture2D>
ResourceManager::isTextureLoaded(const String& fileName)
{
  auto texObj = m_loadedTextures.find(fileName);

  if (texObj != m_loadedTextures.end()) {
    return (*texObj).second;
  }

  return nullptr;
}

void
ResourceManager::proccessNode(const aiNode* node, const aiScene* scene, SPtr<Model>& model)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

    proccessMesh(mesh, scene, model);
  }

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    proccessNode(node->mChildren[i], scene, model);
  }
}

void
ResourceManager::proccessMesh(const aiMesh* mesh, const aiScene* scene, SPtr<Model>& model)
{
  Mesh currentMesh;

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

    model->vertices.push_back(vertex);
  }

  currentMesh.numVertex = mesh->mNumVertices;

  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    currentMesh.numIndices += face.mNumIndices;

    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      model->indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial* pMat = scene->mMaterials[mesh->mMaterialIndex];

  currentMesh.matIndex = mesh->mMaterialIndex;

  model->materials[currentMesh.matIndex] = loadMaterial(pMat->GetName().C_Str());

  model->meshes.push_back(currentMesh);
}

SPtr<Material>
ResourceManager::loadMaterial(const String& name)
{
  if (isMaterialLoaded(name) != nullptr) {
    return m_loadedMaterials[name];
  }

  auto pMat = make_shared<Material>();

  pMat->name = name;
  pMat->textures[TEXTURE_TYPE::kBaseColor] = loadTextureFromFile("resources/White.png");
  m_loadedMaterials[name] = pMat;

  return m_loadedMaterials[name];
}

void
ResourceManager::loadAnimations(const String& fileName, const SPtr<Model>& model)
{
  Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(fileName,
                                                aiProcessPreset_TargetRealtime_MaxQuality |
                                                aiProcess_FlipUVs);

  proccessAnimation(pScene);
}

SPtr<Material>
ResourceManager::isMaterialLoaded(const String& name)
{
  auto matObj = m_loadedMaterials.find(name);

  if (matObj != m_loadedMaterials.end()) {
    return (*matObj).second;
  }

  return nullptr;
}

bool
ResourceManager::readSkeleton(Bone&,
                              aiNode*,
                              UMap<String,
                              Pair<int32, Matrix4>>)
{
  return false;
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

void
ResourceManager::loadModelFromCache(const String& fileName)
{
  SPtr<Model> model = make_shared<Model>();

  fstream modelFile(fileName, ios::in | ios::binary);

  if (!modelFile.is_open())
  {
    return;
  }

  ModelCacheHeader mch;

  modelFile.read(reinterpret_cast<char*>(&mch), sizeof(mch));

  Vector<char> meshData;
  Vector<char> verticesData;
  Vector<char> indicesData;

  int32 meshDataSize = mch.numMeshes * sizeof(Mesh);
  int32 verticesDataSize = mch.numVertices * sizeof(VertexData);
  int32 indicesDataSize = mch.numIndices * sizeof(uint32);

  meshData.resize(meshDataSize);
  verticesData.resize(verticesDataSize);
  indicesData.resize(indicesDataSize);

  modelFile.read(reinterpret_cast<char*>(&meshData[0]), meshDataSize);
  modelFile.read(reinterpret_cast<char*>(&verticesData[0]), verticesDataSize);
  modelFile.read(reinterpret_cast<char*>(&indicesData[0]), indicesDataSize);

  model->meshes.resize(mch.numMeshes);
  model->vertices.resize(mch.numVertices);
  model->indices.resize(mch.numIndices);

  memcpy(model->meshes.data(), meshData.data(), meshDataSize);
  memcpy(model->vertices.data(), verticesData.data(), verticesDataSize);
  memcpy(model->indices.data(), indicesData.data(), indicesDataSize);

  modelFile.close();

  m_loadedModels[fileName] = model;
}

void
ResourceManager::createCacheForModel(const String& fileName, SPtr<Model>& model)
{
  ModelCacheHeader mch;

  mch.numMeshes = static_cast<uint32>(model->meshes.size());
  mch.numVertices = static_cast<uint32>(model->vertices.size());
  mch.numIndices = static_cast<uint32>(model->indices.size());

  Vector<char> meshData;
  Vector<char> verticesData;
  Vector<char> indicesData;

  int32 meshDataSize = mch.numMeshes * sizeof(Mesh);
  int32 verticesDataSize = mch.numVertices * sizeof(VertexData);
  int32 indicesDataSize = mch.numIndices * sizeof(uint32);

  meshData.resize(meshDataSize);
  verticesData.resize(verticesDataSize);
  indicesData.resize(indicesDataSize);

  memcpy(meshData.data(), model->meshes.data(), meshDataSize);
  memcpy(verticesData.data(), model->vertices.data(), verticesDataSize);
  memcpy(indicesData.data(), model->indices.data(), indicesDataSize);

  SystemPath cacheName = fileName;
  cacheName.replace_extension(".shm");
  fstream cacheFile(cacheName.string(), ios::out | ios::binary);

  if (!cacheFile.is_open()) {
    return;
  }

  cacheFile.write(reinterpret_cast<char*>(&mch), sizeof(mch));
  cacheFile.write(meshData.data(), meshData.size());
  cacheFile.write(verticesData.data(), verticesData.size());
  cacheFile.write(indicesData.data(), indicesData.size());

  cacheFile.close();
}
}
