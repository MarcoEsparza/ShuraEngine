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
/**
*  @brief Module that handles all scripts.
*/
class SH_UTILITY_EXPORT ScriptManager : public Module<ScriptManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  ScriptManager();

  /**
  *  @brief Default destructor.
  */
  ~ScriptManager() = default;

  /**
  *  @brief Returns the wanted state.
  * 
  *  @param uint32 id
  */
  FORCEINLINE ScriptState*
  getState(const uint32 id);

  /**
  *  @brief Executes the given script.
  * 
  *  @param String& script
  */
  void
  executeScript(const String& script);

 private:
  /**
  *  @brief All the states are storaged here.
  */
  UMap<uint32, ScriptState> m_states;
};

FORCEINLINE ScriptState*
ScriptManager::getState(const uint32 id)
{
  if (!(m_states.find(id) == m_states.end())) {
    return &m_states[id];
  }

  return nullptr;
}

/**
*  @brief Easier way to access the ScriptManager module.
*/
SH_UTILITY_EXPORT ScriptManager&
g_scriptMan();
}
