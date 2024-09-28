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
}
