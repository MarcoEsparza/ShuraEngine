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
#include "shMeshComponent.h"
#include "shMeshResource.h"
#include "shMath.h"

using std::reinterpret_pointer_cast;

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
GameObject::move(const Vector3& position)
{
  transform.setPosition(position);

  for (auto& component : components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
      auto smComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
      for (auto& vertex : smComponent->meshData->vertices) {
        vertex.position.x += position.x;
        vertex.position.y += position.y;
        vertex.position.z += position.z;
      }
    }
  }
}

void
GameObject::rotate(const Vector3& rotation)
{
  transform.setRotation(rotation);

  for (auto& component : components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
      auto smComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
      for (auto& vertex : smComponent->meshData->vertices) {
        vertex.position = vertex.position.rotateX(rotation.x * Math::DEG2RAD);
        vertex.position = vertex.position.rotateY(rotation.y * Math::DEG2RAD);
        vertex.position = vertex.position.rotateZ(rotation.z * Math::DEG2RAD);
      }
    }
  }
}

void
GameObject::scale(const Vector3& scale)
{
  transform.setScale(scale);

  for (auto& component : components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
      auto smComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
      for (auto& vertex : smComponent->meshData->vertices) {
        vertex.position.x *= scale.x;
        vertex.position.y *= scale.y;
        vertex.position.z *= scale.z;
      }
    }
  }
}
}