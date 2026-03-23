/*****************************************************************************/
/*
*  @file    shOGLDepthStencilState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL depth stencil state class.
*
*  Open GL depth stencil state class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLDepthStencilState.h"

namespace shEngineSDK {
OGLDepthStencilState::~OGLDepthStencilState()
{
  m_depthEnable = true;
  m_depthWriteMask = true;
  m_depthFunc = 0x0201; // GL_LESS
  m_stencilEnable = false;
  m_stencilReadMask = 0xFF;
  m_stencilWriteMask = 0xFF;
  m_frontFace.func = 0;
  m_frontFace.stencilFailOp = 0;
  m_frontFace.depthFailOp = 0;
  m_frontFace.passOp = 0;
  m_backFace.func = 0;
  m_backFace.stencilFailOp = 0;
  m_backFace.depthFailOp = 0;
  m_backFace.passOp = 0;
}
}
