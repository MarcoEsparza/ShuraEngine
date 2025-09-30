/*************************************************************/
/*
*  @file    shAsset.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/28
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
  sh_fstream file(filePath.toString(), ios::in | ios::binary);

  if (!file.is_open()) {
    return nullptr;
  }

  //auto pRes = sh_makeShared<Resource>();
  RESOURCE_TYPE::E resType = RESOURCE_TYPE::kCount;
  file.read(reinterpret_cast<char*>(&resType), sizeof(RESOURCE_TYPE::E));

  if (resType == RESOURCE_TYPE::kStaticMesh) {
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

      mesh.name.resize(mdh.nameSize);
      file.read(mesh.name.data(), mdh.nameSize);

      uint32 verticesSize = mesh.numVertices * sizeof(VertexData);
      uint32 indicesSize = mesh.numIndices * sizeof(uint32);
      Vector<char> verticesData;
      Vector<char> indicesData;
      verticesData.resize(verticesSize);
      indicesData.resize(indicesSize);
      file.read(reinterpret_cast<char*>(verticesData.data()), verticesSize);
      file.read(reinterpret_cast<char*>(indicesData.data()), indicesSize);

      mesh.vertices.resize(mesh.numVertices);
      mesh.indices.resize(mesh.numIndices);
      memcpy(mesh.vertices.data(), verticesData.data(), verticesSize);
      memcpy(mesh.indices.data(), indicesData.data(), indicesSize);
    }

    for (auto& mat : pStaticMesh->m_materials) {
      MaterialAssetHeader mah = {};
      file.read(cast::re<char*>(&mah), sizeof(MaterialAssetHeader));
      //mat = sh_makeShared<Material>();

      String matName;
      matName.resize(mah.nameSize);
      file.read(matName.data(), mah.nameSize);

      mat = cast::re_ptr<Material>(resMan.getResource(matName));

      if(mat){
        continue;
      }

      mat = sh_makeShared<Material>();
      mat->setName(matName.c_str());
      mat->m_properties.properties.value = mah.properties;
      mat->baseColorPath.resize(mah.baseColorMapPathSize);
      file.read(mat->baseColorPath.data(), mah.baseColorMapPathSize);
      mat->normalPath.resize(mah.normalMapPathSize);
      file.read(mat->normalPath.data(), mah.normalMapPathSize);
      mat->metallicPath.resize(mah.metalnessMapPathSize);
      file.read(mat->metallicPath.data(), mah.metalnessMapPathSize);
      mat->roughnessPath.resize(mah.roughnessMapPathSize);
      file.read(mat->roughnessPath.data(), mah.roughnessMapPathSize);
      mat->aoPath.resize(mah.ambientOcclusionMapPathSize);
      file.read(mat->aoPath.data(), mah.ambientOcclusionMapPathSize);

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

    file.close();
    return pStaticMesh;
  }

  file.close();
  return nullptr;
}

void
Asset::saveStaticMesh(const SPtr<Resource>& pRes)
{
  auto pStaticMesh = sh_reinterpretPCast<StaticMeshResource>(pRes);
  RESOURCE_TYPE::E resType = RESOURCE_TYPE::kStaticMesh;

  StaticMeshAssetHeader meshAH = {};
  meshAH.numMeshes = static_cast<uint32>(pStaticMesh->m_meshes.size());
  meshAH.numMaterials = static_cast<uint32>(pStaticMesh->m_materials.size());

  SystemPath pathName = pStaticMesh->getName();
  pathName.replace_extension(".sha");
  String fileName = "resources/assets/models/" + pathName.string();
  sh_fstream outFile(fileName, ios::out | ios::binary);

  outFile.write(reinterpret_cast<char*>(&resType), sizeof(RESOURCE_TYPE::E));
  outFile.write(reinterpret_cast<char*>(&meshAH), sizeof(StaticMeshAssetHeader));

  for (auto& mesh : pStaticMesh->m_meshes) {
    MeshDataHeader meshHeader = {};
    meshHeader.numVertices = mesh.numVertices;
    meshHeader.numIndices = mesh.numIndices;
    meshHeader.matIndex = mesh.materialIndex;
    meshHeader.nameSize = static_cast<uint32>(mesh.name.size() + 1);

    outFile.write(reinterpret_cast<const char*>(&meshHeader), sizeof(MeshDataHeader));
    outFile.write(mesh.name.c_str(), meshHeader.nameSize);
    outFile.write(reinterpret_cast<const char*>(mesh.vertices.data()),
      mesh.numVertices * sizeof(VertexData));
    outFile.write(reinterpret_cast<const char*>(mesh.indices.data()),
      mesh.numIndices * sizeof(uint32));
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

    outFile.write(reinterpret_cast<const char*>(&matHeader), sizeof(MaterialAssetHeader));
    outFile.write(mat->getName().c_str(), matHeader.nameSize);
    outFile.write(mat->baseColorPath.c_str(), matHeader.baseColorMapPathSize);
    outFile.write(mat->normalPath.c_str(), matHeader.normalMapPathSize);
    outFile.write(mat->metallicPath.c_str(), matHeader.metalnessMapPathSize);
    outFile.write(mat->roughnessPath.c_str(), matHeader.roughnessMapPathSize);
    outFile.write(mat->aoPath.c_str(), matHeader.ambientOcclusionMapPathSize);
  }

  outFile.close();
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
