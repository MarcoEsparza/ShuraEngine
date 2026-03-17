/*****************************************************************************/
/*
*  @file    shCubeMap.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/21
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

using std::getline;
using std::stof;
using std::istringstream;

namespace shEngineSDK {
bool
CubeMap::loadFromFile(const String& filePath)
{
  GraphicsManager& graphMan = g_graphicsMan();

  if(!parseCubeFile(filePath)) {
    //SH_LOG_ERROR("CubeMap::loadFromFile: Failed to parse cube file: " + filePath);
    return false;
  }

  const float size = cast::st<float>(m_lutSize);
  m_pTexture = graphMan.createTexture3D(Vector3(size, size, size),
                                        TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                                        &m_textureData);
  return true;
}

bool
CubeMap::parseCubeFile(const String& filePath)
{
  sh_fstream fileStream(filePath, std::ios::in | std::ios::binary);
  if (!fileStream.is_open()) {
    //SH_LOG_ERROR("CubeMap::parseCubeFile: Failed to open file: " + filePath);
    return false;
  }

  String line;
  m_textureData.clear();
  m_lutSize = 0;

  while (getline(fileStream, line)) {
    if(line.empty() || line[0] == '#') {
      continue; // Skip empty lines and comments
    }

    // Parse the line for texture data
    istringstream iss(line);
    String token;
    iss >> token;

    if(token == "LUT_3D_SIZE") {
      iss >> m_lutSize;
      if (m_lutSize <= 0) {
        //SH_LOG_ERROR("CubeMap::parseCubeFile: Invalid LUT size in file: " + filePath);
        return false;
      }
      //m_textureData.resize(m_lutSize * m_lutSize * m_lutSize);
    }
    else if (isdigit(token[0]) || token[0] == '.') {
      float r = std::stof(token);
      float g, b;
      iss >> g >> b;
      m_textureData.emplace_back(r, g, b, 1.0f);
    }
  }

  return m_textureData.size() == size_t(m_lutSize * m_lutSize * m_lutSize);
}
}
