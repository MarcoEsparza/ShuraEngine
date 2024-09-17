/*************************************************************/
/*
*  @file    shMatrix4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/15
*  @brief   Matrix4x4, double array, use double brackets to access to the
*           matrix values.
*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
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
#include "shQuaternion.h"
#include "shVector3.h"
#include "shVector4.h"

namespace shEngineSDK {
/*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
* 
*  Sample usage:
*  Matrix4 myMatrix;
*  myMatrix.m[0][0] = 1.0f;
*/
class SH_UTILITY_EXPORT Matrix4
{
 public:
  /*
  *  Default constructor.
  */
  Matrix4() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  */
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);

  /*
  *  Copy constructor
  *
  *  @param Matrix4
  */
  Matrix4(const Matrix4& _other);

  /*
  *  Constructor to create a rotation Matrix4 from a Quaternion
  *
  *  @param Quaternion
  */
  explicit Matrix4(const Quaternion& _q);

  /*
  *  Constructor to create Matrix4 from four Vector4.
  *
  *  @param Vector4
  * 
  *  @param Vector4
  * 
  *  @param Vector4
  * 
  *  @param Vector4
  */
  Matrix4(const Vector4& _vec1,
          const Vector4& _vec2,
          const Vector4& _vec3,
          const Vector4& _vec4);

  /*
  *  Default destructor.
  */
  ~Matrix4() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Creates an identity matrix.
  *
  *  @param
  *
  *  @return Matrix4 The identity matrix.
  */
  FORCEINLINE static Matrix4
  identity();

  /*
  *  Creates a matrix with values in 0.
  *
  *  @param
  *
  *  @return Matrix4.
  */
  FORCEINLINE static Matrix4
  zeroMatrix();

  /*
  *  This Matrix convert into the transpose matrix from another Matrix4.
  *
  *  @param Matrix4 _other: The another Matrix4.
  *
  *  @return
  */
  void
  transpose(const Matrix4& _other);

  /*
  *  Transpose this Matrix4.
  *
  *  @param
  *
  *  @return Matrix4 The transposed Matrix4.
  */
  Matrix4
  getTransposed();

  /*
  *  Get the inverse of this Matrix4.
  *
  *  @param
  *
  *  @return Matrix4 The inversed Matrix4.
  */
  Matrix4
  getInversed();

  /*
  *  Creates a translation matrix.
  *
  *  @param Vector3 _vec: The 3 dimensional vector with the translate info.
  *
  *  @return Matrix4 The translation matrix.
  */
  Matrix4
  createTranslationMatrix(const Vector3& _vec);

  /*
  *  Convert a Quaternion to a Matrix4.
  *
  *  @param Quaternion _q: The Quaternion to convert.
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  quaternionToMatrix(const Quaternion& _q);

  /*
  *  Convert this Matrix4 to a Quaternion.
  *
  *  @param
  *
  *  @return Quaternion The resultant Quaternion from this Matrix4.
  */
  Quaternion
  matrixToQuaternion();

  /*
  *  Creates a scale matrix.
  *
  *  @param Vector3 _vec: The 3 dimensional vector with the scale info.
  *
  *  @return Matrix4 The scale matrix.
  */
  Matrix4
  createScaleMatrix(const Vector3& _vec);

  /*
  *  Transorms the direction of a Vector3 with the Matrix4 info.
  *
  *  @param Vector3 _vec: The Vector3 to be transformed.
  *
  *  @return Vector3 The transformed Vector3.
  */
  Vector3
  transformDirection(const Vector3& _vec);

  /*
  *  Create a rotation matrix for the x axis with a given angle.
  *
  *  @param  float _angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationXMatrix(const float& _angle);

  /*
  *  Create a rotation matrix for the y axis with a given angle.
  *
  *  @param  float _angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationYMatrix(const float& _angle);

  /*
  *  Create a rotation matrix for the z axis with a given angle.
  *
  *  @param  float _angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationZMatrix(const float& _angle);

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
public:
  /*
  *  The sum of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator+(const Matrix4& _other);

  /*
  *  The substraction of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator-(const Matrix4& _other);

  /*
  *  The multiplication of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator*(const Matrix4& _other);

  /*
  *  The multiplication of a Matrix4 and a constant.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-float.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator*(const float& _delta);

  /*
  *  Operator to sum a Matrix4 values and other Matrix4 values and store
  *  the result in the first Matrix4.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator+=(const Matrix4& _other);

  /*
  *  Operator to substract a Matrix4 values and other Matrix4 values and
  *  store the result in the first Matrix4.
  *
  *  @param lValue-Matrix4.
  *
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator-=(const Matrix4& _other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /*
  *  Use double array to access values.
  */
  float m[4][4];
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Matrix4
Matrix4::identity()
{
  return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix4
Matrix4::zeroMatrix()
{
  return Matrix4(0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f);
}

FORCEINLINE Matrix4
Matrix4::operator+(const Matrix4& _other)
{
  return Matrix4(m[0][0] + _other.m[0][0],
                 m[0][1] + _other.m[0][1],
                 m[0][2] + _other.m[0][2],
                 m[0][3] + _other.m[0][3],

                 m[1][0] + _other.m[1][0],
                 m[1][1] + _other.m[1][1],
                 m[1][2] + _other.m[1][2],
                 m[1][3] + _other.m[1][3],

                 m[2][0] + _other.m[2][0],
                 m[2][1] + _other.m[2][1],
                 m[2][2] + _other.m[2][2],
                 m[2][3] + _other.m[2][3],

                 m[3][0] + _other.m[3][0],
                 m[3][1] + _other.m[3][1],
                 m[3][2] + _other.m[3][2],
                 m[3][3] + _other.m[3][3]);
}

FORCEINLINE Matrix4
Matrix4::operator-(const Matrix4& _other)
{
  return Matrix4(m[0][0] - _other.m[0][0],
                 m[0][1] - _other.m[0][1],
                 m[0][2] - _other.m[0][2],
                 m[0][3] - _other.m[0][3],

                 m[1][0] - _other.m[1][0],
                 m[1][1] - _other.m[1][1],
                 m[1][2] - _other.m[1][2],
                 m[1][3] - _other.m[1][3],

                 m[2][0] - _other.m[2][0],
                 m[2][1] - _other.m[2][1],
                 m[2][2] - _other.m[2][2],
                 m[2][3] - _other.m[2][3],

                 m[3][0] - _other.m[3][0],
                 m[3][1] - _other.m[3][1],
                 m[3][2] - _other.m[3][2],
                 m[3][3] - _other.m[3][3]);
}

FORCEINLINE Matrix4
Matrix4::operator*(const Matrix4& _other)
{
  Matrix4 mat = Matrix4::zeroMatrix();

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      for (int k = 0; k < 4; ++k)
      {
        mat.m[i][j] += this->m[i][k] * _other.m[k][j];
      }
    }
  }

  return mat;
}

FORCEINLINE Matrix4
Matrix4::operator*(const float& _delta)
{
  return Matrix4(m[0][0] * _delta,
                 m[0][1] * _delta,
                 m[0][2] * _delta,
                 m[0][3] * _delta,

                 m[1][0] * _delta,
                 m[1][1] * _delta,
                 m[1][2] * _delta,
                 m[1][3] * _delta,

                 m[2][0] * _delta,
                 m[2][1] * _delta,
                 m[2][2] * _delta,
                 m[2][3] * _delta,

                 m[3][0] * _delta,
                 m[3][1] * _delta,
                 m[3][2] * _delta,
                 m[3][3] * _delta);
}

FORCEINLINE Matrix4
Matrix4::operator+=(const Matrix4& _other)
{
  m[0][0] += _other.m[0][0];
  m[0][1] += _other.m[0][1];
  m[0][2] += _other.m[0][2];
  m[0][3] += _other.m[0][3];

  m[1][0] += _other.m[1][0];
  m[1][1] += _other.m[1][1];
  m[1][2] += _other.m[1][2];
  m[1][3] += _other.m[1][3];

  m[2][0] += _other.m[2][0];
  m[2][1] += _other.m[2][1];
  m[2][2] += _other.m[2][2];
  m[2][3] += _other.m[2][3];

  m[3][0] += _other.m[3][0];
  m[3][1] += _other.m[3][1];
  m[3][2] += _other.m[3][2];
  m[3][3] += _other.m[3][3];

  return *this;
}

FORCEINLINE Matrix4
Matrix4::operator-=(const Matrix4& _other)
{
  m[0][0] -= _other.m[0][0];
  m[0][1] -= _other.m[0][1];
  m[0][2] -= _other.m[0][2];
  m[0][3] -= _other.m[0][3];

  m[1][0] -= _other.m[1][0];
  m[1][1] -= _other.m[1][1];
  m[1][2] -= _other.m[1][2];
  m[1][3] -= _other.m[1][3];

  m[2][0] -= _other.m[2][0];
  m[2][1] -= _other.m[2][1];
  m[2][2] -= _other.m[2][2];
  m[2][3] -= _other.m[2][3];

  m[3][0] -= _other.m[3][0];
  m[3][1] -= _other.m[3][1];
  m[3][2] -= _other.m[3][2];
  m[3][3] -= _other.m[3][3];

  return *this;
}
}
