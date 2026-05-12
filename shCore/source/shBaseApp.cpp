/*************************************************************/
/*
*  @file    shBaseApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/12
*  @brief   Base app for engine.
*
*  Base app for engine.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shBaseApp.h"
#include "shMath.h"

#include "shDynamicLibraryManager.h"
#include "shGraphicsManager.h"
#include "shShaderManager.h"
#include "shRenderManager.h"
#include "shResourceManager.h"
#include "shAudioManager.h"
#include "shScriptManager.h"
#include "shTime.h"
#include "shSceneGraph.h"
#include "shLogger.h"
#include "shFileExplorer.h"
#include "shPhysicsManager.h"
#include "shGizmos.h"

namespace shEngineSDK {
BaseApp::~BaseApp()
{
}

void
BaseApp::run()
{
  // Creates window
  createWindow();

  // Initate system
  initManagers();

  // Send onCreate event
  onCreate();

  float accumulator = 0.0f;

  // Main App Loop
  while (m_mainScreen->isOpen()) {
    accumulator += g_time().getFrameDeltaTime();

    m_eventQueue->update();

    while (!m_eventQueue->empty()) {
      Event wndEvent = m_eventQueue->front();

      if (wndEvent.type == EVENT_TYPE::kClose) {
        onClose();
        m_mainScreen->close();
      }

      // Call the function to handle all events on app
      handleScreenEvents(wndEvent);
      m_eventQueue->pop();
    }
    update();

    if (accumulator >= g_time().FIXED_DELTA_TIME) {
      fixedUpdate();
      accumulator -= g_time().FIXED_DELTA_TIME;
    }

    render();
  }

  onDestroy();
  destroyManagers();
}

void
BaseApp::createWindow()
{
  m_mainScreen = sh_makeShared<Screen>();
  m_eventQueue = sh_makeShared<ScreenEventHandle>();
  m_eventQueue->setParentScreen(m_mainScreen);

  if (!m_mainScreen->init(m_screenDesc, m_eventQueue)) {
    return;
  }
}

void
BaseApp::loadGraphicAPI()
{
  DynamicLibraryManager& dynLibMan = g_dynLibMan();

  // Select dll name
  String apiName = "";
#if SH_DEBUG_MODE == 1
  if (m_graphicAPI == GRAPHIC_API::kDX11) {
    apiName = "shDX11Graphicsd";
  }
  else if (m_graphicAPI == GRAPHIC_API::kOGL) {
    apiName = "shOpenGLGraphicsd";
  }
#else
  if (m_graphicAPI == GRAPHIC_API::kDX11) {
    apiName = "shDX11Graphics";
  }
  else if (m_graphicAPI == GRAPHIC_API::kOGL) {
    apiName = "shOpenGLGraphics";
  }
#endif

  // Load DLL
  dynLibMan.loadDynLibrary(apiName);
}

void
BaseApp::loadAudioAPI()
{
  DynamicLibraryManager& dynLibMan = g_dynLibMan();
#if SH_DEBUG_MODE == 1
  dynLibMan.loadDynLibrary("shFMODAudiod");
#else
  dynLibMan.loadDynLibrary("shFMODAudio");
#endif
}

void
BaseApp::initManagers()
{
  FileExplorer::startUp();
  DynamicLibraryManager::startUp();
  loadGraphicAPI();
  GraphicsManager::instance().initManager(m_mainScreen, false, m_sample);
  ResourceManager::startUp();
  ShaderManager::startUp();
  RenderManager::startUp();
  Gizmos::startUp();
  ScriptManager::startUp();
  Time::startUp();
  SceneGraph::startUp();
  PhysicsManager::startUp();
  Logger::startUp();

  //loadAudioAPI();
}

void
BaseApp::handleScreenEvents(const Event& wndEvent)
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Resize event
  if (wndEvent.type == EVENT_TYPE::kResize) {
    const auto& rszData = wndEvent.data.resize;

    m_mainScreen->setWidth(rszData.width);
    m_mainScreen->setHeight(rszData.height);
    m_screenDesc.width = cast::st<uint32>(m_mainScreen->getClientSize().x);
    m_screenDesc.height = cast::st<uint32>(m_mainScreen->getClientSize().y);

    if (!rszData.resizing) {
      graphMan.updateScreenSize(Vector2(static_cast<float>(m_screenDesc.width),
                                        static_cast<float>(m_screenDesc.height)));

      onResize(rszData);
    }
  }

  // Keyboard events
  if (wndEvent.type == EVENT_TYPE::kKeyboard) {
    const auto& key = wndEvent.data.keyboard;
    if (key.state == BUTTON_STATE::kPressed) {
      onKeyPressed(key.key, key.modifiers);
    }
    else if (key.state == BUTTON_STATE::kReleased) {
      onKeyReleased(key.key, key.modifiers);
    }
  }

  // Mouse move
  if (wndEvent.type == EVENT_TYPE::kMouseMove) {
    const auto& mouse = wndEvent.data.mouseMove;
    onMouseMove(mouse);
  }

  // Mouse input
  if (wndEvent.type == EVENT_TYPE::kMouseInput) {
    const auto& mouseButton = wndEvent.data.mouseInput;
    if (mouseButton.state == BUTTON_STATE::kPressed) {
      onMouseButtonPressed(mouseButton.button, mouseButton.modifiers);
    }
    else if (mouseButton.state == BUTTON_STATE::kReleased) {
      onMouseButtonReleased(mouseButton.button, mouseButton.modifiers);
    }
  }

  // Mouse raw
  if (wndEvent.type == EVENT_TYPE::kMouseRaw) {
    const auto& mouseRaw = wndEvent.data.mouseRaw;
    onMouseRaw(mouseRaw);
  }

  // Mouse wheel
  if (wndEvent.type == EVENT_TYPE::kMouseWheel) {
    const auto& mouseWheel = wndEvent.data.mouseWheel;
    onMouseWheel(mouseWheel.delta, mouseWheel.modifiers);
  }

  if (wndEvent.type == EVENT_TYPE::kMouseHWheel) {
    const auto& mouseHWheel = wndEvent.data.mouseHWheel;
    onMouseHWheel(mouseHWheel.delta, mouseHWheel.modifiers);
  }
}

void
BaseApp::update()
{
  // Update systems
  g_time().update();

  // Call overridable update function
  onUpdate();
}

void
BaseApp::fixedUpdate()
{
  onFixedUpdate();
}

void
BaseApp::render()
{
  GraphicsManager& graphicMan = g_graphicsMan();

  graphicMan.clearRenderTarget(graphicMan.getMainRenderTargetView(),
                               m_backgroundColor);
  graphicMan.clearDepthStencil(graphicMan.getMainDepthStencil());

  // Call overridable render function
  onRender();
  
  // Present backbuffer
  graphicMan.present();
}

void
BaseApp::destroyManagers()
{
  Logger::shutDown();
  PhysicsManager::shutDown();
  SceneGraph::shutDown();
  ResourceManager::shutDown();
  ScriptManager::shutDown();
  Time::shutDown();
  Gizmos::shutDown();
  RenderManager::shutDown();
  ShaderManager::shutDown();
  //AudioManager::shutDown();
  GraphicsManager::shutDown();
  DynamicLibraryManager::shutDown();
  FileExplorer::shutDown();
}
}
