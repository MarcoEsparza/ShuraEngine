/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/
*  @brief
*
*
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
#include "shCamera.h"
#include "shLinearColor.h"
#include "shResourceManager.h"
#include "shDynamicLibrary.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shEngineSDK {
struct WorldViewProjection
{
  Matrix4 world;
  ViewMatrix view;
  ProjectionMatrix proj;
};

void
BaseApp::run(const ScreenDesc& desc, const String& dllStr)
{
  m_mainScreen = make_shared<Screen>();
  m_eventQueue = make_shared<ScreenEventHandle>();
  SampleDesc sample;
  sample.count = 1;
  sample.quality = 1;

  if (!m_mainScreen->init(desc, m_eventQueue)) {
    return;
  }

  DynamicLibrary myDLL(dllStr);
  auto dllSymbol = reinterpret_cast<void(*)()>(myDLL.getSymbol("loadPlugin"));
  SH_ASSERT(dllSymbol && "Could not load function");
  dllSymbol();

  GraphicsManager::instance().initManager(m_mainScreen, false, sample);

  ResourceManager::startUp<ResourceManager>();
  initGraphicAssets();

  m_appRunning = true;
  float transform = 0.0f;

  while (m_appRunning) {
    handleEvents();
    update();
    render();
  }

  GraphicsManager::shutDown();
}

void
BaseApp::updateWorld(float& trsnform)
{

}

void
BaseApp::handleEvents()
{
  m_eventQueue->update();

  while (!m_eventQueue->empty()) {
    auto ev = m_eventQueue->front();

    if (ev.type == EVENT_TYPE::kMouseInput) {
      const MouseInputData mouse = ev.data.mouseInput;
    }
    if (ev.type == EVENT_TYPE::kMouseMove) {
      const MouseMoveData mousePos = ev.data.mouseMove;

      //g_lastMousePos = g_mousePos;
      //g_mousePos.x = mousePos.x;
      //g_mousePos.y = mousePos.y;

      //if ((g_lastMousePos.x - g_mousePos.x) != 0 ||
        //(g_lastMousePos.y - g_mousePos.y) != 0) {
        //updateCameraRotation();
      //}
    }
    if (ev.type == EVENT_TYPE::kKeyboard) {
      const KeyboardData keyboard = ev.data.keyboard;

      if (keyboard.key == KEY::kW) {
        //updateCameraMove(0.1f, 2);
      }
      else if (keyboard.key == KEY::kA) {
        //updateCameraMove(-0.1f, 0);
      }
      else if (keyboard.key == KEY::kS) {
        //updateCameraMove(-0.1f, 2);
      }
      else if (keyboard.key == KEY::kD) {
        //updateCameraMove(0.1f, 0);
      }
      else if (keyboard.key == KEY::kE) {
        //updateCameraMove(0.1f, 1);
      }
      else if (keyboard.key == KEY::kQ) {
        //updateCameraMove(-0.1f, 1);
      }
    }
    if (ev.type == EVENT_TYPE::kClose) {
      m_mainScreen->close();
      m_appRunning = false;
    }

    m_eventQueue->pop();
  }
}

void
BaseApp::update()
{

}

void
BaseApp::render()
{

}

void
BaseApp::initGraphicAssets()
{
  GraphicsManager& gManager = GraphicsManager::instance();
  ResourceManager& rManager = ResourceManager::instance();
  String resourcePath = "M:/ShuraEngine/Repo/ShuraEngine/resources";

  /********************
  *  Shaders
  ********************/
  // TODO: Change abstraction level for file reading and change compile function.

  String staticShaderPath = resourcePath + "shaders/StaticPBRMaterial.hlsl";
  m_pStaticShader = gManager.createProgramShader(staticShaderPath,
                                                 "main",
                                                 "mainPS",
                                                 "vs_5_0",
                                                 "ps_5_0");

  String skeletalShaderPath = resourcePath + "shaders/SkeletalPBRMaterial.hlsl";
  m_pSkeletalShader = gManager.createProgramShader(skeletalShaderPath,
                                                   "main",
                                                   "mainPS",
                                                   "vs_5_0",
                                                   "ps_5_0");

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

  m_pStaticInputLayout = gManager.createInputLayout(ilDesc, m_pStaticShader);

  ilDesc.resize(5);

  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[0].size = 12;

  ilDesc[1].type = INPUT_LAYOUT_TYPES::kNormal;
  ilDesc[1].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[1].size = 12;

  ilDesc[2].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[2].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[2].size = 8;

  ilDesc[3].type = INPUT_LAYOUT_TYPES::kBoneIndices;
  ilDesc[3].format = TEXTURE_FORMAT::kR32G32B32A32_float;
  ilDesc[3].size = 16;

  ilDesc[4].type = INPUT_LAYOUT_TYPES::kBoneWieghts;
  ilDesc[4].format = TEXTURE_FORMAT::kR32G32B32A32_float;
  ilDesc[4].size = 16;

  m_pSkeletalInputLayout = gManager.createInputLayout(ilDesc, m_pSkeletalShader);

  /*****************************
  *  Vertex and Index buffers
  *****************************/

  Vector<String> modelPaths = { "resources/Frieren.fbx",
                                "resources/Treasure.fbx" };

  String frierenModelPath = resourcePath + "Frieren.fbx";
  String treasureModelPath = resourcePath + "Treasure.fbx";

  rManager.loadResourceFromFile(frierenModelPath, RESOURCE_TYPE::kModel);
  rManager.loadResourceFromFile(treasureModelPath, RESOURCE_TYPE::kModel);

  /********************
  *  Sampler state
  ********************/

  m_pSamplerLinear = gManager.createSamplerState();
  SH_ASSERT(m_pSamplerLinear);

  /*************************
  *  Texture and materials
  *************************/

  String texBody = resourcePath + "Frieren_Col_v02.png";
  String texCloth = resourcePath + "FrierenClothing_Col.png";
  String texHair = resourcePath + "FrierenHair_Col_v02.png";
  String texIris = resourcePath + "FrierenIris_Col.png";
  String texLash = resourcePath + "FrierenLash_Col.png";
  String texSclera = resourcePath + "FrierenSclera_Col.png";
  String texChest = resourcePath + "Treasure_Color.png";

  rManager.loadResourceFromFile(texBody, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texCloth, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texHair, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texIris, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texLash, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texSclera, RESOURCE_TYPE::kTexture);
  rManager.loadResourceFromFile(texChest, RESOURCE_TYPE::kTexture);

  /********************
  *  Camera
  ********************/

  m_world = Matrix4::IDENTITY;

  m_pWVP = gManager.createConstantBuffer(sizeof(WorldViewProjection));
  SH_ASSERT(m_pWVP);

  Vector3 eye(0.0f, 0.0f, -5.0f);
  Vector3 at(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);

  m_editorCamera.setViewData(eye, at, up);
  m_editorCamera.setProjectionData(Math::PI / 4.0f,
                                   static_cast<float>(m_mainScreen->getWidth()),
                                   static_cast<float>(m_mainScreen->getHeight()),
                                   0.1f,
                                   100.0f);

  WorldViewProjection wvp;
  wvp.world = m_world;
  wvp.view = m_editorCamera.getView();
  wvp.proj = m_editorCamera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  gManager.updateConstantBuffer(m_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(m_pWVP);
}
}
