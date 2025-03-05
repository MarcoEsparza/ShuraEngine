/*****************************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/07
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
#include "shTime.h"
#include "shSceneGraph.h"
#include "shMath.h"
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
struct AOBuffer {
  Vector2 viewport;
  float samplerRad = 0.0f;
  float scale = 0.0f;
  float bias = 0.0f;
  float intensity = 0.0f;
  Vector2 unused;
};

void
RendererApp::onCreate()
{
  GraphicsManager& graphMan = g_graphicsMan();
  initGraphicAssets();
  initCamera();

  /*auto pGuiRTV = graphMan.createTexture2D(m_desc.width,
                                          m_desc.height,
                                          TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                          USAGE::kDefault,
                                          BIND_FLAGS::kRenderTarget);
  m_guiTarget.push_back(pGuiRTV);*/

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());

  ImGui::StyleColorsDark();

  // Load images
  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  Path text1("resources/base_albedo.png");
  auto baseColor = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text1)); 
  Path text2("resources/base_normal.png");
  auto normal = reinterpret_pointer_cast<ImageResource>(
                g_resourceMan().loadResourceFromFile(text2));

  Path text3("resources/base_metallic.png");
  auto metallic = reinterpret_pointer_cast<ImageResource>(
                  g_resourceMan().loadResourceFromFile(text3));

  Path text4("resources/base_roughness.png");
  auto roughness = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text4));

  Path text5("resources/base_AO.png");
  auto ao = reinterpret_pointer_cast<ImageResource>(
            g_resourceMan().loadResourceFromFile(text5));

  // Load model and set the gameobject
  Path modelPath("resources/DrakeFire.fbx");
  auto modelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                  g_resourceMan().loadResourceFromFile(modelPath));

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
  lights[0] = Vector4(0.0f, 2.0f, 0.0f, 1.0f);


  //m_light.position = Vector3(5.0f, 5.0f, 5.0f);
  //m_light.color = LinearColor(1.0f, 1.0f, 1.0f);
  //m_light.intensity = 1.0f;

  m_pLightBuffer = graphMan.createConstantBuffer(sizeof(lights));

  graphMan.updateConstantBuffer(m_pLightBuffer, lights.data(), sizeof(lights));

  // Set pass buffers
  m_pBasicShader->addVSConstantBuffer(m_pVP);
  m_pBasicShader->addVSConstantBuffer(m_pModelTransform);

  m_pDeferredShader->addPSConstantBuffer(m_pInvVP);
  m_pDeferredShader->addPSConstantBuffer(m_pCameraPosition);
  m_pDeferredShader->addPSConstantBuffer(m_pLightBuffer);
  m_pDeferredShader->addPSConstantBuffer(m_pViewportBuffer);

  AOBuffer aoBuffer;
  aoBuffer.viewport.x = m_desc.width;
  aoBuffer.viewport.y = m_desc.height;
  aoBuffer.samplerRad = 1.0f;
  aoBuffer.scale = 1.0f;
  aoBuffer.bias = 1.0f;
  aoBuffer.intensity = 1.0f;

  auto aoCBuffer = graphMan.createConstantBuffer(sizeof(AOBuffer));
  graphMan.updateConstantBuffer(aoCBuffer, &aoBuffer, sizeof(AOBuffer));
  m_pAOShader->addPSConstantBuffer(aoCBuffer);

  m_pHBlurShader->addPSConstantBuffer(m_pViewportBuffer);
  m_pVBlurShader->addPSConstantBuffer(m_pViewportBuffer);

  m_mainTarget.push_back(graphMan.getMainRenderTargetView());
}

void
RendererApp::onUpdate()
{
  ImGui_ImplShura_NewFrame(m_currentMousePos,
                           m_bLeftClick,
                           m_delta,
                           m_hdelta,
                           m_bKeyTest,
                           m_key);
  ImGui::NewFrame();
  m_delta = 0.0f;
  m_hdelta = 0.0f;
  
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(250.0f, static_cast<float>(m_desc.height)));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Scenegraph",
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  ImGui::PopStyleColor();
  ImGui::Text(m_pModel->name.c_str());
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(1100.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(300.0f, static_cast<float>(m_desc.height)));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin(m_pModel->name.c_str(),
               0,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoResize);
  ImGui::PopStyleColor();

  ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(227, 187, 41, 0xff));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(247, 200, 70, 0xff));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(207, 167, 20, 0xff));
  if (ImGui::CollapsingHeader("Transform")) {
    ImGui::Text("Position:");
    ImGui::SameLine(90.0f);
    float pos[3] = { m_pModel->getPosition().x,
                     m_pModel->getPosition().y,
                     m_pModel->getPosition().z };
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::InputFloat3("##Positions", pos);
    ImGui::PopStyleColor();
    ImGui::Spacing();
    ImGui::Text("Rotation:");
    ImGui::SameLine(90.0f);
    float rot[3] = { m_pModel->getRotation().x,
                     m_pModel->getRotation().y,
                     m_pModel->getRotation().z };
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::InputFloat3("##Rotations", rot);
    ImGui::PopStyleColor();
    ImGui::Spacing();
    ImGui::Text("Scale:");
    ImGui::SameLine(90.0f);
    float scl[3] = { m_pModel->getScale().x,
                     m_pModel->getScale().y,
                     m_pModel->getScale().z };
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::InputFloat3("##Scales", scl);
    ImGui::PopStyleColor();
  }
  ImGui::PopStyleColor(3);

  ImGui::End();

  if (m_bLeftClick) {
    rotateCamera();
  }

  const float speed = 0.01f;

  if (m_bFoward) {
    m_camera.move(Vector3(0.0f, 0.0f, 0.1f) * speed);
  }
  
  if (m_bLeft) {
    m_camera.move(Vector3(-0.1f, 0.0f, 0.0f) * speed);
  }

  if (m_bBack) {
    m_camera.move(Vector3(0.0f, 0.0f, -0.1f) * speed);
  }
  
  if (m_bRight) {
    m_camera.move(Vector3(0.1f, 0.0f, 0.0f) * speed);
  }

  if (m_bUp) {
    m_camera.move(Vector3(0.0f, 0.1f, 0.0f) * speed);
  }

  if (m_bDown) {
    m_camera.move(Vector3(0.0f, -0.1f, 0.0f) * speed);
  }

  const float rotSpeed = 15.0f;
  const float rotAngle = rotSpeed * g_time().getFrameDeltaTime() * Math::DEG2RAD;
  if (m_bRotLeft) {
    m_pModel->rotate(Vector3(0.0f, 1.0f, 0.0f), -rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &m_pModel->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_bRotRight) {
    m_pModel->rotate(Vector3(0.0f, 1.0f, 0.0f), rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &m_pModel->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_bRotUp) {
    m_pModel->rotate(Vector3(1.0f, 0.0f, 0.0f), rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &m_pModel->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_bRotDown) {
    m_pModel->rotate(Vector3(1.0f, 0.0f, 0.0f), -rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &m_pModel->transform.getTransform(),
                                         sizeof(Transform));
  }
  
  
  updateCamera();
}

void
RendererApp::onRender()
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.setPrimitiveTopology();

  // Gbuffer pass
  for (auto& target : m_targets) {
    graphMan.clearRenderTarget(target, LinearColor(0.0f, 0.0f, 0.0f));
  }

  graphMan.setRenderTargets(m_targets, graphMan.getMainDepthStencil());
  m_pBasicShader->setPass();

  auto& smucList = g_sceneGraph().getStaticMeshUnionComponentInScene();
  g_renderMan().drawStaticMeshUnionInScene(smucList);

  graphMan.setShaderResourceView(nullptr, 2);
  graphMan.setShaderResourceView(nullptr, 3);
  graphMan.setShaderResourceView(nullptr, 4);

  // Ambient Occlusion pass
  for (auto& target : m_aoTarget) {
    graphMan.clearRenderTarget(target, LinearColor(0.0f, 0.0f, 0.0f));
  }
  graphMan.setRenderTargets(m_aoTarget, graphMan.getMainDepthStencil());
  m_pAOShader->setPass();

  graphMan.setShaderResourceView(m_targets[0], 0);
  graphMan.setShaderResourceView(m_targets[1], 1);

  graphMan.draw(3, 0);

  graphMan.setShaderResourceView(nullptr, 1);

  // Horizontal Blur pass
  for (auto& target : m_hbTarget) {
    graphMan.clearRenderTarget(target, LinearColor(0.0f, 0.0f, 0.0f));
  }
  graphMan.setRenderTargets(m_hbTarget, graphMan.getMainDepthStencil());
  m_pHBlurShader->setPass();

  graphMan.setShaderResourceView(m_aoTarget[0], 0);

  graphMan.draw(3, 0);

  // Vetical Blur pass
  for (auto& target : m_vbTarget) {
    graphMan.clearRenderTarget(target, LinearColor(0.0f, 0.0f, 0.0f));
  }
  graphMan.setRenderTargets(m_vbTarget, graphMan.getMainDepthStencil());
  m_pVBlurShader->setPass();

  graphMan.setShaderResourceView(m_hbTarget[0], 0);

  graphMan.draw(3, 0);

  // Deferred pass
  graphMan.setRenderTargets(m_mainTarget, graphMan.getMainDepthStencil());
  m_pDeferredShader->setPass();

  graphMan.setShaderResourceView(m_targets[0], 0);
  graphMan.setShaderResourceView(m_targets[1], 1);
  graphMan.setShaderResourceView(m_targets[2], 2);
  graphMan.setShaderResourceView(m_vbTarget[0], 3);
  
  graphMan.draw(3, 0);

  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
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

  m_key = key;
  m_bKeyTest = true;
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
    m_pBasicShader->compileShader();
    m_pAOShader->compileShader();
    m_pHBlurShader->compileShader();
    m_pVBlurShader->compileShader();
    m_pDeferredShader->compileShader();
  }

  m_key = key;
  m_bKeyTest = false;
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
}

void
RendererApp::onMouseMove(const MouseMoveData& mouse)
{
  m_lastMousePos = m_currentMousePos;
  m_currentMousePos.x = static_cast<float>(mouse.x);
  m_currentMousePos.y = static_cast<float>(mouse.y);
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

  m_pBasicShader.reset();
  m_pDeferredShader.reset();

  for (auto& pTex : m_targets) {
    pTex.reset();
  }
  for (auto& pTex : m_mainTarget) {
    pTex.reset();
  }

  m_pVP.reset();
  m_pInvVP.reset();
  m_pModelTransform.reset();
  m_pCameraPosition.reset();
  m_pLightBuffer.reset();
  m_pViewportBuffer.reset();
}

void
RendererApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));
  GraphicsManager& graphMan = g_graphicsMan();

  // Init pass shaders
  m_pBasicShader = make_unique<Pass>();
  m_pBasicShader->setShaderInfo("resources/shaders/BasicShader.hlsl",
                                "main",
                                "mainPS",
                                "vs_5_0",
                                "ps_5_0");
  m_pBasicShader->compileShader();

  m_pDeferredShader = make_unique<Pass>();
  m_pDeferredShader->setShaderInfo("resources/shaders/DeferredShader.hlsl",
                                   "main",
                                   "mainPS",
                                   "vs_5_0",
                                   "ps_5_0");
  m_pDeferredShader->compileShader();

  m_pAOShader = make_unique<Pass>();
  m_pAOShader->setShaderInfo("resources/shaders/AOShader.hlsl",
                             "main",
                             "mainPS",
                             "vs_5_0",
                             "ps_5_0");
  m_pAOShader->compileShader();

  m_pHBlurShader = make_unique<Pass>();
  m_pHBlurShader->setShaderInfo("resources/shaders/HBlurShader.hlsl",
                                "main",
                                "mainPS",
                                "vs_5_0",
                                "ps_5_0");
  m_pHBlurShader->compileShader();

  m_pVBlurShader = make_unique<Pass>();
  m_pVBlurShader->setShaderInfo("resources/shaders/VBlurShader.hlsl",
                                "main",
                                "mainPS",
                                "vs_5_0",
                                "ps_5_0");
  m_pVBlurShader->compileShader();

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

  DepthStencilDesc planeDepthSDesc = {};
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  planeDepthSDesc.depthFunc = COMPARISON_FUNC::kLess;
  planeDepthSDesc.stencilEnable = false;
  planeDepthSDesc.stencilReadMask = 0xFF;
  planeDepthSDesc.stencilWriteMask = 0xFF;
  planeDepthSDesc.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  planeDepthSDesc.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  planeDepthSDesc.frontFace.stencilPassOp = STENCIL_OP::kKeep;
  planeDepthSDesc.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  planeDepthSDesc.backFace.stencilFailOp = STENCIL_OP::kKeep;
  planeDepthSDesc.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  planeDepthSDesc.backFace.stencilPassOp = STENCIL_OP::kKeep;
  planeDepthSDesc.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Fill pass info
  m_pBasicShader->generateInputLayout();
  auto pSamplerLinear = graphMan.createSamplerState();
  m_pBasicShader->setSamplerState(pSamplerLinear);
  m_pBasicShader->setRasterizerState(rasterDesc);
  m_pBasicShader->setBlendState(blendDesc);
  m_pBasicShader->setDepthStencilState(depthSDesc);

  m_pDeferredShader->generateInputLayout();
  m_pDeferredShader->setSamplerState(pSamplerLinear);
  m_pDeferredShader->setDepthStencilState(planeDepthSDesc);

  m_pAOShader->generateInputLayout();
  m_pAOShader->setSamplerState(pSamplerLinear);
  m_pAOShader->setDepthStencilState(planeDepthSDesc);

  m_pHBlurShader->generateInputLayout();
  m_pHBlurShader->setSamplerState(pSamplerLinear);
  m_pHBlurShader->setDepthStencilState(planeDepthSDesc);

  m_pVBlurShader->generateInputLayout();
  m_pVBlurShader->setSamplerState(pSamplerLinear);
  m_pVBlurShader->setDepthStencilState(planeDepthSDesc);

  // Create and set render targets for deferred rendering
  auto depthTarget = graphMan.createTexture2D(m_desc.width,
                              m_desc.height,
                              TEXTURE_FORMAT::kR32G32B32A32_float,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  auto normalTarget = graphMan.createTexture2D(m_desc.width,
                               m_desc.height,
                               TEXTURE_FORMAT::kR8G8B8A8_unorm,
                               USAGE::kDefault,
                               BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  auto colorTarget = graphMan.createTexture2D(m_desc.width,
                              m_desc.height,
                              TEXTURE_FORMAT::kR8G8B8A8_unorm,
                              USAGE::kDefault,
                              BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_targets.push_back(depthTarget);
  m_targets.push_back(normalTarget);
  m_targets.push_back(colorTarget);

  auto aoTarget = graphMan.createTexture2D(m_desc.width,
                           m_desc.height,
                           TEXTURE_FORMAT::kR16_FLOAT,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_aoTarget.push_back(aoTarget);

  auto hbTarget = graphMan.createTexture2D(m_desc.width,
                           m_desc.height,
                           TEXTURE_FORMAT::kR8G8B8A8_unorm,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_hbTarget.push_back(hbTarget);

  auto vbTarget = graphMan.createTexture2D(m_desc.width,
                           m_desc.height,
                           TEXTURE_FORMAT::kR8G8B8A8_unorm,
                           USAGE::kDefault,
                           BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_vbTarget.push_back(vbTarget);
}

void
RendererApp::initCamera()
{
  // Init camera and its constant buffer
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp;

  m_camera = Camera(Vector3(0.0f, 0.0f, -3.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    30.0f * Math::DEG2RAD,
                    static_cast<float>(m_desc.width),
                    static_cast<float>(m_desc.height),
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
  InvVP invVP;

  m_pInvVP = g_graphicsMan().createConstantBuffer(sizeof(InvVP));

  invVP.invVP = (m_camera.getProjection() * m_camera.getView()).getInversed();
  invVP.invV = m_camera.getView().getInversed();

  invVP.invVP.getTransposed();
  invVP.invV.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pInvVP,
                                       &invVP,
                                       sizeof(InvVP));

  // Init buffer for viewport
  Vector4 viewport(static_cast<float>(m_desc.width),
                   static_cast<float>(m_desc.height),
                   m_camera.getFar(),
                   m_camera.getNear());
  m_pViewportBuffer = g_graphicsMan().createConstantBuffer(sizeof(Vector4));
  g_graphicsMan().updateConstantBuffer(m_pViewportBuffer, &viewport, sizeof(Vector4));
}

void
RendererApp::rotateCamera()
{
  const float speed = 0.005f;

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
  VP vp;
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
  InvVP invVP;
  invVP.invVP = (vp.proj * vp.view).getInversed();
  invVP.invV = vp.view.getInversed();
  g_graphicsMan().updateConstantBuffer(m_pInvVP,
                                       &invVP,
                                       sizeof(InvVP));
}
}
