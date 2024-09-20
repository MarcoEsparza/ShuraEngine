/*************************************************************/
/*
*  @file    shUtilitiesTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/15
*  @brief   Here is the main for the utilities test project
*
*  Here is the main for the utilities test project
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shMath.h"
#include "shVector2i.h"
#include "shVector2.h"
#include "shVector3.h"
#include "shVector4.h"
#include "shQuaternion.h"
#include "shMatrix4.h"

//#define CATCH_CONGIG_MAIN
#include "catch.hpp"

using namespace shEngineSDK;

/*************************************************************/
/*
*  Platform Types
*/
/*************************************************************/

TEST_CASE("Platform Types") {
  REQUIRE(sizeof(int8) == 1);
  REQUIRE(sizeof(int16) == 2);
  REQUIRE(sizeof(int32) == 4);
  REQUIRE(sizeof(int64) == 8);

  REQUIRE(sizeof(int8) == 1);
  REQUIRE(sizeof(int16) == 2);
  REQUIRE(sizeof(int32) == 4);
  REQUIRE(sizeof(int64) == 8);

  REQUIRE(sizeof(ANSICHAR) == 1);
  REQUIRE(sizeof(unchar) == 1);
  REQUIRE(sizeof(UNICHAR) == 2);
  REQUIRE(sizeof(WCHAR) == 2);

  REQUIRE(sizeof(TYPE_OF_NULL) == 4);

#if SH_ARCH_TYPE == SH_ARCHITECTURE_X86_64
  REQUIRE(sizeof(SIZE_T) == 8);
#elif SH_ARCH_TYPE == SH_ARCHITECTURE_X86_32
  REQUIRE(sizeof(SIZE_T) == 4);
#endif
}

/*************************************************************/
/*
*  Platform Math: functions
*/
/*************************************************************/

TEST_CASE("Platform Math") {
  REQUIRE(Math::sqrt(30.25f) == (5.5f));
  REQUIRE(Math::sqrtf(36.0f) == (6.0f));
  REQUIRE(Math::pow(4.0f, 3.0f) == (64.0f));
  REQUIRE(Math::lerp(1.0f, 10.0f, 0.5f) == (5.5f));
  REQUIRE(Math::min(12.0f, 3.0f) == (3.0f));
  REQUIRE(Math::max(12.0f, 3.0f) == (12.0f));
  REQUIRE(Math::clamp(8.0f, 10.0f, 15.0f) == (10.0f));
  REQUIRE(Math::fmod(20.0f, 6.0f) == (2.0f));
}

/*************************************************************/
/*
*  Platform Math: trigonometric functions
*/
/*************************************************************/
TEST_CASE("Trigonometric Functions") {
  float angle = 45 * Math::DEG2RAD;

  REQUIRE(Math::cos(0.0f) == (1.0f));
  REQUIRE(Math::sin(0.0f) == (0.0f));
  REQUIRE(Math::tan(angle) == (1.0f));

  REQUIRE(Math::cosh(0.0f) == (1.0f));
  REQUIRE(Math::sinh(angle) == (0.868671f));
  REQUIRE(Math::tanh(0.0f) == (0.0f));

  REQUIRE(Math::acos(1.0f) == (0.0f));
  REQUIRE(Math::asin(0.0f) == (0.0f));
  REQUIRE(Math::atan(0.0f) == (0.0f));
  
  REQUIRE(Math::acosh(1.0f) == (0.0f));
  REQUIRE(Math::asinh(0.0f) == (0.0f));
  REQUIRE(Math::atanh(0.0f) == (0.0f));
}

/*************************************************************/
/*
*  Vectors and Quaternion
*/
/*************************************************************/
TEST_CASE("Vectors") {
  // Vector2i
  Vector2i vec2i_0(5, 8);
  const Vector2i vec2i_1(6, 10);

  REQUIRE(vec2i_0.x == 5);
  REQUIRE(vec2i_0.y == 8);
  REQUIRE(vec2i_0.dot(Vector2i(2, 3)) == (34));
  REQUIRE(vec2i_0.mag() == (9.4339811321f));
  Vector2i vec2i_2 = vec2i_0.lerp(vec2i_1, 1);
  REQUIRE(vec2i_2.x == 16);
  REQUIRE(vec2i_2.y == 26);

  // Vector2
  Vector2 vec2f_0(2.0f, 3.0f);
  const Vector2 vec2f_1(4.5f, 6.2f);

  REQUIRE(vec2f_0.x == 2.0f);
  REQUIRE(vec2f_0.y == 3.0f);
  REQUIRE(vec2f_0.dot(vec2f_1) == Approx(27.6f));
  REQUIRE(vec2f_0.mag() == (3.605551275f));
  Vector2 vec2f_2 = vec2f_0.lerp(vec2f_1, 0.3f);
  REQUIRE(vec2f_2.x == 3.95f);
  REQUIRE(vec2f_2.y == 5.76f);
  vec2f_0.normalize();
  REQUIRE(vec2f_0.x == Approx(0.5547f));
  REQUIRE(vec2f_0.y == Approx(0.83205f));
  REQUIRE(vec2f_0.scalarProjection(vec2f_1) == Approx(0.99921f));
  const Vector2 vec2f_3 = vec2f_2.vectorProjection(Vector2(8.1f, 2.8f));
  REQUIRE(vec2f_3.x == Approx(5.30696f));
  REQUIRE(vec2f_3.y == Approx(1.8345f));

  // Vector3
  Vector3 vec3f_0(15.0f, 8.0f, 9.0f);
  const Vector3 vec3f_1 = Vector3(2.4f, 11.4f, 5.6f);
  Vector3* vec3f_2 = new Vector3(vec3f_1);

  REQUIRE(vec3f_0.x == 15.0f);
  REQUIRE(vec3f_0.y == 8.0f);
  REQUIRE(vec3f_0.z == 9.0f);
  REQUIRE(vec3f_2->x == 2.4f);
  REQUIRE(vec3f_2->y == 11.4f);
  REQUIRE(vec3f_2->z == 5.6f);
  vec3f_0 = vec3f_2->lerp(vec3f_1, 0.6f);
  REQUIRE(vec3f_0.x == Approx(2.4f));
  REQUIRE(vec3f_0.y == Approx(11.4f));
  REQUIRE(vec3f_0.z == Approx(5.6f));
  vec3f_0 = vec3f_1.cross(vec3f_0);
  REQUIRE(vec3f_0.x == 0.0f);
  REQUIRE(vec3f_0.y == 0.0f);
  REQUIRE(vec3f_0.z == 0.0f);
  REQUIRE(vec3f_2->mag() == Approx(12.92594f));
  vec3f_2->normalize();
  REQUIRE(vec3f_2->x == Approx(0.18567f).epsilon(0.1f));
  REQUIRE(vec3f_2->y == Approx(0.88195f));
  REQUIRE(vec3f_2->z == Approx(0.43324f));

  // Vector4
  Vector4 vec4f_0(1.0f, 7.7f, 12.6f, 2.5f);
  const Vector4 vec4f_1 = Vector4(2.4f, 11.4f, 5.6f, 0.0f);
  SPtr<Vector4> vec4f_2 = std::make_shared<Vector4>(vec4f_0);

  REQUIRE(vec4f_2->x == 1.0f);
  REQUIRE(vec4f_2->y == 7.7f);
  REQUIRE(vec4f_2->z == 12.6f);
  REQUIRE(vec4f_2->w == 2.5f);
  vec4f_0 = vec4f_1.lerp(*vec4f_2, 0.8f);
  REQUIRE(vec4f_0.x == 1.8f);
  REQUIRE(vec4f_0.y == 8.5f);
  REQUIRE(vec4f_0.z == Approx(13.4f));
  REQUIRE(vec4f_0.w == 3.3f);
  vec4f_2->normalize();
  REQUIRE(vec4f_2->x == Approx(0.06662f).epsilon(0.1f));
  REQUIRE(vec4f_2->y == Approx(0.51299f));
  REQUIRE(vec4f_2->z == Approx(0.83944f));
  REQUIRE(vec4f_2->w == Approx(0.16656f).epsilon(0.1f));
  REQUIRE(vec4f_1.mag() == Approx(12.92594f));
}

/*************************************************************/
/*
*  Quaternion
*/
/*************************************************************/

TEST_CASE("Quaternion") {
  Quaternion quat_0(90.0f, 43.4f, 65.2f, 10.1f);
  const Quaternion quat_1(71.1, 115.8f, 54.9, 1.0f); 
  Quaternion* quat_2 = new Quaternion(33.2f, 121.4f, 12.0f, 145.8f);

  Quaternion quat_3 = quat_2->inverse();
  REQUIRE(quat_3.x == Approx(-0.17204f).epsilon(0.1f));
  REQUIRE(quat_3.y == Approx(-0.62908f));
  REQUIRE(quat_3.z == Approx(-0.06218f).epsilon(0.1f));
  REQUIRE(quat_3.w == Approx(0.75551f));
  REQUIRE(quat_1.angleTo(Quaternion(23.1f, 54.0f, 167.2f, 78.5f)) == (0.0f));
  quat_3 = quat_2->rotateTowards(Quaternion(23.1f, 54.0f, 167.2f, 78.5f), 0.7f);
  Quaternion quat_4 = quat_3.slerp(*quat_2, 0.6f);
  REQUIRE(quat_4.x == Approx(33.2f));
  REQUIRE(quat_4.y == Approx(121.39999f));
  REQUIRE(quat_4.z == Approx(12.0f));
  REQUIRE(quat_4.w == Approx(145.8f));
}

/*************************************************************/
/*
*  Matrix4
*/
/*************************************************************/
TEST_CASE("Matrix4") {
  Matrix4 mat_0(11.6f, 1.0f, 122.9f, 23.9f,
                16.0f, 65.8f, 72.4f, 33.3f,
                43.5f, 1.0f, 22.0f, 45.0f,
                17.2f, 32.0f, 39.7f, 143.0f);
  const Matrix4 mat_1 = Matrix4::identity();
  const Quaternion q(132.0f, 44.2f, 98.4f, 176.3f);
  Matrix4* mat_2 = new Matrix4(q);
  Vector4 v1(43.5f, 24.8f, 88.3f, 35.7f);
  Vector4 v2(78.1f, 56.6f, 163.4f, 92.9f);
  Vector4 v3(25.1f, 67.7f, 29.6f, 68.5f);
  Vector4 v4(54.4f, 148.0, 12.7f, 1.0f);
  Matrix4 mat_3(v1, v2, v3, v4);
  Matrix4 mat_4 = Matrix4::zeroMatrix();
  const Matrix4 mat_5 = mat_2->getTransposed();

  mat_4.transpose(*mat_2);
  REQUIRE(mat_4.m[0][0] == Approx(mat_5.m[0][0]).epsilon(0.1f));
  REQUIRE(Math::abs(mat_4.m[0][1]) == Approx(mat_5.m[0][1]).epsilon(0.1f));
  REQUIRE(mat_4.m[0][2] == Approx(mat_5.m[0][2]).epsilon(0.1f));
  REQUIRE(mat_4.m[0][3] == Approx(mat_5.m[0][3]).epsilon(0.1f));

  REQUIRE(mat_4.m[1][0] == Approx(mat_5.m[1][0]).epsilon(0.1f));
  REQUIRE(mat_4.m[1][1] == Approx(mat_5.m[1][1]).epsilon(0.1f));
  REQUIRE(mat_4.m[1][2] == Approx(mat_5.m[1][2]).epsilon(0.1f));
  REQUIRE(mat_4.m[1][3] == Approx(mat_5.m[1][3]).epsilon(0.1f));

  REQUIRE(mat_4.m[2][0] == Approx(mat_5.m[2][0]).epsilon(0.1f));
  REQUIRE(mat_4.m[2][1] == Approx(mat_5.m[2][1]).epsilon(0.1f));
  REQUIRE(mat_4.m[2][2] == Approx(mat_5.m[2][2]).epsilon(0.1f));
  REQUIRE(mat_4.m[2][3] == Approx(mat_5.m[2][3]).epsilon(0.1f));

  REQUIRE(mat_4.m[3][0] == Approx(mat_5.m[3][0]));
  REQUIRE(mat_4.m[3][1] == Approx(mat_5.m[3][1]));
  REQUIRE(mat_4.m[3][2] == Approx(mat_5.m[3][2]));
  REQUIRE(mat_4.m[3][3] == Approx(mat_5.m[3][3]));

  Matrix4 mat_6 = mat_0.getInversed();
  mat_6 = mat_6 * mat_0;
  REQUIRE(mat_6.m[0][0] == Approx(mat_1.m[0][0]));
  REQUIRE(mat_6.m[0][1] == Approx(mat_1.m[0][1]));
  REQUIRE(Math::abs(mat_6.m[0][2]) == mat_1.m[0][2]);
  REQUIRE(Math::abs(mat_6.m[0][3]) == Approx(mat_1.m[0][3]).epsilon(0.1f));

  REQUIRE(mat_6.m[1][0] == Approx(mat_1.m[1][0]));
  REQUIRE(mat_6.m[1][1] == Approx(mat_1.m[1][1]));
  REQUIRE(mat_6.m[1][2] == Approx(mat_1.m[1][2]));
  REQUIRE(mat_6.m[1][3] == Approx(mat_1.m[1][3]));

  REQUIRE(mat_6.m[2][0] == Approx(mat_1.m[2][0]));
  REQUIRE(mat_6.m[2][1] == Approx(mat_1.m[2][1]));
  REQUIRE(mat_6.m[2][2] == Approx(mat_1.m[2][2]));
  REQUIRE(mat_6.m[2][3] == Approx(mat_1.m[2][3]));

  REQUIRE(mat_6.m[3][0] == Approx(mat_1.m[3][0]));
  REQUIRE(mat_6.m[3][1] == Approx(mat_1.m[3][1]));
  REQUIRE(mat_6.m[3][2] == Approx(mat_1.m[3][2]));
  REQUIRE(mat_6.m[3][3] == Approx(mat_1.m[3][3]));
}
