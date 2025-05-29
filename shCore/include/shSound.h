/*****************************************************************************/
/*
*  @file    shSound.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Engine sound base class.
*
*  Engine sound base class.
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

namespace shEngineSDK {
/**
*  @breif Engine sound base class.
*/
class Sound
{
 public:
  /**
  *  @breif Default constructor.
  */
  Sound() = default;

  /**
  *  @breif Default destructor.
  */
  virtual ~Sound() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @breif Is sound playing.
  */
  bool m_bIsPlaying = false;

  /**
  *  @breif Is sound on loop.
  */
  bool m_bLoop = false;

  /**
  *  @breif Is sound on loop.
  */
  CHANNEL_TYPE::E m_channel = CHANNEL_TYPE::kMaster;
};
}
