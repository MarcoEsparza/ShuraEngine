/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/
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
namespace RESOURCE_TYPE{
enum SH_CORE_EXPORT E
{
  kTexture = 0,
  kModel,
  kAnmimation
};
}

class SH_CORE_EXPORT Resource
{
 public:
  Resource() = default;
  virtual ~Resource() = default;

  String name;
};
}
