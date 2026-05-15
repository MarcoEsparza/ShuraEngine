/*****************************************************************************/
/*
*  @file    shLoadMeshCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief
*
*
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
#include "shMeshCodec.h"

using namespace shEngineSDK;

SH_EXTERN SH_PLUGIN_EXPORT void
loadPlugin()
{
  SH_LOG_DEBUG("Loading plugin: shMeshCodec");

  if (!g_codecManager().isStarted()) {
    SH_LOG_ERROR("Trying to load plugin: shMeshCodec, but Codec Manager is not started.");
    return;
  }

  auto& codecMan = g_codecManager();
  codecMan.registerCodec<shEngineSDK::MeshCodec>();

  SH_LOG_DEBUG("Loaded plugin: shMeshCodec");
}
