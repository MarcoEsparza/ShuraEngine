/*************************************************************/
/*
*  @file    shUtilitiesTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/12
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
  REQUIRE(sizeof(SIZE_T) == 8);
}

/*************************************************************/
/*
*  Platform Math: functions
*/
/*************************************************************/

TEST_CASE("Platform Math") {
  REQUIRE(Math::sqrt(25.0f) == (5.0f));
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
TEST_CASE("Vectors and Quaternion") {
  Vector2i vec2i(5, 8);
  Vector2 vec2f(2.0f, 3.0f);
  Vector3 vec3f(15.0f, 8.0f, 9.0f);
  Vector4 vec4f(1.0f, 7.7f, 12.6f, 2.5f);
  Quaternion quat(9.0f, 3.4f, 1.2f, 10.1f);

  // Vector2i
  REQUIRE(vec2i.x == 5);
  REQUIRE(vec2i.y == 8);
  REQUIRE(vec2i.dot(Vector2i(2, 3)) == (34));
  REQUIRE(vec2i.mag() == (9.4339811321f));
  Vector2i nVec2i = vec2i.lerp(vec2i, Vector2i(6, 10), 1);
  REQUIRE(nVec2i.x == 16);
  REQUIRE(nVec2i.y == 26);
  vec2i.normalize();
  REQUIRE(vec2i.x == 0);
  REQUIRE(vec2i.y == 0);
  REQUIRE(nVec2i.scalarProjection(Vector2i(6, 10)) == (30.52674816f));
  Vector2i otherVec2i = nVec2i.vectorProjection(Vector2i(6, 10));
  REQUIRE(otherVec2i.x == 15);
  REQUIRE(otherVec2i.y == 26);

  // Vector2
  REQUIRE(vec2f.x == 2.0f);
  REQUIRE(vec2f.y == 3.0f);

  // Vector3
  REQUIRE(vec3f.x == 15.0f);
  REQUIRE(vec3f.y == 8.0f);
  REQUIRE(vec3f.z == 9.0f);

  // Vector4
  REQUIRE(vec4f.x == 1.0f);
  REQUIRE(vec4f.y == 7.7f);
  REQUIRE(vec4f.z == 12.6f);
  REQUIRE(vec4f.w == 2.5f);

  // Quaternion
  REQUIRE(quat.x == 9.0f);
  REQUIRE(quat.y == 3.4f);
  REQUIRE(quat.z == 1.2f);
  REQUIRE(quat.w == 10.1f);
}

/*************************************************************/
/*
*  Matrix4
*/
/*************************************************************/
TEST_CASE("Matrix4") {
  Matrix4 mat(1.0f, 1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f, 1.0f);
}
