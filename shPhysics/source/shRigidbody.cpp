/*****************************************************************************/
/*
*  @file    shRigidbody.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
*  @brief   Rigidbody class for physics simulation.
*
*  Rigidbody class for physics simulation.
*
* @bug      No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRigidbody.h"
//#include "shTransformComponent.h"

namespace shEngineSDK {
Matrix3
Rigidbody::getInvInertiaWorld()
{
  Matrix3 rotationMat = m_rotation.toMatrix3();
  return rotationMat * m_invAngularInertia * rotationMat.transpose();
}

Vector3
Rigidbody::getWorldPosition() const
{
  if(m_colliderType == COLLIDER_TYPE::kConvexMesh) {
    // For convex mesh, we assume the position is the center of the mesh
    //return m_position + m_convexMesh->getCenterOfMass();
  }
  return m_position;
}

void
Rigidbody::applyImpulse(const Vector3& impulse, const Vector3& point)
{
  if(m_invMass == 0.0f) {
    return; // No impulse applied if mass is zero
  }
  // Calculate the point in world space
  Vector3 worldPoint = point + getWorldPosition();

  // Apply linear impulse
  m_linearVelocity += impulse * m_invMass;
  m_angularVelocity += getInvInertiaWorld() * point.cross(impulse);
}


}
