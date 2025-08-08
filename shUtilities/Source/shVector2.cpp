/*****************************************************************************/
/*
*  @file    shVector2.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Vector2 with float
*
*  Vector2 with float
* 
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shVector2.h"
#include "shMath.h"

namespace shEngineSDK {
Vector2::Vector2(const Vector2& other)
{
  x = other.x;
  y = other.y;
}

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/


float
Vector2::dot(const Vector2& other) const
{
  return (x * other.x + y * other.y);
}

float
Vector2::cross(const Vector2& other)
{
  return x * other.y - y * other.x;
}

float
Vector2::mag() const
{
  return Math::sqrt(x * x + y * y);
}

void
Vector2::normalize()
{
  const float invMag = 1/mag();
  if (invMag != 0.0f) {
    x *= invMag;
    y *= invMag;
  }
  else {
    x = 0.0f;
    y = 0.0f;
  }
}

Vector2
Vector2::getNormalized() const
{
  const float invMag = 1 / mag();
  if (invMag != 0.0f) {
    
    return Vector2(x * invMag,
                   y * invMag);
  }
  else {
    return Vector2(0.0f,
                   0.0f);
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
  const float scalar = dot(other) / (other.mag() * other.mag());
  return Vector2(other.x * scalar, other.y * scalar);
}

Vector2
Vector2::lerp(const Vector2& other,
              const float time) const
{
  return (*this + (other + *this) * time);
}
}
