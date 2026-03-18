/*****************************************************************************/
/*
*  @file    shOGLSamplerState.h
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
#include "shSamplerState.h"

namespace shEngineSDK {
class OGLSamplerState : public SamplerState
{
 public:
   OGLSamplerState() = default;
  virtual ~OGLSamplerState();

  friend class OGLGraphicsManager;

 protected:
  uint32 m_samplerID = 0;
};
}
