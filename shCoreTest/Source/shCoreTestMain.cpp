/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
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

#include "functional"

using namespace shEngineSDK;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

SPtr<ProgramShader> g_pProgramShader;
SPtr<InputLayout> g_pInputLayout;
SPtr<Texture2D> g_pTexture;
SPtr<VertexBuffer> g_pVertexBuffer;
SPtr<IndexBuffer> g_pIndexBuffer;
SPtr<ConstantBuffer> g_pWVP;
SPtr<SamplerState> g_pSamplerLinear;
SPtr<Texture2D> g_pDepthSV;

Vector<VertexData> g_mesh;
Vector<uint32> g_index;

FPSCamera g_Camera;
Vector2i g_lastMousePos;
Vector2i g_mousePos;
Matrix4 g_world;

/**
*  @brief Initialize the graphics api assets.
* 
*  @param Screen& _screen
*/
void
initGraphicAssets(const Screen& _screen);

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

int main()
{
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

  Screen mainScreen;
  SPtr<ScreenEventHandle> eventQ = make_shared<ScreenEventHandle>();
  SampleDesc sample;
  sample.count = 1;
  sample.quality = 1;

  if (!mainScreen.init(desc, eventQ)) {
    return -1;
  }
  HINSTANCE hGetProcIDDLL = LoadLibrary("shDX11Graphicsd.dll");
  SH_ASSERT(hGetProcIDDLL && "Could not load dll");

  auto loadPlugin = reinterpret_cast<void(*)()>(GetProcAddress(hGetProcIDDLL, "loadPlugin"));
  SH_ASSERT(loadPlugin && "Could not load function");
  loadPlugin();

  GraphicsManager::instance().initManager(&mainScreen, false, sample);
  
  initGraphicAssets(mainScreen);

  bool isRunning = true;
  float transform = 0.0f;

  while (isRunning) {
    eventQ->update();

    while (!eventQ->empty()) {
      auto ev = eventQ->front();

      if (ev.type == EVENT_TYPE::kMouseInput) {
        const MouseInputData mouse = ev.data.mouseInput;
      }
      if (ev.type == EVENT_TYPE::kMouseMove) {
        const MouseMoveData mousePos = ev.data.mouseMove;

        g_lastMousePos = g_mousePos;
        g_mousePos.x = mousePos.x;
        g_mousePos.y = mousePos.y;

        if ((g_lastMousePos.x - g_mousePos.x) != 0 ||
            (g_lastMousePos.y - g_mousePos.y) != 0) {
          updateCameraRotation();
        }
      }
      if (ev.type == EVENT_TYPE::kKeyboard) {
        const KeyboardData keyboard = ev.data.keyboard;

        if (keyboard.key == KEY::kW) {
          updateCameraMove(0.5f, 2);
        }
        else if (keyboard.key == KEY::kA) {
          updateCameraMove(-0.5f, 0);
        }
        else if (keyboard.key == KEY::kS) {
          updateCameraMove(-0.5f, 2);
        }
        else if (keyboard.key == KEY::kD) {
          updateCameraMove(0.5f, 0);
        }
        else if (keyboard.key == KEY::kE) {
          updateCameraMove(0.5f, 1);
        }
        else if (keyboard.key == KEY::kQ) {
          updateCameraMove(-0.5f, 1);
        }
      }
      if (ev.type == EVENT_TYPE::kClose) {
        mainScreen.close();
        isRunning = false;
      }

      eventQ->pop();
    }

    update(transform);
    render();
  }
  
  GraphicsManager::shutDown();

  return 0;
}

void
initGraphicAssets(const Screen& _screen)
{
  GraphicsManager& gManager = GraphicsManager::instance();

  /********************
  *  Shaders
  ********************/

  g_pProgramShader = gManager.createProgramShader("resources/BasicShader.hlsl",
                                                  "main",
                                                  "mainPS",
                                                  "vs_5_0",
                                                  "ps_5_0");

  SH_ASSERT(g_pProgramShader);

  /********************
  *  Input layout
  ********************/

  Vector<INPUT_LAYOUT_TYPES::E> ilTypes;

  ilTypes.push_back(INPUT_LAYOUT_TYPES::E::kPosition);
  ilTypes.push_back(INPUT_LAYOUT_TYPES::E::kNormal);
  ilTypes.push_back(INPUT_LAYOUT_TYPES::E::kTexcoord);

  g_pInputLayout = gManager.createInputLayout(ilTypes, g_pProgramShader);
  SH_ASSERT(g_pInputLayout);

  /*****************************
  *  Vertex and Index buffers
  *****************************/

  Vector<VertexData> cube;
  cube.resize(24);

  // Up
  cube[0].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[0].tex = Vector2(0.0f, 1.0f);
  cube[1].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[1].tex = Vector2(1.0f, 1.0f);
  cube[2].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[2].tex = Vector2(1.0f, 0.0f);
  cube[3].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[3].tex = Vector2(0.0f, 0.0f);

  // Down
  cube[4].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[4].tex = Vector2(0.0f, 0.0f);
  cube[5].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[5].tex = Vector2(1.0f, 0.0f);
  cube[6].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[6].tex = Vector2(1.0f, 1.0f);
  cube[7].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[7].tex = Vector2(0.0f, 1.0f);

  // Left
  cube[8].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[8].tex = Vector2(0.0f, 1.0f);
  cube[9].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[9].tex = Vector2(1.0f, 1.0f);
  cube[10].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[10].tex = Vector2(1.0f, 0.0f);
  cube[11].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[11].tex = Vector2(0.0f, 0.0f);

  // Right
  cube[12].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[12].tex = Vector2(0.0f, 1.0f);
  cube[13].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[13].tex = Vector2(1.0f, 1.0f);
  cube[14].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[14].tex = Vector2(1.0f, 0.0f);
  cube[15].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[15].tex = Vector2(0.0f, 0.0f);

  // Front
  cube[16].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[16].tex = Vector2(0.0f, 1.0f);
  cube[17].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[17].tex = Vector2(1.0f, 1.0f);
  cube[18].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[18].tex = Vector2(1.0f, 0.0f);
  cube[19].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[19].tex = Vector2(0.0f, 0.0f);

  // Back
  cube[20].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[20].tex = Vector2(0.0f, 0.0f);
  cube[21].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[21].tex = Vector2(1.0f, 0.0f);
  cube[22].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[22].tex = Vector2(1.0f, 1.0f);
  cube[23].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[23].tex = Vector2(0.0f, 1.0f);

  Vector<uint32> indices = { 3,1,0,
                             2,1,3,
                             
                             6,4,5,
                             7,4,6,
                             
                             11,9,8,
                             10,9,11,
                             
                             14,12,13,
                             15,12,14,
                             
                             19,17,16,
                             18,17,19,
                             
                             22,20,21,
                             23,20,22 };

  g_mesh = cube;
  g_index = indices;

  g_pVertexBuffer = gManager.createVertexBuffer(g_mesh);
  SH_ASSERT(g_pVertexBuffer);

  g_pIndexBuffer = gManager.createIndexBuffer(g_index);
  SH_ASSERT(g_pIndexBuffer);

  /********************
  *  Sampler state
  ********************/

  g_pSamplerLinear = gManager.createSamplerState();
  SH_ASSERT(g_pSamplerLinear);

  /********************
  *  Texture
  ********************/

  String path = "resources/ShuraIconOption.png";
  g_pTexture = gManager.createTextureFromFile(path);
  SH_ASSERT(g_pTexture);

  /********************
  *  Camera
  ********************/

  g_world = Matrix4::IDENTITY;

  uint32 sizeWVP = sizeof(Matrix4) * 3;
  g_pWVP = gManager.createConstantBuffer(sizeWVP);
  SH_ASSERT(g_pWVP);

  Vector3 eye(0.0f, 0.0f, -5.0f);
  Vector3 at(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);

  g_Camera.setViewData(eye, at, up);
  g_Camera.setProjectionData(Math::PI / 4.0f,
                             static_cast<float>(_screen.getWidth()),
                             static_cast<float>(_screen.getHeight()),
                             0.1f,
                             100.0f);

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view.transpose(g_Camera.getView());
  wvp.proj.transpose(g_Camera.getProjection());

  gManager.updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}

void
update(float& transform)
{
  transform += Math::PI * 0.000125f;
  g_world = g_world.createRotationXMatrix(transform);

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view.transpose(g_Camera.getView());
  wvp.proj.transpose(g_Camera.getProjection());

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
  gManager.setShaderResourceView(g_pTexture);

  gManager.setProgramShader(g_pProgramShader);

  gManager.setInputLayout(g_pInputLayout);
  gManager.setVertexBuffers(g_pVertexBuffer);
  gManager.setIndexBuffers(g_pIndexBuffer);
  gManager.vsSetConstantBuffers(g_pWVP);
  gManager.setPrimitiveTopology();
  
  gManager.drawIndexed(static_cast<uint32>(g_index.size()), 0, 0);

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
  wvp.view.transpose(g_Camera.getView());
  wvp.proj.transpose(g_Camera.getProjection());

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}

void
updateCameraRotation()
{
  const float dx = static_cast<float>(g_lastMousePos.x - g_mousePos.x) * -0.005f;
  const float dy = static_cast<float>(g_lastMousePos.y - g_mousePos.y) * 0.005f;

  if (g_lastMousePos.x != g_mousePos.x ||
    g_lastMousePos.y != g_mousePos.y)
  {
    g_Camera.rotateCam(dx, dy);
  }

  WorldViewProjection wvp;
  wvp.world = g_world;
  wvp.view.transpose(g_Camera.getView());
  wvp.proj.transpose(g_Camera.getProjection());

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(g_pWVP);
}
