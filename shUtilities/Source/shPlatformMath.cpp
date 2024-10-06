/*************************************************************/
/*
*  @file    shSPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
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

bool
PlatformMath::pointBoxIntersect(const Vector3& point, const shBoxOBB& box)
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

bool
PlatformMath::pointCapsuleIntersect(const Vector3& point, const shCapsule& cap)
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
PlatformMath::pointRectIntersect(const Vector2& point, const shRect& rect)
{
  return rect.pointIntersect(point);
}

bool
PlatformMath::pointPlaneIntersect(const Vector3& point, const shPlane& plane)
{
  const float distance = plane.pointToPlaneDistance(point);

  return PlatformMath::abs(distance) < SMALL_NUMBER;
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

bool
PlatformMath::boxBoxIntersect(const shBoxOBB& box, const shBoxOBB& box1)
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
PlatformMath::boxBoxIntersect(const shBoxAAB& boxA, const shBoxOBB& boxO)
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
PlatformMath::boxCapsuleIntersect(const shBoxAAB& box, const shCapsule& cap)
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
PlatformMath::boxCapsuleIntersect(const shBoxOBB& box, const shCapsule& cap)
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
PlatformMath::boxPlaneIntersect(const shBoxAAB& box, const shPlane& plane)
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
PlatformMath::boxPlaneIntersect(const shBoxOBB& box, const shPlane& plane)
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
PlatformMath::boxRectIntersect(const shBoxAAB& box, const shRect& rect)
{
  return (box.min.x <= rect.max.x &&
          box.max.x >= rect.min.x &&
          box.min.y <= rect.max.y &&
          box.max.y >= rect.min.y);
}

bool
PlatformMath::boxRectIntersect(const shBoxOBB& box, const shRect& rect)
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

bool
PlatformMath::boxSphereIntersect(const shBoxOBB& box, const shSphere& sph)
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
PlatformMath::sphereCapsuleIntersect(const shSphere& sph, const shCapsule& cap)
{
  const Vector3 closestPoint = sph.center.closestPointOnSegment(cap.pointA, cap.pointB);

  const float dist = (closestPoint - sph.center).mag();

  return (dist <= (sph.radius + cap.radius));
}

bool
PlatformMath::spherePlaneIntersect(const shSphere& sph, const shPlane& plane)
{
  float dist = PlatformMath::abs(plane.normal.dot(sph.center) - plane.distance) /
               plane.normal.mag();

  return (dist <= sph.radius);
}

bool
PlatformMath::sphereRectIntersect(const shSphere& sph, const shRect& rect)
{
  const float closestX = clamp(sph.center.x, rect.min.x, rect.max.x);
  const float closestY = clamp(sph.center.y, rect.min.y, rect.max.y);

  const float distX = sph.center.x - closestX;
  const float distY = sph.center.x - closestY;

  return ((distX * distX + distY * distY) <= (sph.radius * sph.radius));
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

bool
PlatformMath::capsulePlaneIntersect(const shCapsule& cap, const shPlane& plane)
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
PlatformMath::capsuleRectIntersect(const shCapsule& cap, const shRect& rect)
{
  // Capsule points projected
  const Vector2 projCapA(cap.pointA.x, cap.pointA.y);
  const Vector2 projCapB(cap.pointB.x, cap.pointB.y);

  shSphere sphereA(cap.pointA, cap.radius);
  shSphere sphereB(cap.pointB, cap.radius);

  if(sphereRectIntersect(sphereA, rect) ||
     sphereRectIntersect(sphereB, rect)) {
    return true;
  }

  return rect.lineIntersect(projCapA, projCapB);
}

bool
PlatformMath::planePlaneIntersect(const shPlane& plane, const shPlane& plane1)
{
  const Vector3 lineDir = plane.normal.cross(plane1.normal);

  if (lineDir.x == 0.0f && lineDir.y == 0.0f && lineDir.z == 0.0f) {
    return false;
  }

  return true;
}

bool
PlatformMath::planeRectIntersect(const shPlane& plane, const shRect& rect)
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
PlatformMath::rectRectIntersect(const shRect& rect, const shRect& rect1)
{
  return (rect.min.x <= rect1.max.x &&
          rect.max.x >= rect1.min.x &&
          rect.min.y <= rect1.max.y &&
          rect.max.y >= rect1.min.y);
}
}
