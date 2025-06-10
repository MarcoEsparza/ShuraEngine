/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/26
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
  //initShaders();
  //setRenderTargets();
  renderMan.createPasses();
  renderMan.setScreenSize(m_screenSize);
  renderMan.setShadowMapSize(m_shadowTexSize);
  renderMan.createRenderTextures();
  initCamera();

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

  // GBuffer constant buffers
  auto pBasicShader = renderMan.getPass("GBufferShader");
  pBasicShader->addVSConstantBuffer(m_pVP, 0);

  // Ambient occlusion buffers
  AOBuffer aoBuffer;
  aoBuffer.viewport.x = m_screenSize.x;
  aoBuffer.viewport.y = m_screenSize.y;
  aoBuffer.samplerRad = m_aoSamplerRad = 1.0f;
  aoBuffer.scale = m_aoScale = 1.0f;
  aoBuffer.bias = m_aoBias = 0.01f;
  aoBuffer.intensity = m_aoIntensity = 1.0f;

  auto pAOShader = renderMan.getPass("AOShader");
  m_pAOBuffer = graphMan.createConstantBuffer(sizeof(AOBuffer));
  graphMan.updateConstantBuffer(m_pAOBuffer, &aoBuffer, sizeof(AOBuffer));
  pAOShader->addPSConstantBuffer(m_pAOBuffer, 0);

  // Blur buffers
  auto pHBlurShader = renderMan.getPass("HBlurShader");
  auto pVBlurShader = renderMan.getPass("VBlurShader");
  pHBlurShader->addCSConstantBuffer(m_pViewportBuffer, 0);
  pVBlurShader->addCSConstantBuffer(m_pViewportBuffer, 0);

  // Shadow shader buffers
  auto pSMapShader = renderMan.getPass("SMapShader");
  pSMapShader->addVSConstantBuffer(m_pLCBuffer, 0);

  // Lightning shader buffers
  auto pLightCS = renderMan.getPass("LightCS");
  pLightCS->addCSConstantBuffer(m_pInvVP, 0);
  pLightCS->addCSConstantBuffer(m_pCameraPosition, 1);
  pLightCS->addCSConstantBuffer(m_pLightBuffer, 2);
  pLightCS->addCSConstantBuffer(m_pViewportBuffer, 3);
  pLightCS->addCSConstantBuffer(m_pLCBuffer, 4);
  pLightCS->addCSConstantBuffer(m_pLSizeBuffer, 5);

  // Skybox shader buffers
  auto pSkyBoxShader = renderMan.getPass("SkyBoxShader");
  pSkyBoxShader->addVSConstantBuffer(m_pVP, 0);

  // Final shader buffers
  auto pFinalShader = renderMan.getPass("FinalShader");
  pFinalShader->addPSConstantBuffer(m_pViewportBuffer, 0);

  // Histogram
  auto pHistogramShader = renderMan.getPass("HistogramShader");
  pHistogramShader->addCSConstantBuffer(m_pViewportBuffer, 0);

  // ToneMap
  ToneMap tm = {};
  tm.lutSize = 1.0f;
  tm.index = static_cast<float>(m_toneMapIndex);
  tm.whitePoint = m_whitePt;
  tm.exposure = m_exposure;

  m_pToneMapBuffer = graphMan.createConstantBuffer(sizeof(ToneMap));
  graphMan.updateConstantBuffer(m_pToneMapBuffer, &tm, sizeof(ToneMap));

  auto pToneMapShader = renderMan.getPass("ToneMapShader");
  pToneMapShader->addCSConstantBuffer(m_pViewportBuffer, 0);
  pToneMapShader->addCSConstantBuffer(m_pToneMapBuffer, 1);

  // Add skybox
  auto pASBShader = renderMan.getPass("ASBShader");
  pASBShader->addCSConstantBuffer(m_pViewportBuffer, 0);

  // Luminance
  BrightMap bm = {};
  bm.brightThreshold = m_brightT;
  m_pLuminanceBuffer = graphMan.createConstantBuffer(sizeof(BrightMap));
  graphMan.updateConstantBuffer(m_pLuminanceBuffer, &bm, sizeof(BrightMap));

  auto pLuminanceShader = renderMan.getPass("LuminanceShader");
  pLuminanceShader->addCSConstantBuffer(m_pViewportBuffer, 0);
  pLuminanceShader->addCSConstantBuffer(m_pLuminanceBuffer, 1);

  // Post Process
  PostProcessValues ppV = {};
  ppV.minR = m_minR / 255.0f;
  ppV.maxR = m_maxR / 255.0f;
  ppV.minG = m_minG / 255.0f;
  ppV.maxG = m_maxG / 255.0f;
  ppV.minB = m_minB / 255.0f;
  ppV.maxB = m_maxB / 255.0f;

  m_pPPBuffer = graphMan.createConstantBuffer(sizeof(PostProcessValues));
  graphMan.updateConstantBuffer(m_pPPBuffer, &ppV, sizeof(PostProcessValues));

  auto pPPShader = renderMan.getPass("PPShader");
  pPPShader->addCSConstantBuffer(m_pViewportBuffer, 0);
  pPPShader->addCSConstantBuffer(m_pPPBuffer, 1);

  // AddMix
  auto pAddMix = renderMan.getPass("AddMixShader");
  pAddMix->addCSConstantBuffer(m_pViewportBuffer, 0);

  // Create audio
  Path audioPath("resources/cat.wav");
  m_testSound = audioMan.createSound(audioPath);
  if(m_testSound)
    m_testSound->m_channel = CHANNEL_TYPE::kUI;
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
    graphMan.updateConstantBuffer(m_pLSizeBuffer, &camSize, sizeof(Vector4));
  }

  // Update ambient occlusion
  AOBuffer aoBuffer;
  aoBuffer.viewport.x = static_cast<float>(getScreenDescription().width);
  aoBuffer.viewport.y = static_cast<float>(getScreenDescription().height);
  aoBuffer.samplerRad = m_aoSamplerRad;
  aoBuffer.scale = m_aoScale;
  aoBuffer.bias = m_aoBias;
  aoBuffer.intensity = m_aoIntensity;

  graphMan.updateConstantBuffer(m_pAOBuffer, &aoBuffer, sizeof(AOBuffer));

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
  
  updateCamera();

  // Update audio
  if (bIsSoundPlaying) {
    audioMan.playSound(m_testSound);
    bIsSoundPlaying = false;
  }
  audioMan.update();

  // Update tone map
  ToneMap tm = {};
  tm.lutSize = 1.0f;
  tm.index = static_cast<float>(m_toneMapIndex);
  tm.whitePoint = m_whitePt;
  tm.exposure = m_exposure;

  graphMan.updateConstantBuffer(m_pToneMapBuffer, &tm, sizeof(ToneMap));

  BrightMap bm = {};
  bm.brightThreshold = m_brightT;
  graphMan.updateConstantBuffer(m_pLuminanceBuffer, &bm, sizeof(BrightMap));

  // Post Process
  PostProcessValues ppV = {};
  ppV.minR = m_minR / 255.0f;
  ppV.maxR = m_maxR / 255.0f;
  ppV.minG = m_minG / 255.0f;
  ppV.maxG = m_maxG / 255.0f;
  ppV.minB = m_minB / 255.0f;
  ppV.maxB = m_maxB / 255.0f;
  graphMan.updateConstantBuffer(m_pPPBuffer, &ppV, sizeof(PostProcessValues));
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
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  m_screenSize.x = static_cast<float>(rszData.width);
  m_screenSize.y = static_cast<float>(rszData.height);

  renderMan.setScreenSize(Vector2(m_screenSize.x, m_screenSize.y));
  renderMan.createRenderTextures();
  //setRenderTargets();

  m_camera.setPerspectiveData(m_camera.getHalfFOV(),
                              m_screenSize.x,
                              m_screenSize.y,
                              m_camera.getNear(),
                              m_camera.getFar());

  updateCamera();

  Vector4 viewport(m_screenSize.x,
                   m_screenSize.y,
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
}

void
RendererApp::initShaders()
{
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  // Init pass shaders
  // GBuffer
  auto pGbufferShader = sh_makeShared<Pass>();
  pGbufferShader->setVShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "main",
                                 "vs_5_0");
  pGbufferShader->setPShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "mainPS",
                                 "ps_5_0");
  pGbufferShader->compileShader();

  // Lightning
  auto pLightCS = sh_makeShared<Pass>();
  pLightCS->setCShaderInfo("resources/shaders/LightCShader.hlsl",
                           "CSMain",
                           "cs_5_0");
  pLightCS->compileShader();

  // AO
  auto pAOShader = sh_makeShared<Pass>();
  pAOShader->setPShaderInfo("resources/shaders/AOShader.hlsl",
                            "mainPS",
                            "ps_5_0");
  pAOShader->compileShader();

  // HBlur
  auto pHBlurShader = sh_makeShared<Pass>();
  pHBlurShader->setCShaderInfo("resources/shaders/HBlurShader.hlsl",
                               "CSMain",
                               "cs_5_0");
  pHBlurShader->compileShader();

  // VBlur
  auto pVBlurShader = sh_makeShared<Pass>();
  pVBlurShader->setCShaderInfo("resources/shaders/VBlurShader.hlsl",
                               "CSMain",
                               "cs_5_0");
  pVBlurShader->compileShader();

  // Shadow map
  auto pSMapShader = sh_makeShared<Pass>();
  pSMapShader->setVShaderInfo("resources/shaders/SMapShader.hlsl",
                              "main",
                              "vs_5_0");
  pSMapShader->setPShaderInfo("resources/shaders/SMapShader.hlsl",
                              "mainPS",
                              "ps_5_0");
  pSMapShader->compileShader();

  // Skybox
  auto pSkyBoxShader = sh_makeShared<Pass>();
  pSkyBoxShader->setVShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "main",
                                "vs_5_0");
  pSkyBoxShader->setPShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "mainPS",
                                "ps_5_0");
  pSkyBoxShader->compileShader();

  // Final shader
  auto pFinalShader = sh_makeShared<Pass>();
  pFinalShader->setPShaderInfo("resources/shaders/FinalShader.hlsl",
                               "mainPS",
                               "ps_5_0");
  pFinalShader->compileShader();

  // Plane Vertex shader
  auto pPlaneVS = sh_makeShared<Pass>();
  pPlaneVS->setVShaderInfo("resources/shaders/PlaneVertexShader.hlsl",
                           "main",
                           "vs_5_0");
  pPlaneVS->compileShader();

  // Histogram shader
  auto pHistogramShader = sh_makeShared<Pass>();
  pHistogramShader->setCShaderInfo("resources/shaders/HistogramShader.hlsl",
                                   "CSMain",
                                   "cs_5_0");
  pHistogramShader->compileShader();

  // Add skybox shader
  auto pASBShader = sh_makeShared<Pass>();
  pASBShader->setCShaderInfo("resources/shaders/AddSkyboxShader.hlsl",
                             "CSMain",
                             "cs_5_0");
  pASBShader->compileShader();

  // Luminance shader
  auto pLuminanceShader = sh_makeShared<Pass>();
  pLuminanceShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                   "LuminanceCS",
                                   "cs_5_0");
  pLuminanceShader->compileShader();

  // Luminance shader
  auto pBrightShader = sh_makeShared<Pass>();
  pBrightShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "BrightCS",
                                "cs_5_0");
  pBrightShader->compileShader();

  // ToneMap shader
  auto pToneMapShader = sh_makeShared<Pass>();
  pToneMapShader->setCShaderInfo("resources/shaders/ToneMappingShader.hlsl",
                                 "CSMain",
                                 "cs_5_0");
  pToneMapShader->compileShader();

  // PostProcess shader
  auto pAddMixShader = sh_makeShared<Pass>();
  pAddMixShader->setCShaderInfo("resources/shaders/AdditiveMixShader.hlsl",
                                "CSMain",
                                "cs_5_0");
  pAddMixShader->compileShader();

  // PostProcess shader
  auto pPPShader = sh_makeShared<Pass>();
  pPPShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "CSMain",
                            "cs_5_0");
  pPPShader->compileShader();

  // Raster state
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

  // Blend state
  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  // Basic depth stencil state
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

  // Depth stencil state for sky box
  DepthStencilDesc skyBoxDepth = {};
  skyBoxDepth.depthEnable = false;
  skyBoxDepth.depthWriteMask = DEPTH_WRITE_MASK::kZero;
  skyBoxDepth.depthFunc = COMPARISON_FUNC::kLessEqual;
  skyBoxDepth.stencilEnable = true;
  skyBoxDepth.stencilReadMask = 0xFF;
  skyBoxDepth.stencilWriteMask = 0xFF;
  skyBoxDepth.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  skyBoxDepth.frontFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  skyBoxDepth.backFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  skyBoxDepth.backFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Dpeth stencil state for planes
  DepthStencilDesc planeDepthSDesc = depthSDesc;
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.stencilEnable = false;

  // States creation
  auto pSamplerLinear = graphMan.createSamplerState();
  auto pRasterState = graphMan.createRasterizerState(rasterDesc);
  auto pBlendState = graphMan.createBlendState(blendDesc);
  auto pDepthStencil = graphMan.createDepthStencilState(depthSDesc);

  // Fill pass info
  // GBuffer
  pGbufferShader->generateInputLayout();
  pGbufferShader->setSamplerState(pSamplerLinear);
  pGbufferShader->setRasterizerState(pRasterState);
  pGbufferShader->setBlendState(pBlendState);
  pGbufferShader->setDepthStencilState(pDepthStencil);

  // AO
  pAOShader->setSamplerState(pSamplerLinear);

  // Shadow Map
  pSMapShader->generateInputLayout();
  pSMapShader->setSamplerState(pSamplerLinear);
  pSMapShader->setRasterizerState(pRasterState);
  pSMapShader->setBlendState(pBlendState);
  pSMapShader->setDepthStencilState(pDepthStencil);

  // SkyBox Map
  pSkyBoxShader->generateInputLayout();
  pSkyBoxShader->setSamplerState(pSamplerLinear);
  pSkyBoxShader->setRasterizerState(pRasterState);
  pSkyBoxShader->setBlendState(pBlendState);
  pSkyBoxShader->setDepthStencilStateFromDesc(skyBoxDepth);

  // Final
  pFinalShader->setSamplerState(pSamplerLinear);

  // Plane vs
  pPlaneVS->generateInputLayout();
  pPlaneVS->setDepthStencilStateFromDesc(planeDepthSDesc);

  // ToneMap
  pToneMapShader->setSamplerState(pSamplerLinear);

  // Save passes on render manager
  //renderMan.addPass(pGbufferShader, "GBufferShader");
  //renderMan.addPass(pAOShader, "AOShader");
  //renderMan.addPass(pHBlurShader, "HBlurShader");
  //renderMan.addPass(pVBlurShader, "VBlurShader");
  //renderMan.addPass(pLightCS, "LightCS");
  //renderMan.addPass(pSMapShader, "SMapShader");
  //renderMan.addPass(pSkyBoxShader, "SkyBoxShader");
  //renderMan.addPass(pFinalShader, "FinalShader");
  //renderMan.addPass(pPlaneVS, "PlaneShader");
  //renderMan.addPass(pHistogramShader, "HistogramShader");
  //renderMan.addPass(pASBShader, "ASBShader");
  //renderMan.addPass(pLuminanceShader, "LuminanceShader");
  //renderMan.addPass(pToneMapShader, "ToneMapShader");
  //renderMan.addPass(pPPShader, "PPShader");
  //renderMan.addPass(pAddMixShader, "AddMixShader");
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
                    m_screenSize.x,
                    m_screenSize.y,
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
  Vector4 viewport(m_screenSize.x,
                   m_screenSize.y,
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

  Vector4 lightS = { m_lightCam.getWidth(), 0.0f,0.0f,0.f };
  m_pLSizeBuffer = graphMan.createConstantBuffer(sizeof(Vector4));
  graphMan.updateConstantBuffer(m_pLSizeBuffer, &lightS, sizeof(Vector4));
}

void
RendererApp::setRenderTargets()
{
  //GraphicsManager& graphMan = g_graphicsMan();
  //RenderManager& renderMan = g_renderMan();

  //uint32 width = static_cast<uint32>(m_screenSize.x);
  //uint32 height = static_cast<uint32>(m_screenSize.y);

  //auto pDepthTarget = graphMan.createTexture2D(width,
  //                                             height,
  //                                             TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
  //                                             USAGE::kDefault,
  //                                             BIND_FLAGS::kRenderTarget |
  //                                             BIND_FLAGS::kShaderResource);

  //auto pNormalTarget = graphMan.createTexture2D(width,
  //                                              height,
  //                                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                              USAGE::kDefault,
  //                                              BIND_FLAGS::kRenderTarget |
  //                                              BIND_FLAGS::kShaderResource);

  //auto pColorTarget = graphMan.createTexture2D(width,
  //                                             height,
  //                                             TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                             USAGE::kDefault,
  //                                             BIND_FLAGS::kRenderTarget |
  //                                             BIND_FLAGS::kShaderResource);

  //auto pPropTarget = graphMan.createTexture2D(width,
  //                                            height,
  //                                            TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                            USAGE::kDefault,
  //                                            BIND_FLAGS::kRenderTarget |
  //                                            BIND_FLAGS::kShaderResource);

  //auto pAoTarget = graphMan.createTexture2D(width,
  //                                          height,
  //                                          TEXTURE_FORMAT::kR16_FLOAT,
  //                                          USAGE::kDefault,
  //                                          BIND_FLAGS::kRenderTarget |
  //                                          BIND_FLAGS::kShaderResource);

  //auto pHbTarget = graphMan.createTexture2D(width,
  //                                          height,
  //                                          TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                          USAGE::kDefault,
  //                                          BIND_FLAGS::kShaderResource |
  //                                          BIND_FLAGS::kUnorderedAccess);

  //auto pVbTarget = graphMan.createTexture2D(width,
  //                                          height,
  //                                          TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                          USAGE::kDefault,
  //                                          BIND_FLAGS::kShaderResource |
  //                                          BIND_FLAGS::kUnorderedAccess);

  //auto pSMapTarget = graphMan.createTexture2D(static_cast<uint32>(m_shadowTexSize),
  //                                            static_cast<uint32>(m_shadowTexSize),
  //                                            TEXTURE_FORMAT::kR32_TYPELESS,
  //                                            USAGE::kDefault,
  //                                            BIND_FLAGS::kDepthStencil |
  //                                            BIND_FLAGS::kShaderResource);

  //auto pShadowTempTarget = graphMan.createTexture2D(static_cast<uint32>(m_shadowTexSize),
  //                                                  static_cast<uint32>(m_shadowTexSize),
  //                                                  TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                                  USAGE::kDefault,
  //                                                  BIND_FLAGS::kRenderTarget |
  //                                                  BIND_FLAGS::kShaderResource);

  //auto pSkyBoxTarget = graphMan.createTexture2D(width,
  //                                              height,
  //                                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                              USAGE::kDefault,
  //                                              BIND_FLAGS::kRenderTarget |
  //                                              BIND_FLAGS::kShaderResource);

  //auto pComputeLight = graphMan.createTexture2D(width,
  //                                              height,
  //                                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                              USAGE::kDefault,
  //                                              BIND_FLAGS::kShaderResource |
  //                                              BIND_FLAGS::kUnorderedAccess);

  //auto pHistogramMap = graphMan.createTexture2D(256,
  //                                              3,
  //                                              TEXTURE_FORMAT::kR32_UINT,
  //                                              USAGE::kDefault,
  //                                              BIND_FLAGS::kShaderResource |
  //                                              BIND_FLAGS::kUnorderedAccess);

  //auto pToneMap = graphMan.createTexture2D(width,
  //                                         height,
  //                                         TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                         USAGE::kDefault,
  //                                         BIND_FLAGS::kShaderResource |
  //                                         BIND_FLAGS::kUnorderedAccess);

  //auto pTempMap = graphMan.createTexture2D(width,
  //                                         height,
  //                                         TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                         USAGE::kDefault,
  //                                         BIND_FLAGS::kShaderResource |
  //                                         BIND_FLAGS::kUnorderedAccess);

  //auto pLuminance = graphMan.createTexture2D(512,
  //                                           512,
  //                                           TEXTURE_FORMAT::kR32_FLOAT,
  //                                           USAGE::kDefault,
  //                                           BIND_FLAGS::kShaderResource |
  //                                           BIND_FLAGS::kUnorderedAccess);

  //auto pBright = graphMan.createTexture2D(512,
  //                                        512,
  //                                        TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
  //                                        USAGE::kDefault,
  //                                        BIND_FLAGS::kShaderResource |
  //                                        BIND_FLAGS::kUnorderedAccess);

  //auto pPPMap = graphMan.createTexture2D(width,
  //                                       height,
  //                                       TEXTURE_FORMAT::kR8G8B8A8_UNORM,
  //                                       USAGE::kDefault,
  //                                       BIND_FLAGS::kShaderResource |
  //                                       BIND_FLAGS::kUnorderedAccess);

  //// Save targets on render manager
  //renderMan.addRenderTarget(pDepthTarget, "DepthMap");
  //renderMan.addRenderTarget(pNormalTarget, "NormalMap");
  //renderMan.addRenderTarget(pColorTarget, "ColorMap");
  //renderMan.addRenderTarget(pPropTarget, "PropMap");
  //renderMan.addRenderTarget(pAoTarget, "AOMap");
  //renderMan.addRenderTarget(pHbTarget, "HBlurMap");
  //renderMan.addRenderTarget(pVbTarget, "VBlurMap");
  //renderMan.addRenderTarget(pSMapTarget, "ShadowMap");
  //renderMan.addRenderTarget(pShadowTempTarget, "ShadowTemp");
  //renderMan.addRenderTarget(pComputeLight, "LightCMap");
  //renderMan.addRenderTarget(pSkyBoxTarget, "SkyBoxMap");
  //renderMan.addRenderTarget(pHistogramMap, "HistogramMap");
  //renderMan.addRenderTarget(pTempMap, "TempMap");
  //renderMan.addRenderTarget(pLuminance, "LuminanceMap");
  //renderMan.addRenderTarget(pToneMap, "ToneMap");
  //renderMan.addRenderTarget(pPPMap, "PPMap");
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
  }

  if (ImGui::Button("Play Sound")) {
    bIsSoundPlaying = true;
  }

  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pTex), ImVec2(160, 80));

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
  ImGui::DragFloat("BrightThreshold:", &m_brightT, 0.01f, 0.0f, 1.0f);
  ImGui::Spacing();
  ImGui::DragFloat("WhitePoint:", &m_whitePt, 0.01f, 0.5f, 11.2f);
  ImGui::Spacing();
  ImGui::DragFloat("Exposure:", &m_exposure, 0.01f, 0.5f, 2.0f);
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
RendererApp::loadPistol()
{
  ResourceManager& resourceMan = g_resourceMan();
  SceneGraph& sceneG = g_sceneGraph();

  auto modelRes = sh_reinterpretPCast<StaticMeshResource>(
                  resourceMan.loadModelFromCache("resources/assets/models/DrakeFire.sha"));

  auto model = sh_makeShared<GameObject>();
  model->name = "DrakeFire";
  auto modelMC = sh_makeShared<StaticMeshComponent>();

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
