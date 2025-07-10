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
#include "shTransformComponent.h"

namespace shEngineSDK {
Matrix3
Rigidbody::getInvInertiaWorld()
{
  Matrix3 rotationMat = m_rotation.toMatrix3();
  return rotationMat * m_invAngularInertia * rotationMat.transpose();
}

void
Rigidbody::applyImpulse(const Vector3& impulse, const Vector3& point)
{
  m_linearVelocity += impulse * m_invMass;
  m_angularVelocity += getInvInertiaWorld() * point.cross(impulse);
}

Matrix3
Rigidbody::getInertiaTensor(OBBox box) const
{
  Matrix3 result = Matrix3::ZEROMATRIX;
  result.m[0][0] = (1.0f / 12.0f) * m_mass * (box.extent.y * box.extent.y + box.extent.z * box.extent.z);
  result.m[1][1] = (1.0f / 12.0f) * m_mass * (box.extent.x * box.extent.x + box.extent.z * box.extent.z);
  result.m[2][2] = (1.0f / 12.0f) * m_mass * (box.extent.x * box.extent.x + box.extent.y * box.extent.y);

  return box.rotation * result * box.rotation.conjugate();
}
}
