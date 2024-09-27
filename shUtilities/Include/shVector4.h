/*************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Vector4 with float.
*
*  Vector4 with float.
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
*  Vector4 with float
*  
*  Sample usage:
*  Vector4 myVector4;
*  Vector4 myVector4(1.0f, 1.0f);
*  Vector4* ptrVector4 = new Vector4(); The constructor may take arguments.
*/
class SH_UTILITY_EXPORT Vector4
{
 public:
  /*
  *  Default constructor: initialize Vector4 values to 0.
  */
  Vector4() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  *
  *  @param float _x: X value.
  * 
  *  @param float _y: Y value.
  * 
  *  @param float _z: Z value.
  * 
  *  @param float _w: W value.
  */
  Vector4(float _x, float _y, float _z, float _w) :
          x(_x), y(_y), z(_z), w(_w) {}

  /*
  *  Copy constructor
  *
  *  @param Vector4
  */
  Vector4(const Vector4& _other);

  /*
  *  Default destructor.
  */
  ~Vector4() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Calculates the dot product of this Vector4 and a given Vector4.
  *
  *  @param Vector4 _other: To calculate with this Vector4.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector4& _other) const;

  /*
  *  Calculates the magnitude of the Vector4.
  *
  *  @param
  *
  *  @return float The magnitude of this Vector4.
  */
  float
  mag() const;

  /*
  *  Normalize this Vector4.
  *
  *  @param
  *
  *  @return void
  */
  void
  normalize();

  /*
  *  Get this Vector4 normalized.
  *
  *  @param
  *
  *  @return Vector4 The normalized Vector4.
  */
  Vector4
  getNormalized() const;

  /*
  *  Linear interpolation between two given Vector4.
  *  @brief This Vector4 is taken as the min value.
  *
  *  @param Vector4 _other: Max value.
  * 
  *  @param float _time: Time.
  *
  *  @return Vector4 Interpolated Vector4.
  */
  Vector4
  lerp(const Vector4& _vecB, const float& _time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /*
  *  Operator to sum a Vector4 values and other Vector4 values.
  *
  *  @param lValue-Vector4.
  * 
  *  @param rValue-Vector4.
  *
  *  @return Vector4.
  */
  FORCEINLINE Vector4
  operator+(const Vector4& other) const;

  /*
  *  Operator to substract a Vector4 values and other Vector4 values.
  *
  *  @param lValue-Vector4.
  * 
  *  @param rValue-Vector4.
  *
  *  @return Vector4.
  */
  FORCEINLINE Vector4
  operator-(const Vector4& other) const;

  /*
  *  Operator to multiply the values of a Vector4 with a float.
  *
  *  @param lValue-Vector4.
  * 
  *  @param rValue-float.
  *
  *  @return Vector4 The result of the multiplication.
  */
  FORCEINLINE Vector4
  operator*(const float& delta) const;

  /*
  *  Operator to sum a Vector4 values and other Vector4 values and store the
  *  result in the first Vector4.
  *
  *  @param lValue-Vector4.
  * 
  *  @param rValue-Vector4.
  * 
  *  @return Vector4 This.
  */
  FORCEINLINE Vector4&
  operator+=(const Vector4& other);

  /*
  *  Operator to substract a Vector4 values and other Vector4 values and
  *  store the result in the first Vector4.
  *
  *  @param lValue-Vector4.
  * 
  *  @param rValue-Vector4.
  * 
  *  @return Vector4 This.
  */
  FORCEINLINE Vector4&
  operator-=(const Vector4& other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   * Vector4 X value.
   */
   float x;
   /*
   * Vector4 Y value.
   */
   float y;
   /*
   * Vector4 Z value.
   */
   float z;
   /*
   * Vector4 W value.
   */
   float w;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Vector4
Vector4::operator+(const Vector4& other) const
{
  return Vector4(x + other.x,
    y + other.y,
    z + other.z,
    w + other.w);
}

FORCEINLINE Vector4
Vector4::operator-(const Vector4& other) const
{
  return Vector4(x - other.x,
    y - other.y,
    z - other.z,
    w - other.w);
}

FORCEINLINE Vector4
Vector4::operator*(const float& delta) const
{
  return Vector4(x + delta,
    y + delta,
    z + delta,
    w + delta);
}

FORCEINLINE Vector4&
Vector4::operator+=(const Vector4& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;

  return *this;
}

FORCEINLINE Vector4&
Vector4::operator-=(const Vector4& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;

  return *this;
}
}
