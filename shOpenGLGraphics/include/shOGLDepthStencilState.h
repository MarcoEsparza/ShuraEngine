/*****************************************************************************/
/*
*  @file    shOGLDepthStencilState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   Open GL depth stencil state class.
*
*  Open GL depth stencil state class.
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
#include "shDepthStencilState.h"

namespace shEngineSDK {
class OGLDepthStencilState : public DepthStencilState
{
  public:
   OGLDepthStencilState() = default;
   virtual ~OGLDepthStencilState();
  private:
    uint32 m_depthStencilStateID = 0;
};
}
