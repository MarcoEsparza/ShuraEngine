/*****************************************************************************/
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
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shTransformComponent.h"

namespace shEngineSDK {
const Matrix4&
TransformComponent::getTransformMatrix()
{
  if (m_bDirty) {
    Matrix4 translation = TranslationMatrix(m_position);
    Matrix4 rotation = RotationMatrix(m_rotation);
    Matrix4 scale = ScaleMatrix(m_scale);
    Matrix4 result = translation * rotation * scale;
    m_transform = result;
    m_bDirty = false;
  }

  return m_transform;
}

const Vector3&
TransformComponent::getPosition() const
{
  return m_position;
}

const Quaternion&
TransformComponent::getRotation() const
{
  return m_rotation;
}

const Vector3&
TransformComponent::getScale() const
{
  return m_scale;
}

void
TransformComponent::setPosition(const Vector3& position)
{
  //m_transform.setPosition(position);
  m_position = position;
  m_bDirty = true;
}

void
TransformComponent::setRotation(const Quaternion& rotation)
{
  //m_transform.setRotation(rotation);
  m_rotation = rotation;
  m_bDirty = true;
}

void
TransformComponent::setScale(const Vector3& scale)
{
  //m_transform.setScale(scale);
  m_scale = scale;
  m_bDirty = true;
}
}
