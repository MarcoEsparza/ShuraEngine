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

namespace shEngineSDK {
class Vector3;
class OBBox;
class AABBox;
class Capsule;
class Sphere;

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
};
}
