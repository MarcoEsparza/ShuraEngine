/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/26
*  @brief   
*
*  
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
#include "shPrerequisitesCore.h"
#include "shModule.h"
#include "shPath.h"

#include <fmod.hpp>

namespace shEngineSDK {
class Sound;

class AudioManager : public Module<AudioManager>
{
 public:
  AudioManager() = default;
  virtual ~AudioManager() = default;

  void
  initSystem();

  void
  update();

  void
  close();

  SPtr<Sound>
  createSound(Path& filePath);

  void
  playSound(const SPtr<Sound>& pSound);

 private:
  FMOD::System* m_system = nullptr;
  FMOD::Channel* m_channel = nullptr;
};
}
