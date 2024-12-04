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
#include "shTransformComponent.h"

namespace shEngineSDK {
GameObject::GameObject()
{
  auto transform = make_shared<TransformComponent>();
  transform->getTransform().setPosition(Vector3(0.0f, 0.0f, 0.0f));
  transform->getTransform().setRotation(Vector3(0.0f, 0.0f, 0.0f));
  transform->getTransform().setScale(Vector3(0.0f, 0.0f, 0.0f));

  components.push_back(transform);
}

void
GameObject::addComponent(const SPtr<Component>& comp)
{
  components.push_back(comp);
}
}