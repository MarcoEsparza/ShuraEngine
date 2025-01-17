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

  Path textPath("resources/ttgl.png");
  Vector2 minPlayerSize(25.0f, 25.0f);
  Vector2 maxPlayerSize(25.0f, 25.0f);

  m_player = make_shared<Player>(minPlayerSize,
                                 maxPlayerSize,
                                 textPath,
                                 Vector2(0.0f, 0.0f),
                                 2.5f,
                                 25.0f);

  Path arrowPath("resources/arrow.png");
  /*auto arrowR = reinterpret_pointer_cast<ImageResource>(
                rManager.loadResourceFromFile(arrowPath));*/

  
}

void
PhysicsApp::onUpdate(float deltaTime)
{
  m_player->update(deltaTime);

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
  }
}

void
PhysicsApp::onRender()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setRenderTargets(gManager.getMainRenderTargetView(),
                            gManager.getMainDepthStencil(),
                            1);
  gManager.setRasterizerState(m_pRasterS);
  gManager.setBlendState(m_pBlendS);

  gManager.setProgramShader(m_pShader);
  gManager.setSamplerState(m_pSamplerLinear);
  gManager.vsSetConstantBuffers(m_pVP);
  gManager.setPrimitiveTopology();
  gManager.setInputLayout(m_pIL);

  // Player
  gManager.vsSetConstantBuffers(m_player->m_modelBuffer, 1);
  gManager.setVertexBuffers(m_player->m_sprite->m_pVB);
  gManager.setIndexBuffers(m_player->m_sprite->m_pIB);
  gManager.setShaderResourceView(m_player->m_sprite->m_pTexture);
  gManager.drawIndexed(static_cast<uint32>(m_player->m_sprite->m_indices.size()), 0, 0);

  //gManager.setVertexBuffers(m_player->m_pDirArrow->m_pVB);
  //gManager.setIndexBuffers(m_player->m_pDirArrow->m_pIB);
  //gManager.setShaderResourceView(m_player->m_pDirArrow->m_pTexture);
  //gManager.drawIndexed(static_cast<uint32>(m_player->m_pDirArrow->m_indices.size()), 0, 0);
}

void
PhysicsApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  const float speed = 200.0f;

  if (key == KEY::kW) {
    m_player->move(Vector2(0.0f, speed));
  }

  if (key == KEY::kA) {
    m_player->move(Vector2(-speed, 0.0f));
  }

  if (key == KEY::kS) {
    m_player->move(Vector2(0.0f, -speed));
  }

  if (key == KEY::kD) {
    m_player->move(Vector2(speed, 0.0f));
  }

  if (key == KEY::kE) {
    m_player->move(Vector2(speed, speed));
  }

  if (key == KEY::kQ) {
    m_player->move(Vector2(-speed, speed));
  }

  if (key == KEY::kZ) {
    m_player->move(Vector2(-speed, -speed));
  }

  if (key == KEY::kX) {
    m_player->move(Vector2(speed, -speed));
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
  GraphicsManager& gManager = GraphicsManager::instance();

  setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));

  m_pShader = gManager.createProgramShader("resources/PhysicsShader.hlsl",
                                          "main",
                                          "mainPS",
                                          "vs_5_0",
                                          "ps_5_0");

  Vector<InputDesc> ilDesc;
  ilDesc.resize(3);
  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[0].size = sizeof(float) * 3;

  ilDesc[1].type = INPUT_LAYOUT_TYPES::kNormal;
  ilDesc[1].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[1].size = sizeof(float) * 3;

  ilDesc[2].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[2].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[2].size = sizeof(float) * 2;

  m_pIL = gManager.createInputLayout(ilDesc, m_pShader);

  m_pSamplerLinear = gManager.createSamplerState();

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

  m_pRasterS = gManager.createRasterizerState(rasterDesc);

  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  m_pBlendS = gManager.createBlendState(blendDesc);
}

void
PhysicsApp::initCamera()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  m_pVP = gManager.createConstantBuffer(sizeof(VP));

  VP vp;
  
  float aspectRatio = m_desc.width * 0.5f;

  m_camera.setOrthographicProjData(-aspectRatio,
                                   aspectRatio,
                                   -aspectRatio,
                                   aspectRatio,
                                   0.1f,
                                   100.0f);

  Vector3 eye(0.0f, 0.0f, -10.0f);
  Vector3 at(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);

  m_camera.setViewData(eye, at, up);

  vp.proj = m_camera.getOrthographicProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  gManager.updateConstantBuffer(m_pVP, &vp, sizeof(vp));
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