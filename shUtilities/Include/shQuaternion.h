/*************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
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
/**
*  @brief Quaternion for rotations
* 
*  @brief Sample usage:
*  @brief Quaternion quat;
*  @brief Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
*/
class SH_UTILITY_EXPORT Quaternion
{
 public:
  /**
  *  @brief Default constructor.
  */
  Quaternion() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  *  @note Recommended to initialize in radians.
  *  
  *  @param float _w: W value.
  *  @param float _x: X value.
  *  @param float _y: Y value.
  *  @param float _z: Z value.
  */
  Quaternion(float _w, float _x, float _y, float _z) :
             w(_w), x(_x), y(_y), z(_z) {}

  /**
  *  @brief Constructor from euler degrees to Quaternion radians.
  *
  *  @param Vector3 _vec: The euler angles to transform.
  */
  explicit Quaternion(const Vector3& _vec);

  /**
  *  @brief Constructor with given axis and angle.
  *
  *  @param Vector3& axis
  *  @param float angle
  */
  Quaternion(const Vector3& axis, const float angle);

  /**
  *  @brief Copy constructor
  *
  *  @param Quaternion
  */
  Quaternion(const Quaternion& _other);

  /**
  *  @brief Default destructor.
  */
  ~Quaternion() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Transform Quaternion angles from radians to euler degrees in Vector3.
  * 
  *  @return Vector3 The angles in euler degrees
  */
  Vector3
  toEulerAngles() const;

  /**
  *  @brief Transform Quaternion to Vector3
  *
  *  @param Vector3& vec
  * 
  *  @return Vector3
  */
  Vector3
  toRotate(const Vector3& vec) const;

  /**
  *  @brief Transform three given Vectors3 to axes vectors for a rotation matrix3.
  *
  *  @param Vector3& right
  *  @param Vector3& up
  *  @param Vector3& forward
  */
  void
  toAxes(Vector3& right, Vector3& up, Vector3& forward) const;

  /**
  *  @brief Transform Vector3 angles from euler degrees to Quaternion radians.
  *
  *  @param Vector3 vec
  *
  *  @return Quaternion Transformed euler angles to radians.
  */
  Quaternion
  fromEulerAngles(const Vector3& vec) const;

  /**
  *  @brief Transform this Quaternion values to radians from euler degrees.
  *
  *  @param Vector3 vec: The euler angles.
  */
  void
  fromAngle(Vector3 vec);

  /**
  *  @brief Computes the euclidian lenght.
  *
  *  @return float
  */
  float
  lenght() const;

  /**
  *  @brief Normalize Quaternion values.
  */
  void
  normalize();

  /**
  *  @brief Normalize Quaternion values.
  *
  *  @return Quaternion
  */
  Quaternion
  getNormalized() const;

  /**
  *  @brief Calculates the dot product of this Quaternion and a given Quaternion.
  *
  *  @param Quaternion other: To calculate with this.
  *
  *  @return float Result from dot product.
  */
  float
  dot(const Quaternion& other) const;

  /**
  *  @brief Change Quaternion value symbol: x = 1.0f -> x = -1.0f.
  *
  *  @return Quaternion
  */
  Quaternion
  conjugate() const;

  /**
  *  @brief Get the inverse rotation of the Quaternion.
  *
  *  @return Quaternion The inverse of this Quaternion.
  */
  Quaternion
  inverse();

  /**
  *  @brief Returns the angle in degrees between two rotations, this and _other.
  *  @brief The resulting angle ranges from 0 to 180.
  *
  *  @param Quaternion other: The Quaternion to be calculate with.
  *
  *  @return float The resultant angle.
  */
  float
  angleTo(const Quaternion& other) const;

  /**
  *  @brief Rotates the Quaternion towards another Quaternion.
  *
  *  @param Quaternion other
  *  @param float step
  *
  *  @return Quaternion
  */
  Quaternion
  rotateTowards(const Quaternion& other, const float step) const;

  /**
  *  @brief Linear interpolation between two given Quaternion.
  * 
  *  @param Quaternion other: Max value.
  *  @param float time: Alpha.
  *
  *  @return Quaternion
  */
  Quaternion
  lerp(const Quaternion& other, const float _time) const;

  /**
  *  @brief Spherically linear interpolates between this Quaternion and another
  *         by this ratio of t.
  *
  *  @param Quaternion other: End unit quaternion value, returned when t = 1.
  *  @param float time: Alpha.
  *
  *  @return Quaternion Returns this Quaternion when t = 0.
  */
  Quaternion
  slerp(const Quaternion& other, const float _time) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Quaternion sum.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator+(const Quaternion& other) const;

  /**
  *  @brief Quaternion substraction.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator-(const Quaternion& other) const;

  /**
  *  @brief Quaternion multiplication with another Quaternion.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator*(const Quaternion& other) const;

  /**
  *  @brief Quaternion multiplication with a float value.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion
  operator*(const float delta) const;

  /**
  *  @brief Quaternion multiplication with an axis.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator*(const Vector3& axis) const;

  /**
  *  @brief Operator to sum a Quaternion values and other Quaternion values and
  *         store the result in the first Quaternion.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion&
  operator+=(const Quaternion& other);

  /**
  *  @brief Operator to sum a Quaternion values and other Quaternion values and
  *         store the result in the first Quaternion.
  *
  *  @param lValue-Quaternion.
  *  @param rValue-Quaternion.
  *
  *  @return Quaternion
  */
  FORCEINLINE Quaternion&
  operator-=(const Quaternion& other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  * @brief Quaternion W value. Real part.
  */
  float w;

  /**
  * @brief Quaternion X value. Imaginary part.
  */
  float x;

  /**
  * @brief Quaternion Y value. Imaginary part.
  */
  float y;

  /**
  * @brief Quaternion Z value. Imaginary part.
  */
  float z;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Quaternion
Quaternion::operator+(const Quaternion& other) const
{
  return Quaternion(x + other.x,
                    y + other.y,
                    z + other.z,
                    w + other.w);
}

FORCEINLINE Quaternion
Quaternion::operator-(const Quaternion& other) const
{
  return Quaternion(x - other.x,
                    y - other.y,
                    z - other.z,
                    w - other.w);
}

FORCEINLINE Quaternion
Quaternion::operator*(const Quaternion& other) const
{
  // w = (w1 * w2) - (x1 * x2) - (y1 * y2) - (z1 * z2)
  // x = (w1 * x2) + (x1 * w2) + (y1 * z2) - (z1 * y2)
  // y = (w1 * y2) - (x1 * z2) + (y1 * w2) + (z1 * x2)
  // z = (w1 * z2) + (x1 * y2) - (y1 * x2) + (z1 * w2)

  return Quaternion((w * other.w) - (x * other.x) - (y * other.y) - (z * other.z),
                    (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y),
                    (w * other.y) - (x * other.z) + (y * other.w) + (z * other.x),
                    (w * other.z) + (x * other.y) + (y * other.x) + (z * other.w));
}

FORCEINLINE Quaternion
Quaternion::operator*(const float delta) const
{
  return Quaternion(x * delta,
                    y * delta,
                    z * delta,
                    w * delta);
}

FORCEINLINE Vector3
Quaternion::operator*(const Vector3& axis) const
{
  const Quaternion qVec(0.0f, axis.x, axis.y, axis.z);
  const Quaternion qConj = conjugate();

  const Quaternion qRot = (*this * qVec) * qConj;

  return Vector3(qRot.x, qRot.y, qRot.z);
}

FORCEINLINE Quaternion&
Quaternion::operator+=(const Quaternion& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;

  return *this;
}

FORCEINLINE Quaternion&
Quaternion::operator-=(const Quaternion& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;

  return *this;
}
}
