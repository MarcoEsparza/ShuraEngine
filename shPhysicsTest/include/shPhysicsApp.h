/*****************************************************************************/
/*
*  @file    shPhysicsApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   App for physics simulation.
*
*  App for physics simulation.
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
#include "shBaseApp.h"
#include "shCamera.h"
#include "shPass.h"
#include "shGameObject.h"
#include "shBoxAAB.h"
#include "shBall.h"
#include "shSpringBall.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class ProgramShader;
class Texture2D;
class InputLayout;
class SamplerState;
class BlendState;
class RasterizerState;
class ConstantBuffer;

/**
*  @brief Struct for View Projection.
*/
struct VP
{
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
};

/**
*  @brief Struct to determine the max and min positions of a 2d box.
*/
struct Box
{
  Vector2 min;
  Vector2 max;
};

/**
*  @brief App for physics simulation.
*/
class PhysicsApp : public BaseApp
{
 public:
  /**
  *  @brief Constructor to initialize app.
  * 
  *  @param ScreenDesc& desc,
  *  @param GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
  *  @param SampleDesc& sample = SampleDesc(1, 1))
  */
  PhysicsApp(const ScreenDesc& desc,
             const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
             const SampleDesc& sample = SampleDesc(1, 1))
             : BaseApp(desc, dllGAPI, sample),
               m_desc(desc) {}

  /**
  *  @brief Default destructor.
  */
  ~PhysicsApp() = default;

 protected:
  /**
  *  @brief Override to add functionality when app creates. This will only be
  *         called once.
  */
  void
  onCreate() override;

  /**
  *  @brief Override to add functionality every frame.
  */
  void
  onUpdate() override;

  /**
  *  @brief Override to update objects independant from frame rate.
  */
  void
  onFixedUpdate() override;

  /**
  *  @brief Override to do the app render pipeline.
  */
  void
  onRender() override;

  /**
  *  @brief Override to set keyboard button event. This will be called every
  *         time a keyboard button is pressed.
  */
  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  /**
  *  @brief Override to set keyboard button event. This will be called every
  *         time a keyboard button is released.
  */
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

  /**
  *  @brief Override to set mouse move event. This will be called every time
  *         the mouse move.
  */
  void
  onMouseMove(const MouseMoveData& mouse) override;

  /**
  *  @brief Override to set mouse wheel event. This will be called every time
  *         the mouse wheel is used.
  */
  void
  onMouseWheel(const double delta, const ModifierState modifier) override;

  /**
  *  @brief Override to set mouse wheel event. This will be called every time
  *         the mouse wheel is used.
  */
  void
  onMouseHWheel(const double delta, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity on app destroy
  */
  void
  onDestroy() override;

  /**
  *  @brief Check if a ball made a collision with a border.
  */
  void
  checkBallCollision(const SPtr<Ball>& ball);

  /**
  *  @brief Initialize the app graphic assets.
  */
  void
  initGraphicAssets();

  /**
  *  @brief Initialize the app camera.
  */
  void
  initCamera();

  /**
  *  @brief Spawns balls and adds them to the scene.
  */
  void
  spawnBall();

  void
  initSpringBall();

  bool
  mouseOnObject(const Vector2& min, const Vector2& max);

  void
  dragSpringBall();

  void
  dragPivot();

 private:
  /**
  *  @brief Screen descriptor.
  */
  ScreenDesc m_desc;

  /**
  *  @brief Shader pass.
  */
  UPtr<Pass> m_pPhysicsShader;

  /**
  *  @brief Render targets.
  */
  Vector<SPtr<Texture2D>> m_targets;

  /**
  *  @brief View projection constant buffer.
  */
  SPtr<ConstantBuffer> m_pVP;

  /**
  *  @brief Constant buffer for turret base.
  */
  SPtr<ConstantBuffer> m_pBase;

  /**
  *  @brief Constant buffer for turret.
  */
  SPtr<ConstantBuffer> m_pTurret;

  /**
  *  @brief Turret base transform.
  */
  Matrix4 m_baseTransform = Matrix4::IDENTITY;

  /**
  *  @brief Turret transform.
  */
  Matrix4 m_turretTransform = Matrix4::IDENTITY;

  /**
  *  @brief Vector of spawned balls.
  */
  Vector<SPtr<Ball>> m_activeBalls;

  /**
  *  @brief Turret base sprite.
  */
  SPtr<Sprite> m_pSpriteBase;

  /**
  *  @brief Turret sprite.
  */
  SPtr<Sprite> m_pSpriteCannon;

  /**
  *  @brief Ball sprite.
  */
  SPtr<Sprite> m_pSpriteBall;

  /**
  *  @brief App Camera.
  */
  Camera m_camera;

  /**
  *  @brief Current mouse position.
  */
  Vector2 m_mousePosition = { 0.0f, 0.0f };

  /**
  *  @brief Wheel delta.
  */
  float m_delta = 0.0f;

  /**
  *  @brief Wheel horizontal delta.
  */
  float m_hdelta = 0.0f;

  /**
  *  @brief Mouse left click.
  */
  bool m_bLeftClick = false;

  /**
  *  @brief Bool to check if user want to shot.
  */
  bool m_bShot = false;

  /**
  *  @brief Enable rotation of the turret to the left.
  */
  bool m_bRotLeft = false;

  /**
  *  @brief Enable rotation of the turret to the right.
  */
  bool m_bRotRight = false;

  /**
  *  @brief Rotation speed.
  */
  float m_rotSpeed = 0.0f;

  /**
  *  @brief Rotation accumulator.
  */
  float m_rotAccumulator = 0.0f;

  /**
  *  @brief Which integration is using.
  */
  INTEGRATION::E m_integration = INTEGRATION::kEuler;

  /**
  *  @brief Index for integration selection.
  */
  int32 m_intIndex = 0;

  /**
  *  @brief Strings to use integration on gui.
  */
  Vector<String> m_intList;

  SPtr<Sprite> m_pSbSprite;
  SPtr<SpringBall> m_springBall;
  Vector2 m_pivotPos;
  bool m_bPivotGrabbed = false;

  float m_springC = 0.0f;
  float m_dragC = 0.0f;
  float m_mass = 0.0f;
  float m_gravity = 0.0f;
  float m_iniLenght = 0.0f;
  float m_maxLenght = 0.0f;
  float m_minLenght = 0.0f;
};
}
