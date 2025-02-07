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
void
RendererApp::onCreate()
{
  initGraphicAssets();
  initCamera();

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
  auto modelMC = make_shared<StaticMeshUnionComponent>();

  modelMC->setMeshData(modelRes);
  m_pModel->addComponent(modelMC);

  m_pModel->transform.getTransform().m[3][3] = 1.0f;

  m_pModel->setPosition(Vector3(0.0f, 0.0f, 0.0f));
  m_pModel->setScale(Vector3(1.0f, 1.0f, 1.0f));
  m_pModel->setRotation(Vector3(0.0f, 90.0f, 0.0f));

  g_sceneGraph().addObject(m_pModel);

  m_pModelTransform = g_graphicsMan().createConstantBuffer(sizeof(Transform));

  g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                       &m_pModel->transform.getTransform(),
                                       sizeof(Transform));

  // Set light buffer
  m_light.position = Vector3(5.0f, 5.0f, 5.0f);
  m_light.color = LinearColor(1.0f, 1.0f, 1.0f);
  m_light.intensity = 1.0f;

  m_pLightBuffer = g_graphicsMan().createConstantBuffer(sizeof(Light));

  g_graphicsMan().updateConstantBuffer(m_pLightBuffer, &m_light, sizeof(Light));

  // Set pass buffers
  m_pBasicShader->addVSConstantBuffer(m_pVP);
  m_pBasicShader->addVSConstantBuffer(m_pModelTransform);

  m_pDeferredShader->addPSConstantBuffer(m_pInvVP);
  m_pDeferredShader->addPSConstantBuffer(m_pCameraPosition);
  m_pDeferredShader->addPSConstantBuffer(m_pLightBuffer);
  m_pDeferredShader->addPSConstantBuffer(m_pViewportBuffer);

  m_mainTarget.push_back(g_graphicsMan().getMainRenderTargetView());
}

void
RendererApp::onUpdate()
{
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
  g_graphicsMan().setPrimitiveTopology();

  // First pass
  for (auto& target : m_targets) {
    g_graphicsMan().clearRenderTarget(target, LinearColor(0.0f, 0.0f, 0.0f));
  }

  g_graphicsMan().setRenderTargets(m_targets, g_graphicsMan().getMainDepthStencil());
  m_pBasicShader->setPass();

  auto& smucList = g_sceneGraph().getStaticMeshUnionComponentInScene();
  g_renderMan().drawStaticMeshUnionInScene(smucList);

  // Second pass
  g_graphicsMan().setRenderTargets(m_mainTarget, g_graphicsMan().getMainDepthStencil());
  m_pDeferredShader->setPass();

  g_graphicsMan().setShaderResourceView(m_targets[0], 0);
  g_graphicsMan().setShaderResourceView(m_targets[1], 1);
  g_graphicsMan().setShaderResourceView(m_targets[2], 2);
  
  g_graphicsMan().draw(3, 0);
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
    m_pDeferredShader->compileShader();
  }
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
RendererApp::onDestroy()
{
  
}

void
RendererApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));

  // Init pass shaders
  m_pBasicShader = make_unique<Pass>();

  m_pBasicShader->setShaderInfo("resources/BasicShader.hlsl",
                                "main",
                                "mainPS",
                                "vs_5_0",
                                "ps_5_0");
  m_pBasicShader->compileShader();

  m_pDeferredShader = make_unique<Pass>();

  m_pDeferredShader->setShaderInfo("resources/DeferredShader.hlsl",
                                   "main",
                                   "mainPS",
                                   "vs_5_0",
                                   "ps_5_0");
  m_pDeferredShader->compileShader();

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
  depthSDesc.depthEnable = false;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.stencilEnable = false;
  depthSDesc.stencilReadMask = 0;
  depthSDesc.stencilWriteMask = 0;
  DepthStencilOpDesc frontOPDesc = {};
  frontOPDesc.stencilFailOp = STENCIL_OP::kZero;
  frontOPDesc.stencilDepthFailOp = STENCIL_OP::kZero;
  frontOPDesc.stencilPassOp = STENCIL_OP::kZero;
  frontOPDesc.stencilFunc = COMPARISON_FUNC::kNever;
  DepthStencilOpDesc backOPDesc = {};
  backOPDesc.stencilFailOp = STENCIL_OP::kZero;
  backOPDesc.stencilDepthFailOp = STENCIL_OP::kZero;
  backOPDesc.stencilPassOp = STENCIL_OP::kZero;
  backOPDesc.stencilFunc = COMPARISON_FUNC::kNever;
  depthSDesc.frontFace = frontOPDesc;
  depthSDesc.backFace = backOPDesc;

  // Fill pass info
  m_pBasicShader->generateInputLayout();
  auto pSamplerLinear = g_graphicsMan().createSamplerState();
  m_pBasicShader->setSamplerState(pSamplerLinear);
  m_pBasicShader->setRasterizerState(rasterDesc);
  m_pBasicShader->setBlendState(blendDesc);
  //m_pBasicShader->setDepthStencilState(depthSDesc);

  m_pDeferredShader->generateInputLayout();
  m_pDeferredShader->setSamplerState(pSamplerLinear);
  m_pDeferredShader->setRasterizerState(rasterDesc);
  m_pDeferredShader->setBlendState(blendDesc);

  // Create and set render targets for deferred rendering
  auto depthTarget = g_graphicsMan().createTexture2D(m_desc.width,
                                     m_desc.height,
                                     TEXTURE_FORMAT::kR32G32B32A32_float,
                                     USAGE::kDefault,
                                     BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  auto normalTarget = g_graphicsMan().createTexture2D(m_desc.width,
                                      m_desc.height,
                                      TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                      USAGE::kDefault,
                                      BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  auto colorTarget = g_graphicsMan().createTexture2D(m_desc.width,
                                     m_desc.height,
                                     TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                     USAGE::kDefault,
                                     BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource);

  m_targets.push_back(depthTarget);
  m_targets.push_back(normalTarget);
  m_targets.push_back(colorTarget);
}

void
RendererApp::initCamera()
{
  // Init camera and its constant buffer
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp;

  m_camera = Camera(Vector3(0.0f, 0.0f, -2.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    45.0f * Math::DEG2RAD,
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
