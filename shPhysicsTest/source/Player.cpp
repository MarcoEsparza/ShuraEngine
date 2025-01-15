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

  Vector2 velPos = m_position + m_velocity;

  if (m_position.x == velPos.x && m_position.y < velPos.y) {
    m_direction = DIRECTION::kUp;
  }
  else if (m_position.x < velPos.x && m_position.y < velPos.y) {
    m_direction = DIRECTION::kUpRight;
  }
  else if (m_position.x < velPos.x && m_position.y == velPos.y) {
    m_direction = DIRECTION::kRight;
  }
  else if (m_position.x < velPos.x && m_position.y > velPos.y) {
    m_direction = DIRECTION::kDownRight;
  }
  else if (m_position.x == velPos.x && m_position.y > velPos.y) {
    m_direction = DIRECTION::kDown;
  }
  else if (m_position.x > velPos.x && m_position.y > velPos.y) {
    m_direction = DIRECTION::kDownLeft;
  }
  else if (m_position.x > velPos.x && m_position.y == velPos.y) {
    m_direction = DIRECTION::kLeft;
  }
  else if (m_position.x > velPos.x && m_position.y < velPos.y) {
    m_direction = DIRECTION::kUpLeft;
  }

  updateArrow();
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

void
Player::updateArrow()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  Vector2 p1 = m_position;
  Vector2 p2 = m_position;
  Vector2 p3 = m_position;
  Vector2 p4 = m_position;

  if (m_direction == DIRECTION::kUp) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }
  else if (m_direction == DIRECTION::kUpRight) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }
  else if (m_direction == DIRECTION::kRight) {
    p1 += Vector2(25.0f, 25.0f);
    p2 += Vector2(65.0f, 25.0f);
    p3 += Vector2(25.0f, -25.0f);
    p4 += Vector2(65.0f, -25.0f);
  }
  else if (m_direction == DIRECTION::kDownRight) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }
  else if (m_direction == DIRECTION::kDown) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }
  else if (m_direction == DIRECTION::kDownLeft) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }
  else if (m_direction == DIRECTION::kLeft) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-65.0f, 25.0f);
    p3 += Vector2(-25.0f, -25.0f);
    p4 += Vector2(-65.0f, -25.0f);
  }
  else if (m_direction == DIRECTION::kUpLeft) {
    p1 += Vector2(-25.0f, 25.0f);
    p2 += Vector2(-25.0f, 65.0f);
    p3 += Vector2(25.0f, 25.0f);
    p4 += Vector2(25.0f, 65.0f);
  }

  m_pDirArrow->m_vertices[0].position.x = p1.x;
  m_pDirArrow->m_vertices[0].position.y = p1.y;

  m_pDirArrow->m_vertices[1].position.x = p2.x;
  m_pDirArrow->m_vertices[1].position.y = p2.y;

  m_pDirArrow->m_vertices[2].position.x = p3.x;
  m_pDirArrow->m_vertices[2].position.y = p3.y;

  m_pDirArrow->m_vertices[3].position.x = p4.x;
  m_pDirArrow->m_vertices[3].position.y = p4.y;

  m_pDirArrow->m_pVB = gManager.createVertexBuffer(m_pDirArrow->m_vertices);
}
}
