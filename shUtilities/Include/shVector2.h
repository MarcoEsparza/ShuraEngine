/*************************************************************/
/*
*  @file    shVector2.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Vector2 with float
*
*  Vector2 with float
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
 *  Vector2 with float
 * 
 *  Sample usage:
 *  Vector2 myVector2;
 *  Vector2 myVector2(1.0f, 1.0f);
 *  Vector2* ptrVector2 = new Vector2(); The constructor may take arguments.
 */
class SH_UTILITY_EXPORT  Vector2
{
 public:
  /*
  *  Default constructor.
  */
  Vector2() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  * 
  *  @param float _x: X value.
  *  @param float _y: Y value.
  */
  Vector2(float _x, float _y) : x(_x), y(_y) {}

  /*
  *  Copy constructor.
  * 
  *  @param Vector2 _other: The Vector2 to be copied.
  */
  Vector2(const Vector2& _other);

  /*
  *  Default destructor.
  */
  ~Vector2() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Calculates the dot product of two given Vector2s.
  * 
  *  @param Vector2 _other: To calculate with this Vector2.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector2& _other) const;

  /*
  *  Calculates the magnitude of the Vector2.
  * 
  *  @param 
  * 
  *  @return float The magnitude of this Vector2.
  */
  float
  mag() const;

  /*
  *  Normalize the Vector2.
  *
  *  @param
  *
  *  @return void
  */
  void
  normalize();

  /*
  *  Calculates the cross product of the Vector2.
  *
  *  @param Vector2 _other: To calculate with this Vector2.
  *
  *  @return Vector2 The ortogonal Vector2 between this and the other Vector2.
  */
  float
  scalarProjection(const Vector2& _other) const;

  /*
  *  Calculates the of a Vector2 to another Vector2.
  *
  *  @param Vector2 _other: To calculate over this.
  *
  *  @return Vector2 The ortogonal Vector2 between this and the other Vector2.
  */
  Vector2
  vectorProjection(const Vector2& _other) const;

  /*
  *  Calculates the linear interpolate between two Vector2.
  *
  *  @param Vector2 _vec: Min value.
  * 
  *  @param Vector2 _other: Max value.
  * 
  *  @param float _time: Time.
  *
  *  @return Vector2 The result of the linear interpolation.
  */
  Vector2
  lerp(const Vector2& _vec, const Vector2& _other, const float& _time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /*
  *  Operator to sum a Vector2 values and other Vector2 values.
  *  
  *  @param lValue-Vector2.
  * 
  *  @param rValue-Vector2.
  * 
  *  @return Vector2
  */
  FORCEINLINE Vector2
  operator+(const Vector2& other) const;

  /*
  *  Operator to substract a Vector2 values and other Vector2 values.
  *
  *  @param lValue-Vector2.
  * 
  *  @param rValue-Vector2.
  *
  *  @return Vector2
  */
  FORCEINLINE Vector2
  operator-(const Vector2& other) const;

  /*
  *  Operator to multiply the values of a Vector2 with a float.
  *
  *  @param lValue-Vector2.
  * 
  *  @param rValue-float.
  *
  *  @return Vector2 The result of the multiplication.
  */
  FORCEINLINE Vector2
  operator*(const float& delta) const;

  /*
  *  Operator to sum a Vector2 values and other Vector2 values and store the
  *  result in the first Vector2.
  *
  *  @param lValue-Vector2.
  * 
  *  @param rValue-Vector2.
  * 
  *  @return Vector2 This
  */
  FORCEINLINE Vector2
  operator+=(const Vector2& other);

  /*
  *  Operator to substract a Vector2 values and other Vector2 values and store
  *  the result in the first Vector2.
  *
  *  @param lValue-Vector2.
  * 
  *  @param rValue-Vector2.
  * 
  *   @return Vector2 This
  */
  FORCEINLINE Vector2
  operator-=(const Vector2& other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   * Vector2 X value.
   */
   float x;
   /*
   * Vector2 Y value.
   */
   float y;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Vector2
Vector2::operator+(const Vector2& other) const
{
  return Vector2(x + other.x, y + other.y);
}

FORCEINLINE Vector2
Vector2::operator-(const Vector2& other) const
{
  return Vector2(x - other.x, y - other.y);
}

FORCEINLINE Vector2
Vector2::operator*(const float& delta) const
{
  return Vector2(x * delta, y * delta);
}

FORCEINLINE Vector2
Vector2::operator+=(const Vector2& other)
{
  x += other.x;
  y += other.y;

  return *this;
}

FORCEINLINE Vector2
Vector2::operator-=(const Vector2& other)
{
  x -= other.x;
  y -= other.y;

  return *this;
}
}
