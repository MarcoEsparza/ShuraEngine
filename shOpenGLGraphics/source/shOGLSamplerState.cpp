/*****************************************************************************/
/*
*  @file    shOGLSamplerState.cpp
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
#include "shOGLSamplerState.h"

namespace shEngineSDK {
OGLSamplerState::~OGLSamplerState()
{
  if (m_samplerID != 0) {
    glDeleteSamplers(1, &m_samplerID);
    m_samplerID = 0;
  }
}
}
