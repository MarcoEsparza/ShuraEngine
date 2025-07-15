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
  case COLLIDER_TYPE::kConvexMesh:
  {
    break;
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
  //float biasImpulse = info.penetrationDepth / effectiveMass;

  float penetration = Math::max(0.0f, info.penetrationDepth - PlatformPhysics::SLOP);
  Vector3 correction = info.normal * ((penetration / effectiveMass) * PlatformPhysics::BETA);

  rb1->applyPositinalImpulse(-correction, info.contactPoint1);
  rb2->applyPositinalImpulse(correction, info.contactPoint2);
}

void
PhysicsManager::subexpressions(float w0, float w1, float w2,
                              float& f1, float& f2, float& f3,
                              float& g0, float& g1, float& g2)
{
  float temp0 = w0 * w1;
  f1 = temp0 + w2;
  float temp1 = w0 * w0;
  float temp2 = temp1 + w1 * temp0;
  f2 = temp2 + w2 * f1;
  f3 = w0 * temp1 + w1 * temp2 + w2 * f2;
  g0 = f2 + w0 * (f1 + w0);
  g1 = f2 + w1 * (f1 + w1);
  g2 = f2 + w2 * (f1 + w2);
}

void
PhysicsManager::computeMassProperties(const ConvexMesh& mesh,
                                      float mass,
                                      Vector3& cm,
                                      Matrix3& inertia)
{
  const float mult[10] = { 1.0f / 6.0f,
                           1.0f / 24.0f, 1.0f / 24.0f, 1.0f / 24.0f,
                           1.0f / 60.0f, 1.0f / 60.0f, 1.0f / 60.0f,
                           1.0f / 120.0f, 1.0f / 120.0f, 1.0f / 120.0f };

  float intg[10] = {};

  for (uint32 t = 0; t < mesh.indices.size(); ++t) {
    const Vector3& p0 = mesh.vertices[mesh.indices[t]];
    const Vector3& p1 = mesh.vertices[mesh.indices[t + 1]];
    const Vector3& p2 = mesh.vertices[mesh.indices[t + 2]];

    float a1 = p1.x - p0.x, b1 = p1.y - p0.y, c1 = p1.z - p0.z;
    float a2 = p2.x - p0.x, b2 = p2.y - p0.y, c2 = p2.z - p0.z;

    float d0 = b1 * c2 - b2 * c1;
    float d1 = a2 * c1 - a1 * c2;
    float d2 = a1 * b2 - a2 * b1;

    float f1x, f2x, f3x, g0x, g1x, g2x;
    float f1y, f2y, f3y, g0y, g1y, g2y;
    float f1z, f2z, f3z, g0z, g1z, g2z;

    subexpressions(p0.x, p1.x, p2.x, f1x, f2x, f3x, g0x, g1x, g2x);
    subexpressions(p0.y, p1.y, p2.y, f1y, f2y, f3y, g0y, g1y, g2y);
    subexpressions(p0.z, p1.z, p2.z, f1z, f2z, f3z, g0z, g1z, g2z);

    intg[0] += d0 * f1x;
    intg[1] += d0 * f2x;
    intg[2] += d1 * f2y;
    intg[3] += d2 * f2z;
    intg[4] += d0 * f3x;
    intg[5] += d1 * f3y;
    intg[6] += d2 * f3z;
    intg[7] += d0 * (p0.y * g0x + p1.y * g1x + p2.y * g2x);
    intg[8] += d1 * (p0.z * g0y + p1.z * g1y + p2.z * g2y);
    intg[9] += d2 * (p0.x * g0z + p1.x * g1z + p2.x * g2z);
  }

  for (uint32 i = 0; i < 10; ++i) {
    intg[i] *= mult[i];
  }

  float volume = intg[0];
  float density = mass / volume;

  cm.x = intg[1] / volume;
  cm.y = intg[2] / volume;
  cm.z = intg[3] / volume;

  inertia.m[0][0] = density * (intg[5] + intg[6] - volume * (cm.y * cm.y + cm.z * cm.z));
  inertia.m[1][1] = density * (intg[4] + intg[6] - volume * (cm.z * cm.z + cm.x * cm.x));
  inertia.m[2][2] = density * (intg[4] + intg[5] - volume * (cm.x * cm.x + cm.y * cm.y));

  inertia.m[0][1] = inertia.m[1][0] = -density * (intg[7] - volume * cm.x * cm.y);
  inertia.m[1][2] = inertia.m[2][1] = -density * (intg[8] - volume * cm.y * cm.z);
  inertia.m[0][2] = inertia.m[2][0] = -density * (intg[9] - volume * cm.z * cm.x);
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
