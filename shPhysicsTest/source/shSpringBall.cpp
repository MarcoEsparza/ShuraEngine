/*************************************************************/
/*
*  @file    shSpringBall.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Spring ball for hookes law simulation.
*
*  Spring ball for hookes law simulation.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shSpringBall.h"
#include "shGraphicsManager.h"
#include "shTime.h"
#include "shMath.h"

namespace shEngineSDK {
SpringBall::SpringBall(const SPtr<Sprite>& spr,
                       const Vector2& pos,
                       const Vector2& vel,
                       const Vector2& accel,
                       const float radius,
                       const float springC,
                       const float gravity,
                       const float limit,
                       const float elasticity,
                       const float drag)
{
  GraphicsManager& graphMan = g_graphicsMan();

  m_sprite = spr;
  m_position = pos;
  m_velocity = vel;
  m_accel = accel;
  m_radius = radius;
  m_springC = springC;
  m_gravity = gravity;
  m_limit = limit;
  m_elasticity = elasticity;
  m_drag = drag;

  m_previousPosition = m_position;
  m_transform = TranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));

  m_ballBuffer = graphMan.createConstantBuffer(sizeof(Matrix4));
  graphMan.updateConstantBuffer(m_ballBuffer, &m_transform, sizeof(Matrix4));
}

void SpringBall::update(const Vector2& anchor, const INTEGRATION::E integration)
{
  if (integration == INTEGRATION::kVerlet) {
    simulateVerlet(anchor);
  }
  else if (integration == INTEGRATION::kEuler) {
    simulateEuler(anchor);
  }
}

void
SpringBall::simulateEuler(const Vector2& anchor)
{
  if (!m_bGrabbed) {
    Vector2 displacement = m_position - anchor;

    const float dist = displacement.mag();

    if (dist > m_limit) {
      Vector2 excess = displacement / dist * (dist - m_limit);
      Vector2 elasticForce = excess * -m_elasticity;
      m_accel += elasticForce;
    }

    const Vector2 springForce = displacement * -m_springC;
    m_accel += springForce;
    m_accel.y += m_gravity;

    m_velocity += m_accel * g_time().getFrameDeltaTime();
    //m_velocity = m_velocity * m_drag;
    m_position = m_velocity * g_time().getFrameDeltaTime();

    m_accel = { 0.0f, 0.0f };
  }
}

void
SpringBall::simulateVerlet(const Vector2& anchor)
{
  const Vector2 tempPos = m_position;
  Vector2 displacement = m_position - anchor;
  const float dist = displacement.mag();

  /*if (dist > m_limit) {
    Vector2 excess = displacement / dist * (dist - m_limit);
    Vector2 elasticForce = excess * -m_elasticity;
    m_accel += elasticForce;
  }*/

  bool clamp = false;

  if (dist > m_maxLenght) {
    displacement = displacement.getNormalized() * m_maxLenght;
    clamp = true;
  }
  else if (dist < m_minLenght) {
    displacement = displacement.getNormalized() * m_minLenght;
    clamp = true;
  }
  if (clamp) {
    m_position = anchor + displacement;
    //Vector2 excess = displacement / dist * (dist - m_maxLenght);
    //Vector2 elasticForce = excess * -m_elasticity;
    //m_accel += elasticForce;
    m_velocity.x = -m_velocity.x;
    m_velocity.y = -m_velocity.y;
  }

  displacement = displacement - displacement.getNormalized() * m_iniLenght;

  const Vector2 springForce = displacement * -m_springC;
  const Vector2 gravityForce = { 0.0f, m_gravity * m_mass };
  const Vector2 sumForces = springForce + gravityForce;
  m_accel += (sumForces / m_mass);
  m_velocity = m_position - m_previousPosition;

  m_previousPosition = tempPos;

  if (!m_bGrabbed) {
    m_position += m_velocity + (m_accel * g_time().FIXED_DELTA_TIME);
  }

  m_accel = { 0.0f, 0.0f };

  updateCBuffer();
}

void
SpringBall::updateCBuffer()
{
  m_transform = TranslationMatrix(Vector3(m_position.x, m_position.y, 0.0f));
  g_graphicsMan().updateConstantBuffer(m_ballBuffer, &m_transform, sizeof(Matrix4));
}
}
