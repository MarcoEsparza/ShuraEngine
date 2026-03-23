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

   friend class OGLGraphicsManager;

  private:
   bool m_depthEnable = true;
   bool m_depthWriteMask = true;
   uint32 m_depthFunc = 0x0201; // GL_LESS

   bool m_stencilEnable = false;
   uint8 m_stencilReadMask = 0xFF;
   uint8 m_stencilWriteMask = 0xFF;

   struct Face
   {
     uint32 func;
     uint32 stencilFailOp;
     uint32 depthFailOp;
     uint32 passOp;
   };

   Face m_frontFace;
   Face m_backFace;
};
}
