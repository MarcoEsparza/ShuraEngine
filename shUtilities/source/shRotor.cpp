/*************************************************************/
/*
*  @file    shRotor.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Rotor class for geometric algebra.
*
*  Rotor class for geometric algebra.
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shRotor.h"
#include "shMath.h"
#include "shVector3.h"
#include "shMatrix3.h"
#include "shRadian.h"

namespace shEngineSDK {
Rotor::Rotor(const Bivector& bvPlane, const float angle)
{
  float sina = Math::sin(Radian(angle * 0.5f));
  w = Math::cos(Radian(angle * 0.5f));
  // The left side of the product have b a, not a b
  bx = -sina * bvPlane.yz;
  by = -sina * bvPlane.zx;
  bz = -sina * bvPlane.xy;
}

Rotor::Rotor(const Vector3& from, const Vector3& to)
{
  w = 1.0f + to.dot(from);
  // The left side of the product have b a, not a b, so we flip the order
  Bivector minusb;
  minusb = minusb.wedge(from, to);
  bx = minusb.yz;
  by = minusb.zx;
  bz = minusb.xy;
  normalize();
}

Rotor
Rotor::reverse() const
{
  return Rotor(w, -bx, -by, -bz);
}

float
Rotor::lenghtSquared() const
{
  return (Math::sqrt(w) + Math::sqrt(bx) + Math::sqrt(by) + Math::sqrt(bz));
}

float
Rotor::lenght() const
{
  return Math::sqrt(lenghtSquared());
}

void
Rotor::normalize()
{
  const float invLength = 1.0f / lenght();
  w *= invLength;
  bx *= invLength;
  by *= invLength;
  bz *= invLength;
}

Rotor
Rotor::getNormalized() const
{
  Rotor result = *this;
  result.normalize();
  return result;
}

Vector3
Rotor::rotate(const Vector3& vector) const
{
  const Rotor& P = *this;

  // q = P * vector
  Vector3 q = Vector3(P.w * vector.x + vector.y * P.bx + vector.z * P.by,
                      P.w * vector.y - vector.x * P.bx + vector.z * P.bz,
                      P.w * vector.z - vector.x * P.by - vector.y * P.bz);

  // Trivector part of the product
  float qBxyz = vector.x * P.bz - vector.y * P.by + vector.z * P.bx; 

  // result = q P*
  Vector3 result = Vector3(P.w * q.x + q.y * P.bx + q.z * P.by + qBxyz * P.bz,
                           P.w * q.y - q.x * P.bx - qBxyz * P.by + q.z * P.bz,
                           P.w * q.z + qBxyz * P.bx - q.x * P.by - q.y * P.bz);

  // Trivector part of the result is always zero, so we can ignore it

  return result;
}

Rotor
Rotor::rotate(const Rotor& r) const
{
  return (*this * r * (*this).reverse());
}

Matrix3
Rotor::toMatrix3() const
{
  Vector3 right = rotate(Vector3::RIGHT);
  Vector3 up = rotate(Vector3::UP);
  Vector3 forward = rotate(Vector3::FORWARD);
  return Matrix3(right, up, forward);
}

Rotor
Rotor::geo(const Vector3& vec1, const Vector3& vec2) const
{
  Bivector bvPlane;
  bvPlane = bvPlane.wedge(vec1, vec2);
  return Rotor(vec1.dot(vec2), bvPlane);
}
}
