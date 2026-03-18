/******************************************************************************/
/*
*  @file    shOGLTexture.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/16
*  @brief   Open GL texture class.
*
*  Open GL texture class.
*
*  @bug     No bug known.
*/
/******************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLTexture.h"

namespace shEngineSDK {
OGLTexture2D::~OGLTexture2D()
{
  glDeleteTextures(1, &m_texture);
  glDeleteTextures(1, &m_shaderResourceView);
  glDeleteTextures(1, &m_depthStencilView);

  for (auto& rtv : m_renderTargetViews) {
    glDeleteTextures(1, &rtv);
  }

  for (auto& uav : m_unorderedAccessViews) {
    glDeleteTextures(1, &uav);
  }
}

void
OGLTexture2D::setDebugName(const String& name)
{
}

OGLTexture3D::~OGLTexture3D()
{
  glDeleteTextures(1, &m_texture);
  glDeleteTextures(1, &m_shaderResourceView);
}
}
