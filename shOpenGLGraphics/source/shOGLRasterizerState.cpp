/*****************************************************************************/
/*
*  @file    shOGLRasterizerState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL rasterizer state class.
*
*  Open GL rasterizer state class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLRasterizerState.h"

namespace shEngineSDK {
OGLRasterizerState::~OGLRasterizerState()
{
  m_fillMode = 0;
  m_cullMode = 0;
  m_bFrontCounterClockwise = false;
  m_depthBias = 0;
  m_slopeScaledDepthBias = 0.0f;
  m_depthBiasClamp = 0.0f;
  m_bDepthClipEnable = true;
  m_bScissorEnable = false;
  m_bMultisampleEnable = false;
  m_bAntialiasedLineEnable = false;
}
}
