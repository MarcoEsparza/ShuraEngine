/*************************************************************/
/*
*  @file    shPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/29
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
#include "shVector3.h"
#include "shVector2.h"
#include "shAABBox.h"
#include "shOBBox.h"
#include "shRect.h"
#include "shPlane.h"
#include "shCapsule.h"
#include "shSphere.h"
#include "shDegree.h"
#include "shRadian.h"
#include "shCollisionInfo.h"

namespace shEngineSDK {
 /*************************************************************/
 /*
 *  Static variables
 */
 /*************************************************************/

const float PlatformMath::PI = 2 * asin(Radian(1.0f));

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

/*************************************************************/
/*
*  Trigonometric functions
*/
/*************************************************************/

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
PlatformMath::atan2(const Radian& rad1, const Radian& rad2)
{
  return std::atan2(rad1.getValueOnRadians(), rad2.getValueOnRadians());
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

/*************************************************************/
/*
*  Figures
*/
/*************************************************************/

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
  colInfo.contactPoint = point;

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
PlatformMath::intersect(const Vector3& point, const OBBox& box, CollisionInfo& colInfo)
{
  Vector3 right, up, forward;
  box.rotation.toAxes(right, up, forward);

  const Vector3 localPoint = point - box.center;

  float projRight = localPoint.dot(right);
  float projUp = localPoint.dot(up);
  float projForward = localPoint.dot(forward);

  if (projRight < -box.extent.x || projRight > box.extent.x &&
      projUp < -box.extent.y || projUp > box.extent.y &&
      projForward < -box.extent.z || projForward > box.extent.z) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Vector3& point, const OBBox& box)
{
  return false;
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

  return distance < sph.radius;
}

bool
PlatformMath::intersect(const Vector3& point, const Sphere& sph)
{
  return false;
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

  colInfo.contactPoint = closestPoint + colInfo.normal * cap.radius;
  return true;
}

bool
PlatformMath::intersect(const Vector3& point, const Capsule& cap)
{
  const Vector3 pointAB = cap.pointB - cap.pointA;
  const Vector3 pointAP = point - cap.pointA;
  const Vector3 abNormalized = pointAB.getNormalized();

  float proj = pointAP.dot(abNormalized);

  if (proj < 0.0f) {
    proj = 0.0f;
  }
  else if (proj > pointAB.mag()) {
    proj = pointAB.mag();
  }

  const Vector3 closestPoint = cap.pointA + abNormalized * proj;
  const Vector3 diff = point - closestPoint;

  return (diff.mag() <= cap.radius);
}

bool
PlatformMath::intersect(const Vector2& point, const Rect& rect, CollisionInfo& colInfo)
{
  return rect.pointIntersect(point);
}

bool
PlatformMath::intersect(const Vector2& point, const Rect& rect)
{
  return false;
}

bool
PlatformMath::intersect(const Vector3& point, const Plane& plane, CollisionInfo& colInfo)
{
  const float distance = plane.pointToPlaneDistance(point);

  return PlatformMath::abs(distance) < SMALL_NUMBER;
}

bool
PlatformMath::intersect(const Vector3& point, const Plane& plane)
{
  return false;
}

bool
PlatformMath::intersect(const AABBox& box, const AABBox& box1, CollisionInfo& colInfo)
{
  return (box.min.x <= box1.max.x &&
          box.max.x >= box1.min.x &&
          box.min.y <= box1.max.y &&
          box.max.y >= box1.min.y &&
          box.min.z <= box1.max.z &&
          box.max.z >= box1.min.z);
}

bool
PlatformMath::intersect(const AABBox& box, const AABBox& box1)
{
  return false;
}

bool
PlatformMath::intersect(const OBBox& box, const OBBox& box1, CollisionInfo& colInfo)
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

bool
PlatformMath::intersect(const OBBox& box, const OBBox& box1)
{
  return false;
}

bool
PlatformMath::intersect(const AABBox& boxA, const OBBox& boxO, CollisionInfo& colInfo)
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
PlatformMath::intersect(const AABBox& boxA, const OBBox& boxO)
{
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
  float tMin = 0.0f;
  float minDistSq = FLT_MAX;
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
      tMin = t;
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
  colInfo.contactPoint = bestP + normal * cap.radius;

  return true;
}

bool
PlatformMath::intersect(const AABBox& box, const Capsule& cap)
{
  Vector3 closestPoint(0.0f, 0.0f, 0.0f);

  closestPoint.x = (PlatformMath::max(box.min.x, PlatformMath::min(cap.pointA.x, box.max.x)));
  closestPoint.y = (PlatformMath::max(box.min.y, PlatformMath::min(cap.pointA.y, box.max.y)));
  closestPoint.z = (PlatformMath::max(box.min.z, PlatformMath::min(cap.pointA.z, box.max.z)));

  const Vector3 diffA = closestPoint - cap.pointA;
  const float distA = diffA.mag();

  closestPoint.x = (PlatformMath::max(box.min.x, PlatformMath::min(cap.pointB.x, box.max.x)));
  closestPoint.y = (PlatformMath::max(box.min.y, PlatformMath::min(cap.pointB.y, box.max.y)));
  closestPoint.z = (PlatformMath::max(box.min.z, PlatformMath::min(cap.pointB.z, box.max.z)));

  const Vector3 diffB = closestPoint - cap.pointB;
  const float distB = diffB.mag();

  return (distA <= cap.radius || distB <= cap.radius);
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
  float minDistSq = FLT_MAX;
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

  float distance = std::sqrt(minDistSq);
  colInfo.penetrationDepth = cap.radius - distance;

  Vector3 normal = (bestQ - bestP);
  if (distance > SMALL_NUMBER) {
    normal = normal * (1.0f / distance);
  }
  else {
    normal = Vector3::RIGHT;
  }

  // Turn to global space
  colInfo.normal = box.rotation.toRotate(normal);
  Vector3 contactLocal = bestP + normal * cap.radius;
  colInfo.contactPoint = box.rotation.toRotate(contactLocal) + box.center;

  return true;
}

bool
PlatformMath::intersect(const OBBox& box, const Capsule& cap)
{
  Vector<Vector3> obbAxes;
  obbAxes.resize(3);
  box.rotation.toAxes(obbAxes[0], obbAxes[1], obbAxes[2]);

  // Projection on obb axis
  for (uint8 i = 0; i < 3; ++i) {
    float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;

    box.projectOnAxis(obbAxes[i], min1, max1);
    cap.projectOnAxis(obbAxes[i], min2, max2);

    if (!box.overlapOnProjection(min1, max1, min2, max2)) {
      return false;
    }
  }

  // Projection on capsule dir
  Vector3 capDir = cap.pointB - cap.pointA;
  capDir.normalize();
  Vector<Vector3> capAxes = { capDir, Vector3(capDir.y, -capDir.x, 0.0f) };

  for (uint8 i = 0; i < 2; ++i) {
    float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;
    box.projectOnAxis(capAxes[i], min1, max1);
    cap.projectOnAxis(capAxes[i], min2, max2);

    if (!box.overlapOnProjection(min1, max1, min2, max2)) {
      return false;
    }
  }

  // Axis combination
  for (uint8 i = 0; i < 3; ++i) {
    for (uint8 j = 0; j < 2; ++j) {
      Vector3 axis = obbAxes[i].cross(capAxes[j]);

      if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
        continue;
      }

      float min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f;
      box.projectOnAxis(axis, min1, max1);
      cap.projectOnAxis(axis, min2, max2);

      if (!box.overlapOnProjection(min1, max1, min2, max2)) {
        return false;
      }
    }
  }

  return true;
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

  return !(allPositive || allNegative);
}

bool
PlatformMath::intersect(const AABBox& box, const Plane& plane)
{
  return false;
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

  return false;
}

bool
PlatformMath::intersect(const OBBox& box, const Plane& plane)
{
  return false;
}

bool
PlatformMath::intersect(const AABBox& box, const Rect& rect, CollisionInfo& colInfo)
{
  return (box.min.x <= rect.max.x &&
          box.max.x >= rect.min.x &&
          box.min.y <= rect.max.y &&
          box.max.y >= rect.min.y);
}

bool
PlatformMath::intersect(const AABBox& box, const Rect& rect)
{
  return false;
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

  return (minX <= rect.max.x &&
          maxX >= rect.min.x &&
          minY <= rect.max.y &&
          maxY >= rect.min.y);
}

bool
PlatformMath::intersect(const OBBox& box, const Rect& rect)
{
  return false;
}

bool
PlatformMath::intersect(const Sphere& sph, const Sphere& sph1, CollisionInfo& colInfo)
{
  const float distance = sqrt((sph.center.x - sph1.center.x) +
                              (sph.center.y - sph1.center.y) +
                              (sph.center.z - sph1.center.z));

  return (distance < (sph.radius + sph1.radius));
}

bool
PlatformMath::intersect(const Sphere& sph, const Sphere& sph1)
{
  return false;
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

  return distance < sph.radius;
}

bool
PlatformMath::intersect(const AABBox& box, const Sphere& sph)
{
  return false;
}



bool
PlatformMath::intersect(const OBBox& box, const Sphere& sph, CollisionInfo& colInfo)
{
  Vector3 sphereToBox = sph.center - box.center;
  Vector3 transformedSphCenter = box.rotation.toRotate(sphereToBox);

  const float clampedX = clamp(transformedSphCenter.x, -box.extent.x, box.extent.x);
  const float clampedY = clamp(transformedSphCenter.y, -box.extent.y, box.extent.y);
  const float clampedZ = clamp(transformedSphCenter.z, -box.extent.z, box.extent.z);

  const Vector3 closestPoint = box.center +
                               box.rotation.toRotate(Vector3(clampedX, clampedY, clampedZ));
  const float dist = (closestPoint - sph.center).mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const OBBox& box, const Sphere& sph)
{
  return false;
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

  float distance = std::sqrt(distSq);
  colInfo.penetrationDepth = radiusSum - distance;

  if (distance > SMALL_NUMBER) {
    colInfo.normal = diff * (1.0f / distance);
  }
  else {
    colInfo.normal = Vector3::RIGHT;
  }

  colInfo.contactPoint = closest + colInfo.normal * cap.radius;
  return true;
}

bool
PlatformMath::intersect(const Sphere& sph, const Capsule& cap)
{
  const Vector3 closestPoint = sph.center.closestPointOnSegment(cap.pointA, cap.pointB);
  const float dist = (closestPoint - sph.center).mag();
  return (dist <= (sph.radius + cap.radius));
}

bool
PlatformMath::intersect(const Sphere& sph, const Plane& plane, CollisionInfo& colInfo)
{
  float dist = PlatformMath::abs(plane.normal.dot(sph.center) - plane.distance) /
               plane.normal.mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::intersect(const Sphere& sph, const Plane& plane)
{
  return false;
}

bool
PlatformMath::intersect(const Sphere& sph, const Rect& rect, CollisionInfo& colInfo)
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
  return false;
}

bool
PlatformMath::intersect(const Capsule& cap, const Capsule& cap1, CollisionInfo& colInfo)
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

bool
PlatformMath::intersect(const Capsule& cap, const Capsule& cap1)
{
  return false;
}

bool
PlatformMath::intersect(const Capsule& cap, const Plane& plane, CollisionInfo& colInfo)
{
  float distA = plane.pointToPlaneDistance(cap.pointA);
  float distB = plane.pointToPlaneDistance(cap.pointB);

  if (distA <= cap.radius || distB >= cap.radius) {
    return true;
  }

  const Vector3 segment = cap.pointB - cap.pointA;
  const float t = -(plane.normal.dot(cap.pointA) - plane.distance) /
                  plane.normal.dot(segment);

  if (t >= 0.0f && t <= 1.0f) {
    return true;
  }

  return false;
}

bool
PlatformMath::intersect(const Capsule& cap, const Plane& pln)
{
  return false;
}

bool
PlatformMath::intersect(const Capsule& cap, const Rect& rect, CollisionInfo& colInfo)
{
  // Capsule points projected
  const Vector2 projCapA(cap.pointA.x, cap.pointA.y);
  const Vector2 projCapB(cap.pointB.x, cap.pointB.y);

  Sphere sphereA(cap.pointA, cap.radius);
  Sphere sphereB(cap.pointB, cap.radius);

  if(intersect(sphereA, rect, colInfo) ||
     intersect(sphereB, rect, colInfo)) {
    return true;
  }

  return rect.lineIntersect(projCapA, projCapB);
}

bool
PlatformMath::intersect(const Capsule& cap, const Rect& rect)
{
  return false;
}

bool
PlatformMath::intersect(const Plane& plane, const Plane& plane1, CollisionInfo& colInfo)
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
  return false;
}

bool
PlatformMath::intersect(const Plane& plane, const Rect& rect, CollisionInfo& colInfo)
{
  auto vertices = rect.getVertices();
  Array<float, 4> evaluations;

  for (int8 i = 0; i < 4; ++i) {
    evaluations[i] = plane.evaluate(vertices[i]);
  }

  bool allPositive = std::all_of(evaluations.begin(), evaluations.end(),
                                 [](float val) { return val > 0.0f; });
  bool allNegative = std::all_of(evaluations.begin(), evaluations.end(),
                                 [](float val) { return val < 0.0f; });

  if (allPositive || allNegative) {
    return false;
  }

  return true;
}

bool
PlatformMath::intersect(const Plane& plane, const Rect& rect)
{
  return false;
}

bool
PlatformMath::intersect(const Rect& rect, const Rect& rect1, CollisionInfo& colInfo)
{
  return (rect.min.x <= rect1.max.x &&
          rect.max.x >= rect1.min.x &&
          rect.min.y <= rect1.max.y &&
          rect.max.y >= rect1.min.y);
}

bool
PlatformMath::intersect(const Rect& rect, const Rect& rect1)
{
  return false;
}
}
