/*****************************************************************************/
/*
*  @file    shMatrix4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Matrix4x4, double array, use double brackets to access to the
*           matrix values.
*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
*
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shMatrix4.h"
#include "shMath.h"
#include "shRadian.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static variables
*/
/*****************************************************************************/

const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4 Matrix4::ZEROMATRIX = Matrix4(0.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 0.0f);

/*****************************************************************************/
/*
*  Constructors
*/
/*****************************************************************************/

Matrix4::Matrix4(const Quaternion& quat)
{
  *this = quaternionToMatrix(quat);
}

Matrix4::Matrix4(const Vector4& vec1,
                 const Vector4& vec2,
                 const Vector4& vec3,
                 const Vector4& vec4)
{
  m[0][0] = vec1.x; m[0][1] = vec1.y; m[0][2] = vec1.z; m[0][3] = vec1.w;
  m[1][0] = vec2.x; m[1][1] = vec2.y; m[1][2] = vec2.z; m[1][3] = vec2.w;
  m[2][0] = vec3.x; m[2][1] = vec3.y; m[2][2] = vec3.z; m[2][3] = vec3.w;
  m[3][0] = vec4.x; m[3][1] = vec4.y; m[3][2] = vec4.z; m[3][3] = vec4.w;
}

TranslationMatrix::TranslationMatrix(const Vector3& position)
{
  m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = position.x;
  m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = position.y;
  m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = position.z;
  m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

ScaleMatrix::ScaleMatrix(const Vector3& scale)
{
  m[0][0] = scale.x; m[0][1] = 0.0f;    m[0][2] = 0.0f;    m[0][3] = 0.0f;
  m[1][0] = 0.0f;    m[1][1] = scale.y; m[1][2] = 0.0f;    m[1][3] = 0.0f;
  m[2][0] = 0.0f;    m[2][1] = 0.0f;    m[2][2] = scale.z; m[2][3] = 0.0f;
  m[3][0] = 0.0f;    m[3][1] = 0.0f;    m[3][2] = 0.0f;    m[3][3] = 1.0f;
}

RotationMatrix::RotationMatrix(const Quaternion& rotation)
{
  *this = quaternionToMatrix(rotation);
}

RotationXMatrix::RotationXMatrix(const float radAngle)
{
  m[0][0] = 1.0f;
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;

  m[1][0] = 0.0f;
  m[1][1] = Math::cos(Radian(radAngle));
  m[1][2] = -Math::sin(Radian(radAngle));
  m[1][3] = 0.0f;

  m[2][0] = 0.0f;
  m[2][1] = Math::sin(Radian(radAngle));
  m[2][2] = Math::cos(Radian(radAngle));
  m[2][3] = 0.0f;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

RotationYMatrix::RotationYMatrix(const float radAngle)
{
  m[0][0] = Math::cos(Radian(radAngle));
  m[0][1] = 0.0f;
  m[0][2] = Math::sin(Radian(radAngle));
  m[0][3] = 0.0f;

  m[1][0] = 0.0f;
  m[1][1] = 1.0f;
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;

  m[2][0] = -Math::cos(Radian(radAngle));
  m[2][1] = 0.0f;
  m[2][2] = Math::cos(Radian(radAngle));
  m[2][3] = 0.0f;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

RotationZMatrix::RotationZMatrix(const float radAngle)
{
  m[0][0] = Math::cos(Radian(radAngle));
  m[0][1] = -Math::sin(Radian(radAngle));
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;

  m[1][0] = Math::sin(Radian(radAngle));
  m[1][1] = Math::cos(Radian(radAngle));
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;

  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 1.0f;
  m[2][3] = 0.0f;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

MatrixRotationAxis::MatrixRotationAxis(const Vector3& axis, float angle)
{
  SH_ASSERT(!axis.isZero());
  SH_ASSERT(!axis.containsNaN());

  Vector3 normAxis = axis.getNormalized();

  //Compute rotation matrix from axis and angle
  float s = Math::sin(Radian(angle));
  float c = Math::cos(Radian(angle));
  float t = 1.0f - c;

  float x = normAxis.x;  float y = normAxis.y;  float z = normAxis.z;

  float tx = t * x;    float ty = t * y;    float tz = t * z;
  float txy = tx * y;  float txz = tx * z;  float tyz = ty * z;
  float sx = s * x;    float sy = s * y;    float sz = s * z;

  m[0][0] = tx * x + c;
  m[0][1] = txy + sz;
  m[0][2] = txz - sy;
  m[0][3] = 0.0f;

  m[1][0] = txy - sz;
  m[1][1] = ty * y + c;
  m[1][2] = tyz + sx;
  m[1][3] = 0.0f;

  m[2][0] = txz + sy;
  m[2][1] = tyz - sx;
  m[2][2] = tz * z + c;
  m[2][3] = 0.0f;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

ViewMatrix::ViewMatrix(const Vector3& camPos,
                       const Vector3& targetPos,
                       const Vector3& upVector)
{
  const Vector3 zAxis = (targetPos - camPos).getNormalized();
  const Vector3 xAxis = upVector.cross(zAxis).getNormalized();
  const Vector3 yAxis = zAxis.cross(xAxis);

  m[0][0] = xAxis.x; m[0][1] = yAxis.x; m[0][2] = zAxis.x; m[0][3] = 0.0f;
  m[1][0] = xAxis.y; m[1][1] = yAxis.y; m[1][2] = zAxis.y; m[1][3] = 0.0f;
  m[2][0] = xAxis.z; m[2][1] = yAxis.z; m[2][2] = zAxis.z; m[2][3] = 0.0f;

  m[3][0] = -xAxis.dot(camPos);
  m[3][1] = -yAxis.dot(camPos);
  m[3][2] = -zAxis.dot(camPos);
  m[3][3] = 1.0f;
}

ProjectionMatrix::ProjectionMatrix(const float halfFOV,
                                   const float width,
                                   const float height,
                                   const float minZ,
                                   const float maxZ)
{
  Radian rHalfFOV(halfFOV);
  float tanHFOV = Math::tan(rHalfFOV);

  m[0][0] = 1.0f / ((width / height) * tanHFOV);
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0f;

  m[1][0] = 0.0f;
  m[1][1] = 1.0f / tanHFOV;
  m[1][2] = 0.0f;
  m[1][3] = 0.0f;

  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = maxZ / (maxZ - minZ);
  m[2][3] = 1.0f;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = -(maxZ * minZ) / (maxZ - minZ);
  m[3][3] = 0.0f;
}

OrthographicProjectionMatrix::OrthographicProjectionMatrix(const float left,
                                                           const float right,
                                                           const float bottom,
                                                           const float top,
                                                           const float nearZ,
                                                           const float farZ)
{
  m[0][0] = 2.0f / (right - left);
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = -(right + left) / (right - left);

  m[1][0] = 0.0f;
  m[1][1] = 2.0f / (top - bottom);
  m[1][2] = 0.0f;
  m[1][3] = -(top + bottom) / (top - bottom);

  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = 1.0f / (farZ - nearZ);
  m[2][3] = -nearZ / (farZ - nearZ);

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
}

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

void
Matrix4::transpose()
{
  Matrix4 other = *this;

  for (int8 i = 0; i < 4; ++i)
  {
    for (int8 j = 0; j < 4; ++j)
    {
      m[j][i] = other.m[i][j];
    }
  }
}

Matrix4
Matrix4::getTransposed() const
{
  Matrix4 other = Matrix4::IDENTITY;

  for (int8 i = 0; i < 4; ++i) {
    for (int8 j = 0; j < 4; ++j) {
      other.m[j][i] = m[i][j];
    }
  }

  return other;
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
Matrix4::createTranslationMatrix(const Vector3& vec) const
{
  return Matrix4(1.0f, 0.0f, 0.0f, vec.x,
                 0.0f, 1.0f, 0.0f, vec.y,
                 0.0f, 0.0f, 1.0f, vec.z,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::quaternionToMatrix(const Quaternion& quat) const
{
  const float p00 = 1.0f - (2.0f * (Math::pow(quat.y, 2.0f))) -
                    (2.0f * (Math::pow(quat.z, 2.0f)));
  const float p01 = (2.0f * (quat.x * quat.y)) - (2.0f * (quat.w * quat.z));
  const float p02 = (2.0f * (quat.x * quat.z)) + (2.0f * (quat.w * quat.y));
  const float p10 = (2.0f * (quat.x * quat.y)) + (2.0f * (quat.w * quat.z));
  const float p11 = 1.0f - (2.0f * (Math::pow(quat.x, 2.0f))) -
                    (2.0f * (Math::pow(quat.z, 2.0f)));
  const float p12 = (2.0f * (quat.y * quat.z)) - (2.0f * (quat.w * quat.x));
  const float p20 = (2.0f * (quat.x * quat.z)) - (2.0f * (quat.w * quat.y));
  const float p21 = (2.0f * (quat.y * quat.z)) + (2.0f * (quat.w * quat.x));
  const float p22 = 1.0f - (2.0f * (Math::pow(quat.x, 2.0f))) -
                    (2.0f * (Math::pow(quat.y, 2.0f)));

  return Matrix4(p00, p01, p02, 0.0f,
                 p10, p11, p12, 0.0f,
                 p20, p21, p22, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion
Matrix4::toQuaternion() const
{
  Quaternion quat;
  const float trace = m[0][0] + m[1][1] + m[2][2];

  if (trace > 0) {
    const float step = 0.5f;
    quat.x = (m[2][1] - m[1][2]) * step;
    quat.y = (m[0][2] - m[2][0]) * step;
    quat.z = (m[1][0] - m[0][1]) * step;
    quat.w = 0.25f / step;
  }
  else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
    const float step = 2.0f * Math::sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]);
    const float invS = 1 / step;
    quat.x = 0.25f * invS;
    quat.y = (m[0][1] - m[1][0]) * invS;
    quat.z = (m[0][2] - m[2][0]) * invS;
    quat.w = (m[2][1] - m[1][2]) * invS;
  }
  else if (m[1][1] > m[2][2]) {
    const float step = 2.0f * Math::sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]);
    const float invS = 1 / step;
    quat.x = (m[0][1] + m[1][0]) * invS;
    quat.y = 0.25f * invS;
    quat.z = (m[1][2] + m[2][1]) * invS;
    quat.w = (m[0][2] - m[2][0]) * invS;
  }
  else {
    const float step = 2.0f * Math::sqrt(1.0f + m[2][2] - m[0][0] - m[1][1]);
    const float invS = 1 / step;
    quat.x = (m[0][2] + m[2][0]) * invS;
    quat.y = (m[1][2] + m[2][1]) * invS;
    quat.z = 0.25f * invS;
    quat.w = (m[1][0] - m[0][1]) * invS;
  }

  return quat;
}

Matrix4
Matrix4::createScaleMatrix(const Vector3& vec) const
{
  return Matrix4(vec.x, 0.0f, 0.0f, 0.0f,
                 0.0f, vec.y, 0.0f, 0.0f,
                 0.0f, 0.0f, vec.z, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3
Matrix4::transformDirection(const Vector3& vec) const
{
  return Vector3((m[0][0] * vec.x) + (m[0][1] * vec.x) + (m[0][2] * vec.x),
                 (m[1][0] * vec.y) + (m[1][1] * vec.y) + (m[1][2] * vec.y),
                 (m[2][0] * vec.y) + (m[2][1] * vec.y) + (m[2][2] * vec.y));
}

Matrix4
Matrix4::createRotationXMatrix(const Radian angle) const
{
  return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, Math::cos(angle), -(Math::sin(angle)), 0.0f,
                 0.0f, Math::sin(angle), Math::cos(angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationYMatrix(const Radian angle) const
{
  return Matrix4(Math::cos(angle), 0.0f, Math::sin(angle), 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 -(Math::sin(angle)), 0.0f, Math::cos(angle), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4
Matrix4::createRotationZMatrix(const Radian angle) const
{
  return Matrix4(Math::cos(angle), -(Math::sin(angle)), 0.0f, 0.0f,
                 Math::sin(angle), Math::cos(angle), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

void
Transform::setPosition(const Vector3& position)
{
  m[0][3] = position.x;
  m[1][3] = position.y;
  m[2][3] = position.z;
}

void
Transform::setRotation(const Vector3& rotation)
{
  const Matrix4 xAxis = MatrixRotationAxis(Vector3::RIGHT, rotation.x);
  const Matrix4 yAxis = MatrixRotationAxis(Vector3::UP, rotation.y);
  const Matrix4 zAxis = MatrixRotationAxis(Vector3::FORWARD, rotation.z);

  *this = zAxis;
  *this *= yAxis;
  *this *= xAxis;
}

void
Transform::setScale(const Vector3& scale)
{
  m[0][0] = scale.x;
  m[1][1] = scale.y;
  m[2][2] = scale.z;
}

Vector3
Transform::getPosition() const
{
  return Vector3(m[0][3],m[1][3],m[2][3]);
}

Vector3
Transform::getRotation() const
{
  auto q = toQuaternion();
  return q.toEulerAngles();
}

Vector3
Transform::getScale() const
{
  return Vector3(m[0][0], m[1][1], m[2][2]);
}

Vector3
Transform::transformPoint(const Vector3& point) const
{
  float tx = m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + m[0][3];
  float ty = m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + m[1][3];
  float tz = m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + m[2][3];
  float tw = m[3][0] * point.x + m[3][1] * point.y + m[3][2] * point.z + m[3][3];

  if(tw != 0.0f && tw!= 1.0f)
  {
    tx /= tw;
    ty /= tw;
    tz /= tw;
  }

  return Vector3(tx, ty, tz);
}
}
