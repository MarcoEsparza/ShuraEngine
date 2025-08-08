/*****************************************************************************/
/*
*  @file    shAudioManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
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
#include <shModule.h>
#include <shPath.h>

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
  virtual void
  update() = 0;

  /**
  *  @brief Create a sound with the given path.
  * 
  *  @param const Path& filePath
  * 
  *  @return SPtr<Sound>
  */
  virtual SPtr<Sound>
  createSound(const Path& filePath) = 0;

  /**
  *  @brief Play sound
  *
  *  @param SPtr<Sound>& pSound
  */
  virtual void
  playSound(const SPtr<Sound>& pSound) = 0;

  /**
  *  @brief Sets volume in a channel group.
  *
  *  @param const CHANNEL_TYPE::E channel
  *  @param const float volume
  */
  virtual void
  setChannelGroupVolume(const CHANNEL_TYPE::E channel, const float volume) = 0;
};
}
