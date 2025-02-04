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

namespace shEngineSDK {
class ProgramShader;
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

  bool
  checkCollision(const Box& box, Vector2& collisionNormal);

  void
  playerBounce(Vector2& collisionNormal);

  void
  initGraphicAssets();

  void
  initCamera();

  void
  initCollisionBoxes();

 private:
  ScreenDesc m_desc;
  UPtr<Pass> m_pPhysicsShader;

  SPtr<ConstantBuffer> m_pVP;
  SPtr<ConstantBuffer> m_pBase;
  SPtr<ConstantBuffer> m_pTurret;

  SPtr<Player> m_player;

  UPtr<Sprite> m_pSpriteBase;
  UPtr<Sprite> m_pSpriteCannon;

  Camera m_camera;

  Box m_left;
  Box m_right;
  Box m_top;
  Box m_bottom;

  bool m_bUp = false;
  bool m_bDown = false;
  bool m_bRight = false;
  bool m_bLeft = false;
};
}
