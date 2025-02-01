/*****************************************************************************/
/*
*  @file    shLight.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/31
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
#include "shPrerequisitesCore.h"
#include "shVector3.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT Light
{
  Vector3 direction;
  Vector3 color;
  float intensity;
  float nothing;
};
}
