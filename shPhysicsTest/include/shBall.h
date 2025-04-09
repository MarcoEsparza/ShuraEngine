/*****************************************************************************/
/*
*  @file    shBall.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Ball class.
*
*  Ball class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shPrerequisitesPhysics.h"
#include "shPhysicsTestUsage.h"
#include "shGraphicTypes.h"
#include "shVector2.h"
#include "shMatrix4.h"
#include "shSphere.h"
#include "shPath.h"
#include "shSprite.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class Texture2D;

class Ball
{
 public:
  /**
  *  @brief Default constructor.
  */
  Ball() = default;

  /**
  *  @brief Default destructor.
  */
  ~Ball() = default;

  /**
  *  @brief Set ball sprite.
  * 
  *  @param SPtr<Sprite>& sprite
  */
  void
  setSprite(const SPtr<Sprite>& sprite);

  /**
  *  @brief Update ball depending on the used integration.
  * 
  *  @param INTEGRATION::E integration
  */
  void
  update(const INTEGRATION::E integration);

  /**
  *  @brief Simulate euler integration.
  */
  void
  simulateEuler();

  /**
  *  @brief Simulate verlet integration.
  */
  void
  simulateVerlet();

 public:
   /**
   *  @brief Ball sprite.
   */
  SPtr<Sprite> m_sprite;

  /**
  *  @brief Ball transform.
  */
  Matrix4 m_transform = Matrix4::IDENTITY;

  /**
  *  @brief Constant buffer for transform.
  */
  SPtr<ConstantBuffer> m_buffer;

  /**
  *  @brief Velocity for physics simulation.
  */
  Vector2 m_velocity = { 0.0f, 0.0f };

  /**
  *  @brief Ball position.
  */
  Vector2 m_position = { 0.0f, 0.0f };

  /**
  *  @brief Acceleration for physics simulation.
  */
  Vector2 m_accel = { 0.0f, 0.0f };

  /**
  *  @brief Speed for euler integration.
  */
  float m_eulerSpeed = 0.0f;
  /**
  *  @brief Gravity for euler integration.
  */
  float m_eulerGravity = 0.0f;

  /**
  *  @brief Speed for verlet integration.
  */
  float m_verletSpeed = 0.0f;

  /**
  *  @brief Gravity for verlet integration.
  */
  float m_verletGravity = 0.0f;

  /**
  *  @brief Mass for physics simulation.
  */
  float m_mass = 0.0f;

  /**
  *  @brief Drag constant for physics simulation.
  */
  float m_dragC = 0.0f;

  /**
  *  @brief Ball radius.
  */
  float m_radius = 0.0f;

  /**
  *  @brief Ball previous position.
  */
  Vector2 m_previousPosition = { 0.0f, 0.0f };

  /**
  *  @brief Delta time.
  */
  float m_time = 0.0f;

  /**
  *  @brief Destroy ball?
  */
  bool m_bDestroy = false;

  /**
  *  @brief Lifetime.
  */
  float m_lifeSpan = 0.0f;
};
}
