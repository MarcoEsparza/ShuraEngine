/*************************************************************/
/*
*  @file    shKinematicBall.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/08
*  @brief   Class for ik ball.
*
*  Class for ik ball.
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
#include "shPhysicsTestUsage.h"
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

class KinematicBall{
 public:
  /**
  *  @brief Default constructor.
  * 
  *  @param SPtr<Sprite>& sprite
  *  @param Vector2& pos
  *  @param float radius
  */
  KinematicBall(const SPtr<Sprite>& sprite,
                const Vector2& pos,
                const float radius);

  /**
  *  @brief Default destructor.
  */
  ~KinematicBall() = default;

  /**
  *  @brief foward movement.
  * 
  *  @param Vector2& newPos
  */
  void
  foward(const Vector2& newPos);

  /**
  *  @brief Updates the constant buffer.
  */
  void
  update();

  /**
  *  @brief Render sprite.
  */
  void
  drawSprite();

  /**
  *  @brief Adds a child to the node.
  * 
  *  @param KinematicBall& child
  */
  void
  setChild(const KinematicBall& child);

 public:
  /**
  *  @brief Nodes child.
  */
  SPtr<KinematicBall> m_child;

  /**
  *  @brief Ball sprite.
  */
  SPtr<Sprite> m_sprite;

  /**
  *  @brief Ball transform constant buffer.
  */
  SPtr<ConstantBuffer> m_ballBuffer;

  /**
  *  @brief Is ball grabbed?
  */
  bool m_bGrabbed = false;

  /**
  *  @brief Ball position.
  */
  Vector2 m_position = { 0.0f, 0.0f };

  /**
  *  @brief Ball radius.
  */
  float m_radius = 0.0f;

  /**
  *  @brief Lenght to parent.
  */
  float m_lenght = 100.0f;

  /**
  *  @brief Ball transform.
  */
  Matrix4 m_transform;
};
}
