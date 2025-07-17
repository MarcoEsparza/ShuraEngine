/*****************************************************************************/
/*
*  @file    shFMODChannel.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Audio FMOD plugin channel.
*
*  Audio FMOD plugin channel.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shFMODChannel.h"

//Add fmod libraries
#if USING_FMOD
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")
#include <fmod.hpp>
#endif // USING_FMOD

namespace shEngineSDK {
FMODChannel::~FMODChannel()
{
#if USING_FMOD
  m_channel->stop();
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}
}
