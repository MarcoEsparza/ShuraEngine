/*************************************************************/
/*
*  @file    shVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
*  @brief   Vector3 with float
*
*  Vector3 with float
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
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

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

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
  *  @breif Computes the closest point betwwen two given Vector3 to this.
  * 
  *  @param Vector3& other
  * 
  *  @return Vector3
  */
  Vector3
  closestPointOnSegment(const Vector3& vec1, const Vector3& vec2) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
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

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
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
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

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
Vector3::operator*(const float delta) const
{
  return Vector3(x * delta, y * delta, z * delta);
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
}
