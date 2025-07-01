/*****************************************************************************/
/*
*  @file    shMatrix3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
*  @brief   Matrix3x3, double array, use double brackets to access to the values.
*
*  Matrix3x3, double array, use double brackets to access to the values.
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
class Quaternion;

class SH_UTILITY_EXPORT Matrix3
{
 public:
  /**
   *  @brief Default constructor.
   */
  Matrix3() = default;
  /**
   *  @brief Constructor to initialize values to the given numbers.
   */
  FORCEINLINE Matrix3(float m00, float m01, float m02,
                      float m10, float m11, float m12,
                      float m20, float m21, float m22) :
                      m{{m00, m01, m02},
                        {m10, m11, m12},
                        {m20, m21, m22}} {}
  /**
   *  @brief Copy constructor
   *
   *  @param Matrix3
   */
  Matrix3(const Matrix3& other) = default;

  /**
   *  @brief Move constructor
   *
   *  @param Matrix3
   */
  Matrix3(Matrix3&& other) noexcept = default;

  /**
   *  @brief Default destructor.
   */
  ~Matrix3() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:

  Matrix3
  transpose();

  /***************************************************************************/
  /*
  *  Operators
  */
  /***************************************************************************/
 public:
  FORCEINLINE Matrix3&
  operator*(const Matrix3& other) const;

  FORCEINLINE Vector3
  operator*(const Vector3& vec) const;

  Matrix3
  operator*(const Quaternion& quat) const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:

  /**
   *  @brief Operator to access the matrix values using double brackets.
   *         Example: m[0][0] = 1.0f;
   * 
   *  @param i Row index
   *  @param j Column index
   * 
   *  @return float& Reference to the value at (i, j)
   */
  float m[3][3];

  /***************************************************************************/
  /*
  *  Static variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Matrix3 initialized with identity values.
  */
  static const Matrix3 IDENTITY;
  
  /**
  *  @brief Matrix3 initialized with zero values.
  */
  static const Matrix3 ZEROMATRIX;
};

FORCEINLINE Matrix3&
Matrix3::operator*(const Matrix3& other) const
{
  Matrix3 result = ZEROMATRIX;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 3; ++k) {
        result.m[i][j] += this->m[i][k] * other.m[k][j];
      }
    }
  }
  return result;
}

FORCEINLINE Vector3
Matrix3::operator*(const Vector3& vec) const
{
  return Vector3(m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
                 m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
                 m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z);
}
}
