/*************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shGraphicsManager.h"

namespace shEngineSDK {
void
GraphicsManager::init(PlatformScreen srcHandle,
                      bool bFullScreen,
                      bool bAntiliasing,
                      uint32 samplesPerPixel,
                      uint32 sampleQuality)
{
  internalInit(srcHandle,
               bFullScreen,
               bAntiliasing,
               samplesPerPixel,
               sampleQuality);
}
}
