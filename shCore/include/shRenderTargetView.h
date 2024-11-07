/*************************************************************/
/*
*  @file    shRenderTargetView.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Render Target View class.
*
*  Engine Render Target View class.
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
*  @brief Engine Render Target View class.
*/
class RenderTargetView
{
 public:
  /**
  *  @brief Default constructor.
  */
  RenderTargetView() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~RenderTargetView() = default;
};
}
