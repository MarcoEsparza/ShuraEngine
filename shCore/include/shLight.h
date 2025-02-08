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
#include "shVector4.h"
#include "shLinearColor.h"

namespace shEngineSDK {
MS_ALIGN(16)
struct SH_CORE_EXPORT Light
{
  //Vector3 position;
  //float intensity;
  //LinearColor color;
  Vector4 pos;
} GCC_ALIGN(16);
}
