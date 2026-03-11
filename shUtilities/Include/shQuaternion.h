/*****************************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Quaternion class for rotations and transformations.
*
*  Quaternion class for rotations and transformations.
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
#include "shVector3.h"

namespace shEngineSDK {
class Matrix3;

/**
*  @brief Quaternion class for rotations and transformations.
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
  *  @param float w: W value.
  *  @param float x: X value.
  *  @param float y: Y value.
  *  @param float z: Z value.
  */
  Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z)
  {}

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

  Quaternion(const Vector3& from, const Vector3& to);

  Quaternion(const Matrix3& rotationMatrix);

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

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Transform Quaternion angles from radians to euler degrees in Vector3.
  * 
  *  @return Vector3 The angles in euler degrees
  */
  Vector3
  toEulerAngles() const;

  /**
  *  @brief Transform Quaternion to rotation Vector3
  *
  *  @param Vector3& vec
  * 
  *  @return Vector3
  */
  Vector3
  rotate(const Vector3& vec) const;

  /**
  *  @brief Transform Quaternion to inverse rotation Vector3
  *
  *  @param Vector3& vec
  * 
  *  @return Vector3
  */
  Vector3
  invRotate(const Vector3& vec) const;

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

  float
  lenghtSquared() const;

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

  /**
  *  @brief Transform this Quaternion to a Matrix3 rotation matrix.
  *
  *  @return Matrix3 The rotation matrix from this Quaternion.
  */
  Matrix3
  toMatrix3() const;

  static Quaternion
  fromBivector(const Vector3& vector);

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Quaternion sum operator.
  *
  *  @param const Quaternion& other: Quaternion to sum with this.
  *
  *  @return Quaternion: Result of the sum of this Quaternion and other Quaternion.
  */
  FORCEINLINE Quaternion
  operator+(const Quaternion& other) const;

  /**
  *  @brief Quaternion substraction.
  *
  *  @param const Quaternion& other: Quaternion to substract from this.
  *
  *  @return Quaternion: Result of the substraction of this Quaternion and other Quaternion.
  */
  FORCEINLINE Quaternion
  operator-(const Quaternion& other) const;

  /**
  *  @brief Quaternion multiplication with another Quaternion.
  *
  *  @param const Quaternion& other: Quaternion to multiply with this.
  *
  *  @return Quaternion: Result of the multiplication of this Quaternion and other Quaternion.
  */
  FORCEINLINE Quaternion
  operator*(const Quaternion& other) const;

  /**
  *  @brief Quaternion multiplication with a float value.
  *
  *  @param const float delta: Float value to multiply with this Quaternion.
  *
  *  @return Quaternion: Result of the multiplication of this Quaternion and the float value.
  */
  FORCEINLINE Quaternion
  operator*(const float delta) const;

  /**
  *  @brief Quaternion multiplication with an axis.
  *
  *  @param const Vector3& axis: Axis to multiply with this Quaternion.
  *
  *  @return Vector3: Result of the multiplication of this Quaternion and the axis Vector3.
  */
  FORCEINLINE Vector3
  operator*(const Vector3& axis) const;

  /**
  *  @brief Quaternion multiplication with a Matrix3.
  *
  *  @param const Matrix3& mat: Matrix3 to multiply with this Quaternion.
  *
  *  @return Matrix3: Result of the multiplication of this Quaternion and the Matrix3.
  */
  Matrix3
  operator*(const Matrix3& mat) const;

  /**
  *  @brief Operator to sum a Quaternion values and other Quaternion values and
  *         store the result in the first Quaternion.
  *
  *  @param const Quaternion& other: Quaternion to sum with this.
  *
  *  @return Quaternion&: Reference to this Quaternion after the sum operation.
  */
  FORCEINLINE Quaternion&
  operator+=(const Quaternion& other);

  /**
  *  @brief Operator to sum a Quaternion values and other Quaternion values and
  *         store the result in the first Quaternion.
  *
  *  @param const Quaternion& other: Quaternion to substract from this.
  *
  *  @return Quaternion&: Reference to this Quaternion after the substraction operation.
  */
  FORCEINLINE Quaternion&
  operator-=(const Quaternion& other);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
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

  /***************************************************************************/
  /*
  *  Static Variables
  */
  /***************************************************************************/
 public:
  /**
  * @brief Identity Quaternion value.
  */
  static const Quaternion IDENTITY;

  /**
  * @brief Zero Quaternion value.
  */
  static const Quaternion ZERO;

  /**
  * @brief Unit Quaternion value.
  */
  static const Quaternion UNIT;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

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
