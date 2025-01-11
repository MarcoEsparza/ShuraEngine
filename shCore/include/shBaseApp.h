/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/09
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
  *  @brief Constructor to initialize screen.
  * 
  *  @param ScreenDesc& desc
     @param String& dllGraphicApiName
     @param SampleDesc& sample = SampleDesc(1, 1)
  */
  BaseApp(const ScreenDesc& desc,
          const String& dllGraphicApiName,
          const SampleDesc& sample = SampleDesc(1, 1));

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
  */
  void
  run();

  /*************************************************************/
  /*
  *  Messages
  */
  /*************************************************************/

  virtual void
  onCreate();

  virtual void
  onUpdate(float deltaTime);

  virtual void
  onRender();

  virtual void
  onDestroy();

  /*************************************************************/
  /*
  *  Events
  */
  /*************************************************************/
  /**
  *  @brief 
  */
  virtual void
  onMouseMove(const MouseMoveData& mouse);

  virtual void
  onMouseClick();

  virtual void
  onKeyboardPressed();

  /*************************************************************/
  /*
  *  Internal functions
  */
  /*************************************************************/
 private:
  /**
  *  @brief Creates window with the descriptor.
  */
  void
  createWindow();

  void
  loadGraphicAPI();

  /**
  *  @brief Initialize all managers singletons.
  */
  void
  initManagers();

  /**
  *  @brief Handle all events.
  */
  void
  handleScreenEvents();

  /**
  *  @brief Update all objects.
  */
  void
  update(float deltaTime);

  /**
  *  @brief Render pipeline.
  */
  void
  render();

  void
  destroyManagers();

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

  SampleDesc m_sample;

  /**
  *  @brief Event queue.
  */
  SPtr<ScreenEventHandle> m_eventQueue;

  Event m_currentEvent;

  /**
  *  @brief Last mouse position.
  */
  Vector2i m_lastMousePos;

  /**
  *  @brief Current mouse position.
  */
  Vector2i m_mousePos;

  /**
  *  @brief Screen Descriptor
  */
  ScreenDesc m_screenDesc;

  String m_gphApiName;
};
}
