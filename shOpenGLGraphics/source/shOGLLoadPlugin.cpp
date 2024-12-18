/*************************************************************/
/*
*  @file    shOGLLoadPlugin.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/27
*  @brief   Load Open GL plugin.
*
*  Load Open GL plugin.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesOGLGraphics.h"
#include "shOGLGraphicsManager.h"
#include "shGraphicsManager.h"
#include "shModule.h"

using namespace shEngineSDK;

SH_EXTERN SH_PLUGIN_EXPORT void
loadPlugin() {
  GraphicsManager::startUp<OGLGraphicsManager>();
}
