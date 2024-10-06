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
#include "shMath.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shCapsule::setPosition(const Vector3& pA, const Vector3& pB)
{
  pointA = pA;
  pointB = pB;
}

void
shCapsule::setRadius(const float _radius)
{
  radius = _radius;
}

Vector3
shCapsule::getPointA() const
{
  return pointA;
}

Vector3
shCapsule::getPointB() const
{
  return pointB;
}

float
shCapsule::getRadius()
{
  return radius;
}
void
shCapsule::projectOnAxis(const Vector3& axis, float& min, float& max) const
{
  Vector3 dir = pointB - pointA;
  float length = Math::sqrt(dir.dot(dir));

  if (length > 0.0f) {
    dir.normalize();
  }

  float projPA = pointA.dot(axis);
  float projPB = pointB.dot(axis);
  float projRadius = radius * Math::abs(axis.dot(dir));

  min = Math::min(projPA, projPB) - projRadius;
  max = Math::max(projPA, projPB) + projRadius;
}
}
