/*****************************************************************************/
/*
*  @file    shMatrix3.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
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

Matrix3
Matrix3::operator*(const Quaternion& quat) const
{
  return *this * quat.toMatrix3();
}

Matrix3 Matrix3::transpose()
{
  Matrix3 transposed = *this;

  for(int i = 0; i < 3; ++i) {
    for (int j = i + 1; j < 3; ++j) {
      swap(transposed.m[i][j], transposed.m[j][i]);
    }
  }

  return transposed;
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
}
