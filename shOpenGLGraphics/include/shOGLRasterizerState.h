/*****************************************************************************/
/*
*  @file    shOGLRasterizerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL rasterizer state class.
*
*  Open GL rasterizer state class.
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
#include "shPrerequisitesOGLGraphics.h"
#include "shRasterizerState.h"

namespace shEngineSDK {
class OGLRasterizerState : public RasterizerState
{
 public:
  OGLRasterizerState() = default;
  virtual ~OGLRasterizerState();
 private:
  uint32 m_rasterizerStateID = 0;
};
}
