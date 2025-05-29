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

//struct SH_CORE_EXPORT ImgResourceAsset
//{
//  String 
//};

struct SH_CORE_EXPORT MaterialAssetHeader
{
  bool hasDiffuse;
  /*uint32 bHasDiffuseMap : 1;
  uint32 bHasSpecularMap : 1;
  uint32 bHasNormalMap : 1;
  uint32 bHasMetalnessMap : 1;
  uint32 bHasRoughnessMap : 1;
  uint32 bHasAmbientOcclusionMap : 1;
  uint32 bIsOpaque : 1;
  uint32 bHasAlphaTest : 1;
  uint32 bHasAlphaBlend : 1;
  uint32 bIsDoubleSided : 1;
  uint32 bWireframeEnabled : 1;
  uint32 bCanCastShadows : 1;
  uint32 bCanReceiveShadows : 1;
  uint32 Unused : 19;*/
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

  void
  loadResourceFromAsset(Path filePath);

  SPtr<Resource> m_res;

 private:
  void
  saveStaticMesh(const SPtr<Resource>& pRes);

  SPtr<Resource>
  loadStaticMesh(sh_fstream& file);
};
}
