/*****************************************************************************/
/*
*  @file    shImgCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   Image codec for loading and saving image files.
*
*  Image codec for loading and saving image files.
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
#include <shPrerequisitesCore.h>
#include <shCodec.h>

namespace shEngineSDK {
/**
*  @brief Image codec for loading and saving image files.
*/
class ImgCodec : public Codec
{
 public:
  /**
  *  @brief Default constructor.
  */
  ImgCodec() = default;

  /**
  *  @brief Default destructor.
  */
  ~ImgCodec() = default;

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
  *  @brief Decodes the file at the given path and loads it as a resource.
  * 
  *  @param const Path& filePath: Path to the file to decode.
  * 
  *  @return bool: True if the file was decoded and loaded successfully, false if it wasn't.
  */
  bool
  decode(const Path& filePath) const override;

  /**
  *  @brief Encodes the resource to a file at the given path.
  * 
  *  @param const Path& filePath: Path to the file to encode.
  * 
  *  @return bool: True if the resource was encoded and saved successfully,
  *                false if it wasn't.
  */
  bool
  encode(const Path& filePath) const override;
};
}
