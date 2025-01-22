/*************************************************************/
/*
*  @file    shRasterizerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/14
*  @brief   Engine Rasterizer State.
*
*  Engine Rasterizer State.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"

namespace shEngineSDK {
/**
*  @brief Engine Rasterizer State.
*/
class RasterizerState
{
 public:
  /**
  *  @brief Default constructor.
  */
  RasterizerState() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~RasterizerState() = default;
};
}
