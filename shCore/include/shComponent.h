/*************************************************************/
/*
*  @file    shComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
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
*  @brief Component type enumerator.
*/
namespace COMPONENT_TYPE{
enum SH_CORE_EXPORT E
{
  kTransform = 0,
  kTexture,
  kStaticMesh,
  kSkeletalMesh,
  kAnimator,

  kCount
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
  *  @brief Constructor that initialize the type variable.
  *
  *  @param COMPONENT_TYPE::E cType
  */
  Component(const COMPONENT_TYPE::E cType) : m_type(cType) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Component() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Gets the component type.
  *
  *  @return COMPONENT_TYPE::E
  */
  FORCEINLINE COMPONENT_TYPE::E
  getType() const { return m_type; }

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Component type.
  */
  COMPONENT_TYPE::E m_type;
};
}
