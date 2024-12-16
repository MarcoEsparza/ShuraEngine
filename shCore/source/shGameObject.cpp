/*************************************************************/
/*
*  @file    shGameObject.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Game Object class.
*
*  Game Object class.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shGameObject.h"

namespace shEngineSDK {
void
GameObject::addComponent(const SPtr<Component>& comp)
{
  components.push_back(comp);
}

void
GameObject::removeComponent(const SPtr<Component>& comp)
{
  components.erase(std::find(components.begin(), components.end(), comp));
}

void
GameObject::addChild(const SPtr<GameObject>& child)
{
  childs.push_back(child);
}

void
GameObject::removeChild(const SPtr<GameObject>& child)
{
  childs.erase(std::find(childs.begin(), childs.end(), child));
}

Vector3
GameObject::getPosition() const
{
  return Vector3();
}

Vector3
GameObject::getRotation() const
{
  return Vector3();
}

Vector3
GameObject::getScale() const
{
  return Vector3();
}

void
GameObject::setPosition(const Vector3& position)
{
  transform.getTransform().setPosition(position);
}

void
GameObject::setRotation(const Vector3& rotation)
{
  transform.getTransform().setRotation(rotation);
}

void
GameObject::setScale(const Vector3& scale)
{
  transform.getTransform().setScale(scale);
}
}