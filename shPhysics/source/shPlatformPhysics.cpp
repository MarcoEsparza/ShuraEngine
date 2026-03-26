/*************************************************************/
/*
*  @file    shPlatformPhysics.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/30
*  @brief
*
*
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlatformPhysics.h"
#include "shMath.h"
#include "shOBBox.h"
#include "shAABBox.h"
#include "shCapsule.h"
#include "shSphere.h"
#include "shCollisionInfo.h"

#define MAX_FLOAT 3.402823466e+38F

namespace shEngineSDK {
const float PlatformPhysics::SLOP = 0.01f; // Tolerance for collision detection
const float PlatformPhysics::BETA = 0.8f; // Bias for collision resolution
const float PlatformPhysics::GRAVITY = 9.81f; // Gravity acceleration in m/s^2

Face::Face(const Vector3& _a, const Vector3& _b, const Vector3& _c)
{
  a = _a;
  b = _b;
  c = _c;

  normal = (b - a).cross(c - a);
  float lenSq = normal.lenghtSq();
  if (lenSq > Math::SMALL_NUMBER) {
    normal = normal * (1.0f / Math::sqrt(lenSq));
    distance = normal.dot(a);
    if (distance < 0) {
      // Ensure outward-facing normal
      normal = normal * -1;
      distance *= -1;
    }
  }
}

Vector3
PlatformPhysics::supportPointOBB(const OBBox& obb, const Vector3& dir)
{
  // Get obb axis on global space
  Vector3 axisX = obb.rotation * Vector3(1, 0, 0);
  Vector3 axisY = obb.rotation * Vector3(0, 1, 0);
  Vector3 axisZ = obb.rotation * Vector3(0, 0, 1);

  Vector3 result = obb.center;

  // Add extent to every axis, on dot sign direction
  result += axisX * obb.extent.x * (axisX.dot(dir) >= 0.0f ? 1.0f : -1.0f);
  result += axisY * obb.extent.y * (axisY.dot(dir) >= 0.0f ? 1.0f : -1.0f);
  result += axisZ * obb.extent.z * (axisZ.dot(dir) >= 0.0f ? 1.0f : -1.0f);

  return result;
}

Vector3
PlatformPhysics::supportPointAABBox(const AABBox& aabb, const Vector3& dir)
{
  return Vector3(dir.x >= 0.0f ? aabb.max.x : aabb.min.x,
                 dir.y >= 0.0f ? aabb.max.y : aabb.min.y,
                 dir.z >= 0.0f ? aabb.max.z : aabb.min.z);
}

Vector3
PlatformPhysics::supportPointCapsule(const Capsule& capsule, const Vector3& dir)
{
  Vector3 direction = dir.getNormalized();

  float halfHeight = 0.0f;
  Vector3 axis = capsule.direction.getNormalized();

  float axisDot = axis.dot(direction);
  Vector3 pointOnAxis = capsule.center + axis * (axisDot >= 0.0f ? halfHeight : -halfHeight);

  return pointOnAxis + direction * capsule.radius;
}

Vector3
PlatformPhysics::supportPointSphere(const Sphere& sphere, const Vector3& dir)
{
  Vector3 direction = dir.getNormalized();
  return sphere.center + direction * sphere.radius;
}

Vector3
PlatformPhysics::supportCSO(const OBBox& obb1, const OBBox& obb2, const Vector3& dir)
{
  return (supportPointOBB(obb1, dir) - supportPointOBB(obb2, -dir));
}

Vector3
PlatformPhysics::supportCSO(const OBBox& obb, const AABBox& aabb, const Vector3& dir)
{
  return (supportPointOBB(obb, dir) - supportPointAABBox(aabb, -dir));
}

Vector3
PlatformPhysics::supportCSO(const OBBox& obb, const Capsule& capsule, const Vector3& dir)
{
  return (supportPointOBB(obb, dir) - supportPointCapsule(capsule, -dir));
}

Vector3
PlatformPhysics::supportCSO(const OBBox& obb, const Sphere& sphere, const Vector3& dir)
{
  return (supportPointOBB(obb, dir) - supportPointSphere(sphere, -dir));
}

Vector3
PlatformPhysics::supportCSO(const AABBox& aabb1, const AABBox& aabb2, const Vector3& dir)
{
  return (supportPointAABBox(aabb1, dir) - supportPointAABBox(aabb2, -dir));
}

Vector3
PlatformPhysics::supportCSO(const AABBox& aabb, const Capsule& capsule, const Vector3& dir)
{
  return (supportPointAABBox(aabb, dir) - supportPointCapsule(capsule, -dir));
}

Vector3
PlatformPhysics::supportCSO(const AABBox& aabb, const Sphere& sphere, const Vector3& dir)
{
  return (supportPointAABBox(aabb, dir) - supportPointSphere(sphere, -dir));
}

Vector3
PlatformPhysics::supportCSO(const Capsule& capsule1,
                            const Capsule& capsule2,
                            const Vector3& dir)
{
  return (supportPointCapsule(capsule1, dir) - supportPointCapsule(capsule2, -dir));
}

Vector3
PlatformPhysics::supportCSO(const Capsule& capsule, const Sphere& sphere, const Vector3& dir)
{
  return (supportPointCapsule(capsule, dir) - supportPointSphere(sphere, -dir));
}

Vector3
PlatformPhysics::supportCSO(const Sphere& sphere1, const Sphere& sphere2, const Vector3& dir)
{
  return (supportPointSphere(sphere1, dir) - supportPointSphere(sphere2, -dir));
}

bool
PlatformPhysics::doSimplex(Vector<Vector3>& simplex, Vector3& direction)
{
  if (simplex.size() == 2) {
    const Vector3 A = simplex[1];
    const Vector3 B = simplex[2];
    const Vector3 AB = B - A;
    const Vector3 AO = -A;

    direction = AB.cross(AO).cross(AB);
    return false;
  }
  else if (simplex.size() == 3) {
    Vector3 A = simplex[2];
    Vector3 B = simplex[1];
    Vector3 C = simplex[0];
    Vector3 AB = B - A;
    Vector3 AC = C - A;
    Vector3 AO = -A;

    Vector3 ABC = AB.cross(AC);

    Vector3 ABC_perp_AB = ABC.cross(AB);
    if (ABC_perp_AB.dot(AO) > 0) {
      //  C
      simplex = { B, A };
      direction = AB.cross(AO).cross(AB);
      return false;
    }

    Vector3 ABC_perp_AC = AC.cross(ABC);
    if (ABC_perp_AC.dot(AO) > 0) {
      //  B
      simplex = { C, A };
      direction = AC.cross(AO).cross(AC);
      return false;
    }

    // Is on triangle
    if (ABC.dot(AO) > 0) {
      direction = ABC;
    }
    else {
      // Change order
      simplex = { B, C, A };
      direction = -ABC;
    }
    return false;
  }
  else if (simplex.size() == 4) {
    // Tithedron: A = last, B, C, D
    Vector3 A = simplex[3];
    Vector3 B = simplex[2];
    Vector3 C = simplex[1];
    Vector3 D = simplex[0];
    Vector3 AO = -A;

    // Face of tithedron
    Vector3 ABC_normal = (B - A).cross(C - A);
    Vector3 ACD_normal = (C - A).cross(D - A);
    Vector3 ADB_normal = (D - A).cross(B - A);

    // Check if origin is outside of a face
    if (ABC_normal.dot(AO) > 0) {
      simplex = { C, B, A };
      direction = ABC_normal;
      return false;
    }
    if (ACD_normal.dot(AO) > 0) {
      simplex = { D, C, A };
      direction = ACD_normal;
      return false;
    }
    if (ADB_normal.dot(AO) > 0) {
      simplex = { B, D, A };
      direction = ADB_normal;
      return false;
    }

    // Origin is on tithedron
    return true;
  }

  return false;
}

bool
PlatformPhysics::GJK(const OBBox& obb1, const OBBox& obb2, Vector<Vector3>& simplex)
{
  Vector3 dir = Vector3{ 1, 0, 0 };
  simplex.clear();
  simplex.push_back(supportCSO(obb1, obb2, dir));
  dir = -simplex[0];

  const int maxIterations = 20;
  for (int iter = 0; iter < maxIterations; ++iter) {
    Vector3 p = supportCSO(obb1, obb2, dir);
    if (p.dot(dir) < 0) {
      return false; // No collision
    }
    simplex.push_back(p);
    if (doSimplex(simplex, dir)) {
      return true; // Collison
    }
  }
  return false;
}

bool
PlatformPhysics::EPA(const OBBox& obb1,
                     const OBBox& obb2,
                     Vector<Vector3>& simplex,
                     CollisionInfo& info)
{
  Vector<Face> faces;

  faces.push_back(Face(simplex[0], simplex[1], simplex[2]));
  faces.push_back(Face(simplex[0], simplex[3], simplex[1]));
  faces.push_back(Face(simplex[0], simplex[2], simplex[3]));
  faces.push_back(Face(simplex[1], simplex[1], simplex[2]));

  const int maxEPAIterations = 64;
  for (int i = 0; i < maxEPAIterations; ++i) {
    // Look for closest face to origin
    int closest = -1;
    float minDist = MAX_FLOAT;
    for (int j = 0; j < faces.size(); ++j) {
      float dist = Math::abs(faces[j].normal.dot(faces[j].a));
      if (dist < minDist) {
        minDist = dist;
        closest = j;
      }
    }

    Face& f = faces[closest];
    Vector3 p = supportCSO(obb1, obb2, f.normal);
    float d = f.normal.dot(p);

    if (d - minDist < Math::SMALL_NUMBER) {
      info.penetrationDepth = d;
      info.normal = f.normal;
      info.contactPoint1 = p - f.normal * 0.5f * d;
      return true;
    }
  }

  return false; // No collision found within max iterations
}
}
