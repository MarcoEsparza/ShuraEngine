/*****************************************************************************/
/*
*  @file    shRigidbody.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief   Rigidbody class for physics simulation.
*
*  Rigidbody class for physics simulation.
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
#include "shVector3.h"
#include "shMatrix3.h"
//#include "shGameObject.h"
#include "shQuaternion.h"
#include "shOBBox.h"
#include "shCollider.h"

namespace shEngineSDK {
//class GameObject;

/**
*  @brief Integration type.
*/
namespace INTEGRATION {
enum E
{
  kEuler = 0,
  kVerlet
};
}

/**
*  @brief Rigidbody class for physics simulation.
*/
class SH_PHYSICS_EXPORT Rigidbody
{
 public:
  /**
  *  @brief Default constructor for the rigidbody.
  */
  Rigidbody() = default;

  /**
  *  @brief Default destructor for the rigidbody.
  */
  ~Rigidbody() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
public:

  Matrix3
  getInvInertiaWorld();

  Vector3
  getWorldPosition() const;

  void
  applyImpulse(const Vector3& impulse, const Vector3& point);

  void
  applyPositinalImpulse(const Vector3& impulse, const Vector3& point);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:

  /**
  *  @brief Integration type for the rigidbody (Euler or Verlet).
  */
  INTEGRATION::E m_integrationType = INTEGRATION::kEuler;

  /**
  *  @brief Linear velocity of the rigidbody in world space.
  */
  Vector3 m_linearVelocity = Vector3::ZERO;

  /**
  *  @brief Angular velocity of the rigidbody in world space.
  */
  Vector3 m_angularVelocity = Vector3::ZERO;

  /**
  *  @brief Previous position of the rigidbody in world space.
  */
  Vector3 m_prevPosition = Vector3::ZERO;

  /**
  *  @brief Position of the rigidbody in world space.
  */
  Vector3 m_position = Vector3::ZERO;

  /**
  *  @brief Linear acceleration of the rigidbody.
  */
  Vector3 m_accel = Vector3::ZERO;

  /**
  *  @brief Angular acceleration of the rigidbody
  */
  Vector3 m_angularAccel = Vector3::ZERO;

  /**
  *  @brief Scale of gravity applied to the rigidbody
  */
  float m_gravityScale = 1.0f;

  /**
  *  @brief Mass of the rigidbody
  */
  float m_mass = 1.0f;

  float m_invMass = 1.0f;

  /**
  *  @brief Coefficient of restitution
  */
  float m_elasticity = 0.5f;

  /**
  *  @brief Friction coefficient for the rigidbody
  */
  float m_friction = 0.5f;

  float m_dragCoefficent = 1.0f;

  Matrix3 m_invAngularInertia = Matrix3::IDENTITY;

  //SPtr<GameObject> m_owner;

  Quaternion m_rotation = Quaternion::IDENTITY;

  COLLIDER_TYPE::E m_colliderType = COLLIDER_TYPE::kNone;
};
}
