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

  SPtr<Screen> mainScreen = make_shared<Screen>();
  SPtr<ScreenEventHandle> eventQ = make_shared<ScreenEventHandle>();
  SampleDesc sample;
  sample.count = 1;
  sample.quality = 1;

  if (!mainScreen->init(desc, eventQ)) {
    return -1;
  }
  HINSTANCE hGetProcIDDLL = LoadLibrary("shDX11Graphicsd.dll");
  SH_ASSERT(hGetProcIDDLL && "Could not load dll");

  auto loadPlugin = reinterpret_cast<void(*)()>(GetProcAddress(hGetProcIDDLL, "loadPlugin"));
  SH_ASSERT(loadPlugin && "Could not load function");
  loadPlugin();

  GraphicsManager::instance().initManager(mainScreen, false, sample);
  
  initGraphicAssets(*mainScreen.get());

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
          updateCameraMove(0.1f, 2);
        }
        else if (keyboard.key == KEY::kA) {
          updateCameraMove(-0.1f, 0);
        }
        else if (keyboard.key == KEY::kS) {
          updateCameraMove(-0.1f, 2);
        }
        else if (keyboard.key == KEY::kD) {
          updateCameraMove(0.1f, 0);
        }
        else if (keyboard.key == KEY::kE) {
          updateCameraMove(0.1f, 1);
        }
        else if (keyboard.key == KEY::kQ) {
          updateCameraMove(-0.1f, 1);
        }
      }
      if (ev.type == EVENT_TYPE::kClose) {
        mainScreen->close();
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

  // TODO: Change abstraction level for file reading and change compile function.
  g_pProgramShader = gManager.createProgramShader("resources/BasicShader.hlsl",
                                                  "main",
                                                  "mainPS",
                                                  "vs_5_0",
                                                  "ps_5_0");

  SH_ASSERT(g_pProgramShader);

  /********************
  *  Input layout
  ********************/

  Vector<InputDesc> ilDesc;

  ilDesc.resize(3);

  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[0].size = 12;

  ilDesc[1].type = INPUT_LAYOUT_TYPES::kNormal;
  ilDesc[1].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[1].size = 12;

  ilDesc[2].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[2].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[2].size = 8;

  g_pInputLayout = gManager.createInputLayout(ilDesc, g_pProgramShader);
  SH_ASSERT(g_pInputLayout);

  /*****************************
  *  Vertex and Index buffers
  *****************************/

  Vector<String> modelPaths = { "resources/Frieren.fbx",
                                "resources/Treasure.fbx" };

  for (uint8 i = 0; i < modelPaths.size(); ++i) {
    g_resManager.loadModelFromFile(modelPaths[i]);
  }

  auto chest = g_resManager.m_loadedModels["resources/Treasure.fbx"];

  for (uint32 i = 0; i < chest->vertices.size(); ++i) {
    chest->vertices[i].position.x += 2.0f;
  }

  for (auto pModel : g_resManager.m_loadedModels) {
    for (uint32 i = 0; i < pModel.second->vertices.size(); ++i) {
      g_mesh.push_back(pModel.second->vertices[i]);
    }
    for (uint32 i = 0; i < pModel.second->indices.size(); ++i) {
      g_index.push_back(pModel.second->indices[i]);
    }
  }

  g_pVertexBuffer = gManager.createVertexBuffer(g_mesh);
  SH_ASSERT(g_pVertexBuffer);

  g_pIndexBuffer = gManager.createIndexBuffer(g_index);
  SH_ASSERT(g_pIndexBuffer);

  /********************
  *  Sampler state
  ********************/

  g_pSamplerLinear = gManager.createSamplerState();
  SH_ASSERT(g_pSamplerLinear);

  /*************************
  *  Texture and materials
  *************************/

  String texBody = "resources/Frieren_Col_v02.png";
  String texCloth = "resources/FrierenClothing_Col.png";
  String texHair = "resources/FrierenHair_Col_v02.png";
  String texIris = "resources/FrierenIris_Col.png";
  String texLash = "resources/FrierenLash_Col.png";
  String texSclera = "resources/FrierenSclera_Col.png";
  String texChest= "resources/Treasure_Color.png";

  g_resManager.loadTextureFromFile(texBody);
  g_resManager.loadTextureFromFile(texCloth);
  g_resManager.loadTextureFromFile(texHair);
  g_resManager.loadTextureFromFile(texIris);
  g_resManager.loadTextureFromFile(texLash);
  g_resManager.loadTextureFromFile(texSclera);
  g_resManager.loadTextureFromFile(texChest);

  auto frierenBodyMat = g_resManager.m_loadedMaterials["FrierenBody"];
  frierenBodyMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texBody];

  auto frierenClothMat = g_resManager.m_loadedMaterials["FrierenSkirt"];
  frierenClothMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texCloth];

  auto frierenHairMat = g_resManager.m_loadedMaterials["FrierenHair"];
  frierenHairMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texHair];

  auto frierenIrisMat = g_resManager.m_loadedMaterials["FrierenIris"];
  frierenIrisMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texIris];

  auto frierenLashMat = g_resManager.m_loadedMaterials["FrierenLash"];
  frierenLashMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texLash];

  auto frierenBrowMat = g_resManager.m_loadedMaterials["FrierenBrow"];
  frierenBrowMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texCloth];

  auto frierenScleraMat = g_resManager.m_loadedMaterials["FrierenSclera"];
  frierenScleraMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texSclera];

  auto chestMat = g_resManager.m_loadedMaterials["Treasure"];
  chestMat->textures[TEXTURE_TYPE::kBaseColor] = g_resManager.m_loadedTextures[texChest];

  /********************
  *  Camera
  ********************/

  g_world = Matrix4::IDENTITY;

  g_pWVP = gManager.createConstantBuffer(sizeof(WorldViewProjection));
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
  wvp.view = g_Camera.getView();
  wvp.proj = g_Camera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

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
  for (auto pModel : g_resManager.m_loadedModels) {
    for (uint32 i = 0; i < pModel.second->meshes.size(); ++i) {
      auto mat = pModel.second->materials[pModel.second->meshes[i].matIndex];
      gManager.setShaderResourceView(mat->textures[TEXTURE_TYPE::kBaseColor]);

      gManager.drawIndexed(pModel.second->meshes[i].numIndices,
                           indexCount,
                           vertexCount);

      indexCount += pModel.second->meshes[i].numIndices;
      vertexCount += pModel.second->meshes[i].numVertex;
    }
  }

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
