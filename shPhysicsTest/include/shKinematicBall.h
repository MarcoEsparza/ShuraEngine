/*************************************************************/
/*
*  @file    
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/03
*  @brief   
*
*  
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
  KinematicBall(const SPtr<Sprite>& sprite,
                const Vector2& pos,
                const float radius);
  ~KinematicBall() = default;

  void
  foward(const Vector2& newPos);

  void
  inverseFabrik();

  void
  update();

  void
  drawSprite();

  void
  setChild(const KinematicBall& child);

 public:
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

  float m_lenght = 75.0f;

  Matrix4 m_transform;
};
}
