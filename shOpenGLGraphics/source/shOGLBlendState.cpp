/*****************************************************************************/
/*
*  @file    shOGLBlendstate.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL blend state class.
*
*  Open GL blend state class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLBlendState.h"
#include <shException.h>

namespace shEngineSDK {
OGLBlendState::~OGLBlendState()
{
  m_blendEnable = false;
  m_srcRGB = 0;
  m_dstRGB = 0;
  m_opRGB = 0;
  m_srcAlpha = 0;
  m_dstAlpha = 0;
  m_opAlpha = 0;
  m_writeMask = 0;
  m_blendFactor = LinearColor::WHITE;
}
void
OGLBlendState::setDebugName(const String& name)
{
  SH_UNREFERENCED_PARAMETER(name);
}
}
