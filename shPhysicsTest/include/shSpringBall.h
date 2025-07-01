/*************************************************************/
/*
*  @file    shSpringBall.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Spring ball for hookes law simulation.
*
*  Spring ball for hookes law simulation.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shPrerequisitesPhysics.h"
#include "shPhysicsTetsUsage.h"
#include "shMatrix4.h"
#include "shVector2.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class Sprite;
class ConstantBuffer;

/**
*  @brief Class for hookes law ball.
*/
class SpringBall
{
 public:
  /**
  *  @brief Default constructor.
  */
  SpringBall() = default;

  /**
  *  @brief Constructor to initialize ball data.
  * 
  *  @param SPtr<Sprite>& spr
  *  @param Vector2& pos
  *  @param Vector2& vel
  *  @param Vector2& accel
  *  @param float radius
  *  @param float springC
  *  @param float gravity
  *  @param float limit
  */
  SpringBall(const SPtr<Sprite>& spr,
             const Vector2& pos,
             const Vector2& vel,
             const Vector2& accel,
             const float radius,
             const float springC,
             const float gravity,
             const float limit,
             const float elasticity,
             const float drag);

  /**
  *  @brief Default constructor.
  */
  ~SpringBall() = default;

  /**
  *  @brief Function to update ball.
  * 
  *  @param Vector2& anchor
  *  @param INTEGRATION::E integration
  */
  void
  update(const Vector2& anchor, const INTEGRATION::E integration);

  /**
  *  @brief Physics simulation with euler integration.
  * 
  *  @param Vector2& anchor
  */
  void
  simulateEuler(const Vector2& anchor);

  /**
  *  @brief Physics simulation with verlet integration.
  * 
  *  @param Vector2& anchor
  */
  void
  simulateVerlet(const Vector2& anchor);

  /**
  *  @brief Update the constant buffer.
  */
  void
  updateCBuffer();

 public:
  /**
  *  @brief Ball sprite.
  */
  SPtr<Sprite> m_sprite;

  /**
  *  @brief Ball transform constant buffer.
  */
  SPtr<ConstantBuffer> m_ballBuffer;

  /**
  *  @brief Ball transform.
  */
  Matrix4 m_transform = Matrix4::IDENTITY;

  /**
  *  @brief Is ball grabbed?
  */
  bool m_bGrabbed = false;

  /**
  *  @brief Ball position.
  */
  Vector2 m_position = { 0.0f, 0.0f };

  /**
  *  @brief Ball previous position.
  */
  Vector2 m_previousPosition = { 0.0f, 0.0f };

  /**
  *  @brief Velocity for physics simulation.
  */
  Vector2 m_velocity = { 0.0f, 0.0f };

  /**
  *  @brief Acceleration for physics simulation.
  */
  Vector2 m_accel = { 0.0f, 0.0f };

  /**
  *  @brief Ball radius.
  */
  float m_radius = 0.0f;

  /**
  *  @brief Spring constant for physics simulation.
  */
  float m_springC = 0.0f;

  /**
  *  @brief Gravity for physics simulation.
  */
  float m_gravity = 0.0f;

  /**
  *  @brief Limit distance to the pivot.
  */
  float m_limit = 0.0f;

  float m_elasticity = 0.0f;

  float m_drag = 0.0f;

  float m_mass = 1.0f;

  float m_iniLenght = 50.0f;
  float m_maxLenght = 200.0f;
  float m_minLenght = 5.0f;
};
}
