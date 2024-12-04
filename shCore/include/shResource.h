/*************************************************************/
/*
*  @file    shResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Resource base class.
*
*  Resource base class.
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
*  @brief Resource type.
*/
namespace RESOURCE_TYPE{
enum SH_CORE_EXPORT E
{
  kTexture = 0,
  kStaticMesh,
  kSkeletalMesh,
  kAnmimation
};
}

/**
*  @brief Resource base class.
*/
class SH_CORE_EXPORT Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  Resource() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Resource() = default;

  /**
  *  @brief Resource name.
  */
  String name;

  /**
  *  @brief Resource type.
  */
  RESOURCE_TYPE::E type;
};
}
