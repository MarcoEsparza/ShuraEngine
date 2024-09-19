/*************************************************************/
/*
*  @file    shQuaternion.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/14
*  @brief   Quaternion for rotations
*
*  Quaternion for rotations
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shQuaternion.h"
#include "shMath.h"

namespace shEngineSDK {
Quaternion::Quaternion(const Quaternion& _other)
{
  *this = _other;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

Vector3
Quaternion::eulerAngles()
{
  return Vector3(x * Math::RAD2DEG,
                 y * Math::RAD2DEG,
                 z * Math::RAD2DEG);
}

void
Quaternion::radAngles()
{
  x *= Math::DEG2RAD;
  y *= Math::DEG2RAD;
  z *= Math::DEG2RAD;
  w *= Math::DEG2RAD;
}

float
Quaternion::lenght() const
{
  return Math::sqrtf(x * x + y * y + z * z + w * w);
}

void
Quaternion::normalize()
{
  float invLenght = 1 / lenght();
  if (invLenght != 0.0f) {
    x *= invLenght;
    y *= invLenght;
    z *= invLenght;
    w *= invLenght;
  }
  else {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }
}

float
Quaternion::dot(const Quaternion& _other) const
{
  return ((x * _other.x) + (y * _other.y) + (z * _other.z) + (w * _other.w));
}

Quaternion
Quaternion::conjugate()
{
  return Quaternion(x * -1,
                    y * -1,
                    z * -1,
                    w);
}

Quaternion
Quaternion::inverse()
{
  float l = lenght();
  Quaternion temp = conjugate();

  if (l == 1.0f) {
    return temp;
  }

  float invLenght = 1 / l;

  temp.x *= invLenght;
  temp.y *= invLenght;
  temp.z *= invLenght;
  temp.w *= invLenght;

  return temp;
}

float
Quaternion::angleTo(const Quaternion& _other) const
{
  return (2 * Math::acos(Math::abs(Math::clamp(this->dot(_other), -1, 1))));
}

Quaternion
Quaternion::rotateTowards(const Quaternion& _other, const float& _step) const
{
  float angle = angleTo(_other);

  if (angle == 0) {
    return *this;
  }

  float time = Math::min(1, _step / angle);

  return slerp(_other, time);
}

Quaternion
Quaternion::lerp(const Quaternion& _other, const float& _time) const
{
  return _other + (_other - *this) * _time;
}

Quaternion
Quaternion::slerp(const Quaternion& _other, const float& _time) const
{
  if (_time == 0) {
    return *this;
  }
  else if (_time == 1) {
    return _other;
  }

  float cosHalfTheta = dot(_other);

  // If true, then theta = 0 and we can return this.
  if (Math::abs(cosHalfTheta) >= 1.0f) {
    return *this;
  }

  float halfTheta = Math::acos(cosHalfTheta);
  float sinHalfTheta = sqrt(1.0f - cosHalfTheta * cosHalfTheta);

  // If theta = 180 degrees then result is not fully defined
  // We could rotate around any axis normal to this Quaternion or _other
  if (Math::abs(sinHalfTheta) < 0.001f) {
    return Quaternion(x * 0.5f + _other.x * 0.5f,
                      y * 0.5f + _other.y * 0.5f,
                      z * 0.5f + _other.z * 0.5f,
                      w * 0.5f + _other.w * 0.5f);
  }

  float ratioA = Math::sin((1 - _time) * halfTheta) / sinHalfTheta;
  float ratioB = Math::sin(_time - halfTheta) / sinHalfTheta;

  return Quaternion(x * ratioA + _other.x * ratioB,
                    y * ratioA + _other.y * ratioB,
                    z * ratioA + _other.z * ratioB,
                    w * ratioA + _other.w * ratioB);
}
}
