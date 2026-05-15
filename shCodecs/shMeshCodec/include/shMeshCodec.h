/*****************************************************************************/
/*
*  @file    shMeshCodec.h
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
class MeshCodec : public Codec
{
 public:
  MeshCodec() = default;
  ~MeshCodec() = default;

  UID
  getUID() const override;

  bool
  canDecodeExtension(const String& extension) const override;

  Vector<String>
  getSupportedExtensions() const override;

  bool
  decode(const Path& filePath) const override;

  bool
  encode(const Path& filePath) const override;
};
}
