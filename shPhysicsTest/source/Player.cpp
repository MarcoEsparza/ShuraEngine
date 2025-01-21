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
#include "shRadian.h"

#define MAX_VERTEX 4.0f

namespace shEngineSDK {
Player::Player(const Vector2& min,
               const Vector2& max,
               const Path& filePath,
               const Vector2& position,
               const float mass,
               const float speed,
               const float dragC,
               const float radius)
{
  setPlayer(min, max, filePath, position, mass, speed, dragC, radius);
  m_velocity = Vector2(0.0f, 0.0f);
}

void
Player::setPlayer(const Vector2& min,
                  const Vector2& max,
                  const Path& filePath,
                  const Vector2& position,
                  const float mass,
                  const float speed,
                  const float dragC,
                  const float radius)
{
  m_sprite = make_shared<Sprite>(filePath, min, max);
  m_position = position;
  m_mass = mass;
  m_speed = speed;
  m_dragC = dragC;
  m_radius = radius;
  m_modelBuffer = GraphicsManager::instance().createConstantBuffer(sizeof(Matrix4));
  updateModelBuffer();
}

void
Player::setArrow(const SPtr<Arrow>& arrow)
{
  m_pDirArrow = arrow;
}

void
Player::move(const Vector2& direction)
{
  m_velocity = direction * m_speed;
}

Vector2
Player::calculateDrag()
{
  float speed = m_velocity.mag();

  if (speed == 0.0f) {
    return Vector2(0.0f, 0.0f);
  }

  Vector2 dragForce = m_velocity * (-m_dragC * speed / m_mass);

  return dragForce;
}

void
Player::eulerDrag(const Vector2& dForce, const float dt)
{
  m_velocity = m_velocity + dForce * dt;
  m_position = m_position + m_velocity * dt;
}

void
Player::verletDrag(const Vector2& dForce, const float dt)
{
  Vector2 nextPosition = m_position * 2.0f - m_previousPosition + dForce * (dt * dt);
  m_velocity = (nextPosition - m_previousPosition);
  m_velocity.x /= (2 * dt);
  m_velocity.y /= (2 * dt);

  m_previousPosition = m_position;
  m_position = nextPosition;
}

void
Player::update(float deltaTime)
{
  if (m_velocity.x == 0.0f && m_velocity.y == 0.0f) {
    return;
  }

  eulerDrag(calculateDrag(), deltaTime);

  //verletDrag(calculateDrag(), deltaTime);

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

  updateArrow();
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
  GraphicsManager& gManager = GraphicsManager::instance();

  float angle = 0.0f;

  if (m_direction == DIRECTION::kUp) {
    angle = 90.0f;
  }
  else if (m_direction == DIRECTION::kUpRight) {
    angle = 45.0f;
  }
  else if (m_direction == DIRECTION::kRight) {
    angle = 0.0f;
  }
  else if (m_direction == DIRECTION::kDownRight) {
    angle = -45.0f;
  }
  else if (m_direction == DIRECTION::kDown) {
    angle = -90.0f;
  }
  else if (m_direction == DIRECTION::kDownLeft) {
    angle = -135.0f;
  }
  else if (m_direction == DIRECTION::kLeft) {
    angle = 180.0f;
  }
  else if (m_direction == DIRECTION::kUpLeft) {
    angle = 135.0f;
  }

  Matrix4 rot = Matrix4::IDENTITY;
  Matrix4 scale = Matrix4::IDENTITY;
  Matrix4 trans = Matrix4::IDENTITY;
  scale = scale.createScaleMatrix(Vector3(1.0f, 1.0f, 1.0f));
  rot = rot.createRotationZMatrix(Radian(angle * Math::DEG2RAD));
  trans = m_transform.createTranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));

  m_pDirArrow->m_position = trans * rot * scale;

  gManager.updateConstantBuffer(m_pDirArrow->m_modelBuffer,
                                &m_pDirArrow->m_position,
                                sizeof(Matrix4));
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

  auto pImg = reinterpret_pointer_cast<ImageResource>(rManager.loadResourceFromFile(filePath));

  m_pTexture = pImg->texture;

  m_pVB = gManager.createVertexBuffer(m_vertices);
  m_pIB = gManager.createIndexBuffer(m_indices);
}

Arrow::Arrow(const Path& filePath, const Vector2& min, const Vector2& max)
{
  m_sprite = make_shared<Sprite>(filePath, min, max);

  GraphicsManager& gManager = GraphicsManager::instance();

  m_modelBuffer = GraphicsManager::instance().createConstantBuffer(sizeof(Matrix4));

  gManager.updateConstantBuffer(m_modelBuffer, &m_position, sizeof(Matrix4));
}
}
