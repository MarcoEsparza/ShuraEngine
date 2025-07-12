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

namespace shEngineSDK {
class ColliderComponent;
class Pass;
class Texture2D;
class VertexBuffer;
class OBBox;

//struct GizmosVertex
//{
//  Vector3 position;
//};

class SH_CORE_EXPORT Gizmos : public Module<Gizmos>
{
 public:
  Gizmos() = default;

  virtual ~Gizmos() = default;

 public:
  void
  onStartUp() override;

  void
  drawGizmos();

 private:
  Vector<WPtr<ColliderComponent>>
  getCollidersInScene();

  void
  drawLine(const Vector3& from, const Vector3& to);

  void
  drawBox(OBBox& box);

 private:
  SPtr<Pass> m_pass;
  //SPtr<Texture2D> m_linesTex;
  //SPtr<Texture2D> m_depthStencil;
  SPtr<VertexBuffer> m_vertexBuffer;

  Vector<Vector3> m_vertices;
  uint32 m_numVerticesInFrame = 0.0f;
};
}
