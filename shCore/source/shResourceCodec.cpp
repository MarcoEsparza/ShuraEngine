/*****************************************************************************/
/*
*  @file    shResourceCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shResourceCodec.h"
#include "shResourceManager.h"
#include <shLogger.h>
#include <shFileExplorer.h>
#include <shStringID.h>
#include <shException.h>
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shImageResource.h"

namespace shEngineSDK {
UID
ResourceCodec::getUID() const
{
  return StringID("ResourceCodec").getID();
}

Vector<String>
ResourceCodec::getSupportedExtensions() const
{
  return { ".shr" };
}

bool
ResourceCodec::decode(const Path& filePath) const
{
  FILE* file = nullptr;

#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&file, filePath.string().c_str(), "rb");
#else
  file = fopen(filePath.toString().c_str(), "rb");
#endif

  if (!file) {
    SH_LOG_ERROR("Failed to open file: " + filePath.string());
    return false;
  }
  setvbuf(file, NULL, _IONBF, 0); // No buffering

  RESOURCE_TYPE::E resType = RESOURCE_TYPE::kCount;
  fread(&resType, sizeof(RESOURCE_TYPE::E), 1, file);

  if (resType == RESOURCE_TYPE::kStaticMesh) {
    SH_LOG_INFO("Decoding static mesh from file: " + filePath.string());
    return decodeStaticMesh(file, filePath);
  }
  else if (resType == RESOURCE_TYPE::kSkeletalMesh) {
  }
  else if (resType == RESOURCE_TYPE::kSkeleton) {
  }
  else if (resType == RESOURCE_TYPE::kAnmimation) {
  }
  else if (resType == RESOURCE_TYPE::kMaterial) {
  }
  else {
    SH_LOG_ERROR("Unsupported resource type in file: " + filePath.string());
    fclose(file);
    return false;
  }

  fclose(file);
  return false;
}

bool
ResourceCodec::encode(const String& objName, const Path& saveFilePath) const
{
  ResourceManager& resMan = g_resourceMan();
  auto pRes = resMan.getResource(objName);

  if (pRes == nullptr) {
    SH_LOG_ERROR("Resource not found for encoding: " + objName);
    return false;
  }

  if (pRes->getType() == RESOURCE_TYPE::kStaticMesh) {
    SH_LOG_INFO("Encoding static mesh to file: " + saveFilePath.string());
    return encodeStaticMesh(objName, saveFilePath);
  }
  else if(pRes->getType() == RESOURCE_TYPE::kSkeletalMesh) {
  }
  else if(pRes->getType() == RESOURCE_TYPE::kSkeleton) {
  }
  else if(pRes->getType() == RESOURCE_TYPE::kAnmimation) {
  }
  else if(pRes->getType() == RESOURCE_TYPE::kCubeMap) {
  }
  else if(pRes->getType() == RESOURCE_TYPE::kMaterial) {
    //SH_LOG_INFO("Encoding material to file: " + saveFilePath.string());
    //return encodeMaterial(objName, saveFilePath);
  }
  else {
    SH_LOG_ERROR("Unsupported resource type for encoding: " + objName);
    return false;
  }

  return false;
}

SPtr<Material>
ResourceCodec::getMaterialFromFile(FILE* pFile, SPtr<Material>& pMat, const Path& filePath) const
{
  ResourceManager& resMan = g_resourceMan();

  MaterialAssetHeader mah = {};
  fread(&mah, sizeof(MaterialAssetHeader), 1, pFile);
  String matName;
  matName.resize(mah.nameSize);
  fread(matName.data(), sizeof(char), mah.nameSize, pFile);

  pMat = cast::re_ptr<Material>(resMan.getResource(matName));
  if (pMat) {
    return pMat;
  }

  SH_LOG_INFO("Loading material: " + matName);

  pMat = sh_makeShared<Material>();
  pMat->setName(matName.c_str());
  pMat->m_properties.properties.value = mah.properties;

  String baseColorPath;
  baseColorPath.resize(mah.baseColorMapPathSize);
  fread(baseColorPath.data(), sizeof(char), mah.baseColorMapPathSize, pFile);
  String fullBaseColorPath = filePath.directory() + "/" + baseColorPath;

  String normalPath;
  normalPath.resize(mah.normalMapPathSize);
  fread(normalPath.data(), sizeof(char), mah.normalMapPathSize, pFile);
  String fullNormalPath = filePath.directory() + "/" + normalPath;

  String metallicPath;
  metallicPath.resize(mah.metalnessMapPathSize);
  fread(metallicPath.data(), sizeof(char), mah.metalnessMapPathSize, pFile);
  String fullMetallicPath = filePath.directory() + "/" + metallicPath;

  String roughnessPath;
  roughnessPath.resize(mah.roughnessMapPathSize);
  fread(roughnessPath.data(), sizeof(char), mah.roughnessMapPathSize, pFile);
  String fullRoughnessPath = filePath.directory() + "/" + roughnessPath;

  String aoPath;
  aoPath.resize(mah.aoMapPathSize);
  fread(aoPath.data(), sizeof(char), mah.aoMapPathSize, pFile);
  String fullAOPath = filePath.directory() + "/" + aoPath;

  String emissivePath;
  emissivePath.resize(mah.emissiveMapPathSize);
  fread(emissivePath.data(), sizeof(char), mah.emissiveMapPathSize, pFile);
  String fullEmissivePath = filePath.directory() + "/" + emissivePath;

  String specularPath;
  specularPath.resize(mah.specularMapPathSize);
  fread(specularPath.data(), sizeof(char), mah.specularMapPathSize, pFile);
  String fullSpecularPath = filePath.directory() + "/" + specularPath;

  String opacityMaskPath;
  opacityMaskPath.resize(mah.opacityMaskMapPathSize);
  fread(opacityMaskPath.data(), sizeof(char), mah.opacityMaskMapPathSize, pFile);
  String fullOpacityMaskPath = filePath.directory() + "/" + opacityMaskPath;

  auto pBaseColor = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullBaseColorPath.c_str())));
  if (pBaseColor) {
    pMat->m_baseColor = pBaseColor;
    pMat->m_properties.properties.flags.bHasDiffuseMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasDiffuseMap = 0;
    pMat->m_baseColor = cast::re_ptr<ImageResource>(resMan.getResource("ErrorTexture"));
  }

  auto pNormal = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullNormalPath.c_str())));
  if (pNormal) {
    pMat->m_normal = pNormal;
    pMat->m_properties.properties.flags.bHasNormalMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasNormalMap = 0;
    pMat->m_normal = cast::re_ptr<ImageResource>(resMan.getResource("DefaultNormal"));
  }

  auto pMetalness = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullMetallicPath.c_str())));
  if (pMetalness) {
    pMat->m_metalness = pMetalness;
    pMat->m_properties.properties.flags.bHasMetalnessMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasMetalnessMap = 0;
    pMat->m_metalness = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  auto pRoughness = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullRoughnessPath.c_str())));
  if (pRoughness) {
    pMat->m_roughness = pRoughness;
    pMat->m_properties.properties.flags.bHasRoughnessMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasRoughnessMap = 0;
    pMat->m_roughness = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  auto pAO = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullAOPath.c_str())));
  if (pAO) {
    pMat->m_ao = pAO;
    pMat->m_properties.properties.flags.bHasAmbientOcclusionMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasRoughnessMap = 0;
    pMat->m_ao = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  auto pEmissive = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullEmissivePath.c_str())));
  if (pEmissive) {
    pMat->m_emissive = pEmissive;
    pMat->m_properties.properties.flags.bHasEmissiveMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasEmissiveMap = 0;
    pMat->m_emissive = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  auto pSpecular = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullSpecularPath.c_str())));
  if (pSpecular) {
    pMat->m_specular = pSpecular;
    pMat->m_properties.properties.flags.bHasSpecularMap = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasSpecularMap = 0;
    pMat->m_specular = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  auto pOpacityMask = cast::re_ptr<ImageResource>(
    resMan.loadResourceFromFile(Path(fullOpacityMaskPath.c_str())));
  if (pOpacityMask) {
    pMat->m_opacityMask = pOpacityMask;
    pMat->m_properties.properties.flags.bHasOpacityMask = 1;
  }
  else {
    pMat->m_properties.properties.flags.bHasOpacityMask = 0;
    pMat->m_opacityMask = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
  }

  pMat->baseColorFactor = mah.baseColorFactor;
  pMat->metallicRoughnessFactor = mah.metallicRoughnessFactor;
  pMat->opacityFactor = mah.opacityFactor;
  pMat->emissiveFactor = mah.emissiveFactor;
  pMat->emmisiveIntensity = mah.emmisiveIntensity;
  pMat->alphaCutoff = mah.alphaCutoff;

  if (!resMan.addResource(pMat)) {
    SH_LOG_ERROR("Failed to add material to resource manager: " + matName);
    pMat.reset();
    return nullptr;
  }
  SH_LOG_INFO("Material loaded: " + matName);

  return pMat;
}

bool
ResourceCodec::decodeStaticMesh(FILE* pFile, const Path& filePath) const
{
  ResourceManager& resMan = g_resourceMan();
  auto pStaticMesh = sh_makeShared<StaticMeshResource>();
  pStaticMesh->setName(filePath.filename());
  pStaticMesh->setPath(filePath);
  SH_LOG_INFO("Reading mesh data from file: " + filePath.string());

  StaticMeshAssetHeader meshAH = {};
  fread(&meshAH, sizeof(StaticMeshAssetHeader), 1, pFile);
  pStaticMesh->m_meshes.resize(meshAH.numMeshes);
  pStaticMesh->m_materials.resize(meshAH.numMaterials);

  if (pStaticMesh->m_meshes.empty()) {
    SH_LOG_WARNING("No meshes found in file: " + filePath.string());
  }
  if (pStaticMesh->m_materials.empty()) {
    SH_LOG_WARNING("No materials found in file: " + filePath.string());
  }

  for (auto& mesh : pStaticMesh->m_meshes) {
    MeshDataHeader mdh = {};
    fread(&mdh, sizeof(MeshDataHeader), 1, pFile);
    mesh.numVertices = mdh.numVertices;
    mesh.numIndices = mdh.numIndices;
    mesh.materialIndex = mdh.matIndex;

    mesh.name.resize(mdh.nameSize);
    fread(mesh.name.data(), sizeof(char), mdh.nameSize, pFile);

    mesh.vertices.resize(mesh.numVertices);
    fread(mesh.vertices.data(), sizeof(VertexData), mesh.numVertices, pFile);
    mesh.indices.resize(mesh.numIndices);
    fread(mesh.indices.data(), sizeof(uint32), mesh.numIndices, pFile);
  }

  SH_LOG_INFO("Mesh data read successfully. Reading materials...");
  for (auto& pMat : pStaticMesh->m_materials) {
    SPtr<Material> pNewMat = nullptr;
    getMaterialFromFile(pFile, pNewMat, filePath);
    pMat = pNewMat;
  }

  SH_LOG_INFO("Materials read successfully. Adding mesh to resource manager...");
  if (!resMan.addResource(pStaticMesh)) {
    SH_LOG_ERROR("Failed to add static mesh to resource manager: " + filePath.filename());
    return false;
  }

  SH_LOG_INFO("Static mesh decoded and loaded successfully: " + filePath.filename());
  fclose(pFile);
  return true;
}

bool
ResourceCodec::encodeStaticMesh(const String& objName, const Path& saveFilePath) const
{
  ResourceManager& resMan = g_resourceMan();
  auto pStaticMesh = cast::re_ptr<StaticMeshResource>(resMan.getResource(objName));

  FILE* outFile = nullptr;
#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&outFile, saveFilePath.c_str(), "wb");
#else
  outFile = fopen(saveFilePath.c_str(), "wb");
#endif

  if (!outFile) {
    return false;
  }

  SH_LOG_INFO("Encoding static mesh: " + objName + " to file: " + saveFilePath.string());

  RESOURCE_TYPE::E resType = RESOURCE_TYPE::kStaticMesh;
  StaticMeshAssetHeader header = {};
  header.numMeshes = static_cast<uint32>(pStaticMesh->m_meshes.size());
  header.numMaterials = static_cast<uint32>(pStaticMesh->m_materials.size());

  fwrite(&resType, sizeof(RESOURCE_TYPE::E), 1, outFile);
  fwrite(&header, sizeof(StaticMeshAssetHeader), 1, outFile);

  for (auto& mesh : pStaticMesh->m_meshes) {
    MeshDataHeader meshHeader = {};
    meshHeader.numVertices = mesh.numVertices;
    meshHeader.numIndices = mesh.numIndices;
    meshHeader.matIndex = mesh.materialIndex;
    meshHeader.nameSize = static_cast<uint32>(mesh.name.size() + 1);

    fwrite(&meshHeader, sizeof(MeshDataHeader), 1, outFile);
    fwrite(mesh.name.c_str(), sizeof(char), meshHeader.nameSize, outFile);
    fwrite(mesh.vertices.data(), sizeof(VertexData), mesh.numVertices, outFile);
    fwrite(mesh.indices.data(), sizeof(uint32), mesh.numIndices, outFile);
  }

  SH_LOG_INFO("Mesh data written successfully. Writing materials...");

  // Save materials
  for (auto& mat : pStaticMesh->m_materials) {
    auto currentMat = mat.lock();
    // Set header info
    MaterialAssetHeader matHeader = {};
    matHeader.nameSize = static_cast<uint32>(currentMat->getName().size() + 1);
    matHeader.properties = currentMat->m_properties.properties.value;
    matHeader.baseColorFactor = currentMat->baseColorFactor;
    matHeader.metallicRoughnessFactor = currentMat->metallicRoughnessFactor;
    matHeader.opacityFactor = currentMat->opacityFactor;
    matHeader.emissiveFactor = currentMat->emissiveFactor;
    matHeader.emmisiveIntensity = currentMat->emmisiveIntensity;
    matHeader.alphaCutoff = currentMat->alphaCutoff;

    // Get texture paths
    String baseColorPath;
    if (!currentMat->m_baseColor.expired()) {
      baseColorPath = currentMat->m_baseColor.lock()->getPath().filename();
    }
    String normalPath;
    if (!currentMat->m_normal.expired()) {
      normalPath = currentMat->m_normal.lock()->getPath().filename();
    }
    String metallicPath;
    if (!currentMat->m_metalness.expired()) {
      metallicPath = currentMat->m_metalness.lock()->getPath().filename();
    }
    String roughnessPath;
    if (!currentMat->m_roughness.expired()) {
      roughnessPath = currentMat->m_roughness.lock()->getPath().filename();
    }
    String aoPath;
    if (!currentMat->m_ao.expired()) {
      aoPath = currentMat->m_ao.lock()->getPath().filename();
    }
    String emissivePath;
    if (!currentMat->m_emissive.expired()) {
      emissivePath = currentMat->m_emissive.lock()->getPath().filename();
    }
    String specularPath;
    if (!currentMat->m_specular.expired()) {
      specularPath = currentMat->m_specular.lock()->getPath().filename();
    }
    String opacityMaskPath;
    if (!currentMat->m_opacityMask.expired()) {
      opacityMaskPath = currentMat->m_opacityMask.lock()->getPath().filename();
    }

    // Save path sizes
    matHeader.baseColorMapPathSize = static_cast<uint32>(baseColorPath.size() + 1);
    matHeader.normalMapPathSize = static_cast<uint32>(normalPath.size() + 1);
    matHeader.metalnessMapPathSize = static_cast<uint32>(metallicPath.size() + 1);
    matHeader.roughnessMapPathSize = static_cast<uint32>(roughnessPath.size() + 1);
    matHeader.aoMapPathSize = static_cast<uint32>(aoPath.size() + 1);
    matHeader.emissiveMapPathSize = static_cast<uint32>(emissivePath.size() + 1);
    matHeader.specularMapPathSize = static_cast<uint32>(specularPath.size() + 1);
    matHeader.opacityMaskMapPathSize = static_cast<uint32>(opacityMaskPath.size() + 1);

    // Wirte material data to file
    fwrite(&matHeader, sizeof(MaterialAssetHeader), 1, outFile);
    fwrite(currentMat->getName().c_str(), sizeof(char), matHeader.nameSize, outFile);
    fwrite(baseColorPath.c_str(), sizeof(char), matHeader.baseColorMapPathSize, outFile);
    fwrite(normalPath.c_str(), sizeof(char), matHeader.normalMapPathSize, outFile);
    fwrite(metallicPath.c_str(), sizeof(char), matHeader.metalnessMapPathSize, outFile);
    fwrite(roughnessPath.c_str(), sizeof(char), matHeader.roughnessMapPathSize, outFile);
    fwrite(aoPath.c_str(), sizeof(char), matHeader.aoMapPathSize, outFile);
    fwrite(emissivePath.c_str(), sizeof(char), matHeader.emissiveMapPathSize, outFile);
    fwrite(specularPath.c_str(), sizeof(char), matHeader.specularMapPathSize, outFile);
    fwrite(opacityMaskPath.c_str(), sizeof(char), matHeader.opacityMaskMapPathSize, outFile);
  }

  SH_LOG_INFO("Materials written successfully. Static mesh encoded successfully: " +
              saveFilePath.string());
  fclose(outFile);
  return true;
}
}
