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
  if (m_textureID != 0) {
    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
    m_target = 0;
    m_width = 0;
    m_height = 0;
    m_mipLevels = 0;
    m_arraySize = 0;
    m_format = 0;
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
