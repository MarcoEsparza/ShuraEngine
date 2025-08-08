/*****************************************************************************/
/*
*  @file    shCollisionInfo.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Basic collision information struct.
*
*  Basic collision information struct.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Basic collision information struct.
*/
struct CollisionInfo
{
  /**
  *  @brief Default constructor for CollisionInfo.
  */
  CollisionInfo() = default;

  /**
  *  @brief Constructor for CollisionInfo with parameters.
  * 
  *  @param penetrationDepth The depth of penetration in the collision.
  *  @param normal The normal vector of the collision surface.
  *  @param contactPoint1 The first contact point of the collision.
  *  @param contactPoint2 The second contact point of the collision.
  */
  CollisionInfo(float penetrationDepth,
                const Vector3& normal,
                const Vector3& contactPoint1,
                const Vector3& contactPoint2)
    : penetrationDepth(penetrationDepth),
      normal(normal),
      contactPoint1(contactPoint1),
      contactPoint2(contactPoint2)
  {}

  /**
  *  @brief Penetration depth of the collision.
  */
  float penetrationDepth = 0.0f;

  /**
  *  @brief Normal vector of the collision surface.
  */
  Vector3 normal = Vector3::ZERO;

  /**
  *  @brief First contact point of the collision.
  */
  Vector3 contactPoint1 = Vector3::ZERO;

  /**
  *  @brief Second contact point of the collision.
  */
  Vector3 contactPoint2 = Vector3::ZERO;
};
}
