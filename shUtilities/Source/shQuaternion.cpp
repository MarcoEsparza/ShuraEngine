/*****************************************************************************/
/*
*  @file    shQuaternion.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Quaternion for rotations
*
*  Quaternion for rotations
*
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shQuaternion.h"
#include "shMath.h"
#include "shRadian.h"
#include "shMatrix3.h"
#include "shMatrix4.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static Variables
*/
/*****************************************************************************/

/**
* @brief Identity Quaternion value.
*/
const Quaternion Quaternion::IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

/**
* @brief Zero Quaternion value.
*/
const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

/**
* @brief Unit Quaternion value.
*/
const Quaternion Quaternion::UNIT = Quaternion(0.0f, 1.0f, 0.0f, 0.0f);

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

Quaternion::Quaternion(const Vector3& vec)
{
  //fromAngle(vec);
  //normalize();
  *this = fromEulerAngles(vec);
}

Quaternion::Quaternion(const Vector3& axis, const float angle)
{
  float sina = Math::sin(Radian(angle * 0.5f));
  Vector3 normalizedAxis = axis.getNormalized();
  w = Math::cos(Radian(angle * 0.5f));
  x = sina * normalizedAxis.x;
  y = sina * normalizedAxis.y;
  z = sina * normalizedAxis.z;
}

Quaternion::Quaternion(const Vector3& from, const Vector3& to)
{
  w = 1.0f + to.dot(from);
  Vector3 cross = from.cross(to);
  x = cross.x;
  y = cross.y;
  z = cross.z;
  normalize();
}

Quaternion::Quaternion(const Matrix3& rotationMatrix)
{
  float trace = rotationMatrix.m[0][0] + rotationMatrix.m[1][1] + rotationMatrix.m[2][2];
  if (trace > 0.0f) {
    float s = Math::sqrt(trace + 1.0f) * 2.0f;
    w = 0.25f * s;
    x = (rotationMatrix.m[1][2] - rotationMatrix.m[2][1]) / s;
    y = (rotationMatrix.m[2][0] - rotationMatrix.m[0][2]) / s;
    z = (rotationMatrix.m[0][1] - rotationMatrix.m[1][0]) / s;
  }
  else if ((rotationMatrix.m[0][0] > rotationMatrix.m[1][1]) &&
           (rotationMatrix.m[0][0] > rotationMatrix.m[2][2])) {
    float s = Math::sqrt(1.0f + rotationMatrix.m[0][0] - rotationMatrix.m[1][1] -
                         rotationMatrix.m[2][2]) * 2.0f;
    w = (rotationMatrix.m[1][2] - rotationMatrix.m[2][1]) / s;
    x = 0.25f * s;
    y = (rotationMatrix.m[1][0] + rotationMatrix.m[0][1]) / s;
    z = (rotationMatrix.m[2][0] + rotationMatrix.m[0][2]) / s;
  }
  else if (rotationMatrix.m[1][1] > rotationMatrix.m[2][2]) {
    float s = Math::sqrt(1.0f + rotationMatrix.m[1][1] - rotationMatrix.m[0][0] -
                         rotationMatrix.m[2][2]) * 2.0f;
    w = (rotationMatrix.m[2][0] - rotationMatrix.m[0][2]) / s;
    x = (rotationMatrix.m[1][0] + rotationMatrix.m[0][1]) / s;
    y = 0.25f * s;
    z = (rotationMatrix.m[2][1] + rotationMatrix.m[1][2]) / s;
  }
  else {
    float s = Math::sqrt(1.0f + rotationMatrix.m[2][2] - rotationMatrix.m[0][0] -
                         rotationMatrix.m[1][1]) * 2.0f;
    w = (rotationMatrix.m[0][1] - rotationMatrix.m[1][0]) / s;
    x = (rotationMatrix.m[2][0] + rotationMatrix.m[0][2]) / s;
    y = (rotationMatrix.m[2][1] + rotationMatrix.m[1][2]) / s;
    z = 0.25f * s;
  }
}

Quaternion::Quaternion(const Quaternion& other)
{
  w = other.w;
  x = other.x;
  y = other.y;
  z = other.z;
}

Matrix3
Quaternion::operator*(const Matrix3& mat) const
{
  // Convert Quaternion to Matrix3
  const float p00 = 1.0f - (2.0f * (y * y)) - (2.0f * (z * z));
  const float p01 = (2.0f * (x * y)) + (2.0f * (w * z));
  const float p02 = (2.0f * (x * z)) - (2.0f * (w * y));
  const float p10 = (2.0f * (x * y)) - (2.0f * (w * z));
  const float p11 = 1.0f - (2.0f * (x * x)) - (2.0f * (z * z));
  const float p12 = (2.0f * (y * z)) + (2.0f * (w * x));
  const float p20 = (2.0f * (x * z)) + (2.0f * (w * y));
  const float p21 = (2.0f * (y * z)) - (2.0f * (w * x));
  const float p22 = 1.0f - (2.0f * (x * x)) - (2.0f * (y * y));
  return Matrix3(p00, p01, p02,
                 p10, p11, p12,
                 p20, p21, p22) * mat;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

Vector3
Quaternion::toEulerAngles() const
{
  Matrix3 rotMat = toMatrix3();
  return rotMat.toEulerDegrees();
}

Vector3
Quaternion::rotate(const Vector3& vec) const
{
  const Quaternion& P = *this;
  Quaternion q;
  q.x = P.w * vec.x + P.y * vec.z - P.z * vec.y;
  q.y = P.w * vec.y + P.z * vec.x - P.x * vec.z;
  q.z = P.w * vec.z + P.x * vec.y - P.y * vec.x;
  q.w = -P.x * vec.x - P.y * vec.y - P.z * vec.z;

  // Now we need to convert q to a vector3
  Vector3 result = Vector3::ZERO;
  result.x = q.w * -P.x + P.w * q.x - q.y * P.z + q.z * P.y;
  result.y = q.w * -P.y + P.w * q.y - q.z * P.x + q.x * P.z;
  result.z = q.w * -P.z + P.w * q.z - q.x * P.y + q.y * P.x;

  return result;
}

Vector3
Quaternion::invRotate(const Vector3& vec) const
{
  Quaternion inv = this->conjugate();
  return inv.rotate(vec);
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
  float cx = Math::cos(vec.x * 0.5f);
  float sx = Math::sin(vec.x * 0.5f);
  float cy = Math::cos(vec.y * 0.5f);
  float sy = Math::sin(vec.y * 0.5f);
  float cz = Math::cos(vec.z * 0.5f);
  float sz = Math::sin(vec.z * 0.5f);

  Quaternion q = Quaternion::ZERO;

  q.w = cy * cx * cz + sy * sx * sz;
  q.x = cy * sx * cz + sy * cx * sz;
  q.y = sy * cx * cz - cy * sx * sz;
  q.z = cy * cx * sz - sy * sx * cz;

  return q.getNormalized();
  /*Matrix3 rotMat(vec);
  return Quaternion(rotMat);*/
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
  return Math::sqrt(lenghtSquared());
}

float
Quaternion::lenghtSquared() const
{
  //return (Math::sqrt(w) + Math::sqrt(x) + Math::sqrt(y) + Math::sqrt(z));
  return w * w + x * x + y * y + z * z;
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
  Quaternion result = *this;
  result.normalize();
  return result;
}

float
Quaternion::dot(const Quaternion& other) const
{
  return ((x * other.x) + (y * other.y) + (z * other.z) + (w * other.w));
}

Quaternion
Quaternion::conjugate() const
{
  return Quaternion(w, -x, -y, -z);
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

Matrix3
Quaternion::toMatrix3() const
{
  /*const float p00 = 1.0f - (2.0f * (y * y)) - (2.0f * (z * z));
  const float p01 = (2.0f * (x * y)) + (2.0f * (w * z));
  const float p02 = (2.0f * (x * z)) - (2.0f * (w * y));
  const float p10 = (2.0f * (x * y)) - (2.0f * (w * z));
  const float p11 = 1.0f - (2.0f * (x * x)) - (2.0f * (z * z));
  const float p12 = (2.0f * (y * z)) + (2.0f * (w * x));
  const float p20 = (2.0f * (x * z)) + (2.0f * (w * y));
  const float p21 = (2.0f * (y * z)) - (2.0f * (w * x));
  const float p22 = 1.0f - (2.0f * (x * x)) - (2.0f * (y * y));
  return Matrix3(p00, p01, p02,
                 p10, p11, p12,
                 p20, p21, p22);*/

  /*Vector3 right = rotate(Vector3::RIGHT);
  Vector3 up = rotate(Vector3::UP);
  Vector3 forward = rotate(Vector3::FORWARD);

  return Matrix3(right, up, forward);*/

  Quaternion q = getNormalized();

  float xx = q.x * q.x;
  float yy = q.y * q.y;
  float zz = q.z * q.z;

  float xy = q.x * q.y;
  float xz = q.x * q.z;
  float yz = q.y * q.z;

  float wx = q.w * q.x;
  float wy = q.w * q.y;
  float wz = q.w * q.z;

  return Matrix3(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy),
                 2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx),
                 2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy));
}

Quaternion
Quaternion::fromBivector(const Vector3& vector)
{
  float angle = vector.mag();
  if (angle < Math::SMALL_NUMBER) {
    return Quaternion::IDENTITY;
  }

  Vector3 axis = vector / angle;
  float halfAngle = angle * 0.5f;
  float sina = Math::sin(Radian(halfAngle));
  float cosa = Math::cos(Radian(halfAngle));

  return Quaternion(cosa, axis.x * sina, axis.y * sina, axis.z * sina);
}
}
