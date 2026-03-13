/*****************************************************************************/
/*
*  @file    shDynamicLibraryManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/12
*  @brief   Dynamic Library Manager
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
#include "shDynamicLibraryManager.h"
#include "shStringID.h"

using std::move;

namespace shEngineSDK {
void
DynamicLibraryManager::loadDynLibrary(const String& name)
{
  uint32 nameID = StringID(name).getID();
  m_dynamicLibraries.emplace(nameID, sh_makeUnique<DynamicLibrary>(name));
  LoadPluginFunc dllSymbol =
    cast::re<void(*)()>(m_dynamicLibraries[nameID]->getSymbol("loadPlugin"));
  if (!dllSymbol) {
    SH_ASSERT(dllSymbol && "Could not load function");
  }
  dllSymbol();
}

void
DynamicLibraryManager::onStartUp()
{
}

void
DynamicLibraryManager::onShutDown()
{
  /*for (auto& dynLib : m_dynamicLibraries) {
    dynLib.second.unload();
  }*/
  /*for (auto& dynLib : m_dynamicLibraries) {
    dynLib->unload();
  }*/
  m_dynamicLibraries.clear();
}

DynamicLibraryManager& g_dynLibMan()
{
  return DynamicLibraryManager::instance();
}
}