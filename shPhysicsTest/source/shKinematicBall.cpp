/*************************************************************/
/*
*  @file    
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/03
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
#include "shKinematicBall.h"
#include "shGraphicsManager.h"
#include "shSprite.h"

namespace shEngineSDK {
KinematicBall::KinematicBall(const SPtr<Sprite>& sprite,
                             const Vector2& pos,
                             const float radius)
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_sprite = sprite;
  m_position = pos;
  m_radius = radius;

  m_transform = TranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));
  m_ballBuffer = graphMan.createConstantBuffer(sizeof(Matrix4));
  graphMan.updateConstantBuffer(m_ballBuffer, &m_transform, sizeof(Matrix4));
}

void
KinematicBall::foward(const Vector2& newPos)
{
  m_position += newPos;
  if (m_child) {
    m_child->foward(newPos);
  }
  update();
}

void
KinematicBall::inverseFabrik()
{
  //Vector2 dir = (m_position - newPos).getNormalized();
  //m_position = newPos + dir * 75.0f;
}

void
KinematicBall::update()
{
  GraphicsManager& graphMan = g_graphicsMan();
  m_transform = TranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));
  graphMan.updateConstantBuffer(m_ballBuffer, &m_transform, sizeof(Matrix4));
}

void
KinematicBall::drawSprite()
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.vsSetConstantBuffers(m_ballBuffer, 1);
  graphMan.setVertexBuffers(m_sprite->m_pVB);
  graphMan.setIndexBuffers(m_sprite->m_pIB);
  graphMan.setShaderResourceView(m_sprite->m_pTexture);
  graphMan.drawIndexed(static_cast<uint32>(m_sprite->m_indices.size()), 0, 0);

  if (m_child) {
    m_child->drawSprite();
  }
}

void
KinematicBall::setChild(const KinematicBall& child)
{
  if (m_child) {
    m_child.reset();
  }
  m_child = make_shared<KinematicBall>(child);
}
}
