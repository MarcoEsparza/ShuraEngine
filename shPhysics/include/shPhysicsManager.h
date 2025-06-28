/*************************************************************/
/*
*  @file    shPhysicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
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

namespace shEngineSDK {
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

  /**
  *  @brief Sweep and Prune algorithm for broad-phase collision detection.
  * 
  *  @param rigidbodies Vector of pointers to rigidbodies to check for collisions.
  * 
  *  @return Vector of pairs of rigidbodies that are potentially colliding.
  */
  Vector<Pair<Rigidbody*, Rigidbody*>>
  sweepAndPrune(const Vector<Rigidbody*>& rigidbodies);
};

/**
*  @brief Easier way to access the PhysicsManager module.
*/
SH_PHYSICS_EXPORT PhysicsManager&
g_physicsMan();
}
