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

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shBaseApp.h"
#include "shException.h"
#include "shMath.h"
#include "shCamera.h"
#include "shLinearColor.h"

#include "shResourceManager.h"
#include "shDynamicLibrary.h"

#include "shImageResource.h"
#include "shMeshResource.h"
#include "shSkeletonResource.h"
#include "shAnimationResource.h"

#include "shMeshComponent.h"
#include "shAnimatorComponent.h"
#include "shTransformComponent.h"

namespace shEngineSDK {
struct WorldViewProjection
{
  Matrix4 world;
  ViewMatrix view;
  ProjectionMatrix proj;
};

BaseApp::BaseApp(const ScreenDesc& desc,
                 const String& dllGraphicApiName,
                 const SampleDesc& sample)
{
  m_screenDesc = desc;
  m_gphApiName = dllGraphicApiName;
  m_sample = sample;
}

void
BaseApp::run()
{
  // Creates window
  createWindow();

  // Load Graphic DLL
  loadGraphicAPI();

  // Initate system
  initManagers();

  // Send onCreate event
  onCreate();

  // Main App Loop
  while (m_mainScreen->isOpen()) {
    m_eventQueue->update();

    while (!m_eventQueue->empty()) {
      m_currentEvent = m_eventQueue->front();

      if (m_currentEvent.type == EVENT_TYPE::kClose) {
        m_mainScreen->close();
      }

      m_eventQueue->pop();
    }
    update(0.0f);
    render();
  }

  onDestroy();
  destroyManagers();
}

void
BaseApp::onCreate()
{}

void
BaseApp::onUpdate(float deltaTime)
{
  SH_UNREFERENCED_PARAMETER(deltaTime);
}

void
BaseApp::onRender()
{}

void
BaseApp::onMouseMove(const MouseMoveData& mouse)
{
  SH_UNREFERENCED_PARAMETER(mouse);
}

void
BaseApp::createWindow()
{
  m_mainScreen = make_shared<Screen>();
  m_eventQueue = make_shared<ScreenEventHandle>();

  if (!m_mainScreen->init(m_screenDesc, m_eventQueue)) {
    return;
  }
}

void
BaseApp::loadGraphicAPI()
{
  DynamicLibrary myDLL(m_gphApiName);
  auto dllSymbol = reinterpret_cast<void(*)()>(myDLL.getSymbol("loadPlugin"));
  SH_ASSERT(dllSymbol && "Could not load function");
  dllSymbol();
}

void
BaseApp::initManagers()
{
  GraphicsManager::instance().initManager(m_mainScreen, false, m_sample);
  ResourceManager::startUp<ResourceManager>();
}

void
BaseApp::handleScreenEvents()
{
  
}

void
BaseApp::update(float deltaTime)
{
  onUpdate(deltaTime);
}

void
BaseApp::render()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  onRender();

  auto pMainRTV = gManager.getMainRenderTargetView();
  LinearColor color(0.5f, 0.5f, 1.0f);
  gManager.clearRenderTarget(pMainRTV, color);

  auto pDepthStencil = gManager.getMainDepthStencil();
  gManager.clearDepthStencil(pDepthStencil);
  gManager.setRenderTargets(pMainRTV, pDepthStencil, 1);
  
  gManager.present();
}

void
BaseApp::destroyManagers()
{
  GraphicsManager::instance().shutDown();
  ResourceManager::instance().shutDown();
}
}
