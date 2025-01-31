/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
*  @brief   Base app for engine.
*
*  Base app for engine.
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
#include "shException.h"
#include "shScreen.h"
#include "shGraphicTypes.h"
#include "shVector2i.h"
#include "shLinearColor.h"

namespace shEngineSDK {
/**
*  @brief Base app for engine.
*/
class SH_CORE_EXPORT BaseApp
{
 public:
  /**
  *  @brief Constructor to initialize screen.
  * 
  *  @param ScreenDesc& desc
  *  @param GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11
  *  @param SampleDesc& sample = SampleDesc(1, 1)
  */
  BaseApp(const ScreenDesc& desc,
          const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
          const SampleDesc& sample = SampleDesc(1, 1))
          : m_screenDesc(desc),
            m_graphicAPI(dllGAPI),
            m_sample(sample),
            m_mousePos(Vector2i(0, 0)),
            m_lastMousePos(Vector2i(0, 0)),
            m_backgroundColor(LinearColor(0.0f, 0.0f, 0.0f)) {}

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

  /**
  *  @brief Sets the background color.
  * 
  *  @param LinearColor& color
  */
  FORCEINLINE void
  setBackgroundColor(const LinearColor& color);

  /*************************************************************/
  /*
  *  Messages
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Override to add functionality when app creates. This will only be
  *         called once.
  */
  virtual void
  onCreate() {}

  /**
  *  @brief Override to add functionality every frame.
  */
  virtual void
  onUpdate() {}

  /**
  *  @brief Override to update objects independant from frame rate.
  */
  virtual void
  onFixedUpdate() {}

  /**
  *  @brief Override to do the app render pipeline.
  */
  virtual void
  onRender() {}

  /**
  *  @brief Override to do something when window close. This will only be
  *         called once.
  */
  virtual void
  onClose() {}

  /**
  *  @brief Override to do something when window destroys. This will only be
  *         called once.
  */
  virtual void
  onDestroy() {}

  /*************************************************************/
  /*
  *  Events
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Override to set mouse move event. This will be called every time
  *         the mouse move.
  */
  virtual void
  onMouseMove(const MouseMoveData& mouse) { SH_UNREFERENCED_PARAMETER(mouse); }

  /**
  *  @brief Override to set mouse button event. This will be called every time
  *         a mouse button is pressed.
  */
  virtual void
  onMouseButtonPressed(const MOUSE_INPUT::E mouseButton, const ModifierState modifier)
  {
    SH_UNREFERENCED_PARAMETER(mouseButton);
    SH_UNREFERENCED_PARAMETER(modifier);
  }

  /**
  *  @brief Override to set mouse button event. This will be called every time
  *         a mouse button is released.
  */
  virtual void
  onMouseButtonReleased(const MOUSE_INPUT::E mouseButton, const ModifierState modifier)
  {
    SH_UNREFERENCED_PARAMETER(mouseButton);
    SH_UNREFERENCED_PARAMETER(modifier);
  }

  /**
  *  @brief Override to set mouse raw event.
  */
  virtual void
  onMouseRaw(const MouseRawData& mouseRaw) { SH_UNREFERENCED_PARAMETER(mouseRaw); }

  /**
  *  @brief Override to set mouse wheel event. This will be called every time
  *         the mouse wheel is used.
  */
  virtual void
  onMouseWheel(const double delta, const ModifierState modifier)
  {
    SH_UNREFERENCED_PARAMETER(delta);
    SH_UNREFERENCED_PARAMETER(modifier);
  }

  /**
  *  @brief Override to set keyboard button event. This will be called every
  *         time a keyboard button is pressed.
  */
  virtual void
  onKeyPressed(const KEY::E key, const ModifierState modifier)
  {
    SH_UNREFERENCED_PARAMETER(key);
    SH_UNREFERENCED_PARAMETER(modifier);
  }

  /**
  *  @brief Override to set keyboard button event. This will be called every
  *         time a keyboard button is released.
  */
  virtual void
  onKeyReleased(const KEY::E key, const ModifierState modifier)
  {
    SH_UNREFERENCED_PARAMETER(key);
    SH_UNREFERENCED_PARAMETER(modifier);
  }

  /*************************************************************/
  /*
  *  Internal functions
  */
  /*************************************************************/
 private:
  /**
  *  @brief Creates window with the given descriptor.
  */
  void
  createWindow();

  /**
  *  @brief Load the selected graphic api.
  */
  void
  loadGraphicAPI();

  /**
  *  @brief Initialize all managers singletons.
  */
  void
  initManagers();

  /**
  *  @brief Handle all events on app.
  * 
  *  @param Event& wndEvent
  */
  void
  handleScreenEvents(const Event& wndEvent);

  /**
  *  @brief Update all objects.
  */
  void
  update();

  /**
  *  @brief Update objects independant from frame rate.
  */
  void
  fixedUpdate();

  /**
  *  @brief Main render function.
  */
  void
  render();

  /**
  *  @brief Destroys all manager singletons.
  */
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

  /**
  *  @brief Graphic sample descriptor.
  */
  SampleDesc m_sample;

  /**
  *  @brief Event queue.
  */
  SPtr<ScreenEventHandle> m_eventQueue;

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

  /**
  *  @brief Selected graphic api.
  */
  GRAPHIC_API::E m_graphicAPI;

  /**
  *  @brief Background color.
  */
  LinearColor m_backgroundColor;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE void
BaseApp::setBackgroundColor(const LinearColor& color)
{
  m_backgroundColor = color;
}
}
