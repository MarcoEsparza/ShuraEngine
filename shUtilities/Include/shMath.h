/*************************************************************/
/*
*  @file    shMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/12
*  @brief   Math wrapper, to use the actual library needed for the system
*
*  If it needs a different Math library depending on the OS, here it'd be defined
* 
*  @bug     No bug known
*/
/*************************************************************/

#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/

#include "shPrerequisitesUtilities.h"
#include "shPlatformMath.h"

namespace shEngineSDK {
#if SH_PLATFORM == SH_PLATFORM_WIN32 || \
    SH_PLATFORM == SH_PLATFORM_LINUX || \
    SH_PLATFORM == SH_PLATFORM_OSX

using Math = PlatformMath;

#else
using Math = PlatformMath;
#endif
}
