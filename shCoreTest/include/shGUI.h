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

  /*************************************/
  /*          Imgui transform          */
  /*************************************/

  /**
  *  @brief Model game object.
  */
  SPtr<GameObject> m_pModel;

  /**
  *  @brief Model position.
  */
  //Vector3 m_modelPos = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model rotation.
  */
  //Vector3 m_modelRot = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model scale.
  */
  //Vector3 m_modelScale = { 0.0f, 0.0f, 0.0f };

  bool m_bTexColor = false;
  int32 m_selectedMat = -1;
};

FORCEINLINE void
GUI::setScreenSize(const Vector2& size)
{
  m_screenSize = size;
}
}
