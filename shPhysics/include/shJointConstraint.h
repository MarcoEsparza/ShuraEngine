/*****************************************************************************/
/*
*  @file    shJointConstraint.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/02
*  @brief   Joint type of constraint.
*
*  Joint type of constraint.
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
#include "shBasePhysicsConstraint.h"
#include <shVector3.h>

namespace shEngineSDK {
class JointConstraint : public BasePhysicsConstraint
{
 public:
  /**
   *  @brief Default constructor for the joint constraint.
   */
  JointConstraint() = default;
  /**
   *  @brief Constructor for the joint constraint with rigidbodies.
   *
   *  @param const Vector<SPtr<Rigidbody>>& rigidbodies: A vector of shared
   *               pointers to the rigidbodies.
   */
  JointConstraint(const Vector<SPtr<Rigidbody>>& rigidbodies, const Vector3& rw)
    : BasePhysicsConstraint(rigidbodies)
  {
    initRelativePositions(rw);
  }

  /**
   *  @brief Default destructor for the joint constraint.
   */
  virtual ~JointConstraint() = default;

 private:
  /**
  *  @brief Initialize the relative positions of the rigidbodies.
  * 
  *  @param const Vector3& rw: The world position to calculate the relative positions from.
  */
  void
  initRelativePositions(const Vector3& rw);

 public:
  /**
  *  @brief Solve function to be called during the physics solver.
  *
  *  @param float deltaTime: The time step for the physics simulation.
  */
  void
  solve(float deltaTime) override;

  /**
  *  @brief Calculate the error of the constraint.
  *
  *  @return float: The error value of the constraint.
  */
  float
  getError() const override;

 private:
  /**
  *  @brief Relative position of the first rigidbody
  */
  Vector3 m_r1;

  /**
  *  @brief Relative position of the second rigidbody
  */
  Vector3 m_r2;

  /**
  *  @brief Constant error value for the constraint
  */
  mutable Vector3 m_errorValue;
};
}
