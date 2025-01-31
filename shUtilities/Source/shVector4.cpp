/*************************************************************/
/*
*  @file    shVector4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
#include "shVector3.h"
#include "shMath.h"

namespace shEngineSDK {
Vector4::Vector4(const Vector3& xyz, const float _w)
{
  x = xyz.x;
  y = xyz.y;
  z = xyz.z;
  w = _w;
}

Vector4::Vector4(const Vector4& other)
{
  x = other.x;
  y = other.y;
  z = other.z;
  w = other.w;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector4::dot(const Vector4& other) const
{
  return ((x * other.x) + (y * other.y) + (z * other.z) + (w * other.w));
}

float
Vector4::mag() const
{
  const float lenght = (x * x) + (y * y) + (z * z) + (w * w);

  return Math::sqrt(lenght);
}

void
Vector4::normalize()
{
  const float invMag = 1 / mag();
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
Vector4::getNormalized() const
{
  const float invMag = 1 / mag();
  if (invMag != 0.0f) {
    
    return Vector4(x * invMag,
                   y * invMag,
                   z * invMag,
                   w * invMag);
  }
  else {
    return Vector4(0.0f,
                   0.0f,
                   0.0f,
                   0.0f);
  }
}

Vector4
Vector4::lerp(const Vector4& other,
              const float time) const
{
  return *this + (other - *this) * time;
}
}
