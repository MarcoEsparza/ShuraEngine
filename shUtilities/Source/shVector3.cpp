/*************************************************************/
/*
*  @file    shVector3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
*  @brief   Vector3 with float
*
*  Vector3 with float
* 
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shVector3.h"
#include "shMath.h"

namespace shEngineSDK {
Vector3::Vector3(const Vector3& _other)
{
  x = _other.x;
  y = _other.y;
  z = _other.z;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector3::dot(const Vector3& _other) const
{
  return ((x * _other.x) + (y * _other.y) + (z * _other.z));
}

Vector3
Vector3::cross(const Vector3& _other) const
{
  return Vector3(y * _other.z - z * _other.y,
                 z * _other.x - x * _other.z,
                 x * _other.y - y * _other.x);
}

float
Vector3::mag() const
{
  float lenght = (x * x) + (y * y) + (z * z);

  return Math::sqrt(lenght);
}

void
Vector3::normalize()
{
  float invMag = 1 / mag();
  if (invMag != 0.0f) {
    x *= invMag;
    y *= invMag;
    z *= invMag;
  }
  else {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
}

Vector3
Vector3::getNormalized()
{
  float invMag = 1 / mag();
  if (invMag != 0.0f) {
    return Vector3(x * invMag,
                   y * invMag,
                   z * invMag);
  }
  else {
    return Vector3(0.0f,
                   0.0f,
                   0.0f);
  }
}

Vector3
Vector3::lerp(const Vector3& _other,
              const float& _time) const
{
  return *this + (_other - *this) * _time;
}
}
