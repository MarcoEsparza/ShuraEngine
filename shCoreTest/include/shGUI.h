/*************************************************************/
/*
*  @file    shGUI.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Graphical User Interface (GUI) system for editor.
*
*  Graphical User Interface (GUI) system for editor.
*
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include <shVector2.h>
#include <shVector3.h>
#include <shVector4.h>

namespace shEngineSDK {
class Screen;
class GameObject;
class TransformComponent;
class StaticMeshComponent;
class SkyBoxComponent;
class ColliderComponent;
class RigidbodyComponent;
class Material;

/**
*  @brief Graphical User Interface (GUI) system for editor.
*/
class GUI
{
 public:
  /**
  *  @brief Default constructor.
  */
  GUI() = default;

  /**
  *  @brief Default destructor.
  */
  ~GUI() = default;

  /**
  *  @brief Initialize the GUI system.
  */
  void
  init(const WPtr<Screen> pScreen);

  /**
  *  @brief Shutdown the GUI system.
  */
  void
  shutdown();

  /**
  *  @brief Render the GUI.
  */
  void
  render();

  /**
  *  @brief Update the GUI.
  */
  void
  update();

  void
  setDockSpace();

  void
  setRendererSettings();

  void
  setSceneGraph();

  void
  setComponentInspector();

  void
  showTransformComponent();

  void
  showStaticMeshComponent(const WPtr<StaticMeshComponent> wpSMesh);

  void
  showMaterialInspector(const WPtr<Material> wpMat);

  void
  showSkyBoxComponent(const WPtr<SkyBoxComponent> wpSkyBox);

  void
  showLightComponent();

  void
  showCameraComponent();

  void
  showColliderComponent(const WPtr<ColliderComponent> wpCollider);

  void
  showRigidbodyComponent(const WPtr<RigidbodyComponent> wpRigidbody);

  FORCEINLINE void
  setScreenSize(const Vector2& size);

 private:
  Vector2 m_screenSize;
  int32 m_sceneIndex = -1;

  /**
  *  @brief Selected game object on scenegraph.
  */
  SPtr<GameObject> m_pActiveGameObject;

  bool m_bTexColor = false;
  int32 m_selectedMat = -1;

 public:
  /**
  *  @brief
  */
  uint32 m_fpsCountGUI = 0;

  float m_camSpeed = 0.0f;
  float m_camFov = 0.0f;

  /**
  *  @brief Light position.
  */
  Vector4 m_lightPos = { 0.0f, 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Light camera target.
  */
  Vector3 m_lightTarget = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Light camera near.
  */
  float m_lcamNear = 0.0f;

  /**
  *  @brief Light camera far.
  */
  float m_lcamFar = 0.0f;

  /**
  *  @brief Light camera size.
  */
  float m_lcamSize = 0.0f;

  /**
  *  @brief Size of shadow texture.
  */
  float m_shadowTexSize = 0;

  bool m_bPlay = false;
};

FORCEINLINE void
GUI::setScreenSize(const Vector2& size)
{
  m_screenSize = size;
}
}
