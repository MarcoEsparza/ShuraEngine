/*****************************************************************************/
/*
*  @file    shBall.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Ball class.
*
*  Ball class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shBall.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shImageResource.h"
#include "shTime.h"
#include "shMath.h"
#include "shRadian.h"

namespace shEngineSDK {
void
Ball::setSprite(const SPtr<Sprite>& sprite)
{
  m_sprite = sprite;

  m_buffer = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));
  g_graphicsMan().updateConstantBuffer(m_buffer, &m_transform, sizeof(Matrix4));
}

void
Ball::update(const INTEGRATION::E integration)
{
  if (integration == INTEGRATION::kEuler) {
    m_time += g_time().getFrameDeltaTime();
  }
  else if (integration == INTEGRATION::kVerlet) {
    m_time += g_time().FIXED_DELTA_TIME;
  }

  if (m_time >= m_lifeSpan) {
    m_bDestroy = true;
  }

  //m_position += m_velocity * g_time().getFrameDeltaTime();
  if (integration == INTEGRATION::kEuler) {
    simulateEuler();
  }
  else if (integration == INTEGRATION::kVerlet) {
    simulateVerlet();
  }

  m_transform = TranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));
  g_graphicsMan().updateConstantBuffer(m_buffer, &m_transform, sizeof(Matrix4));
}

void
Ball::simulateEuler()
{
  const Vector2 gravity(0.0f, m_eulerGravity);

  const Vector2 gravityForce = gravity * m_mass;
  const Vector2 dragForce = m_velocity * -m_dragC;
  const Vector2 accel = dragForce * (1.0f / m_mass);
  m_velocity += gravityForce + (accel * g_time().getFrameDeltaTime());
  m_position += m_velocity * g_time().getFrameDeltaTime();
}

void
Ball::simulateVerlet()
{
  if (m_previousPosition.x == 0.0f && m_previousPosition.y == 0.0f)
  {
    m_previousPosition = m_position;
  }
  const Vector2 gravity(0.0f, m_verletGravity * m_mass);
  const Vector2 gravityForce = m_accel + gravity;
  Vector2 tempPos = m_position;
  m_velocity = (m_position - m_previousPosition);
  Vector2 dragForce = m_velocity * (-m_dragC);

  m_position += dragForce + gravityForce;
  m_previousPosition = tempPos;

  m_accel.x += gravity.x;
  m_accel.y += gravity.y;
}
}
