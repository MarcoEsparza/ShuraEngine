/*************************************************************/
/*
*  @file    shSprite.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/05
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
#include "shMatrix4.h"
#include "shSphere.h"
#include "shPath.h"
#include "shSprite.h"

namespace shEngineSDK {
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class Texture2D;

class Sprite
{
 public:
  Sprite() = default;
  Sprite(const Path& filePath, const Vector2& min, const Vector2& max);
  ~Sprite() = default;

  void
  setSprite(const Path& filePath, const Vector2& min, const Vector2& max);

  SPtr<VertexBuffer> m_pVB;
  SPtr<IndexBuffer> m_pIB;
  SPtr<Texture2D> m_pTexture;
  Vector<VertexData> m_vertices;
  Vector<uint32> m_indices;
};
}
