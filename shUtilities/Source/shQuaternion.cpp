/*************************************************************/
/*
*  @file    shQuaternion.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
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
#include "shRadian.h"

namespace shEngineSDK {
Quaternion::Quaternion(const Vector3& vec)
{
  fromAngle(vec);
}

Quaternion::Quaternion(const Vector3& axis, const float angle)
{
  w = angle;
  x = axis.x;
  y = axis.y;
  z = axis.z;
}

Quaternion::Quaternion(const Quaternion& other)
{
  w = other.w;
  x = other.x;
  y = other.y;
  z = other.z;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

Vector3
Quaternion::toEulerAngles() const
{
  const float theta = (Math::acos(Radian(w))) * 2;
  const Radian radTheta(theta);
  const Radian acosTheta(Math::acos(radTheta));
  const float inv = 1 / (Math::sin(acosTheta));

  return Vector3(x * inv,
                 y * inv,
                 z * inv);
}

Vector3
Quaternion::toRotate(const Vector3& vec) const
{
  Quaternion qv(0.0f, vec.x, vec.y, vec.z);
  Quaternion res = (*this * qv) * conjugate();

  return Vector3(res.x, res.y, res.z);
}

void
Quaternion::toAxes(Vector3& right,
                   Vector3& up,
                   Vector3& forward) const
{
  right = Vector3(1 - 2 * (y * y + z * z), 2 * (x * y - w * z), 2 * (x * z + w * y));
  up = Vector3(2 * (x * y + w * z), 1 - 2 * (x * x + z * z), 2 * (y * z - w * x));
  forward = Vector3(2 * (x * z - w * y), 2 * (y * z + w * x), 1 - 2 * (x * x + y * y));
}

Quaternion
Quaternion::fromEulerAngles(const Vector3& vec) const
{
  const float cr = Math::cos(Radian(vec.x * 0.5f));  // Cosine Roll
  const float sr = Math::sin(Radian(vec.x * 0.5f));  // Sine Roll
  const float cp = Math::cos(Radian(vec.y * 0.5f));  // Cosine Pitch
  const float sp = Math::sin(Radian(vec.y * 0.5f));  // Sine Pich
  const float cy = Math::cos(Radian(vec.z * 0.5f));  // Cosine Yaw
  const float sy = Math::sin(Radian(vec.z * 0.5f));  // Sine Yaw

  return Quaternion((cr * cp * cy) + (sr * sp * sy),
                    (sr * cp * cy) - (cr * sp * sy),
                    (cr * sp * cy) + (sr * cp * sy),
                    (cr * cp * sy) - (sr * sp * cy));
}

void
Quaternion::fromAngle(Vector3 vec)
{
  const float cr = Math::cos(Radian(vec.x * 0.5f));
  const float sr = Math::sin(Radian(vec.x * 0.5f));
  const float cp = Math::cos(Radian(vec.y * 0.5f));
  const float sp = Math::sin(Radian(vec.y * 0.5f));
  const float cy = Math::cos(Radian(vec.z * 0.5f));
  const float sy = Math::sin(Radian(vec.z * 0.5f));

  w = (cr * cp * cy) + (sr * sp * sy);
  x = (sr * cp * cy) - (cr * sp * sy);
  y = (cr * sp * cy) + (sr * cp * sy);
  z = (cr * cp * sy) - (sr * sp * cy);
}

float
Quaternion::lenght() const
{
  return Math::sqrt(x * x + y * y + z * z + w * w);
}

void
Quaternion::normalize()
{
  const float invLenght = 1 / lenght();
  if (invLenght != 0.0f) {
    w *= invLenght;
    x *= invLenght;
    y *= invLenght;
    z *= invLenght;
  }
  else {
    w = 0.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
}

Quaternion
Quaternion::getNormalized() const
{
  const float invLenght = 1 / lenght();
  if (invLenght != 0.0f) {
    return Quaternion(w * invLenght,
                      x * invLenght,
                      y * invLenght,
                      z * invLenght);
  }
  else {
    return Quaternion(0.0f,
                      0.0f,
                      0.0f,
                      0.0f);
  }
}

float
Quaternion::dot(const Quaternion& other) const
{
  return ((x * other.x) + (y * other.y) + (z * other.z) + (w * other.w));
}

Quaternion
Quaternion::conjugate() const
{
  return Quaternion(w,
                    -x,
                    -y,
                    -z);
}

Quaternion
Quaternion::inverse()
{
  const float l = lenght();
  Quaternion temp = conjugate();

  if (l == 1.0f) {
    return temp;
  }

  const float invLenght = 1 / l;

  temp.x *= invLenght;
  temp.y *= invLenght;
  temp.z *= invLenght;
  temp.w *= invLenght;

  return temp;
}

float
Quaternion::angleTo(const Quaternion& other) const
{
  const Radian rad(Math::abs(Math::clamp(dot(other), -1.0f, 1.0f)));
  return (2.0f * Math::acos(rad));
}

Quaternion
Quaternion::rotateTowards(const Quaternion& other, const float step) const
{
  const float angle = angleTo(other);

  if (angle == 0) {
    return *this;
  }

  const float time = Math::min(1, step / angle);

  return slerp(other, time);
}

Quaternion
Quaternion::lerp(const Quaternion& other, const float time) const
{
  return other + (other - *this) * time;
}

Quaternion
Quaternion::slerp(const Quaternion& other, const float time) const
{
  if (time == 0) {
    return *this;
  }
  else if (time == 1) {
    return other;
  }

  const float cosHalfTheta = dot(other);

  // If true, then theta = 0 and we can return this.
  if (Math::abs(cosHalfTheta) >= 1.0f) {
    return *this;
  }

  const float halfTheta = Math::acos(Radian(cosHalfTheta));
  const float sinHalfTheta = Math::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

  // If theta = 180 degrees then result is not fully defined
  // We could rotate around any axis normal to this Quaternion or _other
  if (Math::abs(sinHalfTheta) < 0.001f) {
    return Quaternion(x * 0.5f + other.x * 0.5f,
                      y * 0.5f + other.y * 0.5f,
                      z * 0.5f + other.z * 0.5f,
                      w * 0.5f + other.w * 0.5f);
  }

  const float ratioA = Math::sin(Radian((1.0f - time) * halfTheta)) / sinHalfTheta;
  const float ratioB = Math::sin(Radian(time - halfTheta)) / sinHalfTheta;

  return Quaternion(x * ratioA + other.x * ratioB,
                    y * ratioA + other.y * ratioB,
                    z * ratioA + other.z * ratioB,
                    w * ratioA + other.w * ratioB);
}
}
