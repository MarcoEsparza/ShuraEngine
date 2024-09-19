/*************************************************************/
/*
*  @file    shVector2i.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/18
*  @brief   Vector2 with integers
*
*  Vector2 with integers
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
 *  Vector2 with integers
 * 
 *  Sample usage:
 *  Vector2i myVector2i;
 *  Vector2i myVector2i(1.0f, 1.0f);
 *  Vector2i* ptrVector2i = new Vector2i(); The constructor may take arguments.
 */
class SH_UTILITY_EXPORT Vector2i
{
 public:
  /*
  *  Default constructor.
  */
  Vector2i() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  *
  *  @param _x: X value.
  *
  *  @param _y: Y value.
  */
  Vector2i(int32 _x, int32 _y) : x(_x), y(_y) {}

  /*
  *  Copy constructor
  *
  *  @param _other: Another Vector2i
  */
  Vector2i(const Vector2i& _other);

  /*
  *  Default destructor.
  */
  ~Vector2i() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Calculates the dot product of two given Vector2is.
  *
  *  @param Vector2i _other: To calculate with this Vector2i.
  *
  *  @return int32 The result of the dot product.
  */
  int32
  dot(const Vector2i& _other) const;

  /*
  *  Calculates the magnitude of the Vector2i.
  *
  *  @param
  *
  *  @return int32 The magnitude of this Vector2i.
  */
  float
  mag() const;

  /*
  *  Calculates the cross product of the Vector2i.
  *
  *  @param Vector2i _other: To calculate with this Vector2i.
  *
  *  @return Vector2i The ortogonal Vector2i between this and the other
  *          Vector2i.
  */
  float
  scalarProjection(const Vector2i& _other) const;

  /*
  *  Calculates the of a Vector2i to another Vector2i.
  *
  *  @param Vector2i _other: To calculate over this.
  *
  *  @return Vector2i The ortogonal Vector2i between this and the other
  *          Vector2i.
  */
  Vector2i
  vectorProjection(const Vector2i& _other) const;

  /*
  *  Calculates the linear interpolate between two Vector2i.
  *  @brief This Vector2i is taken as the min value.
  *
  *  @param Vector2i _vec: Max value.
  * 
  *  @param int32 _time: Time.
  *
  *  @return Vector2i The result of the linear interpolation.
  */
  Vector2i
  lerp(const Vector2i& _other, const int32& _time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /*
  *  Operator to sum a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i
  *
  *  @param rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator+(const Vector2i& _other) const;

  /*
  *  Operator to substract a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i
  * 
  *  @param rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator-(const Vector2i& _other) const;

  /*
  *  Operator to multiply the values of a Vector2 with a int32
  *
  *  @param lValue-Vectori2
  * 
  *  @param rValue-int32
  *
  *  @return Vector2i The result of the multiplication
  */
  FORCEINLINE Vector2i
  operator*(const int32& _delta) const;

  /*
  *  Operator to sum a Vector2i values and other Vector2i values and store the
  *  result in the first Vector2i
  *
  *  @param lValue-Vector2i
  * 
  *  @param rValue-Vector2i
  * 
  *  @return Vector2i This.
  */
  FORCEINLINE Vector2i&
  operator+=(const Vector2i& _other);

  /*
  *  Operator to substract a Vector2i values and other Vector2i values and store
  *  the result in the first Vector2i
  *
  *  @param lValue-Vector2i
  * 
  *  @param rValue-Vector2i
  * 
  *  @return Vector2i This.
  */
  FORCEINLINE Vector2i&
  operator-=(const Vector2i& _other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   * Vector2i X value.
   */
   int32 x;
   /*
   * Vector2i Y value.
   */
   int32 y;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Vector2i
Vector2i::operator+(const Vector2i& _other) const
{
  return Vector2i(x + _other.x, y + _other.y);
}

FORCEINLINE Vector2i
Vector2i::operator-(const Vector2i& _other) const
{
  return Vector2i(x - _other.x, y - _other.y);
}

FORCEINLINE Vector2i
Vector2i::operator*(const int32& _delta) const
{
  return Vector2i(x * _delta, y * _delta);
}

FORCEINLINE Vector2i&
Vector2i::operator+=(const Vector2i& _other)
{
  x += _other.x;
  y += _other.y;

  return *this;
}

FORCEINLINE Vector2i&
Vector2i::operator-=(const Vector2i& _other)
{
  x -= _other.x;
  y -= _other.y;

  return *this;
}
}
