/*************************************************************/
/*
*  @file    shPlane.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
shPlane::setNormal(const Vector3& _normal)
{
  normal = _normal;
}

void
shPlane::setDistance(const float _distance)
{
  distance = _distance;
}

Vector3
shPlane::getNormal() const
{
  return normal;
}

float
shPlane::getDistance() const
{
  return distance;
}

float
shPlane::pointToPlaneDistance(const Vector3& point) const
{
  const float srqNormal = Math::sqrt(normal.x * normal.x +
                                     normal.y * normal.y +
                                     normal.z * normal.z);

  return (normal.x * point.x +
          normal.y * point.y +
          normal.z * point.z +
          distance) / srqNormal;
}
}
