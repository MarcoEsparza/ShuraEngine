/*************************************************************/
/*
*  @file    shOGLShader.h
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
#include "shShader.h"

namespace shEngineSDK {
class OGLProgramShader : public ProgramShader
{
 public:
  OGLProgramShader() = default;
  FORCEINLINE ~OGLProgramShader();

  friend class OGLGraphicsManager;

 protected:
  uint32 m_vertexShader = 0;
  uint32 m_fragShader = 0;
  uint32 m_programID = 0;
};

FORCEINLINE OGLProgramShader::~OGLProgramShader()
{

}
}
