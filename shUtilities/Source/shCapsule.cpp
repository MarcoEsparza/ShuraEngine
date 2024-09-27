/*************************************************************/
/*
*  @file    shCapsule.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for capsule figure.
*
*  Math class for capsule figure.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shCapsule.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shCapsule::setPosition(const Vector3& _pA, const Vector3& _pB)
{
  pointA = _pA;
  pointB = _pB;
}

void
shCapsule::setRadius(const float& _radius)
{
  radius = _radius;
}

}
