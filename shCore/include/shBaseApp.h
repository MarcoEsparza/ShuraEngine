/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Base app for engine.
*
*  Base app for engine.
*
*  @bug     Crash on render.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"
#include "shGraphicTypes.h"
#include "shGraphicsManager.h"
#include "shMaterial.h"
#include "shSceneGraph.h"
#include "shCamera.h"
#include "shVector2i.h"

namespace shEngineSDK {
/**
*  @brief Axis enum.
*/
namespace AXIS {
enum E
{
  kX = 0,
  kY,
  kZ
};
}

/**
*  @brief Base app for engine.
*/
class SH_CORE_EXPORT BaseApp
{
 public:
  /**
  *  @brief Default constructor.
  */
  BaseApp() = default;

  /**
  *  @brief Default destructor.
  */
  ~BaseApp() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Run app.
  * 
  *  @param ScreenDesc& desc : Descriptor to create window.
  *  @param String& dllGraphicApiName : graphic dll to use.
  */
  void
  run(const ScreenDesc& desc, const String& dllGraphicApiName);

  /**
  *  @brief Update the Static Mesh vertex and index buffer.
  */
  void
  updateSMBuffers();

  /**
  *  @brief This function is only for testing and it will be deleted
  */
  void
  moveChest(const Vector3& newPos);

 private:
  /**
  *  @brief Handle all events.
  */
  void
  handleEvents();

  /**
  *  @brief Update all objects.
  */
  void
  update(const float time);

  /**
  *  @brief Render pipeline.
  */
  void
  render();

  /**
  *  @brief Draw the static meshes in scene.
  */
  void
  drawStaticMeshesInScene();

  /**
  *  @brief Draw the skeletal meshes in scene.
  */
  void
  drawSkeletalMeshesInScene();

  /**
  *  @brief Moves the editor camera with given dirextion and axis.
  * 
  *  @param float direction
  *  @param AXIS::E axis
  */
  void
  moveCameraPosition(const float direction, const AXIS::E axis);

  /**
  *  @brief Rotates the camera
  */
  void
  rotateCamera();

  /**
  *  @brief Initialize the assets that will be used.
  * 
  *  @note The resource loading in this function is only temporary while
  *        an appropiate editor is created.
  */
  void
  initGraphicAssets();

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief Main app window.
  */
  SPtr<Screen> m_mainScreen;

  /**
  *  @brief Shader for static meshes.
  */
  SPtr<ProgramShader> m_pStaticShader;

  /**
  *  @brief Shader for skeletal meshes.
  */
  SPtr<ProgramShader> m_pSkeletalShader;

  /**
  *  @brief Input layout for static meshes.
  */
  SPtr<InputLayout> m_pStaticInputLayout;

  /**
  *  @brief Input layout for skeletal meshes.
  */
  SPtr<InputLayout> m_pSkeletalInputLayout;

  /**
  *  @brief Static Mesh Vertex Buffer.
  */
  SPtr<VertexBuffer> m_staticVBuffer;

  /**
  *  @brief Static Mesh Index Buffer.
  */
  SPtr<IndexBuffer> m_staticIBuffer;

  /**
  *  @brief Sampler state.
  */
  SPtr<SamplerState> m_pSamplerLinear;

  /**
  *  @brief Camera constant buffer.
  */
  SPtr<ConstantBuffer> m_pWVP;

  /**
  *  @brief Event queue.
  */
  SPtr<ScreenEventHandle> m_eventQueue;

  /**
  *  @brief Is app running?
  */
  bool m_appRunning = false;

  /**
  *  @brief Camera for the editor.
  */
  FPSCamera m_editorCamera;

  /**
  *  @brief World matrix.
  */
  Matrix4 m_world;

  /**
  *  @brief Current scene.
  */
  SceneGraph m_scene;

  /**
  *  @brief Last mouse position.
  */
  Vector2i m_lastMousePos;

  /**
  *  @brief Current mouse position.
  */
  Vector2i m_mousePos;
};
}
