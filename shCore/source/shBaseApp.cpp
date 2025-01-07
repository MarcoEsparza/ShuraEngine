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

#include "shImageResource.h"
#include "shMeshResource.h"
#include "shSkeletonResource.h"
#include "shAnimationResource.h"

#include "shMeshComponent.h"
#include "shAnimatorComponent.h"
#include "shTransformComponent.h"

#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
struct WorldViewProjection
{
  Matrix4 world;
  ViewMatrix view;
  ProjectionMatrix proj;
};

void
BaseApp::run(const ScreenDesc& desc, const String& dllGraphicApiName)
{
  m_mainScreen = make_shared<Screen>();
  m_eventQueue = make_shared<ScreenEventHandle>();
  SampleDesc sample;
  sample.count = 1;
  sample.quality = 1;

  if (!m_mainScreen->init(desc, m_eventQueue)) {
    return;
  }

  DynamicLibrary myDLL(dllGraphicApiName);
  auto dllSymbol = reinterpret_cast<void(*)()>(myDLL.getSymbol("loadPlugin"));
  SH_ASSERT(dllSymbol && "Could not load function");
  dllSymbol();

  GraphicsManager::instance().initManager(m_mainScreen, false, sample);

  ResourceManager::startUp<ResourceManager>();
  initGraphicAssets();

  m_appRunning = true;

  float time = 0.0f;

  while (m_appRunning) {
    auto start = high_resolution_clock::now();

    handleEvents();
    update(time);
    render();

    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;

    time += static_cast<float>(elapsed.count());
  }

  GraphicsManager::shutDown();
}

void
BaseApp::updateSMBuffers()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  Vector<VertexData> smVertexData;
  Vector<uint32> smIndexData;

  for (auto& gObject : m_scene.getGameObjectList()) {
    for (auto& component : gObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
        for (auto& vertex : sMeshComponent->meshData->vertices) {
          smVertexData.push_back(vertex);
        }
        for (auto index : sMeshComponent->meshData->indices) {
          smIndexData.push_back(index);
        }
      }
    }
  }

  m_staticVBuffer = gManager.createVertexBuffer(smVertexData);
  m_staticIBuffer = gManager.createIndexBuffer(smIndexData);

  SH_ASSERT(m_staticVBuffer);
  SH_ASSERT(m_staticIBuffer);
}

void
BaseApp::moveChest(const Vector3& newPos)
{
  m_scene.getGameObjectList()[0]->move(newPos);
  updateSMBuffers();
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

      m_lastMousePos = m_mousePos;
      m_mousePos.x = mousePos.x;
      m_mousePos.y = mousePos.y;

      if ((m_lastMousePos.x - m_mousePos.x) != 0 ||
          (m_lastMousePos.y - m_mousePos.y) != 0) {
        rotateCamera();
      }
    }
    if (ev.type == EVENT_TYPE::kKeyboard) {
      const KeyboardData keyboard = ev.data.keyboard;

      if (keyboard.key == KEY::kW) {
        moveCameraPosition(0.1f, AXIS::kZ);
      }
      else if (keyboard.key == KEY::kA) {
        moveCameraPosition(-0.1f, AXIS::kX);
      }
      else if (keyboard.key == KEY::kS) {
        moveCameraPosition(-0.1f, AXIS::kZ);
      }
      else if (keyboard.key == KEY::kD) {
        moveCameraPosition(0.1f, AXIS::kX);
      }
      else if (keyboard.key == KEY::kE) {
        moveCameraPosition(0.1f, AXIS::kY);
      }
      else if (keyboard.key == KEY::kQ) {
        moveCameraPosition(-0.1f, AXIS::kY);
      }
      else if (keyboard.key == KEY::kUp) {
        moveChest(Vector3(0.0f, 1.0f, 0.0f));
      }
      else if (keyboard.key == KEY::kLeft) {
        moveChest(Vector3(-1.0f, 0.0f, 0.0f));
      }
      else if (keyboard.key == KEY::kDown) {
        moveChest(Vector3(0.0f, -1.0f, 0.0f));
      }
      else if (keyboard.key == KEY::kRight) {
        moveChest(Vector3(1.0f, 0.0f, 0.0f));
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
BaseApp::update(const float time)
{
  GraphicsManager& gManager = GraphicsManager::instance();

  for (auto& gObject : m_scene.getGameObjectList()) {
    for (auto& component : gObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkeletalMesh) {
        auto skMesh = reinterpret_pointer_cast<SkeletalMeshComponent>(component);
        
        for (auto& otherComp : gObject->components) {
          if (otherComp->getType() == COMPONENT_TYPE::kAnimator) {
            /*auto animator = reinterpret_pointer_cast<AnimatorComponent>(otherComp);
            animator->updateAnimation(time);

            gManager.updateConstantBuffer(skMesh->m_meshBuffer,
                                          animator->finalTransform.data(),
                                          skMesh->skeletonData->boneCount * sizeof(Matrix4));*/
          }
        }
      }
    }
  }
}

void
BaseApp::render()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  auto pMainRTV = gManager.getMainRenderTargetView();
  LinearColor color(0.5f, 0.5f, 1.0f);
  gManager.clearRenderTarget(pMainRTV, color);

  auto pDepthStencil = gManager.getMainDepthStencil();
  gManager.clearDepthStencil(pDepthStencil);
  gManager.setRenderTargets(pMainRTV, pDepthStencil, 1);
  gManager.setSamplerState(m_pSamplerLinear);
  gManager.vsSetConstantBuffers(m_pWVP);
  gManager.setPrimitiveTopology();

  drawStaticMeshesInScene();
  drawSkeletalMeshesInScene();

  gManager.present();
}

void
BaseApp::drawStaticMeshesInScene()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setInputLayout(m_pStaticInputLayout);
  gManager.setVertexBuffers(m_staticVBuffer);
  gManager.setIndexBuffers(m_staticIBuffer);

  uint32 vertexCount = 0;
  uint32 indexCount = 0;
  for (auto& gObject : m_scene.getGameObjectList()) {
    for (auto& component : gObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
        auto meshMat = reinterpret_pointer_cast<PBRMaterial>(sMeshComponent->meshData->material);
        gManager.setProgramShader(meshMat->shader);
        gManager.setShaderResourceView(meshMat->baseColor);

        gManager.drawIndexed(sMeshComponent->meshData->numIndex,
                             indexCount,
                             vertexCount);

        indexCount += sMeshComponent->meshData->numIndex;
        vertexCount += sMeshComponent->meshData->numVertex;
      }
    }
  }
}

void
BaseApp::drawSkeletalMeshesInScene()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setInputLayout(m_pSkeletalInputLayout);

  for (auto& gObject : m_scene.getGameObjectList()) {
    for (auto& component : gObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkeletalMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<SkeletalMeshComponent>(component);

        gManager.setVertexBuffers(sMeshComponent->m_vertexBuffer);
        gManager.setIndexBuffers(sMeshComponent->m_indexBuffer);
        gManager.vsSetConstantBuffers(sMeshComponent->m_meshBuffer, 1);

        uint32 vertexCount = 0;
        uint32 indexCount = 0;
        for (uint32 i = 0; i < sMeshComponent->meshData->numMeshes; ++i) {
          auto meshMat = reinterpret_pointer_cast<PBRMaterial>(sMeshComponent->materials[i]);
          gManager.setProgramShader(meshMat->shader);
          gManager.setShaderResourceView(meshMat->baseColor);

          gManager.drawIndexed(sMeshComponent->meshData->numIndices[i],
            indexCount,
            vertexCount);

          indexCount += sMeshComponent->meshData->numIndices[i];
          vertexCount += sMeshComponent->meshData->numVertices[i];
        }
      }
    }
  }
}

void
BaseApp::moveCameraPosition(const float direction, const AXIS::E axis)
{
  if (axis == AXIS::kX) {
    m_editorCamera.moveX(direction);
  }
  else if (axis == AXIS::kY) {
    m_editorCamera.moveY(direction);
  }
  else if (axis == AXIS::kZ) {
    m_editorCamera.moveZ(direction);
  }

  WorldViewProjection wvp;
  wvp.world = m_world;
  wvp.view = m_editorCamera.getView();
  wvp.proj = m_editorCamera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(m_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(m_pWVP);
}

void
BaseApp::rotateCamera()
{
  const float cameraDelay = 0.005f;

  const float dx = static_cast<float>(m_lastMousePos.x - m_mousePos.x) * -cameraDelay;
  const float dy = static_cast<float>(m_lastMousePos.y - m_mousePos.y) * cameraDelay;

  if (m_lastMousePos.x != m_mousePos.x ||
      m_lastMousePos.y != m_mousePos.y)
  {
    m_editorCamera.rotateCam(dx, dy);
  }

  WorldViewProjection wvp;
  wvp.world = m_world;
  wvp.view = m_editorCamera.getView();
  wvp.proj = m_editorCamera.getProjection();

  wvp.view.getTransposed();
  wvp.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(m_pWVP, &wvp, sizeof(wvp));
  SH_ASSERT(m_pWVP);
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

  String whitePNGPath = resourcePath + "White.png";
  Path whiteTex("M:/ShuraEngine/Repo/ShuraEngine/resources/White.png");
  SPtr<ImageResource> whiteIR =
  reinterpret_pointer_cast<ImageResource>(rManager.loadResourceFromFile(whiteTex));

  Path chestPath("M:/ShuraEngine/Repo/ShuraEngine/resources/Treasure.fbx");
  Path chestTexPath("M:/ShuraEngine/Repo/ShuraEngine/resources/Treasure_Color.png");

  SPtr<ImageResource> chestIR = 
  reinterpret_pointer_cast<ImageResource>(rManager.loadResourceFromFile(chestTexPath));

  SPtr<StaticMeshResource> chestSMR =
  reinterpret_pointer_cast<StaticMeshResource>(rManager.loadResourceFromFile(chestPath));

  Path emiliaPath("M:/ShuraEngine/Repo/ShuraEngine/resources/EmiliaDancing.fbx");
  Path emiliaTexPath("M:/ShuraEngine/Repo/ShuraEngine/resources/Rezero_Emilia_diff.png");

  SPtr<ImageResource> emiliaIR = 
  reinterpret_pointer_cast<ImageResource>(rManager.loadResourceFromFile(emiliaTexPath));

  SPtr<SkeletalMeshResource> emiliaSMR =
  reinterpret_pointer_cast<SkeletalMeshResource>(rManager.loadResourceFromFile(emiliaPath));

  /***********************
  *  Create GamoObjects
  * 
  *  The resource loading and creating objects in this function is only
  *  temporary while an appropiate editor is created.
  ***********************/

  auto chestGO = make_shared<GameObject>();
  auto chestSMC = make_shared<StaticMeshComponent>();

  chestSMR->material->shader = m_pStaticShader;
  auto chestPBRMat = reinterpret_pointer_cast<PBRMaterial>(chestSMR->material);
  chestPBRMat->baseColor = chestIR->texture;
  chestSMC->meshData = chestSMR;

  chestGO->name = "TreasureChest";
  chestGO->addComponent(chestSMC);

  chestGO->rotate(Vector3(90.0f, 0.0f, 180.0f));

  auto emiliaGO = make_shared<GameObject>();
  auto emiliaSMC = make_shared<SkeletalMeshComponent>();

  //emiliaSMR->material->shader = m_pStaticShader;
  //auto emiliaPBRMat = reinterpret_pointer_cast<PBRMaterial>(emiliaSMR->material);
  //emiliaPBRMat->baseColor = emiliaIR->texture;
  //emiliaSMC->meshData = emiliaSMR;

  emiliaGO->name = "Emilia";
  emiliaGO->addComponent(emiliaSMC);

  m_scene.addObject(chestGO);

  updateSMBuffers();

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
                                   300.0f);

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
