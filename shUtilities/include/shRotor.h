/*****************************************************************************/
/*
*  @file    shRotor.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Rotor class for geometric algebra.
*
*  Rotor class for geometric algebra.
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
#include "shBivector.h"

namespace shEngineSDK {
class Matrix3;
class Vector3;

/**
*  @brief Rotor class for geometric algebra.
*/
class SH_UTILITY_EXPORT Rotor
{
 public:
  /**
  *  @brief Default constructor for Rotor.
  */
  Rotor() = default;

  /**
  *  @brief Constructor for Rotor with scalar and bivector components.
  * 
  *  @param float w: Scalar part of the rotor.
  *  @param float bx: Bivector X part of the rotor.
  *  @param float by: Bivector Y part of the rotor.
  *  @param float bz: Bivector Z part of the rotor.
  */
  Rotor(const float w, const float bx, const float by, const float bz) :
    w(w), bx(bx), by(by), bz(bz)
  {}

  /**
  *  @brief Constructor for Rotor with angle and bivector.
  * 
  *  @param float angle: Angle of rotation in radians.
  *  @param Bivector bv: Bivector representing the plane of rotation.
  */
  Rotor(const float angle, const Bivector& bv) :
    w(angle), bx(bv.yz), by(bv.zx), bz(bv.xy)
  {}

  /**
  *  @brief Constructor for Rotor with bivector and angle.
  * 
  *  @param Bivector bvPlane: Bivector representing the plane of rotation.
  *  @param float angle: Angle of rotation in radians.
  */
  Rotor(const Bivector& bvPlane, const float angle);

  /**
  *  @brief Constructor for Rotor from two vectors.
  * 
  *  @param Vector3 from: Starting vector.
  *  @param Vector3 to: Ending vector.
  */
  Rotor(const Vector3& from, const Vector3& to);

  /**
  *  @brief Copy constructor for Rotor.
  * 
  *  @param Rotor rotor: The rotor to copy from.
  */
  Rotor(const Rotor& rotor) = default;

  /**
  *  @brief Destructor for Rotor.
  */
  ~Rotor() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Reverse the rotor.
  * 
  *  @return Rotor: The reversed rotor.
  */
  Rotor
  reverse() const;

  /**
  *  @brief Calculate the squared length of the rotor.
  * 
  *  @return float: The squared length of the rotor.
  */
  float
  lenghtSquared() const;

  /**
  *  @brief Calculate the length of the rotor.
  * 
  *  @return float: The length of the rotor.
  */
  float
  lenght() const;

  /**
  *  @brief Normalize the rotor.
  */
  void
  normalize();

  /**
  *  @brief Get a normalized version of the rotor.
  * 
  *  @return Rotor: A normalized rotor.
  */
  Rotor
  getNormalized() const;

  /**
  *  @brief Rotate a vector using the rotor.
  * 
  *  @param Vector3 vector: The vector to rotate.
  * 
  *  @return Vector3: The rotated vector.
  */
  Vector3
  rotate(const Vector3& vector) const;

  /**
  *  @brief Rotate another rotor using this rotor.
  * 
  *  @param Rotor r: The rotor to rotate.
  * 
  *  @return Rotor: The rotated rotor.
  */
  Rotor
  rotate(const Rotor& r) const;

  /**
  *  @brief Convert the rotor to a Matrix3 representation.
  * 
  *  @return Matrix3: The matrix representation of the rotor.
  */
  Matrix3
  toMatrix3() const;

  /**
  *  @brief Geometric product (for reference), produces twice the angle, negative direction
  * 
  *  @param Vector3 vec1: The first vector.
  *  @param Vector3 vec2: The second vector.
  * 
  *  @return Rotor: The rotor representing the rotation from vec1 to vec2.
  */
  Rotor
  geo(const Vector3& vec1, const Vector3& vec2) const;

  /***************************************************************************/
  /*
  *  Operator overloads
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Copy assignment operator for Rotor.
  * 
  *  @param Rotor rotor: The rotor to copy from.
  * 
  *  @return Rotor&: Reference to this rotor after assignment.
  */
  FORCEINLINE Rotor&
  operator=(const Rotor& rotor) = default;

  /**
  *  @brief Multiplication operator for Rotor.
  *
  *  @param Rotor rotor: The rotor to multiply with.
  *
  *  @return Rotor: The result of the multiplication.
  */
  FORCEINLINE Rotor
  operator*(const Rotor& rotor) const;

  /**
  *  @brief Scalar multiplication operator for Rotor.
  *
  *  @param float scalar: The scalar to multiply with.
  *
  *  @return Rotor: The result of the scalar multiplication.
  */
  FORCEINLINE Rotor
  operator*(const float scalar) const;

  /**
  *  @brief Scalar division operator for Rotor.
  *
  *  @param float scalar: The scalar to divide by.
  *
  *  @return Rotor: The result of the scalar division.
  */
  FORCEINLINE Rotor
  operator/(const float scalar) const;

  /**
  *  @brief Addition operator for Rotor.
  *
  *  @param Rotor rotor: The rotor to add.
  *
  *  @return Rotor: The result of the addition.
  */
  FORCEINLINE Rotor
  operator+(const Rotor& rotor) const;

  /**
  *  @brief Subtraction operator for Rotor.
  *
  *  @param Rotor rotor: The rotor to subtract.
  *
  *  @return Rotor: The result of the subtraction.
  */
  FORCEINLINE Rotor
  operator-(const Rotor& rotor) const;

  /**
  *  @brief Equality operator for Rotor.
  *
  *  @param Rotor& rotor: The rotor to compare with.
  *
  *  @return bool: True if the rotors are equal, false otherwise.
  */
  FORCEINLINE bool
  operator==(const Rotor& rotor) const;

  /**
  *  @brief Inequality operator for Rotor.
  *
  *  @param Rotor& rotor: The rotor to compare with.
  *
  *  @return bool: True if the rotors are not equal, false otherwise.
  */
  FORCEINLINE bool
  operator!=(const Rotor& rotor) const;

  /**
  *  @brief Multiplication assignment operator for Rotor.
  *
  *  @param Rotor rotor: The rotor to multiply with.
  *
  *  @return Rotor: The result of the multiplication assignment.
  */
  FORCEINLINE Rotor
  operator*=(const Rotor& rotor) const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Scalar part of the rotor.
  */
  float w = 0.0f;

  /**
  *  @brief Bivector X part of the rotor.
  */
  float bx = 0.0f;

  /**
  *  @brief Bivector Y part of the rotor.
  */
  float by = 0.0f;

  /**
  *  @brief Bivector Z part of the rotor.
  */
  float bz = 0.0f;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE Rotor
Rotor::operator*(const Rotor& rotor) const
{
  const Rotor& P = *this;
  Rotor result;

  result.w = P.w * rotor.w - P.bx * rotor.bx - P.by * rotor.by - P.bz * rotor.bz;
  result.bx = P.bx * rotor.w + P.w * rotor.bx + P.bz * rotor.by - P.by * rotor.bz;
  result.by = P.by * rotor.w + P.w * rotor.by - P.bz * rotor.bx + P.bx * rotor.bz;
  result.bz = P.bz * rotor.w + P.w * rotor.bz + P.by * rotor.bx - P.bx * rotor.by;

  return result;
}

FORCEINLINE Rotor
Rotor::operator*(const float scalar) const
{
  Rotor result;
  result.w = w * scalar;
  result.bx = bx * scalar;
  result.by = by * scalar;
  result.bz = bz * scalar;
  return result;
}

FORCEINLINE Rotor
Rotor::operator/(const float scalar) const
{
  if (scalar == 0.0f) {
    throw std::runtime_error("Division by zero in Rotor operator/");
  }
  Rotor result;
  result.w = w / scalar;
  result.bx = bx / scalar;
  result.by = by / scalar;
  result.bz = bz / scalar;
  return result;
}

FORCEINLINE Rotor
Rotor::operator+(const Rotor& rotor) const
{
  Rotor result;
  result.w = w + rotor.w;
  result.bx = bx + rotor.bx;
  result.by = by + rotor.by;
  result.bz = bz + rotor.bz;
  return result;
}

FORCEINLINE Rotor
Rotor::operator-(const Rotor& rotor) const
{
  Rotor result;
  result.w = w - rotor.w;
  result.bx = bx - rotor.bx;
  result.by = by - rotor.by;
  result.bz = bz - rotor.bz;
  return result;
}

FORCEINLINE bool
Rotor::operator==(const Rotor& rotor) const
{
  return (w == rotor.w && bx == rotor.bx && by == rotor.by && bz == rotor.bz);
}

FORCEINLINE bool
Rotor::operator!=(const Rotor& rotor) const
{
  return !(*this == rotor);
}

FORCEINLINE Rotor
Rotor::operator*=(const Rotor& rotor) const
{
  return (*this) * rotor * (*this).reverse();
}
}
