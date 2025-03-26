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

//#include <fstream>
using std::reinterpret_pointer_cast;

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
}

bool
Asset::loadResourceFromAsset(Path filePath)
{
  return false;
}
}
