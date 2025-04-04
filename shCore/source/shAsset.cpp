/*************************************************************/
/*
*  @file    shAsset.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/25
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
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shImageResource.h"

//#include <fstream>
using std::reinterpret_pointer_cast;
using std::getline;

namespace shEngineSDK {
bool
Asset::saveResourceToAsset(const SPtr<Resource>& pRes)
{
  RESOURCE_TYPE::E resType = pRes->getType();

  if (resType == RESOURCE_TYPE::kMeshUnion) {
    saveSMUnionAsset(pRes);
  }

  return false;
}

void
Asset::saveSMUnionAsset(const SPtr<Resource>& pRes)
{
  auto pSMURes = reinterpret_pointer_cast<StaticMeshUnionResource>(pRes);

  ResourceInfoHeader resIH = {};
  resIH.type = RESOURCE_TYPE::kMeshUnion;
  resIH.name = pSMURes->getName();

  SMUnionAssetHeader smuAH = {};
  smuAH.numMeshes = pSMURes->meshes.size();
  smuAH.numMat = pSMURes->materials.size();

  Path pathName(pSMURes->getName());
  pathName.replaceExtension(".sha");
  String fileName = "resources/assets/models/" + pathName.toString();

  sh_fstream file(fileName, ios::out | ios::binary);

  file.write(reinterpret_cast<char*>(&resIH), sizeof(ResourceInfoHeader));
  file.write(reinterpret_cast<char*>(&smuAH), sizeof(SMUnionAssetHeader));

  for (uint32 i = 0; i < smuAH.numMeshes; ++i) {
    StaticMeshAssetHeader smaH = {};
    smaH.numVertices = pSMURes->meshes[i]->numVertex;
    smaH.numIndices = pSMURes->meshes[i]->numIndex;
    smaH.matIndex = pSMURes->meshes[i]->matIndex;

    Vector<char> vertexInfo;
    uint32 verticesSize = sizeof(VertexData) * pSMURes->meshes[i]->vertices.size();
    memcpy(vertexInfo.data(), pSMURes->meshes[i]->vertices.data(), verticesSize);

    Vector<char> indexInfo;
    uint32 indicesSize = sizeof(VertexData) * pSMURes->meshes[i]->indices.size();
    memcpy(indexInfo.data(), pSMURes->meshes[i]->indices.data(), indicesSize);

    file.write(reinterpret_cast<char*>(&smaH), sizeof(StaticMeshAssetHeader));
    file.write(vertexInfo.data(), vertexInfo.size());
    file.write(indexInfo.data(), indexInfo.size());
  }

  for (uint32 i = 0; i < smuAH.numMat; ++i) {
    MaterialAssetHeader mah = {};
    mah.type = pSMURes->materials[i]->m_type;
    mah.properties = pSMURes->materials[i]->m_properties;

    String baseColorPath = pSMURes->materials[i]->baseColorPath;
    String normalPath = pSMURes->materials[i]->normalPath;
    String metallicPath = pSMURes->materials[i]->metallicPath;
    String roughnessPath = pSMURes->materials[i]->roughnessPath;
    String aoPath = pSMURes->materials[i]->aoPath;

    file.write(reinterpret_cast<char*>(&mah), sizeof(MaterialAssetHeader));
    file << "\n";
    file << baseColorPath << "\n";
    file << normalPath << "\n";
    file << metallicPath << "\n";
    file << roughnessPath << "\n";
    file << aoPath << "\n";
  }

  file.close();
}

SPtr<Resource>
Asset::loadResourceFromAsset(Path filePath)
{
  sh_fstream file(filePath.toString(), ios::in | ios::binary);

  if (!file.is_open()) {
    return nullptr;
  }

  ResourceInfoHeader rih = {};
  file.read(reinterpret_cast<char*>(&rih), sizeof(ResourceInfoHeader));

  if (rih.type == RESOURCE_TYPE::kMeshUnion) {
    auto pSMURes = make_shared<StaticMeshUnionResource>();

    SMUnionAssetHeader smuAH = {};
    file.read(reinterpret_cast<char*>(&smuAH), sizeof(SMUnionAssetHeader));
    
    for (uint32 i = 0; i < smuAH.numMeshes; ++i) {
      StaticMeshAssetHeader smaH = {};
      file.read(reinterpret_cast<char*>(&smaH), sizeof(StaticMeshAssetHeader));

      auto pSMesh = make_shared<StaticMeshResource>();
      pSMesh->numVertex = smaH.numVertices;
      pSMesh->numIndex = smaH.numIndices;
      pSMesh->matIndex = smaH.matIndex;

      pSMesh->vertices.resize(smaH.numVertices);
      uint32 vertInfoSize = smaH.numVertices * sizeof(VertexData);
      Vector<char> verticesInfo;
      verticesInfo.resize(vertInfoSize);
      file.read(verticesInfo.data(), vertInfoSize);
      memcpy(pSMesh->vertices.data(), verticesInfo.data(), vertInfoSize);

      pSMesh->indices.resize(smaH.numIndices);
      uint32 indInfoSize = smaH.numIndices * sizeof(uint32);
      Vector<char> indicesInfo;
      indicesInfo.resize(indInfoSize);
      file.read(indicesInfo.data(), indInfoSize);
      memcpy(pSMesh->indices.data(), indicesInfo.data(), indInfoSize);

      pSMURes->meshes.push_back(pSMesh);
    }

    for (uint32 i = 0; i < smuAH.numMat; ++i) {
      MaterialAssetHeader mah = {};
      file.read(reinterpret_cast<char*>(&mah), sizeof(MaterialAssetHeader));

      auto pMat = make_shared<Material>();
      pMat->m_type = mah.type;
      pMat->m_properties = mah.properties;
      
      Vector<String> texPaths;
      String line;
      while (getline(file, line)) {
        texPaths.push_back(line);
      }

      pMat->baseColorPath = texPaths[0];
      pMat->normalPath = texPaths[1];
      pMat->metallicPath = texPaths[2];
      pMat->roughnessPath = texPaths[3];
      pMat->aoPath = texPaths[4];

      pSMURes->materials.push_back(pMat);
      //auto pBCImage = reinterpret_pointer_cast<ImageResource>()
    }

    return pSMURes;
  }

  return nullptr;
}
}
