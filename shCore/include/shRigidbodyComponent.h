/*****************************************************************************/
/*
*  @file    shRigidbodyComponent.h
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
#include "shRigidbody.h"

namespace shEngineSDK {
class SH_CORE_EXPORT RigidbodyComponent : public Component
{
 public:
  RigidbodyComponent() : Component(COMPONENT_TYPE::kRigidbody)
  {}

  ~RigidbodyComponent() = default;

 public:


 private:
  Rigidbody m_rigidbody;
};
}