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
  return m_transform.getPosition();
}

Vector3
TransformComponent::getRotation() const
{
  return m_transform.getRotation();
}

Vector3
TransformComponent::getScale() const
{
  return m_transform.getScale();
}

void
TransformComponent::setPosition(const Vector3& position)
{
  m_transform.setPosition(position);
}

void
TransformComponent::setRotation(const Vector3& rotation)
{
  m_transform.setRotation(rotation);
}

void
TransformComponent::setScale(const Vector3& scale)
{
  m_transform.setScale(scale);
}
}
