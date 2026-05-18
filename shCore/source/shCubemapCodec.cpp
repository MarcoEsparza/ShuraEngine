/*****************************************************************************/
/*
*  @file    shCubemapCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief   Codec for cube map files. This codec will be used to read and write
*           cube map files.
*
*  Codec for cube map files. This codec will be used to read and write cube map files.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shCubemapCodec.h"
#include "shResourceManager.h"
#include <shLogger.h>
#include <shFileExplorer.h>
#include <shStringID.h>
#include <shException.h>
#include "shCubeMap.h"

using std::getline;
using std::stof;
using std::istringstream;

namespace shEngineSDK {
UID
CubeMapCodec::getUID() const
{
  return StringID("CubeMapCodec").getID();
}

Vector<String>
CubeMapCodec::getSupportedExtensions() const
{
  return { ".cube" };
}

bool
CubeMapCodec::decode(const Path& filePath) const
{
  if (!parseCubeFile(filePath)) {
    SH_LOG_ERROR("Failed to parse cube file: " + filePath.string());
    return false;
  }
  SH_LOG_INFO("Successfully parsed cube file: " + filePath.string());

  return true;
}

bool
CubeMapCodec::encode(const String& objName, const Path& saveFilePath) const
{
  SH_UNREFERENCED_PARAMETER(objName);
  SH_UNREFERENCED_PARAMETER(saveFilePath);
  return false;
}

bool
CubeMapCodec::parseCubeFile(const Path& filePath) const
{
  ResourceManager& resMan = g_resourceMan();

  sh_fstream fileStream(filePath.string(), std::ios::in | std::ios::binary);
  if (!fileStream.is_open()) {
    String errStr = "Failed to open file: " + filePath.string();
    SH_LOG_ERROR(errStr);
    return false;
  }

  String line;
  Vector<LinearColor> textureData;
  uint32 lutSize = 0;

  while (getline(fileStream, line)) {
    if (line.empty() || line[0] == '#') {
      continue; // Skip empty lines and comments
    }

    // Parse the line for texture data
    istringstream iss(line);
    String token;
    iss >> token;

    if (token == "LUT_3D_SIZE") {
      iss >> lutSize;
      if (lutSize <= 0) {
        String errStr = "Invalid LUT size in file: " + filePath.string();
        SH_LOG_ERROR(errStr);
        return false;
      }
    }
    else if (isdigit(token[0]) || token[0] == '.') {
      float r = std::stof(token);
      float g, b;
      iss >> g >> b;
      textureData.emplace_back(r, g, b, 1.0f);
    }
  }

  bool isValid = textureData.size() == size_t(lutSize * lutSize * lutSize);

  if(isValid) {
    SPtr<CubeMap> pCubeMap = sh_makeShared<CubeMap>(lutSize, textureData);
    pCubeMap->setName(filePath.filename());
    pCubeMap->setPath(filePath);
    if (resMan.addResource(pCubeMap)) {
      return true;
    }
    SH_LOG_ERROR("Failed to add CubeMap resource to ResourceManager: " + filePath.filename());
    return false;
  }

  return false;
}
}
