/*****************************************************************************/
/*
*  @file    shAudioTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Basic types for audio engine.
*
*  Basic types for audio engine.
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

#define CHANNELS_PER_GROUP           10

namespace shEngineSDK {
/**
*  @brief Enum for channel selection.
*/
namespace CHANNEL_TYPE {
enum E
{
  kMaster = 0,
  kSFX,
  kMusic,
  kVoice,
  kUI
};
}
}
