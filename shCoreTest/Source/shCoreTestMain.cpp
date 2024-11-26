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

SPtr<ProgramShader> g_pProgramShader;
SPtr<InputLayout> g_pInputLayout;
SPtr<VertexBuffer> g_pVertexBuffer;
SPtr<IndexBuffer> g_pIndexBuffer;
SPtr<ConstantBuffer> g_pWVP;
SPtr<SamplerState> g_pSamplerLinear;

Vector<VertexData> g_mesh;
Vector<uint32> g_index;

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
*  @brief Render the graphics api.
*/
void
render();

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
  desc.iconPath = "resources/ShuraIcon.ico";

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
render()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  auto pMainRTV = gManager.getMainRenderTargetView();
  LinearColor color(0.0f, 0.0f, 1.0f);
  gManager.clearRenderTarget(pMainRTV, color);
  
  auto pDepthStencil = gManager.getMainDepthStencil();
  gManager.clearDepthStencil(pDepthStencil);

  gManager.setRenderTargets(pMainRTV, pDepthStencil, 1);

  gManager.setSamplerState(g_pSamplerLinear);

  gManager.setProgramShader(g_pProgramShader);

  gManager.setInputLayout(g_pInputLayout);
  gManager.setVertexBuffers(g_pVertexBuffer);
  gManager.setIndexBuffers(g_pIndexBuffer);
  gManager.vsSetConstantBuffers(g_pWVP);
  gManager.setPrimitiveTopology();

  Vector<String> modelPaths = { "resources/Frieren.fbx",
                                "resources/Chest.fbx" };

  uint32 vertexCount = 0;
  uint32 indexCount = 0;
  /*for (auto pModel : g_resManager.m_loadedModels) {
    for (uint32 i = 0; i < pModel.second->meshes.size(); ++i) {
      auto mat = pModel.second->materials[pModel.second->meshes[i].matIndex];
      gManager.setShaderResourceView(mat->textures[TEXTURE_TYPE::kBaseColor]);

      gManager.drawIndexed(pModel.second->meshes[i].numIndices,
                           indexCount,
                           vertexCount);

      indexCount += pModel.second->meshes[i].numIndices;
      vertexCount += pModel.second->meshes[i].numVertex;
    }
  }*/

  gManager.present();
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
