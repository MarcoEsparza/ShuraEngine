/*****************************************************************************/
/*
*  @file    shCubemapCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief   Codec for cube map files. This codec will be used to read and write
*           cube map files.
*
*  Codec for cube map files. This codec will be used to read and write cube map files.
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

namespace shEngineSDK {
class CubeMap;

/**
*  @brief Codec for cube map files. This codec will be used to read and write cube map files.
*/
class CubeMapCodec : public Codec
{
 public:
  /**
  *  @brief Default constructor.
  */

  CubeMapCodec() = default;
  /**
  *  @brief Default destructor.
  */
  ~CubeMapCodec() = default;

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
  virtual Vector<String>
  getSupportedExtensions() const override;

  /**
  *  @brief Decodes the cube map file at the given path and creates a CubeMap resource.
  */
  bool
  decode(const Path& filePath) const override;

  /**
  *  @brief Encodes the CubeMap resource from the Resource Manager to a file at the given path.
  * 
  *  @param const String& objName: Name of the CubeMap resource to encode.
  *  @param const Path& saveFilePath: Path of the file to save the encoded CubeMap resource.
  * 
  *  @return bool: True if the CubeMap resource was encoded successfully, false if it wasn't.
  */
  bool
  encode(const String& objName, const Path& saveFilePath) const override;

 private:
  /**
  *  @brief Parses the cube map file at the given path and creates a CubeMap resource.
  */
  bool
  parseCubeFile(const Path& filePath) const;
};
}
