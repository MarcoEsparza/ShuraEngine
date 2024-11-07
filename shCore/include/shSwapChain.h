/*************************************************************/
/*
*  @file    shSwapChain.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Swap Chain class.
*
*  Engine Swap Chain class.
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
*  @brief Engine Swap Chain class.
*/
class SwapChain
{
 public:
  /**
  *  @brief Default constructor.
  */
  SwapChain() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~SwapChain() = default;
};
}
