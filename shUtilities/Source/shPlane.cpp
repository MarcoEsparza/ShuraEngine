/*************************************************************/
/*
*  @file    shPlane.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
*  @brief   Math class for plane figure.
*
*  Math class for plane figure.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlane.h"
#include "shMath.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
Plane::setNormal(const Vector3& _normal)
{
  normal = _normal;
}

void
Plane::setDistance(const float _distance)
{
  distance = _distance;
}

Vector3
Plane::getNormal() const
{
  return normal;
}

float
Plane::getDistance() const
{
  return distance;
}

float
Plane::pointToPlaneDistance(const Vector3& point) const
{
  return (Math::abs(normal.dot(point) - distance) / normal.mag());
}

float Plane::evaluate(const Vector3& point) const
{
  return (normal.dot(point) + distance);
}
}
