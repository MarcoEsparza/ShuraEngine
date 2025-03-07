/*****************************************************************************/
/*
*  @file    shPlayer.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Player and other classes for physics simulation.
*
*  Player and other classes for physics simulation.
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

/**
*  @brief Player class for controlling physics simulation.
*/
class Player
{
 public:
  /**
  *  @brief Defaulr constructor.
  */
  Player() = default;

  /**
  *  @brief Constructor to initialize player.
  * 
  *  @param Vector2& min
  *  @param Vector2& max
  *  @param Path& filePath
  *  @param Vector2& position
  *  @param float mass
  *  @param float speed
  *  @param float dragC
  *  @param float radius
  */
  Player(const Vector2& min,
         const Vector2& max,
         const Path& filePath,
         const Vector2& position,
         const float mass,
         const float speed,
         const float dragC,
         const float radius);

  /**
  *  @brief Default destructor.
  */
  ~Player() = default;

  /**
  *  @brief Set player info.
  * 
  *  @param Vector2& min
  *  @param Vector2& max
  *  @param Path& filePath
  *  @param Vector2& position
  *  @param float mass
  *  @param float speed
  *  @param float dragC
  *  @param float radius
  */
  void
  setPlayer(const Vector2& min,
            const Vector2& max,
            const Path& filePath,
            const Vector2& position,
            const float mass,
            const float speed,
            const float dragC,
            const float radius);

  /**
  *  @brief Move player.
  * 
  *  @param Vector2& direction
  */
  void
  move(const Vector2& direction);

  /**
  *  @brief Calculate dragforce.
  */
  Vector2
  calculateDrag();

  /**
  *  @brief Simulate drag with euler integration.
  * 
  *  @param Vector2& dForce
  *  @param float dt
  */
  void
  eulerDrag(const Vector2& dForce, const float dt);

  /**
  *  @brief Simulate drag with verlet integration.
  *
  *  @param Vector2& dForce
  *  @param float dt
  */
  void
  verletDrag(const Vector2& dForce, const float dt);

  /**
  *  @brief Update player.
  * 
  *  @param flaot deltaTime
  */
  void
  update(float deltaTime);

  /**
  *  @brief Update model transform constant buffer.
  */
  void
  updateModelBuffer();

 public:
  /**
  *  @brief Player sprite.
  */
  SPtr<Sprite> m_sprite;

  /**
  *  @brief Velocity for physics simulation.
  */
  Vector2 m_velocity;

  /**
  *  @brief Player position.
  */
  Vector2 m_position;

  /**
  *  @brief Previous position for verlet integration.
  */
  Vector2 m_previousPosition = Vector2(0.0f, 0.0f);

  /**
  *  @brief Mass for physics simulation.
  */
  float m_mass;

  /**
  *  @brief Speed for physics simulation.
  */
  float m_speed;

  /**
  *  @brief Drag constant for physics simulation.
  */
  float m_dragC;

  /**
  *  @brief PLayer radius.
  */
  float m_radius;

  /**
  *  @brief Player transform.
  */
  Matrix4 m_transform = Matrix4::IDENTITY;

  /**
  *  @brief Constant buffer for player transform.
  */
  SPtr<ConstantBuffer> m_modelBuffer;
};
}
