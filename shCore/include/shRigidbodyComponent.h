/*****************************************************************************/
/*
*  @file    shRigidbodyComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Rigidbody component for the physics system.
*
*  Rigidbody component for the physics system.
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
#include <shRigidbody.h>

namespace shEngineSDK {
/**
*  @brief Rigidbody component for the physics system.
*/
class SH_CORE_EXPORT RigidbodyComponent : public Component
{
 public:
  /**
  *  @brief Default constructor for RigidbodyComponent.
  */
  RigidbodyComponent() : Component(COMPONENT_TYPE::kRigidbody)
  {}

  /**
  *  @brief Destructor for RigidbodyComponent.
  */
  ~RigidbodyComponent() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Rigidbody instance for the physics system.
  */
  Rigidbody m_rigidbody;
};
}