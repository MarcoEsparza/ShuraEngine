/*****************************************************************************/
/*
*  @file    shGizmos.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/10
*  @brief
*
*
*
* @bug      No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shModule.h"
#include "shVector3.h"
#include "shLinearColor.h"
//#include "shCamera.h"

namespace shEngineSDK {
class ColliderComponent;
class Pass;
class Texture2D;
class VertexBuffer;
class OBBox;
class Sphere;
class Capsule;
//class Transform;
class Matrix4;
class TransformComponent;
class Camera;

struct GizmosVertex
{
  GizmosVertex() = default;
  GizmosVertex(const Vector3& pos, const LinearColor& col = LinearColor::YELLOW)
    : position(pos), color(col)
  {}

  Vector3 position = Vector3::ZERO;
  LinearColor color = LinearColor::YELLOW;
};

class SH_CORE_EXPORT Gizmos : public Module<Gizmos>
{
 public:
  Gizmos() = default;

  virtual ~Gizmos() = default;

 public:
  void
  onStartUp() override;

  void
  drawGizmos(const Camera& camera);

  void
  onShutDown() override;

 private:
  void
  render();

  void
  drawLine(const Vector3& from, const Vector3& to);

  void
  drawBox(const OBBox& box, const TransformComponent& InTfrm);

  void
  drawSphere(const Sphere& sphere, const TransformComponent& InTfrm, const Camera& camera);

  void
  drawCapsule(const Capsule& capsule, const TransformComponent& InTfrm, const Camera& camera);

  void
  drawFrustum(const Camera& camera);

 private:
  SPtr<Pass> m_pass;
  //SPtr<Texture2D> m_linesTex;
  //SPtr<Texture2D> m_depthStencil;
  SPtr<VertexBuffer> m_vertexBuffer;

  Vector<GizmosVertex> m_vertices;
  uint32 m_numVerticesInFrame = 0;
};
}
