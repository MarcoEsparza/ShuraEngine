/*************************************************************/
/*
*  @file    shGameObject.h
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
#include "shPrerequisitesCore.h"
#include "shComponent.h"

namespace shEngineSDK {
/**
*  @brief Game Object class.
*/
class GameObject
{
 public:
  /**
  *  @brief Default constructor.
  */
  GameObject();
  /**
  *  @brief Default destructor.
  */
  ~GameObject() = default;

  /**
  *  @brief Add component to the component vector.
  * 
  *  @param SPtr<Component>& comp
  */
  void
  addComponent(const SPtr<Component>& comp);

  /**
  *  @brief Game Object name.
  */
  String name;

  /**
  *  @brief Component vector.
  */
  Vector <SPtr<Component>> components;
};
}
