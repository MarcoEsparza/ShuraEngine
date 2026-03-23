/*****************************************************************************/
/*
*  @file    shOGLInputLayout.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOGLInputLayout.h"

namespace shEngineSDK {
OGLInputLayout::~OGLInputLayout()
{
  if (m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
}
}
