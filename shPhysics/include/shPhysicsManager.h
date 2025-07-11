/*************************************************************/
/*
*  @file    shPhysicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Physics manager class for handling physics simulation.
*
*  Physics manager class for handling physics simulation.
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
#include "shModule.h"
#include "shCollider.h"
#include "shRigidbody.h"
#include "shCollisionInfo.h"

namespace shEngineSDK {
class OBBox;
class Sphere;
class Capsule;
class Quaternion;

/**
*  @brief PhysicsManager class for handling physics simulation.
*/
class SH_PHYSICS_EXPORT PhysicsManager : public Module <PhysicsManager>
{
 public:
  /**
  *  @brief Default constructor for the physics manager.
  */
  PhysicsManager() = default;

  /**
  *  @brief Default destructor for the physics manager.
  */
  ~PhysicsManager() = default;
  
  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:

  void
  onUpdate();

  /**
  *  @brief Sweep and Prune algorithm for broad-phase collision detection.
  * 
  *  @param rigidbodies Vector of pointers to rigidbodies to check for collisions.
  * 
  *  @return Vector of pairs of rigidbodies that are potentially colliding.
  */
  Vector<Pair<Rigidbody*, Rigidbody*>>
  sweepAndPrune(Vector<Rigidbody*>& rigidbodies);

  Matrix3
  getInertiaTensor(OBBox box, float mass) const;

  Matrix3
  getInertiaTensor(Sphere sphere, float mass) const;

  Matrix3
  getInertiaTensor(Capsule capsule, float mass) const;

  Matrix3
  computeInertiaTensor(Collider& collider, float mass);

  float
  getElasticity(Rigidbody* rb1, Rigidbody* rb2);

  float
  getEffectiveMass(Vector3& direction,
                   Rigidbody* rb1,
                   Rigidbody* rb2,
                   Vector3& contact1, 
                   Vector3& contact2);

  void
  resolveCollision(Rigidbody* rb1, 
                   Rigidbody* rb2, 
                   CollisionInfo& info);

  void
  eulerLinearIntegration(Rigidbody& rbdy, const float speed);

  void
  eulerAngularIntegration(Rigidbody& rbdy, const Quaternion& parentRotation);

  void
  verletLinearIntegration(Rigidbody& rbdy, const float speed);

  void
  verletAngularIntegration(Rigidbody& rbdy,
                           const Quaternion& parentRotation,
                           const Vector3* nextAngularAccel = nullptr);
};

/**
*  @brief Easier way to access the PhysicsManager module.
*/
SH_PHYSICS_EXPORT PhysicsManager&
g_physicsMan();
}
