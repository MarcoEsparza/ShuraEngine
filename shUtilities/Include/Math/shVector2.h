/*************************************************************/
/*
*  @file    shVector2.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector2
*
*  Vector2 with float
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

namespace shEngineSDK {
class  Vector2
{
public:
  /*
  *  Default constructor: initialize Vector2 values to 0.
  */
  Vector2();
  /*
  *  Constructor to initialize values to the given numbers.
  * 
  *  @param X value, Y value.
  */
  Vector2(float _x, float _y);
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
  *  @param The Vector2 to calculate with this Vector2.
  * 
  *  @return float The result of the dot product.
  */
  float
  dot(const Vector2& other) const;

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
  *  @param The Vector2 to calculate with this Vector2.
  *
  *  @return Vector2 The ortogonal Vector2 between this and the other Vector2.
  */
  float
  scalarProjection(const Vector2& other) const;

  /*
  *  Calculates the of a Vector2 to another Vector2.
  *
  *  @param The first Vector2, the second Vector2 to calculate over the first
  *
  *  @return Vector2 The ortogonal Vector2 between this and the other Vector2.
  */
  Vector2
  vectorProjection(const Vector2& other) const;

  /*
  *  Calculates the linear interpolate between two Vector2.
  *
  *  @param Min value, max value, time.
  *
  *  @return Vector2 The result of the linear interpolation.
  */
  Vector2
  lerp(const Vector2& vec, const Vector2& other, const float time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/


  Vector2
  operator+(const Vector2& other) const
  {
    return Vector2(x + other.x, y + other.y);
  }

  Vector2
  operator-(const Vector2& other) const
  {
    return Vector2(x - other.x, y - other.y);
  }

  Vector2
  operator*(const float& delta) const
  {
    return Vector2(x * delta, y * delta);
  }

  Vector2
  operator+=(const Vector2& other)
  {
    x += other.x;
    y += other.y;
  }

  Vector2
  operator-=(const Vector2& other)
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
  * Vector2 X value.
  */
  float x;
  /*
  * Vector2 Y value.
  */
  float y;
};
}
