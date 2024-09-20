/*************************************************************/
/*
*  @file    shVector2i.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
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
  x = _other.x;
  y = _other.y;
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

Vector2i
Vector2i::lerp(const Vector2i& _other,
               const int32& _time) const
{
  return (*this + (_other + *this) * _time);
}
}
