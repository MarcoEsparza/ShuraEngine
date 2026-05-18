/*****************************************************************************/
/*
*  @file    shAssetCodec.h
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

namespace shEngineSDK {
class AssetCodec : public Codec
{
 public:
  /**
   *  @brief Default constructor.
   */
  AssetCodec() = default;
  /**
   *  @brief Default destructor.
   */
  ~AssetCodec() = default;

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

  bool
  decode(const Path& filePath) const override;

  bool
  encode(const String& objName, const Path& saveFilePath) const override;
};
}
