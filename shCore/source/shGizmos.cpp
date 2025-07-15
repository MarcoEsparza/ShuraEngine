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
#include "shSphere.h"
#include "shCapsule.h"
#include "shCamera.h"
#include "shMatrix4.h"
#include "shMath.h"

#define MAX_LINES                     1000
#define MIN_VERTICES_IN_SPHERE        8.0f
#define MAX_VERTICES_IN_SPHERE        180.0f

namespace shEngineSDK {
void
Gizmos::onStartUp()
{
  GraphicsManager& graphMan = g_graphicsMan();
  RenderManager& renderMan = g_renderMan();

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
  auto& pMainBuffer = renderMan.getMainBuffer();
  m_pass->addVSConstantBuffer(pMainBuffer, 0);

  m_vertices.resize(sizeof(GizmosVertex) * (MAX_LINES * 2));
  m_vertexBuffer = graphMan.createVertexBuffer(m_vertices);
}

void
Gizmos::drawGizmos(const Camera& camera)
{
  SceneGraph& scene = g_sceneGraph();

  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kCollider) {
        auto colliderComp = sh_reinterpretPCast<ColliderComponent>(component);
        
        if (colliderComp->m_collider.m_type == COLLIDER_TYPE::kOBBox) {
          drawBox(colliderComp->m_collider.m_box, gameObject->transform.getTransform());
        }
        else if(colliderComp->m_collider.m_type == COLLIDER_TYPE::kSphere) {
          drawSphere(colliderComp->m_collider.m_sphere,
                     gameObject->transform.getTransform(),
                     camera);
        }
        else if (colliderComp->m_collider.m_type == COLLIDER_TYPE::kCapsule) {
          drawCapsule(colliderComp->m_collider.m_capsule,
                      gameObject->transform.getTransform(),
                      camera);
        }
      }
      else if(component->getType() == COMPONENT_TYPE::kCamera) {
        //auto cameraComp = sh_reinterpretPCast<CameraComponent>(component);
        //if (cameraComp->getCamera()) {
        //  drawFrustum(*cameraComp->getCamera());
        //}
      }
    }
  }

  render();
}

void
Gizmos::render()
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (m_numVerticesInFrame == 0) {
    return;
  }

  auto pMainTex = graphMan.getMainRenderTargetView();
  auto pDepthTex = graphMan.getMainDepthStencil();

  graphMan.setPrimitiveTopology(PRIMITIVE_TOPOLOGY::kLineList);
  graphMan.updateVertexBuffer(m_vertexBuffer, m_vertices.data(),
                              m_numVerticesInFrame * sizeof(GizmosVertex));
  m_pass->setPass();
  graphMan.setRenderTargets({ pMainTex }, pDepthTex);
  graphMan.setVertexBuffers(m_vertexBuffer);
  graphMan.draw(m_numVerticesInFrame, 0);
  
  m_vertices.clear();
  m_vertices.resize(sizeof(GizmosVertex) * (MAX_LINES * 2));
  m_numVerticesInFrame = 0;
}

void
Gizmos::drawLine(const Vector3& from, const Vector3& to)
{
  m_vertices[m_numVerticesInFrame++] = { from };
  m_vertices[m_numVerticesInFrame++] = { to };
}

void
Gizmos::drawBox(const OBBox& box, const Transform& InTfrm)
{
  auto verts = box.getVertices();

  for(uint32 i = 0; i < 8; ++i) {
    verts[i] = InTfrm.transformPoint(verts[i]);
  }

  // Front face
  drawLine(verts[0], verts[1]);
  drawLine(verts[1], verts[3]);
  drawLine(verts[3], verts[2]);
  drawLine(verts[2], verts[0]);

  // Back face
  drawLine(verts[4], verts[5]);
  drawLine(verts[5], verts[7]);
  drawLine(verts[7], verts[6]);
  drawLine(verts[6], verts[4]);

  // Connect front and back faces
  drawLine(verts[0], verts[4]);
  drawLine(verts[1], verts[5]);
  drawLine(verts[2], verts[6]);
  drawLine(verts[3], verts[7]);
}

void
Gizmos::drawSphere(const Sphere& sphere, const Transform& InTfrm, const Camera& camera)
{
  const Vector3 camPos = camera.getPosition();
  const float maxScale = Math::max(InTfrm.getScale().x,
                                   Math::max(InTfrm.getScale().y, InTfrm.getScale().z));
  const float radius = sphere.getRadius() * maxScale;

  const float distance = Vector3::distance(sphere.getCenter(), camPos);
  const Vector2 viewDims = Vector2(camera.getWidth(), camera.getHeight());
  const float focalLength = Math::min(viewDims.x, viewDims.y);
  const float numVertexRatio = focalLength * radius / distance;
  const uint32 numVertices = static_cast<uint32>(Math::clamp(numVertexRatio,
                                                             MIN_VERTICES_IN_SPHERE,
                                                             MAX_VERTICES_IN_SPHERE));
  Vector<Vector3> vertices(numVertices * 3);
  const float vertexAngle = Math::TWO_PI / numVertices;
  const uint32 numVertices2 = 2 * numVertices;

  for (uint32 i = 0, j = numVertices, k = numVertices2;
       i < numVertices; ++i, ++j, ++k) {
    float sinAngle = Math::sin(Radian(i * vertexAngle)) * radius;
    float cosAngle = Math::cos(Radian(i * vertexAngle)) * radius;

    vertices[i].x = cosAngle;
    vertices[i].y = 0.0f;
    vertices[i].z = sinAngle;

    vertices[j].x = cosAngle;
    vertices[j].y = sinAngle;
    vertices[j].z = 0.0f;

    vertices[k].x = 0.0f;
    vertices[k].y = cosAngle;
    vertices[k].z = sinAngle;

    vertices[i] *= InTfrm.getScale();
    vertices[i] *= InTfrm.getRotation();
    vertices[i] += InTfrm.getPosition();

    vertices[j] *= InTfrm.getScale();
    vertices[j] *= InTfrm.getRotation();
    vertices[j] += InTfrm.getPosition();

    vertices[k] *= InTfrm.getScale();
    vertices[k] *= InTfrm.getRotation();
    vertices[k] += InTfrm.getPosition();
  }

  uint32 numVerticesMinus1 = numVertices - 1;

  for(uint32 i = 0, j = numVertices, k = numVertices2;
      i < numVerticesMinus1; ++i, ++j, ++k) {
    drawLine(vertices[i], vertices[i + 1]);
    drawLine(vertices[j], vertices[j + 1]);
    drawLine(vertices[k], vertices[k + 1]);
  }

  drawLine(vertices[numVerticesMinus1], vertices[0]);
  drawLine(vertices[numVertices2 - 1], vertices[numVertices]);
  drawLine(vertices[numVertices2 + numVerticesMinus1], vertices[numVertices2]);
}

void
Gizmos::drawCapsule(const Capsule& capsule, const Transform& InTfrm, const Camera& camera)
{
  const Vector3 camPos = camera.getPosition();
  const float radius = capsule.radius * InTfrm.getScale().y;
  const float halfHeight = capsule.height * 0.5f;

  const float distance = Vector3::distance(InTfrm.getPosition(), camPos);
  const Vector2 viewDims = Vector2(camera.getWidth(), camera.getHeight());
  const float focalLength = Math::min(viewDims.x, viewDims.y);
  const float numVertexRatio = 0.5f * focalLength * radius / distance;
  uint32 numVertices = static_cast<uint32>(numVertexRatio);
  numVertices = static_cast<uint32>(Math::clamp(static_cast<float>(numVertices),
                                                MIN_VERTICES_IN_SPHERE,
                                                MAX_VERTICES_IN_SPHERE));
  uint32 totalVertices = numVertices * 4;
  Vector<Vector3> vertices(totalVertices);

  const float vertexAngle = Math::HALF_PI / numVertices;

  Vector3 up(0.0f, halfHeight * InTfrm.getScale().y, 0.0f);
  up = InTfrm.getRotation() * up;

  for (uint32 i = 0; i < totalVertices; i += 4) {
    float sinAngle = Math::sin(Radian(i * vertexAngle)) * radius;
    float cosAngle = Math::cos(Radian(i * vertexAngle)) * radius;

    vertices[i]     = Vector3(cosAngle, sinAngle, 0.0f);
    vertices[i + 1] = Vector3(0.0f, sinAngle, cosAngle);
    vertices[i + 2] = Vector3(cosAngle, 0.0f, sinAngle);
    vertices[i + 3] = vertices[i + 2];

    for (uint32 j = 0; j < 4; ++j) {
      vertices[i + j] *= InTfrm.getScale();
      vertices[i + j] *= InTfrm.getRotation();
      vertices[i + j] += InTfrm.getPosition();
    }

    Vector3 hUp = i >= totalVertices / 2 ? up : -up;
    vertices[i] += hUp;
    vertices[i + 1] += hUp;
    vertices[i + 2] += hUp;
    vertices[i + 3] += hUp;
  }

  totalVertices -= 4;

  for (uint32 i = 0; i < totalVertices; i += 4) {
    drawLine(vertices[i], vertices[i + 4]);
    drawLine(vertices[i + 1], vertices[i + 5]);
    drawLine(vertices[i + 2], vertices[i + 6]);
    drawLine(vertices[i + 3], vertices[i + 7]);
  }

  drawLine(vertices[totalVertices], vertices[0]);
  drawLine(vertices[totalVertices + 1], vertices[1]);
  drawLine(vertices[totalVertices + 2], vertices[2]);
  drawLine(vertices[totalVertices + 3], vertices[3]);
}

void
Gizmos::drawFrustum(const Camera&)
{
  // TODO: Implement frustum drawing logic
}
}
