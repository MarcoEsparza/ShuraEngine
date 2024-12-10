/*************************************************************/
/*
*  @file    shOGLFrameBuffer.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/27
*  @brief
*
*
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
#include "shPrerequisitesOGLGraphics.h"
#include "shRenderTargetView.h"

namespace shEngineSDK {
class OGLFrameBuffer : public RenderTargetView
{
 public:
  OGLFrameBuffer() = default;
  FORCEINLINE ~OGLFrameBuffer();

  friend class OGLGraphicsManager;

 protected:
  uint32 m_frameObject = 0;
  uint32 m_texture = 0;
};

FORCEINLINE OGLFrameBuffer::~OGLFrameBuffer()
{
  glDeleteFramebuffers(1, &m_frameObject);
  glDeleteTextures(1, &m_texture);
}
}
