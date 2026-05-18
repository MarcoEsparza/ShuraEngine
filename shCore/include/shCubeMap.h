/*****************************************************************************/
/*
*  @file    shCubeMap.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/16
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
/**
*  @brief Cube Map class for reading and storing cube map textures.
*/
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
  *  @brief Constructor that initializes the cube map with the given LUT size
  *         and texture data.
  * 
  *  @param uint32 lutSize: Size of the LUT texture.
  *  @param Vector<LinearColor>& textureData: Cube map texture data.
  */
  CubeMap(const uint32 lutSize, const Vector<LinearColor>& textureData)
    : Resource(RESOURCE_TYPE::kTexture), m_lutSize(lutSize), m_textureData(textureData)
  {
    createTexture(lutSize, textureData);
  }

  /**
  *  @brief Default destructor.
  */
  ~CubeMap() = default;

 public:
  /**
  *  @brief Get the cube map texture ID.
  *
  *  @return uint32
  */
  FORCEINLINE const uint32
  getLutSize() const;

  /**
  *  @brief Get the cube map texture.
  *
  *  @return SPtr<Texture3D>
  */
  FORCEINLINE const SPtr<Texture3D>&
  getTexture() const;

 private:
  /**
  *  @brief Creates the cube map texture from the given LUT size and texture data.
  *
  *  @param uint32 lutSize: Size of the LUT texture.
  *  @param Vector<LinearColor>& textureData: Cube map texture data.
  */
  void
  createTexture(const uint32 lutSize, const Vector<LinearColor>& textureData);

 private:
  /**
  *  @brief Size of the LUT texture.
  */
  uint32 m_lutSize = 0;

  /**
  *  @brief Cube map texture data.
  */
  Vector<LinearColor> m_textureData;

  /**
  *  @brief Cube map texture.
  */
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
