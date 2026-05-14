/*****************************************************************************/
/*
*  @file    shImgCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/13
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
#include <shPrerequisitesCore.h>
#include <shCodec.h>

namespace shEngineSDK {
class ImgCodec : public Codec
{
 public:
  ImgCodec() = default;
  ~ImgCodec() = default;

  UID
  getUID() const override;

  bool
  canDecodeExtension(const String& extension) const override;

  Vector<String>
  getSupportedExtensions() const override;

  bool
  decode(const String& filePath) const override;

  bool
  encode(const String& filePath) const override;
};
}
