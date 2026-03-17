/*****************************************************************************/
/*
*  @file    shJointConstraint.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/02
*  @brief   Joint type of constraint.
*
*  Joint type of constraint.
*
* @bug      No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shJointConstraint.h"
#include "shMatrix3.h"

namespace shEngineSDK {
void
JointConstraint::initRelativePositions(const Vector3& rw)
{
  if (m_rigidbodies.size() < 2) {
    return; // Not enough rigidbodies to initialize relative positions
  }
  // Assuming the first two rigidbodies are the ones we want to constrain
  // Relative position for the first rigidbody
  m_r1 = m_rigidbodies[0]->m_rotation.conjugate() *
         (rw - m_rigidbodies[0]->getWorldPosition());

  // Relative position for the second rigidbody
  m_r2 = m_rigidbodies[1]->m_rotation.conjugate() *
         (rw - m_rigidbodies[1]->getWorldPosition());

  const Vector3 p1 = m_rigidbodies[0]->getWorldPosition();
  const Vector3 p2 = m_rigidbodies[1]->getWorldPosition();

  m_errorValue = p1 + (m_rigidbodies[0]->m_rotation * m_r1) -
                    p2 - (m_rigidbodies[1]->m_rotation * m_r2);
}

void
JointConstraint::solve(float deltaTime)
{
  SH_UNREFERENCED_PARAMETER(deltaTime);

  if (m_rigidbodies.size() < 2) {
    return; // Not enough rigidbodies to solve the constraint
  }

  // Get the skew-symmetric matrices for the relative positions
  Matrix3 skew1 = Matrix3::getSkewSymmetric(m_r1);
  Matrix3 skew2 = Matrix3::getSkewSymmetric(m_r2);

  // Calculate the effective mass matrix
  Matrix3 effMass = Matrix3::IDENTITY * m_rigidbodies[0]->m_invMass +
                    Matrix3::IDENTITY * m_rigidbodies[1]->m_invMass +
                    skew1 * m_rigidbodies[0]->getInvInertiaWorld() * skew1.getTranspose() +
                    skew2 * m_rigidbodies[1]->getInvInertiaWorld() * skew2.getTranspose();

  Matrix3 effMassP = effMass.getInverse();

  const Vector3 p1 = m_rigidbodies[0]->getWorldPosition();
  const Vector3 p2 = m_rigidbodies[1]->getWorldPosition();

  Vector3 lambda = -m_errorValue * effMassP;

  m_rigidbodies[0]->applyImpulse(lambda, p1 + (m_rigidbodies[0]->m_rotation * m_r1));
  m_rigidbodies[1]->applyImpulse(-lambda, p2 + (m_rigidbodies[1]->m_rotation * m_r2));
}

float
JointConstraint::getError() const
{
  const Vector3 p1 = m_rigidbodies[0]->getWorldPosition();
  const Vector3 p2 = m_rigidbodies[1]->getWorldPosition();
  m_errorValue = (p1 + (m_rigidbodies[0]->m_rotation * m_r1) -
                  p2 - (m_rigidbodies[1]->m_rotation * m_r2));

  return m_errorValue.lenghtSq();
}
}
