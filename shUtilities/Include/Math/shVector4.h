/*************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector4
*
*  float Vector4
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

namespace shEngineSDK {
class  Vector4
{
public:
  /*
  *  Default constructor: initialize Vector4 values to 0.
  */
  Vector4();
  /*
  *  Constructor to initialize values to the given numbers.
  *
  *  @param X value, Y value, Z value, W value
  */
  Vector4(float _x,
          float _y,
          float _z,
          float _w) : x(_x), y(_y), z(_z), w(_w) {}
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
  *  Operator overload
  */
  /*************************************************************/


  Vector4
  operator+(const Vector4& other) const
  {
    return Vector4(x + other.x,
                   y + other.y,
                   z + other.z,
                   w + other.w);
  }

  Vector4
  operator-(const Vector4& other) const
  {
    return Vector4(x - other.x,
                   y - other.y,
                   z - other.z,
                   w - other.w);
  }

  Vector4
  operator*(const float& delta) const
  {
    return Vector4(x + delta,
                   y + delta,
                   z + delta,
                   w + delta);
  }

  void
  operator+=(const Vector4& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
  }

  void
  operator-=(const Vector4& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
  }

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
}
