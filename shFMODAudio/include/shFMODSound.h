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
#include "shSound.h"

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
  FORCEINLINE virtual ~FMODSound();

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
  safeRelease(m_sound);
}
}
