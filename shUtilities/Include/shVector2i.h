/*************************************************************/
/*
*  @file    shVector2i.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/12
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
 *   Vector2i myVector2i(1.0f, 1.0f);
 *   Vector2i* ptrVector2i = new Vector2i(); The constructor may take arguments.
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
  *  @param X value, Y value.
  */
  Vector2i(int32 _x, int32 _y);

  /*
  *  Copy constructor
  *
  *  @param Vector2i
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
  *  @param The Vector2i to calculate with this Vector2i.
  *
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector2i& vec, const Vector2i& other) const;

  /*
  *  Calculates the magnitude of the Vector2i.
  *
  *  @param
  *
  *  @return float The magnitude of this Vector2i.
  */
  float
  mag() const;

  /*
  *  Calculates the linear interpolate between two Vector2i.
  *
  *  @param Min value, max value, time.
  *
  *  @return Vector2i The result of the linear interpolation.
  */
  Vector2i
  lerp(const Vector2i& vec, const Vector2i& other, const float time) const;

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

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
  public:
  /*
  *  Operator to sum a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i, rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator+(const Vector2i& other) const
  {
    return Vector2i(x + other.x, y + other.y);
  }

  /*
  *  Operator to substract a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i, rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator-(const Vector2i& other) const
  {
    return Vector2i(x - other.x, y - other.y);
  }

  /*
  *  Operator to multiply the values of a Vector2 with a int32
  *
  *  @param lValue-Vectori2, rValue-int32
  *
  *  @return Vector2i The result of the multiplication
  */
  FORCEINLINE Vector2i
  operator*(const int32& delta) const
  {
    return Vector2i(x * delta, y * delta);
  }

  /*
  *  Operator to sum a Vector2i values and other Vector2i values and store the
  *  result in the first Vector2i
  *
  *  @param lValue-Vector2i, rValue-Vector2i
  */
  FORCEINLINE void
  operator+=(const Vector2i& other)
  {
    x += other.x;
    y += other.y;
  }

  /*
  *  Operator to substract a Vector2i values and other Vector2i values and store
  *  the result in the first Vector2i
  *
  *  @param lValue-Vector2i, rValue-Vector2i
  */
  FORCEINLINE void
  operator-=(const Vector2i& other)
  {
    x -= other.x;
    y -= other.y;
  }
};
}
