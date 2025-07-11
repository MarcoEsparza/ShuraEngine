/*****************************************************************************/
/*
*  @file    shGizmos.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/10
*  @brief
*
*
*
* @bug      No bug known
*/
/*****************************************************************************/


/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shGizmos.h"
#include "shGraphicsManager.h"
#include "shRenderManager.h"
#include "shSceneGraph.h"
#include "shPass.h"
#include "shCollider.h"
#include "shColliderComponent.h"
#include "shOBBox.h"

#define MAX_LINES                     1000

namespace shEngineSDK {
void
Gizmos::onStartUp()
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_pass = sh_makeShared<Pass>();
  m_pass->setVShaderInfo("resources/shaders/DebugLines.hlsl",
                         "main",
                         "vs_5_0");
  m_pass->setPShaderInfo("resources/shaders/DebugLines.hlsl",
                         "mainPS",
                         "ps_5_0");
  m_pass->compileShader();
  m_pass->generateInputLayout();

  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kWireframe;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  m_pass->setRasterizerStateFromDesc(rasterDesc);

  m_vertices.resize(MAX_LINES * 2);
  m_vertexBuffer = graphMan.createVertexBuffer(m_vertices);
}

void
Gizmos::drawGizmos()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

  auto colliders = getCollidersInScene();
  Vector<OBBox> boxes;

  for (auto& collider : colliders) {
    if (!collider.expired()) {
      auto pColl = collider.lock();
      if (pColl->m_collider.m_type == COLLIDER_TYPE::kOBBox) {
        boxes.push_back(pColl->m_collider.m_box);
      }
    }
  }

  for (auto& obb : boxes) {
    auto verts = obb.getVertices();
    for(int32 i = 0; i < 24; i += 2) {
      drawLine(verts[i], verts[i + 1]);
    }
  }

  auto pMainTex = graphMan.getMainRenderTargetView();
  auto pDepthTex = graphMan.getMainDepthStencil();
  auto& pMainBuffer = renderMan.getMainBuffer(); 

  if (m_vertices.size() > 0) {
    graphMan.setPrimitiveTopology(PRIMITIVE_TOPOLOGY::kLineList);
    graphMan.updateVertexBuffer(m_vertexBuffer, m_vertices.data(),
                                m_vertices.size() * sizeof(Vector3));
    graphMan.psSetConstantBuffers(pMainBuffer, 0);
    m_pass->setPass();
    graphMan.setRenderTargets({ pMainTex }, pDepthTex);
    graphMan.setVertexBuffers(m_vertexBuffer);
    graphMan.draw(m_vertices.size(), 0);
    m_vertices.clear();
  }
}

Vector<WPtr<ColliderComponent>>
Gizmos::getCollidersInScene()
{
  SceneGraph& scene = g_sceneGraph();
  Vector<WPtr<ColliderComponent>> colliders;

  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kCollider) {
        auto colliderComp = sh_reinterpretPCast<ColliderComponent>(component);
        colliders.push_back(colliderComp);
      }
    }
  }

  return colliders;
}

void
Gizmos::drawLine(const Vector3& from, const Vector3& to)
{
  m_vertices.push_back(from);
  m_vertices.push_back(to);
}
}
