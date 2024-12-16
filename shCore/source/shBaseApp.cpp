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
            auto animator = reinterpret_pointer_cast<AnimatorComponent>(otherComp);
            animator->updateAnimation(time);

            gManager.updateConstantBuffer(skMesh->m_meshBuffer,
                                          animator->finalTransform.data(),
                                          skMesh->skeletonData->boneCount * sizeof(Matrix4));
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

  uint32 vbSlot = 1;
  uint32 ibSlot = 1;
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
        ++vbSlot;
        ++ibSlot;
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
  rManager.loadResourceFromFile(whitePNGPath);

  String frierenModelPath = resourcePath + "FrierenSalsaDancing.fbx";
  String treasureModelPath = resourcePath + "Treasure.fbx";
  String sponzaModelPath = resourcePath + "sponza.obj";

  rManager.loadResourceFromFile(treasureModelPath);
  rManager.loadResourceFromFile(frierenModelPath);
  rManager.loadResourceFromFile(sponzaModelPath);

  String texBody = resourcePath + "Frieren_Col_v02.png";
  String texCloth = resourcePath + "FrierenClothing_Col.png";
  String texHair = resourcePath + "FrierenHair_Col_v02.png";
  String texIris = resourcePath + "FrierenIris_Col.png";
  String texLash = resourcePath + "FrierenLash_Col.png";
  String texSclera = resourcePath + "FrierenSclera_Col.png";
  String texChest = resourcePath + "Treasure_Color.png";

  String texSponza = resourcePath + "background.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "chain_texture.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "lion.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "spnza_bricks_a_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_arch_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_ceiling_a_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_column_a_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_column_b_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_column_c_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_curtain_blue_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_curtain_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_curtain_green_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_details_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_fabric_blue_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_fabric_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_fabric_green_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_flagpole_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_floor_a_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_roof_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "sponza_thorn_diff.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "vase_dif.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "vase_hanging.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "vase_plant.tga";
  rManager.loadResourceFromFile(texSponza);
  texSponza = resourcePath + "vase_round.tga";
  rManager.loadResourceFromFile(texSponza);

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
  auto componentMaterial = reinterpret_pointer_cast<PBRMaterial>(smComponent->meshData->material);
  auto chestTexture =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("Treasure_Color.png"));
  componentMaterial->baseColor = chestTexture->texture;
  componentMaterial->shader = m_pStaticShader;

  auto frierenObject = make_shared<GameObject>();
  frierenObject->name = "FrierenGO";

  auto skeletalMC = make_shared<SkeletalMeshComponent>();
  auto frierenMeshResource =
  reinterpret_pointer_cast<SkeletalMeshResource>(rManager.getResource("FrierenSalsaDancing"));
  
  //for (auto& vertex : frierenMeshResource->vertices) {
  //  vertex.position = vertex.position.rotateX(90.0f * Math::DEG2RAD);
  //  vertex.position = vertex.position.rotateZ(180.0f * Math::DEG2RAD);
  //}

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
  frierenClothMat->baseColor = frierenClothTex->texture;

  auto frierenHairMat = make_shared<PBRMaterial>();
  frierenHairMat->name = "FrierenHairMat";
  frierenHairMat->shader = m_pSkeletalShader;
  auto frierenHairTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenHair_Col_v02.png"));
  frierenHairMat->baseColor = frierenHairTex->texture;

  auto frierenIrisMat = make_shared<PBRMaterial>();
  frierenIrisMat->name = "FrierenIrisMat";
  frierenIrisMat->shader = m_pSkeletalShader;
  auto frierenIrisTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenIris_Col.png"));
  frierenIrisMat->baseColor = frierenIrisTex->texture;

  auto frierenLashMat = make_shared<PBRMaterial>();
  frierenLashMat->name = "FrierenLashMat";
  frierenLashMat->shader = m_pSkeletalShader;
  auto frierenLashTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenLash_Col.png"));
  frierenLashMat->baseColor = frierenLashTex->texture;

  auto frierenScleraMat = make_shared<PBRMaterial>();
  frierenScleraMat->name = "FrierenScleraMat";
  frierenScleraMat->shader = m_pSkeletalShader;
  auto frierenScleraTex =
  reinterpret_pointer_cast<ImageResource>(rManager.getResource("FrierenSclera_Col.png"));
  frierenScleraMat->baseColor = frierenScleraTex->texture;

  auto frierenBrowMat = make_shared<PBRMaterial>();
  frierenBrowMat->name = "FrierenBrowMat";
  frierenBrowMat->shader = m_pSkeletalShader;
  frierenBrowMat->baseColor = frierenClothTex->texture;

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

  auto skeletonRes = rManager.getResource("FrierenSalsaDancingSkeleton");
  auto skeletonF = reinterpret_pointer_cast<SkeletonResource>(skeletonRes);
  skeletalMC->skeletonData = skeletonF;

  skeletalMC->currentPose.resize(skeletalMC->skeletonData->boneCount, Matrix4::IDENTITY);

  skeletalMC->m_meshBuffer = gManager.createConstantBuffer(skeletalMC->skeletonData->boneCount *
                                                           sizeof(Matrix4));

  gManager.updateConstantBuffer(skeletalMC->m_meshBuffer,
                                skeletalMC->currentPose.data(),
                                skeletalMC->skeletonData->boneCount * sizeof(Matrix4));

  auto frierenAnimator = make_shared<AnimatorComponent>();
  auto animRes = rManager.getResource("FrierenSalsaDancingAnimation");
  auto danceAnimation = reinterpret_pointer_cast<AnimationResource>(animRes);
  frierenAnimator->animations.push_back(danceAnimation);

  frierenAnimator->setCurrentAnimation(danceAnimation);
  frierenAnimator->finalTransform.resize(skeletalMC->skeletonData->boneCount,
                                         Matrix4::IDENTITY);

  auto sponzaRes = rManager.getResource("sponza.obj");
  auto sponzaUnion = reinterpret_pointer_cast<StaticMeshUnionResource>(sponzaRes);

  for (auto& spzResMat : sponzaUnion->materials) {
    auto sponzaMat = reinterpret_pointer_cast<PBRMaterial>(spzResMat);
    sponzaMat->shader = m_pStaticShader;
    SPtr<Resource> spzResImg;
    
    if (sponzaMat->name == "arch") {
      spzResImg = rManager.getResource("sponza_arch_diff.tga");
    }
    else if (sponzaMat->name == "bricks") {
      spzResImg = rManager.getResource("spnza_bricks_a_diff.tga");
    }
    else if (sponzaMat->name == "ceiling") {
      spzResImg = rManager.getResource("sponza_ceiling_a_diff.tga");
    }
    else if (sponzaMat->name == "chain") {
      spzResImg = rManager.getResource("chain_texture.tga");
    }
    else if (sponzaMat->name == "column_a") {
      spzResImg = rManager.getResource("sponza_column_a_diff.tga");
    }
    else if (sponzaMat->name == "column_b") {
      spzResImg = rManager.getResource("sponza_column_b_diff.tga");
    }
    else if (sponzaMat->name == "column_c") {
      spzResImg = rManager.getResource("sponza_column_c_diff.tga");
    }
    else if (sponzaMat->name == "details") {
      spzResImg = rManager.getResource("sponza_details_diff.tga");
    }
    else if (sponzaMat->name == "fabric_a") {
      spzResImg = rManager.getResource("sponza_fabric_diff.tga");
    }
    else if (sponzaMat->name == "fabric_c") {
      spzResImg = rManager.getResource("sponza_curtain_diff.tga");
    }
    else if (sponzaMat->name == "fabric_d") {
      spzResImg = rManager.getResource("sponza_fabric_blue_diff.tga");
    }
    else if (sponzaMat->name == "fabric_e") {
      spzResImg = rManager.getResource("sponza_fabric_green_diff.tga");
    }
    else if (sponzaMat->name == "fabric_f") {
      spzResImg = rManager.getResource("sponza_curtain_green_diff.tga");
    }
    else if (sponzaMat->name == "fabric_g") {
      spzResImg = rManager.getResource("sponza_curtain_blue_diff.tga");
    }
    else if (sponzaMat->name == "flagpole") {
      spzResImg = rManager.getResource("sponza_flagpole_diff.tga");
    }
    else if (sponzaMat->name == "floor") {
      spzResImg = rManager.getResource("sponza_floor_a_diff.tga");
    }
    else if (sponzaMat->name == "leaf") {
      spzResImg = rManager.getResource("sponza_thorn_diff.tga");
    }
    else if (sponzaMat->name == "Material__25") {
      spzResImg = rManager.getResource("lion.tga");
    }
    else if (sponzaMat->name == "Material__47") {
      spzResImg = rManager.getResource("White.png");
    }
    else if (sponzaMat->name == "Material__57") {
      spzResImg = rManager.getResource("vase_plant.tga");
    }
    else if (sponzaMat->name == "Material__298") {
      spzResImg = rManager.getResource("background.tga");
    }
    else if (sponzaMat->name == "roof") {
      spzResImg = rManager.getResource("sponza_roof_diff.tga");
    }
    else if (sponzaMat->name == "vase") {
      spzResImg = rManager.getResource("vase_dif.tga");
    }
    else if (sponzaMat->name == "vase_hanging") {
      spzResImg = rManager.getResource("vase_hanging.tga");
    }
    else if (sponzaMat->name == "vase_round") {
      spzResImg = rManager.getResource("vase_round.tga");
    }

    if (spzResImg != nullptr) {
      auto spzBC = reinterpret_pointer_cast<ImageResource>(spzResImg);
      sponzaMat->baseColor = spzBC->texture;
    }
    else {
      spzResImg = rManager.getResource("White.png");
      auto spzBC = reinterpret_pointer_cast<ImageResource>(spzResImg);
      sponzaMat->baseColor = spzBC->texture;
    }
  }

  for (auto& spzMeshRes : sponzaUnion->meshes) {
    auto sponzaComponent = make_shared<StaticMeshComponent>();
    sponzaComponent->meshData = spzMeshRes;

    auto sponzaGO = make_shared<GameObject>();
    sponzaGO->name = sponzaComponent->meshData->getName();
    sponzaGO->addComponent(sponzaComponent);

    for (auto& vertex : spzMeshRes->vertices) {
      vertex.position.x *= 0.01f;
      vertex.position.y *= 0.01f;
      vertex.position.z *= 0.01f;
    }

    m_scene.addObject(sponzaGO);
  }

  chestGO->addComponent(smComponent);
  frierenObject->addComponent(skeletalMC);
  frierenObject->addComponent(frierenAnimator);

  auto chestTransform = reinterpret_pointer_cast<TransformComponent>(chestGO->components[0]);
  Vector3 rotGO(180.0f, 90.0f, 0.0f);
  chestTransform->getTransform().setRotation(rotGO);
  
  for (auto& vertex : chestResource->vertices) {
    vertex.position = vertex.position.rotateX(90.0f * Math::DEG2RAD);
    vertex.position = vertex.position.rotateZ(180.0f * Math::DEG2RAD);

    vertex.position.x *= 0.3f;
    vertex.position.y *= 0.3f;
    vertex.position.z *= 0.3f;

    vertex.position.x += 10.0f;
  }

  m_scene.addObject(chestGO);
  m_scene.addObject(frierenObject);

  // Create Buffers for static meshes.
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
