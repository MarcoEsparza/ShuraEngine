/*****************************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Vector4 with float.
*
*  Vector4 with float.
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
class Vector3;

/*
*  @brief Vector4 with float.
*/
class SH_UTILITY_EXPORT Vector4
{
 public:
  /**
  *  @brief Default constructor: initialize Vector4 values to 0.
  */
  Vector4() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  *
  *  @param float x: X value.
  *  @param float y: Y value.
  *  @param float z: Z value.
  *  @param float w: W value.
  */
  Vector4(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
  {}

  /**
  *  @brief Constructor with a given vec3 and a w component.
  *
  *  @param Vector4 other
  */
  Vector4(const Vector3& xyz, const float _w);

  /**
  *  @brief Copy constructor
  *
  *  @param Vector4 other
  */
  Vector4(const Vector4& other);

  /**
  *  Default destructor.
  */
  ~Vector4() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Calculates the dot product of this Vector4 and a given Vector4.
  *
  *  @param Vector4 other: To calculate with this Vector4.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector4& other) const;

  /**
  *  @brief Calculates the magnitude of the Vector4.
  *
  *  @return float The magnitude of this Vector4.
  */
  float
  mag() const;

  /**
  *  @brief Normalize this Vector4.
  */
  void
  normalize();

  /**
  *  @brief Get this Vector4 normalized.
  *
  *  @return Vector4 The normalized Vector4.
  */
  Vector4
  getNormalized() const;

  /**
  *  @brief Linear interpolation between two given Vector4.
  *  @brief This Vector4 is taken as the min value.
  *
  *  @param Vector4 other: Max value.
  *  @param float time: Alpha.
  *
  *  @return Vector4 Interpolated Vector4.
  */
  Vector4
  lerp(const Vector4& other, const float time) const;

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector4 values and other Vector4 values.
  *
  *  @param const Vector4& other: Vector4 to sum with this Vector4.
  *
  *  @return Vector4: The result of the sum of this Vector4 and the other Vector4.
  */
  FORCEINLINE Vector4
  operator+(const Vector4& other) const;

  /**
  *  @brief Operator to substract a Vector4 values and other Vector4 values.
  *
  *  @param const Vector4& other: Vector4 to substract from this Vector4.
  *
  *  @return Vector4: The result of the substraction of this Vector4 and the other Vector4.
  */
  FORCEINLINE Vector4
  operator-(const Vector4& other) const;

  /**
  *  @brief Operator to multiply the values of a Vector4 with a float.
  *
  *  @param const float delta: The value to multiply with this Vector4.
  *
  *  @return Vector4: The result of the multiplication.
  */
  FORCEINLINE Vector4
  operator*(const float delta) const;

  /**
  *  @brief Operator to sum a Vector4 values and other Vector4 values and store
  *         the result in the first Vector4.
  *
  *  @param const Vector4& other: Vector4 to sum with this Vector4.
  * 
  *  @return Vector4: This Vector4 with the result of the sum.
  */
  FORCEINLINE Vector4&
  operator+=(const Vector4& other);

  /**
  *  @brief Operator to substract a Vector4 values and other Vector4 values and
  *         store the result in the first Vector4.
  *
  *  @param const Vector4& other: Vector4 to substract from this Vector4.
  * 
  *  @return Vector4: This with the result of the substraction.
  */
  FORCEINLINE Vector4&
  operator-=(const Vector4& other);

  /**
  *  @brief Operator to compere if two Vector4 are equal.
  *
  *  @param const Vector4& other: Vector4 to compere with this Vector4.
  * 
  *  @return Vector4: This if both are equal, false otherwise.
  */
  FORCEINLINE bool
  operator==(const Vector4& other);

  /**
  *  @brief Operator to compere if two Vector4 are different.
  *
  *  @param const Vector4& other: Vector4 to compere with this Vector4.
  * 
  *  @return Vector4: This if both are different, false otherwise.
  */
  FORCEINLINE bool
  operator!=(const Vector4& other);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  * @brief Vector4 X value.
  */
  float x;

  /**
  * @brief Vector4 Y value.
  */
  float y;

  /**
  * @brief Vector4 Z value.
  */
  float z;

  /**
  * @brief Vector4 W value.
  */
  float w;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

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
Vector4::operator*(const float delta) const
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

FORCEINLINE bool
Vector4::operator==(const Vector4& other)
{
  return ((x == other.x) ||
          (y == other.y) ||
          (z == other.z) ||
          (w == other.w));
}

FORCEINLINE bool
Vector4::operator!=(const Vector4& other)
{
  return ((x != other.x) ||
          (y != other.y) ||
          (z != other.z) ||
          (w != other.w));
}
}
