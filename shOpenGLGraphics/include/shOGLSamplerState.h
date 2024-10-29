/*************************************************************/
/*
*  @file    shOGLSamplerState.h
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
#include "shSamplerState.h"

namespace shEngineSDK {
class OGLSamplerState : public SamplerState
{
 public:
   OGLSamplerState() = default;
  FORCEINLINE ~OGLSamplerState();

  friend class OGLGraphicsManager;

 protected:
  uint32 m_samplerID = 0;
};

FORCEINLINE OGLSamplerState::~OGLSamplerState()
{
  
}
}
