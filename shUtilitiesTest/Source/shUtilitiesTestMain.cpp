/*************************************************************/
/*
*  @file    shUtilitiesTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
#include "shPrerequisitesUtilities.h"
#include "shMath.h"
#include "shVector2i.h"
#include "shVector2.h"
#include "shVector3.h"
#include "shVector4.h"
#include "shQuaternion.h"
#include "shMatrix4.h"
#include "shModule.h"

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

#if SH_ARCH_TYPE == SH_ARQUITECTURE_X86_64
  REQUIRE(sizeof(SIZE_T) == 8);
#elif SH_ARCH_TYPE == SH_ARQUITECTURE_X86_32
  REQUIRE(sizeof(SIZE_T) == 4);
#endif
}

/*************************************************************/
/*
*  Platform Math: functions
*/
/*************************************************************/
TEST_CASE("Platform Math") {
  REQUIRE(Math::sqrt(130.25f) == Approx(11.41271f));
  REQUIRE(Math::pow(15.21f, 3.5f) == Approx(13723.10059f));
  REQUIRE(Math::lerp(11.0f, 43.2f, 2.5f) == (91.5f));
  REQUIRE(Math::min(12.0f, 3.0f) == (3.0f));
  REQUIRE(Math::max(12.0f, 3.0f) == (12.0f));
  REQUIRE(Math::clamp(8.3f, 130.2f, 27.8f) == (130.2f));
  REQUIRE(Math::fmod(154.0f, 41.1f) == Approx(30.7f));
  REQUIRE(Math::PI == Approx(3.1415).epsilon(0.01f));
}

/*************************************************************/
/*
*  Platform Math: trigonometric functions
*/
/*************************************************************/
TEST_CASE("Trigonometric Functions") {
  float angle = 45 * Math::DEG2RAD;
  float angle1 = 90 * Math::DEG2RAD;
  float angle2 = 172 * Math::DEG2RAD;
  float angle3 = 13 * Math::DEG2RAD;
  float angle4 = 65 * Math::DEG2RAD;

  REQUIRE(Math::cos(angle2) == Approx(-0.99027f));
  REQUIRE(Math::sin(angle1) == (1.0f));
  REQUIRE(Math::tan(angle) == (1.0f));

  REQUIRE(Math::cosh(angle3) == Approx(1.02585f));
  REQUIRE(Math::sinh(angle) == (0.868671f));
  REQUIRE(Math::tanh(angle2) == Approx(0.99507f));

  REQUIRE(Math::acos(angle) == Approx(0.66746f));
  REQUIRE(Math::asin(angle3) == Approx(0.22889f).epsilon(0.1f));
  REQUIRE(Math::atan(angle3) == Approx(0.22312f).epsilon(0.1f));
  
  REQUIRE(Math::acosh(angle4) == Approx(0.51294f));
  REQUIRE(Math::asinh(angle1) == Approx(1.2334f));
  REQUIRE(Math::atanh(angle3) == Approx(0.23091f));
}

/*************************************************************/
/*
*  Vector2i
*/
/*************************************************************/
TEST_CASE("Vector2i") {
  Vector2i vec2i_0(5, 8);
  const Vector2i vec2i_1(6, 10);

  REQUIRE(vec2i_0.x == 5);
  REQUIRE(vec2i_0.y == 8);
  REQUIRE(vec2i_0.dot(Vector2i(2, 3)) == (34));
  REQUIRE(vec2i_0.mag() == (9.4339811321f));
  Vector2i vec2i_2 = vec2i_0.lerp(vec2i_1, 1);
  REQUIRE(vec2i_2.x == 16);
  REQUIRE(vec2i_2.y == 26);
}

/*************************************************************/
/*
*  Vector2
*/
/*************************************************************/
TEST_CASE("Vector2") {
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
  const Vector2 vec2f_4 = vec2f_3.getNormalized();
  REQUIRE(vec2f_4.x == Approx(0.94512f));
  REQUIRE(vec2f_4.y == Approx(0.32671f));
}

/*************************************************************/
/*
*  Vector3
*/
/*************************************************************/
TEST_CASE("Vector3") {
  Vector3 vec3f_0(15.0f, 8.0f, 9.0f);
  const Vector3 vec3f_1 = Vector3(2.4f, 11.4f, 5.6f);
  Vector3* vec3f_2 = new Vector3(vec3f_1);

  REQUIRE(vec3f_0.x == 15.0f);
  REQUIRE(vec3f_0.y == 8.0f);
  REQUIRE(vec3f_0.z == 9.0f);
  REQUIRE(vec3f_2->x == 2.4f);
  REQUIRE(vec3f_2->y == 11.4f);
  REQUIRE(vec3f_2->z == 5.6f);
  REQUIRE(vec3f_2->dot(vec3f_0) == Approx(177.59999f));
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
  const Vector3 vec3f_3 = vec3f_1.getNormalized();
  REQUIRE(vec3f_3.x == Approx(0.18567f).epsilon(0.1f));
  REQUIRE(vec3f_3.y == Approx(0.88195f));
  REQUIRE(vec3f_3.z == Approx(0.43324f));
}

/*************************************************************/
/*
*  Vector4
*/
/*************************************************************/
TEST_CASE("Vector4") {
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
  const Vector4 vec4f_3 = vec4f_1.getNormalized();
  REQUIRE(vec4f_3.x == Approx(0.18567f).epsilon(0.1f));
  REQUIRE(vec4f_3.y == Approx(0.88195f));
  REQUIRE(vec4f_3.z == Approx(0.43324f));
  REQUIRE(vec4f_3.w == Approx(0.0f));
  REQUIRE(vec4f_0.dot(vec4f_1) == Approx(176.25999f));
}

/*************************************************************/
/*
*  Quaternion
*/
/*************************************************************/

TEST_CASE("Quaternion") {
  Quaternion quat_0(1.0f, 43.4f, 65.2f, 10.1f);
  Quaternion quat_1(1.0f, 115.8f, 54.9f, 90.0f);
  Quaternion* quat_2 = new Quaternion(1.0f, 121.4f, 12.0f, 145.8f);
   // Inverse
  Quaternion quat_3 = quat_2->inverse();
  REQUIRE(quat_3.w == Approx(-0.63859f));
  REQUIRE(quat_3.x == Approx(-0.06312f).epsilon(0.1f));
  REQUIRE(quat_3.y == Approx(-0.76694f));
  REQUIRE(quat_3.z == Approx(0.00526f).epsilon(0.1f));
  
  //Angles
  REQUIRE(quat_1.angleTo(Quaternion(0.0f, 54.0f, 167.2f, 78.5f)) == (0.0f));
  quat_3 = quat_2->rotateTowards(Quaternion(1.0f, 54.0f, 167.2f, 78.5f), 0.7f);
  Quaternion quat_4 = quat_3.slerp(*quat_2, 0.6f);
  REQUIRE(quat_4.w == Approx(1.0f));
  REQUIRE(quat_4.x == Approx(121.4f));
  REQUIRE(quat_4.y == Approx(12.0f));
  REQUIRE(quat_4.z == Approx(145.8f));

  // Conversions
  const Vector3 vec3f = quat_4.toEulerAngles();
  REQUIRE(vec3f.x == Approx(121.4f));
  REQUIRE(vec3f.y == Approx(12.0f));
  REQUIRE(vec3f.z == Approx(145.8f));
  const Quaternion quat_5 = quat_0.fromEulerAngles(vec3f);
  REQUIRE(quat_5.w == Approx(0.26683f));
  REQUIRE(quat_5.x == Approx(0.73963f));
  REQUIRE(quat_5.y == Approx(0.36875f));
  REQUIRE(quat_5.z == Approx(0.49576f));
  Quaternion quat_6;
  quat_6.fromAngle(vec3f);
  REQUIRE(quat_6.w == Approx(quat_5.w));
  REQUIRE(quat_6.x == Approx(quat_5.x));
  REQUIRE(quat_6.y == Approx(quat_5.y));
  REQUIRE(quat_6.z == Approx(quat_5.z));

  // Lenght and normalize
  REQUIRE(quat_5.lenght() == Approx(1.0f));
  quat_6.normalize();
  const Quaternion quat_7 = quat_5.getNormalized();
  REQUIRE(quat_6.w == Approx(0.26683f));
  REQUIRE(quat_6.x == Approx(0.73963f));
  REQUIRE(quat_6.y == Approx(0.36875f));
  REQUIRE(quat_6.z == Approx(0.49576f));
  REQUIRE(quat_7.w == Approx(quat_6.w));
  REQUIRE(quat_7.x == Approx(quat_6.x));
  REQUIRE(quat_7.y == Approx(quat_6.y));
  REQUIRE(quat_7.z == Approx(quat_6.z));
  
  // Dot
  REQUIRE(quat_7.dot(quat_4) == Approx(166.76457f));
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
  const Matrix4 mat_1 = identity;
  const Quaternion q(132.0f, 44.2f, 98.4f, 176.3f);
  Matrix4* mat_2 = new Matrix4(q);
  Vector4 v1(43.5f, 24.8f, 88.3f, 35.7f);
  Vector4 v2(78.1f, 56.6f, 163.4f, 92.9f);
  Vector4 v3(25.1f, 67.7f, 29.6f, 68.5f);
  Vector4 v4(54.4f, 148.0, 12.7f, 1.0f);
  Matrix4 mat_3(v1, v2, v3, v4);
  Matrix4 mat_4 = zeroMatrix;
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

  const Matrix4 mat_7 = mat_1.createTranslationMatrix(Vector3(23.0f, 14.2f, 1.0f));
  REQUIRE(mat_7.m[0][0] == Approx(1.0f));
  REQUIRE(mat_7.m[0][1] == Approx(0.0f));
  REQUIRE(mat_7.m[0][2] == Approx(0.0f));
  REQUIRE(mat_7.m[0][3] == Approx(23.0f));

  REQUIRE(mat_7.m[1][0] == Approx(0.0f));
  REQUIRE(mat_7.m[1][1] == Approx(1.0f));
  REQUIRE(mat_7.m[1][2] == Approx(0.0f));
  REQUIRE(mat_7.m[1][3] == Approx(14.2f));

  REQUIRE(mat_7.m[2][0] == Approx(0.0f));
  REQUIRE(mat_7.m[2][1] == Approx(0.0f));
  REQUIRE(mat_7.m[2][2] == Approx(1.0f));
  REQUIRE(mat_7.m[2][3] == Approx(1.0f));

  REQUIRE(mat_7.m[3][0] == Approx(0.0f));
  REQUIRE(mat_7.m[3][1] == Approx(0.0f));
  REQUIRE(mat_7.m[3][2] == Approx(0.0f));
  REQUIRE(mat_7.m[3][3] == Approx(1.0f));

  const Matrix4 mat_8 = mat_1.createScaleMatrix(Vector3(2.0f, 1.5f, 3.4f));
  REQUIRE(mat_8.m[0][0] == Approx(2.0f));
  REQUIRE(mat_8.m[0][1] == Approx(0.0f));
  REQUIRE(mat_8.m[0][2] == Approx(0.0f));
  REQUIRE(mat_8.m[0][3] == Approx(0.0f));

  REQUIRE(mat_8.m[1][0] == Approx(0.0f));
  REQUIRE(mat_8.m[1][1] == Approx(1.5f));
  REQUIRE(mat_8.m[1][2] == Approx(0.0f));
  REQUIRE(mat_8.m[1][3] == Approx(0.0f));

  REQUIRE(mat_8.m[2][0] == Approx(0.0f));
  REQUIRE(mat_8.m[2][1] == Approx(0.0f));
  REQUIRE(mat_8.m[2][2] == Approx(3.4f));
  REQUIRE(mat_8.m[2][3] == Approx(0.0f));

  REQUIRE(mat_8.m[3][0] == Approx(0.0f));
  REQUIRE(mat_8.m[3][1] == Approx(0.0f));
  REQUIRE(mat_8.m[3][2] == Approx(0.0f));
  REQUIRE(mat_8.m[3][3] == Approx(1.0f));

  Quaternion quat = mat_3.toQuaternion();
  REQUIRE(quat.w == Approx(0.5f));
  REQUIRE(quat.x == Approx(-47.85f));
  REQUIRE(quat.y == Approx(31.6f));
  REQUIRE(quat.z == Approx(26.65f));

  Vector3 vec3f_0(123.4f, 67.3f, 90.0f);
  Vector3 vec3f_1 = mat_0.transformDirection(vec3f_0);
  REQUIRE(vec3f_1.x == Approx(16720.70117f));
  REQUIRE(vec3f_1.y == Approx(10377.66113f));
  REQUIRE(vec3f_1.z == Approx(4475.4502f));

  Matrix4 mat_9 = mat_1.createRotationXMatrix(87.0f * Math::DEG2RAD);
  REQUIRE(mat_9.m[0][0] == Approx(1.0f));
  REQUIRE(mat_9.m[0][1] == Approx(0.0f));
  REQUIRE(mat_9.m[0][2] == Approx(0.0f));
  REQUIRE(mat_9.m[0][3] == Approx(0.0f));

  REQUIRE(mat_9.m[1][0] == Approx(0.0f));
  REQUIRE(mat_9.m[1][1] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_9.m[1][2] == Approx(-0.99863f));
  REQUIRE(mat_9.m[1][3] == Approx(0.0f));

  REQUIRE(mat_9.m[2][0] == Approx(0.0f));
  REQUIRE(mat_9.m[2][1] == Approx(0.99863f));
  REQUIRE(mat_9.m[2][2] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_9.m[2][3] == Approx(0.0f));

  REQUIRE(mat_9.m[3][0] == Approx(0.0f));
  REQUIRE(mat_9.m[3][1] == Approx(0.0f));
  REQUIRE(mat_9.m[3][2] == Approx(0.0f));
  REQUIRE(mat_9.m[3][3] == Approx(1.0f));

  Matrix4 mat_10 = mat_1.createRotationYMatrix(87.0f * Math::DEG2RAD);
  REQUIRE(mat_10.m[0][0] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_10.m[0][1] == Approx(0.0f));
  REQUIRE(mat_10.m[0][2] == Approx(0.99863f).epsilon(0.1f));
  REQUIRE(mat_10.m[0][3] == Approx(0.0f));

  REQUIRE(mat_10.m[1][0] == Approx(0.0f));
  REQUIRE(mat_10.m[1][1] == Approx(1.0f));
  REQUIRE(mat_10.m[1][2] == Approx(0.0f));
  REQUIRE(mat_10.m[1][3] == Approx(0.0f));

  REQUIRE(mat_10.m[2][0] == Approx(-0.99863f).epsilon(0.1f));
  REQUIRE(mat_10.m[2][1] == Approx(0.0f));
  REQUIRE(mat_10.m[2][2] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_10.m[2][3] == Approx(0.0f));

  REQUIRE(mat_10.m[3][0] == Approx(0.0f));
  REQUIRE(mat_10.m[3][1] == Approx(0.0f));
  REQUIRE(mat_10.m[3][2] == Approx(0.0f));
  REQUIRE(mat_10.m[3][3] == Approx(1.0f));

  Matrix4 mat_11 = mat_1.createRotationZMatrix(87.0f * Math::DEG2RAD);
  REQUIRE(mat_11.m[0][0] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_11.m[0][1] == Approx(-0.99863f).epsilon(0.1f));
  REQUIRE(mat_11.m[0][2] == Approx(0.0f));
  REQUIRE(mat_11.m[0][3] == Approx(0.0f));

  REQUIRE(mat_11.m[1][0] == Approx(0.99863f).epsilon(0.1f));
  REQUIRE(mat_11.m[1][1] == Approx(0.05234f).epsilon(0.1f));
  REQUIRE(mat_11.m[1][2] == Approx(0.0f));
  REQUIRE(mat_11.m[1][3] == Approx(0.0f));

  REQUIRE(mat_11.m[2][0] == Approx(0.0f));
  REQUIRE(mat_11.m[2][1] == Approx(0.0f));
  REQUIRE(mat_11.m[2][2] == Approx(1.0f));
  REQUIRE(mat_11.m[2][3] == Approx(0.0f));

  REQUIRE(mat_11.m[3][0] == Approx(0.0f));
  REQUIRE(mat_11.m[3][1] == Approx(0.0f));
  REQUIRE(mat_11.m[3][2] == Approx(0.0f));
  REQUIRE(mat_11.m[3][3] == Approx(1.0f));
}

/*************************************************************/
/*
*  BoxAAB
*/
/*************************************************************/
TEST_CASE("BoxAAB") {
  Vector3 pos1(3.0f, 1.5f, 2.0f);
  Vector3 size1(3.0f, 1.5f, 2.0f);
  Vector3 pos2(3.0f, 1.5f, 2.0f);
  Vector3 size2(3.0f, 1.5f, 2.0f);
  Vector3 point(4.5f, 2.5f, 2.0f);
  Quaternion quat;

  shBoxAAB box1(pos1, size1);
  shBoxAAB box2;
  box2.setPosition(pos2);
  box2.setSize(size2);

  shBoxOBB obb(pos1, quat, size1);
  shCapsule cap1(2.0f,Vector3(3.1f, 4.3f, 2.0f), Vector3(3.1f,1.8f, 2.0f));
  shPlane plane(Vector3(3.1f, 4.3f, 2.0f), 1.0f);
  shRect rect(Vector2(2.8f, 1.7f), Vector2(1.0f, 1.0f));
  shSphere sph(pos2, 1.5f);

  REQUIRE(Math::boxBoxIntersect(box1, box2) == (true));
  REQUIRE(Math::pointBoxIntersect(point, box1) == (true));
  REQUIRE(Math::boxBoxIntersect(box1, obb) == (true));
  REQUIRE(Math::boxCapsuleIntersect(box1, cap1) == (true));
  REQUIRE(Math::boxPlaneIntersect(box1, plane) == (true));
  REQUIRE(Math::boxRectIntersect(box1, rect) == (true));
  REQUIRE(Math::boxSphereIntersect(box1, sph) == (true));
}

/*************************************************************/
/*
*  BoxOBB
*/
/*************************************************************/
TEST_CASE("BoxOBB") {
  shBoxOBB box1(Vector3(0.0f, 0.0f, 0.0f),
                Quaternion(0.0f, 0.0f, 0.0f, 0.0f),
                Vector3(0.0f, 0.0f, 0.0f));
  shBoxOBB box2(Vector3(0.0f, 0.0f, 0.0f),
                Quaternion(0.0f, 0.0f, 0.0f, 0.0f),
                Vector3(0.0f, 0.0f, 0.0f));
  Vector3 point(4.5f, 2.5f, 2.0f);

  shCapsule cap1(2.0f, Vector3(3.1f, 4.3f, 2.0f), Vector3(3.1f, 1.8f, 2.0f));
  shPlane plane(Vector3(3.1f, 4.3f, 2.0f), 1.0f);
  shRect rect(Vector2(2.8f, 1.7f), Vector2(1.0f, 1.0f));
  shSphere sph(Vector3(0.0f, 0.0f, 0.0f), 1.5f);

  REQUIRE(Math::boxBoxIntersect(box1, box2) == (false));
  REQUIRE(Math::pointBoxIntersect(point, box1) == (true));
  REQUIRE(Math::boxCapsuleIntersect(box1, cap1) == (true));
  REQUIRE(Math::boxPlaneIntersect(box1, plane) == (true));
  REQUIRE(Math::boxRectIntersect(box1, rect) == (true));
  REQUIRE(Math::boxSphereIntersect(box1, sph) == (true));
}

/*************************************************************/
/*
*  Capsule
*/
/*************************************************************/
TEST_CASE("Capsule") {
  shCapsule cap1(1.5f, Vector3(3.0f, 1.5f, 2.0f), Vector3(4.0f, 2.5f, 2.0f));
  shCapsule cap2(2.5f, Vector3(4.0f, 1.5f, 2.0f), Vector3(5.0f, 2.5f, 2.0f));
  Vector3 point(4.5f, 2.5f, 2.0f);

  shPlane plane(Vector3(3.1f, 4.3f, 2.0f), 1.0f);
  shRect rect(Vector2(2.8f, 1.7f), Vector2(1.0f, 1.0f));

  REQUIRE(Math::capsuleCapsuleIntersect(cap1, cap2) == (true));
  REQUIRE(Math::pointCapsuleIntersect(point, cap1) == (true));
  REQUIRE(Math::capsulePlaneIntersect(cap1, plane) == (true));
  REQUIRE(Math::capsuleRectIntersect(cap1, rect) == (true));
}

/*************************************************************/
/*
*  Sphere
*/
/*************************************************************/
TEST_CASE("Sphere") {
  shSphere sph1(Vector3(3.0f, 1.5f, 2.0f), 3.0f);
  shSphere sph2(Vector3(3.0f, 1.5f, 2.0f), 3.0f);
  Vector3 point(4.5f, 2.5f, 2.0f);

  shCapsule cap1(2.0f, Vector3(3.1f, 4.3f, 2.0f), Vector3(3.1f, 1.8f, 2.0f));
  shPlane plane(Vector3(3.1f, 4.3f, 2.0f), 1.0f);
  shRect rect(Vector2(2.8f, 1.7f), Vector2(1.0f, 1.0f));

  REQUIRE(Math::sphereSphereIntersect(sph1, sph2) == (true));
  REQUIRE(Math::pointSphereIntersect(point, sph1) == (true));
  REQUIRE(Math::sphereCapsuleIntersect(sph1, cap1) == (true));
  REQUIRE(Math::spherePlaneIntersect(sph1, plane) == (true));
  REQUIRE(Math::sphereRectIntersect(sph1, rect) == (true));
}

/*************************************************************/
/*
*  Rect
*/
/*************************************************************/
TEST_CASE("Rect") {
  shRect r1(Vector2(3.0f, 2.5f), Vector2(1.0f, 1.0f));
  shRect r2(Vector2(3.0f, 2.5f), Vector2(1.0f, 1.0f));
  Vector2 point(4.5f, 2.5f);

  REQUIRE(Math::rectRectIntersect(r1, r2) == (true));
  REQUIRE(Math::pointRectIntersect(point, r1) == (true));
}

/*************************************************************/
/*
*  Plane
*/
/*************************************************************/
TEST_CASE("Plane") {
  shPlane pln1(Vector3(3.0f, 1.5f, 2.0f), 1.0f);
  shPlane pln2(Vector3(3.0f, 1.5f, 2.0f), 1.0f);
  Vector2 point(4.5f, 2.5f);

  shRect rect(Vector2(2.8f, 1.7f), Vector2(1.0f, 1.0f));

  REQUIRE(Math::planePlaneIntersect(pln1, pln2) == (false));
  REQUIRE(Math::pointPlaneIntersect(point, pln1) == (true));
  REQUIRE(Math::planeRectIntersect(pln1, rect) == (true));
}

/*************************************************************/
/*
*  Module
*/
/*************************************************************/
TEST_CASE("Module") {
  class SubModule : public Module<SubModule>
  {
   public:
     int32 TestNumber = 2412;
  };

  class SubSubModule : public SubModule
  {

  };

  /*SubModule::startUp<SubModule>();

  REQUIRE(SubModule::instance().TestNumber == (2412));

  SubModule::shutDown();*/
}
