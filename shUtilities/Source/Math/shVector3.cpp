/*************************************************************/
/*
*  @file    shVector3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector3
*
*  float Vector3
* 
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "Math/shVector3.h"
#include "Math/shPlatformMath.h"

namespace shEngineSDK {
Vector3::Vector3()
{
  x = 0;
  y = 0;
  z = 0;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector3::dot(const Vector3& vec, const Vector3& other) const
{
  return ((vec.x * other.x) + (vec.y * other.y) + (vec.z * other.z));
}

Vector3
Vector3::cross(const Vector3& vec, const Vector3& other) const
{
  return Vector3(y * other.z - z * other.y,
                 z * other.x - x * other.z,
                 x * other.y - y * other.x);
}

float
Vector3::fastReverseSqrt(const float& num) const
{
  long i;
  float x2, _y;
  const float threehalfs = 1.5F;

  x2 = num * 0.5F;
  _y = num;
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

  return PlatformMath::sqrt(lenght);
}

void
Vector3::normalize()
{
  float magnitude = mag();
  if (magnitude != 0.0f)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
  }
  else
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
}

Vector3
Vector3::lerp(const Vector3& vecA,
              const Vector3& vecB,
              float time) const
{
  return vecA + (vecB - vecA) * time;
}
}
