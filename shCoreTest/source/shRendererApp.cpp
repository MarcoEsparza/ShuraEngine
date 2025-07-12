/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/19
*  @brief   App for render testing.
*
*  App for render testing.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRendererApp.h"

#include "shGraphicsManager.h"
#include "shRenderManager.h"
#include "shResourceManager.h"
#include "shAudioManager.h"
#include "shTime.h"
#include "shSceneGraph.h"
#include "shMath.h"
#include "shLogger.h"
#include "imgui_impl_shura.h"
#include "shStringID.h"

#include "shPath.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMaterial.h"
#include "shSkyBoxComponent.h"

#include "shRadian.h"
#include "shVector4.h"

#include "shSound.h"

namespace shEngineSDK {
void
RendererApp::onCreate()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  AudioManager& audioMan = AudioManager::instance();

  m_shadowTexSize = 2048.0f;
  m_screenSize = Vector2(static_cast<float>(getScreenDescription().width),
                         static_cast<float>(getScreenDescription().height));
  m_minR = 0.0f;
  m_maxR = 255.0f;
  m_minG = 0.0f;
  m_maxG = 255.0f;
  m_minB = 0.0f;
  m_maxB = 255.0f;

  // Initialize graphics
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  renderMan.createPasses();
  renderMan.setScreenSize(m_screenSize);
  renderMan.setShadowMapSize(m_shadowTexSize);
  renderMan.createRenderTextures();
  
  m_camera = Camera(Vector3(0.0f, 0.0f, -3.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    30.0f * Math::DEG2RAD,
                    m_screenSize.x,
                    m_screenSize.y,
                    0.1f,
                    2000.0f);

  // Imgui initialize
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());
  ImGui::StyleColorsDark();

  // Load images
  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  // Load resources
  loadPistol();
  loadSponza();
  loadSkybox();

  // Initialize light orthographic camera
  initLightCamera();

  // Set light buffer
  Vector<Vector4> lights;
  lights.resize(12);
  m_lightPos = { 0.0f, 500.0f, 0.0f, 1.0f };
  lights[0] = m_lightPos;
  m_pLightBuffer = graphMan.createConstantBuffer(sizeof(lights));
  graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

  m_aoSamplerRad = 1.0f;
  m_aoScale = 1.0f;
  m_aoBias = 0.01f;
  m_aoIntensity = 1.0f;
  m_toneMapIndex = 0;
  m_whitePt = 1.0f;
  m_bloomMultiplier = 1.0f;
  m_brightT = 1.0f;
  updateShaderDataBuffer();
  updateMainBuffer();

  auto& pMainBuffer = renderMan.getMainBuffer();
  auto& pShaderDataBuffer = renderMan.getShaderDataBuffer();

  // GBuffer constant buffers
  auto pBasicShader = renderMan.getPass("GBufferShader");
  pBasicShader->addVSConstantBuffer(pMainBuffer, 0);
  pBasicShader->addVSConstantBuffer(pShaderDataBuffer, 1);
  // Shadow shader buffers
  auto pSMapShader = renderMan.getPass("SMapShader");
  pSMapShader->addCSConstantBuffer(pMainBuffer, 0);
  pSMapShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  pSMapShader->addVSConstantBuffer(m_pLCBuffer, 3);
  // Skybox shader buffers
  auto pSkyBoxShader = renderMan.getPass("SkyBoxShader");
  pSkyBoxShader->addVSConstantBuffer(pMainBuffer, 0);
  // Lightning shader buffers
  auto pLightCS = renderMan.getPass("LightCS");
  pLightCS->addCSConstantBuffer(pMainBuffer, 0);
  pLightCS->addCSConstantBuffer(pShaderDataBuffer, 1);
  pLightCS->addCSConstantBuffer(m_pLightBuffer, 2);
  pLightCS->addCSConstantBuffer(m_pLCBuffer, 3);
  // Ambient occlusion buffers
  auto pAOShader = renderMan.getPass("AOShader");
  pAOShader->addPSConstantBuffer(pMainBuffer, 0);
  pAOShader->addPSConstantBuffer(pShaderDataBuffer, 1);
  // Post process buffers
  auto pPPShader = renderMan.getPass("PPShader");
  pPPShader->addCSConstantBuffer(pMainBuffer, 0);
  pPPShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  // Tone map buffers
  auto pToneMapShader = renderMan.getPass("ToneMapShader");
  pToneMapShader->addCSConstantBuffer(pMainBuffer, 0);
  pToneMapShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  // Luminance buffers
  auto pLuminanceShader = renderMan.getPass("LuminanceShader");
  pLuminanceShader->addCSConstantBuffer(pMainBuffer, 0);
  pLuminanceShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  // Bright buffers
  auto pBrightShader = renderMan.getPass("BrightShader");
  pBrightShader->addCSConstantBuffer(pMainBuffer, 0);
  pBrightShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  // AddMix buffers
  auto pAddMix = renderMan.getPass("AddMixShader");
  pAddMix->addCSConstantBuffer(pMainBuffer, 0);
  pAddMix->addCSConstantBuffer(pShaderDataBuffer, 1);
  // Blur buffers
  auto pHBlurShader = renderMan.getPass("HBlurShader");
  auto pVBlurShader = renderMan.getPass("VBlurShader");
  pHBlurShader->addCSConstantBuffer(pMainBuffer, 0);
  pHBlurShader->addCSConstantBuffer(pShaderDataBuffer, 1);
  pVBlurShader->addCSConstantBuffer(pMainBuffer, 0);
  pVBlurShader->addCSConstantBuffer(pShaderDataBuffer, 1);

  // Blur buffers
  auto pHBlurCS = renderMan.getPass("HBlurCS");
  auto pVBlurCS = renderMan.getPass("VBlurCS");
  pHBlurCS->addCSConstantBuffer(pMainBuffer, 0);
  pHBlurCS->addCSConstantBuffer(pShaderDataBuffer, 1);
  pVBlurCS->addCSConstantBuffer(pMainBuffer, 0);
  pVBlurCS->addCSConstantBuffer(pShaderDataBuffer, 1);

  // Final shader buffers
  auto pFinalShader = renderMan.getPass("FinalShader");
  pFinalShader->addPSConstantBuffer(pMainBuffer, 0);
  // Histogram
  auto pHistogramShader = renderMan.getPass("HistogramShader");
  pHistogramShader->addCSConstantBuffer(pMainBuffer, 0);
  // Add skybox
  auto pASBShader = renderMan.getPass("ASBShader");
  pASBShader->addCSConstantBuffer(pMainBuffer, 0);

  // Create audio
  Path audioPath("resources/cat.wav");
  m_testSound = audioMan.createSound(audioPath);
  if (m_testSound) {
    m_testSound->m_channel = CHANNEL_TYPE::kUI;
  }
}

void
RendererApp::onUpdate()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  AudioManager& audioMan = AudioManager::instance();
  SceneGraph& scene = g_sceneGraph();
  Time& time = g_time();

  m_fpsTimer += time.getFrameDeltaTime();

  // Update imgui
  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();
  ImGui_ImplShura_AddMouseWheelEvent(m_hdelta, m_delta);
  m_delta = 0.0f;
  m_hdelta = 0.0f;

  if (scene.getGameObjectList().size() && m_sceneIndex >= 0) {
    m_pModel = scene.getGameObjectList()[m_sceneIndex];
  }
  if (m_pModel) {
    m_modelPos = m_pModel->getPosition();
    m_modelRot = m_pModel->getRotation() * Math::RAD2DEG;
    m_modelScale = m_pModel->getScale();
  }
  
  Vector<Vector4> lights;
  lights.resize(12);
  lights[0] = m_lightPos;

  setImgui();

  if (m_fpsTimer >= 1.0f) {
    m_fpsTimer = 0.0f;
    m_fpsCount = 0;
  }
  else
  {
    ++m_fpsCount;
  }

  // Update models transform
  if (m_pModel) {
    if (m_modelPos != m_pModel->getPosition()) {
      m_pModel->setPosition(m_modelPos);
    }
    if (m_modelRot != m_pModel->getRotation()) {
      m_pModel->setRotation(m_modelRot * Math::DEG2RAD);
    }
    if (m_modelScale != m_pModel->getScale()) {
      m_pModel->setScale(m_modelScale);
    }
  }

  // Update light
  Vector3 lightTarget = m_lightCam.getTarget();
  float lcamNear = m_lightCam.getNear();
  float lcamFar = m_lightCam.getFar();
  float lcamSize = m_lightCam.getWidth();

  if (lights[0] != m_lightPos ||
      lightTarget != m_lightTarget ||
      lcamNear != m_lcamNear ||
      lcamFar != m_lcamFar ||
      lcamSize != m_lcamSize) {
    lights[0] = m_lightPos;
    graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

    m_lightCam.setPosition(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z));
    m_lightCam.setTarget(m_lightTarget);
    m_lightCam.setNear(m_lcamNear);
    m_lightCam.setFar(m_lcamFar);
    m_lightCam.setWidth(m_lcamSize);
    m_lightCam.setHeight(m_lcamSize);

    VP lcam = {};
    lcam.proj = m_lightCam.getProjection();
    lcam.view = m_lightCam.getView();
    lcam.proj.getTransposed();
    lcam.view.getTransposed();

    Vector4 camSize = { m_lcamSize, 0.0f, 0.0f, 0.0f };
    renderMan.setShadowMapSize(m_lcamSize);

    graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));
  }

  // Update camera
  if (m_bRightClick) {
    rotateCamera();
  }

  const float camSpeed = 100.0f * time.getFrameDeltaTime();
  if (m_bFoward) {
    m_camera.move(Vector3(0.0f, 0.0f, 0.1f) * camSpeed);
  }
  if (m_bLeft) {
    m_camera.move(Vector3(-0.1f, 0.0f, 0.0f) * camSpeed);
  }
  if (m_bBack) {
    m_camera.move(Vector3(0.0f, 0.0f, -0.1f) * camSpeed);
  }
  if (m_bRight) {
    m_camera.move(Vector3(0.1f, 0.0f, 0.0f) * camSpeed);
  }
  if (m_bUp) {
    m_camera.move(Vector3(0.0f, 0.1f, 0.0f) * camSpeed);
  }
  if (m_bDown) {
    m_camera.move(Vector3(0.0f, -0.1f, 0.0f) * camSpeed);
  }

  updateShaderDataBuffer();
  updateMainBuffer();

  // Update audio
  if (bIsSoundPlaying) {
    audioMan.playSound(m_testSound);
    bIsSoundPlaying = false;
  }
  audioMan.update();
}

void
RendererApp::onRender()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  graphMan.setPrimitiveTopology();
  renderMan.renderScene();
  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
}

void
RendererApp::onResize(const ResizeData& rszData)
{
  RenderManager& renderMan = g_renderMan();

  m_screenSize.x = static_cast<float>(rszData.width);
  m_screenSize.y = static_cast<float>(rszData.height);

  renderMan.setScreenSize(Vector2(m_screenSize.x, m_screenSize.y));
  renderMan.createRenderTextures();

  m_camera.setPerspectiveData(m_camera.getHalfFOV(),
                              m_screenSize.x,
                              m_screenSize.y,
                              m_camera.getNear(),
                              m_camera.getFar());

  Vector4 viewport(m_screenSize.x,
                   m_screenSize.y,
                   m_camera.getFar(),
                   m_camera.getNear());

  ImGui_ImplShura_Resize(m_screenSize);
}

void
RendererApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kW) {
    m_bFoward = true;
  }

  if (key == KEY::kA) {
    m_bLeft = true;
  }

  if (key == KEY::kS) {
    m_bBack = true;
  }

  if (key == KEY::kD) {
    m_bRight = true;
  }

  if (key == KEY::kQ) {
    m_bDown = true;
  }

  if (key == KEY::kE) {
    m_bUp = true;
  }

  if (key == KEY::kUp) {
    m_bRotUp = true;
  }

  if (key == KEY::kDown) {
    m_bRotDown = true;
  }

  if (key == KEY::kLeft) {
    m_bRotLeft = true;
  }

  if (key == KEY::kRight) {
    m_bRotRight = true;
  }

  ImGui_ImplShura_AddKeyEvent(key, true);
}

void
RendererApp::onKeyReleased(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kW) {
    m_bFoward = false;
  }

  if (key == KEY::kA) {
    m_bLeft = false;
  }

  if (key == KEY::kS) {
    m_bBack = false;
  }

  if (key == KEY::kD) {
    m_bRight = false;
  }

  if (key == KEY::kQ) {
    m_bDown = false;
  }

  if (key == KEY::kE) {
    m_bUp = false;
  }

  if (key == KEY::kUp) {
    m_bRotUp = false;
  }

  if (key == KEY::kDown) {
    m_bRotDown = false;
  }

  if (key == KEY::kLeft) {
    m_bRotLeft = false;
  }

  if (key == KEY::kRight) {
    m_bRotRight = false;
  }

  if (key == KEY::kC) {
    g_renderMan().recompileShaders();
  }

  ImGui_ImplShura_AddKeyEvent(key, false);
}

void
RendererApp::onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                                  const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_bLeftClick = true;
  }

  if (mouseButton == MOUSE_INPUT::kRight)
  {
    m_bRightClick = true;
  }

  ImGui_ImplShura_AddMouseButtonEvent(true);
}

void
RendererApp::onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                                   const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_bLeftClick = false;
  }

  if (mouseButton == MOUSE_INPUT::kRight)
  {
    m_bRightClick = false;
  }

  ImGui_ImplShura_AddMouseButtonEvent(false);
}

void
RendererApp::onMouseMove(const MouseMoveData& mouse)
{
  m_lastMousePos = m_currentMousePos;
  m_currentMousePos.x = static_cast<float>(mouse.x);
  m_currentMousePos.y = static_cast<float>(mouse.y);

  ImGui_ImplShura_AddMousePosEvent(m_currentMousePos);
}

void
RendererApp::onMouseWheel(const double delta, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  m_delta = static_cast<float>(delta);
}

void
RendererApp::onMouseHWheel(const double delta, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  m_hdelta = static_cast<float>(delta);
}

void
RendererApp::onDestroy()
{
  //m_pMainBuffer.reset();
  //m_pShaderDataBuffer.reset();
  m_pLCBuffer.reset();
  m_pLightBuffer.reset();
  m_pModel.reset();
  ImGui_ImplShura_Shutdown();
  ImGui::DestroyContext();
}

void
RendererApp::rotateCamera()
{
  const float speed = 0.5f;

  const float dx = (m_lastMousePos.x - m_currentMousePos.x) * speed;
  const float dy = (m_lastMousePos.y - m_currentMousePos.y) * speed;

  if (m_lastMousePos.x != m_currentMousePos.x ||
      m_lastMousePos.y != m_currentMousePos.y) {
    m_camera.rotate(dx * Math::DEG2RAD, dy * Math::DEG2RAD);
  }
}

void
RendererApp::initLightCamera()
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_lightTarget = Vector3::ZERO;
  m_lcamNear = 0.1f;
  m_lcamFar = 1000.0f;
  m_lcamSize = 1000.0f;

  m_lightCam = Camera(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z),
                      m_lightTarget,
                      Vector3::UP,
                      m_lcamSize,
                      m_lcamSize,
                      m_lcamNear,
                      m_lcamFar);

  VP lcam = {};
  lcam.proj = m_lightCam.getProjection();
  lcam.view = m_lightCam.getView();
  lcam.proj.getTransposed();
  lcam.view.getTransposed();

  m_pLCBuffer = graphMan.createConstantBuffer(sizeof(VP));
  graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));
}

void
RendererApp::updateMainBuffer()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  Time& time = g_time();

  auto& pMainBuffer = renderMan.getMainBuffer();
  auto& mbd = renderMan.getMainBufferData();
  mbd.viewMatrix = m_camera.getView();
  mbd.transposeViewMatrix = m_camera.getView();
  mbd.transposeViewMatrix.getTransposed();
  mbd.inverseViewMatrix = m_camera.getView().getInversed();
  mbd.inverseTransposeViewMatrix = mbd.inverseViewMatrix * mbd.transposeViewMatrix;

  mbd.projectionMatrix = m_camera.getProjection();
  mbd.transposeProjectionMatrix = m_camera.getProjection();
  mbd.transposeProjectionMatrix.getTransposed();
  mbd.inverseProjectionMatrix = m_camera.getView().getInversed();
  mbd.inverseTransposeProjectionMatrix = mbd.inverseProjectionMatrix *
                                         mbd.transposeProjectionMatrix;

  mbd.screenSize = m_screenSize;
  mbd.nearPlane = m_camera.getNear();
  mbd.farPlane = m_camera.getFar();

  mbd.cameraPosition = Vector4(m_camera.getPosition(), 0.0f);
  mbd.cameraDirection = Vector4(m_camera.getTarget(), 0.0f);

  mbd.time = time.getTime();
  mbd.deltaTime = time.getFrameDeltaTime();
  mbd.cosTime = Math::cos(Radian(mbd.time));
  mbd.sinTime = Math::sin(Radian(mbd.time));

  if (!pMainBuffer) {
    pMainBuffer = graphMan.createConstantBuffer(sizeof(MainBufferData));
  }
  graphMan.updateConstantBuffer(pMainBuffer, &mbd, sizeof(MainBufferData));
}

void
RendererApp::updateShaderDataBuffer()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  auto& pShaderDataBuffer = renderMan.getShaderDataBuffer();
  auto& sd = renderMan.getShaderData();
  sd.shadowMapSize = m_shadowTexSize;
  sd.sampleRadius = m_aoSamplerRad;
  sd.aoScale = m_aoScale;
  sd.aoBias = m_aoBias;
  sd.aoIntensity = m_aoIntensity;
  sd.toneMappingIndex = static_cast<float>(m_toneMapIndex);
  sd.lutSize = 0.0f;
  sd.whitePoint = m_whitePt;
  sd.bloomMultiplier = m_bloomMultiplier;
  sd.brightThreshold = m_brightT;
  float normChannel = 1.0f / 255.0f;
  sd.minR = m_minR * normChannel;
  sd.maxR = m_maxR * normChannel;
  sd.minG = m_minG * normChannel;
  sd.maxG = m_maxG * normChannel;
  sd.minB = m_minB * normChannel;
  sd.maxB = m_maxB * normChannel;
  sd.lightIntensity = m_lightIntensity;
  sd.middleGrey = m_middleGrey;

  if (!pShaderDataBuffer) {
    pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
  }
  graphMan.updateConstantBuffer(pShaderDataBuffer, &sd, sizeof(ShaderData));
}

void
RendererApp::setImgui()
{
  //GraphicsManager& graphMan = g_graphicsMan();
  //RenderManager& renderMan = g_renderMan();
  SceneGraph& scene = g_sceneGraph();

  float width = m_screenSize.x;
  float height = m_screenSize.y;

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(width * 0.2f, height * 0.5f));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Scenegraph",
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  ImGui::PopStyleColor();
  if (scene.getGameObjectList().size() > 0) {
    for (uint8 i = 0; i < scene.getGameObjectList().size(); ++i) {
      bool isSelected = (m_sceneIndex == i);
      if (ImGui::Selectable(scene.getGameObjectList()[i]->name.c_str()), isSelected) {
        m_sceneIndex = i;
      }

      if (ImGui::IsItemClicked()) {
        m_sceneIndex = i;
      }
    }
  }
  ImGui::End();

  String name = "None";
  if (scene.getGameObjectList().size() > 0 && m_sceneIndex >= 0) {
    name = scene.getGameObjectList()[m_sceneIndex]->name;
  }
  ImGui::SetNextWindowPos(ImVec2(0.0f, height * 0.5f));
  ImGui::SetNextWindowSize(ImVec2(width * 0.2f, height * 0.5f));
  ImGui::Begin(name.c_str(),
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  if (scene.getGameObjectList().size() > 0) {
    drawTransformComponent();
    if(m_pModel){
      for (uint32 i = 0; i < m_pModel->components.size(); ++i) {
        auto& pComponent = m_pModel->components[i];
        if (pComponent->getType() == COMPONENT_TYPE::kStaticMesh) {
          auto pMesh = sh_reinterpretPCast<StaticMeshComponent>(pComponent);
          if (ImGui::CollapsingHeader("Static Mesh Component")) {
            ImGui::Text("Material Count: %d",
              static_cast<uint32>(pMesh->m_mesh->m_materials.size()));
            uint32 vertexCount = 0;
            uint32 indexCount = 0;
            for (uint32 j = 0; j < pMesh->m_mesh->m_meshes.size(); ++j) {
              vertexCount += pMesh->m_mesh->m_meshes[j].numVertices;
              indexCount += pMesh->m_mesh->m_meshes[j].numIndices;
            }
            ImGui::Text("Vertex Count: %d", vertexCount);
            ImGui::Text("Index Count: %d", indexCount);

            for (int32 j = 0; j < pMesh->m_mesh->m_materials.size(); ++j) {
              auto& currentMat = pMesh->m_mesh->m_materials[j];
              String matName = "Material:" + currentMat->name;
              //ImGui::Text(matName.c_str());
              if (ImGui::Button(matName.c_str())) {
                m_selectedMat = j;
                m_bTexColor = false;
              }

              if (m_selectedMat >= 0 && m_selectedMat == j) {
                //m_bTexColor = false;
                showMaterialInspector(currentMat);
              }
            }
          }
        }
      }
      /*else if (pComponent->getType() == ComponentType::kSkyBox) {
        auto pSkyBox = sh_reinterpretPCast<SkyBoxComponent>(pComponent);
        if (ImGui::CollapsingHeader("SkyBox Component")) {
          ImGui::Text("SkyBox Name: %s", pSkyBox->getSkyBoxName().c_str());
        }
      }*/
    }
  }
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(width - (width * 0.2f), 0.0f));
  ImGui::SetNextWindowSize(ImVec2(width * 0.2f, height));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Renderer Settings",
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  ImGui::PopStyleColor();

  ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(227, 187, 41, 0xff));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(247, 200, 70, 0xff));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(207, 167, 20, 0xff));
  if (ImGui::CollapsingHeader("Shaders Data")) {
    ImGui::DragFloat("AO sampler rad", &m_aoSamplerRad, 0.1f);
    ImGui::DragFloat("AO scale", &m_aoScale, 0.1f);
    ImGui::DragFloat("AO bias", &m_aoBias, 0.1f);
    ImGui::DragFloat("AO intensity", &m_aoIntensity, 0.1f);

    if (ImGui::Button("Recompile Shaders")) {
      g_renderMan().recompileShaders();
    }
  }
  ImGui::PopStyleColor(3);

  if (ImGui::CollapsingHeader("Light settings")) {
    // Light Position
    ImGui::Text("Light Pos:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##LPosX", &m_lightPos.x, 1.0f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##LPosY", &m_lightPos.y, 1.0f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##LPosZ", &m_lightPos.z, 1.0f);
    ImGui::PopStyleColor(3);

    // Light Target
    ImGui::Text("Light Target:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##LTarX", &m_lightTarget.x, 1.0f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##LTarY", &m_lightTarget.y, 1.0f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##LTarZ", &m_lightTarget.z, 1.0f);
    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::DragFloat("Light Cam Near:", &m_lcamNear, 0.1f);
    ImGui::Spacing();
    ImGui::DragFloat("Light Cam Far:", &m_lcamFar, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloat("Light Cam Size:", &m_lcamSize, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloat("Light Intensity:", &m_lightIntensity, 0.01f, 0.0f, 10.0f);
  }

  if (ImGui::Button("Play Sound")) {
    bIsSoundPlaying = true;
  }

  //auto& pComponent = scene.getGameObjectList()[0]->components[0];
  //auto pMesh = sh_reinterpretPCast<StaticMeshComponent>(pComponent);
  //auto& pTex = pMesh->m_mesh->m_materials[0]->baseColor;
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pTex), ImVec2(256, 256));

  ImGui::Combo("ToneMapping", &m_toneMapIndex, "Reinhard\0ACES\0Uncharted2\0AgX\0LUT\0");
  ImGui::Spacing();
  ImGui::DragFloat("Min R:", &m_minR, 1.0f, 0.0f, 255.0f);
  ImGui::DragFloat("Max R:", &m_maxR, 1.0f, 0.0f, 255.0f);
  ImGui::Spacing();
  ImGui::DragFloat("Min G:", &m_minG, 1.0f, 0.0f, 255.0f);
  ImGui::DragFloat("Max G:", &m_maxG, 1.0f, 0.0f, 255.0f);
  ImGui::Spacing();
  ImGui::DragFloat("Min B:", &m_minB, 1.0f, 0.0f, 255.0f);
  ImGui::DragFloat("Max B:", &m_maxB, 1.0f, 0.0f, 255.0f);
  ImGui::Spacing();

  ImGui::Spacing();
  ImGui::DragFloat("Bright Threshold:", &m_brightT, 0.01f, 0.0f, 1.0f);
  ImGui::Spacing();
  ImGui::DragFloat("White Point:", &m_whitePt, 0.01f, 0.5f, 11.2f);
  ImGui::Spacing();
  ImGui::DragFloat("Bloom Multiplier:", &m_bloomMultiplier, 0.01f, 0.5f, 2.0f);
  ImGui::Spacing();
  ImGui::DragFloat("MiddleGrey:", &m_middleGrey, 0.01f, 0.5f, 2.0f);
  ImGui::Spacing();

  if (m_fpsTimer >= 1.0f) {
    m_fpsCountGUI = m_fpsCount;
  }
  String strCount = std::to_string(m_fpsCountGUI);
  String text = strCount + ": fps";
  ImGui::Text(text.c_str());

  ImGui::End();
}

void
RendererApp::drawTransformComponent()
{
  if (ImGui::CollapsingHeader("Transform")) {
    // Position
    ImGui::Text("Position:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##PosX", &m_modelPos.x, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##PosY", &m_modelPos.y, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##PosZ", &m_modelPos.z, 0.01f);
    ImGui::PopStyleColor(3);

    // Rotation
    ImGui::Text("Rotation:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##RotX", &m_modelRot.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##RotY", &m_modelRot.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##RotZ", &m_modelRot.z, 0.1f);
    ImGui::PopStyleColor(3);

    // Scale
    ImGui::Text("Scale:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##SclX", &m_modelScale.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##SclY", &m_modelScale.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##SclZ", &m_modelScale.z, 0.1f);
    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(220, 220, 220, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 180, 180, 150));
    if (ImGui::Button("Reset")) {
      m_modelPos = { 0.0f, 0.0f, 0.0f };
      m_modelRot = { 0.0f, 0.0f, 0.0f };
      m_modelScale = { 1.0f, 1.0f, 1.0f };
    }
    ImGui::PopStyleColor(3);
  }
}

void
RendererApp::showMaterialInspector(const WPtr<Material> pMat)
{
  if (pMat.expired()) {
    return;
  }
  auto currentMat = pMat.lock();

  auto& pBaseColor = currentMat->baseColor;
  auto& pNormal = currentMat->normal;
  auto& pMetallic = currentMat->metallic;
  auto& pRoughness = currentMat->roughness;

  // Base Color
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pBaseColor), ImVec2(64, 64));
  /*if(ImGui::ImageButton("##BaseColorSelection",
                        reinterpret_cast<ImTextureID*>(&pBaseColor),
                        ImVec2(64, 64))) {

  }*/
  ImGui::SameLine();
  String buttonID = "##ColorButton" + currentMat->name;
  Vector3& baseColor = currentMat->baseColorFactor;
  if (ImGui::ColorButton(buttonID.c_str(), ImVec4(baseColor.x,
    baseColor.y,
    baseColor.z,
    1.0f)))
  {
    m_bTexColor = m_bTexColor ? false : true;
  }
  ImGui::SameLine();
  bool bHasDiffuseMap = currentMat->m_properties.bHasDiffuseMap;
  ImGui::Checkbox("BaseColor", &bHasDiffuseMap);
  currentMat->m_properties.bHasDiffuseMap = bHasDiffuseMap;

  if (m_bTexColor) {
    float texColor[3] = { baseColor.x, baseColor.y, baseColor.z };
    ImGui::Begin("Color Picker", 0, ImGuiWindowFlags_NoTitleBar);
    texColor[0] = baseColor.x;
    texColor[1] = baseColor.y;
    texColor[2] = baseColor.z;
    ImGui::ColorPicker3("TexColor", texColor);
    baseColor.x = texColor[0];
    baseColor.y = texColor[1];
    baseColor.z = texColor[2];
    ImGui::End();
  }

  // Normal
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pNormal), ImVec2(64, 64));
  ImGui::SameLine();
  bool bHasNormalMap = currentMat->m_properties.bHasNormalMap;
  ImGui::Checkbox("Normal", &bHasNormalMap);
  currentMat->m_properties.bHasNormalMap = bHasNormalMap;

  // Metallic
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pMetallic), ImVec2(64, 64));
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Metallic Factor",
                   &currentMat->metallicRoughnessFactor.x,
                   0.01f,
                   0.0f,
                   1.0f);
  ImGui::SameLine();
  bool bHasMetallicMap = currentMat->m_properties.bHasMetalnessMap;
  ImGui::Checkbox("Metallic", &bHasMetallicMap);
  currentMat->m_properties.bHasMetalnessMap = bHasMetallicMap;

  // Roughness
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pRoughness), ImVec2(64, 64));
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Roughness Factor",
                   &currentMat->metallicRoughnessFactor.y,
                   0.01f,
                   0.0f,
                   1.0f);
  ImGui::SameLine();
  bool bHasRoughnessMap = currentMat->m_properties.bHasRoughnessMap;
  ImGui::Checkbox("Roughness", &bHasRoughnessMap);
  currentMat->m_properties.bHasRoughnessMap = bHasRoughnessMap;
}

void
RendererApp::loadPistol()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = sh_reinterpretPCast<StaticMeshResource>(
                  resourceMan.loadModelFromCache("resources/assets/models/DrakeFire.sha"));

  auto model = sh_makeShared<GameObject>();
  model->name = "DrakeFire";
  auto modelMC = sh_makeShared<StaticMeshComponent>();
  modelRes->m_materials[0]->m_properties.bHasRoughnessMap = true;

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 5.0f);

  sceneG.addObject(model);
}

void
RendererApp::loadSponza()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto sponzaModelRes = sh_reinterpretPCast<StaticMeshResource>(
                        resourceMan.loadModelFromCache("resources/assets/models/Sponza.sha"));

  auto model = sh_makeShared<GameObject>();
  model->name = "Sponza";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  sponzaModelRes->m_materials[0]->m_properties.bHasAlphaTest = true;
  sponzaModelRes->m_materials[1]->m_properties.bHasAlphaTest = true;
  sponzaModelRes->m_materials[20]->m_properties.bHasAlphaTest = true;

  modelMC->setMeshData(sponzaModelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 0.25f);

  sceneG.addObject(model);
}

void
RendererApp::loadSkybox()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& scene = g_sceneGraph();

  auto skyboxTx = sh_reinterpretPCast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/skybox1.png")));

  Vector<Vector3> vertices = { {-1.0f, -1.0f, -1.0f},
                               {1.0f, -1.0f, -1.0f},
                               {1.0f, 1.0f, -1.0f},
                               {-1.0f, 1.0f, -1.0f},
                               {-1.0f, -1.0f, 1.0f},
                               {1.0f, -1.0f, 1.0f},
                               {1.0f, 1.0f, 1.0f},
                               {-1.0f, 1.0f, 1.0f} };

  Vector<uint32> indices = { // front
                             0, 1, 2,
                             2, 3, 0,
                             // right
                             1, 5, 6,
                             6, 2, 1,
                             // back
                             7, 6, 5,
                             5, 4, 7,
                             // left
                             4, 0, 3,
                             3, 7, 4,
                             // top
                             3, 2, 6,
                             6, 7, 3,
                             // bottom
                             4, 5, 1,
                             1, 0, 4 };

  auto pSkyBox = sh_makeShared<SkyBoxComponent>();
  pSkyBox->setVertices(vertices);
  pSkyBox->setIndices(indices);

  auto pSkyBoxMat = sh_makeShared<Material>();
  pSkyBoxMat->m_properties.bHasDiffuseMap = true;
  pSkyBoxMat->baseColor = skyboxTx->texture;
  pSkyBox->setMaterial(pSkyBoxMat);
  
  auto pSkyBoxGO = sh_makeShared<GameObject>();
  pSkyBoxGO->addComponent(pSkyBox);
  pSkyBoxGO->name = "SkyBox";

  scene.addObject(pSkyBoxGO);
}
}
