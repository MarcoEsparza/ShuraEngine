/*****************************************************************************/
/*
*  @file    shMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math wrapper, to use the actual library needed for the system.
*
*  If it needs a different Math library depending on the OS, here it'd
*  be defined.
* 
*  @bug     No bug known
*/
/*****************************************************************************/

#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/

#include "shPrerequisitesUtilities.h"
#include "shPlatformMath.h"

namespace shEngineSDK {
#if SH_PLATFORM == SH_PLATFORM_WIN32 || \
    SH_PLATFORM == SH_PLATFORM_LINUX || \
    SH_PLATFORM == SH_PLATFORM_OSX

/*
*  Math class container
*/
using Math = PlatformMath;

#else
using Math = PlatformMath;
#endif
}
