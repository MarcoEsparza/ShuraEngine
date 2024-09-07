/*************************************************************/
/*
*  @file    shVector2.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector2
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
#include "Math/shVector2.h"
#include "Math/shPlatformMath.h"

namespace shEngineSDK {
Vector2::Vector2()
{
  x = 0;
  y = 0;
}

Vector2::Vector2(float _x, float _y)
{
  x = _x;
  y = _y;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector2::dot(const Vector2& other) const
{
  return (x * other.x + y * other.y);
}

float
Vector2::mag() const
{
  return PlatformMath::sqrtf(x * x + y * y);
}

void
Vector2::normalize()
{
  float magnitude = mag();
  if (magnitude != 0.0f)
  {
    x /= magnitude;
    y /= magnitude;
  }
  else
  {
    x = 0.0f;
    y = 0.0f;
  }
}

float
Vector2::scalarProjection(const Vector2& other) const
{
  return dot(other) / other.mag();
}

Vector2
Vector2::vectorProjection(const Vector2& other) const
{
  float scalar = dot(other) / (other.mag() * other.mag());
  return Vector2(other.x * scalar, other.y * scalar);
}

Vector2
Vector2::lerp(const Vector2& vecA,
              const Vector2& vecB,
              const float time) const
{
  return (vecA + (vecB + vecA) * time);
}
}
