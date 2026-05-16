/*************************************************************/
/*
*  @file    shAsset.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/30
*  @brief   Asset class for cache generation.
*
*  Asset class for cache generation.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shAsset.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shImageResource.h"
#include "shException.h"
#include <shTimer.h>

using std::getline;

namespace shEngineSDK {
bool
Asset::saveResourceToAsset(const SPtr<Resource>& pRes, const String& path)
{
  RESOURCE_TYPE::E resType = pRes->getType();
  if (resType == RESOURCE_TYPE::kStaticMesh) {
    saveStaticMesh(pRes, path);
    return true;
  }

  return false;
}

SPtr<Resource>
Asset::loadResourceFromAsset(Path filePath)
{
  //GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  FILE* file = nullptr;

#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&file, filePath.string().c_str(), "rb");
#else
  file = fopen(filePath.toString().c_str(), "rb");
#endif
  
  if (!file) {
    return nullptr;
  }
  setvbuf(file, NULL, _IONBF, 0); // No buffering

  RESOURCE_TYPE::E resType = RESOURCE_TYPE::kCount;
  fread(&resType, sizeof(RESOURCE_TYPE::E), 1, file);

  if (resType == RESOURCE_TYPE::kStaticMesh) {
    auto pStaticMesh = sh_makeShared<StaticMeshResource>();

    StaticMeshAssetHeader meshAH = {};
    fread(&meshAH, sizeof(StaticMeshAssetHeader), 1, file);
    pStaticMesh->m_meshes.resize(meshAH.numMeshes);
    pStaticMesh->m_materials.resize(meshAH.numMaterials);

    for (auto& mesh : pStaticMesh->m_meshes) {
      MeshDataHeader mdh = {};
      fread(&mdh, sizeof(MeshDataHeader), 1, file);
      mesh.numVertices = mdh.numVertices;
      mesh.numIndices = mdh.numIndices;
      mesh.materialIndex = mdh.matIndex;

      mesh.name.resize(mdh.nameSize);
      fread(mesh.name.data(), sizeof(char), mdh.nameSize, file);

      mesh.vertices.resize(mesh.numVertices);
      fread(mesh.vertices.data(), sizeof(VertexData), mesh.numVertices, file);
      mesh.indices.resize(mesh.numIndices);
      fread(mesh.indices.data(), sizeof(uint32), mesh.numIndices, file);
    }

    for (auto& mat : pStaticMesh->m_materials) {
      MaterialAssetHeader mah = {};
      fread(&mah, sizeof(MaterialAssetHeader), 1, file);

      String matName;
      matName.resize(mah.nameSize);
      fread(matName.data(), sizeof(char), mah.nameSize, file);

      mat = cast::re_ptr<Material>(resMan.getResource(matName));

      if(!mat.expired()){
        continue;
      }

      auto newMat = sh_makeShared<Material>();
      newMat->setName(matName.c_str());
      newMat->m_properties.properties.value = mah.properties;

      String baseColorPath;
      baseColorPath.resize(mah.baseColorMapPathSize);
      fread(baseColorPath.data(), sizeof(char), mah.baseColorMapPathSize, file);

      String normalPath;
      normalPath.resize(mah.normalMapPathSize);
      fread(normalPath.data(), sizeof(char), mah.normalMapPathSize, file);

      String metallicPath;
      metallicPath.resize(mah.metalnessMapPathSize);
      fread(metallicPath.data(), sizeof(char), mah.metalnessMapPathSize, file);

      String roughnessPath;
      roughnessPath.resize(mah.roughnessMapPathSize);
      fread(roughnessPath.data(), sizeof(char), mah.roughnessMapPathSize, file);

      String aoPath;
      aoPath.resize(mah.aoMapPathSize);
      fread(aoPath.data(), sizeof(char), mah.aoMapPathSize, file);
     
      String emissivePath;
      emissivePath.resize(mah.emissiveMapPathSize);
      fread(emissivePath.data(), sizeof(char), mah.emissiveMapPathSize, file);

      String specularPath;
      specularPath.resize(mah.specularMapPathSize);
      fread(specularPath.data(), sizeof(char), mah.specularMapPathSize, file);

      String opacityMaskPath;
      opacityMaskPath.resize(mah.opacityMaskMapPathSize);
      fread(opacityMaskPath.data(), sizeof(char), mah.opacityMaskMapPathSize, file);

      auto pBaseColor = cast::re_ptr<ImageResource>(
                        resMan.loadResourceFromFile(Path(baseColorPath.c_str())));
      if (pBaseColor) {
        newMat->m_baseColor = pBaseColor;
        newMat->m_properties.properties.flags.bHasDiffuseMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasDiffuseMap = 0;
        newMat->m_baseColor = cast::re_ptr<ImageResource>(resMan.getResource("ErrorTexture"));
      }

      auto pNormal = cast::re_ptr<ImageResource>(
                      resMan.loadResourceFromFile(Path(normalPath.c_str())));
      if (pNormal) {
        newMat->m_normal = pNormal;
        newMat->m_properties.properties.flags.bHasNormalMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasNormalMap = 0;
        newMat->m_normal = cast::re_ptr<ImageResource>(resMan.getResource("DefaultNormal"));
      }

      auto pMetalness = cast::re_ptr<ImageResource>(
                        resMan.loadResourceFromFile(Path(metallicPath.c_str())));
      if (pMetalness) {
        newMat->m_metalness = pMetalness;
        newMat->m_properties.properties.flags.bHasMetalnessMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasMetalnessMap = 0;
        newMat->m_metalness = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      auto pRoughness = cast::re_ptr<ImageResource>(
                         resMan.loadResourceFromFile(Path(roughnessPath.c_str())));
      if (pRoughness) {
        newMat->m_roughness = pRoughness;
        newMat->m_properties.properties.flags.bHasRoughnessMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasRoughnessMap = 0;
        newMat->m_roughness = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      auto pAO = cast::re_ptr<ImageResource>(
                  resMan.loadResourceFromFile(Path(aoPath.c_str())));
      if (pAO) {
        newMat->m_ao = pAO;
        newMat->m_properties.properties.flags.bHasAmbientOcclusionMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasRoughnessMap = 0;
        newMat->m_ao = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      auto pEmissive = cast::re_ptr<ImageResource>(
                        resMan.loadResourceFromFile(Path(emissivePath.c_str())));
      if (pEmissive) {
        newMat->m_emissive = pEmissive;
        newMat->m_properties.properties.flags.bHasEmissiveMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasEmissiveMap = 0;
        newMat->m_emissive = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      auto pSpecular = cast::re_ptr<ImageResource>(
                        resMan.loadResourceFromFile(Path(specularPath.c_str())));
      if (pSpecular) {
        newMat->m_specular = pSpecular;
        newMat->m_properties.properties.flags.bHasSpecularMap = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasSpecularMap = 0;
        newMat->m_specular = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      auto pOpacityMask = cast::re_ptr<ImageResource>(
                            resMan.loadResourceFromFile(Path(opacityMaskPath.c_str())));
      if (pOpacityMask) {
        newMat->m_opacityMask = pOpacityMask;
        newMat->m_properties.properties.flags.bHasOpacityMask = 1;
      }
      else {
        newMat->m_properties.properties.flags.bHasOpacityMask = 0;
        newMat->m_opacityMask = cast::re_ptr<ImageResource>(resMan.getResource("BlackTexture"));
      }

      resMan.addResource(newMat);
      mat = newMat;
    }
    fclose(file);
    return pStaticMesh;
  }
  fclose(file);
  return nullptr;
}

void
Asset::saveStaticMesh(const SPtr<Resource>& pRes, const String& path)
{
  auto pStaticMesh = cast::re_ptr<StaticMeshResource>(pRes);

  FILE* outFile = nullptr;
#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&outFile, path.c_str(), "wb");
#else
  outFile = fopen(path.c_str(), "wb");
#endif

  if (!outFile) {
    return;
  }

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

  // Save materials
  for (auto& mat : pStaticMesh->m_materials) {
    auto currentMat = mat.lock();
    // Set header info
    MaterialAssetHeader matHeader = {};
    matHeader.nameSize = static_cast<uint32>(currentMat->getName().size() + 1);
    matHeader.properties = currentMat->m_properties.properties.value;

    // Get texture paths
    String baseColorPath;
    if (!currentMat->m_baseColor.expired()) {
      baseColorPath = currentMat->m_baseColor.lock()->getPath().string();
    }
    String normalPath;
    if (!currentMat->m_normal.expired()) {
      normalPath = currentMat->m_normal.lock()->getPath().string();
    }
    String metallicPath;
    if (!currentMat->m_metalness.expired()) {
      metallicPath = currentMat->m_metalness.lock()->getPath().string();
    }
    String roughnessPath;
    if (!currentMat->m_roughness.expired()) {
      roughnessPath = currentMat->m_roughness.lock()->getPath().string();
    }
    String aoPath;
    if (!currentMat->m_ao.expired()) {
      aoPath = currentMat->m_ao.lock()->getPath().string();
    }
    String emissivePath;
    if (!currentMat->m_emissive.expired()) {
      emissivePath = currentMat->m_emissive.lock()->getPath().string();
    }
    String specularPath;
    if (!currentMat->m_specular.expired()) {
      specularPath = currentMat->m_specular.lock()->getPath().string();
    }
    String opacityMaskPath;
    if (!currentMat->m_opacityMask.expired()) {
      opacityMaskPath = currentMat->m_opacityMask.lock()->getPath().string();
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

  fclose(outFile);
}

SPtr<Resource>
Asset::loadStaticMesh(sh_fstream& file)
{
  //ResourceManager& resMan = g_resourceMan();
  auto pStaticMesh = sh_makeShared<StaticMeshResource>();

  StaticMeshAssetHeader meshAH = {};
  file.read(reinterpret_cast<char*>(&meshAH), sizeof(StaticMeshAssetHeader));
  pStaticMesh->m_meshes.resize(meshAH.numMeshes);
  pStaticMesh->m_materials.resize(meshAH.numMaterials);

  for (auto& mesh : pStaticMesh->m_meshes) {
    MeshDataHeader mdh = {};
    file.read(reinterpret_cast<char*>(&mdh), sizeof(MeshDataHeader));
    mesh.numVertices = mdh.numVertices;
    mesh.numIndices = mdh.numIndices;
    mesh.materialIndex = mdh.matIndex;

    file.read(mesh.name.data(), mdh.nameSize);

    uint32 verticesSize = mesh.numVertices * sizeof(VertexData);
    uint32 indicesSize = mesh.numIndices * sizeof(uint32);
    Vector<char> verticesData;
    Vector<char> indicesData;
    verticesData.resize(verticesSize);
    indicesData.resize(indicesSize);
    mesh.vertices.resize(mesh.numVertices);
    mesh.indices.resize(mesh.numIndices);

    file.read(reinterpret_cast<char*>(verticesData.data()), verticesSize);
    file.read(reinterpret_cast<char*>(indicesData.data()), indicesSize);
    memcpy(mesh.vertices.data(), verticesData.data(), verticesSize);
    memcpy(mesh.indices.data(), indicesData.data(), indicesSize);
  }

  for (auto& mat : pStaticMesh->m_materials) {
    mat = sh_makeShared<Material>();

    /*file.read(reinterpret_cast<char*>(&mat->m_properties), sizeof(MaterialProperties));
    String line;
    getline(file, line);
    getline(file, line);
    mat->setName(line);
    getline(file, line);
    mat->baseColorPath = line;
    getline(file, line);
    mat->metallicPath = line;
    getline(file, line);
    mat->roughnessPath = line;
    getline(file, line);
    mat->normalPath = line;
    getline(file, line);
    mat->aoPath = line;

    auto pBaseColor = sh_reinterpretPCast<ImageResource>(
                      resMan.loadResourceFromFile(Path(mat->baseColorPath)));
    auto pMetallic = sh_reinterpretPCast<ImageResource>(
                     resMan.loadResourceFromFile(Path(mat->metallicPath)));
    auto pRoughness = sh_reinterpretPCast<ImageResource>(
                      resMan.loadResourceFromFile(Path(mat->roughnessPath)));
    auto pNormal = sh_reinterpretPCast<ImageResource>(
                   resMan.loadResourceFromFile(Path(mat->normalPath)));
    auto pAO = sh_reinterpretPCast<ImageResource>(
               resMan.loadResourceFromFile(Path(mat->aoPath)));

    mat->baseColor = pBaseColor->texture;
    mat->metallic = pMetallic->texture;
    mat->roughness = pRoughness->texture;
    mat->normal = pNormal->texture;
    mat->ao = pAO->texture;*/
  }

  file.close();
  return pStaticMesh;
}
}
