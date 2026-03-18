/*****************************************************************************/
/*
*  @file    shOGLShader.cpp
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
#include "shOGLShader.h"

namespace shEngineSDK {
OGLVertexShader::~OGLVertexShader()
{
  glDeleteShader(m_vertexShaderID);
}

OGLPixelShader::~OGLPixelShader()
{
  glDeleteShader(m_pixelShaderID);
}

OGLGeometryShader::~OGLGeometryShader()
{
  glDeleteShader(m_geometryShaderID);
}

OGLComputeShader::~OGLComputeShader()
{
  glDeleteShader(m_computeShaderID);
}
}
