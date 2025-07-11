/*****************************************************************************/
/*
*  @file    shBivector.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Bivector class definition for representing bivectors in 3D space.
*
*  Bivector class definition for representing bivectors in 3D space.
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

/**
*  @brief Bivector class definition for representing bivectors in 3D space.
*/
class SH_UTILITY_EXPORT Bivector
{
 public:
  /**
  *  @brief Default constructor for Bivector.
  */
  Bivector() = default;

  /**
  *  @brief Constructor for Bivector with specified components.
  * 
  *  @param float yz: Component on YZ plane.
  *  @param float zx: Component on ZX plane.
  *  @param float xy: Component on XY plane.
  */
  Bivector(float yz, float zx, float xy)
    : yz(yz), zx(zx), xy(xy)
  {}

  /**
  *  @brief Copy constructor for Bivector.
  * 
  *  @param const Bivector& other: The Bivector to copy from.
  */
  Bivector(const Bivector& other) = default;

  /**
  *  @brief Destructor for Bivector.
  */
  ~Bivector() = default;
  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Magnitude of the bivector.
  * 
  *  @return float: The magnitude of the bivector.
  */
  float
  magnitude() const;

  /**
  *  @brief Wedge product of two vectors to create a bivector.
  * 
  *  @param Vector3& u: The first vector.
  *  @param Vector3& v: The second vector.
  *
  *  @return Bivector: The resulting bivector from the wedge product.
  */
  Bivector
  wedge(const Vector3& u, const Vector3& v);

  /***************************************************************************/
  /*
  *  Operators
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Assignment operator for Bivector.
  * 
  *  @param Bivector& other: The Bivector to assign from.
  *
  *  @return Bivector: A reference to this Bivector after assignment.
  */
  Bivector&
  operator=(const Bivector& other) = default;

  /**
  *  @brief Addition operator for Bivector.
  *
  *  @param Bivector& other: The Bivector to add.
  *
  *  @return Bivector: A new Bivector that is the sum of this and the other Bivector.
  */
  FORCEINLINE Bivector
  operator+(const Bivector& other) const;

  /**
  *  @brief Subtraction operator for Bivector.
  *
  *  @param Bivector& other: The Bivector to subtract.
  *
  *  @return Bivector: A new Bivector that is the difference of this and the other Bivector.
  */
  FORCEINLINE Bivector
  operator-(const Bivector& other) const;

  /**
  *  @brief Negation operator for Bivector.
  *
  *  @return Bivector: A new Bivector that is the negation of this Bivector.
  */
  FORCEINLINE Bivector
  operator-() const;

  /**
  *  @brief Scalar multiplication operator for Bivector.
  *
  *  @param float scalar: The scalar to multiply with.
  *
  *  @return Bivector: A new Bivector that is the result of multiplying this Bivector
  *                    by the scalar.
  */
  FORCEINLINE Bivector
  operator*(const float scalar) const;

  /**
  *  @brief Scalar division operator for Bivector.
  *
  *  @param float scalar: The scalar to divide by.
  *
  *  @return Bivector: A new Bivector that is the result of dividing this Bivector
  */
  FORCEINLINE Bivector
  operator/(const float scalar) const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Component on YZ plane (x)
  */
  float yz = 0.0f;

  /**
  *  @brief Component on ZX plane (y)
  */
  float zx = 0.0f;

  /**
  *  @brief Component on XY plane (z)
  */
  float xy = 0.0f;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE Bivector
Bivector::operator+(const Bivector& other) const
{
  return Bivector(yz + other.yz, zx + other.zx, xy + other.xy);
}

FORCEINLINE Bivector
Bivector::operator-(const Bivector& other) const
{
  return Bivector(yz - other.yz, zx - other.zx, xy - other.xy);
}

FORCEINLINE Bivector
Bivector::operator*(const float scalar) const
{
  return Bivector(yz * scalar, zx * scalar, xy * scalar);
}

FORCEINLINE Bivector
Bivector::operator/(const float scalar) const
{
  return Bivector(yz / scalar, zx / scalar, xy / scalar);
}

FORCEINLINE Bivector
Bivector::operator-() const
{
  return Bivector(-yz, -zx, -xy);
}
}
