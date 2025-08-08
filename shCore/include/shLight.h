/*****************************************************************************/
/*
*  @file    shLight.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Basic light structure.
*
*  Basic light structure.
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
#include <shVector3.h>
#include <shVector4.h>
//#include "shLinearColor.h"

namespace shEngineSDK {
/**
*  @brief Basic light structure.
*/
struct SH_CORE_EXPORT ALIGN_AS(16) Light
{
  //Vector3 position;
  //float intensity;
  //LinearColor color;
  Vector4 pos;
};
}
