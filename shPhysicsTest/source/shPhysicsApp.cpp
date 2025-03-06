/*****************************************************************************/
/*
*  @file    shPhysicsApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   App for physics simulation.
*
*  App for physics simulation.s
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPhysicsApp.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shTime.h"
#include "imgui_impl_shura.h"

#include "shShader.h"
#include "shInputLayout.h"
#include "shBlendState.h"
#include "shRasterizerState.h"
#include "shBuffers.h"
#include "shSamplerState.h"
#include "shImageResource.h"
#include "shPath.h"
#include "shMath.h"
#include "shRadian.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
PhysicsApp::onCreate()
{
  initGraphicAssets();
  initCamera();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());

  ImGui::StyleColorsDark();

  m_rotSpeed = 25.0f;

  Vector2 minPlayerSize(-25.0f, 25.0f);
  Vector2 maxPlayerSize(25.0f, -25.0f);
  float spawnPointY = -350.0f;

  Path basePath("resources/Tower.png");
  Path turretPath("resources/Cannon3.png");
  m_pSpriteBase = make_shared<Sprite>(basePath, minPlayerSize, maxPlayerSize);
  m_pSpriteCannon = make_shared<Sprite>(turretPath, minPlayerSize, maxPlayerSize);
  m_pBase = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));
  m_pTurret = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));

  Path ballPath("resources/egball.png");
  m_pSpriteBall = make_shared<Sprite>(ballPath, minPlayerSize, maxPlayerSize);

  m_baseTransform = TranslationMatrix(Vector3(0.0f, spawnPointY, 10.0f));
  m_turretTransform = TranslationMatrix(Vector3(0.0f, spawnPointY, 0.0f));

  g_graphicsMan().updateConstantBuffer(m_pBase,
                                       &m_baseTransform,
                                       sizeof(Matrix4));
  g_graphicsMan().updateConstantBuffer(m_pTurret,
                                       &m_turretTransform,
                                       sizeof(Matrix4));

  m_targets.push_back(g_graphicsMan().getMainRenderTargetView());

  m_intList.push_back("Euler");
  m_intList.push_back("Verlet");
}

void
PhysicsApp::onUpdate()
{
  ImGui_ImplShura_NewFrame(m_mousePosition,
                           m_bLeftClick,
                           m_delta,
                           m_hdelta,
                           false,
                           KEY::kKeysMax);
  ImGui::NewFrame();

  //ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(300.0f, 150.0f));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Physics", 0);
  ImGui::PopStyleColor();

  const char* items = "Euler\0Verlet";
  const char* text = "Integration";

  ImGui::Combo(text,
               &m_intIndex,
               items,
               static_cast<int32>(m_intList.size()));

  ImGui::Text("Press 'space' to shoot");
  ImGui::Spacing();
  ImGui::Text("Press '<- ' button to rotate left");
  ImGui::Spacing();
  ImGui::Text("Press '-> ' button to rotate right");

  ImGui::End();

  if (m_intIndex != static_cast<int32>(m_integration)) {
    m_integration = static_cast<INTEGRATION::E>(m_intIndex);

    if (m_integration == INTEGRATION::kVerlet) {
      setBackgroundColor(LinearColor(0.5f, 1.0f, 0.5f));
    }
    else if (m_integration == INTEGRATION::kEuler) {
      setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));
    }

    m_activeBalls.clear();
  }

  Vector2 direction(0.0f, 0.0f);

  const float rotAngle = m_rotSpeed * g_time().getFrameDeltaTime() * Math::DEG2RAD;

  if (m_bRotLeft) {
    if (m_rotAccumulator > -(90.0f * Math::DEG2RAD)) {
      m_turretTransform *= MatrixRotationAxis(Vector3::FORWARD, -rotAngle);
      g_graphicsMan().updateConstantBuffer(m_pTurret,
                                           &m_turretTransform,
                                           sizeof(Matrix4));
      m_rotAccumulator -= rotAngle;
    }
  }

  if (m_bRotRight) {
    if (m_rotAccumulator < (90.0f * Math::DEG2RAD)) {
      m_turretTransform *= MatrixRotationAxis(Vector3::FORWARD, rotAngle);
      g_graphicsMan().updateConstantBuffer(m_pTurret,
                                           &m_turretTransform,
                                           sizeof(Matrix4));
      m_rotAccumulator += rotAngle;
    }
  }

  if (m_bShot) {
    m_bShot = false;
    spawnBall();
  }

  if (m_integration == INTEGRATION::kEuler) {
    for (auto& ball : m_activeBalls) {
      if (ball) {
        checkBallCollision(ball);

        ball->update(m_integration);
        if (ball->m_bDestroy) {
          m_activeBalls.erase(m_activeBalls.begin());
        }
      }
    }
  }
}

void
PhysicsApp::onFixedUpdate()
{
  if (m_integration == INTEGRATION::kVerlet) {
    for (auto& ball : m_activeBalls) {
      if (ball) {
        checkBallCollision(ball);
    
        ball->update(m_integration);
        if (ball->m_bDestroy) {
          m_activeBalls.erase(m_activeBalls.begin());
        }
      }
    }
  }
}

void
PhysicsApp::onRender()
{
  g_graphicsMan().setRenderTargets(m_targets, g_graphicsMan().getMainDepthStencil());
  
  m_pPhysicsShader->setPass();
  g_graphicsMan().vsSetConstantBuffers(m_pVP);
  g_graphicsMan().setPrimitiveTopology();

  g_graphicsMan().vsSetConstantBuffers(m_pTurret, 1);
  g_graphicsMan().setVertexBuffers(m_pSpriteCannon->m_pVB);
  g_graphicsMan().setIndexBuffers(m_pSpriteCannon->m_pIB);
  g_graphicsMan().setShaderResourceView(m_pSpriteCannon->m_pTexture);
  g_graphicsMan().drawIndexed(static_cast<uint32>(m_pSpriteCannon->m_indices.size()), 0, 0);

  g_graphicsMan().vsSetConstantBuffers(m_pBase, 1);
  g_graphicsMan().setVertexBuffers(m_pSpriteBase->m_pVB);
  g_graphicsMan().setIndexBuffers(m_pSpriteBase->m_pIB);
  g_graphicsMan().setShaderResourceView(m_pSpriteBase->m_pTexture);
  g_graphicsMan().drawIndexed(static_cast<uint32>(m_pSpriteBase->m_indices.size()), 0, 0);

  for (auto& ball : m_activeBalls) {
    g_graphicsMan().vsSetConstantBuffers(ball->m_buffer, 1);
    g_graphicsMan().setVertexBuffers(ball->m_sprite->m_pVB);
    g_graphicsMan().setIndexBuffers(ball->m_sprite->m_pIB);
    g_graphicsMan().setShaderResourceView(ball->m_sprite->m_pTexture);
    g_graphicsMan().drawIndexed(static_cast<uint32>(ball->m_sprite->m_indices.size()), 0, 0);
  }

  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
}

void
PhysicsApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  
  if (key == KEY::kLeft) {
    m_bRotLeft = true;
  }

  if (key == KEY::kRight) {
    m_bRotRight = true;
  }
}

void
PhysicsApp::onKeyReleased(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kLeft) {
    m_bRotLeft = false;
  }

  if (key == KEY::kRight) {
    m_bRotRight = false;
  }

  if (key == KEY::kSpace) {
    m_bShot = true;
  }
}

void
PhysicsApp::onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                                 const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_bLeftClick = true;
  }
}

void
PhysicsApp::onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                                  const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_bLeftClick = false;
  }
}

void
PhysicsApp::onMouseMove(const MouseMoveData& mouse)
{
  m_mousePosition.x = static_cast<float>(mouse.x);
  m_mousePosition.y = static_cast<float>(mouse.y);
}

void
PhysicsApp::onMouseWheel(const double delta, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  m_delta = static_cast<float>(delta);
}

void
PhysicsApp::onMouseHWheel(const double delta, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  m_hdelta = static_cast<float>(delta);
}

void
PhysicsApp::onDestroy()
{
  ImGui_ImplShura_Shutdown();
  ImGui::DestroyContext();
}

void
PhysicsApp::checkBallCollision(const SPtr<Ball>& ball)
{
  const float ballTopPos = ball->m_position.y + ball->m_radius;
  const float ballBottomPos = ball->m_position.y - ball->m_radius;
  const float ballLeftPos = ball->m_position.x - ball->m_radius;
  const float ballRightPos = ball->m_position.x + ball->m_radius;

  const float limit = m_desc.width * 0.5f;

  if (ballTopPos >= limit) {
    ball->m_previousPosition.y = ball->m_position.y + ball->m_velocity.y;
    ball->m_accel.y = -ball->m_accel.y;
    ball->m_velocity.y = -ball->m_velocity.y;
  }

  if (ballBottomPos <= -limit) {
    ball->m_previousPosition.y = ball->m_position.y + ball->m_velocity.y;
    ball->m_accel.y = -ball->m_accel.y;
    ball->m_velocity.y = -ball->m_velocity.y;
  }

  if (ballLeftPos <= -limit) {
    ball->m_previousPosition.x = ball->m_position.x + ball->m_velocity.x;
    ball->m_accel.x = -ball->m_accel.x;
    ball->m_velocity.x = -ball->m_velocity.x;
  }

  if (ballRightPos >= limit) {
    ball->m_previousPosition.x = ball->m_position.x + ball->m_velocity.x;
    ball->m_accel.x = -ball->m_accel.x;
    ball->m_velocity.x = -ball->m_velocity.x;
  }
}

void
PhysicsApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));

  m_pPhysicsShader = make_unique<Pass>();
  m_pPhysicsShader->setShaderInfo("resources/shaders/PhysicsShader.hlsl",
                                  "main",
                                  "mainPS",
                                  "vs_5_0",
                                  "ps_5_0");
  m_pPhysicsShader->compileShader();

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

  auto pSamplerLinear = g_graphicsMan().createSamplerState();
  m_pPhysicsShader->setSamplerState(pSamplerLinear);
  m_pPhysicsShader->generateInputLayout();
  m_pPhysicsShader->setRasterizerState(rasterDesc);
  m_pPhysicsShader->setBlendState(blendDesc);
  m_pPhysicsShader->setDepthStencilState(depthSDesc);
}

void
PhysicsApp::initCamera()
{
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp;

  m_camera = Camera(Vector3(0.0f, 0.0f, -10.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3(0.0f, 1.0f, 0.0f),
                    static_cast<float>(m_desc.width),
                    static_cast<float>(m_desc.height),
                    0.1f,
                    100.0f);

  m_camera.update();

  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));
}

void
PhysicsApp::spawnBall()
{
  auto newBall = make_shared<Ball>();
  newBall->setSprite(m_pSpriteBall);
  newBall->m_transform = m_turretTransform;

  // This values are exposed here to test
  newBall->m_eulerSpeed = 5000.0f;
  newBall->m_verletSpeed = 50.0f;

  newBall->m_eulerGravity = -2.5f;
  newBall->m_verletGravity = -1.5f;

  newBall->m_mass = 1.0f;
  newBall->m_dragC = 0.6f;
  newBall->m_radius = 25.0f;

  newBall->m_position = Vector2(0.0f, -370.0f);
  newBall->m_lifeSpan = 8.0f;

  Vector2 direction = Vector2(0.0f, 1.0f).getNormalized();
  float cosA = Math::cos(Radian(-m_rotAccumulator));
  float sinA = Math::sin(Radian(-m_rotAccumulator));
  Vector2 newDirection(0.0f, 0.0f);
  newDirection.x = direction.x * cosA - direction.y * sinA;
  newDirection.y = direction.x * sinA + direction.y * cosA;

  if (m_integration == INTEGRATION::kEuler) {
    newBall->m_velocity = newDirection * newBall->m_eulerSpeed;
  }
  else if (m_integration == INTEGRATION::kVerlet) {
    newBall->m_velocity = newDirection * newBall->m_verletSpeed;
    newBall->m_accel = newDirection * newBall->m_verletSpeed;
  }

  newBall->m_previousPosition = newBall->m_position - newBall->m_velocity;

  m_activeBalls.push_back(newBall);
}
}