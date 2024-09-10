/*************************************************************/
/*
*  @file    shVector2i.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
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
Vector2i::Vector2i(int32 _x, int32 _y)
{
  x = _x;
  y = _y;
}

Vector2i::Vector2i(const Vector2i& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector2i::dot(const Vector2i& vec, const Vector2i& other) const
{
  return (vec.x * other.x + vec.y * other.y);
}

float
Vector2i::mag() const
{
  return Math::sqrtf(x * x + y * y);
}

Vector2i
Vector2i::lerp(const Vector2i& vec,
               const Vector2i& other,
               const float time) const
{
  return (vec + (other + vec) * time);
}
}
