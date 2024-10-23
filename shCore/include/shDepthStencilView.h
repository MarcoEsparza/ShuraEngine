/*************************************************************/
/*
*  @file    shDepthStencilView.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Depth Stencil View class wrapper.
*
*  Engine Depth Stencil View class wrapper.
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
*  @brief Engine Depth Stencil View class.
*/
class DepthStencilView
{
 public:
  /**
  *  @brief Default constructor.
  */
  DepthStencilView() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~DepthStencilView() = default;
};
}
