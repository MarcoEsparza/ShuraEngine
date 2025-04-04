/*************************************************************/
/*
*  @file    shAsset.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/25
*  @brief   Asset class for cache generation.
*
*  Asset class for cache generation.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shResource.h"
#include "shPath.h"
#include "shMaterial.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT ResourceInfoHeader
{
  RESOURCE_TYPE::E type;
  String name;
};

struct SH_CORE_EXPORT StaticMeshAssetHeader
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 matIndex = 0;
};

struct SH_CORE_EXPORT SkeletalMeshAssetHeader
{
  uint32 numMat = 0;
};

struct SH_CORE_EXPORT SMUnionAssetHeader
{
  uint32 numMeshes = 0;
  uint32 numMat = 0;
};

struct SH_CORE_EXPORT MaterialAssetHeader
{
  MATERIAL_TYPE::E type;
  MaterialProperties properties;
};

struct SH_CORE_EXPORT PBRMaterialAsset
{
  bool bHasAlbedo;
  bool bHasNormal;
  bool bHasMetallic;
  bool bHasRoughness;
  bool bHasAO;
};

class SH_CORE_EXPORT Asset
{
 public:
  Asset() = default;
  ~Asset() = default;

  bool
  saveResourceToAsset(const SPtr<Resource>& pRes);

  SPtr<Resource>
  loadResourceFromAsset(Path filePath);

 private:
  void
  saveSMUnionAsset(const SPtr<Resource>& pRes);

  /*void
  loadSMUnionAsset(const )*/
};
}
