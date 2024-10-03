/*************************************************************/
/*
*  @file    shVector2i.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
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
 /**
 *  @brief Vector2 with integers
 * 
 *  @brief Sample usage:
 *  @brief Vector2i myVector2i;
 *  @brief Vector2i myVector2i(1.0f, 1.0f);
 *  @brief Vector2i* ptrVector2i = new Vector2i(); The constructor may take arguments.
 */
class SH_UTILITY_EXPORT Vector2i
{
 public:
  /**
  *  @brief Default constructor.
  */
  Vector2i() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  *
  *  @param _x: X value.
  *  @param _y: Y value.
  */
  Vector2i(const int32 _x, const int32 _y) : x(_x), y(_y) {}

  /**
  *  @brief Copy constructor
  *
  *  @param other: Another Vector2i
  */
  Vector2i(const Vector2i& other);

  /**
  *  @brief Default destructor.
  */
  ~Vector2i() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Calculates the dot product of two given Vector2is.
  *
  *  @param Vector2i other: To calculate with this Vector2i.
  *
  *  @return int32 The result of the dot product.
  */
  int32
  dot(const Vector2i& other) const;

  /**
  *  @brief Calculates the magnitude of the Vector2i.
  *
  *  @return int32 The magnitude of this Vector2i.
  */
  float
  mag() const;

  /**
  *  @brief Calculates the linear interpolate between two Vector2i.
  *  @brief This Vector2i is taken as the min value.
  *
  *  @param Vector2i vec: Max value.
  *  @param int32 time: Alpha.
  *
  *  @return Vector2i The result of the linear interpolation.
  */
  Vector2i
  lerp(const Vector2i& other, const int32 time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i
  *  @param rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator+(const Vector2i& other) const;

  /**
  *  @brief Operator to substract a Vector2i values and other Vector2i values
  *
  *  @param lValue-Vector2i
  *  @param rValue-Vector2i
  *
  *  @return Vector2i
  */
  FORCEINLINE Vector2i
  operator-(const Vector2i& other) const;

  /**
  *  @brief Operator to multiply the values of a Vector2 with a int32
  *
  *  @param lValue-Vectori2
  *  @param rValue-int32
  *
  *  @return Vector2i The result of the multiplication
  */
  FORCEINLINE Vector2i
  operator*(const int32& delta) const;

  /**
  *  @brief Operator to sum a Vector2i values and other Vector2i values and store
  *         the result in the first Vector2i
  *
  *  @param lValue-Vector2i
  *  @param rValue-Vector2i
  * 
  *  @return Vector2i This.
  */
  FORCEINLINE Vector2i&
  operator+=(const Vector2i& other);

  /**
  *  @brief Operator to substract a Vector2i values and other Vector2i values and
  *         store the result in the first Vector2i
  *
  *  @param lValue-Vector2i
  *  @param rValue-Vector2i
  * 
  *  @return Vector2i This.
  */
  FORCEINLINE Vector2i&
  operator-=(const Vector2i& other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  * @brief Vector2i X value.
  */
  int32 x;

  /**
  * @brief Vector2i Y value.
  */
  int32 y;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Vector2i
Vector2i::operator+(const Vector2i& other) const
{
  return Vector2i(x + other.x, y + other.y);
}

FORCEINLINE Vector2i
Vector2i::operator-(const Vector2i& other) const
{
  return Vector2i(x - other.x, y - other.y);
}

FORCEINLINE Vector2i
Vector2i::operator*(const int32& delta) const
{
  return Vector2i(x * delta, y * delta);
}

FORCEINLINE Vector2i&
Vector2i::operator+=(const Vector2i& other)
{
  x += other.x;
  y += other.y;

  return *this;
}

FORCEINLINE Vector2i&
Vector2i::operator-=(const Vector2i& other)
{
  x -= other.x;
  y -= other.y;

  return *this;
}
}
