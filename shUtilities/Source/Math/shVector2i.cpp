/*************************************************************/
/*
*  @file    shVector2i.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector2i
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
#include "Math/shVector2i.h"
#include "Math/shPlatformMath.h"

namespace shEngineSDK {
Vector2i::Vector2i()
{
  x = 0;
  y = 0;
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
  return PlatformMath::sqrtf(x * x + y * y);
}

Vector2i
Vector2i::lerp(const Vector2i& vec,
               const Vector2i& other,
               const float time) const
{
  return (vec + (other + vec) * time);
}
}
