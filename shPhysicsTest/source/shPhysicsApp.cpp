/*************************************************************/
/*
*  @file    shPhysicsApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
*  @brief   
*
*  
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPhysicsApp.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shTime.h"

#include "shShader.h"
#include "shInputLayout.h"
#include "shBlendState.h"
#include "shRasterizerState.h"
#include "shBuffers.h"
#include "shSamplerState.h"
#include "shImageResource.h"
#include "shPath.h"
#include "shMath.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
PhysicsApp::onCreate()
{
  initGraphicAssets();
  initCamera();
  initCollisionBoxes();

  Vector2 minPlayerSize(-25.0f, 25.0f);
  Vector2 maxPlayerSize(25.0f, -25.0f);
  /*Path textPath("resources/ttgl.png");

  m_player = make_shared<Player>(minPlayerSize,
                                 maxPlayerSize,
                                 textPath,
                                 Vector2(0.0f, 0.0f),
                                 1.5f,
                                 500.0f,
                                 0.8f,
                                 25.0f);

  Path arrowPath("resources/arrow.png");
  auto arrow = make_shared<Arrow>(arrowPath, Vector2(0.0f, 40.0f), Vector2(80.0f, -40.0f));

  m_player->setArrow(arrow);*/

  Path basePath("resources/Tower.png");
  Path turretPath("resources/Cannon3.png");
  m_pSpriteBase = make_unique<Sprite>(basePath, minPlayerSize, minPlayerSize);
  m_pSpriteCannon = make_unique<Sprite>(turretPath, minPlayerSize, minPlayerSize);
  m_pBase = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));
  m_pTurret = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));
  g_graphicsMan().updateConstantBuffer(m_pBase,
                                       &m_pSpriteBase->m_transform,
                                       sizeof(Matrix4));
  g_graphicsMan().updateConstantBuffer(m_pTurret,
                                       &m_pSpriteCannon->m_transform,
                                       sizeof(Matrix4));
}

void
PhysicsApp::onUpdate()
{
  Vector2 direction(0.0f, 0.0f);

  if (m_bUp) {
    direction.y = 1.0f;
  }

  if (m_bLeft) {
    direction.x = -1.0f;
  }

  if (m_bDown) {
    direction.y = -1.0f;
  }

  if (m_bRight) {
    direction.x = 1.0f;
  }

  /*m_player->move(direction);

  m_player->update(g_time().getFrameDeltaTime());

  Vector2 boxNormal(0.0f, 0.0f);
  if (checkCollision(m_left, boxNormal)) {
    playerBounce(boxNormal);
  }
  else if (checkCollision(m_right, boxNormal)) {
    playerBounce(boxNormal);
  }
  else if (checkCollision(m_top, boxNormal)) {
    playerBounce(boxNormal);
  }
  else if (checkCollision(m_bottom, boxNormal)) {
    playerBounce(boxNormal);
  }*/
}

void
PhysicsApp::onFixedUpdate()
{
  //m_player->update(g_time().FIXED_DELTA_TIME);
}

void
PhysicsApp::onRender()
{
  g_graphicsMan().setRenderTargets(g_graphicsMan().getMainRenderTargetView(),
                                   g_graphicsMan().getMainDepthStencil(),
                                   1);
  
  m_pPhysicsShader->setPass();
  g_graphicsMan().vsSetConstantBuffers(m_pVP);
  g_graphicsMan().setPrimitiveTopology();

  // Player
  //g_graphicsMan().vsSetConstantBuffers(m_player->m_modelBuffer, 1);
  //g_graphicsMan().setVertexBuffers(m_player->m_sprite->m_pVB);
  //g_graphicsMan().setIndexBuffers(m_player->m_sprite->m_pIB);
  //g_graphicsMan().setShaderResourceView(m_player->m_sprite->m_pTexture);
  //g_graphicsMan().drawIndexed(static_cast<uint32>(m_player->m_sprite->m_indices.size()), 0, 0);
  
  //g_graphicsMan().vsSetConstantBuffers(m_player->m_pDirArrow->m_modelBuffer, 1);
  //g_graphicsMan().setVertexBuffers(m_player->m_pDirArrow->m_sprite->m_pVB);
  //g_graphicsMan().setIndexBuffers(m_player->m_pDirArrow->m_sprite->m_pIB);
  //g_graphicsMan().setShaderResourceView(m_player->m_pDirArrow->m_sprite->m_pTexture);
  //g_graphicsMan().drawIndexed(static_cast<uint32>(m_player->m_pDirArrow->m_sprite->m_indices.size()), 0, 0);

  g_graphicsMan().vsSetConstantBuffers(m_pBase, 1);
  g_graphicsMan().setVertexBuffers(m_pSpriteBase->m_pVB);
  g_graphicsMan().setIndexBuffers(m_pSpriteBase->m_pIB);
  g_graphicsMan().setShaderResourceView(m_pSpriteBase->m_pTexture);
  g_graphicsMan().drawIndexed(static_cast<uint32>(m_pSpriteBase->m_indices.size()), 0, 0);

  //g_graphicsMan().vsSetConstantBuffers(m_pTurret, 1);
  //g_graphicsMan().setVertexBuffers(m_pSpriteCannon->m_pVB);
  //g_graphicsMan().setIndexBuffers(m_pSpriteCannon->m_pIB);
  //g_graphicsMan().setShaderResourceView(m_pSpriteCannon->m_pTexture);
  //g_graphicsMan().drawIndexed(static_cast<uint32>(m_pSpriteCannon->m_indices.size()), 0, 0);
}

void
PhysicsApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);
  
  if (key == KEY::kW) {
    m_bUp = true;
  }

  if (key == KEY::kA) {
    m_bLeft = true;
  }

  if (key == KEY::kS) {
    m_bDown = true;
  }

  if (key == KEY::kD) {
    m_bRight = true;
  }
}

void
PhysicsApp::onKeyReleased(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kW) {
    m_bUp = false;
  }

  if (key == KEY::kA) {
    m_bLeft = false;
  }

  if (key == KEY::kS) {
    m_bDown = false;
  }

  if (key == KEY::kD) {
    m_bRight = false;
  }
}

bool
PhysicsApp::checkCollision(const Box& box, Vector2& collisionNormal)
{
  float closestX = Math::max(box.min.x, Math::min(m_player->m_position.x, box.max.x));
  float closestY = Math::max(box.min.y, Math::min(m_player->m_position.y, box.max.y));

  Vector2 closestPoint = { closestX, closestY };
  Vector2 difference = m_player->m_position - closestPoint;

  if (difference.mag() < m_player->m_radius) {
    collisionNormal = difference.getNormalized();
    return true;
  }
  return false;
}

void
PhysicsApp::playerBounce(Vector2& collisionNormal)
{
  float dot = m_player->m_velocity.dot(collisionNormal);
  m_player->m_velocity = m_player->m_velocity - collisionNormal * (2.0f * dot);
}

void
PhysicsApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));

  m_pPhysicsShader = make_unique<Pass>();
  m_pPhysicsShader->setShaderInfo("resources/PhysicsShader.hlsl",
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

  auto pSamplerLinear = g_graphicsMan().createSamplerState();
  m_pPhysicsShader->setSamplerState(pSamplerLinear);
  m_pPhysicsShader->generateInputLayout();
  m_pPhysicsShader->setRasterizerState(rasterDesc);
  m_pPhysicsShader->setBlendState(blendDesc);
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
PhysicsApp::initCollisionBoxes()
{
  float size = m_desc.width * 0.5f;

  m_left.min = Vector2(-size, -size);
  m_left.max = Vector2(-size, size);
  m_right.min = Vector2(size, -size);
  m_right.max = Vector2(size, size);
  m_top.min = Vector2(-size, -size);
  m_top.max = Vector2(size, -size);
  m_bottom.min = Vector2(-size, size);
  m_bottom.max = Vector2(size, size);
}
}