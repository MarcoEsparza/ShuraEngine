/*****************************************************************************/
/*
*  @file    shResourceCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shCodec.h"
#include <shPath.h>
#include <shVector2.h>
#include <shVector3.h>

namespace shEngineSDK {
class Material;
class StaticMeshResource;

/**
*  @brief Header structure for mesh data in asset files.
*/
struct SH_CORE_EXPORT MeshDataHeader
{
  uint32 numVertices = 0;
  uint32 numIndices = 0;
  uint32 matIndex = 0;
  uint32 nameSize = 0;
};

/**
*  @brief Header structure for static mesh asset files.
*/
struct SH_CORE_EXPORT StaticMeshAssetHeader
{
  uint32 numMeshes = 0;
  uint32 numMaterials = 0;
};

/**
*  @brief Header structure for skeletal mesh asset files.
*/
struct SH_CORE_EXPORT SkeletalMeshAssetHeader
{
  uint32 numMeshes = 0;
  uint32 numMaterials = 0;
  uint32 numVertices = 0;
  uint32 numIndices = 0;
};

/**
*  @brief Header structure for material asset files.
*/
struct SH_CORE_EXPORT MaterialAssetHeader {
  uint32 nameSize = 0;
  uint32 properties = 0;
  uint32 baseColorMapPathSize = 0;
  uint32 normalMapPathSize = 0;
  uint32 metalnessMapPathSize = 0;
  uint32 roughnessMapPathSize = 0;
  uint32 aoMapPathSize = 0;
  uint32 emissiveMapPathSize = 0;
  uint32 specularMapPathSize = 0;
  uint32 opacityMaskMapPathSize = 0;
  Vector2 metallicRoughnessFactor = Vector2::ONE;
  Vector3 baseColorFactor = Vector3::ONE;
  float opacityFactor = 1.0f;
  Vector3 emissiveFactor = Vector3::ZERO;
  float emmisiveIntensity = 1.0f;
  float alphaCutoff = 0.5f;
};

/**
*  @brief Codec for encoding and decoding engine resources to and from files.
*/
class ResourceCodec : public Codec
{
 public:
  /**
  *  @brief Default constructor.
  */
  ResourceCodec() = default;

  /**
  *  @brief Default destructor.
  */
  ~ResourceCodec() = default;

  /**
  *  @brief Gets the unique identifier (UID) of the codec.
  * 
  *  @return UID: Unique identifier of the codec.
  */
  UID
  getUID() const override;

  /**
  *  @brief Gets the list of supported file extensions by the codec.
  *
  *  @return Vector<String>: List of supported file extensions by the codec,
  *                          including the dot.
  */
  Vector<String>
  getSupportedExtensions() const override;

  /**
  *  @brief Decodes the file at the given path.
  * 
  *  @param const Path& filePath: Path of the file to decode.
  * 
  *  @return bool: True if the file was decoded successfully, false if it wasn't.
  */
  bool
  decode(const Path& filePath) const override;

  /**
  *  @brief Encodes the given object to a file at the given path.
  * 
  *  @param const String& objName: Name of the object to encode.
  *  @param const Path& saveFilePath: Path of the file to save the encoded object.
  * 
  *  @return bool: True if the object was encoded successfully, false if it wasn't.
  */
  bool
  encode(const String& objName, const Path& saveFilePath) const override;

 private:
  SPtr<Material>
  getMaterialFromFile(FILE* pFile, SPtr<Material>& pMat, const Path& filePath) const;

  bool
  decodeStaticMesh(FILE* pFile, const Path& filePath) const;

  bool
  encodeStaticMesh(const String& objName, const Path& saveFilePath) const;
};
}
