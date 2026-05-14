/*****************************************************************************/
/*
*  @file    shCubeMap.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   Cube Map class for reading and storing cube map textures.
*
*  Cube Map class for reading and storing cube map textures.
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
#include "shResource.h"
#include "shTexture.h"
#include <shVector3.h>
#include <shLinearColor.h>
#include <shPath.h>

namespace shEngineSDK {
class CubeMap : public Resource
{
 public:
  /**
   *  @brief Default constructor.
   */
  CubeMap()
    : Resource(RESOURCE_TYPE::kTexture)
  {}

  /**
   *  @brief Default destructor.
   */
  ~CubeMap() = default;

 public:
  /**
   *  @brief Load the cube map texture from file.
   *
   *  @param const String& filePath
   *  @return bool
   */
  bool
  loadFromFile(const Path& filePath);

  /**
   *  @brief Get the cube map texture ID.
   *
   *  @return uint32
   */
  FORCEINLINE const uint32
  getLutSize() const;

  FORCEINLINE const SPtr<Texture3D>&
  getTexture() const;

 private:
  bool
  parseCubeFile(const Path& filePath);

 private:
  /**
   *  @brief Size of the LUT texture.
   */
  uint32 m_lutSize = 0;

  /**
   *  @brief Cube map texture data.
   */
  Vector<LinearColor> m_textureData;

  SPtr<Texture3D> m_pTexture;
};

FORCEINLINE const uint32
CubeMap::getLutSize() const
{
  return m_lutSize;
}

FORCEINLINE const SPtr<Texture3D>&
CubeMap::getTexture() const
{
  return m_pTexture;
}
}
