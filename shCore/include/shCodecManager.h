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
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shModule.h"
#include "shCodec.h"
#include <shLogger.h>

namespace shEngineSDK {
/**
*  @brief Codec Manager module for registering and retrieving codecs.
*/
class SH_CORE_EXPORT CodecManager : public Module<CodecManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  CodecManager() = default;

  /**
  *  @brief Default destructor.
  */
  ~CodecManager() = default;

  /**
  *  @brief Override start event for module.
  */
  void
  onStartUp() override;

  /**
  *  @brief Override shut down event for module.
  */
  void
  onShutDown() override;

  /**
  *  @brief Registers a codec to the manager. Codec will be registered using its UID,
  *         and the provided type must derive from Codec and have a default constructor.
  * 
  *  @tparam CodecType: Type of the codec to register.
  * 
  *  @return bool: True if the codec was registered successfully, false if it wasn't
  *                (e.g., if a codec with the same UID or supported extension already exists).
  */
  template <typename CodecType>
  _NODISCARD bool
  registerCodec()
  {
    static_assert(std::derived_from<CodecType, Codec>, "Provided type must derive from Codec.");
    auto pCodec = sh_makeShared<CodecType>();

    UID codecUID = pCodec->getUID();
    if (m_codecs.find(codecUID) != m_codecs.end()) {
      String errString = "Trying to register a codec with an already existing UID: " +
                         std::to_string(codecUID);
      SH_LOG_ERROR(errString);
      return false;
    }

    Vector<String> supportedExtensions = pCodec->getSupportedExtensions();
    for (auto& ext : supportedExtensions) {
      if(std::find(m_registeredExtensions.begin(), m_registeredExtensions.end(), ext) !=
         m_registeredExtensions.end()) {
        String errString =
        "Trying to register a codec that supports an already registered extension: " + ext;
        SH_LOG_ERROR(errString);
        return false;
      }

      m_registeredExtensions.push_back(ext);
    }

    m_codecs[codecUID] = pCodec;
    SH_LOG(LogVerbosity::kInfo, "Registered codec with UID: " + std::to_string(codecUID));

    return true;
  }

  /**
  *  @brief Retrieves a codec that can decode the given extension.
  * 
  *  @param const String& extension: Extension to find a codec for.
  * 
  *  @return SPtr<Codec>: Shared pointer to the codec that can decode the given extension,
  *                       or nullptr if no codec is found.
  */
  SPtr<Codec>
  getCodecByExtension(const String& extension) const;

 private:
  /**
  *  @brief All registered codecs are stored here, mapped by their UID.
  */
  UMap<UID, SPtr<Codec>> m_codecs;

  Vector<String> m_registeredExtensions;
};

/**
*  @brief Easier way to access the Codec Manager module.
*/
SH_CORE_EXPORT CodecManager&
g_codecManager();
}
