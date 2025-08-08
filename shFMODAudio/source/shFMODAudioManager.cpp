/*****************************************************************************/
/*
*  @file    shFMODAudioManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Audio FMOD plugin.
*
*  Audio FMOD plugin.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

//Add fmod libraries
#if USING_FMOD
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")
#endif // USING_FMOD

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shFMODAudioManager.h"
#include "shFMODSound.h"
#include "shFMODChannel.h"
#include "shFMODChannelGroup.h"

#include <shException.h>

#if USING_FMOD
#include <fmod.hpp>
#endif

#define MAX_CHANNELS            512

namespace shEngineSDK {
void
FMODAudioManager::onStartUp()
{
#if USING_FMOD
  FMOD_RESULT result = FMOD::System_Create(&m_system);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system creation");
  }

  result = m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system initialization");
  }

  auto pMasterChG = sh_makeShared<FMODChannelGroup>();
  auto pSFXChG = sh_makeShared<FMODChannelGroup>();
  auto pMusicChG = sh_makeShared<FMODChannelGroup>();
  auto pVoiceChG = sh_makeShared<FMODChannelGroup>();
  auto pUIChG = sh_makeShared<FMODChannelGroup>();

  m_system->createChannelGroup("Master", &pMasterChG->m_channelGroup);
  m_system->createChannelGroup("SFX", &pSFXChG->m_channelGroup);
  m_system->createChannelGroup("Music", &pMusicChG->m_channelGroup);
  m_system->createChannelGroup("Voice", &pVoiceChG->m_channelGroup);
  m_system->createChannelGroup("UI", &pUIChG->m_channelGroup);

  pMasterChG->m_channelGroup->addGroup(pSFXChG->m_channelGroup);
  pMasterChG->m_channelGroup->addGroup(pMusicChG->m_channelGroup);
  pMasterChG->m_channelGroup->addGroup(pVoiceChG->m_channelGroup);
  pMasterChG->m_channelGroup->addGroup(pUIChG->m_channelGroup);

  fillChannelGroup(pSFXChG);
  fillChannelGroup(pMusicChG);
  fillChannelGroup(pVoiceChG);
  fillChannelGroup(pUIChG);

  m_chGroups[CHANNEL_TYPE::kMaster] = pMasterChG;
  m_chGroups[CHANNEL_TYPE::kSFX] = pSFXChG;
  m_chGroups[CHANNEL_TYPE::kMusic] = pMusicChG;
  m_chGroups[CHANNEL_TYPE::kVoice] = pVoiceChG;
  m_chGroups[CHANNEL_TYPE::kUI] = pUIChG;
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif
}

void
FMODAudioManager::onShutDown()
{
#if USING_FMOD
  safeRelease(m_system);
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}

SPtr<Sound>
FMODAudioManager::createSound(const Path& filePath)
{
#if USING_FMOD
  auto pSound = sh_makeShared<FMODSound>();

  FMOD_RESULT res = FMOD_OK;
  res = m_system->createSound(filePath.toString().c_str(),
                              FMOD_DEFAULT,
                              nullptr,
                              &pSound->m_sound);
  if (res != FMOD_OK) {
    SH_ASSERT("Error on sound creation");
  }

  return pSound;
#else
  SH_UNREFERENCED_PARAMETER(filePath);
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
  return nullptr;
}

void
FMODAudioManager::update()
{
#if USING_FMOD
  SH_ASSERT(m_system);
  m_system->update();
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}

void
FMODAudioManager::playSound(const SPtr<Sound>& pSound)
{
#if USING_FMOD
  auto pFMODSound = reinterpret_pointer_cast<FMODSound>(pSound);

  FMOD_RESULT res = FMOD_OK;

  auto& channelGruop = m_chGroups[pFMODSound->m_channel];

  auto pChannel = getUnusedChannelOnGroup(channelGruop);

  res = m_system->playSound(pFMODSound->m_sound,
                            channelGruop->m_channelGroup,
                            false,
                            &pChannel->m_channel);
  if (res != FMOD_OK) {
    SH_ASSERT("Error on sound play");
  }
  pFMODSound->m_bIsPlaying = true;
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}

void
FMODAudioManager::setChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume)
{
#if USING_FMOD
  m_chGroups[channel]->m_channelGroup->setVolume(volume);
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}

void
FMODAudioManager::fillChannelGroup(SPtr<FMODChannelGroup>& pFMODChG)
{
#if USING_FMOD
  pFMODChG->m_channels.resize(CHANNELS_PER_GROUP);

  for (uint8 i = 0; i < CHANNELS_PER_GROUP; ++i) {
    pFMODChG->m_channels[i] = sh_makeShared<FMODChannel>();
    pFMODChG->m_channels[i]->index = i;
  }
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}

SPtr<FMODChannel>
FMODAudioManager::getUnusedChannelOnGroup(const SPtr<FMODChannelGroup>& pFMODChG)
{
#if USING_FMOD
  for (uint8 i = 0; i < pFMODChG->m_channels.size(); ++i) {
    auto pChannel = reinterpret_pointer_cast<FMODChannel>(pFMODChG->m_channels[i]);
    bool isPlaying = false;

    if (pChannel->m_channel) {
      pChannel->m_channel->isPlaying(&isPlaying);

      if (!isPlaying) {
        return pChannel;
      }
    }
    else {
      return pChannel;
    }
  }

#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD

  return nullptr;
}
}
