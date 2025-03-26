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
#include <fmod.hpp>

namespace shEngineSDK {
class Sound
{
 public:
  Sound() = default;
  ~Sound();

 public:
  FMOD::Sound* m_sound = nullptr;
};
}
