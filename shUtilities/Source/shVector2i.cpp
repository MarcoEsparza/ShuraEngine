/*************************************************************/
/*
*  @file    shVector2i.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Vector2 with integers
*
*  Vector2 with integers
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shVector2i.h"
#include "shMath.h"

namespace shEngineSDK {
Vector2i::Vector2i(const Vector2i& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


int32
Vector2i::dot(const Vector2i& _other) const
{
  return (x * _other.x + y * _other.y);
}

float
Vector2i::mag() const
{
  return Math::sqrtf(static_cast<float>(x * x + y * y));
}

void
Vector2i::normalize()
{
  float invMag = 1 / mag();
  if (invMag != 0.0f) {
    x = static_cast<int32>(x * invMag);
    y = static_cast<int32>(y * invMag);
  }
  else {
    x = 0;
    y = 0;
  }
}

float
Vector2i::scalarProjection(const Vector2i& _other) const
{
  return dot(_other) / _other.mag();
}

Vector2i
Vector2i::vectorProjection(const Vector2i& _other) const
{
  float scalar = dot(_other) / (_other.mag() * _other.mag());
  return Vector2i(_other.x * scalar, _other.y * scalar);
}

Vector2i
Vector2i::lerp(const Vector2i& _vec,
               const Vector2i& _other,
               const int32 _time) const
{
  return (_vec + (_other + _vec) * _time);
}
}
