/*****************************************************************************/
/*
*  @file    shOGLShader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief
*
*
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
#include "shShader.h"

namespace shEngineSDK {
class OGLVertexShader : public VertexShader
{
 public:
  OGLVertexShader() = default;
  virtual ~OGLVertexShader();

  friend class OGLGraphicsManager;

 private:
   uint32 m_vertexShaderID = 0;
};

class OGLPixelShader : public PixelShader
{
 public:
  OGLPixelShader() = default;
  virtual ~OGLPixelShader();

  friend class OGLGraphicsManager;

 private:
   uint32 m_pixelShaderID = 0;
};

class OGLGeometryShader : public GeometryShader
{
 public:
  OGLGeometryShader() = default;
  virtual ~OGLGeometryShader();

  friend class OGLGraphicsManager;

 private:
   uint32 m_geometryShaderID = 0;
};

class OGLComputeShader : public ComputeShader
{
 public:
  OGLComputeShader() = default;
  virtual ~OGLComputeShader();

  friend class OGLGraphicsManager;

 private:
   uint32 m_computeShaderID = 0;
};
}
