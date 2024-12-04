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
#include "shMeshComponent.h"
#include "shImageResource.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using std::reinterpret_pointer_cast;

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

  while (m_appRunning) {
    handleEvents();
    update();
    render();
  }

  GraphicsManager::shutDown();
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
  GraphicsManager& gManager = GraphicsManager::instance();

  auto pMainRTV = gManager.getMainRenderTargetView();
  LinearColor color(0.0f, 0.0f, 1.0f);
  gManager.clearRenderTarget(pMainRTV, color);

  auto pDepthStencil = gManager.getMainDepthStencil();
  gManager.clearDepthStencil(pDepthStencil);
  gManager.setRenderTargets(pMainRTV, pDepthStencil, 1);
  gManager.setSamplerState(m_pSamplerLinear);
  gManager.vsSetConstantBuffers(m_pWVP);
  gManager.setPrimitiveTopology();

  m_scene.drawStaticMeshesInScene(m_pStaticInputLayout);
  m_scene.drawSkeletalMeshesInScene(m_pSkeletalInputLayout);

  gManager.present();
}

void
BaseApp::initGraphicAssets()
{
  GraphicsManager& gManager = GraphicsManager::instance();
  ResourceManager& rManager = ResourceManager::instance();
  String resourcePath = "M:/ShuraEngine/Repo/ShuraEngine/resources/";

  /********************
  *  Shaders
  ********************/
  // TODO: Change abstraction level for file reading and change compile function.

  String staticShaderPath = resourcePath + "shaders/StaticPBRShader.hlsl";
  m_pStaticShader = gManager.createProgramShader(staticShaderPath,
                                                 "main",
                                                 "mainPS",
                                                 "vs_5_0",
                                                 "ps_5_0");

  String skeletalShaderPath = resourcePath + "shaders/SkeletalPBRShader.hlsl";
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

  /********************
  *  Sampler state
  ********************/

  m_pSamplerLinear = gManager.createSamplerState();
  SH_ASSERT(m_pSamplerLinear);

  /********************
  *  Load resources
  ********************/

  String frierenModelPath = resourcePath + "Frieren.fbx";
  String treasureModelPath = resourcePath + "Treasure.fbx";

  rManager.loadResourceFromFile(treasureModelPath);
  rManager.loadResourceFromFile(frierenModelPath);

  String texBody = resourcePath + "Frieren_Col_v02.png";
  String texCloth = resourcePath + "FrierenClothing_Col.png";
  String texHair = resourcePath + "FrierenHair_Col_v02.png";
  String texIris = resourcePath + "FrierenIris_Col.png";
  String texLash = resourcePath + "FrierenLash_Col.png";
  String texSclera = resourcePath + "FrierenSclera_Col.png";
  String texChest = resourcePath + "Treasure_Color.png";

  rManager.loadResourceFromFile(texChest);
  rManager.loadResourceFromFile(texBody);
  rManager.loadResourceFromFile(texCloth);
  rManager.loadResourceFromFile(texHair);
  rManager.loadResourceFromFile(texIris);
  rManager.loadResourceFromFile(texLash);
  rManager.loadResourceFromFile(texSclera);

  /***********************
  *  Create GamoObjects
  * 
  *  The resource loading and creating objects in this function is only
  *  temporary while an appropiate editor is created.
  ***********************/

  auto chestGO = make_shared<GameObject>();
  chestGO->name = "TreasureChestGO";

  auto smComponent = make_shared<StaticMeshComponent>();

  auto chestResource =
  reinterpret_pointer_cast<StaticMeshResource>(rManager.getResource("Cube.002"));
  smComponent->meshData = chestResource;
  smComponent->material = make_shared<PBRMaterial>();
  smComponent->material->name = "TreasureMat";
  smComponent->material->shader = m_pStaticShader;
  auto componentMaterial = reinterpret_pointer_cast<PBRMaterial>(smComponent->material);
  auto chestTexture =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("Treasure_Color.png"));
  componentMaterial->baseColor = chestTexture->texture;

  auto frierenObject = make_shared<GameObject>();
  frierenObject->name = "FrierenGO";

  auto skeletalMC = make_shared<SkeletalMeshComponent>();
  auto frierenMeshResource =
  reinterpret_pointer_cast<SkeletalMeshResource>(rManager.getResource("Frieren"));
  skeletalMC->setMeshData(frierenMeshResource);
  skeletalMC->materials.resize(skeletalMC->meshData->numMeshes);
  
  auto frierenBodyMat = make_shared<PBRMaterial>();
  frierenBodyMat->name = "FrirenBodyMat";
  frierenBodyMat->shader = m_pSkeletalShader;
  auto frierenBodyTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("Frieren_Col_v02.png"));
  frierenBodyMat->baseColor = frierenBodyTex->texture;

  auto frierenClothMat = make_shared<PBRMaterial>();
  frierenClothMat->name = "FrierenClothMat";
  frierenClothMat->shader = m_pSkeletalShader;
  auto frierenClothTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenClothing_Col.png"));
  frierenBodyMat->baseColor = frierenClothTex->texture;

  auto frierenHairMat = make_shared<PBRMaterial>();
  frierenHairMat->name = "FrierenHairMat";
  frierenHairMat->shader = m_pSkeletalShader;
  auto frierenHairTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenHair_Col_v02.png"));
  frierenBodyMat->baseColor = frierenHairTex->texture;

  auto frierenIrisMat = make_shared<PBRMaterial>();
  frierenIrisMat->name = "FrierenIrisMat";
  frierenIrisMat->shader = m_pSkeletalShader;
  auto frierenIrisTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenIris_Col.png"));
  frierenBodyMat->baseColor = frierenIrisTex->texture;

  auto frierenLashMat = make_shared<PBRMaterial>();
  frierenLashMat->name = "FrierenLashMat";
  frierenLashMat->shader = m_pSkeletalShader;
  auto frierenLashTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenLash_Col.png"));
  frierenBodyMat->baseColor = frierenLashTex->texture;

  auto frierenScleraMat = make_shared<PBRMaterial>();
  frierenScleraMat->name = "FrierenScleraMat";
  frierenScleraMat->shader = m_pSkeletalShader;
  auto frierenScleraTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenSclera_Col.png"));
  frierenBodyMat->baseColor = frierenScleraTex->texture;

  auto frierenBrowMat = make_shared<PBRMaterial>();
  frierenBrowMat->name = "FrierenBrowMat";
  frierenBrowMat->shader = m_pSkeletalShader;
  frierenBodyMat->baseColor = frierenClothTex->texture;

  skeletalMC->materials[0] = frierenHairMat;
  skeletalMC->materials[1] = frierenHairMat;
  skeletalMC->materials[2] = frierenClothMat;
  skeletalMC->materials[3] = frierenClothMat;
  skeletalMC->materials[4] = frierenLashMat;
  skeletalMC->materials[5] = frierenScleraMat;
  skeletalMC->materials[6] = frierenIrisMat;
  skeletalMC->materials[7] = frierenClothMat;
  skeletalMC->materials[8] = frierenBrowMat;
  skeletalMC->materials[9] = frierenClothMat;
  skeletalMC->materials[10] = frierenBodyMat;

  chestGO->addComponent(smComponent);
  frierenObject->addComponent(skeletalMC);
  m_scene.addObject(chestGO);
  m_scene.addObject(frierenObject);

  m_scene.updateBuffers();

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
