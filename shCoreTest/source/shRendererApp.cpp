/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/11/14
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

#include <shDynamicLibraryManager.h>
#include <shGraphicsManager.h>
#include <shRenderManager.h>
#include <shShaderManager.h>
#include <shResourceManager.h>
#include <shPhysicsManager.h>
#include <shAudioManager.h>
#include <shTime.h>
#include <shSceneGraph.h>
#include <shMath.h>
#include <shLogger.h>
#include "imgui_impl_shura.h"

#include <shStringID.h>
#include <shPath.h>
#include <shRadian.h>
#include <shVector4.h>
#include <shVector2i.h>

#include <shGameObject.h>
#include <shGizmos.h>
#include <shMaterial.h>
#include <shImageResource.h>
#include <shMeshResource.h>
#include <shSkeletonResource.h>
#include <shMeshComponent.h>
#include <shSkyBoxComponent.h>
#include <shColliderComponent.h>
#include <shAnimatorComponent.h>
#include <shLightComponent.h>
#include <shRigidbodyComponent.h>

#include <shSound.h>
#include <shTimer.h>

using DirectoryIterator = std::filesystem::directory_iterator;

namespace shEngineSDK {
void
RendererApp::onCreate()
{
  //GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  ShaderManager& shaderMan = g_shaderMan();
  //AudioManager& audioMan = AudioManager::instance();
  SceneGraph& scene = g_sceneGraph();

  m_shadowTexSize = 2048.0f;
  auto& pScreen = getScreen();
  m_screenSize = Vector2(cast::st<float>(pScreen->getClientSize().x),
                         cast::st<float>(pScreen->getClientSize().y));

  // Initialize graphics
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  shaderMan.createPipelinePasses();
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
  scene.createDefaultScene();

  updateMainBuffer();
  shaderMan.updateShaderDataCB();

  renderMan.computeIBL();

  // Create audio
  /*Path audioPath("resources/cat.wav");
  m_testSound = audioMan.createSound(audioPath);
  if (m_testSound) {
    m_testSound->m_channel = CHANNEL_TYPE::kUI;
  }*/

  //loadMods();
}

void
RendererApp::onUpdate()
{
  //AudioManager& audioMan = AudioManager::instance();
  Time& time = g_time();
  //SceneGraph& scene = g_sceneGraph();

  m_fpsTimer += time.getFrameDeltaTime();

  // Update imgui

  if (m_fpsTimer >= 1.0f) {
    m_gui.m_fpsCountGUI = m_fpsCount;
  }
  m_gui.update();
  m_sceneSize = m_gui.m_sceneWindowSize;
  m_camera.setHalfFOV(m_gui.m_camFov * Math::DEG2RAD);
  m_camera.setNear(m_gui.m_camNear);
  m_camera.setFar(m_gui.m_camFar);
  
  if (m_fpsTimer >= 1.0f) {
    m_fpsTimer = 0.0f;
    m_fpsCount = 0;
  }
  else
  {
    ++m_fpsCount;
  }

  // Update camera
  if (m_gui.m_bSceneWindowFocused) {
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
  }

  updateMainBuffer();

  //scene.update();

  // Update audio
  /*if (bIsSoundPlaying) {
    audioMan.playSound(m_testSound);
    bIsSoundPlaying = false;
  }
  audioMan.update();*/

  m_delta = 0.0f;
  m_hdelta = 0.0f;
  m_lastMousePos = m_currentMousePos;

  /*if(m_gui.m_bPlay != m_bPlayScene) {
    m_bPlayScene = m_gui.m_bPlay;

    if (m_bPlayScene) {
      playScene();
    }
    else {
      restartScene();
    }
  }*/

  /*if(m_gui.m_bReloadMods) {
    loadMods();
    m_gui.m_bReloadMods = false;
  }*/
}

void
RendererApp::onFixedUpdate()
{
  SceneGraph& scene = g_sceneGraph();
  PhysicsManager& physicsMan = g_physicsMan();

  if (m_bPlayScene) {
    Vector<Rigidbody*> rigidbodies;
    for (auto& gameObject : scene.getGameObjectList()) {
      for (auto& component : gameObject->m_componentList) {
        if (component->getType() == COMPONENT_TYPE::kRigidbody) {
          auto rb = cast::re_ptr<RigidbodyComponent>(component);
          rigidbodies.push_back(&rb->m_rigidbody);
        }
      }
    }
    physicsMan.onUpdate(rigidbodies);

    for (auto& gameObject : scene.getGameObjectList()) {
      for (auto& component : gameObject->m_componentList) {
        if (component->getType() == COMPONENT_TYPE::kRigidbody) {
          auto rb = cast::re_ptr<RigidbodyComponent>(component);
          gameObject->move(rb->m_rigidbody.m_position);
          gameObject->setRotation(rb->m_rigidbody.m_rotation);
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
  //Gizmos& gizmos = Gizmos::instance();

  graphMan.setPrimitiveTopology();
  renderMan.renderScene();
  //gizmos.drawGizmos(m_camera);
  m_gui.render();
  //renderMan.cleanShaderObjects();
  graphMan.unbindAll();
}

void
RendererApp::onResize(const ResizeData& rszData)
{
  SH_UNREFERENCED_PARAMETER(rszData);
  RenderManager& renderMan = g_renderMan();

  auto& pScreen = getScreen();
  m_screenSize = Vector2(cast::st<float>(pScreen->getClientSize().x),
                         cast::st<float>(pScreen->getClientSize().y));

  renderMan.setScreenSize(m_sceneSize);
  renderMan.createRenderTextures();

  m_camera.setPerspectiveData(m_camera.getHalfFOV(),
                              m_sceneSize.x,
                              m_sceneSize.y,
                              m_camera.getNear(),
                              m_camera.getFar());

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
  ShaderManager& shaderMan = g_shaderMan();

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
    shaderMan.recompileShaders();
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
  //m_pLCBuffer.reset();
  //m_pLightBuffer.reset();
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
RendererApp::updateMainBuffer()
{
  //GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();
  Time& time = g_time();

  //auto& pMainBuffer = shaderMan.m_pMainBuffer;
  auto& mbd = shaderMan.m_mainBufferData;
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

  mbd.screenSize = m_sceneSize;
  mbd.nearPlane = m_camera.getNear();
  mbd.farPlane = m_camera.getFar();

  mbd.cameraPosition = Vector4(m_camera.getPosition(), 0.0f);
  mbd.cameraDirection = Vector4(m_camera.getTarget(), 0.0f);

  mbd.time = time.getTime();
  mbd.deltaTime = time.getFrameDeltaTime();
  mbd.cosTime = Math::cos(Radian(mbd.time));
  mbd.sinTime = Math::sin(Radian(mbd.time));

  shaderMan.updateMainCB();
}

void
RendererApp::loadPistol()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();
  Logger& logger = g_logger();

  Timer timer;
  float time = timer.getTime();
  logger.consoleLog("Loading DrakeFire Shura Asset model...");

  auto modelRes = cast::re_ptr<StaticMeshResource>(
                  resMan.loadModelFromCache("resources/assets/models/DrakeFire.sha"));

  auto model = sh_makeShared<GameObject>();
  model->m_name = "Drakefire";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  //model->m_transform.getTransformMatrix() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 5.0f);

  float total = timer.getTime() - time;
  logger.consoleLog("Model loaded in " + std::to_string(total) + " seconds.");

  /*auto pCollider = sh_makeShared<ColliderComponent>();
  pCollider->m_collider.m_type = COLLIDER_TYPE::kOBBox;
  pCollider->m_collider.m_box.center = Vector3::ZERO;
  pCollider->m_collider.m_box.extent = Vector3(0.5f, 0.5f, 0.5f);
  pCollider->m_collider.m_box.rotation = Quaternion::IDENTITY;*/
  //pCollider->m_collider.m_sphere.center = Vector3::ZERO;
  //pCollider->m_collider.m_sphere.radius = 0.5f;
  //pCollider->m_collider.m_capsule.center = Vector3::ZERO;
  //pCollider->m_collider.m_capsule.radius = 0.5f;
  //pCollider->m_collider.m_capsule.height = 1.0f;
  //pCollider->m_collider.m_capsule.direction = Vector3::ZERO;

  /*auto pRigidbody = sh_makeShared<RigidbodyComponent>();
  pRigidbody->m_rigidbody.m_mass = 1.0f;
  pRigidbody->m_rigidbody.m_integrationType = INTEGRATION::kVerlet;
  pRigidbody->m_rigidbody.m_colliderType = COLLIDER_TYPE::kOBBox;
  pRigidbody->m_rigidbody.m_dragCoefficent = 0.1f;
  pRigidbody->m_rigidbody.m_elasticity = 0.0f;
  pRigidbody->m_rigidbody.m_friction = 0.5f;
  pRigidbody->m_rigidbody.m_gravityScale = 1.0f;
  pRigidbody->m_rigidbody.m_position = model->transform.getPosition();
  pRigidbody->m_rigidbody.m_rotation = Quaternion(model->transform.getRotation());*/

  //model->addComponent(pCollider);
  //model->addComponent(pRigidbody);

  sceneG.addObject(model);
}

void
RendererApp::loadSponza()
{
  ResourceManager& resMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto sponzaModelRes = cast::re_ptr<StaticMeshResource>(
                        resMan.loadModelFromCache("resources/assets/models/Sponza.sha"));

  auto model = sh_makeShared<GameObject>();
  model->m_name = "Sponza";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

  modelMC->setMeshData(sponzaModelRes);
  model->addComponent(modelMC);

  //model->m_transform.getTransformMatrix() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE * 0.25f);

  /*auto pCollider = sh_makeShared<ColliderComponent>();
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
  pRigidbody->m_rigidbody.m_position = model->m_transform.getPosition();
  pRigidbody->m_rigidbody.m_rotation = Quaternion(model->m_transform.getRotation());

  model->addComponent(pCollider);
  model->addComponent(pRigidbody);*/

  sceneG.addObject(model);
}

void
RendererApp::tempLoad()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = cast::re_ptr<SkeletalMeshResource>(
    resourceMan.loadResourceFromFile(Path("resources/models/Breakdance Freezes.fbx")));

  auto model = sh_makeShared<GameObject>();
  model->m_name = "BlueMan";
  auto modelMC = sh_makeShared<SkeletalMeshComponent>();

  modelMC->setMeshData(modelRes);
  model->addComponent(modelMC);

  //model->m_transform.getTransformMatrix() = Matrix4::IDENTITY;
  model->setScale(Vector3::ONE);

  auto animator = sh_makeShared<AnimatorComponent>();
  animator->m_skeletonData = modelRes->m_skeleton;
  //animator->m_currentAnim = animator->m_skeletonData->m_animations[0];

  sceneG.addObject(model);
}

void
RendererApp::playScene()
{
  //SceneGraph& scene = g_sceneGraph();
  //m_tempGameObjects = scene.getGameObjectList();
}

void
RendererApp::restartScene()
{
  //SceneGraph& scene = g_sceneGraph();
  //scene.getGameObjectList() = m_tempGameObjects;
}

void
RendererApp::loadMods()
{
  DynamicLibraryManager& dllMan = g_dynLibMan();

  const String modsPath = "mods/";

  for (auto& entry : DirectoryIterator(modsPath)) {
    if (!entry.is_regular_file()) {
      continue;
    }

    String extension = "";

#if SH_PLATFORM == SH_PLATFORM_WIN32
    extension = ".dll";
#elif SH_PLATFORM == SH_PLATFORM_LINUX
    extension = ".so";
#endif

    if (entry.path().extension() != extension) {
      continue;
    }

    SystemPath filename = entry.path().string();
    filename.replace_extension("");

    if (dllMan.isDynLibLoaded(filename.string())) {
      dllMan.unloadDynLibrary(filename.string());
    }

    dllMan.loadDynLibrary(filename.string());

    m_loadedModIds.push_back(StringID(filename.string()).getID());
  }
}
}
