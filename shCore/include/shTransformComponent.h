/*************************************************************/
/*
*  @file    shTransformComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Transform Component class.
*
*  Transform Component class.
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
#include "shMatrix4.h"

namespace shEngineSDK {
/**
*  @brief Transform Component class.
*/
class TransformComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  TransformComponent() : Component(COMPONENT_TYPE::kTransform) {}

  /**
  *  @brief Default destructor.
  */
  ~TransformComponent() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Gets the transform data.
  * 
  *  @return Transform&
  */
  Transform&
  getTransform();

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief Transform data.
  */
  Transform m_transform;
};
}
