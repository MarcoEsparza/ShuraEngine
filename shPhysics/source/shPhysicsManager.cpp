/*************************************************************/
/*
*  @file    shPhysicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Physics manager class for handling physics simulation.
*
*  Physics manager class for handling physics simulation.
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPhysicsManager.h"
#include "shPlatformPhysics.h"
#include "shMath.h"
#include "shOBBox.h"
#include "shSphere.h"
#include "shCapsule.h"
#include "shTime.h"


// Temporary include for sorting
using std::sort;

namespace shEngineSDK {
void
PhysicsManager::onUpdate()
{
  
}

Vector<Pair<Rigidbody*, Rigidbody*>>
PhysicsManager::sweepAndPrune(Vector<Rigidbody*>& rigidbodies)
{
  sort(rigidbodies.begin(), rigidbodies.end(),
     [](const Rigidbody* a, const Rigidbody* b) {
       return a->m_position.x < b->m_position.x;
     });

  Vector<Pair<Rigidbody*, Rigidbody*>> possiblePairs;

  for (size_t i = 0; i < rigidbodies.size(); ++i) {
    Rigidbody* rb1 = rigidbodies[i];
    for (size_t j = i + 1; j < rigidbodies.size(); ++j) {
      Rigidbody* rb2 = rigidbodies[j];
      // Check if the bounding boxes overlap on the x-axis
      if (rb1->m_position.x + rb1->m_linearVelocity.x >
          rb2->m_position.x - rb2->m_linearVelocity.x &&
          rb1->m_position.x - rb1->m_linearVelocity.x <
          rb2->m_position.x + rb2->m_linearVelocity.x) {
        possiblePairs.push_back({rb1, rb2});
      }
    }
  }

  return possiblePairs;
}

Matrix3
PhysicsManager::getInertiaTensor(OBBox& box, float mass) const
{
  Matrix3 result = Matrix3::ZEROMATRIX;
  result.m[0][0] = (1.0f / 12.0f) * mass *
                   (box.extent.y * box.extent.y + box.extent.z * box.extent.z);
  result.m[1][1] = (1.0f / 12.0f) * mass *
                   (box.extent.x * box.extent.x + box.extent.z * box.extent.z);
  result.m[2][2] = (1.0f / 12.0f) * mass *
                   (box.extent.x * box.extent.x + box.extent.y * box.extent.y);

  return box.rotation * result * box.rotation.conjugate();
}

Matrix3
PhysicsManager::getInertiaTensor(Sphere& sphere, float mass) const
{
  Matrix3 result = Matrix3::ZEROMATRIX;
  result.m[0][0] = (2.0f / 5.0f) * mass * sphere.radius * sphere.radius;
  result.m[1][1] = (2.0f / 5.0f) * mass * sphere.radius * sphere.radius;
  result.m[2][2] = (2.0f / 5.0f) * mass * sphere.radius * sphere.radius;

  return result;
}

Matrix3 PhysicsManager::getInertiaTensor(Capsule& capsule, float mass) const
{
  // For a capsule, we can approximate the inertia tensor as a cylinder
  // with two hemispherical ends.
  Matrix3 result = Matrix3::ZEROMATRIX;
  float r = capsule.radius;
  float h = capsule.height;

  float perpendicularInertia = (1.0f / 12.0f) * mass * (3.0f * r * r + h * h);
  float inertiaAxis = 0.5f * mass * r * r;

  // Create the inertia tensor aligned to Y-axis
  result.m[0][0] = perpendicularInertia; // X-axis inertia
  result.m[1][1] = inertiaAxis; // Y-axis inertia
  result.m[2][2] = perpendicularInertia; // Z-axis inertia

  // Rotate the inertia tensor to match the capsule's orientation
  Quaternion rotation(Vector3::UP, capsule.center);
  Matrix3 rotatedTensor = rotation.toMatrix3();
  Matrix3 inertiaWorld = rotatedTensor * result * rotatedTensor.transpose();

  return inertiaWorld;
}

Matrix3
PhysicsManager::computeInertiaTensor(Collider& collider, float mass) const
{
  switch (collider.m_type) {
  case COLLIDER_TYPE::kOBBox:
  {
    return getInertiaTensor(collider.m_box, mass);
  }
  case COLLIDER_TYPE::kSphere:
  {
    return getInertiaTensor(collider.m_sphere, mass);
  }
  case COLLIDER_TYPE::kCapsule:
  {
    return getInertiaTensor(collider.m_capsule, mass);
  }
  default:
    break;
  }

  return Matrix3::ZEROMATRIX;
}

float
PhysicsManager::getElasticity(Rigidbody* rb1, Rigidbody* rb2)
{
  return rb1->m_elasticity + rb2->m_elasticity;
}

float
PhysicsManager::getEffectiveMass(Vector3& direction,
                                 Rigidbody* rb1,
                                 Rigidbody* rb2,
                                 Vector3& contact1,
                                 Vector3& contact2)
{
  Vector3 angularEffect = rb1->getInvInertiaWorld() *
                          (contact1.cross(direction)).cross(contact1) +
                          rb2->getInvInertiaWorld() *
                          (contact2.cross(direction)).cross(contact2);

  return rb1->m_invMass + rb2->m_invMass + angularEffect.dot(direction);
}

void
PhysicsManager::resolveCollision(Rigidbody* rb1, Rigidbody* rb2, CollisionInfo& info)
{
  // Restitution calculation
  Vector3 r1 = info.contactPoint1 - rb1->getWorldPosition();
  Vector3 r2 = info.contactPoint2 - rb2->getWorldPosition();

  Vector3 relativeVelocity = rb1->m_linearVelocity +
                             rb1->m_angularVelocity.cross(info.contactPoint1) -
                             rb2->m_linearVelocity -
                             rb2->m_angularVelocity.cross(info.contactPoint2);

  float alongNormal = relativeVelocity.dot(info.normal);
  if (alongNormal > 0.0f) {
    // The bodies are separating, no need to resolve
    return;
  }

  float impulseScalar = -(1.0f + getElasticity(rb1, rb2)) * alongNormal;
  impulseScalar /= getEffectiveMass(info.normal,
                                    rb1,
                                    rb2,
                                    info.contactPoint1,
                                    info.contactPoint2);

  Vector3 impulse = info.normal * impulseScalar;

  rb1->applyImpulse(impulse, info.contactPoint1);
  rb2->applyImpulse(-impulse, info.contactPoint2);

  // Recalculate velocities after impulse
  relativeVelocity = rb1->m_linearVelocity +
                     rb1->m_angularVelocity.cross(info.contactPoint1) -
                     rb2->m_linearVelocity -
                     rb2->m_angularVelocity.cross(info.contactPoint2);
  alongNormal = relativeVelocity.dot(info.normal);

  // Friction calculation (Coulomb's law)
  Vector3 tangent = relativeVelocity - (info.normal * alongNormal);
  float tangentMagnitude = Math::pow(tangent.mag(), 2.0f);
  if (tangentMagnitude <= Math::SMALL_NUMBER) {
    return; // No friction if the tangent is negligible
  }

  Vector3 tangentDir = tangent / Math::sqrt(tangentMagnitude);

  float impulseDeal = relativeVelocity.dot(tangentDir);
  impulseDeal /= getEffectiveMass(tangentDir,
                                  rb1,
                                  rb2,
                                  info.contactPoint1,
                                  info.contactPoint2);

  float maxFriction = getElasticity(rb1, rb2) * impulseScalar;
  float impulseTangent = Math::clamp(impulseDeal, -maxFriction, maxFriction);

  Vector3 frictionTangent = tangentDir * impulseTangent;

  rb1->applyImpulse(frictionTangent, info.contactPoint1);
  rb2->applyImpulse(-frictionTangent, info.contactPoint2);

  // Positional correction (split projection)
  if(info.penetrationDepth <= 0.0f) {
    return; // No penetration, no need to correct
  }

  float effectiveMass = getEffectiveMass(info.normal,
                                         rb1,
                                         rb2,
                                         info.contactPoint1,
                                         info.contactPoint2);
  float biasImpulse = info.penetrationDepth / effectiveMass;

  float penetration = Math::max(0.0f, info.penetrationDepth - PlatformPhysics::SLOP);
  Vector3 correction = info.normal * ((penetration / effectiveMass) * PlatformPhysics::BETA);

  rb1->applyPositinalImpulse(-correction, info.contactPoint1);
  rb2->applyPositinalImpulse(correction, info.contactPoint2);
}

void
PhysicsManager::eulerLinearIntegration(Rigidbody& rbdy, const float speed)
{
  Time& time = g_time();
  float deltaTime = time.getFrameDeltaTime();

  Vector3 gravityForce(0.0f,
                       rbdy.m_mass * -PlatformPhysics::GRAVITY * rbdy.m_gravityScale,
                       0.0f);

  rbdy.m_linearVelocity += rbdy.m_accel * speed * deltaTime;
  rbdy.m_linearVelocity += gravityForce; // Apply gravity force
  rbdy.m_linearVelocity = rbdy.m_linearVelocity * Math::pow(rbdy.m_dragCoefficent, deltaTime);

  rbdy.m_position += rbdy.m_linearVelocity * deltaTime; // Update position
}

void
PhysicsManager::eulerAngularIntegration(Rigidbody& rbdy, const Quaternion& parentRotation)
{
  Time& time = g_time();
  float deltaTime = time.getFrameDeltaTime();

  // Compute angular velocity
  rbdy.m_angularVelocity += rbdy.m_angularAccel * deltaTime;

  // Compute delta rotation
  Quaternion deltaRotation = Quaternion(rbdy.m_angularVelocity, 0.0f);

  // Apply the delta rotation to the rigidbody's rotation
  Quaternion newRotation = (deltaRotation * 0.5f) * parentRotation;
  newRotation = newRotation * deltaTime; // Scale by delta time
  newRotation.normalize(); // Ensure the quaternion is normalized to avoid drift
  rbdy.m_rotation = newRotation;
}

void
PhysicsManager::verletLinearIntegration(Rigidbody& rbdy, const float speed)
{
  Time& time = g_time();
  float deltaTime = time.FIXED_DELTA_TIME;

  Vector3 gravityForce(0.0f,
                       rbdy.m_mass * -PlatformPhysics::GRAVITY * rbdy.m_gravityScale,
                       0.0f);

  rbdy.m_linearVelocity = (rbdy.m_position - rbdy.m_prevPosition) *
                          speed * deltaTime * rbdy.m_dragCoefficent;
  rbdy.m_prevPosition = rbdy.m_position; // Store previous position
  rbdy.m_accel += gravityForce; // Apply gravity force
  rbdy.m_position += rbdy.m_linearVelocity * rbdy.m_accel; // Update position
}

void
PhysicsManager::verletAngularIntegration(Rigidbody& rbdy,
                                         const Quaternion& parentRotation,
                                         const Vector3* nextAngularAccel)
{
  Time& time = g_time();
  float halfDeltaTime = time.FIXED_DELTA_TIME * 0.5f;

  // Compute angular velocity
  Vector3 angularMidPoint = rbdy.m_angularVelocity + rbdy.m_angularAccel * halfDeltaTime;

  // Compute delta rotation
  Quaternion deltaRotation = Quaternion::fromBivector(angularMidPoint * halfDeltaTime);

  // Apply the delta rotation to the rigidbody's rotation
  Quaternion newRotation = deltaRotation * parentRotation;
  newRotation.normalize(); // Ensure the quaternion is normalized to avoid drift

  rbdy.m_rotation = newRotation;

  if (nextAngularAccel) {
    rbdy.m_angularVelocity = angularMidPoint + (*nextAngularAccel * halfDeltaTime);
  }
}

SH_PHYSICS_EXPORT PhysicsManager&
g_physicsMan()
{
  return PhysicsManager::instance();
}
}
