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
shSphere::setSize(const float _radius)
{
  radius = _radius;
}

}
