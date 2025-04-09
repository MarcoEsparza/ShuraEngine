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
  GraphicsManager& graphMan = g_graphicsMan();

  // Graphic init
  initGraphicAssets();
  m_targets.push_back(graphMan.getMainRenderTargetView());
  setBackgroundColor(LinearColor(0.5f, 1.0f, 0.5f));
  initCamera();

  // Imgui init
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(getScreen());
  ImGui::StyleColorsDark();

  // Objects for simulation init
  Vector2 minSpriteSize(-25.0f, 25.0f);
  Vector2 maxSpriteSize(25.0f, -25.0f);
  Vector2 minBoneSpriteSize(-15.0f, 15.0f);
  Vector2 maxBoneSpriteSize(15.0f, -15.0f);

  m_pSpriteBase = make_shared<Sprite>(Path("resources/Tower.png"),
                                      minSpriteSize,
                                      maxSpriteSize);
  

  m_pSpriteBall = make_shared<Sprite>(Path("resources/egball.png"),
                                      minSpriteSize,
                                      maxSpriteSize);

  m_pSbSprite = make_shared<Sprite>(Path("resources/ttgl.png"),
                                    minSpriteSize,
                                    maxSpriteSize);

  m_pSpriteBone = make_shared<Sprite>(Path("resources/cannonball.png"),
                                      minBoneSpriteSize,
                                      maxBoneSpriteSize);

  m_integration = INTEGRATION::kVerlet;
  m_intIndex = 1;

  initPivot();
  initSpringBall();
  initKinematicArm();
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

  manageImgui();

  if (m_intIndex != static_cast<int32>(m_integration)) {
    m_integration = static_cast<INTEGRATION::E>(m_intIndex);

    if (m_integration == INTEGRATION::kVerlet) {
      setBackgroundColor(LinearColor(0.5f, 1.0f, 0.5f));
    }
    else if (m_integration == INTEGRATION::kEuler) {
      setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));
    }

    //m_activeBalls.clear();
    initSpringBall();
  }

  if (m_ikIndex != static_cast<int32>(m_ikAlgorithm)) {
    m_ikAlgorithm = static_cast<IK_ALGORITHM::E>(m_ikIndex);
  }
  if (m_mtIndex != static_cast<int32>(m_moveType)) {
    m_moveType = static_cast<MOVEMENT_TYPE::E>(m_mtIndex);
  }

  m_springBall->m_springC = m_springC;
  m_springBall->m_drag = m_dragC;
  m_springBall->m_mass = m_mass;
  m_springBall->m_gravity = m_gravity;
  m_springBall->m_iniLenght = m_iniLenght;
  m_springBall->m_maxLenght = m_maxLenght;
  m_springBall->m_minLenght = m_minLenght;

  if (m_bLeftClick) {
    if (containsMouse(m_pivotPos, 50.0f)) {
      if (m_moveType == MOVEMENT_TYPE::kFoward) {
        Vector2 tempPos = m_pivotPos;
        dragPivot();
        Vector2 newPos = tempPos - m_pivotPos;
        m_ikBase->foward(Vector2(-newPos.x, -newPos.y));
        auto& pLastBall = m_ikBase->m_child->m_child;
        m_lastBallPos = pLastBall->m_position;
      }
    }
    else if (containsMouse(m_springBall->m_position, 50.0f)) {
      m_springBall->m_bGrabbed = true;
      dragSpringBall();
    }

    if (m_moveType == MOVEMENT_TYPE::kInverse) {
      auto& pChild = m_ikBase->m_child;
      auto& pChild1 = pChild->m_child;
      if (containsMouse(m_ikBase->m_position, 20.0f)) {
        m_selectedIndex = 0;
      }
      else if (containsMouse(pChild->m_position, 20.0f)) {
        m_selectedIndex = 1;
      }
      else if (containsMouse(pChild1->m_position, 20.0f)) {
        m_selectedIndex = 2;
      }

      Vector<Vector2> points;
      points.push_back(m_ikBase->m_position);
      points.push_back(pChild->m_position);
      points.push_back(pChild1->m_position);

      Vector<float> lenghts;
      lenghts.push_back(m_ikBase->m_lenght);
      lenghts.push_back(pChild->m_lenght);
      lenghts.push_back(pChild1->m_lenght);

      if (m_selectedIndex != -1) {
        points[m_selectedIndex].x = m_mousePosition.x - (m_desc.width * 0.5f);
        points[m_selectedIndex].y = -m_mousePosition.y + (m_desc.height * 0.5f);

        if (m_selectedIndex < points.size() - 1) {
          Vector<Vector2> subPoints(points.begin() + m_selectedIndex, points.end());
          Vector<float> subLenghts(lenghts.begin() + m_selectedIndex, lenghts.end());

          fabrik(subPoints, subLenghts, points.back());

          for (uint32 i = 1; i < subPoints.size(); ++i) {
            points[m_selectedIndex + i] = subPoints[i];
          }
        }
      }

      if (m_selectedIndex > 0) {
        Vector<Vector2> subPoints(points.begin(), points.begin() + m_selectedIndex + 1);
        Vector<float> subLenghts(lenghts.begin(), lenghts.begin() + m_selectedIndex);

        std::reverse(subPoints.begin(), subPoints.end());
        std::reverse(subLenghts.begin(), subLenghts.end());

        fabrik(subPoints, subLenghts, points[0]);

        std::reverse(subPoints.begin(), subPoints.end());
        for (uint32 i = 0; i < m_selectedIndex; ++i) {
          points[i] = subPoints[i];
        }
      }

      m_ikBase->m_position = points[0];
      pChild->m_position = points[1];
      pChild1->m_position = points[2];
      m_lastBallPos = pChild1->m_position;

      m_ikBase->update();
      pChild->update();
      pChild1->update();

      m_selectedIndex = -1;
      m_springBall->m_bGrabbed = false;
    }
  }

  if (m_integration == INTEGRATION::kEuler) {
    /*for (auto& ball : m_activeBalls) {
      if (ball) {
        checkBallCollision(ball);

        ball->update(m_integration);
        if (ball->m_bDestroy) {
          m_activeBalls.erase(m_activeBalls.begin());
        }
      }
    }*/
    m_springBall->update(m_lastBallPos, m_integration);
  }
}

void
PhysicsApp::onFixedUpdate()
{
  if (m_integration == INTEGRATION::kVerlet) {
    /*for (auto& ball : m_activeBalls) {
      if (ball) {
        checkBallCollision(ball);
    
        ball->update(m_integration);
        if (ball->m_bDestroy) {
          m_activeBalls.erase(m_activeBalls.begin());
        }
      }
    }*/
    m_springBall->update(m_lastBallPos, m_integration);
  }
}

void
PhysicsApp::onRender()
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.setRenderTargets(m_targets, g_graphicsMan().getMainDepthStencil());
  
  m_pPhysicsShader->setPass();
  graphMan.vsSetConstantBuffers(m_pVP);
  graphMan.setPrimitiveTopology();

  /*graphMan.vsSetConstantBuffers(m_pTurret, 1);
  graphMan.setVertexBuffers(m_pSpriteCannon->m_pVB);
  graphMan.setIndexBuffers(m_pSpriteCannon->m_pIB);
  graphMan.setShaderResourceView(m_pSpriteCannon->m_pTexture);
  graphMan.drawIndexed(static_cast<uint32>(m_pSpriteCannon->m_indices.size()), 0, 0);*/

  graphMan.vsSetConstantBuffers(m_pBase, 1);
  graphMan.setVertexBuffers(m_pSpriteBase->m_pVB);
  graphMan.setIndexBuffers(m_pSpriteBase->m_pIB);
  graphMan.setShaderResourceView(m_pSpriteBase->m_pTexture);
  graphMan.drawIndexed(static_cast<uint32>(m_pSpriteBase->m_indices.size()), 0, 0);

  graphMan.vsSetConstantBuffers(m_springBall->m_ballBuffer, 1);
  graphMan.setVertexBuffers(m_springBall->m_sprite->m_pVB);
  graphMan.setIndexBuffers(m_springBall->m_sprite->m_pIB);
  graphMan.setShaderResourceView(m_springBall->m_sprite->m_pTexture);
  graphMan.drawIndexed(static_cast<uint32>(m_springBall->m_sprite->m_indices.size()), 0, 0);

  /*for (auto& ball : m_activeBalls) {
    graphMan.vsSetConstantBuffers(ball->m_buffer, 1);
    graphMan.setVertexBuffers(ball->m_sprite->m_pVB);
    graphMan.setIndexBuffers(ball->m_sprite->m_pIB);
    graphMan.setShaderResourceView(ball->m_sprite->m_pTexture);
    graphMan.drawIndexed(static_cast<uint32>(ball->m_sprite->m_indices.size()), 0, 0);
  }*/

  m_ikBase->drawSprite();

  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
}

void
PhysicsApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  
  if (key == KEY::kLeft) {
    //m_bRotLeft = true;
  }

  if (key == KEY::kRight) {
    //m_bRotRight = true;
  }
}

void
PhysicsApp::onKeyReleased(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kLeft) {
    //m_bRotLeft = false;
  }

  if (key == KEY::kRight) {
    //m_bRotRight = false;
  }

  if (key == KEY::kSpace) {
    //m_bShot = true;
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
PhysicsApp::manageImgui()
{
  ImGui::SetNextWindowSize(ImVec2(350.0f, 500.0f));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(242, 128, 5, 0xff));
  ImGui::Begin("Physics", 0);
  ImGui::PopStyleColor();

  if (ImGui::CollapsingHeader("Transformations:")) {
    if (ImGui::CollapsingHeader("Pivot:")) {

    }
  }

  if (ImGui::CollapsingHeader("Simulation:")) {
    const char* integrationItems = "Euler\0Verlet";
    const char* integrationText = "Integration";

    ImGui::Combo(integrationText,
                 &m_intIndex,
                 integrationItems,
                 2);

    const char* ikItems = "Fabrik\0CCD";
    const char* ikText = "IK Algorithm";

    ImGui::Combo(ikText,
                 &m_ikIndex,
                 ikItems,
                 2);

    const char* moveItems = "Foward\0Inverse";
    const char* moveText = "Movement type";

    ImGui::Combo(moveText,
                 &m_mtIndex,
                 moveItems,
                 2);

    if (ImGui::Button("Reset")) {
      initPivot();
      initSpringBall();
      initKinematicArm();
    }
  }

  if (ImGui::CollapsingHeader("Hookes constants:")) {
    ImGui::DragFloat("Spring Constant", &m_springC, 0.01f);
    ImGui::DragFloat("Drag Constant", &m_dragC, 0.01f);
    ImGui::DragFloat("Mass", &m_mass, 0.1f);
    ImGui::DragFloat("Gravity", &m_gravity, 0.1f);
    ImGui::DragFloat("Ini Lenght", &m_iniLenght, 1.0f);
    ImGui::DragFloat("Max Lenght", &m_maxLenght, 1.0f);
    ImGui::DragFloat("Min Lenght", &m_minLenght, 1.0f);
  }

  ImGui::End();
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

void
PhysicsApp::initPivot()
{
  GraphicsManager& graphMan = g_graphicsMan();

  if (!m_pBase) {
    m_pBase = graphMan.createConstantBuffer(sizeof(Matrix4));
  }

  float spawnPointY = 100.0f;
  m_baseTransform = TranslationMatrix(Vector3(0.0f, spawnPointY, 10.0f));
  graphMan.updateConstantBuffer(m_pBase,
                                &m_baseTransform,
                                sizeof(Matrix4));

  m_pivotPos = { 0.0f, spawnPointY };
}

void
PhysicsApp::initSpringBall()
{
  if (m_springBall != nullptr) {
    m_springBall.reset();
  }

  if(m_integration == INTEGRATION::kVerlet) {
    m_springC = 0.11f;
    m_dragC = 0.840f;
    m_mass = 2.5f;
    m_gravity = -2.7f;
    m_iniLenght = 80.0f;
    m_maxLenght = 180.0f;
    m_minLenght = 50.0f;

    /**
    * Spring C = 0.110
    * Drag C = 0.840
    * Mass = 2.5
    * Gravity = -2.7
    * Ini lenght = 80
    * Max = 180
    * Min = 50
    */

    m_springBall = make_shared<SpringBall>(m_pSbSprite,           // Sprite
                                           m_lastBallPos + Vector2(0.0f, -100.0f),// Position
                                           Vector2(0.0f, 0.0f),   // Velocity
                                           Vector2(0.0f, 0.0f),   // Accel
                                           25.0f,                 // Radius
                                           m_springC,                  // SpringConstant
                                           m_gravity,                // Gravity
                                           m_iniLenght,                // Ini
                                           m_maxLenght,                // Max
                                           m_minLenght,                // Min
                                           m_dragC);                // Drag
  }
  else if(m_integration == INTEGRATION::kEuler) {
    m_springC = 0.2f;
    m_dragC = 0.60f;
    m_mass = 1.0f;
    m_gravity = -9.8f;
    m_iniLenght = 80.0f;
    m_maxLenght = 180.0f;
    m_minLenght = 50.0f;

    m_springBall = make_shared<SpringBall>(m_pSbSprite,           // Sprite
                                           Vector2(0.0f, 0.0f),   // Position
                                           Vector2(0.0f, 0.0f),   // Velocity
                                           Vector2(0.0f, 0.0f),   // Accel
                                           25.0f,                 // Radius
                                           m_springC,                  // SpringConstant
                                           m_gravity,                // Gravity
                                           m_iniLenght,                // Ini
                                           m_maxLenght,                // Max
                                           m_minLenght,                // Min
                                           m_dragC);                // Drag
  }
}

void
PhysicsApp::initKinematicArm()
{
  if (m_ikBase) {
    m_ikBase.reset();
  }

  float radius = 15.0f;
  //float lenght = 75.0f;
  Vector2 offset(75.0f, 75.0f);
  offset.normalize();
  Vector2 ball = m_pivotPos + (offset * 100.0f);
  Vector2 ball1 = ball + (offset * 100.0f);
  m_lastBallPos = ball1 + (offset * 100.0f);

  m_ikBase = make_shared<KinematicBall>(m_pSpriteBone, ball, radius);
  m_ikBase->setChild(KinematicBall(m_pSpriteBone, ball1, radius));

  auto& pChild1 = m_ikBase->m_child;
  pChild1->setChild(KinematicBall(m_pSpriteBone, m_lastBallPos, radius));
}

bool
PhysicsApp::containsMouse(const Vector2& point, const float radius)
{
  Vector2 mouse = m_mousePosition;
  mouse.x = mouse.x - (m_desc.width * 0.5f);
  mouse.y = -mouse.y + (m_desc.height * 0.5f);
  return ((mouse - point).mag() < radius);
}

void
PhysicsApp::dragSpringBall()
{
  float screenOffset = -400.0f;
  m_springBall->m_position.x = screenOffset + m_mousePosition.x;
  m_springBall->m_position.y = -screenOffset - m_mousePosition.y;
  m_springBall->updateCBuffer();
}

void
PhysicsApp::dragPivot()
{
  float screenOffset = -400.0f;
  m_pivotPos.x = screenOffset + m_mousePosition.x;
  m_pivotPos.y = -screenOffset - m_mousePosition.y;
  m_baseTransform = TranslationMatrix(Vector3(m_pivotPos.x,
                                              m_pivotPos.y,
                                              0.0f));
  g_graphicsMan().updateConstantBuffer(m_pBase, &m_baseTransform, sizeof(Matrix4));
}

void
PhysicsApp::fabrik(Vector<Vector2>& points,
                   const Vector<float>& lenghts,
                   const Vector2& target,
                   const float tolerance)
{
  uint32 numBones = static_cast<uint32>(points.size());
  Vector2 rootPos = points[0];

  float totalLenght = 0.0f;
  for (float lenght : lenghts) {
    totalLenght += lenght;
  }

  if ((target - rootPos).mag() > totalLenght) {
    // Target is out of reach, so it moves on a straight line
    for (uint32 i = 1; i < numBones; ++i) {
      Vector2 dir = (target - points[i]).getNormalized();
      points[i] = points[i - 1] + dir * lenghts[i - 1];
    }
    return;
  }

  for (uint32 iter = 0; iter < 10; ++iter) {
    points[numBones - 1] = target;

    // Foward reaching
    for (int32 i = numBones - 2; i >= 0; --i) {
      Vector2 dir = (points[i] - points[i + 1]).getNormalized();
      points[i] = points[i + 1] + dir * lenghts[i];
    }

    // Backward reaching
    points[0] = rootPos;
    for (uint32 i = 1; i < numBones; ++i) {
      Vector2 dir = (points[i] - points[i - 1]).getNormalized();
      points[i] = points[i - 1] + dir * lenghts[i - 1];
    }

    if ((points[numBones - 1] - target).mag() < tolerance) {
      break;
    }
  }
}
}
