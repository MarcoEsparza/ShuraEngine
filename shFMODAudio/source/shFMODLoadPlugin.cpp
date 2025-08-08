/*****************************************************************************/
/*
*  @file    shFMODLoadPlugin.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Load FMOD plugin.
*
*  Load FMOD plugin.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesFMODAudio.h"
#include <shAudioManager.h>
#include "shFMODAudioManager.h"
#include <shModule.h>

using namespace shEngineSDK;

SH_EXTERN SH_PLUGIN_EXPORT void
loadPlugin()
{
  AudioManager::startUp<FMODAudioManager>();
}
