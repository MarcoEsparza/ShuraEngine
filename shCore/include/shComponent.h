/*************************************************************/
/*
*  @file    shComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Component Base class.
*
*  Component Base class.
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
/**
*  @brief Component type.
*/
namespace COMPONENT_TYPE{
enum SH_CORE_EXPORT E
{
  kTransform = 0,
  kTexture,
  kStaticMesh,
  kSkeletalMesh
};
}

/**
*  @brief Component Base class.
*/
class SH_CORE_EXPORT Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  Component() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Component() = default;

  /**
  *  @brief Component name.
  */
  String name;

  /**
  *  @brief Component type.
  */
  COMPONENT_TYPE::E type;
};
}
