/*************************************************************/
/*
*  @file    shMatrix4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
Matrix4::Matrix4(const Matrix4& _other)
{
  m[0][0] = _other.m[0][0];
  m[0][1] = _other.m[0][1];
  m[0][2] = _other.m[0][2];
  m[0][3] = _other.m[0][3];

  m[1][0] = _other.m[1][0];
  m[1][1] = _other.m[1][1];
  m[1][2] = _other.m[1][2];
  m[1][3] = _other.m[1][3];

  m[2][0] = _other.m[2][0];
  m[2][1] = _other.m[2][1];
  m[2][2] = _other.m[2][2];
  m[2][3] = _other.m[2][3];

  m[3][0] = _other.m[3][0];
  m[3][1] = _other.m[3][1];
  m[3][2] = _other.m[3][2];
  m[3][3] = _other.m[3][3];
}

Matrix4::Matrix4(const Quaternion& _q)
{
  *this = quaternionToMatrix(_q);
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
  for (int8 i = 0; i < 4; ++i)
  {
    for (int8 j = 0; j < 4; ++j)
    {
      m[j][i] = _other.m[i][j];
    }
  }
}

Matrix4
Matrix4::getTransposed() const
{
  return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                 m[0][1], m[1][1], m[2][1], m[3][1],
                 m[0][2], m[1][2], m[2][2], m[3][2],
                 m[0][3], m[1][3], m[2][3], m[3][3]);
}

Matrix4
Matrix4::getInversed() const
{
  // Cofactors
  Vector<float> inv;
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
Matrix4::createTranslationMatrix(const Vector3& _vec) const
{
  return Matrix4(1.0f, 0.0f, 0.0f, _vec.x,
                 0.0f, 1.0f, 0.0f, _vec.y,
                 0.0f, 0.0f, 1.0f, _vec.z,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::quaternionToMatrix(const Quaternion& _q) const
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

Quaternion
Matrix4::matrixToQuaternion() const
{
  Quaternion q;
  float trace = m[0][0] + m[1][1] + m[2][2];

  if (trace > 0) {
    float s = 0.5f;
    q.x = (m[2][1] - m[1][2]) * s;
    q.y = (m[0][2] - m[2][0]) * s;
    q.z = (m[1][0] - m[0][1]) * s;
    q.w = 0.25f / s;
  }
  else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
    float s = 2.0f * Math::sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
    float invS = 1 / s;
    q.x = 0.25f * invS;
    q.y = (m[0][1] - m[1][0]) * invS;
    q.z = (m[0][2] - m[2][0]) * invS;
    q.w = (m[2][1] - m[1][2]) * invS;
  }
  else if (m[1][1] > m[2][2]) {
    float s = 2.0f * Math::sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
    float invS = 1 / s;
    q.x = (m[0][1] + m[1][0]) * invS;
    q.y = 0.25f * invS;
    q.z = (m[1][2] + m[2][1]) * invS;
    q.w = (m[0][2] - m[2][0]) * invS;
  }
  else {
    float s = 2.0f * Math::sqrtf(1.0f + m[2][2] - m[0][0] - m[1][1]);
    float invS = 1 / s;
    q.x = (m[0][2] + m[2][0]) * invS;
    q.y = (m[1][2] + m[2][1]) * invS;
    q.z = 0.25f * invS;
    q.w = (m[1][0] - m[0][1]) * invS;
  }

  return q;
}

Matrix4
Matrix4::createScaleMatrix(const Vector3& _vec) const
{
  return Matrix4(_vec.x, 0.0f, 0.0f, 0.0f,
                 0.0f, _vec.y, 0.0f, 0.0f,
                 0.0f, 0.0f, _vec.z, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3
Matrix4::transformDirection(const Vector3& _vec) const
{
  return Vector3((m[0][0] * _vec.x) + (m[0][1] * _vec.x) + (m[0][2] * _vec.x),
                 (m[1][0] * _vec.y) + (m[1][1] * _vec.y) + (m[1][2] * _vec.y),
                 (m[2][0] * _vec.y) + (m[2][1] * _vec.y) + (m[2][2] * _vec.y));
}

Matrix4
Matrix4::createRotationXMatrix(const float _angle) const
{
  return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, Math::cos(_angle), -(Math::sin(_angle)), 0.0f,
                 0.0f, Math::sin(_angle), Math::cos(_angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationYMatrix(const float _angle) const
{
  return Matrix4(Math::cos(_angle), 0.0f, Math::sin(_angle), 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 -(Math::sin(_angle)), 0.0f, Math::cos(_angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationZMatrix(const float _angle) const
{
  return Matrix4(Math::cos(_angle), -(Math::sin(_angle)), 0.0f, 0.0f,
                 Math::sin(_angle), Math::cos(_angle), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}
}
