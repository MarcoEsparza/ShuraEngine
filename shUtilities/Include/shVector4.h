/*************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
*  @brief   Vector4
*
*  float Vector4
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
#include "shPrerequsitesUtilities.h"

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
  *  @param X value, Y value, Z value, W value
  */
  Vector4(float _x, float _y, float _z, float _w);

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
  *  Linear interpolation between two given Vector4.
  *
  *  @param Min value, max value, time.
  *
  *  @return Vector4 Interpolated Vector4.
  */
  Vector4
  lerp(const Vector4& vecA, const Vector4& vecB, float time) const;

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

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
  public:
  /*
  *  Operator to sum a Vector4 values and other Vector4 values
  *
  *  @param lValue-Vector4, rValue-Vector4
  *
  *  @return Vector4
  */
  FORCEINLINE Vector4
  operator+(const Vector4& other) const
  {
    return Vector4(x + other.x,
                   y + other.y,
                   z + other.z,
                   w + other.w);
  }

  /*
  *  Operator to substract a Vector4 values and other Vector4 values
  *
  *  @param lValue-Vector4, rValue-Vector4
  *
  *  @return Vector4
  */
  FORCEINLINE Vector4
  operator-(const Vector4& other) const
  {
    return Vector4(x - other.x,
                   y - other.y,
                   z - other.z,
                   w - other.w);
  }

  /*
  *  Operator to multiply the values of a Vector4 with a float
  *
  *  @param lValue-Vector4, rValue-float
  *
  *  @return Vector4 The result of the multiplication
  */
  FORCEINLINE Vector4
  operator*(const float& delta) const
  {
    return Vector4(x + delta,
                   y + delta,
                   z + delta,
                   w + delta);
  }

  /*
  *  Operator to sum a Vector4 values and other Vector4 values and store the
  *  result in the first Vector4
  *
  *  @param lValue-Vector4, rValue-Vector4
  */
  FORCEINLINE void
  operator+=(const Vector4& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
  }

  /*
  *  Operator to substract a Vector4 values and other Vector4 values and store
  *  the result in the first Vector4
  *
  *  @param lValue-Vector4, rValue-Vector4
  */
  FORCEINLINE void
  operator-=(const Vector4& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
  }
};
}
