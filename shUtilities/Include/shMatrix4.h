/*************************************************************/
/*
*  @file    shMatrix4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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
class Radian;

/**
*  @brief Matrix4x4, double array, use double brackets to access to the matrix values.
* 
*  @brief Sample usage:
*  @brief Matrix4 myMatrix;
*  @brief myMatrix.m[0][0] = 1.0f;
*/
class SH_UTILITY_EXPORT Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  */
  Matrix4() = default;

  /**
  *  @brief Constructor to initialize values to the given numbers.
  */
  FORCEINLINE Matrix4(float m00, float m01, float m02, float m03,
                      float m10, float m11, float m12, float m13,
                      float m20, float m21, float m22, float m23,
                      float m30, float m31, float m32, float m33)  :
                      m{{m00, m01, m02, m03},
                        {m10, m11, m12, m13},
                        {m20, m21, m22, m23},
                        {m30, m31, m32, m33}} {}

  /**
  *  @brief Copy constructor
  *
  *  @param Matrix4
  */
  Matrix4(const Matrix4& other);

  /**
  *  @brief Constructor to create a rotation Matrix4 from a Quaternion
  *
  *  @param Quaternion
  */
  explicit Matrix4(const Quaternion& quat);

  /**
  *  @brief Constructor to create Matrix4 from four Vector4.
  *
  *  @param Vector4& vec1
  *  @param Vector4& vec2
  *  @param Vector4& vec3
  *  @param Vector4& vec4
  */
  Matrix4(const Vector4& vec1,
          const Vector4& vec2,
          const Vector4& vec3,
          const Vector4& vec4);

  /**
  *  @brief Default destructor.
  */
  ~Matrix4() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief This Matrix convert into the transpose matrix from another Matrix4.
  *
  *  @param Matrix4 _other: The another Matrix4.
  */
  void
  transpose(const Matrix4& other);

  /**
  *  @brief Transpose this Matrix4.
  *
  *  @return Matrix4 The transposed Matrix4.
  */
  Matrix4
  getTransposed();

  /**
  *  @brief Get the inverse of this Matrix4.
  *
  *  @return Matrix4 The inversed Matrix4.
  */
  Matrix4
  getInversed() const;

  /**
  *  @brief Creates a translation matrix.
  *
  *  @param Vector3 _vec: The 3 dimensional vector with the translate info.
  *
  *  @return Matrix4 The translation matrix.
  */
  Matrix4
  createTranslationMatrix(const Vector3& vec) const;

  /**
  *  @brief Convert a Quaternion to a Matrix4.
  *
  *  @param Quaternion _q: The Quaternion to convert.
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  quaternionToMatrix(const Quaternion& quat) const;

  /**
  *  @brief Convert this Matrix4 to a Quaternion.
  *
  *  @return Quaternion The resultant Quaternion from this Matrix4.
  */
  Quaternion
  toQuaternion() const;

  /**
  *  @brief Creates a scale matrix.
  *
  *  @param Vector3 vec: The 3 dimensional vector with the scale info.
  *
  *  @return Matrix4 The scale matrix.
  */
  Matrix4
  createScaleMatrix(const Vector3& vec) const;

  /**
  *  @brief Transforms the direction of a Vector3 with the Matrix4 info.
  *
  *  @param Vector3 vec: The Vector3 to be transformed.
  *
  *  @return Vector3 The transformed Vector3.
  */
  Vector3
  transformDirection(const Vector3& vec) const;

  /**
  *  @brief Create a rotation matrix for the x axis with a given angle.
  *
  *  @param Radian angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationXMatrix(const Radian angle) const;

  /**
  *  @brief Create a rotation matrix for the y axis with a given angle.
  *
  *  @param Radian angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationYMatrix(const Radian angle) const;

  /**
  *  @brief Create a rotation matrix for the z axis with a given angle.
  *
  *  @param Radian _angle: The given angle
  *
  *  @return Matrix4 The resultant rotation matrix.
  */
  Matrix4
  createRotationZMatrix(const Radian angle) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
public:
  /**
  *  @brief The sum of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator+(const Matrix4& other) const;

  /**
  *  @brief The substraction of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator-(const Matrix4& other) const;

  /**
  *  @brief The multiplication of two Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator*(const Matrix4& other) const;

  /**
  *  @brief The multiplication of a Matrix4 and a constant.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-float.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4
  operator*(const float delta) const;

  /**
  *  @brief The multiplication of a Matrix4 and a Vector3.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Vector3.
  *
  *  @return Vector3
  */
  FORCEINLINE Vector3
  operator*(const Vector3& vec) const;

  /**
  *  @brief The multiplication of a Matrix4 and a Vector4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Vector4.
  *
  *  @return Vector4
  */
  FORCEINLINE Vector4
  operator*(const Vector4& v) const;

  /**
  *  @brief Operator to sum a Matrix4 values and other Matrix4 values and store
  *         the result in the first Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4&
  operator+=(const Matrix4& other);

  /**
  *  @brief Operator to substract a Matrix4 values and other Matrix4 values and
  *         store the result in the first Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4&
  operator-=(const Matrix4& other);

  /**
  *  @brief Operator to multiply a Matrix4 values and other Matrix4 values and
  *         store the result in the first Matrix4.
  *
  *  @param lValue-Matrix4.
  *  @param rValue-Matrix4.
  *
  *  @return Matrix4
  */
  FORCEINLINE Matrix4&
  operator*=(const Matrix4& other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Use double array to access values.
  */
  float m[4][4];

  /*************************************************************/
  /*
  *  Static variables
  */
  /*************************************************************/

 public:
  /**
  *  @brief Matrix4 initialized with identity values.
  */
  static const Matrix4 IDENTITY;

  /**
  *  @brief Matrix4 initialized with zero values.
  */
  static const Matrix4 ZEROMATRIX;
};

/**
*  @brief Translation matrix.
*/
class SH_UTILITY_EXPORT TranslationMatrix : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param Vector3& position
  */
  TranslationMatrix(const Vector3& position);

  /**
  *  @brief Default destructor.
  */
  ~TranslationMatrix() = default;
};

/**
*  @brief Scale matrix.
*/
class SH_UTILITY_EXPORT ScaleMatrix : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param Vector3& scale
  */
  ScaleMatrix(const Vector3& scale);

  /**
  *  @brief Default destructor.
  */
  ~ScaleMatrix() = default;
};

/**
*  @brief Rotation in X axis.
*/
class SH_UTILITY_EXPORT RotationXMatrix : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param float radAngle
  */
  RotationXMatrix(const float radAngle);

  /**
  *  @brief Default destructor.
  */
  ~RotationXMatrix() = default;
};

/**
*  @brief Rotation in Y axis.
*/
class SH_UTILITY_EXPORT RotationYMatrix : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param float radAngle
  */
  RotationYMatrix(const float radAngle);

  /**
  *  @brief Default destructor.
  */
  ~RotationYMatrix() = default;
};

/**
*  @brief Rotation in Z axis.
*/
class SH_UTILITY_EXPORT RotationZMatrix : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param float radAngle
  */
  RotationZMatrix(const float radAngle);

  /**
  *  @brief Default destructor.
  */
  ~RotationZMatrix() = default;
};

/**
*  @brief Rotation in Z axis.
*/
class SH_UTILITY_EXPORT MatrixRotationAxis : public Matrix4
{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param Vector3& axis
  *  @param float angle
  */
  MatrixRotationAxis(Vector3 axis, float angle);

  /**
  *  @brief Default destructor.
  */
  ~MatrixRotationAxis() = default;
};

/**
*  @brief View Matrix for graphics.
*/
class SH_UTILITY_EXPORT ViewMatrix : public Matrix4
{
 public:
  /**
  *  @brief Constructor to initialize matrix values.
  * 
  *  @param Vector3& camPos
  *  @param Vector3& targetPos
  *  @param Vector3& upVector
  */
  ViewMatrix(const Vector3& camPos,
             const Vector3& targetPos,
             const Vector3& upVector);

  /**
  *  @brief Default destructor.
  */
  ~ViewMatrix() = default;
};

/**
*  @brief Projection Matrix for graphics.
*/
class SH_UTILITY_EXPORT ProjectionMatrix : public Matrix4
{
 public:
  /**
  *  @brief Constructor to initialize matrix values.
  * 
  *  @param const float halfFOV
  *  @param const float width
  *  @param const float height
  *  @param const float minZ
  *  @param const float maxZ
  */
  ProjectionMatrix(const float halfFOV,
                   const float width,
                   const float height,
                   const float minZ,
                   const float maxZ);

  /**
  *  @brief Default destructor.
  */
  ~ProjectionMatrix() = default;
};

/**
*  @brief Orthographic Projection Matrix for graphics.
*/
class SH_UTILITY_EXPORT OrthographicProjectionMatrix : public Matrix4
{
 public:
  /**
  *  @brief Constructor to initialize matrix values.
  * 
  *  @param const float left
  *  @param const float right
  *  @param const float bottom
  *  @param const float top
  *  @param const float nearZ
  *  @param const float farZ
  */
  OrthographicProjectionMatrix(const float left,
                               const float right,
                               const float bottom,
                               const float top,
                               const float nearZ,
                               const float farZ);

  /**
  *  @brief Default destructor.
  */
  ~OrthographicProjectionMatrix() = default;
};

/**
*  @brief Transform class.
*/
class SH_UTILITY_EXPORT Transform : public Matrix4
{
 public:
  /**
  *  @breif Default constructor.
  */
  Transform() { *this = Matrix4::IDENTITY; };

  /**
  *  @breif Default destructor.
  */
  ~Transform() = default;

  /**
  *  @breif Set the transform position.
  * 
  *  @param Vector3& position
  */
  void
  setPosition(const Vector3& position);

  /**
  *  @breif Set the transform rotation.
  * 
  *  @param Vector3& rotation
  */
  void
  setRotation(const Vector3& rotation);

  /**
  *  @breif Set the transform scale.
  * 
  *  @param Vector3& scale
  */
  void
  setScale(const Vector3& scale);

  /**
  *  @breif Gets the position.
  * 
  *  @return Vector3
  */
  Vector3
  getPosition() const;

  /**
  *  @breif Gets the rotation.
  * 
  *  @return Vector3
  */
  Vector3
  getRotation() const;

  /**
  *  @breif Gets the Scale.
  * 
  *  @return Vector3
  */
  Vector3
  getScale() const;

  Vector3
  transformPoint(const Vector3& point) const;

  FORCEINLINE Transform&
  operator=(const Matrix4& mat4);
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Matrix4
Matrix4::operator+(const Matrix4& other) const
{
  return Matrix4(m[0][0] + other.m[0][0],
                 m[0][1] + other.m[0][1],
                 m[0][2] + other.m[0][2],
                 m[0][3] + other.m[0][3],

                 m[1][0] + other.m[1][0],
                 m[1][1] + other.m[1][1],
                 m[1][2] + other.m[1][2],
                 m[1][3] + other.m[1][3],

                 m[2][0] + other.m[2][0],
                 m[2][1] + other.m[2][1],
                 m[2][2] + other.m[2][2],
                 m[2][3] + other.m[2][3],

                 m[3][0] + other.m[3][0],
                 m[3][1] + other.m[3][1],
                 m[3][2] + other.m[3][2],
                 m[3][3] + other.m[3][3]);
}

FORCEINLINE Matrix4
Matrix4::operator-(const Matrix4& other) const
{
  return Matrix4(m[0][0] - other.m[0][0],
                 m[0][1] - other.m[0][1],
                 m[0][2] - other.m[0][2],
                 m[0][3] - other.m[0][3],

                 m[1][0] - other.m[1][0],
                 m[1][1] - other.m[1][1],
                 m[1][2] - other.m[1][2],
                 m[1][3] - other.m[1][3],

                 m[2][0] - other.m[2][0],
                 m[2][1] - other.m[2][1],
                 m[2][2] - other.m[2][2],
                 m[2][3] - other.m[2][3],

                 m[3][0] - other.m[3][0],
                 m[3][1] - other.m[3][1],
                 m[3][2] - other.m[3][2],
                 m[3][3] - other.m[3][3]);
}

FORCEINLINE Matrix4
Matrix4::operator*(const Matrix4& other) const
{
  Matrix4 mat = ZEROMATRIX;

  for (int8 i = 0; i < 4; ++i)
  {
    for (int8 j = 0; j < 4; ++j)
    {
      for (int8 k = 0; k < 4; ++k)
      {
        mat.m[i][j] += this->m[i][k] * other.m[k][j];
      }
    }
  }

  return mat;
}

FORCEINLINE Matrix4
Matrix4::operator*(const float delta) const
{
  return Matrix4(m[0][0] * delta,
                 m[0][1] * delta,
                 m[0][2] * delta,
                 m[0][3] * delta,

                 m[1][0] * delta,
                 m[1][1] * delta,
                 m[1][2] * delta,
                 m[1][3] * delta,

                 m[2][0] * delta,
                 m[2][1] * delta,
                 m[2][2] * delta,
                 m[2][3] * delta,

                 m[3][0] * delta,
                 m[3][1] * delta,
                 m[3][2] * delta,
                 m[3][3] * delta);
}

FORCEINLINE Vector3
Matrix4::operator*(const Vector3& vec) const
{
  Vector4 tempVec(vec, 0.0f);

  tempVec = *this * tempVec;

  return Vector3(tempVec.x, tempVec.y, tempVec.z);
}

FORCEINLINE Vector4
Matrix4::operator*(const Vector4& v) const
{
  return Vector4((m[0][0] * v.x) + (m[0][1] * v.y) + (m[0][2] * v.z) + (m[0][3] * v.w),
                 (m[1][0] * v.x) + (m[1][1] * v.y) + (m[1][2] * v.z) + (m[1][3] * v.w),
                 (m[2][0] * v.x) + (m[2][1] * v.y) + (m[2][2] * v.z) + (m[2][3] * v.w),
                 (m[3][0] * v.x) + (m[3][1] * v.y) + (m[3][2] * v.z) + (m[3][3] * v.w));
}

FORCEINLINE Matrix4&
Matrix4::operator+=(const Matrix4& other)
{
  m[0][0] += other.m[0][0];
  m[0][1] += other.m[0][1];
  m[0][2] += other.m[0][2];
  m[0][3] += other.m[0][3];

  m[1][0] += other.m[1][0];
  m[1][1] += other.m[1][1];
  m[1][2] += other.m[1][2];
  m[1][3] += other.m[1][3];

  m[2][0] += other.m[2][0];
  m[2][1] += other.m[2][1];
  m[2][2] += other.m[2][2];
  m[2][3] += other.m[2][3];

  m[3][0] += other.m[3][0];
  m[3][1] += other.m[3][1];
  m[3][2] += other.m[3][2];
  m[3][3] += other.m[3][3];

  return *this;
}

FORCEINLINE Matrix4&
Matrix4::operator-=(const Matrix4& other)
{
  m[0][0] -= other.m[0][0];
  m[0][1] -= other.m[0][1];
  m[0][2] -= other.m[0][2];
  m[0][3] -= other.m[0][3];

  m[1][0] -= other.m[1][0];
  m[1][1] -= other.m[1][1];
  m[1][2] -= other.m[1][2];
  m[1][3] -= other.m[1][3];

  m[2][0] -= other.m[2][0];
  m[2][1] -= other.m[2][1];
  m[2][2] -= other.m[2][2];
  m[2][3] -= other.m[2][3];

  m[3][0] -= other.m[3][0];
  m[3][1] -= other.m[3][1];
  m[3][2] -= other.m[3][2];
  m[3][3] -= other.m[3][3];

  return *this;
}

FORCEINLINE Matrix4&
Matrix4::operator*=(const Matrix4& other)
{
  Matrix4 mat = ZEROMATRIX;

  for (int8 i = 0; i < 4; ++i)
  {
    for (int8 j = 0; j < 4; ++j)
    {
      for (int8 k = 0; k < 4; ++k)
      {
        mat.m[i][j] += this->m[i][k] * other.m[k][j];
      }
    }
  }

  *this = mat;

  return *this;
}

FORCEINLINE Transform&
Transform::operator=(const Matrix4& mat4)
{
  m[0][0] = mat4.m[0][0];
  m[0][1] = mat4.m[0][1];
  m[0][2] = mat4.m[0][2];
  m[0][3] = mat4.m[0][3];

  m[1][0] = mat4.m[1][0];
  m[1][1] = mat4.m[1][1];
  m[1][2] = mat4.m[1][2];
  m[1][3] = mat4.m[1][3];
  
  m[2][0] = mat4.m[2][0];
  m[2][1] = mat4.m[2][1];
  m[2][2] = mat4.m[2][2];
  m[2][3] = mat4.m[2][3];
  
  m[3][0] = mat4.m[3][0];
  m[3][1] = mat4.m[3][1];
  m[3][2] = mat4.m[3][2];
  m[3][3] = mat4.m[3][3];

  return *this;
}
}
