/*****************************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/18
*  @brief   Graphics Manager module that uses function from loaded API.
*
*  Graphics Manager module that uses function from loaded API.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shGraphicsManager.h"

namespace shEngineSDK {
GraphicsManager& g_graphicsMan()
{
  return GraphicsManager::instance();
}
}
