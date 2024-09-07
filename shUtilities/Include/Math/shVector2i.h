/*************************************************************/
/*
*  @file    shVector2i.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector2i
*
*  Vector2 with integers
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

namespace shEngineSDK {
class Vector2i
{
public:
  /*
  *  Default constructor: initialize Vector2i values to 0.
  */
  Vector2i();
  /*
  *  Constructor to initialize values to the given numbers.
  *
  *  @param X value, Y value.
  */
  Vector2i(int _x, int _y) : x(_x), y(_y) {}
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
  *  Operator overload
  */
  /*************************************************************/


  Vector2i
  operator+(const Vector2i& other) const
  {
    return Vector2i(x + other.x, y + other.y);
  }

  Vector2i
  operator-(const Vector2i& other) const
  {
    return Vector2i(x - other.x, y - other.y);
  }

  Vector2i
  operator*(const int& delta) const
  {
    return Vector2i(x * delta, y * delta);
  }

  Vector2i
  operator+=(const Vector2i& other)
  {
    x += other.x;
    y += other.y;
  }

  Vector2i
  operator-=(const Vector2i& other)
  {
    x -= other.x;
    y -= other.y;
  }

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
}
