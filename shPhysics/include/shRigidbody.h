/*************************************************************/
/*
*  @file    shRigidbody.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/30
*  @brief
*
*
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
#include "shVector3.h"

namespace shEngineSDK {
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

class SH_PHYSICS_EXPORT Rigidbody
{
 public:
  Rigidbody() = default;
  ~Rigidbody() = default;

  INTEGRATION::E m_integrationType;
  Vector3 m_velocity;
  Vector3 m_prevPosition;
  Vector3 m_position;
  Vector3 m_accel;
  float gravityScale;
  float mass;
};
}
