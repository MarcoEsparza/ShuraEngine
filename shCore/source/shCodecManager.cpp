/*****************************************************************************/
/*
*  @file    shCodecManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/13
*  @brief   Codec Manager module for registering and retrieving codecs.
*
*  Codec Manager module for registering and retrieving codecs.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shCodecManager.h"
#include <shLogger.h>
#include "shAssetCodec.h"
#include "shResourceCodec.h"
#include "shCubemapCodec.h"

namespace shEngineSDK {
void
CodecManager::onStartUp()
{
  // For external formats, their codecs should be registered as plugins
  // For internal formats, we register their codecs here

  SH_LOG_INFO("Registering internal codecs...");

  if(registerCodec<AssetCodec>()) {
    SH_LOG_INFO("Registered AssetCodec successfully.");
  } else {
    SH_LOG_ERROR("Failed to register AssetCodec.");
  }

  if (registerCodec<ResourceCodec>()) {
    SH_LOG_INFO("Registered ResourceCodec successfully.");
  }
  else {
    SH_LOG_ERROR("Failed to register ResourceCodec.");
  }

  if(registerCodec<CubeMapCodec>()){
    SH_LOG_INFO("Registered CubeMapCodec successfully.");
  }
  else {
    SH_LOG_ERROR("Failed to register CubeMapCodec.");
  }

  SH_LOG_INFO("Finished registering internal codecs.");
}

void
CodecManager::onShutDown()
{
  m_codecs.clear();
}

SPtr<Codec>
CodecManager::getCodecByExtension(const String& extension) const
{
  for (auto& codec : m_codecs) {
    if(codec.second->canDecodeExtension(extension)) {
      return codec.second;
    }
  }

  return nullptr;
}

SH_CORE_EXPORT CodecManager&
g_codecManager()
{
  return CodecManager::instance();
}
}
