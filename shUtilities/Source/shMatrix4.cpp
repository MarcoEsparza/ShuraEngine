/*************************************************************/
/*
*  @file    shMatrix4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Matrix4x4, double array, use double brackets to access to the
*           matrix values.
*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shMatrix4.h"
#include "shMath.h"

namespace shEngineSDK {
Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33)
{
  m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
  m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
  m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
  m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4::Matrix4(const Matrix4& _other)
{
  *this = _other;
}

Matrix4::Matrix4(const Quaternion& _q)
{
  float p00 = 1.0f - (2 * (Math::pow(_q.y, 2))) - (2 * (Math::pow(_q.z, 2)));
  float p01 = (2 * (_q.x * _q.y)) - (2 * (_q.w * _q.z));
  float p02 = (2 * (_q.x * _q.z)) + (2 * (_q.w * _q.y));
  float p10 = (2 * (_q.x * _q.y)) + (2 * (_q.w * _q.z));
  float p11 = 1.0f - (2 * (Math::pow(_q.x, 2))) - (2 * (Math::pow(_q.z, 2)));
  float p12 = (2 * (_q.y * _q.z)) - (2 * (_q.w * _q.x));
  float p20 = (2 * (_q.x * _q.z)) - (2 * (_q.w * _q.y));
  float p21 = (2 * (_q.y * _q.z)) + (2 * (_q.w * _q.x));
  float p22 = 1.0f - (2 * (Math::pow(_q.x, 2))) - (2 * (Math::pow(_q.y, 2)));

  m[0][0] = p00; m[0][1] = p01; m[0][2] = p02; m[0][3] = 0.0f;
  m[1][0] = p10; m[1][1] = p11; m[1][2] = p12; m[1][3] = 0.0f;
  m[2][0] = p20; m[2][1] = p21; m[2][2] = p22; m[2][3] = 0.0f;
  m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

Matrix4::Matrix4(const Vector4& _vec1,
                 const Vector4& _vec2,
                 const Vector4& _vec3,
                 const Vector4& _vec4)
{
  m[0][0] = _vec1.x; m[0][1] = _vec1.y; m[0][2] = _vec1.z; m[0][3] = _vec1.w;
  m[1][0] = _vec2.x; m[1][1] = _vec2.y; m[1][2] = _vec2.z; m[1][3] = _vec2.w;
  m[2][0] = _vec3.x; m[2][1] = _vec3.y; m[2][2] = _vec3.z; m[2][3] = _vec3.w;
  m[3][0] = _vec4.x; m[3][1] = _vec4.y; m[3][2] = _vec4.z; m[3][3] = _vec4.w;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
Matrix4::transpose(const Matrix4& _other)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      this->m[j][i] = _other.m[i][j];
    }
  }
}

Matrix4
Matrix4::getTransposed()
{
  return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                 m[0][1], m[1][1], m[2][1], m[3][1],
                 m[0][2], m[1][2], m[2][2], m[3][2],
                 m[0][3], m[1][3], m[2][3], m[3][3]);
}

Matrix4
Matrix4::getInversed()
{
  // Cofactors
  std::vector<float> inv;
  inv.resize(16);
  float det;

  inv[0] = m[1][1] * m[2][2] * m[3][3] -
           m[1][1] * m[2][3] * m[3][2] -
           m[2][1] * m[1][2] * m[3][3] +
           m[2][1] * m[1][3] * m[3][2] +
           m[3][1] * m[1][2] * m[2][3] -
           m[3][1] * m[1][3] * m[2][2];

  inv[1] = -m[0][1] * m[2][2] * m[3][3] +
            m[0][1] * m[2][3] * m[3][2] +
            m[2][1] * m[0][2] * m[3][3] -
            m[2][1] * m[0][3] * m[3][2] -
            m[3][1] * m[0][2] * m[2][3] +
            m[3][1] * m[0][3] * m[2][2];

  inv[2] = m[0][1] * m[1][2] * m[3][3] -
           m[0][1] * m[1][3] * m[3][2] -
           m[1][1] * m[0][2] * m[3][3] +
           m[1][1] * m[0][3] * m[3][2] +
           m[3][1] * m[0][2] * m[1][3] -
           m[3][1] * m[0][3] * m[1][2];

  inv[3] = -m[0][1] * m[1][2] * m[2][3] +
            m[0][1] * m[1][3] * m[2][2] +
            m[1][1] * m[0][2] * m[2][3] -
            m[1][1] * m[0][3] * m[2][2] -
            m[2][1] * m[0][2] * m[1][3] +
            m[2][1] * m[0][3] * m[1][2];

  inv[4] = -m[1][0] * m[2][2] * m[3][3] +
            m[1][0] * m[2][3] * m[3][2] +
            m[2][0] * m[1][2] * m[3][3] -
            m[2][0] * m[1][3] * m[3][2] -
            m[3][0] * m[1][2] * m[2][3] +
            m[3][0] * m[1][3] * m[2][2];

  inv[5] = m[0][0] * m[2][2] * m[3][3] -
           m[0][0] * m[2][3] * m[3][2] -
           m[2][1] * m[0][2] * m[3][3] +
           m[2][1] * m[0][3] * m[3][2] +
           m[3][1] * m[0][2] * m[2][3] -
           m[3][1] * m[0][3] * m[2][2];

  inv[6] = -m[0][0] * m[2][2] * m[3][3] +
            m[0][0] * m[2][3] * m[3][2] +
            m[1][0] * m[0][2] * m[3][3] -
            m[1][0] * m[0][3] * m[3][2] -
            m[3][0] * m[0][2] * m[1][3] +
            m[3][0] * m[0][3] * m[1][2];

  inv[7] = m[0][0] * m[1][2] * m[2][3] -
           m[0][0] * m[1][3] * m[2][2] -
           m[1][0] * m[0][2] * m[2][3] +
           m[1][0] * m[0][3] * m[2][2] +
           m[2][0] * m[0][2] * m[1][3] -
           m[2][0] * m[0][3] * m[1][2];

  inv[8] = m[1][0] * m[2][1] * m[3][3] -
           m[1][0] * m[2][3] * m[3][1] -
           m[2][0] * m[1][1] * m[3][3] +
           m[2][0] * m[1][3] * m[3][1] +
           m[3][0] * m[1][1] * m[2][3] -
           m[3][0] * m[1][3] * m[2][1];

  inv[9] = -m[0][0] * m[2][1] * m[3][3] +
            m[0][0] * m[2][3] * m[3][1] +
            m[2][0] * m[0][1] * m[3][3] -
            m[2][0] * m[0][3] * m[3][1] -
            m[3][0] * m[0][1] * m[2][3] +
            m[3][0] * m[0][3] * m[2][1];

  inv[10] = m[0][0] * m[1][1] * m[3][3] -
            m[0][0] * m[1][3] * m[3][1] -
            m[1][0] * m[0][1] * m[3][3] +
            m[1][0] * m[0][3] * m[3][1] +
            m[3][0] * m[0][1] * m[1][3] -
            m[3][0] * m[0][3] * m[1][1];

  inv[11] = -m[0][0] * m[1][1] * m[2][3] +
             m[0][0] * m[1][3] * m[2][1] +
             m[1][0] * m[0][1] * m[2][3] -
             m[1][0] * m[0][3] * m[2][1] -
             m[2][0] * m[0][1] * m[1][3] +
             m[2][0] * m[0][3] * m[1][1];

  inv[12] = -m[1][0] * m[2][1] * m[3][2] +
             m[1][0] * m[2][2] * m[3][1] +
             m[2][0] * m[1][1] * m[3][2] -
             m[2][0] * m[1][2] * m[3][1] -
             m[3][0] * m[1][1] * m[2][2] +
             m[3][0] * m[1][2] * m[2][1];

  inv[13] = m[0][0] * m[2][1] * m[3][2] -
            m[0][0] * m[2][2] * m[3][1] -
            m[2][0] * m[0][1] * m[3][2] +
            m[2][0] * m[0][2] * m[3][1] +
            m[3][0] * m[0][1] * m[2][2] -
            m[3][0] * m[0][2] * m[2][1];

  inv[14] = -m[0][0] * m[1][1] * m[3][2] +
             m[0][0] * m[1][2] * m[3][1] +
             m[1][0] * m[0][1] * m[3][2] -
             m[1][0] * m[0][2] * m[3][1] -
             m[3][0] * m[0][1] * m[1][2] +
             m[3][0] * m[0][2] * m[1][1];

  inv[15] = m[0][0] * m[1][1] * m[2][2] -
            m[0][0] * m[1][2] * m[2][1] -
            m[1][0] * m[0][1] * m[2][2] +
            m[1][0] * m[0][2] * m[2][1] +
            m[2][0] * m[0][1] * m[1][2] -
            m[2][0] * m[0][2] * m[1][1];

  det = m[0][0] * inv[0] +
        m[0][1] * inv[4] +
        m[0][2] * inv[8] +
        m[0][3] * inv[12];

  det = 1.0f / det;

  return Matrix4(inv[0] * det, inv[1] * det, inv[2] * det, inv[3] * det,
                 inv[4] * det, inv[5] * det, inv[6] * det, inv[7] * det,
                 inv[8] * det, inv[9] * det, inv[10] * det, inv[11] * det,
                 inv[12] * det, inv[13] * det, inv[14] * det, inv[15] * det);
}

Matrix4
Matrix4::createTranslationMatrix(const Vector3& _vec)
{
  return Matrix4(1.0f, 0.0f, 0.0f, _vec.x,
                 0.0f, 1.0f, 0.0f, _vec.y,
                 0.0f, 0.0f, 1.0f, _vec.z,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::quaternionToMatrix(const Quaternion& _q)
{
  float p00 = 1.0f - (2 * (Math::pow(_q.y, 2))) - (2 * (Math::pow(_q.z, 2)));
  float p01 = (2 * (_q.x * _q.y)) - (2 * (_q.w * _q.z));
  float p02 = (2 * (_q.x * _q.z)) + (2 * (_q.w * _q.y));
  float p10 = (2 * (_q.x * _q.y)) + (2 * (_q.w * _q.z));
  float p11 = 1.0f - (2 * (Math::pow(_q.x, 2))) - (2 * (Math::pow(_q.z, 2)));
  float p12 = (2 * (_q.y * _q.z)) - (2 * (_q.w * _q.x));
  float p20 = (2 * (_q.x * _q.z)) - (2 * (_q.w * _q.y));
  float p21 = (2 * (_q.y * _q.z)) + (2 * (_q.w * _q.x));
  float p22 = 1.0f - (2 * (Math::pow(_q.x, 2))) - (2 * (Math::pow(_q.y, 2)));

  return Matrix4(p00, p01, p02, 0.0f,
                 p10, p11, p12, 0.0f,
                 p20, p21, p22, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createScaleMatrix(const Vector3& _vec)
{
  return Matrix4(_vec.x, 0.0f, 0.0f, 0.0f,
                 0.0f, _vec.y, 0.0f, 0.0f,
                 0.0f, 0.0f, _vec.z, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3
Matrix4::transformDirection(const Vector3& _vec)
{
  return Vector3((m[0][0] * _vec.x) + (m[0][1] * _vec.x) + (m[0][2] * _vec.x),
                 (m[1][0] * _vec.y) + (m[1][1] * _vec.y) + (m[1][2] * _vec.y),
                 (m[2][0] * _vec.y) + (m[2][1] * _vec.y) + (m[2][2] * _vec.y));
}

Matrix4
Matrix4::createRotationXMatrix(const float& _angle)
{
  return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, Math::cos(_angle), -(Math::sin(_angle)), 0.0f,
                 0.0f, Math::sin(_angle), Math::cos(_angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationYMatrix(const float& _angle)
{
  return Matrix4(Math::cos(_angle), 0.0f, Math::sin(_angle), 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 -(Math::sin(_angle)), 0.0f, Math::cos(_angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationZMatrix(const float& _angle)
{
  return Matrix4(Math::cos(_angle), -(Math::sin(_angle)), 0.0f, 0.0f,
                 Math::sin(_angle), Math::cos(_angle), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}
}
