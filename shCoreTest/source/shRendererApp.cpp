/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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

  initGraphicAssets();
  initCamera();
  audioMan.initSystem();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());

  ImGui::StyleColorsDark();

  // Load images
  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  Path text1("resources/base_albedo.png");
  auto baseColor = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(text1));
  Path text2("resources/base_normal.png");
  auto normal = reinterpret_pointer_cast<ImageResource>(
                resourceMan.loadResourceFromFile(text2));

  Path text3("resources/base_metallic.png");
  auto metallic = reinterpret_pointer_cast<ImageResource>(
                  resourceMan.loadResourceFromFile(text3));

  Path text4("resources/base_roughness.png");
  auto roughness = reinterpret_pointer_cast<ImageResource>(
                   resourceMan.loadResourceFromFile(text4));

  Path text5("resources/base_AO.png");
  auto ao = reinterpret_pointer_cast<ImageResource>(
            resourceMan.loadResourceFromFile(text5));

  // Load model and set the gameobject
  Path modelPath("resources/DrakeFire.fbx");
  auto modelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                  resourceMan.loadResourceFromFile(modelPath));

  Path sponzaPath("resources/Models/Sponza.fbx");
  auto sponzaModelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                        resourceMan.loadResourceFromFile(sponzaPath));

  auto modelMat = reinterpret_pointer_cast<PBRMaterial>(modelRes->materials[0]);
  modelMat->baseColor = baseColor->texture;
  modelMat->normal = normal->texture;
  modelMat->metallic = metallic->texture;
  modelMat->roughness = roughness->texture;
  modelMat->ao = ao->texture;

  m_pModel = make_shared<GameObject>();
  m_pModel->name = "DrakeFire";
  auto modelMC = make_shared<StaticMeshUnionComponent>();

  modelMC->setMeshData(modelRes);
  m_pModel->addComponent(modelMC);

  m_pModel->transform.getTransform().m[3][3] = 1.0f;

  m_pModel->setPosition(Vector3::ZERO);
  m_pModel->setScale(Vector3::ONE);
  m_pModel->setRotation(Vector3::ZERO);

  g_sceneGraph().addObject(m_pModel);

  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Transform));

  graphMan.updateConstantBuffer(m_pModelTransform,
                                &m_pModel->transform.getTransform(),
                                sizeof(Transform));

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

  m_lightCam = Camera(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z),
                      Vector3::ZERO,
                      Vector3::UP,
                      screenW,
                      screenH,
                      0.1f,
                      100.0f);

  VP lcam = {};
  lcam.proj = m_lightCam.getProjection();
  lcam.view = m_lightCam.getView();
  lcam.proj.getTransposed();
  lcam.view.getTransposed();

  m_pLCBuffer = graphMan.createConstantBuffer(sizeof(VP));
  graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));

  auto pSMapShader = renderMan.getPass("SMapShader");
  pSMapShader->addVSConstantBuffer(m_pLCBuffer);
  pSMapShader->addVSConstantBuffer(m_pModelTransform);

  Path audioPath("resources/cat.wav");
  m_testSound = audioMan.createSound(audioPath);
}

void
RendererApp::onUpdate()
{
  GraphicsManager& graphMan = g_graphicsMan();

  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();

  ImGui_ImplShura_AddMouseWheelEvent(m_hdelta, m_delta);
  m_delta = 0.0f;
  m_hdelta = 0.0f;

  m_modelPos = m_pModel->getPosition();
  m_modelRot = m_pModel->getRotation() * Math::RAD2DEG;
  m_modelScale = m_pModel->getScale();
  
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

  graphMan.updateConstantBuffer(m_pModelTransform,
                                &m_pModel->transform.getTransform(),
                                sizeof(Transform));

  if (lights[0] != m_lightPos) {
    lights[0] = m_lightPos;
    graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

    m_lightCam.setViewData(Vector3(m_lightPos.x, m_lightPos.y, m_lightPos.z),
                           Vector3::ZERO,
                           Vector3::UP);

    VP lcam = {};
    lcam.proj = m_lightCam.getProjection();
    lcam.view = m_lightCam.getView();
    lcam.proj.getTransposed();
    lcam.view.getTransposed();

    graphMan.updateConstantBuffer(m_pLCBuffer, &lcam, sizeof(VP));
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

  const float camSpeed = 0.01f;

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
}

void
RendererApp::onRender()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  graphMan.setPrimitiveTopology();
  auto pDepthSV = graphMan.getMainDepthStencil();

  // Shadow Mapping
  graphMan.clearRenderTarget(m_pSMapTarget, LinearColor(0.0f, 0.0f, 0.0f));
  renderMan.makePass("SMapShader");

  graphMan.setRenderTargets({ m_pSMapTarget }, pDepthSV);

  auto& smucList = g_sceneGraph().getStaticMeshUnionComponentInScene();
  renderMan.drawSMUInScene(smucList);

  // Gbuffer pass
  graphMan.clearRenderTarget(m_pDepthTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_pNormalTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearRenderTarget(m_pColorTarget, LinearColor(0.0f, 0.0f, 0.0f));
  graphMan.clearDepthStencil(pDepthSV);

  graphMan.setRenderTargets({m_pDepthTarget, m_pNormalTarget, m_pColorTarget }, pDepthSV);
  renderMan.makePass("BasicShader");

  //auto& smucList = g_sceneGraph().getStaticMeshUnionComponentInScene();
  renderMan.drawSMUInScene(smucList);

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
  
  graphMan.draw(3, 0);

  graphMan.setShaderResourceView(nullptr, 0);
  graphMan.setShaderResourceView(nullptr, 1);
  graphMan.setShaderResourceView(nullptr, 2);
  graphMan.setShaderResourceView(nullptr, 3);

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
  AudioManager& audioMan = AudioManager::instance();
  audioMan.close();

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
                    100.0f);

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
  const float speed = 0.05f;

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
  m_camera.update();

  // Update camera buffer
  VP vp = {};
  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();
  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));

  // Update camera position buffer
  Vector4 foward(m_camera.getPosition(), 0.0f);
  g_graphicsMan().updateConstantBuffer(m_pCameraPosition,
                                       &foward,
                                       sizeof(Vector4));

  // Update inverse view-projection buffer
  InvVP invVP = {};
  invVP.invVP = (vp.proj * vp.view).getInversed();
  invVP.invV = vp.view.getInversed();
  g_graphicsMan().updateConstantBuffer(m_pInvVP,
                                       &invVP,
                                       sizeof(InvVP));
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
                              BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_pNormalTarget = graphMan.createTexture2D(getScreenDescription().width,
                               getScreenDescription().height,
                               TEXTURE_FORMAT::kR8G8B8A8_unorm,
                               USAGE::kDefault,
                               BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_pColorTarget = graphMan.createTexture2D(getScreenDescription().width,
                              getScreenDescription().height,
                              TEXTURE_FORMAT::kR8G8B8A8_unorm,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  //m_targets.push_back(depthTarget);
  //m_targets.push_back(normalTarget);
  //m_targets.push_back(colorTarget);

  m_pAoTarget = graphMan.createTexture2D(getScreenDescription().width,
                           getScreenDescription().height,
                           TEXTURE_FORMAT::kR16_FLOAT,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  //m_aoTarget.push_back(aoTarget);

  m_pHbTarget = graphMan.createTexture2D(getScreenDescription().width,
                           getScreenDescription().height,
                           TEXTURE_FORMAT::kR8G8B8A8_unorm,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  //m_hbTarget.push_back(hbTarget);

  m_pVbTarget = graphMan.createTexture2D(getScreenDescription().width,
                           getScreenDescription().height,
                           TEXTURE_FORMAT::kR8G8B8A8_unorm,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  //m_vbTarget.push_back(vbTarget);

  m_pSMapTarget = graphMan.createTexture2D(getScreenDescription().width,
                           getScreenDescription().height,
                           TEXTURE_FORMAT::kR32G32B32A32_float,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);
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

  ImGui::SetNextWindowPos(ImVec2(width - 300.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(300.0f, height));
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

    // Light Position
    ImGui::Text("Light Pos:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##LPosX", &m_lightPos.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##LPosY", &m_lightPos.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##LPosZ", &m_lightPos.z, 0.1f);
    ImGui::PopStyleColor(3);
  }
  ImGui::PopStyleColor(3);

  ImGui::InputFloat("Light Intensity", &m_lIntensity);

  static char buf[32] = "hello";
  ImGui::InputText("TestingKeys", buf, 32);

  if (ImGui::Button("Play Sound")) {
    AudioManager& audioMan = AudioManager::instance();
    audioMan.playSound(m_testSound);
  }

  ImGui::End();
}
}
