/*****************************************************************************/
/*
*  @file    shVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Vector3 with float.
*
*  Vector3 with float.
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
class Matrix3;

/**
*  @brief Vector3 with float.
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
  *  @param float x: X value.
  *  @param float y: Y value.
  *  @param float z: Z value.
  */
  Vector3(float x, float y, float z)
    : x(x), y(y), z(z)
  {}

  /**
  *  @brief Copy constructor
  *
  *  @param const Vector3& other: The Vector3 to be copied.
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

  /*
  *  @brief Calculates the distance between two Vector3s.
  *
  *  @param Vector3 a: First Vector3.
  *  @param Vector3 b: Second Vector3.
  * 
  *  @return float The distance between the two Vector3s.
  */
  static float
  distance(const Vector3& a, const Vector3& b);

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector3 values and other Vector3 values.
  *
  *  @param const Vector3& other: The Vector3 to sum with this Vector3.
  *
  *  @return Vector3: The result of the sum of the two Vector3s.
  */
  FORCEINLINE Vector3
  operator+(const Vector3& other) const;

  /**
  *  @brief Operator to sum a Vector3 values and a constant.
  *
  *  @param const float delta: The constant to sum with this Vector3.
  *
  *  @return Vector3: The result of the sum of the Vector3 and the constant.
  */
  FORCEINLINE Vector3
  operator+(const float delta) const;

  /**
  *  @brief Operator to substract a Vector3 values and other Vector3 values.
  *
  *  @param const Vector3& other: The Vector3 to substract from this Vector3.
  *
  *  @return Vector3: The result of the substraction of the two Vector3s.
  */
  FORCEINLINE Vector3
  operator-(const Vector3& other) const;

  /**
  *  @brief Change Vector3 values sign.
  *
  *  @return Vector3: The Vector3 with all values negated.
  */
  FORCEINLINE Vector3
  operator-() const;

  /**
  *  @brief Operator to multiply the values of a Vector3 with a float.
  *
  *  @param const float delta: The float to multiply with this Vector3.
  *
  *  @return Vector3: The result of the multiplication
  */
  FORCEINLINE Vector3
  operator*(const float delta) const;

  /**
  *  @brief Operator to multiply the values of a Vector3 with a Matrix3.
  * 
  *  @param const Matrix3& mat: The Matrix3 to multiply.
  * 
  *  @return Vector3 The result of the multiplication
  */
  Vector3
  operator*(const Matrix3& mat) const;

  /**
  *  @brief Operator to multiply the values of a Vector3 with another Vector3.
  *
  *  @param const Vector3& other: The Vector3 to multiply.
  *
  *  @return Vector3: The result of the multiplication
  */
  FORCEINLINE Vector3
  operator*(const Vector3& other) const;

  /**
  *  @brief Operator to divide the values of a Vector3 with a float.
  *
  *  @param const Vector3& other: The Vector3 to divide.
  *
  *  @return Vector3: The result of the division
  */
  FORCEINLINE Vector3
  operator/(const float delta) const;

  /**
  *  @brief Operator to sum a Vector3 values and other Vector3 values and store
  *         the result in the first Vector3.
  *
  *  @param const Vector3& other: The Vector3 to sum with this Vector3.
  * 
  *  @return Vector3&: This Vector3 with the result of the sum.
  */
  FORCEINLINE Vector3&
  operator+=(const Vector3& other);

  /**
  *  @brief Operator to substract a Vector3 values and other Vector3 values and
  *         store the result in the first Vector3.
  *
  *  @param const Vector3& other: The Vector3 to substract from this Vector3.
  * 
  *  @return Vector3&: This Vector3 with the result of the substraction.
  */
  FORCEINLINE Vector3&
  operator-=(const Vector3& other);

  /**
  *  @brief Operator to multiply the values of a Vector3 with another Vector3
  *
  *  @param const Vector3& other: The Vector3 to multiply from this Vector3.
  *
  *  @return Vector3&: This Vector3 with the result of the multiplication.
  */
  FORCEINLINE Vector3&
  operator*=(const Vector3& other);

  /**
  *  @brief Operator to check if two Vector3s are not equal.
  *
  *  @param const Vector3& other: The Vector3 to compare with this Vector3.
  *
  *  @return bool: True if the two Vector3s are not equal, false otherwise.
  */
  FORCEINLINE bool
  operator!=(const Vector3& other);

  /**
  *  @brief Operator to check if two Vector3s are equal.
  *
  *  @param const Vector3& other: The Vector3 to compare with this Vector3.
  *
  *  @return bool: True if the two Vector3s are equal, false otherwise.
  */
  FORCEINLINE bool
  operator==(const Vector3& other);

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

FORCEINLINE Vector3&
Vector3::operator*=(const Vector3& other)
{
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return *this;
}

FORCEINLINE bool
Vector3::operator!=(const Vector3& other)
{
  return x != other.x || y != other.y || z != other.z;
}

FORCEINLINE bool
Vector3::operator==(const Vector3& other)
{
  return x == other.x && y == other.y && z == other.z;
}
}
