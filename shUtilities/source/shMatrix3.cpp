/*****************************************************************************/
/*
*  @file    shMatrix3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/02
*  @brief   Matrix3x3, double array, use double brackets to access to the values.
*
*  Matrix3x3, double array, use double brackets to access to the values.
*
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shMatrix3.h"
#include "shVector3.h"
#include "shQuaternion.h"

// Temporary include for swapping
using std::swap;

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static variables
*/
/*****************************************************************************/
const Matrix3 Matrix3::IDENTITY = Matrix3(1.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f);

const Matrix3 Matrix3::ZEROMATRIX = Matrix3(0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f);

Matrix3::Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2)
{
  m[0][0] = row0.x; m[0][1] = row0.y; m[0][2] = row0.z;
  m[1][0] = row1.x; m[1][1] = row1.y; m[1][2] = row1.z;
  m[2][0] = row2.x; m[2][1] = row2.y; m[2][2] = row2.z;
}

Vector3
Matrix3::operator*(const Vector3& vec) const
{
  return Vector3(m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
                 m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
                 m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z);
}

Matrix3
Matrix3::operator*(const Quaternion& quat) const
{
  return *this * quat.toMatrix3();
}

Matrix3 Matrix3::getTranspose()
{
  Matrix3 transposed = *this;

  for(int i = 0; i < 3; ++i) {
    for (int j = i + 1; j < 3; ++j) {
      swap(transposed.m[i][j], transposed.m[j][i]);
    }
  }

  return transposed;
}

Matrix3
Matrix3::getInverse() const
{
  float det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
              m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
              m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  if (det == 0.0f) {
    //throw std::runtime_error("Matrix3::getInverse: Matrix is singular and cannot be inverted.");
    return ZEROMATRIX; // Return zero matrix if the determinant is zero
  }
  float invDet = 1.0f / det;
  Matrix3 inverse;
  inverse.m[0][0] = invDet * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
  inverse.m[0][1] = invDet * (m[0][2] * m[2][1] - m[0][1] * m[2][2]);
  inverse.m[0][2] = invDet * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
  inverse.m[1][0] = invDet * (m[1][2] * m[2][0] - m[1][0] * m[2][2]);
  inverse.m[1][1] = invDet * (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
  inverse.m[1][2] = invDet * (m[0][2] * m[1][0] - m[0][0] * m[1][2]);
  inverse.m[2][0] = invDet * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  inverse.m[2][1] = invDet * (m[0][1] * m[2][0] - m[0][0] * m[2][1]);
  inverse.m[2][2] = invDet * (m[0][0] * m[1][1] - m[0][1] * m[1][0]);

  // Ensure the inverse matrix is valid
  if (inverse.m[0][0] == 0.0f && inverse.m[0][1] == 0.0f && inverse.m[0][2] == 0.0f &&
      inverse.m[1][0] == 0.0f && inverse.m[1][1] == 0.0f && inverse.m[1][2] == 0.0f &&
      inverse.m[2][0] == 0.0f && inverse.m[2][1] == 0.0f && inverse.m[2][2] == 0.0f) {
    return ZEROMATRIX; // Return zero matrix if the inverse is invalid
  }


  return inverse;
}

Vector3
Matrix3::getColumn(uint32 index) const
{
  if (index >= 3) {
    //throw std::out_of_range("Index out of range in Matrix3::getColumn()");
    return Vector3::ZERO; // Return zero vector if index is out of range
  }
  return Vector3(m[0][index], m[1][index], m[2][index]);
}

Matrix3
Matrix3::getSkewSymmetric(const Vector3& vec)
{
  return Matrix3(0.0f, -vec.z, vec.y,
                 vec.z, 0.0f, -vec.x,
                 -vec.y, vec.x, 0.0f);
}
}
