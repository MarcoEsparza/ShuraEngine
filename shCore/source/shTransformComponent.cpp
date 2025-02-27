/*************************************************************/
/*
*  @file    shTransformComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/18
*  @brief   Transform Component class.
*
*  Transform Component class.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shTransformComponent.h"

namespace shEngineSDK {
Transform&
TransformComponent::getTransform()
{
  return m_transform;
}

Vector3
TransformComponent::getPosition() const
{
  return m_position;
}

Vector3
TransformComponent::getRotation() const
{
  return m_rotation;
}

Vector3
TransformComponent::getScale() const
{
  return m_scale;
}

void
TransformComponent::setPosition(const Vector3& position)
{
  m_transform.setPosition(position);
  m_position = position;
}

void
TransformComponent::setRotation(const Vector3& rotation)
{
  m_transform.setRotation(rotation);
  m_rotation = rotation;
}

void
TransformComponent::setScale(const Vector3& scale)
{
  m_transform.setScale(scale);
  m_scale = scale;
}
}
