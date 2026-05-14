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

namespace shEngineSDK {
void
CodecManager::onStartUp()
{
  // Nothing to do for now, codecs will be registered by
  // their respective plugins when they are loaded.
}

void
CodecManager::onShutDown()
{
  m_codecs.clear();
}

//void
//CodecManager::registerCodec(const SPtr<Codec>& pCodec)
//{
//  UID codecUID = pCodec->getUID();
//  if (m_codecs.find(codecUID) != m_codecs.end()) {
//    String errString = "Trying to register a codec with an already existing UID: " +
//                       std::to_string(codecUID);
//    SH_LOG_ERROR(errString);
//    return;
//  }
//
//  m_codecs[codecUID] = pCodec;
//  SH_LOG(LogVerbosity::kInfo, "Registered codec with UID: " + std::to_string(codecUID));
//}

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
