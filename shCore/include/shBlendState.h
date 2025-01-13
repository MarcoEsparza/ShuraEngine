/*************************************************************/
/*
*  @file    shBlendState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/13
*  @brief   Engine Blend State.
*
*  Engine Blend State.
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
  *  @brief Engine Blend State.
  */
  class BlendState
  {
  public:
    /**
    *  @brief Default constructor.
    */
    BlendState() = default;

    /**
    *  @brief Default destructor.
    */
    virtual ~BlendState() = default;
  };
}
