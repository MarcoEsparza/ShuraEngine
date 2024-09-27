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

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shPlane::setNormal(const Vector3& _norm)
{
  normal = _norm;
}

void
shPlane::setDistance(const float _d)
{
  distance = _d;
}

}
