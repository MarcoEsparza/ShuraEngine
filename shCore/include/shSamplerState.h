/*************************************************************/
/*
*  @file    shSamplerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   SamplerState class wrapper.
*
*  SamplerState class wrapper.
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

namespace shEngineSDK {

#if SH_PLATFORM == SH_PLATFORM_WIN32
class DX11SamplerState;

/**
*  @brief SamplerState wrapper.
*/
using SamplerState = DX11SamplerState;
#endif

}
