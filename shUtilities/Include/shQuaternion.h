/*************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/18
*  @brief   Quaternion for rotations
*
*  Quaternion for rotations
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
#include "shVector3.h"

namespace shEngineSDK {
/*
*  Quaternion for rotations
* 
*  Sample usage:
*  
*/
class SH_UTILITY_EXPORT Quaternion
{
 public:
  /*
  *  Default constructor.
  */
  Quaternion() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  *  Recommended to initialize in radians.
  *  
  *  @param float _x: X value.
  *  @param float _y: Y value.
  *  @param float _z: Z value.
  *  @param float _w: W value.
  */
  Quaternion(float _x, float _y, float _z, float _w) :
             x(_x), y(_y), z(_z), w(_w) {}

  /*
  *  Copy constructor
  *
  *  @param Quaternion
  */
  Quaternion(const Quaternion& _other);

  /*
  *  Default destructor.
  */
  ~Quaternion() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Transform quaternion angles from radians to euler degrees.
  *
  *  @param
  * 
  *  @return Vector3 The transformed euler degrees.
  */
  Vector3
  eulerAngles();

  /*
  *  Transform quaternion angles from euler degrees to radians.
  *  @brief WARNING: make sure your values are euler degrees, because in this
  *  case this function may give you wrong answers.
  *
  *  @param
  *
  *  @return
  */
  void
  radAngles();

  /*
  *  Computes the euclidian lenght.
  *
  *  @param
  *
  *  @return
  */
  float
  lenght() const;

  /*
  *  Normalize Quaternion values.
  *
  *  @param
  *
  *  @return
  */
  void
  normalize();

  /*
  *  Calculates the dot product of this Quaternion and a given Quaternion.
  *
  *  @param Quaternion _other: To calculate with this.
  *
  *  @return float Result from dot product.
  */
  float
  dot(const Quaternion& _other) const;

  /*
  *  Change Quaternion value symbol: x = 1.0f -> x = -1.0f.
  *
  *  @param
  *
  *  @return
  */
  Quaternion
  conjugate();

  /*
  *  Get the inverse rotation of the Quaternion.
  *
  *  @param
  *
  *  @return Quaternion The inverse of this Quaternion.
  */
  Quaternion
  inverse();

  /*
  *  Returns the angle in degrees between two rotations, this and _other.
  *  The resulting angle ranges from 0 to 180.
  *
  *  @param Quaternion _other: The Quaternion to be calculate with.
  *
  *  @return float The resultant angle.
  */
  float
  angleTo(const Quaternion& _other) const;

  /*
  *  Rotates the Quaternion towards another Quaternion.
  *
  *  @param Quaternion _other
  * 
  *  @param float _step
  *
  *  @return Quaternion
  */
  Quaternion
  rotateTowards(const Quaternion& _other, const float& _step) const;

  /*
  *  Linear interpolation between two given Quaternion.
  *
  *  @param Quaternion _quatA: Min value.
  * 
  *  @param Quaternion _quatB: Max value.
  * 
  *  @param float _time: Time.
  *
  *  @return Quaternion
  */
  Quaternion
  lerp(const Quaternion& _other, const float& _time) const;

  /*
  *  Spherically linear interpolates between this Quaternion and another
  *  by this ratio of t.
  *
  *  @param Quaternion _other: End unit quaternion value, returned when t = 1.
  * 
  *  @param float _time: Time.
  *
  *  @return Quaternion Returns this Quaternion when t = 0.
  */
  Quaternion
  slerp(const Quaternion& _other, const float& _time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /*
  *  Quaternion sum.
  *
  *  @param Quaternion _other
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator+(const Quaternion& _other) const;

  /*
  *  Quaternion substraction.
  *
  *  @param lValue-Quaternion.
  * 
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator-(const Quaternion& _other) const;

  /*
  *  Quaternion multiplication with another Quaternion.
  *
  *  @param lValue-Quaternion.
  *
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
    operator*(const Quaternion& _delta) const;

  /*
  *  Quaternion multiplication with a float value.
  *
  *  @param lValue-Quaternion.
  * 
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator*(const float& _delta) const;

  /*
  *  Operator to sum a Quaternion values and other Quaternion values and
  *  store the result in the first Quaternion.
  *
  *  @param lValue-Quaternion.
  * 
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion&
  operator+=(const Quaternion& _other);

  /*
  *  Operator to sum a Quaternion values and other Quaternion values and
  *  store the result in the first Quaternion.
  *
  *  @param lValue-Quaternion.
  * 
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion&
  operator-=(const Quaternion& _other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /*
  * Quaternion X value.
  */
  float x;
  /*
  * Quaternion Y value.
  */
  float y;
  /*
  * Quaternion Z value.
  */
  float z;
  /*
  * Quaternion W value.
  */
  float w;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Quaternion
Quaternion::operator+(const Quaternion& _other) const
{
  return Quaternion(x + _other.x,
                    y + _other.y,
                    z + _other.z,
                    w + _other.w);
}

FORCEINLINE Quaternion
Quaternion::operator-(const Quaternion& _other) const
{
  return Quaternion(x - _other.x,
                    y - _other.y,
                    z - _other.z,
                    w - _other.w);
}

FORCEINLINE Quaternion
Quaternion::operator*(const Quaternion& _other) const
{
  return Quaternion(x * _other.w + w * _other.x + y * _other.z + z * _other.y,
                    y * _other.w + w * _other.y + z * _other.x + x * _other.z,
                    z * _other.w + w * _other.z + x * _other.y + y * _other.x,
                    w * _other.w - x * _other.x - y * _other.y - z * _other.z);
}

FORCEINLINE Quaternion
Quaternion::operator*(const float& _delta) const
{
  return Quaternion(x * _delta,
                    y * _delta,
                    z * _delta,
                    w * _delta);
}

FORCEINLINE Quaternion&
Quaternion::operator+=(const Quaternion& _other)
{
  x += _other.x;
  y += _other.y;
  z += _other.z;
  w += _other.w;

  return *this;
}

FORCEINLINE Quaternion&
Quaternion::operator-=(const Quaternion& _other)
{
  x -= _other.x;
  y -= _other.y;
  z -= _other.z;
  w -= _other.w;

  return *this;
}
}
