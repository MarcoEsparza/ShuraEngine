/*****************************************************************************/
/*
*  @file    shPlane.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for plane figure.
*
*  Math class for plane figure.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPlane.h"
#include "shMath.h"

namespace shEngineSDK {

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

float
Plane::pointToPlaneDistance(const Vector3& point) const
{
  return (Math::abs(normal.dot(point) - distance) / normal.mag());
}

float
Plane::evaluate(const Vector3& point) const
{
  return (normal.dot(point) + distance);
}

void
Plane::normalize()
{
  normal.normalize();
  distance /= normal.mag();
}
}
