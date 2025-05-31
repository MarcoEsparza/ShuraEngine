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
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlatformPhysics.h"
#include "shVector3.h"
#include "shOBBox.h"
#include "shAABBox.h"
#include "shCapsule.h"
#include "shSphere.h"

namespace shEngineSDK {
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
}
