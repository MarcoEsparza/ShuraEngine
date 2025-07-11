/*****************************************************************************/
/*
*  @file    shVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/05
*  @brief   Vector3 with float
*
*  Vector3 with float
* 
*  @bug     No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
class Radian;

/**
*  @brief Vector3 with float
* 
*  @brief Sample usage:
*  @brief Vector3 myVector3;
*  @brief Vector3 myVector3(1.0f, 1.0f);
*  @brief Vector3* ptrVector3 = new Vector3(); The constructor may take arguments.
*/
class SH_UTILITY_EXPORT Vector3
{
 public:
  /**
  *  Default constructor.
  */
  Vector3() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  *
  *  @param float _x: X value.
  *  @param float _y: Y value.
  *  @param float _z: Z value.
  */
  Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

  /**
  *  @brief Copy constructor
  *
  *  @param Vector3 _other: The Vector3 to be copied.
  */
  Vector3(const Vector3& other);

  /**
  *  @brief Default destructor.
  */
  ~Vector3() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/

  /**
  *  @brief Calculates the dot product of two given Vector3s.
  *
  *  @param Vector3 other: To calculate with this Vector3.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector3& other) const;

  /**
  *  @brief Calculates the cross product of two given Vector3s.
  *
  *  @param Vector3 other: To calculate with this Vector3.
  *
  *  @return Vector3 The result of the cross product.
  */
  Vector3
  cross(const Vector3& other) const;

  /**
  *  @brief Calculates the magnitude of the Vector3.
  *
  *  @return float The magnitude of this Vector3.
  */
  float
  mag() const;

  /**
  *  @brief Normalize this Vector3.
  */
  void
  normalize();

  /**
  *  @brief Get this Vector3 normalized.
  *
  *  @return Vector3 The normalized Vector3.
  */
  Vector3
  getNormalized() const;

  /**
  *  @brief Linear interpolation between two given Vector3.
  *  @brief This Vector3 is taken as the min value.
  *
  *  @param Vector3 other: Max value.
  *  @param float time: Alpha.
  *
  *  @return Vector3 Interpolated Vector3.
  */
  Vector3
  lerp(const Vector3& other, const float time) const;

  /**
  *  @brief Clamp this Vector with other 2 Vectors as min and max.
  *
  *  @param Vector3 min: Max value.
  *  @param Vector3 max: Min value.
  *
  *  @return Vector3 Interpolated Vector3.
  */
  Vector3
  clamp(const Vector3& min, const Vector3& max) const;

  /**
  *  @brief Vector3 lenght.
  */
  float
  lenght() const;

  /**
  *  @brief Vector3 squared lenght.
  */
  float
  lenghtSq() const;

  /**
  *  @brief Rotate Vector3 in X axis.
  *
  *  @param Radian angle
  *
  *  @return Vector3
  */
  Vector3
  rotateX(const Radian angle);

  /**
  *  @brief Rotate Vector3 in Y axis.
  *
  *  @param Radian angle
  *
  *  @return Vector3
  */
  Vector3
  rotateY(const Radian angle);

  /**
  *  @brief Rotate Vector3 in Z axis.
  *
  *  @param Radian angle
  *
  *  @return Vector3
  */
  Vector3
  rotateZ(const Radian angle);

  /**
  *  @brief Rotate Vector3.
  *
  *  @param Vector3 angles
  *
  *  @return Vector3 Rotation Vector3 in 3 axis.
  */
  Vector3
  rotate(const Vector3& angles);

  /**
  *  @brief This vector3 values to radians.
  *  @note Function modify this.
  *
  *  @return Vector3
  */
  Vector3
  toRadians();

  /*
  *  @brief Computes the closest point betwwen two given Vector3 to this.
  * 
  *  @param Vector3& A
  *  @param Vector3& B
  * 
  *  @return Vector3
  */
  Vector3
  closestPointOnSegment(const Vector3& A, const Vector3& B) const;

  /*
  *  @brief Returns true if all components are zero.
  *
  *  @return bool
  */
  bool
  isZero() const;

  /*
  *  @brief Returns true if a component is NaN.
  *
  *  @return bool
  */
  bool
  containsNaN() const;

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector3 values and other Vector3 values.
  *
  *  @param lValue-Vector3.
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator+(const Vector3& other) const;

  /**
  *  @brief Operator to sum a Vector3 values and a constant.
  *
  *  @param lValue-Vector3.
  *  @param rValue-float.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator+(const float delta) const;

  /**
  *  @brief Operator to substract a Vector3 values and other Vector3 values.
  *
  *  @param lValue-Vector3.
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator-(const Vector3& other) const;

  /**
  *  @brief Change Vector3 values sign.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator-() const;

  /**
  *  @brief Operator to multiply the values of a Vector3 with a float.
  *
  *  @param lValue-Vector3.
  *  @param rValue-float.
  *
  *  @return Vector3 The result of the multiplication
  */
  FORCEINLINE Vector3
  operator*(const float delta) const;

  /**
  *  @brief Operator to multiply the values of a Vector3 with another Vector3.
  *
  *  @param Vector3: The Vector3 to multiply.
  *
  *  @return Vector3 The result of the multiplication
  */
  FORCEINLINE Vector3
  operator*(const Vector3& other) const;

  /**
  *  @brief Operator to divide the values of a Vector3 with a float.
  *
  *  @param lValue-Vector3.
  *  @param rValue-float.
  *
  *  @return Vector3 The result of the division
  */
  FORCEINLINE Vector3
  operator/(const float delta) const;

  /**
  *  @brief Operator to sum a Vector3 values and other Vector3 values and store
  *         the result in the first Vector3.
  *
  *  @param lValue-Vector3.
  *  @param rValue-Vector3.
  * 
  *  @return Vector3 This.
  */
  FORCEINLINE Vector3&
  operator+=(const Vector3& other);

  /**
  *  @brief Operator to substract a Vector3 values and other Vector3 values and
  *         store the result in the first Vector3.
  *
  *  @param lValue-Vector3.
  *  @param rValue-Vector3.
  * 
  *  @return Vector3 This.
  */
  FORCEINLINE Vector3&
  operator-=(const Vector3& other);

  FORCEINLINE bool
  operator!=(const Vector3& other);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  * @brief Vector3 X value.
  */
  float x;

  /**
  * @brief Vector3 Y value.
  */
  float y;

  /**
  * Vector3 Z value.
  */
  float z;

  /***************************************************************************/
  /*
  *  Static Variables
  */
  /***************************************************************************/
 public:
   /**
   * Vector3 Forward vector = 0.0f, 0.0f, 1.0f.
   */
   static const Vector3 FORWARD;

   /**
   * Vector3 Up vector = 0.0f, 1.0f, 0.0f.
   */
   static const Vector3 UP;

   /**
   * Vector3 Right vector = 1.0f, 0.0f, 0.0f.
   */
   static const Vector3 RIGHT;

   /**
   * Vector3 Zero vector = 0.0f, 0.0f, 0.0f.
   */
   static const Vector3 ZERO;

   /**
   * Vector3 One vector = 1.0f, 1.0f, 1.0f.
   */
   static const Vector3 ONE;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE Vector3
Vector3::operator+(const Vector3& other) const
{
  return Vector3(x + other.x, y + other.y, z + other.z);
}
FORCEINLINE Vector3
Vector3::operator+(const float delta) const
{
  return Vector3(x + delta, y + delta, z + delta);
}

FORCEINLINE Vector3
Vector3::operator-(const Vector3& other) const
{
  return Vector3(x - other.x, y - other.y, z - other.z);
}

FORCEINLINE Vector3
Vector3::operator-() const
{
  return Vector3(-x, -y, -z);
}

FORCEINLINE Vector3
Vector3::operator*(const float delta) const
{
  return Vector3(x * delta, y * delta, z * delta);
}

FORCEINLINE Vector3
Vector3::operator*(const Vector3& other) const
{
  return Vector3(x * other.x, y * other.y, z * other.z);
}

FORCEINLINE Vector3
Vector3::operator/(const float delta) const
{
  if (delta == 0.0f)
  {
    // Handle division by zero, could throw an exception or return a zero vector
    return Vector3(0.0f, 0.0f, 0.0f);
  }
  return Vector3(x / delta, y / delta, z / delta);
}

FORCEINLINE Vector3&
Vector3::operator+=(const Vector3& other)
{
  x += other.x;
  y += other.y;
  z += other.z;

  return *this;
}

FORCEINLINE Vector3&
Vector3::operator-=(const Vector3& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;

  return *this;
}

FORCEINLINE bool
Vector3::operator!=(const Vector3& other)
{
  return x != other.x || y != other.y || z != other.z;
}
}
