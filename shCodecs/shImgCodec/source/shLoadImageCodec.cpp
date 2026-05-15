/*****************************************************************************/
/*
*  @file    shLoadImageCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   Loads the image codec plugin and registers the codec to the Codec Manager.
*
*  Loads the image codec plugin and registers the codec to the Codec Manager.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include <shPrerequisitesCore.h>
#include <shCodecManager.h>
#include <shLogger.h>
#include "shImgCodec.h"

using namespace shEngineSDK;

SH_EXTERN SH_PLUGIN_EXPORT void
loadPlugin()
{
  SH_LOG_DEBUG("Loading plugin: shImgCodec");

  if (!g_codecManager().isStarted()) {
    SH_LOG_ERROR("Trying to load plugin: shImgCodec, but Codec Manager is not started.");
    return;
  }

  auto& codecMan = g_codecManager();
  if (!codecMan.registerCodec<shEngineSDK::ImgCodec>()) {
    SH_LOG_ERROR("Failed to register shImgCodec to Codec Manager.");
  }

  SH_LOG_DEBUG("Loaded plugin: shImgCodec");
}
