/*****************************************************************************/
/*
*  @file    shFMODAudioManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/28
*  @brief   Audio FMOD plugin.
*
*  Audio FMOD plugin.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shFMODAudioManager.h"
#include "shFMODSound.h"
#include "shFMODChannel.h"
#include "shFMODChannelGroup.h"

#define MAX_CHANNELS            512

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
FMODAudioManager::onStartUp()
{
  FMOD_RESULT result = FMOD::System_Create(&m_system);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system creation");
  }

  result = m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system initialization");
  }

  auto pMasterChG = make_shared<FMODChannelGroup>();
  auto pSFXChG = make_shared<FMODChannelGroup>();
  auto pMusicChG = make_shared<FMODChannelGroup>();
  auto pVoiceChG = make_shared<FMODChannelGroup>();
  auto pUIChG = make_shared<FMODChannelGroup>();

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
}
void
FMODAudioManager::onShutDown()
{
  safeRelease(m_system);
}

SPtr<Sound>
FMODAudioManager::internalCreateSound(const Path& filePath)
{
  auto pSound = make_shared<FMODSound>();

  FMOD_RESULT res = FMOD_OK;
  res = m_system->createSound(filePath.toString().c_str(),
                              FMOD_DEFAULT,
                              nullptr,
                              &pSound->m_sound);
  if (res != FMOD_OK) {
    SH_ASSERT("Error on sound creation");
  }

  return pSound;
}

void
FMODAudioManager::internalUpdate()
{
  SH_ASSERT(m_system);
  m_system->update();
}

void
FMODAudioManager::internalPlaySound(const SPtr<Sound>& pSound)
{
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
}

void
FMODAudioManager::internalSetChannelGroupVolume(const CHANNEL_TYPE::E channel,
                                                const float volume)
{
  m_chGroups[channel]->m_channelGroup->setVolume(volume);
}

void
FMODAudioManager::fillChannelGroup(SPtr<FMODChannelGroup>& pFMODChG)
{
  pFMODChG->m_channels.resize(CHANNELS_PER_GROUP);

  for (uint8 i = 0; i < CHANNELS_PER_GROUP; ++i) {
    pFMODChG->m_channels[i] = make_shared<FMODChannel>();
    pFMODChG->m_channels[i]->index = i;
  }
}

SPtr<FMODChannel>
FMODAudioManager::getUnusedChannelOnGroup(const SPtr<FMODChannelGroup>& pFMODChG)
{
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

  return nullptr;
}
}
