/*****************************************************************************/
/*
*  @file    shDepthStencilState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/15
*  @brief   Engine Depth Stencil State.
*
*  Engine Depth Stencil State.
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
*  @brief Engine Depth Stencil State.
*/
class DepthStencilState
{
 public:
  /**
  *  @brief Default constructor.
  */
  DepthStencilState() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~DepthStencilState() = default;
};
}
