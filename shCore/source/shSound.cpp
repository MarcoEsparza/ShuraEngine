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
#include "shSound.h"

namespace shEngineSDK {
Sound::~Sound()
{
  m_sound->release();
  //m_sound = nullptr;
}
}
