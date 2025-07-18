/*****************************************************************************/
/*
*  @file    shTexture.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Engine base Texture classes.
*
*  Engine base Texture classes.
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
/**
*  @brief Engine base Texture2D class.
*/
class Texture2D
{
public:
  /**
  *  @brief Default constructor.
  */
  Texture2D() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Texture2D() = default;
};
}
