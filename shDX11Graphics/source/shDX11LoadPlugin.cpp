/*************************************************************/
/*
*  @file    shDX11LoadPlugin.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Load DirectX 11 plugin.
*
*  Load DirectX 11 plugin.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesDX11Graphics.h"
#include "shDX11GraphicsManager.h"
#include "shGraphicsManager.h"
#include "shModule.h"

using namespace shEngineSDK;

SH_EXTERN SH_PLUGIN_EXPORT void
loadPlugin() {
  GraphicsManager::startUp<DX11GraphicsManager>();
}

