/*************************************************************/
/*
*  @file    shSprite.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/05
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
#include "shSprite.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shImageResource.h"
#include "shTime.h"
#include "shMath.h"
#include "shRadian.h"

#define MAX_VERTEX 4

namespace shEngineSDK {
Sprite::Sprite(const Path& filePath, const Vector2& min, const Vector2& max)
{
  setSprite(filePath, min, max);
}

void
Sprite::setSprite(const Path& filePath, const Vector2& min, const Vector2& max)
{
  m_vertices.resize(MAX_VERTEX);
  m_vertices[0].position = Vector3(min.x, min.y, 0.0f);
  m_vertices[0].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[0].tex = Vector2(0.0f, 0.0f);
  
  m_vertices[1].position = Vector3(max.x, min.y, 0.0f);
  m_vertices[1].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[1].tex = Vector2(1.0f, 0.0f);
  
  m_vertices[2].position = Vector3(min.x, max.y, 0.0f);
  m_vertices[2].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[2].tex = Vector2(0.0f, 1.0f);
  
  m_vertices[3].position = Vector3(max.x, max.y, 0.0f);
  m_vertices[3].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[3].tex = Vector2(1.0f, 1.0f);

  m_indices = { 0, 1, 2,
                2, 1, 3 };

  auto pImg = reinterpret_pointer_cast<ImageResource>(g_resourceMan().loadResourceFromFile(filePath));

  m_pTexture = pImg->texture;

  m_pVB = g_graphicsMan().createVertexBuffer(m_vertices);
  m_pIB = g_graphicsMan().createIndexBuffer(m_indices);
}
}
