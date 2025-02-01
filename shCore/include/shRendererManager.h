/*****************************************************************************/
/*
*  @file    shRendererManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/31
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
#include "shModule.h"

namespace shEngineSDK {
class SH_CORE_EXPORT RendererManager : Module<RendererManager>
{
 public:
  RendererManager() = default;
  ~RendererManager() = default;
};
}
