/*****************************************************************************/
/*
*  @file    shScriptManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/29
*  @brief
*
*
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
#include "shPrerequisitesUtilities.h"
#include "shModule.h"

#include <sol/sol.hpp>

using ScriptState = sol::state;

namespace shEngineSDK {
class SH_UTILITY_EXPORT ScriptManager : public Module<ScriptManager>
{
 public:
  ScriptManager();
  ~ScriptManager() = default;

  FORCEINLINE ScriptState&
  getState();

  void
  executeScript(const String& script);

 private:
  ScriptState m_state;
  UMap<uint32, ScriptManager> m_stateMap;
};

FORCEINLINE ScriptState&
ScriptManager::getState()
{
  return m_state;
}

/**
*  @brief Easier way to access the ScriptManager module.
*/
SH_UTILITY_EXPORT ScriptManager&
g_scriptMan();
}
