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

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shScriptManager.h"

namespace shEngineSDK {
ScriptManager::ScriptManager()
{
  m_state.open_libraries(
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
  m_state.script(script);
}

SH_UTILITY_EXPORT ScriptManager&
g_scriptMan()
{
  return ScriptManager::instance();
}
}
