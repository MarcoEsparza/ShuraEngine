/*************************************************************/
/*
*  @file    shCollisionInfo.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/29
*  @brief   Basic collision information struct.
*
*  Basic collision information struct.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Basic collision information struct.
*/
struct CollisionInfo
{
  float penetrationDepth = 0.0f;
  Vector3 normal = Vector3::ZERO;
  Vector3 contactPoint = Vector3::ZERO;
};
}
