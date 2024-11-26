/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
*  @brief
*
*
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

namespace shEngineSDK {
class Component;

class GameObject
{
public:
  GameObject() = default;
  virtual ~GameObject() = default;

  Vector<Component> components;
};
}
