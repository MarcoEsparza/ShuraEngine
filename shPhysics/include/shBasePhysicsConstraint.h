/*****************************************************************************/
/*
*  @file    shBasePhysicsConstraint.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/01
*  @brief   Base class for physics constraints.
*
*  Base class for physics constraints.
*
* @bug      No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesPhysics.h"
#include "shRigidbody.h"
#include <shException.h>

namespace shEngineSDK {
/**
*  @brief Base class for physics constraints.
*/
class BasePhysicsConstraint
{
 public:
  /**
  *  @brief Default constructor for the base physics constraint.
  *  This constructor initializes an empty constraint.
  *
  *  @note This constructor is intended to be used by derived classes.
  *        It should not be used directly.
  */
  BasePhysicsConstraint() = default;

  /**
  *  @brief Constructor for the base physics constraint with rigidbodies.
  * 
  *  @param const Vector<SPtr<Rigidbody>>& rigidbodies: A vector of shared
  *               pointers to the rigidbodies.
  */
  BasePhysicsConstraint(const Vector<SPtr<Rigidbody>>& rigidbodies)
    : m_rigidbodies(rigidbodies)
  {}

  /**
  *  @brief Default destructor for the base physics constraint.
  */
  virtual ~BasePhysicsConstraint() = default;

 public:
  /**
  *  @brief Pre-solve function to be called before the physics solver.
  * 
  *  @param float deltaTime: The time step for the physics simulation.
  */
  virtual void
  preSolve(float deltaTime) { SH_UNREFERENCED_PARAMETER(deltaTime); };

  /**
  *  @brief Solve function to be called during the physics solver.
  *
  *  @param float deltaTime: The time step for the physics simulation.
  */
  virtual void
  solve(float deltaTime) { SH_UNREFERENCED_PARAMETER(deltaTime); };

  /**
  *  @brief Post-solve function to be called after the physics solver.
  *
  *  @param float deltaTime: The time step for the physics simulation.
  */
  virtual void
  postSolve(float deltaTime) { SH_UNREFERENCED_PARAMETER(deltaTime); };

  /**
  *  @brief Calculate the error of the constraint.
  * 
  *  @return float: The error value of the constraint.
  */
  virtual float
  getError() const { return 0.0f; };

 protected:
  /**
  *  @brief A vector of shared pointers to the rigidbodies involved in the constraint.
  */
  Vector<SPtr<Rigidbody>> m_rigidbodies;
};
}
