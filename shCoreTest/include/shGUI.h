/*****************************************************************************/
/*
*  @file    shGUI.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/10/02
*  @brief   Graphical User Interface (GUI) system for editor.
*
*  Graphical User Interface (GUI) system for editor.
*
*  @bug     No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
#include <shVector2.h>
#include <shVector3.h>
#include <shVector4.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class Screen;
class GameObject;
class TransformComponent;
class StaticMeshComponent;
class SkyBoxComponent;
class ColliderComponent;
class RigidbodyComponent;
class LightComponent;
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

  /***************************************************************************/
  /*
  *  Public Functions
  */
  /***************************************************************************/
 public:
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

  /***************************************************************************/
  /*
  *  Private Functions
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Set the dock space.
  */
  void
  setDockSpace();

  void
  setScene();

  /**
  *  @brief Set the console logs window.
  */
  void
  setConsoleLogs();

  /**
  *  @brief Set the renderer settings window.
  */
  void
  setRendererSettings();

  /**
  *  @brief Recursive function to show the scene graph.
  * 
  *  @param SPtr<GameObject>& pNode
  *  @param SPtr<GameObject>& pSelectedObj
  *  @param int32& matSelection
  */
  void
  showSceneGraph(const SPtr<GameObject>& pNode,
                 SPtr<GameObject>& pSelectedObj,
                 int32& matSelection);

  /**
  *  @brief Set the scene graph window.
  */
  void
  setSceneGraph();

  /**
  *  @brief Set the component inspector window.
  */
  void
  setComponentInspector();

  /**
  *  @brief Show the transform component.
  */
  void
  showTransformComponent();

  /**
  *  @brief Show the static mesh component.
  * 
  *  @param WPtr<StaticMeshComponent> wpSMesh
  */
  void
  showStaticMeshComponent(const WPtr<StaticMeshComponent> wpSMesh);

  /**
  *  @brief Show the material inspector.
  * 
  *  @param WPtr<Material> wpMat
  */
  void
  showMaterialInspector(const WPtr<Material> wpMat);

  /**
  *  @brief Show the skybox component.
  * 
  *  @param WPtr<SkyBoxComponent> wpSkyBox
  */
  void
  showSkyBoxComponent(const WPtr<SkyBoxComponent> wpSkyBox);

  /**
  *  @brief Show the light component.
  * 
  *  @param WPtr<LightComponent> wpLight
  */
  void
  showLightComponent(const WPtr<LightComponent> wpLight);

  /**
  *  @brief Show the camera component.
  */
  void
  showCameraComponent();

  /**
  *  @brief Show the collider component.
  * 
  *  @param WPtr<ColliderComponent> wpCollider
  */
  void
  showColliderComponent(const WPtr<ColliderComponent> wpCollider);

  /**
  *  @brief Show the rigidbody component.
  * 
  *  @param WPtr<RigidbodyComponent> wpRigidbody
  */
  void
  showRigidbodyComponent(const WPtr<RigidbodyComponent> wpRigidbody);

  /**
  *  @brief Add a static mesh component to a game object.
  * 
  *  @param SPtr<GameObject>& pObj
  */
  void
  addStaticMeshComponentToObject(SPtr<GameObject>& pObj);

  /**
  *  @brief Set the screen size.
  * 
  *  @param const Vector2& size
  */
  FORCEINLINE void
  setScreenSize(const Vector2& size);

  /***************************************************************************/
  /*
  *  Private Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Screen size;
  */
  Vector2 m_screenSize = Vector2(0.0f, 0.0f);

  /**
  *  @brief Selected game object on scenegraph.
  */
  SPtr<GameObject> m_pActiveGameObject;

  /**
  *  @brief Game object being renamed.
  */
  SPtr<GameObject> m_pRenamingGameObject;

  /**
  *  @brief Selected material index.
  */
  int32 m_selectedMat = -1;

  /**
  *  @brief Console logs.
  */
  Vector<String> m_logs;

  /**
  *  @brief Is renaming a game object?
  */
  bool m_bRename = false;

  bool m_bOpenDockSpace = false;

  /***************************************************************************/
  /*
  *  Public Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief FPS counter.
  */
  uint32 m_fpsCountGUI = 0;

  /**
  *  @brief Camera speed.
  */
  float m_camSpeed = 0.0f;

  /**
  *  @brief Camera fov.
  */
  float m_camFov = 0.0f;

  /**
  *  @brief Camera near plane.
  */
  float m_camNear = 0.0f;

  /**
  *  @brief Camera far plane.
  */
  float m_camFar = 0.0f;

  /**
  *  @brief Size of shadow texture.
  */
  float m_shadowTexSize = 0;

  bool m_bSSAO = true;

  bool m_bSceneWindowFocused = false;

  Vector2 m_sceneWindowSize = Vector2(0.0f, 0.0f);

  /**
  *  @brief Is scene playing?
  */
  bool m_bPlay = false;
};

FORCEINLINE void
GUI::setScreenSize(const Vector2& size)
{
  m_screenSize = size;
}
}
