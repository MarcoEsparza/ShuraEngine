/*****************************************************************************/
/*
*  @file    shCubeMap.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief   Cube Map class for reading and storing cube map textures.
*
*  Cube Map class for reading and storing cube map textures.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shCubeMap.h"
#include "shGraphicsManager.h"
#include <shLogger.h>

using std::getline;
using std::stof;
using std::istringstream;

namespace shEngineSDK {
void
CubeMap::createTexture(const uint32 lutSize, const Vector<LinearColor>& textureData)
{
  GraphicsManager& graphMan = g_graphicsMan();
  float size = cast::st<float>(lutSize);
  m_pTexture = graphMan.createTexture3D(Vector3(size, size, size),
                                        TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                                        &textureData);
}
}
