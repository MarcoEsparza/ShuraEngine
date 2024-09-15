/*************************************************************/
/*
*  @file    shVector2.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/15
*  @brief   Vector2 with float
*
*  Vector2 with float
* 
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shVector2.h"
#include "shMath.h"

namespace shEngineSDK {
Vector2::Vector2(const Vector2& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector2::dot(const Vector2& _other) const
{
  return (x * _other.x + y * _other.y);
}

float
Vector2::mag() const
{
  return Math::sqrtf(x * x + y * y);
}

void
Vector2::normalize()
{
  float invMag = 1/mag();
  if (invMag != 0.0f) {
    x *= invMag;
    y *= invMag;
  }
  else {
    x = 0.0f;
    y = 0.0f;
  }
}

float
Vector2::scalarProjection(const Vector2& _other) const
{
  return dot(_other) / _other.mag();
}

Vector2
Vector2::vectorProjection(const Vector2& _other) const
{
  float scalar = dot(_other) / (_other.mag() * _other.mag());
  return Vector2(_other.x * scalar, _other.y * scalar);
}

Vector2
Vector2::lerp(const Vector2& _other,
              const float& _time) const
{
  return (*this + (_other + *this) * _time);
}
}
