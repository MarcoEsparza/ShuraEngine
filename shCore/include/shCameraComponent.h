/*****************************************************************************/
/*
*  @file    shCameraComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/11/19
*  @brief   
*
*  
*
*  @bug     No bug known.
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
#include "shCamera.h"

namespace shEngineSDK {
class SH_CORE_EXPORT CameraComponent : public Component
{
public:
  /**
  *  @brief Default constructor.
  */
  CameraComponent() : Component(COMPONENT_TYPE::kCamera) {}
  /**
  *  @brief Default destructor.
  */
  ~CameraComponent() = default;
  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
public:
  /**
  *  @brief Camera instance.
  */
  Camera camera;
};
}
