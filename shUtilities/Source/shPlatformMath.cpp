/*************************************************************/
/*
*  @file    shSPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/14
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX.
*
*  Math class wrapper, suing the STD. compatible with Windows, Linux and OSX.
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlatformMath.h"
#include "shBoxAAB.h"
#include "shBoxOBB.h"
#include "shSphere.h"

namespace shEngineSDK {
 /*************************************************************/
 /*
 *  Static variables
 */
 /*************************************************************/

const float PlatformMath::PI = acos(-1.0f);

const float PlatformMath::RAD2DEG = 180.0f / PI;

const float PlatformMath::DEG2RAD = PI / 180.0f;

const float PlatformMath::SMALL_NUMBER = (1.e-6f);

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

float
PlatformMath::sqrt(const float value)
{
  return std::sqrt(value);
}

float
PlatformMath::pow(const float value, const float vPow)
{
  return std::pow(value, vPow);
}

float
PlatformMath::min(const float x, const float y)
{
  return std::min(x, y);
}

float
PlatformMath::max(const float x, const float y)
{
  return std::max(x, y);
}

float
PlatformMath::fmod(const float value1, const float value2)
{
  return std::fmod(value1, value2);
}

float
PlatformMath::abs(const float value)
{
  return std::abs(value);
}

/*************************************************************/
/*
*  Trigonometric functions
*/
/*************************************************************/

float
PlatformMath::cos(const float radian)
{
  return std::cos(radian);
}

float
PlatformMath::sin(const float radian)
{
  return std::sin(radian);
}

float
PlatformMath::tan(const float radian)
{
  return std::tan(radian);
}

float
PlatformMath::cosh(const float radian)
{
  return std::cosh(radian);
}

float
PlatformMath::sinh(const float radian)
{
  return std::sinh(radian);
}

float
PlatformMath::tanh(const float radian)
{
  return std::tanh(radian);
}

float
PlatformMath::acos(const float radian)
{
  return std::acos(radian);
}

float
PlatformMath::asin(const float radian)
{
  return std::asin(radian);
}

float
PlatformMath::atan(const float radian)
{
  return std::atan(radian);
}

float
PlatformMath::acosh(const float radian)
{
  return std::acosh(radian);
}

float
PlatformMath::asinh(const float radian)
{
  return std::asinh(radian);
}

float
PlatformMath::atanh(const float radian)
{
  return std::atanh(radian);
}

/*************************************************************/
/*
*  Figures
*/
/*************************************************************/

bool
PlatformMath::pointBoxIntersect(const Vector3& point, const shBoxAAB& box)
{
  return (point.x >= box.min.x &&
          point.x <= box.max.x &&
          point.y >= box.min.y &&
          point.y <= box.max.y &&
          point.z >= box.min.z &&
          point.z <= box.max.z);
}

//TODO: finish this function.
bool
PlatformMath::pointBoxIntersect(const Vector3& point, const shBoxOBB& box)
{
  return false;
}

bool
PlatformMath::pointSphereIntersect(const Vector3& point, const shSphere& sph)
{
  const float distance = sqrt(((point.x - sph.center.x) *
                              (point.x - sph.center.x)) +
                              ((point.y - sph.center.y) *
                              (point.y - sph.center.y)) +
                              ((point.z - sph.center.z) *
                              (point.z - sph.center.z)));

  return distance < sph.radius;
}

//TODO: finish this function.
bool
PlatformMath::pointCapsuleIntersect(const Vector3& point, const shCapsule& cap)
{
  return false;
}

bool
PlatformMath::pointRectIntersect(const Vector2& point, const shRect& rect)
{
  return (point.x >= rect.min.x &&
          point.x <= rect.max.x &&
          point.y >= rect.min.y &&
          point.y <= rect.max.y);
}

//TODO: finish this function.
bool
PlatformMath::pointPlaneIntersect(const Vector2& point, const shPlane& plane)
{
  return false;
}

bool
PlatformMath::boxBoxIntersect(const shBoxAAB& box, const shBoxAAB& box1)
{
  return (box.min.x <= box1.max.x &&
          box.max.x >= box1.min.x &&
          box.min.y <= box1.max.y &&
          box.max.y >= box1.min.y &&
          box.min.z <= box1.max.z &&
          box.max.z >= box1.min.z);
}

//TODO: finish this function.
bool
PlatformMath::boxBoxIntersect(const shBoxOBB& box, const shBoxOBB& box1)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::boxBoxIntersect(const shBoxAAB& boxA, const shBoxOBB& boxO)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::boxCapsuleIntersect(const shBoxAAB& box, const shCapsule& cap)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::boxCapsuleIntersect(const shBoxOBB& box, const shCapsule& cap)
{
  return false;
}

bool
PlatformMath::boxPlaneIntersect(const shBoxAAB& box, const shPlane& plane)
{
  const float distance = plane.normal.dot(box.max - box.min) - plane.distance;

  const float projRadius = box.max.x - box.min.x + abs(plane.normal.x) +
                           box.max.y - box.min.y + abs(plane.normal.y) +
                           box.max.z - box.min.z + abs(plane.normal.z);

  return abs(distance) <= projRadius;
}

//TODO: finish this function.
bool
PlatformMath::boxPlaneIntersect(const shBoxOBB& box, const shPlane& plane)
{
  return false;
}

bool
PlatformMath::boxRectIntersect(const shBoxAAB& box, const shRect& rect)
{
  return (box.min.x <= rect.max.x &&
          box.max.x >= rect.min.x &&
          box.min.y <= rect.max.y &&
          box.max.y >= rect.min.y);
}

//TODO: finish this function.
bool
PlatformMath::boxRectIntersect(const shBoxOBB& box, const shRect& rect)
{
  return false;
}

bool
PlatformMath::sphereSphereIntersect(const shSphere& sph, const shSphere& sph1)
{
  const float distance = sqrt((sph.center.x - sph1.center.x) +
                              (sph.center.y - sph1.center.y) +
                              (sph.center.z - sph1.center.z));

  return (distance < (sph.radius + sph1.radius));
}

bool
PlatformMath::boxSphereIntersect(const shBoxAAB& box, const shSphere& sph)
{
  const float x = max(box.min.x, min(sph.center.x, box.max.x));
  const float y = max(box.min.y, min(sph.center.y, box.max.y));
  const float z = max(box.min.z, min(sph.center.z, box.max.z));

  const float distance = sqrt(((x - sph.center.x) * (x - sph.center.x)) +
                              ((y - sph.center.y) * (y - sph.center.y)) +
                              ((z - sph.center.z) * (z - sph.center.z)));

  return distance < sph.radius;
}

//TODO: finish this function.
bool
PlatformMath::boxSphereIntersect(const shBoxOBB& box, const shSphere& sph)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::sphereCapsuleIntersect(const shSphere& sph, const shCapsule& cap)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::spherePlaneIntersect(const shSphere& sph, const shPlane& plane)
{
  return false;
}

bool
PlatformMath::sphereRectIntersect(const shSphere& sph, const shRect& rect)
{
  const float x = max(rect.min.x, min(sph.center.x, rect.max.x));
  const float y = max(rect.min.y, min(sph.center.y, rect.max.y));

  const  float distance = sqrt(((x - sph.center.x) * (x - sph.center.x)) +
                               ((y - sph.center.y) * (y - sph.center.y)));

  return distance < sph.radius;
}

bool
PlatformMath::capsuleCapsuleIntersect(const shCapsule& cap, const shCapsule& cap1)
{
  const Vector3 c1Normal = (cap.pointB - cap.pointA).getNormalized();
  const Vector3 c1EndOffset = c1Normal + cap.radius;
  const Vector3 c1A = cap.pointA + c1EndOffset;
  const Vector3 c1B = cap.pointB + c1EndOffset;

  const Vector3 c2Normal = (cap1.pointB - cap1.pointA).getNormalized();
  const Vector3 c2EndOffset = c1Normal + cap1.radius;
  const Vector3 c2A = cap1.pointA + c1EndOffset;
  const Vector3 c2B = cap1.pointB + c1EndOffset;

  const Vector3 vec0 = c2A - c1A;
  const Vector3 vec1 = c2B - c1A;
  const Vector3 vec2 = c2A - c1B;
  const Vector3 vec3 = c2B - c1B;

  const float d0 = vec0.dot(vec0);
  const float d1 = vec1.dot(vec1);
  const float d2 = vec2.dot(vec2);
  const float d3 = vec3.dot(vec3);

  Vector3 bestA = c1A;
  if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
    bestA = c1B;
  }

  Vector3 AB = c2B - c2A;
  float t = clamp((bestA - c2A).dot(AB) / AB.dot(AB), 0.0f, 1.0f);
  Vector3 bestB = c2A + (AB * t);

  AB = c1B - c1A;
  t = clamp((bestB - c1A).dot(AB) / AB.dot(AB), 0.0f, 1.0f);
  bestA = c1A + (AB * t);

  const float dist = (bestA - bestB).mag();
  const float depth = cap.radius + cap1.radius - dist;
  
  return depth > 0;
}

//TODO: finish this function.
bool
PlatformMath::capsulePlaneIntersect(const shCapsule& cap, const shPlane& plane)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::capsuleRectIntersect(const shCapsule& cap, const shRect& rect)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::planePlaneIntersect(const shPlane& plane, const shPlane& plane1)
{
  return false;
}

//TODO: finish this function.
bool
PlatformMath::planeRectIntersect(const shPlane& plane, const shRect& rect)
{
  return false;
}

bool
PlatformMath::rectRectIntersect(const shRect& rect, const shRect& rect1)
{
  return (rect.min.x <= rect1.max.x &&
          rect.max.x >= rect1.min.x &&
          rect.min.y <= rect1.max.y &&
          rect.max.y >= rect1.min.y);
}
}
