/*************************************************************/
/*
*  @file    shAudioManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Audio engine module.
*
*  Audio engine module.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shAudioManager.h"
#include "shSound.h"

#define MAX_CHANNELS            512

namespace shEngineSDK {
void
AudioManager::update()
{
  internalUpdate();
}

SPtr<Sound>
AudioManager::createSound(const Path& filePath)
{
  return internalCreateSound(filePath);
}

void
AudioManager::playSound(const SPtr<Sound>& pSound)
{
  internalPlaySound(pSound);
}

void
AudioManager::setChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume)
{
  internalSetChannelGroupVolume(channel, volume);
}
}
