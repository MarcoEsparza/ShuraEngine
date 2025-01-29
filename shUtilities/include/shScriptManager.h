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
};

FORCEINLINE ScriptState&
ScriptManager::getState()
{
  return m_state;
}

SH_UTILITY_EXPORT ScriptManager&
g_scriptMan();
}
