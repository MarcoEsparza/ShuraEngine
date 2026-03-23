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

  friend class OGLGraphicsManager;

 private:
  uint32 m_fillMode = 0;
  uint32 m_cullMode = 0;
  bool m_bFrontCounterClockwise = false;

  int32 m_depthBias = 0;
  float m_slopeScaledDepthBias = 0.0f;
  float m_depthBiasClamp = 0.0f;

  bool m_bDepthClipEnable = true;
  bool m_bScissorEnable = false;
  bool m_bMultisampleEnable = false;
  bool m_bAntialiasedLineEnable = false;
};
}
