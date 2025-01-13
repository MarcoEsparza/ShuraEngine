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
#include "shImageResource.h"
#include "shPath.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
PhysicsApp::onCreate()
{
  GraphicsManager& gManager = GraphicsManager::instance();
  ResourceManager& rManager = ResourceManager::instance();

  setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));

  m_pShader = gManager.createProgramShader("resources/PhysicsShader.hlsl",
                                          "main",
                                          "mainPS",
                                          "vs_5_0",
                                          "ps_5_0");

  Vector<InputDesc> ilDesc;
  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[0].size = 12;

  ilDesc[1].type = INPUT_LAYOUT_TYPES::kNormal;
  ilDesc[1].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[1].size = 12;

  ilDesc[2].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[2].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[2].size = 8;

  m_pIL = gManager.createInputLayout(ilDesc, m_pShader);

  m_pSamplerLinear = gManager.createSamplerState();

  Path textPath("resources/ttgl.png");
  auto pImgR = reinterpret_pointer_cast<ImageResource>(
               rManager.loadResourceFromFile(textPath));

  Vector<VertexData> vertices;
  Vector<uint32> indices;

  vertices.resize(4);
  vertices[0].position = Vector3(-0.5f, 0.5f, 0.0f);
  vertices[0].normal = Vector3(0.0f, 0.0f, 0.0f);
  vertices[0].tex = Vector2(0.0f, 0.0f);

  vertices[1].position = Vector3(0.5f, 0.5f, 0.0f);
  vertices[1].normal = Vector3(0.0f, 0.0f, 0.0f);
  vertices[1].tex = Vector2(1.0f, 0.0f);

  vertices[2].position = Vector3(-0.5f, -0.5f, 0.0f);
  vertices[2].normal = Vector3(0.0f, 0.0f, 0.0f);
  vertices[2].tex = Vector2(0.0f, 1.0f);

  vertices[3].position = Vector3(0.5f, -0.5f, 0.0f);
  vertices[3].normal = Vector3(0.0f, 0.0f, 0.0f);
  vertices[3].tex = Vector2(1.0f, 1.0f);

  indices = { 0, 1, 2,
              2, 1, 3 };

  auto pVB = gManager.createVertexBuffer(vertices);
  auto pIB = gManager.createIndexBuffer(indices);

  m_player = make_shared<Player>(pVB, pIB, pImgR->texture, vertices, indices);
}

void
PhysicsApp::onUpdate(float deltaTime)
{

}

void
PhysicsApp::onRender()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setRenderTargets(gManager.getMainRenderTargetView(),
                            gManager.getMainDepthStencil(),
                            1);
  gManager.setProgramShader(m_pShader);
  gManager.setSamplerState(m_pSamplerLinear);
  //gManager.vsSetConstantBuffers(m_pWVP);
  gManager.setPrimitiveTopology();
  gManager.setInputLayout(m_pIL);
  gManager.setVertexBuffers(m_player->m_pVB);
  gManager.setIndexBuffers(m_player->m_pIB);
  gManager.setShaderResourceView(m_player->m_pTexture);
  gManager.drawIndexed(6, 0, 0);
}
}