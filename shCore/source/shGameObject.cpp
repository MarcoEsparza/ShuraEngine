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
#include <shMath.h>
#include <shRadian.h>

using std::find;

namespace shEngineSDK {
GameObject::~GameObject()
{
  for (auto& component : components) {
    component.reset();
  }

  components.clear();

  for (auto& child : childs) {
    child->~GameObject();
  }
}

void
GameObject::addComponent(const SPtr<Component>& comp)
{
  components.push_back(comp);
}

void
GameObject::removeComponent(const SPtr<Component>& comp)
{
  components.erase(find(components.begin(), components.end(), comp));
}

void
GameObject::addChild(const SPtr<GameObject>& child)
{
  childs.push_back(child);
}

void
GameObject::removeChild(const SPtr<GameObject>& child)
{
  childs.erase(find(childs.begin(), childs.end(), child));
}

Vector3
GameObject::getPosition() const
{
  return transform.getPosition();
}

Vector3
GameObject::getRotation() const
{
  return transform.getRotation();
}

Vector3
GameObject::getScale() const
{
  return transform.getScale();
}

void
GameObject::setPosition(const Vector3& position)
{
  transform.setPosition(position);

  for (auto& child : childs) {
    child->setPosition(child->getPosition() + position);
  }
}

void
GameObject::setRotation(const Vector3& rotation)
{
  transform.setRotation(rotation);

  for (auto& child : childs) {
    child->setRotation(child->getRotation() + rotation);
  }
}

void
GameObject::setScale(const Vector3& scale)
{
  transform.setScale(scale);

  for (auto& child : childs) {
    child->setScale(child->getScale() + scale);
  }
}

void
GameObject::move(const Vector3& position)
{
  transform.getTransform() *= TranslationMatrix(position);
}

void
GameObject::rotate(const Vector3& rotation, float angle)
{
  transform.getTransform() *= MatrixRotationAxis(rotation, angle);
}

void
GameObject::scale(const Vector3& scale)
{
  transform.getTransform() *= ScaleMatrix(scale);
}
}