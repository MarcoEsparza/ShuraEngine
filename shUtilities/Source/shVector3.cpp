/*************************************************************/
/*
*  @file    shVector3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
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
  *this = _other;
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
Vector3::fastReverseSqrt(const float& _num) const
{
  long i;
  float x2, _y;
  const float threehalfs = 1.5F;

  x2 = _num * 0.5F;
  _y = _num;
  i = *(long*)&_y;
  i = 0x5f3759df - (i >> 1);
  _y = *(float*)&i;
  _y = _y * (threehalfs - (x2 * _y * _y));

  return _y;
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
Vector3::lerp(const Vector3& _vecA,
              const Vector3& _vecB,
              float _time) const
{
  return _vecA + (_vecB - _vecA) * _time;
}
}
