/*************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
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
*  @brief Vector4 with float.
*  
*  @brief Sample usage:
*  @brief Vector4 myVector4;
*  @brief Vector4 myVector4(1.0f, 1.0f);
*  @brief Vector4* ptrVector4 = new Vector4(); The constructor may take arguments.
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
  *  @param float _x: X value.
  *  @param float _y: Y value.
  *  @param float _z: Z value.
  *  @param float _w: W value.
  */
  Vector4(const float _x, const float _y, const float _z, const float _w) :
          x(_x), y(_y), z(_z), w(_w) {}

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

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

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

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Operator to sum a Vector4 values and other Vector4 values.
  *
  *  @param lValue-Vector4.
  *  @param rValue-Vector4.
  *
  *  @return Vector4.
  */
  FORCEINLINE Vector4
  operator+(const Vector4& other) const;

  /**
  *  @brief Operator to substract a Vector4 values and other Vector4 values.
  *
  *  @param lValue-Vector4.
  *  @param rValue-Vector4.
  *
  *  @return Vector4.
  */
  FORCEINLINE Vector4
  operator-(const Vector4& other) const;

  /**
  *  @brief Operator to multiply the values of a Vector4 with a float.
  *
  *  @param lValue-Vector4.
  *  @param rValue-float.
  *
  *  @return Vector4 The result of the multiplication.
  */
  FORCEINLINE Vector4
  operator*(const float delta) const;

  /**
  *  @brief Operator to sum a Vector4 values and other Vector4 values and store
  *         the result in the first Vector4.
  *
  *  @param lValue-Vector4.
  *  @param rValue-Vector4.
  * 
  *  @return Vector4 This.
  */
  FORCEINLINE Vector4&
  operator+=(const Vector4& other);

  /**
  *  @brief Operator to substract a Vector4 values and other Vector4 values and
  *         store the result in the first Vector4.
  *
  *  @param lValue-Vector4.
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
}
