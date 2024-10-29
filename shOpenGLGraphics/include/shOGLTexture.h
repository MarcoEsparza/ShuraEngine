/*************************************************************/
/*
*  @file    shOGLTexture.h
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
#include "shTexture.h"
#include "shLinearColor.h"

namespace shEngineSDK {
class OGLTexture2D : public Texture2D
{
 public:
   OGLTexture2D() = default;
  FORCEINLINE ~OGLTexture2D();

  friend class OGLGraphicsManager;

 protected:
  uint32 m_texture = 0;
};

class OGLDepthRender : public Texture2D
{
public:
  OGLDepthRender() = default;
  FORCEINLINE ~OGLDepthRender();

  friend class OGLGraphicsManager;

protected:
  uint32 m_depthBuffer = 0;
  LinearColor m_color;
};

FORCEINLINE OGLTexture2D::~OGLTexture2D()
{
  glDeleteTextures(1, &m_texture);
}

FORCEINLINE OGLDepthRender::~OGLDepthRender()
{
  glDeleteRenderbuffers(1, &m_depthBuffer);
}
}
