/*************************************************************/
/*
*  @file    shOGLInputLayout.h
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
#include "shInputLayout.h"

namespace shEngineSDK {
class OGLInputLayout : public InputLayout
{
 public:
  OGLInputLayout() = default;
  FORCEINLINE ~OGLInputLayout();

  friend class OGLGraphicsManager;

 protected:
   UMap<uint32, String> m_inputData;
   uint32 m_shaderID = 0;
};

FORCEINLINE OGLInputLayout::~OGLInputLayout()
{

}
}
