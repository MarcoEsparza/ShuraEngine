/*****************************************************************************/
/*
*  @file    shPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX.
*
*  Math class wrapper, suing the STD. compatible with Windows, Linux and OSX.
*
* @bug      No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPlatformMath.h"
#include "shVector3.h"
#include "shVector2.h"
#include "shMatrix3.h"
#include "shAABBox.h"
#include "shOBBox.h"
#include "shRect.h"
#include "shPlane.h"
#include "shCapsule.h"
#include "shSphere.h"
#include "shDegree.h"
#include "shRadian.h"
#include "shCollisionInfo.h"

#define MAX_FLOAT 3.402823466e+38F

using std::all_of;

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static variables
*/
/*****************************************************************************/

const float PlatformMath::PI = 2 * asin(Radian(1.0f));

const float PlatformMath::TWO_PI = 2 * PlatformMath::PI;

const float PlatformMath::HALF_PI = PI * 0.5f;

const float PlatformMath::RAD2DEG = 180.0f / PI;

const float PlatformMath::DEG2RAD = PI / 180.0f;

const float PlatformMath::SMALL_NUMBER = (1.e-6f);

const float PlatformMath::KINDA_SMALL_NUMBER = (1.e-4f);

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

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
PlatformMath::lerp(const float A, const float B, const float alpha)
{
  return (A + (B - A) * alpha);
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
PlatformMath::clamp(const float x, const float min, const float max)
{
  return x < min ? min : x < max ? x : max;
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

float PlatformMath::log2(const float value)
{
  // Temporary workaround for platforms that do not support log2 directly
  return std::log2(value);
}

Array<float, 4>::iterator
PlatformMath::min_element(const Array<float, 4>::iterator first,
                          const Array<float, 4>::iterator last)
{
  return std::min_element(first, last);
}

Array<float, 4>::iterator
PlatformMath::max_element(const Array<float, 4>::iterator first,
                          const Array<float, 4>::iterator last)
{
  return std::max_element(first, last);
}

bool
PlatformMath::is_NaN(const float value)
{
  return std::isnan(value);
}

float
PlatformMath::copySign(float number, const float sign)
{
  return std::copysign(number, sign);
}

float
PlatformMath::floor(const float value)
{
  return std::floor(value);
}

/*****************************************************************************/
/*
*  Trigonometric functions
*/
/*****************************************************************************/

float
PlatformMath::cos(const Radian& radian)
{
  return std::cos(radian.getValueOnRadians());
}

float
PlatformMath::sin(const Radian& radian)
{
  return std::sin(radian.getValueOnRadians());
}

float
PlatformMath::tan(const Radian& radian)
{
  return std::tan(radian.getValueOnRadians());
}

float
PlatformMath::cosh(const Radian& radian)
{
  return std::cosh(radian.getValueOnRadians());
}

float
PlatformMath::sinh(const Radian& radian)
{
  return std::sinh(radian.getValueOnRadians());
}

float
PlatformMath::tanh(const Radian& radian)
{
  return std::tanh(radian.getValueOnRadians());
}

float
PlatformMath::acos(const Radian& radian)
{
  return std::acos(radian.getValueOnRadians());
}

float
PlatformMath::asin(const Radian& radian)
{
  return std::asin(radian.getValueOnRadians());
}

float
PlatformMath::atan(const Radian& radian)
{
  return std::atan(radian.getValueOnRadians());
}

float
PlatformMath::atan2(const Radian& radian1, const Radian& radian2)
{
  return std::atan2(radian1.getValueOnRadians(), radian2.getValueOnRadians());
}

float
PlatformMath::acosh(const Radian& radian)
{
  return std::acosh(radian.getValueOnRadians());
}

float
PlatformMath::asinh(const Radian& radian)
{
  return std::asinh(radian.getValueOnRadians());
}

float
PlatformMath::atanh(const Radian& radian)
{
  return std::atanh(radian.getValueOnRadians());
}

/*****************************************************************************/
/*
*  Figures
*/
/*****************************************************************************/

bool
PlatformMath::intersect(const Vector3& point, const AABBox& box, CollisionInfo& colInfo)
{
  if (point.x < box.min.x || point.x > box.max.x ||
    point.y < box.min.y || point.y > box.max.y ||
    point.z < box.min.z || point.z > box.max.z)
  {
    return false;
  }

  // Compute distance to each face of the box
  float dxMin = point.x - box.min.x;
  float dxMax = box.max.x - point.x;
  float dyMin = point.y - box.min.y;
  float dyMax = box.max.y - point.y;
  float dzMin = point.z - box.min.z;
  float dzMax = box.max.z - point.z;

  // Find minimum penetration and normal
  float minPen = dxMin;
  Vector3 normal(1.0f, 0.0f, 0.0f);

  if (dxMax < minPen) {
    minPen = dxMax;
    normal = Vector3(0.0f, 0.0f, 0.0f);
  }
  if (dyMin < minPen) {
    minPen = dyMin;
    normal = Vector3(0.0f, 1.0f, 0.0f);
  }
  if (dyMax < minPen) {
    minPen = dyMax;
    normal = Vector3(0.0f, -1.0f, 0.0f);
  }
  if (dzMin < minPen) {
    minPen = dzMin;
    normal = Vector3(0.0f, 0.0f, 1.0f);
  }
  if (dzMax < minPen) {
    minPen = dzMax;
    normal = Vector3(0.0f, 0.0f, -1.0f);
  }

  colInfo.penetrationDepth = minPen;
  colInfo.normal = normal;
  colInfo.contactPoint1 = point;
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;

  return true;
}

bool
PlatformMath::intersect(const Vector3& point, const AABBox& box)
{
  return (point.x >= box.min.x &&
          point.x <= box.max.x &&
          point.y >= box.min.y &&
          point.y <= box.max.y &&
          point.z >= box.min.z &&
          point.z <= box.max.z);
}

bool
PlatformMath::intersect(const Vector3& point, const OBBox& obb, CollisionInfo& colInfo)
{
  // Transform the point to box local space
  const Vector3 localPoint = obb.rotation.invRotate(point - obb.center);

  // Clamp the point to the obb local space limits
  Vector3 clampedLocal = Vector3::ZERO;
  clampedLocal.x = max(-obb.extent.x, min(localPoint.x, obb.extent.x));
  clampedLocal.y = max(-obb.extent.y, min(localPoint.y, obb.extent.y));
  clampedLocal.z = max(-obb.extent.z, min(localPoint.z, obb.extent.z));

  // Closest point in global space
  Vector3 closestPointWorld = obb.rotation.rotate(clampedLocal) + obb.center;
  Vector3 diff = point - closestPointWorld;
  float distSq = diff.dot(diff);

  if (distSq > SMALL_NUMBER) {
    float dist = sqrt(distSq);
    colInfo.penetrationDepth = -dist;
    colInfo.normal = diff * (1.0f / dist);
    colInfo.contactPoint1 = closestPointWorld;
    return false;
  }
  else {
    // Point is inside of the obb
    // Calculate normal and min depth
    Vector3 axes[3] = { obb.rotation.rotate(Vector3::RIGHT),
                        obb.rotation.rotate(Vector3::UP),
                        obb.rotation.rotate(Vector3::FORWARD) };


    float minDist = obb.extent.x - std::abs(localPoint.x);
    Vector3 normal = axes[0] * ((localPoint.x < 0) ? -1.0f : 1.0f);

    float d = obb.extent.y - std::abs(localPoint.y);
    if (d < minDist) {
      minDist = d; normal = axes[1] * ((localPoint.y < 0) ? -1.0f : 1.0f);
    }

    d = obb.extent.z - std::abs(localPoint.z);
    if (d < minDist) {
      minDist = d; normal = axes[2] * ((localPoint.z < 0) ? -1.0f : 1.0f);
    }

    colInfo.penetrationDepth = minDist;
    colInfo.normal = normal;
    colInfo.contactPoint1 = point - normal * minDist;
    // Contact point 2 is not applicable for point intersection, so we leave it as zero
    colInfo.contactPoint2 = Vector3::ZERO;
    return true;
  }
}

bool
PlatformMath::intersect(const Vector3& point, const OBBox& box)
{
  Vector3 right, up, forward;
  box.rotation.toAxes(right, up, forward);

  const Vector3 localPoint = point - box.center;

  float projRight = localPoint.dot(right);
  float projUp = localPoint.dot(up);
  float projForward = localPoint.dot(forward);

  if ((projRight < -box.extent.x || projRight > box.extent.x) &&
      (projUp < -box.extent.y || projUp > box.extent.y) &&
      (projForward < -box.extent.z || projForward > box.extent.z)) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Vector3& point, const Sphere& sph, CollisionInfo& colInfo)
{
  const float distance = sqrt(((point.x - sph.center.x) *
                              (point.x - sph.center.x)) +
                              ((point.y - sph.center.y) *
                              (point.y - sph.center.y)) +
                              ((point.z - sph.center.z) *
                              (point.z - sph.center.z)));

  colInfo.penetrationDepth = sph.radius - distance;
  if (distance > SMALL_NUMBER) {
    colInfo.normal = Vector3((point.x - sph.center.x) / distance,
                             (point.y - sph.center.y) / distance,
                             (point.z - sph.center.z) / distance);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0); // Arbitrary normal if point is at the center
  }
  colInfo.contactPoint1 = point + colInfo.normal * sph.radius;
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;

  return distance < sph.radius;
}

bool
PlatformMath::intersect(const Vector3& point, const Sphere& sph)
{
  const float distanceSq = ((point.x - sph.center.x) * (point.x - sph.center.x)) +
                           ((point.y - sph.center.y) * (point.y - sph.center.y)) +
                           ((point.z - sph.center.z) * (point.z - sph.center.z));
  return distanceSq <= (sph.radius * sph.radius);
}

bool
PlatformMath::intersect(const Vector3& point, const Capsule& cap, CollisionInfo& colInfo)
{
  // Capsule points
  Vector3 direction = cap.direction.getNormalized();
  Vector3 A = cap.center - direction * (cap.height / 2.0f);
  Vector3 B = cap.center + direction * (cap.height / 2.0f);

  // Points projection
  Vector3 AB = B - A;
  Vector3 AP = point - A;

  float t = AB.dot(AP) / AB.dot(AB);
  t = clamp(t, 1.0f, 0.0f);

  Vector3 closestPoint = A + AB * t;
  Vector3 vectorToPoint = point - closestPoint;
  float distanceSq = vectorToPoint.dot(vectorToPoint);

  if (distanceSq > cap.radius * cap.radius) {
    return false;
  }

  float distance = sqrt(distanceSq);
  colInfo.penetrationDepth = cap.radius - distance;

  if (distance > SMALL_NUMBER) {
    colInfo.normal = vectorToPoint * (1.0f / distance);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0);
  }

  colInfo.contactPoint1 = closestPoint + colInfo.normal * cap.radius;
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;
  return true;
}

bool
PlatformMath::intersect(const Vector3& point, const Capsule& cap)
{
  Vector3 direction = cap.direction.getNormalized();
  Vector3 A = cap.center - direction * (cap.height / 2.0f);
  Vector3 B = cap.center + direction * (cap.height / 2.0f);
  Vector3 AB = B - A;
  Vector3 AP = point - A;
  float t = AB.dot(AP) / AB.dot(AB);
  t = PlatformMath::clamp(t, 0.0f, 1.0f);
  Vector3 closestPoint = A + AB * t;
  Vector3 vectorToPoint = point - closestPoint;
  float distanceSq = vectorToPoint.dot(vectorToPoint);

  return (distanceSq <= cap.radius * cap.radius);
}

bool
PlatformMath::intersect(const Vector2& point, const Rect& rect, CollisionInfo& colInfo)
{
  if (point.x < rect.min.x || point.x > rect.max.x ||
      point.y < rect.min.y || point.y > rect.max.y) {
    return false;
  }
  // Compute distance to each edge of the rectangle
  float dxMin = point.x - rect.min.x;
  float dxMax = rect.max.x - point.x;
  float dyMin = point.y - rect.min.y;
  float dyMax = rect.max.y - point.y;
  
  // Find minimum penetration and normal
  float minPen = dxMin;
  Vector2 normal(1.0f, 0.0f);
  if (dxMax < minPen) {
    minPen = dxMax;
    normal = Vector2(-1.0f, 0.0f);
  }
  if (dyMin < minPen) {
    minPen = dyMin;
    normal = Vector2(0.0f, 1.0f);
  }
  if (dyMax < minPen) {
    minPen = dyMax;
    normal = Vector2(0.0f, -1.0f);
  }
  colInfo.penetrationDepth = minPen;
  colInfo.normal = Vector3(normal.x, normal.y, 0.0f);
  colInfo.contactPoint1 = Vector3(point.x, point.y, 0.0f);
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;
  return true;
}

bool
PlatformMath::intersect(const Vector2& point, const Rect& rect)
{
  return (point.x >= rect.min.x &&
          point.x <= rect.max.x &&
          point.y >= rect.min.y &&
          point.y <= rect.max.y);
}

bool
PlatformMath::intersect(const Vector3& point, const Plane& plane, CollisionInfo& colInfo)
{
  const float distance = plane.pointToPlaneDistance(point);
  if (PlatformMath::abs(distance) < SMALL_NUMBER) {
    colInfo.penetrationDepth = 0.0f;
    colInfo.normal = plane.normal;
    colInfo.contactPoint1 = point;
    // Contact point 2 is not applicable for point intersection, so we leave it as zero
    colInfo.contactPoint2 = Vector3::ZERO;
    return true;
  }
  return false;
}

bool
PlatformMath::intersect(const Vector3& point, const Plane& plane)
{
  const float distance = plane.pointToPlaneDistance(point);

  return PlatformMath::abs(distance) < SMALL_NUMBER;
}

bool
PlatformMath::intersect(const AABBox& box, const AABBox& box1, CollisionInfo& colInfo)
{
  if (box.min.x > box1.max.x || box.max.x < box1.min.x ||
      box.min.y > box1.max.y || box.max.y < box1.min.y ||
      box.min.z > box1.max.z || box.max.z < box1.min.z) {
    return false; // No intersection
  }

  // Calculate collision info
  colInfo.penetrationDepth = 0.0f;
  colInfo.normal = Vector3::ZERO;
  colInfo.contactPoint1 = Vector3::ZERO;
  colInfo.contactPoint2 = Vector3::ZERO;

  // Find the minimum penetration depth and normal
  float dxMin = box.min.x - box1.max.x;
  float dxMax = box.max.x - box1.min.x;
  float dyMin = box.min.y - box1.max.y;
  float dyMax = box.max.y - box1.min.y;
  float dzMin = box.min.z - box1.max.z;
  float dzMax = box.max.z - box1.min.z;
  float minPen = dxMin;
  Vector3 normal(1.0f, 0.0f, 0.0f);
  if (dxMax < minPen) {
    minPen = dxMax; normal = Vector3(-1.0f, 0.0f, 0.0f);
  }
  if (dyMin < minPen) {
    minPen = dyMin; normal = Vector3(0.0f, 1.0f, 0.0f);
  }
  if (dyMax < minPen) {
    minPen = dyMax; normal = Vector3(0.0f, -1.0f, 0.0f);
  }
  if (dzMin < minPen) {
    minPen = dzMin; normal = Vector3(0.0f, 0.0f, 1.0f);
  }
  if (dzMax < minPen) {
    minPen = dzMax; normal = Vector3(0.0f, 0.0f, -1.0f);
  }
  colInfo.penetrationDepth = minPen;
  colInfo.normal = normal;
  
  // Calculate contact points
  colInfo.contactPoint1.x = (box.min.x + box.max.x) * 0.5f;
  colInfo.contactPoint1.y = (box.min.y + box.max.y) * 0.5f;
  colInfo.contactPoint1.z = (box.min.z + box.max.z) * 0.5f;
  colInfo.contactPoint2.x = (box1.min.x + box1.max.x) * 0.5f;
  colInfo.contactPoint2.y = (box1.min.y + box1.max.y) * 0.5f;
  colInfo.contactPoint2.z = (box1.min.z + box1.max.z) * 0.5f;

  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const AABBox& box, const AABBox& box1)
{
  return (box.min.x <= box1.max.x &&
          box.max.x >= box1.min.x &&
          box.min.y <= box1.max.y &&
          box.max.y >= box1.min.y &&
          box.min.z <= box1.max.z &&
          box.max.z >= box1.min.z);
}

bool
TestAxis(const Vector3& axis,
         const OBBox& boxA,
         const OBBox& boxB,
         const Vector3& t,
         float& minPenetration,
         Vector3& bestAxis)
{
  if(axis.lenghtSq() < 1e-6f) {
    return true; // Axiss is zero-length, skip it
  }

  Vector3 axisNorm = axis.getNormalized();

  // Project distance between centers onto the axis
  float distance = PlatformMath::abs(t.dot(axisNorm));

  float rA = 0.0f;
  Matrix3 rotA = boxA.rotation.toMatrix3();
  rA += boxA.extent.x * PlatformMath::abs(rotA.getColumn(0).dot(axisNorm));
  rA += boxA.extent.y * PlatformMath::abs(rotA.getColumn(1).dot(axisNorm));
  rA += boxA.extent.z * PlatformMath::abs(rotA.getColumn(2).dot(axisNorm));

  float rB = 0.0f;
  Matrix3 rotB = boxB.rotation.toMatrix3();
  rB += boxB.extent.x * PlatformMath::abs(rotB.getColumn(0).dot(axisNorm));
  rB += boxB.extent.y * PlatformMath::abs(rotB.getColumn(1).dot(axisNorm));
  rB += boxB.extent.z * PlatformMath::abs(rotB.getColumn(2).dot(axisNorm));

  float overlap = rA + rB - distance;

  if (overlap <= 0.0f) {
    return false; // No intersection on this axis
  }

  if(overlap < minPenetration) {
    minPenetration = overlap;
    bestAxis = axisNorm; // Store the best axis
  }

  return true; // Intersection on this axis
}

bool
PlatformMath::intersect(const OBBox& box, const OBBox& box1, CollisionInfo& colInfo)
{
  // 1. Build matrices for each OBB
  Matrix3 rotA = box.rotation.toMatrix3();
  Matrix3 rotB = box1.rotation.toMatrix3();

  // 2. Axis vectors for each OBB
  Vector3 axesA[3] = { rotA.getColumn(0), rotA.getColumn(1), rotA.getColumn(2) };
  Vector3 axesB[3] = { rotB.getColumn(0), rotB.getColumn(1), rotB.getColumn(2) };

  // 3. Vector between centers
  Vector3 t = box.center - box1.center;

  float minPenetration = MAX_FLOAT;
  Vector3 bestAxis;

  // Evaluate axes
  for (int i = 0; i < 3; ++i) {
    // Axes A
    Vector3 axis = axesA[i];
    if (!TestAxis(axis, box, box1, t, minPenetration, bestAxis))
      return false;
  }

  for (int i = 0; i < 3; ++i) {
    // Axes B
    Vector3 axis = axesB[i];
    if (!TestAxis(axis, box, box1, t, minPenetration, bestAxis)) {
      return false;
    }
  }

  // Cross products of axes A and B
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      Vector3 axis = axesA[i].cross(axesB[j]);
      if (axis.lenghtSq() < 1e-6f) continue; // ejes paralelos

      axis = axis.getNormalized();
      if (!TestAxis(axis, box, box1, t, minPenetration, bestAxis))
        return false;
    }
  }

  // 4. If we reach here, we have a collision
  colInfo.penetrationDepth = minPenetration;
  colInfo.normal = bestAxis;

  // Invert normal if necessary
  if (bestAxis.dot(t) < 0) {
    colInfo.normal = -bestAxis;
  }

  // 5. Calculate contact points
  colInfo.contactPoint1 = box.center + colInfo.normal *
                          (box.extent.lenght() * 0.5f - 0.5f * minPenetration);
  colInfo.contactPoint2 = box1.center - colInfo.normal *
                          (box1.extent.lenght() * 0.5f - 0.5f * minPenetration);

  return true;
}

bool
PlatformMath::intersect(const OBBox& box, const OBBox& box1)
{
  Vector<Vector3> axes;
  axes.resize(15);

  Vector<Vector3> axes1;
  axes1.resize(3);

  Vector<Vector3> axes2;
  axes2.resize(3);

  box.rotation.toAxes(axes1[0], axes1[1], axes1[2]);
  for (uint8 i = 0; i < 3; ++i) {
    axes[i] = axes1[i];
  }

  uint8 index = 3;
  box1.rotation.toAxes(axes2[0], axes2[1], axes2[2]);
  for (uint8 i = 0; i < 3; ++i) {
    axes[index + i] = axes1[i];
  }

  index = 6;
  for (uint8 i = 0; i < 3; ++i) {
    for (uint8 j = 0; j < 3; ++j) {
      axes[index++] = axes1[i].cross(axes2[j]);
    }
  }

  for (uint8 i = 0; i < 15; ++i) {
    float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;

    box.projectOnAxis(axes[i], min1, max1);
    box1.projectOnAxis(axes[i], min2, max2);

    if (!box.overlapOnProjection(min1, max1, min2, max2)) {
      return true;
    }
  }

  return false;
}

bool TestAxis(const Vector3& axis,
              const Vector3& extentA,
              const OBBox& obb,
              const Vector3& t,
              float& minPenetration,
              Vector3& bestAxis)
{
  if (axis.lenghtSq() < 1e-6f) {
    return true; // Axiss is zero-length, skip it
  }

  Vector3 axisNorm = axis.getNormalized();

  // Project distance between centers onto the axis
  float distance = PlatformMath::abs(t.dot(axisNorm));

  float rA = 0.0f;
  rA += extentA.x * PlatformMath::abs(Vector3::RIGHT.dot(axisNorm));
  rA += extentA.y * PlatformMath::abs(Vector3::UP.dot(axisNorm));
  rA += extentA.z * PlatformMath::abs(Vector3::FORWARD.dot(axisNorm));

  Matrix3 rotB = obb.rotation.toMatrix3();
  float rB = 0.0f;
  rB += obb.extent.x * PlatformMath::abs(rotB.getColumn(0).dot(axisNorm));
  rB += obb.extent.y * PlatformMath::abs(rotB.getColumn(1).dot(axisNorm));
  rB += obb.extent.z * PlatformMath::abs(rotB.getColumn(2).dot(axisNorm));

  float overlap = rA + rB - distance;
  if (overlap <= 0.0f) {
    return false; // No intersection on this axis
  }

  if (overlap < minPenetration) {
    minPenetration = overlap;
    bestAxis = axisNorm; // Store the best axis
  }

  return true; // Intersection on this axis
}

bool
PlatformMath::intersect(const AABBox& boxA, const OBBox& boxO, CollisionInfo& colInfo)
{
  Vector3 center = boxA.min + boxA.max * 0.5f;
  Vector3 extent = boxA.max - boxA.min * 0.5f;

  Matrix3 rotB = boxO.rotation.toMatrix3();

  Vector3 axesA[3] = { Vector3::RIGHT, Vector3::UP, Vector3::FORWARD };
  Vector3 axesB[3] = { rotB.getColumn(0), rotB.getColumn(1), rotB.getColumn(2) };

  Vector3 t = boxO.center - center;

  float minPenetration = MAX_FLOAT;
  Vector3 bestAxis;

  for(uint32 i = 0; i < 3; ++i) {
    // Test axes A
    if (!TestAxis(axesA[i], extent, boxO, t, minPenetration, bestAxis)) {
      return false;
    }
  }

  for(uint32 i = 0; i < 3; ++i) {
    // Test axes B
    if (!TestAxis(axesB[i], extent, boxO, t, minPenetration, bestAxis)) {
      return false;
    }
  }

  // Cross products of axes A and B
  for (uint32 i = 0; i < 3; ++i) {
    for (uint32 j = 0; j < 3; ++j) {
      Vector3 axis = axesA[i].cross(axesB[j]);
      if (axis.lenghtSq() < 1e-6f) {
        continue; // Skip zero-length axes
      }
      axis = axis.getNormalized();
      if (!TestAxis(axis, extent, boxO, t, minPenetration, bestAxis)) {
        return false;
      }
    }
  }

  // If we reach here, we have a collision
  colInfo.penetrationDepth = minPenetration;
  colInfo.normal = bestAxis;
  if (bestAxis.dot(t) < 0) {
    colInfo.normal = -bestAxis; // Invert normal if necessary
  }

  // Calculate contact points
  colInfo.contactPoint1 = center + colInfo.normal *
                          (extent.lenght() * 0.5f - 0.5f * minPenetration);
  colInfo.contactPoint2 = boxO.center - colInfo.normal *
                          (boxO.extent.lenght() * 0.5f - 0.5f * minPenetration);

  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const AABBox& boxA, const OBBox& boxO)
{
  Vector<Vector3> aabbAxes = { Vector3(1.0f,0.0f,0.0f),
                               Vector3(0.0f,1.0f,0.0f),
                               Vector3(0.0f,0.0f,1.0f) };

  Vector<Vector3> obbAxes;
  obbAxes.resize(3);
  boxO.rotation.toAxes(obbAxes[0], obbAxes[1], obbAxes[2]);

  // Project on aabb axes
  for (uint8 i = 0; i < 3; ++i) {
    float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;

    boxA.projectOnAxis(aabbAxes[i], min1, max1);
    boxO.projectOnAxis(aabbAxes[i], min2, max2);

    if (!boxO.overlapOnProjection(min1, max1, min2, max2)) {
      return true;
    }
  }

  // Project on obb axes
  for (uint8 i = 0; i < 3; ++i) {
    float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;

    boxA.projectOnAxis(obbAxes[i], min1, max1);
    boxO.projectOnAxis(obbAxes[i], min2, max2);

    if (!boxO.overlapOnProjection(min1, max1, min2, max2)) {
      return true;
    }
  }

  // Axis combination
  for (uint8 i = 0; i < 3; ++i) {
    for (uint8 j = 0; j < 3; ++j) {
      Vector3 axis = aabbAxes[i].cross(obbAxes[j]);

      if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
        continue;
      }

      float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;
      boxA.projectOnAxis(axis, min1, max1);
      boxO.projectOnAxis(axis, min2, max2);

      if (!boxO.overlapOnProjection(min1, max1, min2, max2)) {
        return true;
      }
    }
  }

  return false;
}

bool
PlatformMath::intersect(const AABBox& box, const Capsule& cap, CollisionInfo& colInfo)
{
  Vector3 dir = cap.direction.getNormalized();

  // Capsule points
  Vector3 A = cap.center - dir * (cap.height / 2.0f);
  Vector3 B = cap.center + dir * (cap.height / 2.0f);
  Vector3 AB = B - A;

  // Test on multiple points along the segment
  const int steps = 10;
  float minDistSq = MAX_FLOAT;
  Vector3 bestP = Vector3::ZERO;
  Vector3 bestQ = Vector3::ZERO;

  for (int i = 0; i <= steps; ++i) {
    float t = (float)i / steps;
    // Point on capsule
    Vector3 p = A + AB * t;
    // Closest point to box
    Vector3 q = p.clamp(box.min, box.max);

    float distSq = (q - p).lenghtSq();
    if (distSq < minDistSq) {
      minDistSq = distSq;
      bestP = p;
      bestQ = q;
    }
  }

  if (minDistSq > cap.radius * cap.radius) {
    return false;
  }

  float distance = sqrt(minDistSq);
  colInfo.penetrationDepth = cap.radius - distance;

  Vector3 normal = (bestQ - bestP);
  if (distance > SMALL_NUMBER) {
    normal = normal * (1.0f / distance);
  }
  else {
    normal = Vector3(1.0f, 0.0f, 0.0f);
  }

  colInfo.normal = normal;
  colInfo.contactPoint1 = bestP + normal * cap.radius;
  colInfo.contactPoint2 = bestQ + normal * cap.radius;

  return true;
}

bool
PlatformMath::intersect(const AABBox& box, const Capsule& cap)
{
  Vector3 dir = cap.direction.getNormalized();

  // Capsule points
  Vector3 A = cap.center - dir * (cap.height / 2.0f);
  Vector3 B = cap.center + dir * (cap.height / 2.0f);
  Vector3 AB = B - A;

  // Test on multiple points along the segment
  const int steps = 10;
  float minDistSq = MAX_FLOAT;
  Vector3 bestP = Vector3::ZERO;
  Vector3 bestQ = Vector3::ZERO;

  for (int i = 0; i <= steps; ++i) {
    float t = (float)i / steps;
    // Point on capsule
    Vector3 p = A + AB * t;
    // Closest point to box
    Vector3 q = p.clamp(box.min, box.max);

    float distSq = (q - p).lenghtSq();
    if (distSq < minDistSq) {
      minDistSq = distSq;
      bestP = p;
      bestQ = q;
    }
  }

  return (minDistSq > cap.radius * cap.radius);
}

bool
PlatformMath::intersect(const OBBox& box, const Capsule& cap, CollisionInfo& colInfo)
{
  // Transform capsule to obb local space
  Vector3 localCapsuleCenter = box.rotation.invRotate(cap.center - box.center);
  Vector3 localCapsuleDir = box.rotation.invRotate(cap.direction).getNormalized();

  // Define segment edges on local space
  Vector3 A = localCapsuleCenter - localCapsuleDir * (cap.height / 2.0f);
  Vector3 B = localCapsuleCenter + localCapsuleDir * (cap.height / 2.0f);
  Vector3 AB = B - A;

  // Test multiple points along the capsule axis
  const int steps = 10;
  float minDistSq = MAX_FLOAT;
  Vector3 bestP = Vector3::ZERO;
  Vector3 bestQ = Vector3::ZERO;

  for (int i = 0; i <= steps; ++i) {
    float t = (float)i / steps;
    Vector3 p = A + AB * t;
    Vector3 q = p.clamp(-box.extent, box.extent); // AABB local

    float distSq = (q - p).lenghtSq();
    if (distSq < minDistSq) {
      minDistSq = distSq;
      bestP = p;
      bestQ = q;
    }
  }

  if (minDistSq > cap.radius * cap.radius) {
    return false;
  }

  float distance = sqrt(minDistSq);
  colInfo.penetrationDepth = cap.radius - distance;

  Vector3 normal = (bestQ - bestP);
  if (distance > SMALL_NUMBER) {
    normal = normal * (1.0f / distance);
  }
  else {
    normal = Vector3::RIGHT;
  }

  // Turn to global space
  colInfo.normal = box.rotation.rotate(normal);
  Vector3 contactLocal = bestP + normal * cap.radius;
  colInfo.contactPoint1 = box.rotation.rotate(contactLocal) + box.center;
  colInfo.contactPoint2 = box.rotation.rotate(bestQ + normal * cap.radius) + box.center;

  return true;
}

bool
PlatformMath::intersect(const OBBox& box, const Capsule& cap)
{
  // Transform capsule to obb local space
  Vector3 localCapsuleCenter = box.rotation.invRotate(cap.center - box.center);
  Vector3 localCapsuleDir = box.rotation.invRotate(cap.direction).getNormalized();

  // Define segment edges on local space
  Vector3 A = localCapsuleCenter - localCapsuleDir * (cap.height / 2.0f);
  Vector3 B = localCapsuleCenter + localCapsuleDir * (cap.height / 2.0f);
  Vector3 AB = B - A;

  // Test multiple points along the capsule axis
  const int steps = 10;
  float minDistSq = MAX_FLOAT;
  Vector3 bestP = Vector3::ZERO;
  Vector3 bestQ = Vector3::ZERO;

  for (int i = 0; i <= steps; ++i) {
    float t = (float)i / steps;
    Vector3 p = A + AB * t;
    Vector3 q = p.clamp(-box.extent, box.extent); // AABB local

    float distSq = (q - p).lenghtSq();
    if (distSq < minDistSq) {
      minDistSq = distSq;
      bestP = p;
      bestQ = q;
    }
  }

  return (minDistSq > cap.radius * cap.radius);
}

bool
PlatformMath::intersect(const AABBox& box, const Plane& plane, CollisionInfo& colInfo)
{
  auto vertices = box.getVertices();
  Array<float, 8> evaluations;

  for (uint8 i = 0; i < 8; ++i) {
    evaluations[i] = plane.evaluate(vertices[i]);
  }

  bool allPositive = true;
  bool allNegative = true;

  for (float val : evaluations) {
    if (val > 0.0f) {
      allNegative = false;
    }
    if (val < 0.0f) {
      allPositive = false;
    }
  }

  if (allPositive || allNegative) {
    return false; // No intersection
  }
  
  // Calculate collision info
  colInfo.penetrationDepth = 0.0f; // Placeholder, actual depth calculation needed
  colInfo.normal = plane.normal;
  colInfo.contactPoint1 = Vector3::ZERO; // Placeholder, actual contact point needed
  colInfo.contactPoint2 = Vector3::ZERO; // Placeholder, actual contact point needed
  
  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const AABBox& box, const Plane& plane)
{
  auto vertices = box.getVertices();
  Array<float, 8> evaluations;

  for (uint8 i = 0; i < 8; ++i) {
    evaluations[i] = plane.evaluate(vertices[i]);
  }

  bool allPositive = true;
  bool allNegative = true;

  for (float val : evaluations) {
    if (val > 0.0f) {
      allNegative = false;
    }
    if (val < 0.0f) {
      allPositive = false;
    }
  }

  return !(allPositive || allNegative);
}

bool
PlatformMath::intersect(const OBBox& box, const Plane& plane, CollisionInfo& colInfo)
{
  auto corners = box.getCorners();

  bool allPositive = true;
  bool allNegative = true;

  for (const auto& corner : corners) {
    float distance = plane.normal.dot(corner) - plane.distance;

    if (distance > 0) {
      allNegative = false;
    }
    else if (distance < 0) {
      allPositive = false;
    }

    if (!allPositive && !allNegative) {
      return true;
    }
  }

  // Calculate collision info
  colInfo.penetrationDepth = 0.0f; // Placeholder, actual depth calculation needed
  colInfo.normal = plane.normal;
  colInfo.contactPoint1 = Vector3::ZERO; // Placeholder, actual contact point needed
  colInfo.contactPoint2 = Vector3::ZERO; // Placeholder, actual contact point needed

  return false;
}

bool
PlatformMath::intersect(const OBBox& box, const Plane& plane)
{
  auto corners = box.getCorners();

  bool allPositive = true;
  bool allNegative = true;

  for (const auto& corner : corners) {
    float distance = plane.normal.dot(corner) - plane.distance;

    if (distance > 0) {
      allNegative = false;
    }
    else if (distance < 0) {
      allPositive = false;
    }

    if (!allPositive && !allNegative) {
      return true;
    }
  }

  return false;
}

bool
PlatformMath::intersect(const AABBox& box, const Rect& rect, CollisionInfo& colInfo)
{
  if (box.min.x > rect.max.x || box.max.x < rect.min.x ||
      box.min.y > rect.max.y || box.max.y < rect.min.y) {
    return false; // No intersection
  }
  // Calculate collision info
  colInfo.penetrationDepth = 0.0f; // Placeholder, actual depth calculation needed
  colInfo.normal = Vector3::ZERO; // Placeholder, actual normal needed
  colInfo.contactPoint1 = Vector3::ZERO; // Placeholder, actual contact point needed
  colInfo.contactPoint2 = Vector3::ZERO; // Placeholder, actual contact point needed
  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const AABBox& box, const Rect& rect)
{
  return (box.min.x <= rect.max.x &&
          box.max.x >= rect.min.x &&
          box.min.y <= rect.max.y &&
          box.max.y >= rect.min.y);
}

bool
PlatformMath::intersect(const OBBox& box, const Rect& rect, CollisionInfo& colInfo)
{
  Array<float, 4> projX;
  Array<float, 4> projY;

  const auto vertices = box.getCorners();

  for (int8 i = 0; i < 8; ++i) {
    projX[i % 4] = vertices[i].x;
    projY[i % 4] = vertices[i].y;
  }

  const float minX = *min_element(projX.begin(), projX.end());
  const float maxX = *max_element(projX.begin(), projX.end());
  const float minY = *min_element(projY.begin(), projY.end());
  const float maxY = *max_element(projY.begin(), projY.end());

  // Calculate collision info
  colInfo.penetrationDepth = 0.0f; // Placeholder, actual depth calculation needed
  colInfo.normal = Vector3::ZERO; // Placeholder, actual normal needed
  colInfo.contactPoint1 = Vector3::ZERO; // Placeholder, actual contact point needed
  colInfo.contactPoint2 = Vector3::ZERO; // Placeholder, actual contact point needed

  return (minX <= rect.max.x &&
          maxX >= rect.min.x &&
          minY <= rect.max.y &&
          maxY >= rect.min.y);
}

bool
PlatformMath::intersect(const OBBox& box, const Rect& rect)
{
  Array<float, 4> projX;
  Array<float, 4> projY;

  const auto vertices = box.getCorners();

  for (int8 i = 0; i < 8; ++i) {
    projX[i % 4] = vertices[i].x;
    projY[i % 4] = vertices[i].y;
  }

  const float minX = *min_element(projX.begin(), projX.end());
  const float maxX = *max_element(projX.begin(), projX.end());
  const float minY = *min_element(projY.begin(), projY.end());
  const float maxY = *max_element(projY.begin(), projY.end());

  return (minX <= rect.max.x &&
          maxX >= rect.min.x &&
          minY <= rect.max.y &&
          maxY >= rect.min.y);
}

bool
PlatformMath::intersect(const Sphere& sph, const Sphere& sph1, CollisionInfo& colInfo)
{
  const float distance = sqrt(((sph.center.x - sph1.center.x) *
                               (sph.center.x - sph1.center.x)) +
                              ((sph.center.y - sph1.center.y) *
                               (sph.center.y - sph1.center.y)) +
                              ((sph.center.z - sph1.center.z) *
                               (sph.center.z - sph1.center.z)));
  if (distance >= (sph.radius + sph1.radius)) {
    return false; // No intersection
  }
  colInfo.penetrationDepth = (sph.radius + sph1.radius) - distance;
  if (distance > SMALL_NUMBER) {
    colInfo.normal = Vector3((sph1.center.x - sph.center.x) / distance,
                             (sph1.center.y - sph.center.y) / distance,
                             (sph1.center.z - sph.center.z) / distance);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0); // Arbitrary normal if spheres are at the same point
  }
  
  colInfo.contactPoint1 = sph.center + colInfo.normal * sph.radius;
  colInfo.contactPoint2 = sph1.center + colInfo.normal * sph1.radius;
  return true;
}

bool
PlatformMath::intersect(const Sphere& sph, const Sphere& sph1)
{
  const float distance = sqrt((sph.center.x - sph1.center.x) +
                              (sph.center.y - sph1.center.y) +
                              (sph.center.z - sph1.center.z));

  return (distance < (sph.radius + sph1.radius));
}

bool
PlatformMath::intersect(const AABBox& box, const Sphere& sph, CollisionInfo& colInfo)
{
  const float x = max(box.min.x, min(sph.center.x, box.max.x));
  const float y = max(box.min.y, min(sph.center.y, box.max.y));
  const float z = max(box.min.z, min(sph.center.z, box.max.z));
  const float distance = sqrt(((x - sph.center.x) * (x - sph.center.x)) +
                              ((y - sph.center.y) * (y - sph.center.y)) +
                              ((z - sph.center.z) * (z - sph.center.z)));
  colInfo.penetrationDepth = sph.radius - distance;
  if (distance > SMALL_NUMBER) {
    colInfo.normal = Vector3((x - sph.center.x) / distance,
                             (y - sph.center.y) / distance,
                             (z - sph.center.z) / distance);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0); // Arbitrary normal if point is at the center
  }
  
  colInfo.contactPoint1 = Vector3(x, y, z) + colInfo.normal * sph.radius;
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;
  return distance < sph.radius;
}

bool
PlatformMath::intersect(const AABBox& box, const Sphere& sph)
{
  const float x = max(box.min.x, min(sph.center.x, box.max.x));
  const float y = max(box.min.y, min(sph.center.y, box.max.y));
  const float z = max(box.min.z, min(sph.center.z, box.max.z));

  const float distance = sqrt(((x - sph.center.x) * (x - sph.center.x)) +
                              ((y - sph.center.y) * (y - sph.center.y)) +
                              ((z - sph.center.z) * (z - sph.center.z)));

  return distance < sph.radius;
}



bool
PlatformMath::intersect(const OBBox& box, const Sphere& sph, CollisionInfo& colInfo)
{
  Vector3 sphereToBox = sph.center - box.center;
  Vector3 transformedSphCenter = box.rotation.invRotate(sphereToBox);
  const float clampedX = clamp(transformedSphCenter.x, -box.extent.x, box.extent.x);
  const float clampedY = clamp(transformedSphCenter.y, -box.extent.y, box.extent.y);
  const float clampedZ = clamp(transformedSphCenter.z, -box.extent.z, box.extent.z);
  const Vector3 closestPoint = box.center + box.rotation.rotate(Vector3(clampedX, clampedY, clampedZ));
  const float dist = (closestPoint - sph.center).mag();
  colInfo.penetrationDepth = sph.radius - dist;
  if (dist > SMALL_NUMBER) {
    colInfo.normal = (closestPoint - sph.center) * (1.0f / dist);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0); // Arbitrary normal if point is at the center
  }
  colInfo.contactPoint1 = closestPoint + colInfo.normal * sph.radius;
  // Contact point 2 is not applicable for point intersection, so we leave it as zero
  colInfo.contactPoint2 = Vector3::ZERO;

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const OBBox& box, const Sphere& sph)
{
  Vector3 sphereToBox = sph.center - box.center;
  Vector3 transformedSphCenter = box.rotation.rotate(sphereToBox);

  const float clampedX = clamp(transformedSphCenter.x, -box.extent.x, box.extent.x);
  const float clampedY = clamp(transformedSphCenter.y, -box.extent.y, box.extent.y);
  const float clampedZ = clamp(transformedSphCenter.z, -box.extent.z, box.extent.z);

  const Vector3 closestPoint = box.center +
                               box.rotation.rotate(Vector3(clampedX, clampedY, clampedZ));
  const float dist = (closestPoint - sph.center).mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const Sphere& sph, const Capsule& cap, CollisionInfo& colInfo)
{
  Vector3 dir = cap.direction.getNormalized();
  Vector3 A = cap.center - dir * (cap.height / 2.0f);
  Vector3 B = cap.center + dir * (cap.height / 2.0f);

  // Closest point on the capsule axis to the sphere center
  Vector3 closest = sph.center.closestPointOnSegment(A, B);
  Vector3 diff = sph.center - closest;
  float distSq = diff.dot(diff);
  float radiusSum = cap.radius + sph.radius;

  if (distSq > radiusSum * radiusSum) {
    return false;
  }

  float distance = sqrt(distSq);
  colInfo.penetrationDepth = radiusSum - distance;

  if (distance > SMALL_NUMBER) {
    colInfo.normal = diff * (1.0f / distance);
  }
  else {
    colInfo.normal = Vector3::RIGHT;
  }

  colInfo.contactPoint1 = closest + colInfo.normal * cap.radius;
  colInfo.contactPoint2 = sph.center + colInfo.normal * sph.radius;
  return true;
}

bool
PlatformMath::intersect(const Sphere& sph, const Capsule& cap)
{
  Vector3 dir = cap.direction.getNormalized();
  Vector3 A = cap.center - dir * (cap.height / 2.0f);
  Vector3 B = cap.center + dir * (cap.height / 2.0f);

  // Closest point on the capsule axis to the sphere center
  Vector3 closest = sph.center.closestPointOnSegment(A, B);
  Vector3 diff = sph.center - closest;
  float distSq = diff.dot(diff);
  float radiusSum = cap.radius + sph.radius;

  if (distSq > radiusSum * radiusSum) {
    return false;
  }
  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const Sphere& sph, const Plane& plane, CollisionInfo&)
{
  float dist = PlatformMath::abs(plane.normal.dot(sph.center) - plane.distance) /
                                 plane.normal.mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const Sphere& sph, const Plane& plane)
{
  float dist = PlatformMath::abs(plane.normal.dot(sph.center) - plane.distance) /
                                 plane.normal.mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const Sphere& sph, const Rect& rect, CollisionInfo&)
{
  const float closestX = clamp(sph.center.x, rect.min.x, rect.max.x);
  const float closestY = clamp(sph.center.y, rect.min.y, rect.max.y);

  const float distX = sph.center.x - closestX;
  const float distY = sph.center.x - closestY;

  return ((distX * distX + distY * distY) <= (sph.radius * sph.radius));
}

bool
PlatformMath::intersect(const Sphere& sph, const Rect& rect)
{
  const float closestX = clamp(sph.center.x, rect.min.x, rect.max.x);
  const float closestY = clamp(sph.center.y, rect.min.y, rect.max.y);

  const float distX = sph.center.x - closestX;
  const float distY = sph.center.x - closestY;

  return ((distX * distX + distY * distY) <= (sph.radius * sph.radius));
}

void
closestPtSegmentSegment(const Vector3& p1,
                        const Vector3& q1,
                        const Vector3& p2,
                        const Vector3& q2,
                        Vector3& c1,
                        Vector3& c2)
{
  const Vector3 d1 = q1 - p1;
  const Vector3 d2 = q2 - p2;
  const Vector3 r = p1 - p2;
  const float a = d1.dot(d1);
  const float e = d2.dot(d2);
  const float f = d2.dot(r);

  float s = 0.0f;
  float t = 0.0f;

  if (a <= 1e-6f) {
    s = 0.0f;
    t = f / e;
    t = PlatformMath::clamp(t, 0.0f, 1.0f);
  }
  else {
    float c = d1.dot(r);
    if (e <= 1e-6f) {
      t = 0.0f;
      s = PlatformMath::max(0.0f, PlatformMath::min(1.0f, -c / a));
    }
    else {
      float b = d1.dot(d2);
      float denom = a * e - b * b;

      if (denom != 0.0f) {
        s = PlatformMath::max(0.0f, PlatformMath::min(1.0f, (b * f - c * e) / denom));
      }
      else {
        s = 0.0f;
      }

      t = (b * s + f) / e;
      t = PlatformMath::max(0.0f, PlatformMath::min(1.0f, t));
    }
  }

  s = PlatformMath::max(0.0f, PlatformMath::min(1.0f, (d1.dot((p2 + d2 * t) - p1)) / a));
  c1 = p1 + d1 * s;
  c2 = p2 + d2 * t;
}

bool
PlatformMath::intersect(const Capsule& cap, const Capsule& cap1, CollisionInfo& colInfo)
{
  const Vector3 d1 = cap.direction.getNormalized();
  const Vector3 d2 = cap1.direction.getNormalized();

  const Vector3 A1 = cap.center - d1 * (cap.height / 2.0f);
  const Vector3 B1 = cap.center + d1 * (cap.height / 2.0f);
  const Vector3 A2 = cap1.center - d2 * (cap1.height / 2.0f);
  const Vector3 B2 = cap1.center + d2 * (cap1.height / 2.0f);

  Vector3 pt1 = Vector3::ZERO;
  Vector3 pt2 = Vector3::ZERO;
  closestPtSegmentSegment(A1, B1, A2, B2, pt1, pt2);

  const Vector3 delta = pt2 - pt1;
  const float distSq = delta.lenghtSq();
  const float radiusSum = cap.radius + cap1.radius;

  if (distSq > radiusSum * radiusSum) {
    return false;
  }

  float dist = sqrt(distSq);
  colInfo.penetrationDepth = radiusSum - dist;

  if (dist > 1e-6f) {
    colInfo.normal = delta * (1.0f / dist);
  }
  else {
    colInfo.normal = Vector3(1, 0, 0);
  }

  colInfo.contactPoint1 = pt1 + colInfo.normal * cap.radius;
  colInfo.contactPoint2 = pt2 + colInfo.normal * cap1.radius;
  return true;
}

bool
PlatformMath::intersect(const Capsule& cap, const Capsule& cap1)
{
  const Vector3 d1 = cap.direction.getNormalized();
  const Vector3 d2 = cap1.direction.getNormalized();

  const Vector3 A1 = cap.center - d1 * (cap.height / 2.0f);
  const Vector3 B1 = cap.center + d1 * (cap.height / 2.0f);
  const Vector3 A2 = cap1.center - d2 * (cap1.height / 2.0f);
  const Vector3 B2 = cap1.center + d2 * (cap1.height / 2.0f);

  Vector3 pt1 = Vector3::ZERO;
  Vector3 pt2 = Vector3::ZERO;
  closestPtSegmentSegment(A1, B1, A2, B2, pt1, pt2);

  const Vector3 delta = pt2 - pt1;
  const float distSq = delta.lenghtSq();
  const float radiusSum = cap.radius + cap1.radius;

  return (distSq > radiusSum * radiusSum);
}

bool
PlatformMath::intersect(const Capsule& cap, const Plane& plane, CollisionInfo& colInfo)
{
  // Project capsule points onto the plane
  const Vector3 dir = cap.direction.getNormalized();
  const Vector3 A = cap.center - dir * (cap.height / 2.0f);
  const Vector3 B = cap.center + dir * (cap.height / 2.0f);
  float distA = plane.evaluate(A);
  float distB = plane.evaluate(B);
  if (distA > cap.radius || distB > cap.radius) {
    return false; // No intersection
  }
  // Calculate collision info
  colInfo.penetrationDepth = cap.radius - min(distA, distB);
  colInfo.normal = plane.normal;
  colInfo.contactPoint1 = A + colInfo.normal * cap.radius;
  colInfo.contactPoint2 = B + colInfo.normal * cap.radius;
  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const Capsule& cap, const Plane& plane)
{
  // Project capsule points onto the plane
  const Vector3 dir = cap.direction.getNormalized();
  const Vector3 A = cap.center - dir * (cap.height / 2.0f);
  const Vector3 B = cap.center + dir * (cap.height / 2.0f);
  float distA = plane.evaluate(A);
  float distB = plane.evaluate(B);
  
  // Check if the capsule intersects the plane
  return (distA <= cap.radius || distB <= cap.radius);
}

bool
PlatformMath::intersect(const Capsule& cap, const Rect& rect, CollisionInfo& colInfo)
{
  // Check if the capsule's bounding box intersects with the rectangle's bounding box
  const Vector3 dir = cap.direction.getNormalized();
  const Vector3 A = cap.center - dir * (cap.height / 2.0f);
  const Vector3 B = cap.center + dir * (cap.height / 2.0f);
  // Check if the capsule's bounding box intersects with the rectangle's bounding box
  if (A.x > rect.max.x || B.x < rect.min.x ||
      A.y > rect.max.y || B.y < rect.min.y) {
    return false; // No intersection
  }
  // Calculate collision info
  colInfo.penetrationDepth = cap.radius; // Placeholder, actual depth calculation needed
  colInfo.normal = Vector3::ZERO; // Placeholder, actual normal needed
  colInfo.contactPoint1 = Vector3::ZERO; // Placeholder, actual contact point needed
  colInfo.contactPoint2 = Vector3::ZERO; // Placeholder, actual contact point needed
  return true; // Intersection occurred
}

bool
PlatformMath::intersect(const Capsule& cap, const Rect& rect)
{
  // Check if the capsule's bounding box intersects with the rectangle's bounding box
  const Vector3 dir = cap.direction.getNormalized();
  const Vector3 A = cap.center - dir * (cap.height / 2.0f);
  const Vector3 B = cap.center + dir * (cap.height / 2.0f);
  // Check if the capsule's bounding box intersects with the rectangle's bounding box
  if (A.x > rect.max.x || B.x < rect.min.x ||
      A.y > rect.max.y || B.y < rect.min.y) {
    return false; // No intersection
  }
  // If we reach here, it means the bounding boxes intersect
  return true;
}

bool
PlatformMath::intersect(const Plane& plane, const Plane& plane1, CollisionInfo&)
{
  const Vector3 lineDir = plane.normal.cross(plane1.normal);

  if (lineDir.x == 0.0f && lineDir.y == 0.0f && lineDir.z == 0.0f) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Plane& plane, const Plane& plane1)
{
  const Vector3 lineDir = plane.normal.cross(plane1.normal);

  if (lineDir.x == 0.0f && lineDir.y == 0.0f && lineDir.z == 0.0f) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Plane& plane, const Rect& rect, CollisionInfo&)
{
  auto vertices = rect.getVertices();
  Array<float, 4> evaluations;

  for (int8 i = 0; i < 4; ++i) {
    evaluations[i] = plane.evaluate(vertices[i]);
  }

  bool allPositive = all_of(evaluations.begin(), evaluations.end(),
                            [](float val) { return val > 0.0f; });
  bool allNegative = all_of(evaluations.begin(), evaluations.end(),
                            [](float val) { return val < 0.0f; });

  if (allPositive || allNegative) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Plane& plane, const Rect& rect)
{
  auto vertices = rect.getVertices();
  Array<float, 4> evaluations;

  for (int8 i = 0; i < 4; ++i) {
    evaluations[i] = plane.evaluate(vertices[i]);
  }

  bool allPositive = all_of(evaluations.begin(), evaluations.end(),
                            [](float val) { return val > 0.0f; });
  bool allNegative = all_of(evaluations.begin(), evaluations.end(),
                            [](float val) { return val < 0.0f; });

  if (allPositive || allNegative) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Rect& rect, const Rect& rect1, CollisionInfo&)
{
  return (rect.min.x <= rect1.max.x &&
          rect.max.x >= rect1.min.x &&
          rect.min.y <= rect1.max.y &&
          rect.max.y >= rect1.min.y);
}

bool
PlatformMath::intersect(const Rect& rect, const Rect& rect1)
{
  return (rect.min.x <= rect1.max.x &&
          rect.max.x >= rect1.min.x &&
          rect.min.y <= rect1.max.y &&
          rect.max.y >= rect1.min.y);
}
}
