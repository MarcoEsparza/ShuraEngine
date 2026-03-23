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
#include <shLinearColor.h>

namespace shEngineSDK {
class OGLBlendState : public BlendState
{
 public:
  OGLBlendState() = default;
  virtual ~OGLBlendState();

  friend class OGLGraphicsManager;

 protected:
   bool m_blendEnable = false;
   uint32 m_srcRGB = 0;
   uint32 m_dstRGB = 0;
   uint32 m_opRGB = 0;
   uint32 m_srcAlpha = 0;
   uint32 m_dstAlpha = 0;
   uint32 m_opAlpha = 0;
   uint32 m_writeMask = 0;
   LinearColor m_blendFactor = LinearColor::WHITE;
};
}
