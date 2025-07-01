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
  /*Vector3 dir = pointB - pointA;
  float length = Math::sqrt(dir.dot(dir));

  if (length > 0.0f) {
    dir.normalize();
  }

  float projPA = pointA.dot(axis);
  float projPB = pointB.dot(axis);
  float projRadius = radius * Math::abs(axis.dot(dir));

  min = Math::min(projPA, projPB) - projRadius;
  max = Math::max(projPA, projPB) + projRadius;*/
}
}
