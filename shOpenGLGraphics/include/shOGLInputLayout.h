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
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesOGLGraphics.h"
#include "shInputLayout.h"

namespace shEngineSDK {
class OGLInputLayout : public InputLayout
{
 public:
  OGLInputLayout() = default;
  virtual ~OGLInputLayout();

  friend class OGLGraphicsManager;

 protected:
   //UMap<uint32, String> m_inputData;
   uint32 m_vao = 0;
};
}
