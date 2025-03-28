/*****************************************************************************/
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
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shAudioTypes.h"
#include "shModule.h"
#include "shPath.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/

class Sound;

/**
*  @brief Audio engine module.
*/
class SH_CORE_EXPORT AudioManager : public Module<AudioManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  AudioManager() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~AudioManager() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Default constructor.
  */
  void
  update();

  /**
  *  @brief Create a sound with the given path.
  * 
  *  @param Path& filePath
  * 
  *  @return SPtr<Sound>
  */
  SPtr<Sound>
  createSound(const Path& filePath);

  /**
  *  @brief Play sound
  *
  *  @param SPtr<Sound>& pSound
  */
  void
  playSound(const SPtr<Sound>& pSound);

  /**
  *  @brief Sets volume in a channel group.
  *
  *  @param CHANNEL_TYPE::E channel
  *  @param float volume
  */
  void
  setChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume);

  /***************************************************************************/
  /*
  *  Internal functions
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief Plugin internal function.
  *
  *  @param Path& filePath
  *
  *  @return SPtr<Sound>
  */
  virtual SPtr<Sound>
  internalCreateSound(const Path& filePath) = 0;

  /**
  *  @brief Plugin internal function.
  */
  virtual void
  internalUpdate() = 0;

  /**
  *  @brief Plugin internal function.
  *
  *  @param SPtr<Sound>& pSound
  */
  virtual void
  internalPlaySound(const SPtr<Sound>& pSound) = 0;

  /**
  *  @brief Plugin internal function.
  *
  *  @param CHANNEL_TYPE::E channel
  *  @param float volume
  */
  virtual void
  internalSetChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume) = 0;
};
}
