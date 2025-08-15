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

#include <shPhysicsManager.h>

#include "shPath.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMaterial.h"
#include "shSkyBoxComponent.h"
#include "shColliderComponent.h"
#include "shGizmos.h"

#include "shRadian.h"
#include "shVector4.h"

//#include <shRigidbody.h>
#include <shRigidbodyComponent.h>

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

  // Initialize graphics
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  renderMan.createPasses();
  renderMan.setScreenSize(m_screenSize);
  renderMan.setShadowMapSize(m_shadowTexSize);
  renderMan.createRenderTextures();
  renderMan.computeBRDF();
  
  
  m_camera = Camera(Vector3(0.0f, 0.0f, -3.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    30.0f * Math::DEG2RAD,
                    m_screenSize.x,
                    m_screenSize.y,
                    0.1f,
                    2000.0f);

  // Imgui initialize
  m_gui.init(getScreen());

  // Load resources
  //loadPistol();
  //loadSponza();
  loadSkybox();
  //loadCoat();
  tempLoad();

  // Initialize light orthographic camera
  initLightCamera();

  // Set light buffer
  Vector<Vector4> lights;
  lights.resize(12);
  m_lightPos = { 0.0f, 100.0f, 0.0f, 1.0f };
  lights[0] = m_lightPos;
  m_pLightBuffer = graphMan.createConstantBuffer(sizeof(lights));
  graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

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
  pSkyBoxShader->addCSConstantBuffer(pMainBuffer, 0);
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

  auto pSPCubeMap = renderMan.getPass("SpecularPreMapShader");
  pSPCubeMap->addCSConstantBuffer(pShaderDataBuffer, 1);

  // Final shader buffers
  auto pFinalShader = renderMan.getPass("FinalShader");
  pFinalShader->addPSConstantBuffer(pMainBuffer, 0);
  // Histogram
  auto pHistogramShader = renderMan.getPass("HistogramShader");
  pHistogramShader->addCSConstantBuffer(pMainBuffer, 0);
  // Add skybox
  auto pASBShader = renderMan.getPass("ASBShader");
  pASBShader->addCSConstantBuffer(pMainBuffer, 0);

  renderMan.computeIBL();

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
  Time& time = g_time();

  m_fpsTimer += time.getFrameDeltaTime();

  // Update imgui

  if (m_fpsTimer >= 1.0f) {
    m_gui.m_fpsCountGUI = m_fpsCount;
  }
  m_gui.update();
  m_camera.setHalfFOV(m_gui.m_camFov * Math::DEG2RAD);
  
  Vector<Vector4> lights;
  lights.resize(12);
  lights[0] = m_lightPos;

  if (m_fpsTimer >= 1.0f) {
    m_fpsTimer = 0.0f;
    m_fpsCount = 0;
  }
  else
  {
    ++m_fpsCount;
  }

  // Update light
  Vector3 lightTarget = m_lightCam.getTarget();
  float lcamNear = m_lightCam.getNear();
  float lcamFar = m_lightCam.getFar();
  float lcamSize = m_lightCam.getWidth();

  if (lights[0] != m_gui.m_lightPos ||
      lightTarget != m_gui.m_lightTarget ||
      lcamNear != m_gui.m_lcamNear ||
      lcamFar != m_gui.m_lcamFar ||
      lcamSize != m_gui.m_lcamSize) {
    lights[0] = m_gui.m_lightPos;
    graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

    m_lightCam.setPosition(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z));
    m_lightCam.setTarget(m_gui.m_lightTarget);
    m_lightCam.setNear(m_gui.m_lcamNear);
    m_lightCam.setFar(m_gui.m_lcamFar);
    m_lightCam.setWidth(m_gui.m_lcamSize);
    m_lightCam.setHeight(m_gui.m_lcamSize);

    VP lcam = {};
    lcam.proj = m_lightCam.getProjection();
    lcam.view = m_lightCam.getView();
    lcam.proj.getTransposed();
    lcam.view.getTransposed();

    Vector4 camSize = { m_gui.m_lcamSize, 0.0f, 0.0f, 0.0f };
    renderMan.setShadowMapSize(m_gui.m_lcamSize);

    graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));
  }

  // Update camera
  if (m_bRightClick) {
    rotateCamera();
  }

  const float camSpeed = m_gui.m_camSpeed * time.getFrameDeltaTime();
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

  updateMainBuffer();

  // Update audio
  if (bIsSoundPlaying) {
    audioMan.playSound(m_testSound);
    bIsSoundPlaying = false;
  }
  audioMan.update();

  m_delta = 0.0f;
  m_hdelta = 0.0f;
  m_lastMousePos = m_currentMousePos;

  if(m_gui.m_bPlay != m_bPlayScene) {
    m_bPlayScene = m_gui.m_bPlay;

    if (m_bPlayScene) {
      playScene();
    }
    else {
      restartScene();
    }
  }
}

void
RendererApp::onFixedUpdate()
{
  SceneGraph& scene = g_sceneGraph();
  PhysicsManager& physicsMan = g_physicsMan();

  if (m_bPlayScene) {
    Vector<Rigidbody*> rigidbodies;
    for (auto& gameObject : scene.getGameObjectList()) {
      for (auto& component : gameObject->components) {
        if (component->getType() == COMPONENT_TYPE::kRigidbody) {
          auto rb = cast::rePointer<RigidbodyComponent>(component);
          rigidbodies.push_back(&rb->m_rigidbody);
        }
      }
    }
    physicsMan.onUpdate(rigidbodies);

    for (auto& gameObject : scene.getGameObjectList()) {
      for (auto& component : gameObject->components) {
        if (component->getType() == COMPONENT_TYPE::kRigidbody) {
          auto rb = cast::rePointer<RigidbodyComponent>(component);
          gameObject->move(rb->m_rigidbody.m_position);
          gameObject->setRotation(rb->m_rigidbody.m_rotation.toEulerAngles());
        }
      }
    }
  }
}

void
RendererApp::onRender()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  Gizmos& gizmos = Gizmos::instance();

  graphMan.setPrimitiveTopology();
  renderMan.renderScene();
  gizmos.drawGizmos(m_camera);
  m_gui.render();
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

  ImGui_ImplShura_AddKeyEvent(key, true, modifier);
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

  ImGui_ImplShura_AddKeyEvent(key, false, modifier);
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

  ImGui_ImplShura_AddMouseButtonEvent(true, mouseButton);
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

  ImGui_ImplShura_AddMouseButtonEvent(false, mouseButton);
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
  ImGui_ImplShura_AddMouseWheelEvent(m_hdelta, m_delta);
}

void
RendererApp::onMouseHWheel(const double delta, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  m_hdelta = static_cast<float>(delta);
  ImGui_ImplShura_AddMouseWheelEvent(m_hdelta, m_delta);
}

void
RendererApp::onDestroy()
{
  m_pLCBuffer.reset();
  m_pLightBuffer.reset();
  m_gui.shutdown();
}

void
RendererApp::rotateCamera()
{
  const float speed = 0.5f;

  const float dx = (m_lastMousePos.x - m_currentMousePos.x) * speed;
  const float dy = (m_lastMousePos.y - m_currentMousePos.y) * speed;

  if (Math::abs(dx) > 0.0f || Math::abs(dy) > 0.0f) {
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
  mbd.transposeViewMatrix = m_camera.getView().getTransposed();
  mbd.inverseViewMatrix = m_camera.getView().getInversed();
  mbd.inverseTransposeViewMatrix = mbd.inverseViewMatrix * mbd.transposeViewMatrix;

  mbd.projectionMatrix = m_camera.getProjection();
  mbd.transposeProjectionMatrix = m_camera.getProjection().getTransposed();
  mbd.inverseProjectionMatrix = m_camera.getView().getInversed();
  mbd.inverseTransposeProjectionMatrix = mbd.inverseProjectionMatrix *
                                         mbd.transposeProjectionMatrix;

  mbd.inverseViewProjMatrix = (mbd.viewMatrix * mbd.projectionMatrix).getInversed();
  mbd.inverseTransposeViewProjMatrix = mbd.inverseViewProjMatrix.getTransposed();

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
RendererApp::setImgui()
{
  //GraphicsManager& graphMan = g_graphicsMan();
  //RenderManager& renderMan = g_renderMan();
  //SceneGraph& scene = g_sceneGraph();
  //
  //if (ImGui::Button("Play Sound")) {
  //  bIsSoundPlaying = true;
  //}
}

void
RendererApp::loadPistol()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = cast::rePointer<StaticMeshResource>(
                  resMan.loadModelFromCache("resources/assets/models/DrakeFire.sha"));

  auto model = sh_makeShared<GameObject>();
  model->name = "DrakeFire";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 5.0f);

  auto pCollider = sh_makeShared<ColliderComponent>();
  pCollider->m_collider.m_type = COLLIDER_TYPE::kOBBox;
  pCollider->m_collider.m_box.center = Vector3::ZERO;
  pCollider->m_collider.m_box.extent = Vector3(0.5f, 0.5f, 0.5f);
  pCollider->m_collider.m_box.rotation = Quaternion::IDENTITY;
  //pCollider->m_collider.m_sphere.center = Vector3::ZERO;
  //pCollider->m_collider.m_sphere.radius = 0.5f;
  //pCollider->m_collider.m_capsule.center = Vector3::ZERO;
  //pCollider->m_collider.m_capsule.radius = 0.5f;
  //pCollider->m_collider.m_capsule.height = 1.0f;
  //pCollider->m_collider.m_capsule.direction = Vector3::ZERO;

  auto pRigidbody = sh_makeShared<RigidbodyComponent>();
  pRigidbody->m_rigidbody.m_mass = 1.0f;
  pRigidbody->m_rigidbody.m_integrationType = INTEGRATION::kVerlet;
  pRigidbody->m_rigidbody.m_colliderType = COLLIDER_TYPE::kOBBox;
  pRigidbody->m_rigidbody.m_dragCoefficent = 0.1f;
  pRigidbody->m_rigidbody.m_elasticity = 0.0f;
  pRigidbody->m_rigidbody.m_friction = 0.5f;
  pRigidbody->m_rigidbody.m_gravityScale = 1.0f;
  pRigidbody->m_rigidbody.m_position = model->transform.getPosition();
  pRigidbody->m_rigidbody.m_rotation = Quaternion(model->transform.getRotation());

  model->addComponent(pCollider);
  model->addComponent(pRigidbody);

  sceneG.addObject(model);
}

void
RendererApp::loadSponza()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto sponzaModelRes = cast::rePointer<StaticMeshResource>(
                        resMan.loadModelFromCache("resources/assets/models/Sponza.sha"));

  auto model = sh_makeShared<GameObject>();
  model->name = "Sponza";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(sponzaModelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 0.25f);

  auto pCollider = sh_makeShared<ColliderComponent>();
  pCollider->m_collider.m_type = COLLIDER_TYPE::kOBBox;
  pCollider->m_collider.m_box.center = Vector3::ZERO;
  pCollider->m_collider.m_box.extent = Vector3(50.0f, 5.0f, 50.0f);
  pCollider->m_collider.m_box.rotation = Quaternion::IDENTITY;

  auto pRigidbody = sh_makeShared<RigidbodyComponent>();
  pRigidbody->m_rigidbody.m_mass = 1.0f;
  pRigidbody->m_rigidbody.m_integrationType = INTEGRATION::kVerlet;
  pRigidbody->m_rigidbody.m_colliderType = COLLIDER_TYPE::kOBBox;
  pRigidbody->m_rigidbody.m_dragCoefficent = 0.1f;
  pRigidbody->m_rigidbody.m_elasticity = 0.0f;
  pRigidbody->m_rigidbody.m_friction = 0.5f;
  pRigidbody->m_rigidbody.m_gravityScale = 0.0f;
  pRigidbody->m_rigidbody.m_position = model->transform.getPosition();
  pRigidbody->m_rigidbody.m_rotation = Quaternion(model->transform.getRotation());

  model->addComponent(pCollider);
  model->addComponent(pRigidbody);

  sceneG.addObject(model);
}

void
RendererApp::loadSkybox()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& scene = g_sceneGraph();

  auto pTex = resMan.loadResourceFromFile(Path("resources/textures/shanghai_bund_4k.hdr"));
  auto skyboxTx = cast::rePointer<ImageResource>(pTex);

  auto pSkyBox = sh_makeShared<SkyBoxComponent>();
  pSkyBox->setSkyBoxResource(skyboxTx);
  
  auto pSkyBoxGO = sh_makeShared<GameObject>();
  pSkyBoxGO->addComponent(pSkyBox);
  pSkyBoxGO->name = "SkyBox";

  scene.addObject(pSkyBoxGO);
}

void
RendererApp::loadCoat()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& scene = g_sceneGraph();

  auto modelRes = cast::rePointer<StaticMeshResource>(
    resMan.loadResourceFromFile(Path("resources/models/export3dcoat.obj")));

  Path colorPath("resources/textures/export3dcoat_lambert3SG_color.tga");
  Path glossPath("resources/textures/export3dcoat_lambert3SG_gloss.tga");
  Path metalPath("resources/textures/export3dcoat_lambert3SG_metalness.tga");
  Path normalPath("resources/textures/Emmisive_Eye_Class_Normal.png");

  auto pAlbedo = cast::rePointer<ImageResource>(resMan.loadResourceFromFile(colorPath));
  auto pGloss = cast::rePointer<ImageResource>(resMan.loadResourceFromFile(glossPath));
  auto pMetal = cast::rePointer<ImageResource>(resMan.loadResourceFromFile(metalPath));
  auto pNormal = cast::rePointer<ImageResource>(resMan.loadResourceFromFile(normalPath));

  auto& pMat = modelRes->m_materials[0];
  pMat->baseColor = pAlbedo->texture;
  pMat->metallic = pMetal->texture;
  pMat->normal = pNormal->texture;
  pMat->roughness = pGloss->texture;

  pMat->m_properties.bHasDiffuseMap = true;
  pMat->m_properties.bHasMetalnessMap = true;
  pMat->m_properties.bHasNormalMap = true;
  pMat->m_properties.bHasRoughnessMap = true;
  pMat->m_properties.bInvertRoughness = true;

  modelRes->m_meshes[0].materialIndex = 0;

  auto model = sh_makeShared<GameObject>();
  model->name = "CoatBall";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;

  scene.addObject(model);
}

void
RendererApp::tempLoad()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = cast::rePointer<StaticMeshResource>(
    resourceMan.loadResourceFromFile(Path("resources/models/scene.gltf")));

  auto model = sh_makeShared<GameObject>();
  model->name = "Spartan";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  model->transform.getTransform() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE);

  sceneG.addObject(model);
}

void
RendererApp::playScene()
{
  SceneGraph& scene = g_sceneGraph();
  m_tempGameObjects = scene.getGameObjectList();
}

void
RendererApp::restartScene()
{
  SceneGraph& scene = g_sceneGraph();
  scene.getGameObjectList() = m_tempGameObjects;
}
}
