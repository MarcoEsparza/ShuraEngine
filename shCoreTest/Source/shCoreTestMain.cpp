/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/06
*  @brief   Here is the main to test the window initialize.
*
*  Here is the main to test the window initialize.
* 
*  @bug     Camera movement not working properly.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"
#include "shGraphicsManager.h"
#include "shMath.h"
#include "shCamera.h"
#include "shLinearColor.h"
#include "shResourceManager.h"

#include "functional"

using namespace shEngineSDK;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "shBaseApp.h"

SPtr<ConstantBuffer> g_pWVP;

FPSCamera g_Camera;
Vector2i g_lastMousePos;
Vector2i g_mousePos;
Matrix4 g_world;

ResourceManager g_resManager;

/**
*  @brief View struct.
*/
struct WorldViewProjection
{
  Matrix4 world;
  ViewMatrix view;
  ProjectionMatrix proj;
};

/**
*  @brief Update the world matrix to move the cube.
*  @note This is only for test.
*
*  @param float& transform
*/
void
update(float& transform);

/**
*  @brief Updates the camera position.
*  @note This is only for test.
*
*  @param float& direction
*  @param const uint32 axis
*/
void
updateCameraMove(const float& direction, const uint32 axis);

/**
*  @brief Updates the camera rotation.
*  @note This is only for test.
*  @bug Not working properly
*/
void
updateCameraRotation();

int main(int argc, const char** argv)
{
  BaseApp app;

  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "Graphics Test";
  desc.visible = true;
  desc.fullscreen = false;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 1000;
  desc.height = 600;
  desc.iconPath = "M:/ShuraEngine/Repo/ShuraEngine/resources/ShuraIcon.ico";

  String dllName;

  String graphicsName = argv[1];

#ifdef SH_DEBUG_MODE

  if (graphicsName == "DX11") {
    dllName = "shDX11Graphicsd";
  }
  else if (graphicsName == "OGL") {
    dllName = "shOGLGraphicsd";
  }

#else

  if (graphicsName == "DX11") {
    dllName = "shDX11Graphics";
  }
  else if (graphicsName == "OGL") {
    dllName = "shOGLGraphics";
  }

#endif

  app.run(desc, dllName);
}

void
update(float& transform)
{
  transform += Math::PI * 0.000125f;
  g_world = g_world.createRotationXMatrix(transform);

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view = g_Camera.getView();
  wvp.proj = g_Camera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}

void
updateCameraMove(const float& direction, const uint32 axis)
{
  if (axis == 0) {
    g_Camera.moveX(direction);
  }
  else if (axis == 1) {
    g_Camera.moveY(direction);
  }
  else if (axis == 2) {
    g_Camera.moveZ(direction);
  }

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view = g_Camera.getView();
  wvp.proj = g_Camera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}

void
updateCameraRotation()
{
  const float cameraDelay = 0.005f;

  const float dx = static_cast<float>(g_lastMousePos.x - g_mousePos.x) * -cameraDelay;
  const float dy = static_cast<float>(g_lastMousePos.y - g_mousePos.y) * cameraDelay;

  if (g_lastMousePos.x != g_mousePos.x ||
    g_lastMousePos.y != g_mousePos.y)
  {
    g_Camera.rotateCam(dx, dy);
  }

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view = g_Camera.getView();
  wvp.proj = g_Camera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}
