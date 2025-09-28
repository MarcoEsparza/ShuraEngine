/*****************************************************************************/
/*
*  @file    shLightComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/27
*  @brief   Light component class.
*
*  Light component class.
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
#include <shVector3.h>
#include <shVector4.h>
#include <shLinearColor.h>

namespace shEngineSDK {
/**
*  @brief Light type enumerator.
*/
namespace LIGHT_TYPE{
enum E
{
  kDirectional = 0,
  kPoint,
  kSpot,
  kCount
};
}

/**
*  @brief Light component class.
*/
class SH_CORE_EXPORT LightComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  LightComponent() : Component(COMPONENT_TYPE::kLight) {}

  /**
  *  @brief Default destructor.
  */
  ~LightComponent() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Light position.
  */
  Vector4 m_position = {0.0f, 0.0f, 0.0f, 1.0f};

  /**
  *  @brief Light target (for directional and spot lights).
  */
  Vector3 m_target = Vector3::ZERO;

  /**
  *  @brief Light color.
  */
  LinearColor m_color = LinearColor::WHITE;

  /**
  *  @brief Light intensity.
  */
  float m_intensity = 1.0f;

  /**
  *  @brief Light type.
  */
  LIGHT_TYPE::E m_lightType = LIGHT_TYPE::kPoint;

  /**
  *  @brief Camera for shadow mapping.
  */
  Camera m_lightCamera;
};
}
