/*****************************************************************************/
/*
*  @file    shCodec.h
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
#include "shPrerequisitesCore.h"

namespace shEngineSDK {
class Codec : public sh_enableShared<Codec>
{
 public:
  Codec() = default;
  ~Codec() = default;

  virtual UID
  getUID() const = 0;

  virtual bool
  canDecodeExtension(const String& extension) const = 0;

  virtual Vector<String>
  getSupportedExtensions() const = 0;

  virtual bool
  decode(const String& filePath) const = 0;

  virtual bool
  encode(const String& filePath) const = 0;
};
}
