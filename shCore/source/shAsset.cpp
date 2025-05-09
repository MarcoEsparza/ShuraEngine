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

void
Asset::loadResourceFromAsset(Path filePath)
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();
  sh_fstream file(filePath.toString(), ios::in | ios::binary);

  if (!file.is_open()) {
    return;
  }

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
    
      String line;
      getline(file, line);
      getline(file, line);
      mesh.name = line;
    }

    for (auto& mat : pStaticMesh->m_materials) {
      mat = sh_makeShared<Material>();
    
      file.read(reinterpret_cast<char*>(&mat->m_properties), sizeof(MaterialProperties));
      String line;
      getline(file, line);
      getline(file, line);
      mat->name = line;
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
    
      auto pBaseColor = sh_makeShared<ImageResource>();

      if (mat->baseColorPath != "") {
        pBaseColor = sh_reinterpretPCast<ImageResource>(
                     resMan.loadResourceFromFile(Path(mat->baseColorPath)));
      }
      else {
        pBaseColor->texture = graphMan.createErrorTexturre();
      }

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

    m_res = pStaticMesh;
  }

  file.close();
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
  sh_fstream file(fileName, ios::out | ios::binary);

  file.write(reinterpret_cast<char*>(&resType), sizeof(RESOURCE_TYPE::E));
  file.write(reinterpret_cast<char*>(&meshAH), sizeof(StaticMeshAssetHeader));

  for (auto& mesh : pStaticMesh->m_meshes) {
    MeshDataHeader mdh = {};
    mdh.numVertices = mesh.numVertices;
    mdh.numIndices = mesh.numIndices;
    mdh.matIndex = mesh.materialIndex;
    file.write(reinterpret_cast<char*>(&mdh), sizeof(MeshDataHeader));

    Vector<char> verticesData;
    Vector<char> indicesData;
    uint32 verticesSize = static_cast<uint32>(mesh.vertices.size() * sizeof(VertexData));
    uint32 indicesSize = static_cast<uint32>(mesh.indices.size() * sizeof(uint32));
    verticesData.resize(verticesSize);
    indicesData.resize(indicesSize);
    memcpy(verticesData.data(), mesh.vertices.data(), verticesSize);
    memcpy(indicesData.data(), mesh.indices.data(), indicesSize);

    file.write(verticesData.data(), verticesSize);
    file.write(indicesData.data(), indicesSize);
    file << "\n" << mesh.name << "\n";
  }

  for (auto& mat : pStaticMesh->m_materials) {
    file.write(reinterpret_cast<char*>(&mat->m_properties), sizeof(MaterialProperties));
    file << "\n" << mat->name << "\n";
    file << mat->baseColorPath << "\n";
    file << mat->metallicPath << "\n";
    file << mat->roughnessPath << "\n";
    file << mat->normalPath << "\n";
    file << mat->aoPath << "\n";
  }

  file.close();
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

    String line;
    getline(file, line);
    getline(file, line);
    mesh.name = line;
  }

  for (auto& mat : pStaticMesh->m_materials) {
    mat = sh_makeShared<Material>();

    file.read(reinterpret_cast<char*>(&mat->m_properties), sizeof(MaterialProperties));
    String line;
    getline(file, line);
    getline(file, line);
    mat->name = line;
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
