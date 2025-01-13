/*************************************************************/
/*
*  @file    Player.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/13
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shPrerequisitesPhysics.h"
#include "shGraphicTypes.h"
#include "shVector2.h"

namespace shEngineSDK {
class VertexBuffer;
class IndexBuffer;
class Texture2D;

class Player
{
 public:
  Player(const SPtr<VertexBuffer>& vertexB,
         const SPtr<IndexBuffer>& indexB,
         const SPtr<Texture2D>& texture,
         const Vector<VertexData>& vertices,
         const Vector<uint32>& indices)
         : m_pVB(vertexB),
           m_pIB(indexB),
           m_pTexture(texture),
           m_vertices(vertices),
           m_indices(indices) {}
  ~Player() = default;

  void
  move(const Vector2& direction);

 public:
  SPtr<VertexBuffer> m_pVB;
  SPtr<IndexBuffer> m_pIB;
  SPtr<Texture2D> m_pTexture;
  Vector<VertexData> m_vertices;
  Vector<uint32> m_indices;

  Vector2 m_velocity;
  Vector2 m_direction;
  Vector2 m_position;
};
}
