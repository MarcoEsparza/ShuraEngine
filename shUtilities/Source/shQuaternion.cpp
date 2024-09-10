/*************************************************************/
/*
*  @file    shQuaternion.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
*  @brief   Quaternion for rotations
*
*  Quaternion for rotations
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shQuaternion.h"

namespace shEngineSDK {
Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}

Quaternion::Quaternion(const Quaternion& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/



}
