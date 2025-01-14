/*************************************************************/
/*
*  @file    Player.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/13
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
#include "Player.h"
#include "shGraphicsManager.h"
#include "shMath.h"

namespace shEngineSDK {
void
Player::move(const Vector2& direction)
{
  m_velocity = direction;
}

Vector2
Player::calculateDrag()
{
  float speed = m_velocity.mag();

  if (speed == 0.0f) {
    return Vector2(0.0f, 0.0f);
  }

  float dragMag = 0.5f * 1.225f * 0.47f * 0.1f * speed * speed;

  Vector2 dragForce = m_velocity.getNormalized() * -dragMag;

  return dragForce;
}

void
Player::update(float deltaTime)
{
  if (m_velocity.x == 0.0f && m_velocity.y == 0.0f) {
    return;
  }

  Vector2 dragForce = calculateDrag();

  Vector2 acceleration(0.0f, 0.0f);
  acceleration.x = dragForce.x / m_mass;
  acceleration.y = dragForce.y / m_mass;

  m_velocity = m_velocity + acceleration * deltaTime;
  m_position = m_position + m_velocity * deltaTime;

  updateVertexBuffer();
}

void
Player::updateVertexBuffer()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  m_vertices[0].position.x = m_position.x - 25.0f;
  m_vertices[0].position.y = m_position.y + 25.0f;

  m_vertices[1].position.x = m_position.x + 25.0f;
  m_vertices[1].position.y = m_position.y + 25.0f;
  
  m_vertices[2].position.x = m_position.x - 25.0f;
  m_vertices[2].position.y = m_position.y - 25.0f;
  
  m_vertices[3].position.x = m_position.x + 25.0f;
  m_vertices[3].position.y = m_position.y - 25.0f;

  m_pVB = gManager.createVertexBuffer(m_vertices);
}
}
