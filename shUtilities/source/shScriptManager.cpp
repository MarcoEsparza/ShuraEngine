/*****************************************************************************/
/*
*  @file    shScriptManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/17
*  @brief   Module that handles all scripts.
*
*  Module that handles all scripts.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shScriptManager.h"

namespace shEngineSDK {
ScriptManager::ScriptManager()
{
  m_states[0] = ScriptState();
  auto& state = m_states[0];

  state.open_libraries(
    sol::lib::base,
    sol::lib::package,
    sol::lib::string,
    sol::lib::math,
    sol::lib::table,
    sol::lib::os
  );
}

void
ScriptManager::executeScript(const String& script)
{
  auto pState = getState(0);
  if (pState == nullptr) {
    return;
  }

  pState->script(script);
}

SH_UTILITY_EXPORT ScriptManager&
g_scriptMan()
{
  return ScriptManager::instance();
}
}
