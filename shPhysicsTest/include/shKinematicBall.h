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

class KinematicBall{
 public:
  KinematicBall() = default;
  ~KinematicBall() = default;

  void
  foward();

  void
  inverseFabrik();

  void
  update();

 public:
  SPtr<KinematicBall> child;

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
};
}
