/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   App for render testing.
*
*  App for render testing.
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
#include <shPrerequisitesUtilities.h>
#include <shBaseApp.h>
#include <shCamera.h>
#include <shMatrix4.h>
#include <shVector3.h>
#include <shVector2.h>
#include <shPass.h>
#include "shGUI.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/

class RenderTargetView;
class InputLayout;
class SamplerState;
class RasterizerState;
class BlendState;
class DepthStencilState;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class SceneGraph;
class PBRMaterial;
class GameObject;
class Texture2D;
class Sound;
class Material;

/**
*  @brief App for render testing.
*/
class RendererApp : public BaseApp
{
 public:
  /**
  *  @brief Default constructor.
  */
  RendererApp() = default;

  /**
  *  @brief Constructor that initialize the app.
  * 
  *  @param ScreenDesc& desc
  *  @param SampleDesc& sample = SampleDesc(1, 1)
  *  @param GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11
  */
  RendererApp(const ScreenDesc& desc,
              const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
              const SampleDesc& sample = SampleDesc(1, 1))
    : BaseApp(desc, dllGAPI, sample)
  {}

  /**
  *  @brief Default destructor.
  */
  ~RendererApp() = default;

  /***************************************************************************/
  /*
  *  Event functions
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Override function to add functionallity on app creation.
  */
  void
  onCreate() override;

  /**
  *  @brief Override function to add functionallity when app updates.
  */
  void
  onUpdate() override;
  
  void
  onFixedUpdate() override;

  /**
  *  @brief Override function to add functionallity on app render.
  */
  void
  onRender() override;

  /**
  *  @brief Override to add functionality when this event pops.
  */
  virtual void
  onResize(const ResizeData& rszData) override;

  /**
  *  @brief Override function to add functionallity when a key is pressed.
  * 
  *  @param KEY::E key
  *  @param ModifierState modifier
  */
  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a key is released.
  *
  *  @param KEY::E key
  *  @param ModifierState modifier
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
  *  @brief Override function to add functionallity when a mouse move.
  *
  *  @param MouseMoveData& mouse
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

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 private:

  /**
  *  @brief Rotate the camera.
  */
  void
  rotateCamera();

  /**
  *  @brief Updates the main buffer data.
  */
  void
  updateMainBuffer();

  void
  tempLoad();

  void
  loadMods();

  void
  registerCodecs();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /*************************************/
  /*              Camera               */
  /*************************************/

  /**
  *  @brief App camera.
  */
  Camera m_camera;

  /*************************************/
  /*              Inputs               */
  /*************************************/

  /**
  *  @brief Is left mouse button clicked?
  */
  bool m_bLeftClick = false;

  /**
  *  @brief Is right mouse button clicked?
  */
  bool m_bRightClick = false;

  /**
  *  @brief Last mouse position.
  */
  Vector2 m_lastMousePos = Vector2(0.0f, 0.0f);

  /**
  *  @brief Current mouse position.
  */
  Vector2 m_currentMousePos = Vector2(0.0f, 0.0f);

  /**
  *  @brief Is camera going foward?
  */
  bool m_bFoward = false;

  /**
  *  @brief Is camera going left?
  */
  bool m_bLeft = false;

  /**
  *  @brief Is camera going back?
  */
  bool m_bBack = false;

  /**
  *  @brief Is camera going right?
  */
  bool m_bRight = false;

  /**
  *  @brief Is camera going up?
  */
  bool m_bUp = false;

  /**
  *  @brief Is camera going down?
  */
  bool m_bDown = false;

  /**
  *  @brief Is model rotating right?
  */
  bool m_bRotRight = false;

  /**
  *  @brief Is model rotating left?
  */
  bool m_bRotLeft = false;

  /**
  *  @brief Is model rotating up?
  */
  bool m_bRotUp = false;

  /**
  *  @brief Is model rotating down?
  */
  bool m_bRotDown = false;

  /**
  *  @brief Mouse wheel delta.
  */
  float m_delta = 0.0f;

  /**
  *  @brief Mouse horizontal wheel delta.
  */
  float m_hdelta = 0.0f;

  /*************************************/
  /*           Imgui shaders           */
  /*************************************/
  /**
  *  @brief Size of shadow texture.
  */
  float m_shadowTexSize = 0;

  /**
  *  @brief
  */
  float m_fpsTimer = 0.0f;

  /**
  *  @brief
  */
  uint32 m_fpsCount = 0;

  /*************************************/
  /*               Other               */
  /*************************************/

  /**
  *  @brief Screen size.
  */
  Vector2 m_screenSize = Vector2(0.0f, 0.0f);

  Vector2 m_sceneSize = Vector2(0.0f, 0.0f);

  /**
  *  @brief Testing audio.
  */
  //SPtr<Sound> m_testSound;

  /**
  *  @brief Is audio playing?
  */
  bool bIsSoundPlaying = false;

  GUI m_gui;

  bool m_bPlayScene = false;

  Vector<uint32> m_loadedModIds;

  //Vector<SPtr<GameObject>> m_tempGameObjects;
};
}
