/*****************************************************************************/
/*
*  @file    shCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   Base class for codecs to decode and encode files.
*
*  Base class for codecs to decode and encode files.
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
#include <shPath.h>

namespace shEngineSDK {
/**
*  @brief Base class for codecs to decode and encode files.
*/
class Codec : public sh_enableShared<Codec>
{
 public:
  /**
  *  @brief Default constructor.
  */
  Codec() = default;

  /**
  *  @brief Default destructor.
  */
  ~Codec() = default;
  
  /**
  *  @brief Gets the unique identifier (UID) of the codec.
  *
  *  @return UID: Unique identifier of the codec.
  */
  virtual UID
  getUID() const = 0;

  /**
  *  @brief Checks if the codec can decode the given file extension.
  *
  *  @param const String& extension: File extension to check, including the dot,
  *                                  for example: ".png".
  *
  *  @return bool: True if the codec can decode the given file extension, false if it can't.
  */
  FORCEINLINE virtual bool
  canDecodeExtension(const String& extension) const;

  /**
  *  @brief Gets the list of supported file extensions by the codec.
  *
  *  @return Vector<String>: List of supported file extensions by the codec,
  *                          including the dot.
  */
  virtual Vector<String>
  getSupportedExtensions() const = 0;

  /**
  *  @brief Decodes the file at the given path.
  * 
  *  @param const Path& filePath: Path of the file to decode.
  * 
  *  @return bool: True if the file was decoded successfully, false if it wasn't.
  */
  virtual bool
  decode(const Path& filePath) const = 0;

  /**
  *  @brief Encodes the given object to a file at the given path.
  * 
  *  @param const String& objName: Name of the object to encode.
  *  @param const Path& saveFilePath: Path of the file to save the encoded object.
  * 
  *  @return bool: True if the object was encoded successfully, false if it wasn't.
  */
  virtual bool
  encode(const String& objName, const Path& saveFilePath) const = 0;
};

FORCEINLINE bool
Codec::canDecodeExtension(const String& extension) const
{
  Vector<String> supportedExtensions = getSupportedExtensions();
  for (const String& ext : supportedExtensions) {
    if (ext.compare(extension) == 0) {
      return true;
    }
  }
  return false;
}
}
