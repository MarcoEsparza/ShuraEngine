/*************************************************************/
/*
*  @file    shResourceManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
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
#include <shLogger.h>
#include <shTimer.h>
#include "shFileExplorer.h"
#include "shCodecManager.h"

#include "shResource.h"
#include "shMeshResource.h"
#include "shImageResource.h"
#include "shSkeletonResource.h"
#include "shAnimationResource.h"
#include "shAsset.h"
#include "shCubeMap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

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

const Vector<String> ResourceManager::MODEL_EXTENSIONS = { ".fbx",
                                                           ".obj",
                                                           ".gltf" };
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
  if(vertex.boneWeights.x == 0.0f) {
    vertex.boneIds.x = static_cast<float>(boneID);
    vertex.boneWeights.x = weight;
  }
  if (vertex.boneWeights.y == 0.0f) {
    vertex.boneIds.y = static_cast<float>(boneID);
    vertex.boneWeights.y = weight;
  }
  if (vertex.boneWeights.z == 0.0f) {
    vertex.boneIds.z = static_cast<float>(boneID);
    vertex.boneWeights.z = weight;
  }
  if (vertex.boneWeights.w == 0.0f) {
    vertex.boneIds.w = static_cast<float>(boneID);
    vertex.boneWeights.w = weight;
  }

  /*if (vertex.boneIds.x < 0) {
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
  }*/
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

void
ResourceManager::onStartUp()
{
  
}

void
ResourceManager::loadDefaultResources()
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pErrorTex = graphMan.createErrorTexture();
  auto pErrorImg = sh_makeShared<ImageResource>();
  pErrorImg->texture = pErrorTex;
  m_loadedResources["ErrorTexture"] = pErrorImg;

  auto pNormalTex = graphMan.createDefaultNormalTexture();
  auto pNormalImg = sh_makeShared<ImageResource>();
  pNormalImg->texture = pNormalTex;
  m_loadedResources["DefaultNormal"] = pNormalImg;

  auto pBlackTex = graphMan.createBlackTexture();
  auto pBlackImg = sh_makeShared<ImageResource>();
  pBlackImg->texture = pBlackTex;
  m_loadedResources["BlackTexture"] = pBlackImg;

  Path whiteTexPath(FileExplorer::DEFAULT_ASSETS_FOLDER + "Textures/White.png");
  auto white = cast::re_ptr<ImageResource>(loadResourceFromFile(whiteTexPath));
  /*auto normal = cast::re_ptr<ImageResource>(loadResourceFromFile(
                                              Path("resources/textures/normal.png")));*/

  Path cubePath(FileExplorer::DEFAULT_ASSETS_FOLDER + "Models/cube.fbx");
  auto pCube = cast::re_ptr<StaticMeshResource>(loadResourceFromFile(cubePath));
}

SPtr<Resource>
ResourceManager::loadResourceFromFile(const Path& filePath)
{
  CodecManager& codecMan = g_codecManager();

  // Check if resource is already on memory
  if (isResourceLoaded(filePath.filename())) {
    return m_loadedResources[filePath.filename()];
  }

  SPtr<Resource> resource;

  // Check if there is a cache for resource
  //if (isCacheForResource(filePath, resource)) {
  //  if(resource) {
  //    return resource;
  //  }
  //  //return resource;
  //}

  auto codec = codecMan.getCodecByExtension(filePath.extension());
  if (codec != nullptr) {
    if (codec->decode(filePath)) {
      String resName = filePath.filename();
      resource = m_loadedResources[resName];
    }

    if (resource) {
      return resource;
    }
  }

  /*if (filePath.compareExtensions(IMAGE_EXTENSIONS)) {
    resource = loadTextureFromFile(filePath);
  }*/
  if (filePath.compareExtensions(MODEL_EXTENSIONS)) {
    Timer timer;
    resource = loadModelFromFile(filePath);
    float elapsed = timer.getTime();
    String logMsg = filePath.string() + " loaded in " +
                    std::to_string(elapsed) + " seconds.";
    SH_LOG(LogVerbosity::kInfo, logMsg);
  }
  else if (filePath.compareExtensions({ ".cube" })) {
    resource = loadCubeMapFromFile(filePath);
  }
  /*else if (filePath.compareExtensions({ ".dds" })) {
    resource = loadTextureFromDDS(filePath);
  }*/
  else {
    return nullptr;
  }

  return resource;
}

bool
ResourceManager::addResource(const SPtr<Resource> pRes)
{
  if(isResourceLoaded(pRes->getName())) {
    return false;
  }

  m_loadedResources[pRes->getName()] = pRes;
  if (isResourceLoaded(pRes->getName())) {
    return true;
  }

  return false;
}

SPtr<Resource>
ResourceManager::getResource(const String& resourceName)
{
  if (isResourceLoaded(resourceName)) {
    return m_loadedResources[resourceName];
  }
  return nullptr;
}

bool
ResourceManager::saveResourceToAsset(const SPtr<Resource> pRes, const String& path)
{
  Asset resAsset;
  return resAsset.saveResourceToAsset(pRes, path);
}

bool
ResourceManager::isResourceLoaded(const String& fileName)
{
  auto resObj = m_loadedResources.find(fileName);

  if (resObj != m_loadedResources.end()) {
    return true;
  }

  return false;
}

SPtr<Resource>
ResourceManager::isResourceLoaded(const Path& fileName)
{
  auto resObj = m_loadedResources.find(fileName.string());

  if (resObj != m_loadedResources.end()) {
    return (*resObj).second;
  }

  return nullptr;
}

//bool
//ResourceManager::isResourceOnMemory(const Path& filePath, SPtr<Resource>& pRes)
//{
//  SystemPath path = filePath.string();
//
//  auto resObj = m_loadedResources.find(path.filename().string());
//
//  if (resObj != m_loadedResources.end()) {
//    pRes = (*resObj).second;
//    return true;
//  }
//
//  pRes = nullptr;
//  return false;
//}

//bool
//ResourceManager::isCacheForResource(const Path& filePath, SPtr<Resource>& pRes)
//{
//  //auto& logger = g_logger();
//
//  if (filePath.compareExtensions(IMAGE_EXTENSIONS)) {
//    SystemPath path = filePath.string();
//    path.replace_extension(".dds");
//    SystemPath fullPath = "resources/assets/textures/" + path.filename().string();
//
//    if (std::filesystem::exists(fullPath)) {
//      pRes = loadTextureFromDDS(fullPath.string());
//      return true;
//    }
//  }
//  else if (filePath.compareExtensions({ ".dds" })) {
//    SystemPath fullPath = "resources/assets/textures/" + filePath.filename();
//    pRes = loadTextureFromDDS(fullPath.string());
//    return true;
//  }
//  else if (filePath.compareExtensions(MODEL_EXTENSIONS)) {
//    SystemPath path = filePath.string();
//    path.replace_extension(".sha");
//    SystemPath fullPath = "resources/assets/models/" + path.filename().string();
//
//    if (std::filesystem::exists(fullPath)) {
//      pRes = loadModelFromCache(fullPath.string());
//      return true;
//    }
//  }
//  else if(filePath.compareExtensions({ ".sha" })) {
//    //SystemPath fullPath = "resources/assets/models/" + filePath.filename();
//    Timer timer;
//    pRes = loadModelFromCache(filePath.string());
//    float elapsed = timer.getTime();
//    /*logger.consoleLog(filePath.toString() + " loaded from cache in " +
//                      std::to_string(elapsed) + " seconds.");*/
//    String logMsg = filePath.string() + " loaded from cache in " +
//                    std::to_string(elapsed) + " seconds.";
//    SH_LOG(LogVerbosity::kInfo, logMsg);
//    return true;
//  }
//
//  return false;
//}

SPtr<Resource>
ResourceManager::loadTextureFromFile(const Path& filePath)
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pImage = sh_makeShared<ImageResource>();

  int32 width = 0;
  int32 height = 0;
  int32 bpp = 0;

  //SystemPath path = fileName;
  //String file = filePath.filename();

  if (filePath.compareExtensions({".hdr"})) {
    float* data = stbi_loadf(filePath.c_str(), &width, &height, &bpp, 4);
    pImage->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);

    stbi_image_free(data);
  }
  else {
    //int32 reqComp = STBI_rgb_alpha;
    //if (path.extension() == ".jpg") {
    //  reqComp = STBI_rgb;
    //}
    uint8* data = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    pImage->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);
    stbi_image_free(data);
  }
  
  pImage->width = width;
  pImage->height = height;
  pImage->setName(filePath.filename());

  m_loadedResources[pImage->getName()] = pImage;
  pImage->setPath(filePath);

  //path = file;
  //path.replace_extension(".dds");
  //String saveTex = "resources/assets/textures/" + path.string();

  //graphMan.saveTextureToDDS(pImage->texture, saveTex);

  //Path texPath(fileName);

  return pImage;
}

SPtr<Resource>
ResourceManager::loadTextureFromDDS(const Path& filePath)
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pImage = sh_makeShared<ImageResource>();

  pImage->texture = graphMan.createTextureFromDDS(filePath);

  if(!pImage->texture) {
    return nullptr;
  }

  pImage->setName(filePath.filename());
  m_loadedResources[pImage->getName()] = pImage;
  pImage->setPath(filePath);

  return pImage;
}

SPtr<Resource>
ResourceManager::loadCubeMapFromFile(const Path& filePath)
{
  //GraphicsManager& graphMan = g_graphicsMan();
  //SystemPath file = fileName;
  auto pCubeMap = sh_makeShared<CubeMap>();
  pCubeMap->setName(filePath.filename());
  pCubeMap->loadFromFile(filePath);
  return pCubeMap;
}

SPtr<Resource>
ResourceManager::loadModelFromFile(const Path& filePath)
{
  Assimp::Importer fileImporter;

  const aiScene* pScene = fileImporter.ReadFile(filePath.string(),
                                                CUSTOM_AI_MAX_QUALITY_FLAG);

  aiNode* rootNode = pScene->mRootNode;
  if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !rootNode) {
    SH_LOG_ERROR("Assimp error: " + String(fileImporter.GetErrorString()));
    return nullptr;
  }
  if (pScene->mNumMeshes == 0) {
    SH_LOG_ERROR("Assimp error: No meshes found in the scene.");
    return nullptr;
  }
  //if (pScene->mNumMaterials == 0) {
  //  //SH_LOG_ERROR("Assimp error: No materials found in the scene.");
  //  return nullptr;
  //}
  
  aiMesh* mesh = pScene->mMeshes[0];

  if (mesh->HasBones()) {
    return createSkeletalMesh(pScene, filePath);
  }
  else {
    return createStaticMesh(filePath, rootNode, pScene);
  }
}

SPtr<Material>
ResourceManager::createMaterialFromFile(const aiMaterial* pMat, const Path& resPath)
{
  //GraphicsManager& graphMan = g_graphicsMan();
  auto pMeshMat = isMaterialLoaded(pMat->GetName().C_Str());
  //SystemPath path = resPath;
  const String directory = resPath.directory() + "/";

  if(pMeshMat){
    return pMeshMat;
  }

  pMeshMat = sh_makeShared<Material>();

  //return pMeshMat;

  uint32 diffCount = pMat->GetTextureCount(aiTextureType_DIFFUSE);
  uint32 normCount = pMat->GetTextureCount(aiTextureType_NORMALS);
  uint32 metalCount = pMat->GetTextureCount(aiTextureType_METALNESS);
  uint32 roughCount = pMat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
  uint32 aoCount = pMat->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
  uint32 emmCount = pMat->GetTextureCount(aiTextureType_EMISSIVE);
  uint32 opacityCount = pMat->GetTextureCount(aiTextureType_OPACITY);

  if (diffCount == 0) {
    // Create error texture
    pMeshMat->m_properties.properties.flags.bHasDiffuseMap = true;
    auto it = m_loadedResources.find("ErrorTexture");
    pMeshMat->m_baseColor = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasDiffuseMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = cast::re_ptr<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      pMeshMat->m_properties.properties.flags.bHasDiffuseMap = true;
      auto it = m_loadedResources.find("ErrorTexture");
      pMeshMat->m_baseColor = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_baseColor = pImage;
    }
  }

  if (normCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasNormalMap = false;
    auto it = m_loadedResources.find("DefaultNormal");
    pMeshMat->m_normal = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasNormalMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_NORMALS, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("DefaultNormal");
      pMeshMat->m_normal = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_normal = pImage;
    }
  }

  if (metalCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasMetalnessMap = false;
    auto it = m_loadedResources.find("BlackTexture");
    pMeshMat->m_metalness = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasMetalnessMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_METALNESS, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("BlackTexture");
      pMeshMat->m_metalness = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_metalness = pImage;
    }
  }

  if (roughCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasRoughnessMap = false;
    auto it = m_loadedResources.find("BlackTexture");
    pMeshMat->m_roughness = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasRoughnessMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("BlackTexture");
      pMeshMat->m_roughness = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_roughness = pImage;
    }
  }

  if (aoCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasAmbientOcclusionMap = false;
    auto it = m_loadedResources.find("BlackTexture");
    pMeshMat->m_ao = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasAmbientOcclusionMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("BlackTexture");
      pMeshMat->m_ao = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_ao = pImage;
    }
  }

  if (emmCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasEmissiveMap = false;
    auto it = m_loadedResources.find("BlackTexture");
    pMeshMat->m_emissive = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasEmissiveMap = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_EMISSIVE, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("BlackTexture");
      pMeshMat->m_emissive = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_emissive = pImage;
    }
  }

  if (opacityCount == 0) {
    pMeshMat->m_properties.properties.flags.bHasOpacityMask = false;
    auto it = m_loadedResources.find("BlackTexture");
    pMeshMat->m_opacityMask = cast::re_ptr<ImageResource>((*it).second);
  }
  else {
    pMeshMat->m_properties.properties.flags.bHasOpacityMask = true;
    aiString aiPath;
    pMat->GetTexture(aiTextureType_OPACITY, 0, &aiPath);
    Path filename(aiPath.C_Str());
    Path filePath(directory + filename.filename());
    auto pImage = sh_reinterpretPCast<ImageResource>(loadResourceFromFile(filePath));
    if (!pImage) {
      auto it = m_loadedResources.find("BlackTexture");
      pMeshMat->m_opacityMask = cast::re_ptr<ImageResource>((*it).second);
    }
    else {
      pMeshMat->m_opacityMask = pImage;
    }
  }
  
  //pMeshMat->m_type = MATERIAL_TYPE::kPBR;
  pMeshMat->setName(pMat->GetName().C_Str());
  m_loadedResources[pMeshMat->getName()] = pMeshMat;

  return pMeshMat;
}

SPtr<Material>
ResourceManager::isMaterialLoaded(const String& materialName)
{
  auto resObj = m_loadedResources.find(materialName);

  if (resObj != m_loadedResources.end()) {
    return cast::re_ptr<Material>((*resObj).second);
  }

  return nullptr;
}

void
ResourceManager::loadMaterial(const SPtr<Material>& material)
{
  m_loadedResources[material->getName()] = material;
}

SPtr<Resource>
ResourceManager::loadModelFromCache(const Path& filePath)
{
  auto pRes = Asset::loadResourceFromAsset(filePath);

  if (pRes == nullptr) {
    return nullptr;
  }
  //SystemPath path = fileName;
  pRes->setName(filePath.filename());
  pRes->setPath(filePath);
  m_loadedResources[pRes->getName()] = pRes;

  return pRes;
}

SPtr<Resource>
ResourceManager::createStaticMesh(const Path& filePath,
                                  const aiNode* node,
                                  const aiScene* scene)
{
  auto currentMesh = sh_makeShared<StaticMeshResource>();

  //SystemPath path = fileName;
  currentMesh->setName(filePath.filename());
  currentMesh->setPath(filePath);
  proccessStaticMeshNode(node, scene, currentMesh);
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
  auto currentMat = createMaterialFromFile(aiMat, currentMesh->getPath());

  if(currentMat == nullptr) {
    auto it = m_loadedResources.find("ErrorTexture");
    currentMat = sh_makeShared<Material>();
    currentMat->m_baseColor = cast::re_ptr<ImageResource>((*it).second);
    currentMat->setName("ErrorMaterial");
  }

  if (currentMesh->m_materials.empty()) {
    currentMesh->m_materials.push_back(currentMat);
  }
  else {
    for (uint32 i = 0; i < currentMesh->m_materials.size(); ++i) {
      auto mat = currentMesh->m_materials[i].lock();
      if (currentMat->getName() == mat->getName()) {
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
ResourceManager::createSkeletalMesh(const aiScene* scene, const Path& filePath)
{
  auto skeletalMesh = sh_makeShared<SkeletalMeshResource>();
  auto skeleton = sh_makeShared<SkeletonResource>();

  //skeletalMesh->m_materials.resize(scene->mNumMaterials);

  auto rootBone = sh_makeShared<BoneHierarchy>();
  skeletalMesh->setPath(filePath);
  proccessSkeletalMeshNode(scene->mRootNode, scene, rootBone, skeletalMesh, skeleton);

  Path file = filePath;
  file.replaceExtension("");
  skeletalMesh->setName(file.filename());
  skeleton->setName(file.filename() + "Skeleton");

  m_loadedResources[skeletalMesh->getName()] = skeletalMesh;
  m_loadedResources[skeleton->getName()] = skeleton;

  checkModelAnimations(scene, skeleton);
  skeletalMesh->m_skeleton = skeleton;

  return skeletalMesh;
}

void
ResourceManager::proccessSkeletalMeshNode(const aiNode* node,
                                          const aiScene* scene,
                                          SPtr<BoneHierarchy> boneNode,
                                          SPtr<SkeletalMeshResource>& skeletalMesh,
                                          SPtr<SkeletonResource>& skeleton)
{
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    proccessSkeletalMesh(mesh, scene, skeletalMesh, skeleton);
  }
  boneNode->name = node->mName.C_Str();
  boneNode->transformation = aiMatrixToMatrix4(node->mTransformation);

  for (uint32 i = 0; i < node->mNumChildren; ++i) {
    auto childBone = sh_makeShared<BoneHierarchy>();
    boneNode->children.push_back(childBone);
    proccessSkeletalMeshNode(node->mChildren[i], scene, childBone, skeletalMesh, skeleton);
  }
}

void
ResourceManager::proccessSkeletalMesh(const aiMesh* mesh,
                                      const aiScene* scene,
                                      SPtr<SkeletalMeshResource>& skeletalMesh,
                                      SPtr<SkeletonResource>& skeleton)
{
  // Skeletal Mesh
  SkeletalMeshData currentMeshInfo;

  Vector<VertexData> currentMeshVertices = getVertexDataFromMesh(mesh);
  currentMeshInfo.numVertices = mesh->mNumVertices;
  for (auto& vertex : currentMeshVertices) {
    skeletalMesh->m_vertices.push_back(vertex);
  }
  skeletalMesh->m_indices = getIndicesFromMesh(mesh, currentMeshInfo.numIndices);

  currentMeshInfo.name = mesh->mName.C_Str();
  //currentMeshInfo.materialIndex = mesh->mMaterialIndex;

  // Materials
  /*if (skeletalMesh->m_materials[currentMeshInfo.materialIndex] == nullptr) {
    auto meshMat = createMaterialFromFile(scene->mMaterials[mesh->mMaterialIndex]);
    auto& imgRes = m_loadedResources["White.png"];
    auto img = sh_reinterpretPCast<ImageResource>(imgRes);
    meshMat->baseColor = img->texture;
    auto mat = scene->mMaterials[mesh->mMaterialIndex];
    meshMat->name = mat->GetName().C_Str();
    skeletalMesh->m_materials[currentMeshInfo.materialIndex] = meshMat;
  }*/

  auto currentMat = createMaterialFromFile(scene->mMaterials[mesh->mMaterialIndex],
                                           skeletalMesh->getPath());

  if (skeletalMesh->m_materials.empty()) {
    skeletalMesh->m_materials.push_back(currentMat);
  }
  else {
    for (uint32 i = 0; i < skeletalMesh->m_materials.size(); ++i) {
      auto& mat = skeletalMesh->m_materials[i];
      if (currentMat->getName() == mat->getName()) {
        currentMeshInfo.materialIndex = i;
        break;
      }
      else {
        if (i == skeletalMesh->m_materials.size() - 1) {
          skeletalMesh->m_materials.push_back(currentMat);
          currentMeshInfo.materialIndex = i + 1;
        }
      }
    }
  }

  skeletalMesh->m_meshes.push_back(currentMeshInfo);

  // Bones
  for (uint32 b = 0; b < mesh->mNumBones; ++b) {
    aiBone* bone = mesh->mBones[b];
    String boneName = bone->mName.C_Str();

    int32 boneIndex = 0;
    if(skeleton->m_boneMapping.find(boneName) == skeleton->m_boneMapping.end()) {
      boneIndex = skeleton->m_boneCount++;
      skeleton->m_boneMapping[boneName] = boneIndex;

      BoneInfo bi;
      bi.name = boneName;
      bi.offset = aiMatrixToMatrix4(bone->mOffsetMatrix);
      skeleton->m_bones.push_back(bi);
    }
    else {
      boneIndex = skeleton->m_boneMapping[boneName];
    }

    // Assign weights to vertices
    for (uint32 w = 0; w < bone->mNumWeights; ++w) {
      uint32 vertexID = bone->mWeights[w].mVertexId;
      float weight = bone->mWeights[w].mWeight;
      setVertexBoneData(skeletalMesh->m_vertices[vertexID], boneIndex, weight);
    }
  }
}

void
ResourceManager::checkModelAnimations(const aiScene* scene, SPtr<SkeletonResource>& skeleton)
{
  if (scene->HasAnimations()) {
    for (uint32 i = 0; i < scene->mNumAnimations; ++i) {
      auto animation = sh_makeShared<AnimationResource>();
      //animation->m_skeletonData = skeleton;
      aiAnimation* anim = scene->mAnimations[i];
      proccessAnimation(anim, animation);
      String name = scene->mAnimations[i]->mName.C_Str();
      m_loadedResources[name] = animation;
      skeleton->m_animations.push_back(animation);
    }
  }
}

void
ResourceManager::proccessAnimation(const aiAnimation* anim,
                                   SPtr<AnimationResource>& outAnimation)
{
  outAnimation->m_duration = cast::st<float>(anim->mDuration);
  outAnimation->m_ticksPerSecond = cast::st<float>(anim->mTicksPerSecond) != 0.0f ?
                                   cast::st<float>(anim->mTicksPerSecond) :
                                   25.0f;

  for(uint32 c = 0; c < anim->mNumChannels; ++c) {
    aiNodeAnim* channel = anim->mChannels[c];
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

    outAnimation->m_channels[outChannel.name] = outChannel;
  }
}

ResourceManager& g_resourceMan()
{
  return ResourceManager::instance();
}
}
