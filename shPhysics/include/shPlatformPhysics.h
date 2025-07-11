/*************************************************************/
/*
*  @file    shPlatformPhysics.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/30
*  @brief   
*
*  
*
* @bug      No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesPhysics.h"
#include "shVector3.h"

namespace shEngineSDK {
class OBBox;
class AABBox;
class Capsule;
class Sphere;
struct CollisionInfo;

/**
*  @breif Face structure
*/
struct Face
{
  Face(const Vector3& _a, const Vector3& _b, const Vector3& _c);
  
  Vector3 a = Vector3::ZERO;
  Vector3 b = Vector3::ZERO;
  Vector3 c = Vector3::ZERO;
  Vector3 normal = Vector3::ZERO;
  float distance = 0.0f;
};

class SH_PHYSICS_EXPORT PlatformPhysics
{
 public:
  PlatformPhysics() = default;
  
  ~PlatformPhysics() = default;

  static Vector3
  supportPointOBB(const OBBox& obb, const Vector3& dir);

  static Vector3
  supportPointAABBox(const AABBox& aabb, const Vector3& dir);

  static Vector3
  supportPointCapsule(const Capsule& capsule, const Vector3& dir);

  static Vector3
  supportPointSphere(const Sphere& sphere, const Vector3& dir);

  static Vector3
  supportCSO(const OBBox& obb1, const OBBox& obb2, const Vector3& dir);

  static Vector3
  supportCSO(const OBBox& obb, const AABBox& aabb, const Vector3& dir);

  static Vector3
  supportCSO(const OBBox& obb, const Capsule& capsule, const Vector3& dir);

  static Vector3
  supportCSO(const OBBox& obb, const Sphere& sphere, const Vector3& dir);

  static Vector3
  supportCSO(const AABBox& aabb1, const AABBox& aabb2, const Vector3& dir);

  static Vector3
  supportCSO(const AABBox& aabb, const Capsule& capsule, const Vector3& dir);

  static Vector3
  supportCSO(const AABBox& aabb, const Sphere& sphere, const Vector3& dir);

  static Vector3
  supportCSO(const Capsule& capsule1, const Capsule& capsule2, const Vector3& dir);

  static Vector3
  supportCSO(const Capsule& capsule, const Sphere& sphere, const Vector3& dir);

  static Vector3
  supportCSO(const Sphere& sphere1, const Sphere& sphere2, const Vector3& dir);

  bool
  doSimplex(Vector<Vector3>& simplex, Vector3& direction);

  bool
  GJK(const OBBox& obb1, const OBBox& obb2, Vector<Vector3>& simplex);

  bool
  EPA(const OBBox& obb1, const OBBox& obb2, Vector<Vector3>& simplex, CollisionInfo& info);

  static const float SLOP;
  static const float BETA;
};
}
