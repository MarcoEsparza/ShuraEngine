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
  if (m_bufferID != 0) {
    glDeleteBuffers(1, &m_bufferID);
    m_bufferID = 0;
    m_stride = 0;
    m_size = 0;
  }
}

OGLIndexBuffer::~OGLIndexBuffer()
{
  if (m_bufferID != 0) {
    glDeleteBuffers(1, &m_bufferID);
    m_bufferID = 0;
    m_count = 0;
  }
}

OGLConstantBuffer::~OGLConstantBuffer()
{
  if (m_bufferID != 0) {
    glDeleteBuffers(1, &m_bufferID);
    m_bufferID = 0;
    m_size = 0;
  }
}
}
