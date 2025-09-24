/*****************************************************************************/
/*
*  @file    shColliderComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Collider Component class for physics collision detection.
*
*  Collider Component class for physics collision detection.
*
*  @bug     No bug known
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
#include <shCollider.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward Declarations
*/
/*****************************************************************************/

class GameObject;

/**
*  @brief ColliderComponent class for physics collision detection.
*/
class SH_CORE_EXPORT ColliderComponent : public Component
{
 public:
  /**
  *  @brief Default constructor for ColliderComponent.
  *  Initializes the component with the type kCollider.
  */
  ColliderComponent()
    : Component(COMPONENT_TYPE::kCollider)
  {}

  /**
  *  @brief Destructor for ColliderComponent.
  */
  ~ColliderComponent() = default;

 protected:
  void
  onUpdate() override {}

  /***************************************************************************/
  /*
  *  Forward Declarations
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Physics Collider.
  */
  Collider m_collider;
};
}
