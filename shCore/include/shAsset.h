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
  RESOURCE_TYPE::E type = RESOURCE_TYPE::kCount;
  String name;
};

struct SH_CORE_EXPORT MeshDataHeader
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 matIndex = 0;
  uint32 nameSize = 0;
};

struct SH_CORE_EXPORT StaticMeshAssetHeader
{
  uint32 numMeshes = 0;
  uint32 numMaterials = 0;
};

struct SH_CORE_EXPORT SkeletalMeshAssetHeader
{
  uint32 numMeshes = 0;
  uint32 numMaterials = 0;
  uint32 numVertices = 0;
  uint32 numIndices = 0;
};

struct SH_CORE_EXPORT MaterialAssetHeader {
  uint32 nameSize = 0;
  uint32 properties = 0;
  uint32 baseColorMapPathSize = 0;
  uint32 normalMapPathSize = 0;
  uint32 metalnessMapPathSize = 0;
  uint32 roughnessMapPathSize = 0;
  uint32 ambientOcclusionMapPathSize = 0;
};

class SH_CORE_EXPORT Asset
{
 public:
  Asset() = default;
  ~Asset() = default;

  static bool
  saveResourceToAsset(const SPtr<Resource>& pRes);

  static SPtr<Resource>
  loadResourceFromAsset(Path filePath);

  //SPtr<Resource> m_res;

 private:
  static void
  saveStaticMesh(const SPtr<Resource>& pRes);

  SPtr<Resource>
  loadStaticMesh(sh_fstream& file);
};
}
