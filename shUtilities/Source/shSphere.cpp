/*************************************************************/
/*
*  @file    shSphere.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
*  @brief   Math class for sphere figure.
*
*  Math class for sphere figure.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shSphere.h"
#include "shMath.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

bool
shSphere::intersect(const shSphere& _other)
{
  float distance = Math::sqrt((center.x - _other.center.x) +
                              (center.y - _other.center.y) +
                              (center.z - _other.center.z));

  return (distance < (radius + _other.radius));
}
}
