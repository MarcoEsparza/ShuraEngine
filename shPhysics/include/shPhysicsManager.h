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

struct SH_PHYSICS_EXPORT ConvexMesh
{
  Vector<Vector3> vertices;
  Vector<int32> indices;
};

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

  /*void
  setRigidbodies(const Vector<Rigidbody*>& rigidbodies);*/

  /**
  *  @brief Update function for the physics manager.
  */
  void
  onUpdate(Vector<Rigidbody*>& rigidbodies);

  /**
  *  @brief Sweep and Prune algorithm for broad-phase collision detection.
  * 
  *  @param rigidbodies Vector of pointers to rigidbodies to check for collisions.
  * 
  *  @return Vector of pairs of rigidbodies that are potentially colliding.
  */
  Vector<Pair<Rigidbody*, Rigidbody*>>
  sweepAndPrune(Vector<Rigidbody*>& rigidbodies);

  /**
  *  @brief Get the inertia tensor for a OBBox and mass.
  *
  *  @param OBBox box: The OBBox for which to calculate the inertia tensor.
  *  @param float mass: The mass of the OBBox.
  *
  *  @return Matrix3: The inertia tensor of the OBBox.
  */
  Matrix3
  getInertiaTensor(OBBox& box, float mass) const;

  /**
  *  @brief Get the inertia tensor for a Sphere and mass.
  *
  *  @param Sphere sphere: The Sphere for which to calculate the inertia tensor.
  *  @param float mass: The mass of the OBBox.
  *
  *  @return Matrix3: The inertia tensor of the Sphere.
  */
  Matrix3
  getInertiaTensor(Sphere& sphere, float mass) const;

  /**
  *  @brief Get the inertia tensor for a Capsule and mass.
  *
  *  @param Capsule capsule: The Capsule for which to calculate the inertia tensor.
  *  @param float mass: The mass of the OBBox.
  *
  *  @return Matrix3: The inertia tensor of the Capsule.
  */
  Matrix3
  getInertiaTensor(Capsule& capsule, float mass) const;

  /**
  *  @brief Get the inertia tensor for a given collider and mass.
  *
  *  @param Collider& collider: The collider for which to calculate the inertia tensor.
  *  @param float mass: The mass of the OBBox.
  *
  *  @return Matrix3: The inertia tensor of the Collider.
  */
  Matrix3
  computeInertiaTensor(Collider& collider, float mass) const;

  /**
  *  @brief Get the elasticity coefficient for a collision between two rigidbodies.
  *
  *  @param Rigidbody* rb1: The first rigidbody involved in the collision.
  *  @param Rigidbody* rb2: The second rigidbody involved in the collision.
  *
  *  @return float: The elasticity coefficient for the collision.
  */
  float
  getElasticity(Rigidbody* rb1, Rigidbody* rb2);

  /**
  *  @brief Get the effective mass for a collision between two rigidbodies.
  *
  *  @param Vector3& direction: The direction of the collision.
  *  @param Rigidbody* rb1: The first rigidbody involved in the collision.
  *  @param Rigidbody* rb2: The second rigidbody involved in the collision.
  *  @param Vector3& contact1: The contact point on the first rigidbody.
  *  @param Vector3& contact2: The contact point on the second rigidbody.
  *
  *  @return float: The effective mass for the collision.
  */
  float
  getEffectiveMass(Vector3& direction,
                   Rigidbody* rb1,
                   Rigidbody* rb2,
                   Vector3& contact1, 
                   Vector3& contact2);

  /**
  *  @brief Resolve a collision between two rigidbodies.
  *
  *  @param Rigidbody* rb1: The first rigidbody involved in the collision.
  *  @param Rigidbody* rb2: The second rigidbody involved in the collision.
  *  @param CollisionInfo& info: The collision information containing details
  *                              about the collision.
  */
  void
  resolveCollision(Rigidbody* rb1, 
                   Rigidbody* rb2, 
                   CollisionInfo& info);

  void
  subexpressions(float w0, float w1, float w2,
                float& f1, float& f2, float& f3, float& g0, float& g1, float& g2);

  void
  computeMassProperties(const ConvexMesh& mesh, float mass, Vector3& cm, Matrix3& inertia);

  /**
  *  @brief Integrate the linear motion of a Rigidbody using Euler integration.
  *
  *  @param Rigidbody& rbdy: The Rigidbody to integrate.
  *  @param float speed: The speed of the Rigidbody.
  */
  void
  eulerLinearIntegration(Rigidbody& rbdy, const float speed);

  /**
  *  @brief Integrate the angular motion of a Rigidbody using Euler integration.
  *
  *  @param Rigidbody& rbdy: The Rigidbody to integrate.
  *  @param Quaternion& parentRotation: The rotation of the parent object.
  */
  void
  eulerAngularIntegration(Rigidbody& rbdy, const Quaternion& parentRotation);

  /**
  *  @brief Integrate the linear motion of a Rigidbody using Verlet integration.
  *
  *  @param Rigidbody& rbdy: The Rigidbody to integrate.
  *  @param float speed: The speed of the Rigidbody.
  */
  void
  verletLinearIntegration(Rigidbody& rbdy, const float speed);

  /**
  *  @brief Integrate the angular motion of a Rigidbody using Verlet integration.
  *
  *  @param Rigidbody& rbdy: The Rigidbody to integrate.
  *  @param Quaternion& parentRotation: The rotation of the parent object.
  *  @param nextAngularAccel: Optional next angular acceleration to use for integration.
  */
  void
  verletAngularIntegration(Rigidbody& rbdy,
                           const Quaternion& parentRotation,
                           const Vector3* nextAngularAccel = nullptr);

 private:
  //Vector<Rigidbody*> m_rigidbodies; ///< List of all rigidbodies in the physics manager.
};

/**
*  @brief Easier way to access the PhysicsManager module.
*/
SH_PHYSICS_EXPORT PhysicsManager&
g_physicsMan();
}
