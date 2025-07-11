/*****************************************************************************/
/*
*  @file    shColliderComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/10
*  @brief
*
*
*
* @bug      No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shComponent.h"
#include "shCollider.h"

namespace shEngineSDK {
class GameObject;

class SH_CORE_EXPORT ColliderComponent : public Component
{
 public:
  ColliderComponent() : Component(COMPONENT_TYPE::kCollider)
  {}

  ~ColliderComponent() = default;

 public:
  Collider m_collider;
};
}
