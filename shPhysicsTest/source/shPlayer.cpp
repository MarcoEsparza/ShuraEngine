/*****************************************************************************/
/*
*  @file    Player.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Player and other classes for physics simulation.
*
*  Player and other classes for physics simulation.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPlayer.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shImageResource.h"
#include "shTime.h"
#include "shMath.h"
#include "shRadian.h"

#define MAX_VERTEX 4

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
  Vector2 nextPosition = (m_position * 2.0f) - m_previousPosition + (dForce * (dt * dt));
  m_velocity = (nextPosition - m_previousPosition);
  m_velocity.x /= (2.0f * dt);
  m_velocity.y /= (2.0f * dt);

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
}

void
Player::updateModelBuffer()
{
  GraphicsManager& gManager = GraphicsManager::instance();

  m_transform = m_transform.createTranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));

  gManager.updateConstantBuffer(m_modelBuffer, &m_transform, sizeof(Matrix4));
}
}
