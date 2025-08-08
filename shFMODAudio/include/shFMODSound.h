/*****************************************************************************/
/*
*  @file    shFMODSound.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Sound class implementation for FMOD.
*
*  Sound class implementation for FMOD.
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
#include "shPrerequisitesFMODAudio.h"
#include <shSound.h>

namespace FMOD
{
  class Sound;
}

namespace shEngineSDK {
/**
*  @brief Sound class implementation for FMOD.
*/
class FMODSound : public Sound
{
 public:
  /**
  *  @brief Default constructor.
  */
  FMODSound() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~FMODSound();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief FMOD sound.
  */
  FMOD::Sound* m_sound = nullptr;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE FMODSound::~FMODSound()
{
  if (m_sound != nullptr) {
    //m_sound->release();
    //delete m_sound;
    m_sound = nullptr;
  }
}
}
