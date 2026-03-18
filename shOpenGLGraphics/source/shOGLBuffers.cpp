/*****************************************************************************/
/*
*  @file    shOGLBuffers.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   OpenGL buffer class wrappers.
*
*  OpenGL buffer class wrappers.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLBuffers.h"

namespace shEngineSDK {
OGLVertexBuffer::~OGLVertexBuffer()
{
  if (m_vbo != 0) {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }
}

OGLIndexBuffer::~OGLIndexBuffer()
{
  if (m_ibo != 0) {
    glDeleteBuffers(1, &m_ibo);
    m_ibo = 0;
  }
}

OGLConstantBuffer::~OGLConstantBuffer()
{
  if (m_ubo != 0) {
    glDeleteBuffers(1, &m_ubo);
    m_ubo = 0;
  }
}
}
