/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
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

#include "shPath.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMaterial.h"

#include "shRadian.h"
#include "shVector4.h"

#include "shSound.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
/**
*  @brief Structure for ambient occlusion buffer.
*/
struct AOBuffer {
  Vector2 viewport = { 0.0f, 0.0f };
  float samplerRad = 0.0f;
  float scale = 0.0f;
  float bias = 0.0f;
  float intensity = 0.0f;
  Vector2 unused = { 0.0f, 0.0f };
};

void
RendererApp::onCreate()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();
  ResourceManager& resourceMan = g_resourceMan();
  AudioManager& audioMan = AudioManager::instance();

  m_shadowTexSize = 2048.0f;

  initGraphicAssets();
  initCamera();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());

  ImGui::StyleColorsDark();

  // Load images
  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  loadPistol();
  loadSponza();

  // Set light buffer
  Vector<Vector4> lights;
  lights.resize(12);
  m_lightPos = { 0.0f, 2.0f, 0.0f, 1.0f };
  lights[0] = m_lightPos;

  //m_light.position = Vector3(5.0f, 5.0f, 5.0f);
  //m_light.color = LinearColor(1.0f, 1.0f, 1.0f);
  //m_light.intensity = 1.0f;

  m_pLightBuffer = graphMan.createConstantBuffer(sizeof(lights));

  graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

  // Set pass buffers
  auto pBasicShader = renderMan.getPass("BasicShader");
  pBasicShader->addVSConstantBuffer(m_pVP);
  pBasicShader->addVSConstantBuffer(m_pModelTransform);

  auto pDeferredShader = renderMan.getPass("DeferredShader");
  pDeferredShader->addPSConstantBuffer(m_pInvVP);
  pDeferredShader->addPSConstantBuffer(m_pCameraPosition);
  pDeferredShader->addPSConstantBuffer(m_pLightBuffer);
  pDeferredShader->addPSConstantBuffer(m_pViewportBuffer);

  float screenW = static_cast<float>(getScreenDescription().width);
  float screenH = static_cast<float>(getScreenDescription().height);

  AOBuffer aoBuffer;
  aoBuffer.viewport.x = screenW;
  aoBuffer.viewport.y = screenH;
  aoBuffer.samplerRad = 1.0f;
  aoBuffer.scale = 1.0f;
  aoBuffer.bias = 0.01f;
  aoBuffer.intensity = 1.0f;

  auto pAOShader = renderMan.getPass("AOShader");
  m_pAOBuffer = graphMan.createConstantBuffer(sizeof(AOBuffer));
  graphMan.updateConstantBuffer(m_pAOBuffer, &aoBuffer, sizeof(AOBuffer));
  pAOShader->addPSConstantBuffer(m_pAOBuffer);

  auto pHBlurShader = renderMan.getPass("HBlurShader");
  auto pVBlurShader = renderMan.getPass("VBlurShader");
  pHBlurShader->addPSConstantBuffer(m_pViewportBuffer);
  pVBlurShader->addPSConstantBuffer(m_pViewportBuffer);

  m_lightTarget = Vector3::ZERO;
  m_lcamSize = 2048.0f;
  m_lcamNear = 1.0f;
  m_lcamFar = 2000.0f;

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

  Vector4 lightS = { m_lightCam.getWidth(), 0.0f,0.0f,0.f };
  m_pLSizeBuffer = graphMan.createConstantBuffer(sizeof(Vector4));
  graphMan.updateConstantBuffer(m_pLSizeBuffer, &lightS, sizeof(Vector4));

  auto pSMapShader = renderMan.getPass("SMapShader");
  pSMapShader->addVSConstantBuffer(m_pLCBuffer);
  pSMapShader->addVSConstantBuffer(m_pModelTransform);

  Path audioPath("resources/cat.wav");
  m_testSound = audioMan.createSound(audioPath);
  m_testSound->m_channel = CHANNEL_TYPE::kUI;

  pDeferredShader->addPSConstantBuffer(m_pLCBuffer);
  pDeferredShader->addPSConstantBuffer(m_pLSizeBuffer);
}

void
RendererApp::onUpdate()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();
  AudioManager& audioMan = AudioManager::instance();

  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();

  ImGui_ImplShura_AddMouseWheelEvent(m_hdelta, m_delta);
  m_delta = 0.0f;
  m_hdelta = 0.0f;

  m_modelPos = m_pModel->getPosition();
  m_modelRot = m_pModel->getRotation() * Math::RAD2DEG;
  m_modelScale = m_pModel->getScale();

  m_sponzaPos = m_pSponza->getPosition();
  m_sponzaRot = m_pSponza->getRotation() * Math::RAD2DEG;
  m_sponzaScale = m_pSponza->getScale();
  
  Vector<Vector4> lights;
  lights.resize(12);
  lights[0] = m_lightPos;

  setImgui();

  if (m_modelPos != m_pModel->getPosition()) {
    m_pModel->setPosition(m_modelPos);
  }
  if (m_modelRot != m_pModel->getRotation()) {
    m_pModel->setRotation(m_modelRot * Math::DEG2RAD);
  }
  if (m_modelScale != m_pModel->getScale()) {
    m_pModel->setScale(m_modelScale);
  }

  if (m_sponzaPos != m_pSponza->getPosition()) {
    m_pSponza->setPosition(m_sponzaPos);
  }
  if (m_sponzaRot != m_pSponza->getRotation()) {
    m_pSponza->setRotation(m_sponzaRot * Math::DEG2RAD);
  }
  if (m_sponzaScale != m_pSponza->getScale()) {
    m_pSponza->setScale(m_sponzaScale);
  }

  graphMan.updateConstantBuffer(m_pModelTransform,
                                &m_pModel->transform.getTransform(),
                                sizeof(Transform));

  graphMan.updateConstantBuffer(m_pSponzaTransform,
                                &m_pSponza->transform.getTransform(),
                                sizeof(Transform));

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

    /*m_lightCam.setViewData(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z),
                           m_lightTarget,
                           Vector3::UP);*/
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

    graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));
    graphMan.updateConstantBuffer(m_pLSizeBuffer, &camSize, sizeof(Vector4));
  }

  AOBuffer aoBuffer;
  aoBuffer.viewport.x = static_cast<float>(getScreenDescription().width);
  aoBuffer.viewport.y = static_cast<float>(getScreenDescription().height);
  aoBuffer.samplerRad = m_aoSamplerRad;
  aoBuffer.scale = m_aoScale;
  aoBuffer.bias = m_aoBias;
  aoBuffer.intensity = m_aoIntensity;

  graphMan.updateConstantBuffer(m_pAOBuffer, &aoBuffer, sizeof(AOBuffer));

  if (m_bRightClick) {
    rotateCamera();
  }

  const float camSpeed = 0.5f;

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
  
  updateCamera();

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
  auto pDepthSV = graphMan.getMainDepthStencil();

  // Shadow Mapping
  Viewport shadowVP = {};
  shadowVP.width = m_lcamSize;
  shadowVP.height = m_lcamSize;
  shadowVP.minDepth = 0.0f;
  shadowVP.maxDepth = 1.0f;
  shadowVP.topLeftX = 0.0f;
  shadowVP.topLeftY = 0.0f;

  graphMan.setViewport(shadowVP);

  graphMan.clearDepthStencil(m_pSMapTarget);
  renderMan.makePass("SMapShader");

  Vector<SPtr<Texture2D>> vShadow;
  graphMan.setRenderTargets(vShadow, m_pSMapTarget);

  SPtr<StaticMeshUnionComponent> pistol;
  SPtr<StaticMeshUnionComponent> sponza;
  for (auto& component : m_pModel->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
      pistol = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
    }
  }
  for (auto& component : m_pSponza->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
      sponza = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
    }
  }

  renderMan.drawSMUInScene({ pistol });
  graphMan.vsSetConstantBuffers(m_pSponzaTransform, 1);
  renderMan.drawSMUInScene({ sponza });

  graphMan.setShaderResourceView(nullptr, 0);
  graphMan.setShaderResourceView(nullptr, 1);
  graphMan.setShaderResourceView(nullptr, 2);
  graphMan.setShaderResourceView(nullptr, 3);
  graphMan.setShaderResourceView(nullptr, 4);

  // Gbuffer pass
  Viewport normalVP = {};
  normalVP.width = getScreenDescription().width;
  normalVP.height = getScreenDescription().height;
  normalVP.minDepth = 0.0f;
  normalVP.maxDepth = 1.0f;
  normalVP.topLeftX = 0.0f;
  normalVP.topLeftY = 0.0f;

  graphMan.setViewport(normalVP);

  graphMan.clearRenderTarget(m_pDepthTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_pNormalTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_pColorTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearDepthStencil(pDepthSV);

  graphMan.setRenderTargets({m_pDepthTarget, m_pNormalTarget, m_pColorTarget, }, pDepthSV);
  renderMan.makePass("BasicShader");

  renderMan.drawSMUInScene({ pistol });
  graphMan.vsSetConstantBuffers(m_pSponzaTransform, 1);
  renderMan.drawSMUInScene({ sponza });

  graphMan.setShaderResourceView(nullptr, 2);
  graphMan.setShaderResourceView(nullptr, 3);
  graphMan.setShaderResourceView(nullptr, 4);

  // Ambient Occlusion pass
  graphMan.clearRenderTarget(m_pAoTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_pAoTarget }, pDepthSV);
  renderMan.makePass("AOShader");

  graphMan.setShaderResourceView(m_pDepthTarget, 0);
  graphMan.setShaderResourceView(m_pNormalTarget, 1);

  graphMan.draw(3, 0);

  graphMan.setShaderResourceView(nullptr, 1);

  // Horizontal Blur pass
  graphMan.clearRenderTarget(m_pHbTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_pHbTarget }, pDepthSV);
  renderMan.makePass("HBlurShader");

  graphMan.setShaderResourceView(m_pAoTarget, 0);

  graphMan.draw(3, 0);

  // Vetical Blur pass
  graphMan.clearRenderTarget(m_pVbTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.setRenderTargets({ m_pVbTarget }, pDepthSV);
  renderMan.makePass("VBlurShader");

  graphMan.setShaderResourceView({ m_pHbTarget }, 0);

  graphMan.draw(3, 0);

  // Deferred pass
  graphMan.setRenderTargets({ m_mainTarget }, pDepthSV);
  renderMan.makePass("DeferredShader");

  graphMan.setShaderResourceView(m_pDepthTarget, 0);
  graphMan.setShaderResourceView(m_pNormalTarget, 1);
  graphMan.setShaderResourceView(m_pColorTarget, 2);
  graphMan.setShaderResourceView(m_pVbTarget, 3);
  graphMan.setShaderResourceView(m_pSMapTarget, 4);
  
  graphMan.draw(3, 0);

  graphMan.setShaderResourceView(nullptr, 0);
  graphMan.setShaderResourceView(nullptr, 1);
  graphMan.setShaderResourceView(nullptr, 2);
  graphMan.setShaderResourceView(nullptr, 3);
  graphMan.setShaderResourceView(nullptr, 4);

  graphMan.vsSetConstantBuffers(nullptr, 0);
  graphMan.vsSetConstantBuffers(nullptr, 1);
  graphMan.vsSetConstantBuffers(nullptr, 2);
  graphMan.vsSetConstantBuffers(nullptr, 3);

  graphMan.psSetConstantBuffers(nullptr, 0);
  graphMan.psSetConstantBuffers(nullptr, 1);
  graphMan.psSetConstantBuffers(nullptr, 2);
  graphMan.psSetConstantBuffers(nullptr, 3);

  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
}

void
RendererApp::onResize(const ResizeData& rszData)
{
  GraphicsManager& graphMan = g_graphicsMan();

  float width = static_cast<float>(rszData.width);
  float height = static_cast<float>(rszData.height);

  m_pDepthTarget.reset();
  m_pNormalTarget.reset();
  m_pColorTarget.reset();
  m_mainTarget.reset();
  m_pAoTarget.reset();
  m_pHbTarget.reset();
  m_pVbTarget.reset();
  m_pSMapTarget.reset();

  setRenderTargets();

  m_camera.setPerspectiveData(m_camera.getHalfFOV(),
                              width,
                              height,
                              m_camera.getNear(),
                              m_camera.getFar());

  updateCamera();

  Vector4 viewport(width,
                   height,
                   m_camera.getFar(),
                   m_camera.getNear());
  graphMan.updateConstantBuffer(m_pViewportBuffer, &viewport, sizeof(Vector4));

  ImGui_ImplShura_Resize(getScreen());
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
  ImGui_ImplShura_Shutdown();
  ImGui::DestroyContext();

  m_pVP.reset();
  m_pInvVP.reset();
  m_pModelTransform.reset();
  m_pCameraPosition.reset();
  m_pLightBuffer.reset();
  m_pViewportBuffer.reset();
  m_pAOBuffer.reset();

  m_pModel->~GameObject();
  m_pModel.reset();
}

void
RendererApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  // Init pass shaders
  // Basic
  auto pBasicShader = make_shared<Pass>();
  pBasicShader->setShaderInfo("resources/shaders/BasicShader.hlsl",
                              "main",
                              "mainPS",
                              "vs_5_0",
                              "ps_5_0");
  pBasicShader->compileShader();

  // Deferred
  auto pDeferredShader = make_shared<Pass>();
  pDeferredShader->setShaderInfo("resources/shaders/DeferredShader.hlsl",
                                 "main",
                                 "mainPS",
                                 "vs_5_0",
                                 "ps_5_0");
  pDeferredShader->compileShader();

  // AO
  auto pAOShader = make_shared<Pass>();
  pAOShader->setShaderInfo("resources/shaders/AOShader.hlsl",
                           "main",
                           "mainPS",
                           "vs_5_0",
                           "ps_5_0");
  pAOShader->compileShader();

  // HBlur
  auto pHBlurShader = make_shared<Pass>();
  pHBlurShader->setShaderInfo("resources/shaders/HBlurShader.hlsl",
                              "main",
                              "mainPS",
                              "vs_5_0",
                              "ps_5_0");
  pHBlurShader->compileShader();

  // VBlur
  auto pVBlurShader = make_shared<Pass>();
  pVBlurShader->setShaderInfo("resources/shaders/VBlurShader.hlsl",
                              "main",
                              "mainPS",
                              "vs_5_0",
                              "ps_5_0");
  pVBlurShader->compileShader();

  auto pSMapShader = make_shared<Pass>();
  pSMapShader->setShaderInfo("resources/shaders/SMapShader.hlsl",
                             "main",
                             "mainPS",
                             "vs_5_0",
                             "ps_5_0");
  pSMapShader->compileShader();

  // Set pass states
  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kSolid;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  DepthStencilDesc depthSDesc = {};
  depthSDesc.depthEnable = true;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kLess;
  depthSDesc.stencilEnable = true;
  depthSDesc.stencilReadMask = 0xFF;
  depthSDesc.stencilWriteMask = 0xFF;
  depthSDesc.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  depthSDesc.frontFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.backFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  depthSDesc.backFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  DepthStencilDesc planeDepthSDesc = depthSDesc;
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.stencilEnable = false;

  auto pSamplerLinear = graphMan.createSamplerState();

  // Fill pass info
  // Basic
  pBasicShader->generateInputLayout();
  pBasicShader->setSamplerState(pSamplerLinear);
  pBasicShader->setRasterizerState(rasterDesc);
  pBasicShader->setBlendState(blendDesc);
  pBasicShader->setDepthStencilState(depthSDesc);

  // Deferred
  pDeferredShader->generateInputLayout();
  pDeferredShader->setSamplerState(pSamplerLinear);
  pDeferredShader->setDepthStencilState(planeDepthSDesc);

  // AO
  pAOShader->generateInputLayout();
  pAOShader->setSamplerState(pSamplerLinear);
  pAOShader->setDepthStencilState(planeDepthSDesc);

  // HBlur
  pHBlurShader->generateInputLayout();
  pHBlurShader->setSamplerState(pSamplerLinear);
  pHBlurShader->setDepthStencilState(planeDepthSDesc);

  // VBlur
  pVBlurShader->generateInputLayout();
  pVBlurShader->setSamplerState(pSamplerLinear);
  pVBlurShader->setDepthStencilState(planeDepthSDesc);

  // Shadow Map
  pSMapShader->generateInputLayout();
  pSMapShader->setSamplerState(pSamplerLinear);
  pSMapShader->setRasterizerState(rasterDesc);
  pSMapShader->setBlendState(blendDesc);
  pSMapShader->setDepthStencilState(depthSDesc);

  renderMan.setPass(pBasicShader, "BasicShader");
  renderMan.setPass(pAOShader, "AOShader");
  renderMan.setPass(pHBlurShader, "HBlurShader");
  renderMan.setPass(pVBlurShader, "VBlurShader");
  renderMan.setPass(pDeferredShader, "DeferredShader");
  renderMan.setPass(pSMapShader, "SMapShader");

  // Create and set render targets for deferred rendering
  setRenderTargets();
}

void
RendererApp::initCamera()
{
  // Init camera and its constant buffer
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp = {};

  m_camera = Camera(Vector3(0.0f, 0.0f, -3.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    30.0f * Math::DEG2RAD,
                    static_cast<float>(getScreenDescription().width),
                    static_cast<float>(getScreenDescription().height),
                    0.1f,
                    2000.0f);

  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));

  // Init constant buffer of camera position
  Vector4 foward(m_camera.getPosition(), 0.0f);
  
  m_pCameraPosition = g_graphicsMan().createConstantBuffer(sizeof(Vector4));

  g_graphicsMan().updateConstantBuffer(m_pCameraPosition,
                                       &foward,
                                       sizeof(Vector4));

  // Init buffer for inverse view and projection
  InvVP invVP = {};

  m_pInvVP = g_graphicsMan().createConstantBuffer(sizeof(InvVP));

  invVP.invVP = (m_camera.getProjection() * m_camera.getView()).getInversed();
  invVP.invV = m_camera.getView().getInversed();

  invVP.invVP.getTransposed();
  invVP.invV.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pInvVP,
                                       &invVP,
                                       sizeof(InvVP));

  // Init buffer for viewport
  Vector4 viewport(static_cast<float>(getScreenDescription().width),
                   static_cast<float>(getScreenDescription().height),
                   m_camera.getFar(),
                   m_camera.getNear());
  m_pViewportBuffer = g_graphicsMan().createConstantBuffer(sizeof(Vector4));
  g_graphicsMan().updateConstantBuffer(m_pViewportBuffer, &viewport, sizeof(Vector4));
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
    /*m_camera.orbitCamera(Radian(dx * Math::DEG2RAD),
                         Radian(dy * Math::DEG2RAD),
                         Vector3::ZERO);*/
  }
}

void
RendererApp::updateCamera()
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_camera.update();
  m_lightCam.update();

  // Update camera buffer
  VP vp = {};
  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();
  vp.proj.getTransposed();
  vp.view.getTransposed();

  graphMan.updateConstantBuffer(m_pVP, &vp, sizeof(vp));

  // Update camera position buffer
  Vector4 foward(m_camera.getPosition(), 0.0f);
  graphMan.updateConstantBuffer(m_pCameraPosition, &foward, sizeof(Vector4));

  // Update inverse view-projection buffer
  InvVP invVP = {};
  invVP.invVP = (vp.proj * vp.view).getInversed();
  invVP.invV = vp.view.getInversed();
  graphMan.updateConstantBuffer(m_pInvVP, &invVP, sizeof(InvVP));
}

void
RendererApp::setRenderTargets()
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_mainTarget = graphMan.getMainRenderTargetView();

  m_pDepthTarget = graphMan.createTexture2D(getScreenDescription().width,
                                            getScreenDescription().height,
                                            TEXTURE_FORMAT::kR32G32B32A32_float,
                                            USAGE::kDefault,
                                            BIND_FLAGS::kRenderTarget |
                                            BIND_FLAGS::kShaderResource);

  m_pNormalTarget = graphMan.createTexture2D(getScreenDescription().width,
                                             getScreenDescription().height,
                                             TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                             USAGE::kDefault,
                                             BIND_FLAGS::kRenderTarget |
                                             BIND_FLAGS::kShaderResource);

  m_pColorTarget = graphMan.createTexture2D(getScreenDescription().width,
                                            getScreenDescription().height,
                                            TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                            USAGE::kDefault,
                                            BIND_FLAGS::kRenderTarget |
                                            BIND_FLAGS::kShaderResource);

  m_pAoTarget = graphMan.createTexture2D(getScreenDescription().width,
                                         getScreenDescription().height,
                                         TEXTURE_FORMAT::kR16_FLOAT,
                                         USAGE::kDefault,
                                         BIND_FLAGS::kRenderTarget |
                                         BIND_FLAGS::kShaderResource);

  m_pHbTarget = graphMan.createTexture2D(getScreenDescription().width,
                                         getScreenDescription().height,
                                         TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                         USAGE::kDefault,
                                         BIND_FLAGS::kRenderTarget |
                                         BIND_FLAGS::kShaderResource);

  //m_hbTarget.push_back(hbTarget);

  m_pVbTarget = graphMan.createTexture2D(getScreenDescription().width,
                                         getScreenDescription().height,
                                         TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                         USAGE::kDefault,
                                         BIND_FLAGS::kRenderTarget |
                                         BIND_FLAGS::kShaderResource);

  //m_vbTarget.push_back(vbTarget);

  m_pSMapTarget = graphMan.createTexture2D(static_cast<uint32>(m_shadowTexSize),
                                           static_cast<uint32>(m_shadowTexSize),
                                           TEXTURE_FORMAT::kR32_Typeless,
                                           USAGE::kDefault,
                                           BIND_FLAGS::kDepthStencil |
                                           BIND_FLAGS::kShaderResource);
}

void
RendererApp::setImgui()
{
  float width = static_cast<float>(getScreenDescription().width);
  float height = static_cast<float>(getScreenDescription().height);

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(250.0f, height));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Scenegraph",
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  ImGui::PopStyleColor();
  ImGui::Text(m_pModel->name.c_str());
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(width - 400.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(400.0f, height));
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
  if (ImGui::CollapsingHeader("Model Transform")) {

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
    ImGui::DragFloat("x##SclX", &m_modelScale.x, 0.01f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##SclY", &m_modelScale.y, 0.01f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##SclZ", &m_modelScale.z, 0.01f);
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

    // Position
    ImGui::Text("Position:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##sPosX", &m_sponzaPos.x, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##sPosY", &m_sponzaPos.y, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##sPosZ", &m_sponzaPos.z, 0.01f);
    ImGui::PopStyleColor(3);

    // Rotation
    ImGui::Text("Rotation:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##sRotX", &m_sponzaRot.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##sRotY", &m_sponzaRot.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##sRotZ", &m_sponzaRot.z, 0.1f);
    ImGui::PopStyleColor(3);

    // Scale
    ImGui::Text("Scale:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##sSclX", &m_sponzaScale.x, 0.01f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##sSclY", &m_sponzaScale.y, 0.01f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##sSclZ", &m_sponzaScale.z, 0.01f);
    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(220, 220, 220, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 180, 180, 150));
    if (ImGui::Button("Reset##SponzaReset")) {
      m_sponzaPos = { 0.0f, 0.0f, 0.0f };
      m_sponzaRot = { 0.0f, 0.0f, 0.0f };
      m_sponzaScale = { 1.0f, 1.0f, 1.0f };
    }
    ImGui::PopStyleColor(3);
  }
  ImGui::PopStyleColor(3);

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
    ImGui::DragFloat("Light Cam Near:", &m_lcamNear, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloat("Light Cam Far:", &m_lcamFar, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloat("Light Cam Size:", &m_lcamSize, 1.0f);
  }

  ImGui::InputFloat("Light Intensity", &m_lIntensity);

  static char buf[32] = "hello";
  ImGui::InputText("TestingKeys", buf, 32);

  if (ImGui::Button("Play Sound")) {
    bIsSoundPlaying = true;
  }

  ImGui::End();
}

void
RendererApp::loadPistol()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                  resourceMan.loadResourceFromFile(Path("resources/DrakeFire.fbx")));

  /*auto baseColor = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(Path("resources/base_albedo.png")));
  auto normal = reinterpret_pointer_cast<ImageResource>(
                resourceMan.loadResourceFromFile(Path("resources/base_normal.png")));
  auto metallic = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(Path("resources/base_metallic.png")));*/
  auto roughness = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(Path("resources/base_roughness.png")));
  auto ao = reinterpret_pointer_cast<ImageResource>(
            resourceMan.loadResourceFromFile(Path("resources/base_AO.png")));

  /*modelRes->materials[0]->baseColor = baseColor->texture;
  modelRes->materials[0]->baseColorPath = baseColor->getPath().toString();
  modelRes->materials[0]->normal = normal->texture;
  modelRes->materials[0]->normalPath = normal->getPath().toString();
  modelRes->materials[0]->metallic = metallic->texture;
  modelRes->materials[0]->metallicPath = metallic->getPath().toString();*/
  modelRes->materials[0]->roughness = roughness->texture;
  modelRes->materials[0]->roughnessPath = roughness->getPath().toString();
  modelRes->materials[0]->ao = ao->texture;
  modelRes->materials[0]->aoPath = ao->getPath().toString();

  m_pModel = make_shared<GameObject>();
  m_pModel->name = "DrakeFire";
  auto modelMC = make_shared<StaticMeshUnionComponent>();

  modelMC->setMeshData(modelRes);
  m_pModel->addComponent(modelMC);

  m_pModel->transform.getTransform().m[3][3] = 1.0f;

  m_pModel->setPosition(Vector3::ZERO);
  m_pModel->setScale(Vector3::ONE);
  m_pModel->setRotation(Vector3::ZERO);

  sceneG.addObject(m_pModel);

  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Transform));

  graphMan.updateConstantBuffer(m_pModelTransform,
                                &m_pModel->transform.getTransform(),
                                sizeof(Transform));

  //resourceMan.saveResourceToAsset(modelRes);
}

void
RendererApp::loadSponza()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto sponzaModelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                        resourceMan.loadResourceFromFile(
                        Path("resources/Models/Sponza.fbx")));

  // Normal textures
  auto bgNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/Background_Normal.png")));
  auto chainNormal = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/ChainTexture_Normal.png")));
  auto lionNormal = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Lion_Normal.png")));
  auto archNormal = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Sponza_Arch_normal.png")));
  auto bricksNormal = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Bricks_a_Normal.png")));
  auto ceilingNormal = reinterpret_pointer_cast<ImageResource>(
                       resourceMan.loadResourceFromFile(
                       Path("resources/textures/Sponza_Ceiling_normal.png")));
  auto caNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/Sponza_Column_a_normal.png")));
  auto cbNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/Sponza_Column_b_normal.png")));
  auto ccNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/Sponza_Column_c_normal.png")));
  auto curtainNormal = reinterpret_pointer_cast<ImageResource>(
                       resourceMan.loadResourceFromFile(
                       Path("resources/textures/Sponza_Curtain_Blue_normal.png")));
  auto detailsNormal = reinterpret_pointer_cast<ImageResource>(
                       resourceMan.loadResourceFromFile(
                       Path("resources/textures/Sponza_Details_normal.png")));
  auto fabricNormal = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Fabric_Blue_normal.png")));
  auto fpNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/Sponza_FlagPole_normal.png")));
  auto floorNormal = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/Sponza_Floor_normal.png")));
  auto roofNormal = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Sponza_Roof_normal.png")));
  auto thornNormal = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/Sponza_Thorn_normal.png")));
  auto vaseNormal = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Vase_normal.png")));
  auto vhNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/VaseHanging_normal.png")));
  auto vpNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/VasePlant_normal.png")));
  auto vrNormal = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(
                  Path("resources/textures/VaseRound_normal.png")));

  // Roughness textures
  auto bgRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/Background_Roughness.png")));
  auto chainRough = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/ChainTexture_Roughness.png")));
  auto lionRough = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Lion_Roughness.png")));
  auto archRough = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Sponza_Arch_roughness.png")));
  auto bricksRough = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/Sponza_Bricks_a_Roughness.png")));
  auto ceilingRough = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Ceiling_roughness.png")));
  auto caRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/Sponza_Column_a_roughness.png")));
  auto cbRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/Sponza_Column_b_roughness.png")));
  auto ccRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/Sponza_Column_c_roughness.png")));
  auto curtainRough = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Curtain_roughness.png")));
  auto detailsRough = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Details_roughness.png")));
  auto fabricRough = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/Sponza_Fabric_roughness.png")));
  auto fpRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/Sponza_FlagPole_roughness.png")));
  auto floorRough = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Sponza_Floor_roughness.png")));
  auto roofRough = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(
                   Path("resources/textures/Sponza_Roof_roughness.png")));
  auto thornRough = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/Sponza_Thorn_roughness.png")));
  auto vaseRough = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(
                   Path("resources/textures/Vase_roughness.png")));
  auto vhRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/VaseHanging_roughness.png")));
  auto vpRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/VasePlant_roughness.png")));
  auto vrRough = reinterpret_pointer_cast<ImageResource>(
                 resourceMan.loadResourceFromFile(
                 Path("resources/textures/VaseRound_roughness.png")));

  // Metallic textures
  auto metalFull = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(
                   Path("resources/textures/Metallic_metallic.png")));
  auto metalNone = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(
                   Path("resources/textures/Dielectric_metallic.png")));
  auto chainMetal = reinterpret_pointer_cast<ImageResource>(
                    resourceMan.loadResourceFromFile(
                    Path("resources/textures/ChainTexture_Metallic.png")));
  auto curtainMetal = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Curtain_metallic.png")));
  auto detailsMetal = reinterpret_pointer_cast<ImageResource>(
                      resourceMan.loadResourceFromFile(
                      Path("resources/textures/Sponza_Details_metallic.png")));
  auto fabricMetal = reinterpret_pointer_cast<ImageResource>(
                     resourceMan.loadResourceFromFile(
                     Path("resources/textures/Sponza_Fabric_metallic.png")));

  sponzaModelRes->materials[0]->normal = thornNormal->texture;
  sponzaModelRes->materials[1]->normal = vpNormal->texture;
  sponzaModelRes->materials[2]->normal = vrNormal->texture;
  sponzaModelRes->materials[3]->normal = bgNormal->texture;
  sponzaModelRes->materials[4]->normal = bricksNormal->texture;
  sponzaModelRes->materials[5]->normal = archNormal->texture;
  sponzaModelRes->materials[6]->normal = ceilingNormal->texture;
  sponzaModelRes->materials[7]->normal = caNormal->texture;
  sponzaModelRes->materials[8]->normal = floorNormal->texture;
  sponzaModelRes->materials[9]->normal = ccNormal->texture;
  sponzaModelRes->materials[10]->normal = detailsNormal->texture;
  sponzaModelRes->materials[11]->normal = cbNormal->texture;
  sponzaModelRes->materials[12]->normal = thornNormal->texture;
  sponzaModelRes->materials[13]->normal = fpNormal->texture;
  sponzaModelRes->materials[14]->normal = fabricNormal->texture;
  sponzaModelRes->materials[15]->normal = fabricNormal->texture;
  sponzaModelRes->materials[16]->normal = fabricNormal->texture;
  sponzaModelRes->materials[17]->normal = curtainNormal->texture;
  sponzaModelRes->materials[18]->normal = curtainNormal->texture;
  sponzaModelRes->materials[19]->normal = curtainNormal->texture;
  sponzaModelRes->materials[20]->normal = chainNormal->texture;
  sponzaModelRes->materials[21]->normal = vhNormal->texture;
  sponzaModelRes->materials[22]->normal = vaseNormal->texture;
  sponzaModelRes->materials[23]->normal = lionNormal->texture;
  sponzaModelRes->materials[24]->normal = roofNormal->texture;

  sponzaModelRes->materials[0]->roughness = thornRough->texture;
  sponzaModelRes->materials[1]->roughness = vpRough->texture;
  sponzaModelRes->materials[2]->roughness = vrRough->texture;
  sponzaModelRes->materials[3]->roughness = bgRough->texture;
  sponzaModelRes->materials[4]->roughness = bricksRough->texture;
  sponzaModelRes->materials[5]->roughness = archRough->texture;
  sponzaModelRes->materials[6]->roughness = ceilingRough->texture;
  sponzaModelRes->materials[7]->roughness = caRough->texture;
  sponzaModelRes->materials[8]->roughness = floorRough->texture;
  sponzaModelRes->materials[9]->roughness = ccRough->texture;
  sponzaModelRes->materials[10]->roughness = detailsRough->texture;
  sponzaModelRes->materials[11]->roughness = cbRough->texture;
  sponzaModelRes->materials[12]->roughness = thornRough->texture;
  sponzaModelRes->materials[13]->roughness = fpRough->texture;
  sponzaModelRes->materials[14]->roughness = fabricRough->texture;
  sponzaModelRes->materials[15]->roughness = fabricRough->texture;
  sponzaModelRes->materials[16]->roughness = fabricRough->texture;
  sponzaModelRes->materials[17]->roughness = curtainRough->texture;
  sponzaModelRes->materials[18]->roughness = curtainRough->texture;
  sponzaModelRes->materials[19]->roughness = curtainRough->texture;
  sponzaModelRes->materials[20]->roughness = chainRough->texture;
  sponzaModelRes->materials[21]->roughness = vhRough->texture;
  sponzaModelRes->materials[22]->roughness = vaseRough->texture;
  sponzaModelRes->materials[23]->roughness = lionRough->texture;
  sponzaModelRes->materials[24]->roughness = roofRough->texture;

  sponzaModelRes->materials[0]->metallic = metalNone->texture;
  sponzaModelRes->materials[1]->metallic = metalNone->texture;
  sponzaModelRes->materials[2]->metallic = metalNone->texture;
  sponzaModelRes->materials[3]->metallic = metalNone->texture;
  sponzaModelRes->materials[4]->metallic = metalNone->texture;
  sponzaModelRes->materials[5]->metallic = metalNone->texture;
  sponzaModelRes->materials[6]->metallic = metalNone->texture;
  sponzaModelRes->materials[7]->metallic = metalNone->texture;
  sponzaModelRes->materials[8]->metallic = metalNone->texture;
  sponzaModelRes->materials[9]->metallic = metalNone->texture;
  sponzaModelRes->materials[10]->metallic = detailsMetal->texture;
  sponzaModelRes->materials[11]->metallic = metalNone->texture;
  sponzaModelRes->materials[12]->metallic = metalNone->texture;
  sponzaModelRes->materials[13]->metallic = metalFull->texture;
  sponzaModelRes->materials[14]->metallic = fabricMetal->texture;
  sponzaModelRes->materials[15]->metallic = fabricMetal->texture;
  sponzaModelRes->materials[16]->metallic = fabricMetal->texture;
  sponzaModelRes->materials[17]->metallic = curtainMetal->texture;
  sponzaModelRes->materials[18]->metallic = curtainMetal->texture;
  sponzaModelRes->materials[19]->metallic = curtainMetal->texture;
  sponzaModelRes->materials[20]->metallic = chainMetal->texture;
  sponzaModelRes->materials[21]->metallic = metalNone->texture;
  sponzaModelRes->materials[22]->metallic = metalNone->texture;
  sponzaModelRes->materials[23]->metallic = metalNone->texture;
  sponzaModelRes->materials[24]->metallic = metalNone->texture;

  m_pSponza = make_shared<GameObject>();
  m_pSponza->name = "Sponza";
  auto modelMC = make_shared<StaticMeshUnionComponent>();

  modelMC->setMeshData(sponzaModelRes);
  m_pSponza->addComponent(modelMC);

  m_pSponza->transform.getTransform().m[3][3] = 1.0f;

  m_pSponza->setPosition(Vector3::ZERO);
  m_pSponza->setScale(Vector3::ONE);
  m_pSponza->setRotation(Vector3::ZERO);

  sceneG.addObject(m_pSponza);

  m_pSponzaTransform = graphMan.createConstantBuffer(sizeof(Transform));

  graphMan.updateConstantBuffer(m_pSponzaTransform,
                                &m_pSponza->transform.getTransform(),
                                sizeof(Transform));
}
}
