/*****************************************************************************/
/*
*  @file    shOGLBlendstate.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL blend state class.
*
*  Open GL blend state class.
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
#include "shBlendState.h"

namespace shEngineSDK {
class OGLBlendState : public BlendState
{
 public:
  OGLBlendState() = default;
  virtual ~OGLBlendState();
 private:
   uint32 m_blendStateID = 0;
};
}
