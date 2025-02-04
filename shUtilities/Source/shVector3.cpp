/*****************************************************************************/
/*
*  @file    shVector3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/29
*  @brief   Vector3 with float
*
*  Vector3 with float
* 
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shVector3.h"
#include "shMath.h"
#include "shRadian.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static Variables
*/
/*****************************************************************************/
const Vector3 Vector3::FORWARD = { 0.0f, 0.0f, 1.0f };
const Vector3 Vector3::UP = { 0.0f, 1.0f, 0.0f };
const Vector3 Vector3::RIGHT = { 1.0f, 0.0f, 0.0f };
const Vector3 Vector3::ZERO = { 0.0f, 0.0f, 0.0f };

Vector3::Vector3(const Vector3& other)
{
  x = other.x;
  y = other.y;
  z = other.z;
}

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/


float
Vector3::dot(const Vector3& other) const
{
  return ((x * other.x) + (y * other.y) + (z * other.z));
}

Vector3
Vector3::cross(const Vector3& other) const
{
  return Vector3(y * other.z - z * other.y,
                 z * other.x - x * other.z,
                 x * other.y - y * other.x);
}

float
Vector3::mag() const
{
  const float lenght = (x * x) + (y * y) + (z * z);

  if (lenght != 0) {
    return Math::sqrt(lenght);
  }
  
  return 0.0f;
}

void
Vector3::normalize()
{
  const float invMag = 1 / mag();
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
Vector3::getNormalized() const
{
  const float vecMag = mag();
  if(vecMag != 0.0f) {
    const float invMag = 1 / vecMag;
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
  else {
    return Vector3(0.0f,
                   0.0f,
                   0.0f);
  }
}

Vector3
Vector3::lerp(const Vector3& _other,
              const float _time) const
{
  return *this + (_other - *this) * _time;
}

Vector3
Vector3::rotateX(const Radian angle)
{
  const float cosA = Math::cos(angle);
  const float sinA = Math::sin(angle);

  return Vector3(x,
                 (y * cosA - z * sinA),
                 (y * sinA + z * cosA));
}

Vector3
Vector3::rotateY(const Radian angle)
{
  const float cosA = Math::cos(angle);
  const float sinA = Math::sin(angle);

  return Vector3((x * cosA + z * sinA),
                 y,
                 (-x * sinA + z * cosA));
}

Vector3
Vector3::rotateZ(const Radian angle)
{
  const float cosA = Math::cos(angle);
  const float sinA = Math::sin(angle);

  return Vector3((x * cosA - y * sinA),
                 (x * sinA + y * cosA),
                 z);
}

Vector3
Vector3::rotate(const Vector3& angles)
{
  Vector3 resX = rotateX(Radian(angles.x));
  Vector3 resY = resX.rotateY(Radian(angles.y));
  Vector3 resZ = resY.rotateZ(Radian(angles.z));

  return resZ;
}

Vector3
Vector3::toRadians()
{
  return Vector3(x * Math::DEG2RAD,
                 y * Math::DEG2RAD, 
                 z * Math::DEG2RAD);
}

Vector3
Vector3::closestPointOnSegment(const Vector3& vec1, const Vector3& vec2) const
{
  const Vector3 abVec = vec2 - vec1;
  const float t = (dot(abVec) / abVec.dot(abVec));

  if (t < 0) {
    return vec1;
  }
  else if (t > 1) {
    return vec2;
  }

  return Vector3((vec1.x + t * abVec.x),
                 (vec1.y + t * abVec.y),
                 (vec1.z + t * abVec.z));
}

bool
Vector3::isZero() const
{
  return ((x == 0.0f) && (y == 0.0f) && (z == 0.0f));
}

bool
Vector3::containsNaN() const
{
  return ((Math::is_NaN(x)) && (Math::is_NaN(y)) && (Math::is_NaN(z)));
}
}
