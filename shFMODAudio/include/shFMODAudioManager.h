/*************************************************************/
/*
*  @file    shFMODAudioManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/28
*  @brief   Audio FMOD plugin.
*
*  Audio FMOD plugin.
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
#include "shPrerequisitesFMODAudio.h"
#include "shAudioManager.h"
#include "shModule.h"
#include "shPath.h"

namespace FMOD
{
  class System;
}

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/

class Sound;
class FMODChannel;
class FMODChannelGroup;

/**
*  @brief Audio FMOD plugin.
*/
class FMODAudioManager : public AudioManager
{
 public:
  /**
  *  @brief Default constructor.
  */
  FMODAudioManager() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~FMODAudioManager() = default;

 protected:
  /**
  *  @brief Event called when module is started.
  */
  void
  onStartUp() override;

  /**
  *  @brief Event called when module is shut downed.
  */
  void
  onShutDown() override;

  /**
  *  @brief Internal function to create sound.
  * 
  *  @param Path& filePath
  * 
  *  @return SPtr<Sound>
  */
  SPtr<Sound>
  internalCreateSound(const Path& filePath) override;

  /**
  *  @brief Internal function to update plugin.
  */
  void
  internalUpdate() override;

  /**
  *  @brief Internal function to play sound.
  * 
  *  @param SPtr<Sound>& pSound
  */
  void
  internalPlaySound(const SPtr<Sound>& pSound) override;

  /**
  *  @brief Internal function to set channel group volume.
  * 
  *  @param CHANNEL_TYPE::E channel
  *  @param float volume
  */
  void
  internalSetChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume) override;

 private:
  /**
  *  @brief Fill a given channel group.
  * 
  *  @param SPtr<FMODChannelGroup>& pFMODChG
  */
  void
  fillChannelGroup(SPtr<FMODChannelGroup>& pFMODChG);

  /**
  *  @brief Returns an unused channel in a channel group.
  * 
  *  @param SPtr<FMODChannelGroup>& pFMODChG
  * 
  *  @return SPtr<FMODChannel>
  */
  SPtr<FMODChannel>
  getUnusedChannelOnGroup(const SPtr<FMODChannelGroup>& pFMODChG);

 private:
  /**
  *  @brief FMOD System.
  */
  FMOD::System* m_system = nullptr;

  /**
  *  @brief Map for channel groups.
  */
  UMap<CHANNEL_TYPE::E, SPtr<FMODChannelGroup>> m_chGroups;
};
}
