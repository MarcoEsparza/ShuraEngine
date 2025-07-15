/*************************************************************/
/*
*  @file    shCapsule.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/06
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
#include "shMath.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
Capsule::setRadius(const float _radius)
{
  radius = _radius;
}

float
Capsule::getRadius()
{
  return radius;
}

void
Capsule::projectOnAxis(const Vector3& axis, float& min, float& max) const
{
  Vector3 pA = center + (direction * (height * 0.5f));
  Vector3 pB = center - (direction * (height * 0.5f));
  float dotA = pA.dot(axis);
  float dotB = pB.dot(axis);
  min = Math::min(dotA, dotB) - radius;
  max = Math::max(dotA, dotB) + radius;
}
}
