/*************************************************************/
/*
*  @file    shVector4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Vector4 with float.
*
*  Vector4 with float.
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shVector4.h"
#include "shMath.h"

namespace shEngineSDK {
Vector4::Vector4(const Vector4& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector4::dot(const Vector4& _other) const
{
  return ((x * _other.x) + (y * _other.y) + (z * _other.z) + (w * _other.w));
}

float
Vector4::mag() const
{
  float lenght = (x * x) + (y * y) + (z * z) + (w * w);

  return Math::sqrt(lenght);
}

void
Vector4::normalize()
{
  float invMag = 1 / mag();
  if (invMag != 0.0f) {
    x *= invMag;
    y *= invMag;
    z *= invMag;
    w *= invMag;
  }
  else {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }
}

Vector4
Vector4::lerp(const Vector4& _vecA,
              const Vector4& _vecB,
              const float& _time) const
{
  return _vecA + (_vecB - _vecA) * _time;
}
}
