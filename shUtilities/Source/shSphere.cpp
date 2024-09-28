/*************************************************************/
/*
*  @file    shSphere.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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

void
shSphere::setCenter(const Vector3& _center)
{
  center = _center;
}

void
shSphere::setRadius(const float _radius)
{
  radius = _radius;
}

Vector3
shSphere::getCenter() const
{
  return center;
}

float
shSphere::getRadius() const
{
  return radius;
}
}
