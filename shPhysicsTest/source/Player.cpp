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
#include "shResourceManager.h"
#include "shImageResource.h"
#include "shMath.h"

namespace shEngineSDK {
Player::Player(const Vector2& min,
               const Vector2& max,
               const Path& filePath,
               const Vector2& position,
               const float mass, 
               const float radius)
{
  setPlayer(min, max, filePath, position, mass, radius);
  m_velocity = Vector2(0.0f, 0.0f);
}

void
Player::setPlayer(const Vector2& min,
                  const Vector2& max,
                  const Path& filePath,
                  const Vector2& position,
                  const float mass,
                  const float radius)
{
  m_sprite = make_shared<Sprite>(filePath, min, max);
  m_position = position;
  m_mass = mass;
  m_radius = radius;
  m_modelBuffer = GraphicsManager::instance().createConstantBuffer(sizeof(Matrix4));
  updateModelBuffer();
}

void
Player::setArrow(const Arrow& arrow)
{
  
}

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

  Vector2 velPos = m_position + m_velocity;

  updateModelBuffer();

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

  //updateArrow();
}

void
Player::updateModelBuffer()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  m_transform = m_transform.createTranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));

  gManager.updateConstantBuffer(m_modelBuffer, &m_transform, sizeof(Matrix4));
}

void
Player::updateArrow()
{
  //GraphicsManager& gManager = GraphicsManager::instance();

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

}

Sprite::Sprite(const Path& filePath, const Vector2& min, const Vector2& max)
{
  setSprite(filePath, min, max);
}

void
Sprite::setSprite(const Path& filePath, const Vector2& min, const Vector2& max)
{
  GraphicsManager& gManager = GraphicsManager::instance();
  ResourceManager& rManager = ResourceManager::instance();

  m_vertices.resize(4);
  m_vertices[0].position = Vector3(-min.x, min.y, 0.0f);
  m_vertices[0].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[0].tex = Vector2(0.0f, 0.0f);
  
  m_vertices[1].position = Vector3(max.x, min.y, 0.0f);
  m_vertices[1].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[1].tex = Vector2(1.0f, 0.0f);
  
  m_vertices[2].position = Vector3(-min.x, -max.y, 0.0f);
  m_vertices[2].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[2].tex = Vector2(0.0f, 1.0f);
  
  m_vertices[3].position = Vector3(max.x, -max.y, 0.0f);
  m_vertices[3].normal = Vector3(0.0f, 0.0f, 0.0f);
  m_vertices[3].tex = Vector2(1.0f, 1.0f);

  m_indices = { 0, 1, 2,
                2, 1, 3 };

  auto pImg = reinterpret_pointer_cast<ImageResource>(rManager.loadResourceFromFile(filePath));

  m_pTexture = pImg->texture;

  m_pVB = gManager.createVertexBuffer(m_vertices);
  m_pIB = gManager.createIndexBuffer(m_indices);
}
}
