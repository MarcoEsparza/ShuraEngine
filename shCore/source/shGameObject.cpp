/*****************************************************************************/
/*
*  @file    shGameObject.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Game Object class.
*
*  Game Object class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMeshResource.h"
#include <shVector3.h>
#include <shQuaternion.h>
#include <shMatrix4.h>
#include <shMath.h>
#include <shRadian.h>

using std::find;

namespace shEngineSDK {
GameObject::~GameObject()
{
  for (auto& component : m_componentList) {
    component.reset();
  }

  m_componentList.clear();

  for (auto& child : m_childList) {
    child->~GameObject();
  }
}

void
GameObject::addComponent(const SPtr<Component>& comp)
{
  m_componentList.push_back(comp);
}

void
GameObject::removeComponent(const SPtr<Component>& comp)
{
  m_componentList.erase(find(m_componentList.begin(), m_componentList.end(), comp));
}

void
GameObject::addChild(const SPtr<GameObject>& child)
{
  m_childList.push_back(child);
}

void
GameObject::removeChild(const SPtr<GameObject>& child)
{
  m_childList.erase(find(m_childList.begin(), m_childList.end(), child));
}

Vector3
GameObject::getPosition() const
{
  return m_transform.getPosition();
}

Quaternion
GameObject::getRotation() const
{
  return m_transform.getRotation();
}

Vector3
GameObject::getScale() const
{
  return m_transform.getScale();
}

void
GameObject::setPosition(const Vector3& position)
{
  m_transform.setPosition(position);

  for (auto& child : m_childList) {
    child->setPosition(child->getPosition() + position);
  }
}

void
GameObject::setRotation(const Quaternion& rotation)
{
  m_transform.setRotation(rotation);

  for (auto& child : m_childList) {
    child->setRotation(child->getRotation() + rotation);
  }
}

void
GameObject::setScale(const Vector3& scale)
{
  m_transform.setScale(scale);

  for (auto& child : m_childList) {
    child->setScale(child->getScale() + scale);
  }
}

void
GameObject::move(const Vector3& position)
{
  //transform.getTransformMatrix() *= TranslationMatrix(position);
  m_transform.setPosition(m_transform.getPosition() + position);

  for (auto& child : m_childList) {
    child->move(position);
  }
}

void
GameObject::rotate(const Vector3& rotation, float angle)
{
  //transform.getTransformMatrix() *= MatrixRotationAxis(rotation, angle);
  m_transform.setRotation(m_transform.getRotation() + Quaternion(rotation, angle));

  for (auto& child : m_childList) {
    child->rotate(rotation, angle);
  }
}

void
GameObject::scale(const Vector3& scale)
{
  //transform.getTransformMatrix() *= ScaleMatrix(scale);
  m_transform.setScale(m_transform.getScale() + scale);

  for (auto& child : m_childList) {
    child->scale(scale);
  }
}

const Matrix4&
GameObject::getTransformMatrix()
{
  /*Matrix4 parentTransform = Matrix4::IDENTITY;
  if (auto parent = m_parent.lock()) {
    parentTransform = parent->getTransformMatrix();
  }*/

  return m_transform.getTransformMatrix();
}
}