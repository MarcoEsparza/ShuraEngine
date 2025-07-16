/*****************************************************************************/
/*
*  @file    shVector2.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Vector2 with float
*
*  Vector2 with float
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
 /**
 *  @brief Vector2 with float
 */
class SH_UTILITY_EXPORT  Vector2
{
 public:
  /**
  *  @brief Default constructor.
  */
  Vector2() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  * 
  *  @param float x: X value.
  *  @param float y: Y value.
  */
  Vector2(float x, float y)
    : x(x), y(y)
  {}

  /**
  *  @brief Copy constructor.
  * 
  *  @param const Vector2& other: The Vector2 to be copied.
  */
  Vector2(const Vector2& other);

  /**
  *  @brief Default destructor.
  */
  ~Vector2() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/

  /**
  *  @brief Calculates the dot product of two given Vector2s.
  * 
  *  @param const Vector2& other: To calculate with this Vector2.
  * 
  *  @return float: The result of the dot product.
  */
  float
  dot(const Vector2& other) const;

  /**
  *  @brief Calculates the cross product of this Vector2 with another Vector2.
  *
  *  @param const Vector2& other: To calculate with this Vector2.
  *
  *  @return float: The result of the cross product.
  */
  float
  cross(const Vector2& other);

  /**
  *  @brief Calculates the magnitude of the Vector2.
  * 
  *  @return float: The magnitude of this Vector2.
  */
  float
  mag() const;

  /**
  *  @brief Normalize this Vector2.
  */
  void
  normalize();

  /**
  *  @brief Get this Vector2 normalized.
  *
  *  @return Vector2: The normalized Vector2.
  */
  Vector2
  getNormalized() const;

  /**
  *  @brief Calculates the cross product of the Vector2.
  *
  *  @param const Vector2& other: To calculate with this Vector2.
  *
  *  @return Vector2: The ortogonal Vector2 between this and the other Vector2.
  */
  float
  scalarProjection(const Vector2& other) const;

  /**
  *  @brief Calculates the of a Vector2 to another Vector2.
  *
  *  @param const Vector2& other: To calculate over this.
  *
  *  @return Vector2 The ortogonal Vector2 between this and the other Vector2.
  */
  Vector2
  vectorProjection(const Vector2& other) const;

  /**
  *  @brief Calculates the linear interpolate between two Vector2.
  *  @brief This Vector2 is taken as the min value.
  *
  *  @param const Vector2& other: Max value.
  *  @param const float time: Alpha.
  *
  *  @return Vector2 The result of the linear interpolation.
  */
  Vector2
  lerp(const Vector2& other, const float time) const;

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector2 values and other Vector2 values.
  *  
  *  @param const Vector2& other: Vector2 to be added.
  * 
  *  @return Vector2: The result of the addition.
  */
  FORCEINLINE Vector2
  operator+(const Vector2& other) const;

  /**
  *  @brief Operator to substract a Vector2 values and other Vector2 values.
  *
  *  @param const Vector2& other: Vector2 to be substracted.
  *
  *  @return Vector2: The result of the substraction.
  */
  FORCEINLINE Vector2
  operator-(const Vector2& other) const;

  /**
  *  @brief Operator to multiply the values of a Vector2 with a float.
  *
  *  @param const float delta: The float to multiply with.
  *
  *  @return Vector2: The result of the multiplication.
  */
  FORCEINLINE Vector2
  operator*(const float delta) const;

  /**
  *  @brief Operator to divide the values of a Vector2 with a float.
  *
  *  @param const float delta: The float to divide with.
  *
  *  @return Vector2: The result of the division.
  */
  FORCEINLINE Vector2
  operator/(const float delta) const;

  /**
  *  @brief Operator to multiply the values of a Vector2 with another Vector2.
  *
  *  @param const Vector2& other: The Vector2 to multiply with.
  *
  *  @return Vector2: The result of the multiplication.
  */
  FORCEINLINE Vector2
  operator*(const Vector2& other) const;

  /**
  *  @brief Operator to divide the values of a Vector2 with another Vector2.
  *
  *  @param const Vector2& other: The Vector2 to divide with.
  *
  *  @return Vector2: The result of the division.
  */
  FORCEINLINE Vector2
  operator/(const Vector2& other) const;

  /**
  *  @brief Operator to sum a Vector2 values and other Vector2 values and store
  *         the result in the first Vector2.
  *
  *  @param const Vector2& other: Vector2 to be added.
  * 
  *  @return Vector2: This Vector2 with the result of the addition.
  */
  FORCEINLINE Vector2&
  operator+=(const Vector2& other);

  /**
  *  @brief Operator to substract a Vector2 values and other Vector2 values and
  *         store the result in the first Vector2.
  *
  *  @param const Vector2& other: Vector2 to be substracted.
  * 
  *  @return Vector2: This Vector2 with the result of the substraction.
  */
  FORCEINLINE Vector2&
  operator-=(const Vector2& other);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  * @brief Vector2 X value.
  */
  float x;

  /**
  * @brief Vector2 Y value.
  */
  float y;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

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
Vector2::operator*(const float delta) const
{
  return Vector2(x * delta, y * delta);
}

FORCEINLINE Vector2
Vector2::operator/(const float delta) const
{
  return Vector2(x * delta, y * delta);
}

FORCEINLINE Vector2
Vector2::operator*(const Vector2& other) const
{
  return Vector2(x * other.x, y * other.y);
}

FORCEINLINE Vector2
Vector2::operator/(const Vector2& other) const
{
  return Vector2(x / other.x, y / other.y);
}

FORCEINLINE Vector2&
Vector2::operator+=(const Vector2& other)
{
  x += other.x;
  y += other.y;

  return *this;
}

FORCEINLINE Vector2&
Vector2::operator-=(const Vector2& other)
{
  x -= other.x;
  y -= other.y;

  return *this;
}
}
