/*************************************************************/
/*
*  @file    shPhysicsApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
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
#include "shBaseApp.h"
#include "Player.h"
#include "shCamera.h"
#include "shPass.h"
#include "shGameObject.h"
#include "shBoxAAB.h"

namespace shEngineSDK {
class ProgramShader;
class Texture2D;
class InputLayout;
class SamplerState;
class BlendState;
class RasterizerState;
class ConstantBuffer;

struct VP
{
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
};

struct Box
{
  Vector2 min;
  Vector2 max;
};

class PhysicsApp : public BaseApp
{
 public:
  PhysicsApp(const ScreenDesc& desc,
             const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
             const SampleDesc& sample = SampleDesc(1, 1))
             : BaseApp(desc, dllGAPI, sample),
               m_desc(desc) {}

  ~PhysicsApp() = default;

 protected:
  void
  onCreate() override;

  void
  onUpdate() override;

  void
  onFixedUpdate() override;

  void
  onRender() override;

  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  void
  onKeyReleased(const KEY::E key, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a mouse button is pressed.
  *
  *  @param MOUSE_INPUT::E mouseButton
  *  @param ModifierState modifier
  */
  void
  onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a mouse button is released.
  *
  *  @param MOUSE_INPUT::E mouseButton
  *  @param ModifierState modifier
  */
  void
  onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  void
  onMouseMove(const MouseMoveData& mouse) override;

  void
  onMouseWheel(const double delta, const ModifierState modifier) override;

  void
  onMouseHWheel(const double delta, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity on app destroy
  */
  void
  onDestroy() override;

  bool
  checkCollision(const Box& box, Vector2& collisionNormal);

  void
  checkBallCollision(const SPtr<Ball>& ball);

  void
  playerBounce(Vector2& collisionNormal);

  void
  initGraphicAssets();

  void
  initCamera();

  void
  spawnBall();

 private:
  ScreenDesc m_desc;
  UPtr<Pass> m_pPhysicsShader;

  Vector<SPtr<Texture2D>> m_targets;

  SPtr<ConstantBuffer> m_pVP;
  SPtr<ConstantBuffer> m_pBase;
  SPtr<ConstantBuffer> m_pTurret;

  Matrix4 m_baseTransform = Matrix4::IDENTITY;
  Matrix4 m_turretTransform = Matrix4::IDENTITY;

  SPtr<Player> m_player;
  Vector<SPtr<Ball>> m_activeBalls;

  SPtr<Sprite> m_pSpriteBase;
  SPtr<Sprite> m_pSpriteCannon;
  SPtr<Sprite> m_pSpriteBall;

  Camera m_camera;

  Vector2 m_mousePosition;
  float m_delta;
  float m_hdelta;
  bool m_bLeftClick;

  bool m_bShot = false;
  bool m_bRotLeft = false;
  bool m_bRotRight = false;
  float m_rotSpeed = 0.0f;

  float m_rotAccumulator = 0.0f;

  INTEGRATION::E m_integration = INTEGRATION::kEuler;
};
}
