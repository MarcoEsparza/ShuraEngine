/*****************************************************************************/
/*
*  @file    shAssetCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shAssetCodec.h"
#include "shResourceManager.h"
#include <shLogger.h>
#include <shFileExplorer.h>
#include <shStringID.h>
#include <shException.h>
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shImageResource.h"

using std::getline;

namespace shEngineSDK {
UID
AssetCodec::getUID() const
{
  return StringID("AssetCodec").getID();
}

Vector<String>
AssetCodec::getSupportedExtensions() const
{
  return { ".sha" };
}

bool
AssetCodec::decode(const Path& filePath) const
{
  SH_UNREFERENCED_PARAMETER(filePath);
  return false;
}

bool
AssetCodec::encode(const String& objName, const Path& saveFilePath) const
{
  SH_UNREFERENCED_PARAMETER(objName);
  SH_UNREFERENCED_PARAMETER(saveFilePath);
  return false;
}
}
