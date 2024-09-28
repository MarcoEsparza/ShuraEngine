/*************************************************************/
/*
*  @file    shVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
/*
*  Vector3 with float
* 
*  Sample usage:
*  Vector3 myVector3;
*  Vector3 myVector3(1.0f, 1.0f);
*  Vector3* ptrVector3 = new Vector3(); The constructor may take arguments.
*/
class SH_UTILITY_EXPORT Vector3
{
 public:
  /*
  *  Default constructor.
  */
  Vector3() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  *
  *  @param float _x: X value.
  * 
  *  @param float _y: Y value.
  * 
  *  @param float _z: Z value.
  */
  Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

  /*
  *  Copy constructor
  *
  *  @param Vector3 _other: The Vector3 to be copied.
  */
  Vector3(const Vector3& other);

  /*
  *  Default destructor.
  */
  ~Vector3() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Calculates the dot product of two given Vector3s.
  *
  *  @param Vector3 other: To calculate with this Vector3.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector3& other) const;

  /*
  * Calculates the cross product of two given Vector3s.
  *
  *  @param Vector3 other: To calculate with this Vector3.
  *
  *  @return Vector3 The result of the cross product.
  */
  Vector3
  cross(const Vector3& other) const;

  /*
  *  Calculates the magnitude of the Vector3.
  *
  *  @param
  *
  *  @return float The magnitude of this Vector3.
  */
  float
  mag() const;

  /*
  *  Normalize this Vector3.
  *
  *  @param
  *
  *  @return void
  */
  void
  normalize();

  /*
  *  Get this Vector3 normalized.
  *
  *  @param
  *
  *  @return Vector3 The normalized Vector3.
  */
  Vector3
  getNormalized() const;

  /*
  *  Linear interpolation between two given Vector3.
  *  @brief This Vector3 is taken as the min value.
  *
  *  @param Vector3 other: Max value.
  * 
  *  @param float time: Alpha.
  *
  *  @return Vector3 Interpolated Vector3.
  */
  Vector3
  lerp(const Vector3& other, const float time) const;

  /*
  *  Rotate Vector3 in X axis.
  *
  *  @param float angle
  *
  *  @return Vector3
  */
  Vector3
  rotateX(const float angle);

  /*
  *  Rotate Vector3 in Y axis.
  *
  *  @param float angle
  *
  *  @return Vector3
  */
  Vector3
  rotateY(const float angle);

  /*
  *  Rotate Vector3 in Z axis.
  *
  *  @param float angle
  *
  *  @return Vector3
  */
  Vector3
  rotateZ(const float angle);

  /*
  *  Rotate Vector3
  *
  *  @param Vector3 angles
  *
  *  @return Vector3 Rotation Vector3 in 3 axis
  */
  Vector3
  rotate(const Vector3& angles);

  /*
  *  This vector3 values to radians
  *  @note Function modify this
  *
  *  @param 
  *
  *  @return Vector3
  */
  Vector3
  toRadians();

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /*
  *  Operator to sum a Vector3 values and other Vector3 values
  *
  *  @param lValue-Vector3.
  * 
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator+(const Vector3& other) const;

  /*
  *  Operator to sum a Vector3 values and a constant
  *
  *  @param lValue-Vector3.
  * 
  *  @param rValue-float.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator+(const float delta) const;

  /*
  *  Operator to substract a Vector3 values and other Vector3 values
  *
  *  @param lValue-Vector3.
  * 
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator-(const Vector3& other) const;

  /*
  *  Operator to multiply the values of a Vector3 with a float
  *
  *  @param lValue-Vector3.
  * 
  *  @param rValue-float.
  *
  *  @return Vector3 The result of the multiplication
  */
  FORCEINLINE Vector3
  operator*(const float delta) const;

  /*
  *  Operator to sum a Vector3 values and other Vector3 values and store
  *  the result in the first Vector3
  *
  *  @param lValue-Vector3.
  * 
  *  @param rValue-Vector3.
  * 
  *  @return Vector3 This.
  */
  FORCEINLINE Vector3&
  operator+=(const Vector3& other);

  /*
  *  Operator to substract a Vector3 values and other Vector3 values and
  *  store the result in the first Vector3
  *
  *  @param lValue-Vector3.
  * 
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
   /*
   * Vector3 X value.
   */
   float x;
   /*
   * Vector3 Y value.
   */
   float y;
   /*
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
