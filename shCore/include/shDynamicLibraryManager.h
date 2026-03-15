/*****************************************************************************/
/*
*  @file    shDynamicLibraryManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/12
*  @brief   Dynamic Library Manager
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
#include "shPrerequisitesCore.h"
#include "shModule.h"
#include "shDynamicLibrary.h"

namespace shEngineSDK {
class SH_CORE_EXPORT DynamicLibraryManager : public Module<DynamicLibraryManager>
{
 public:
  /**
   *  @brief Default constructor.
   */
  DynamicLibraryManager() = default;

  /**
   *  @brief Default destructor.
   */
  ~DynamicLibraryManager() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
   *  @brief Load a dynamic library by its name and execute its loadPlugin function.
   * 
   *  @param String& name
   */
  void
  loadDynLibrary(const String& name);

  bool
  isDynLibLoaded(const String& name) const;

  void
  unloadDynLibrary(const String& name);

 private:
  /**
   *  @brief Start up the module, initialize any resources it needs and prepare it for use.
   */
  void
  onStartUp() override;

  /**
   *  @brief Shut down the module and free any resources it is using.
   */
  void
  onShutDown() override;

 private:
  /**
   *  @brief Map of loaded dynamic libraries, the key is the name of the library.
   */
  UMap<uint32, UPtr<DynamicLibrary>> m_dynamicLibraries;
};

/**
 *  @brief Easier way to access the Dynamic Library Manager module.
 */
SH_CORE_EXPORT DynamicLibraryManager&
g_dynLibMan();
}
