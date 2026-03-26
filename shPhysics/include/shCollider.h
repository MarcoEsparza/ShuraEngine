/*************************************************************/
/*
*  @file    shCollider.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
*  @brief   Collider class for physics simulation.
*
*  Collider class for physics simulation.
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
#include <shAABBox.h>
#include <shOBBox.h>
#include <shCapsule.h>
#include <shSphere.h>

namespace shEngineSDK {
using Shape = std::variant<Sphere, Capsule, OBBox>;

/**
*  @brief Collider type enumeration.
*/
namespace COLLIDER_TYPE {
enum E
{
  kNone = 0,
  kSphere,
  kCapsule,
  kAABBox,
  kOBBox,
  kConvexMesh
};
}

/**
*  @brief Collider class for physics simulation.
*/
class SH_PHYSICS_EXPORT Collider
{
public:
  /**
  *  @brief Default constructor for the collider.
  */
  Collider() = default;

  /**
  *  @brief Default destructor for the collider.
  */
  virtual ~Collider() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Collider type.
  */
  COLLIDER_TYPE::E m_type = COLLIDER_TYPE::kNone;

  /**
  *  @brief Collider.
  */
  Shape m_shape;
};
}
