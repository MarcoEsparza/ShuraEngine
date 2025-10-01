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
Asset::saveResourceToAsset(const SPtr<Resource>& pRes)
{
  RESOURCE_TYPE::E resType = pRes->getType();
  if (resType == RESOURCE_TYPE::kStaticMesh) {
    saveStaticMesh(pRes);
    return true;
  }

  return false;
}

SPtr<Resource>
Asset::loadResourceFromAsset(Path filePath)
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  FILE* file = nullptr;
  fopen_s(&file, filePath.toString().c_str(), "rb");
  
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

      if(mat){
        continue;
      }

      mat = sh_makeShared<Material>();
      mat->setName(matName.c_str());
      mat->m_properties.properties.value = mah.properties;
      mat->baseColorPath.resize(mah.baseColorMapPathSize);
      fread(mat->baseColorPath.data(), sizeof(char), mah.baseColorMapPathSize, file);
      mat->normalPath.resize(mah.normalMapPathSize);
      fread(mat->normalPath.data(), sizeof(char), mah.normalMapPathSize, file);
      mat->metallicPath.resize(mah.metalnessMapPathSize);
      fread(mat->metallicPath.data(), sizeof(char), mah.metalnessMapPathSize, file);
      mat->roughnessPath.resize(mah.roughnessMapPathSize);
      fread(mat->roughnessPath.data(), sizeof(char), mah.roughnessMapPathSize, file);
      mat->aoPath.resize(mah.ambientOcclusionMapPathSize);
      fread(mat->aoPath.data(), sizeof(char), mah.ambientOcclusionMapPathSize, file);
     
      auto pBaseColor = sh_reinterpretPCast<ImageResource>(
                        resMan.loadResourceFromFile(Path(mat->baseColorPath.c_str())));
      if (pBaseColor) {
        mat->baseColor = pBaseColor->texture;
        mat->m_properties.properties.flags.bHasDiffuseMap = 1;
      }

      auto pNormal = sh_reinterpretPCast<ImageResource>(
                      resMan.loadResourceFromFile(Path(mat->normalPath.c_str())));
      if (pNormal) {
        mat->normal = pNormal->texture;
        mat->m_properties.properties.flags.bHasNormalMap = 1;
      }

      auto pMetallic = sh_reinterpretPCast<ImageResource>(
                        resMan.loadResourceFromFile(Path(mat->metallicPath.c_str())));
      if (pMetallic) {
        mat->metallic = pMetallic->texture;
        mat->m_properties.properties.flags.bHasMetalnessMap = 1;
      }

      auto pRoughness = sh_reinterpretPCast<ImageResource>(
                         resMan.loadResourceFromFile(Path(mat->roughnessPath.c_str())));
      if (pRoughness) {
        mat->roughness = pRoughness->texture;
        mat->m_properties.properties.flags.bHasRoughnessMap = 1;
      }

      auto pAO = sh_reinterpretPCast<ImageResource>(
                  resMan.loadResourceFromFile(Path(mat->aoPath.c_str())));
      if (pAO) {
        mat->ao = pAO->texture;
        mat->m_properties.properties.flags.bHasAmbientOcclusionMap = 1;
      }

      resMan.loadMaterial(mat);
    }
    fclose(file);
    return pStaticMesh;
  }
  fclose(file);
  return nullptr;
}

void
Asset::saveStaticMesh(const SPtr<Resource>& pRes)
{
  auto pStaticMesh = cast::re_ptr<StaticMeshResource>(pRes);

  SystemPath pathName = pStaticMesh->getName();
  pathName.replace_extension(".sha");
  String fileName = "resources/assets/models/" + pathName.string();
  FILE* outFile = nullptr;
  fopen_s(&outFile, fileName.c_str(), "wb");
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

  for (auto& mat : pStaticMesh->m_materials) {
    MaterialAssetHeader matHeader = {};
    matHeader.nameSize = static_cast<uint32>(mat->getName().size() + 1);
    matHeader.properties = mat->m_properties.properties.value;
    matHeader.baseColorMapPathSize = static_cast<uint32>(mat->baseColorPath.size() + 1);
    matHeader.normalMapPathSize = static_cast<uint32>(mat->normalPath.size() + 1);
    matHeader.metalnessMapPathSize = static_cast<uint32>(mat->metallicPath.size() + 1);
    matHeader.roughnessMapPathSize = static_cast<uint32>(mat->roughnessPath.size() + 1);
    matHeader.ambientOcclusionMapPathSize =
      static_cast<uint32>(mat->aoPath.size() + 1);

    fwrite(&matHeader, sizeof(MaterialAssetHeader), 1, outFile);
    fwrite(mat->getName().c_str(), sizeof(char), matHeader.nameSize, outFile);
    fwrite(mat->baseColorPath.c_str(), sizeof(char), matHeader.baseColorMapPathSize, outFile);
    fwrite(mat->normalPath.c_str(), sizeof(char), matHeader.normalMapPathSize, outFile);
    fwrite(mat->metallicPath.c_str(), sizeof(char), matHeader.metalnessMapPathSize, outFile);
    fwrite(mat->roughnessPath.c_str(), sizeof(char), matHeader.roughnessMapPathSize, outFile);
    fwrite(mat->aoPath.c_str(), sizeof(char), matHeader.ambientOcclusionMapPathSize, outFile);
  }

  fclose(outFile);
}

SPtr<Resource>
Asset::loadStaticMesh(sh_fstream& file)
{
  ResourceManager& resMan = g_resourceMan();
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

    file.read(reinterpret_cast<char*>(&mat->m_properties), sizeof(MaterialProperties));
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
    mat->ao = pAO->texture;
  }

  file.close();
  return pStaticMesh;
}
}
