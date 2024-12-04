/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
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

namespace shEngineSDK {
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

  /**
  *  @brief Run app.
  * 
  *  @param ScreenDesc& desc : Descriptor to create window.
  *  @param String& dllStr : graphic dll to use.
  */
  void
  run(const ScreenDesc& desc, const String& dllStr);

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
  update();

  /**
  *  @brief Render pipeline.
  */
  void
  render();

  /**
  *  @brief Initialize the assets that will be used.
  * 
  *  @note The resource loading in this function is only temporary while
  *        an appropiate editor is created.
  */
  void
  initGraphicAssets();

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
};
}
