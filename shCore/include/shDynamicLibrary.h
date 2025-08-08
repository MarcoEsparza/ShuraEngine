/*****************************************************************************/
/*
*  @file    shDynamicLibrary.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Dynamic Library object.
*
*  Dynamic Library object.
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

#if SH_PLATFORM == SH_PLATFORM_WIN32

struct HINSTANCE__;
using hInstance = HINSTANCE__*;

# define DYNAMIC_LIBRARY_HANDLE hInstance
# define DYNAMIC_LIBRARY_LOAD(x) LoadLibraryA(x)
# define DYNAMIC_LIBRARY_GET_SYMBOL(x, y) GetProcAddress(x, y)
# define DYNAMIC_LIBRARY_UNLOAD(x) !FreeLibrary(x)

#elif SH_PLATFORM == SH_PLATFORM_LINUX

// TODO: Macros for linux.

#endif

namespace shEngineSDK {
/**
*  @brief Dynamic Library object.
*/
class DynamicLibrary
{
 public:
  /**
  *  @brief Default constructor.
  */
  DynamicLibrary() = default;

  /**
  *  @brief Constructor with dll name.
  * 
  *  @param String& name
  */
  DynamicLibrary(const String& name);

  /**
  *  @brief Default destructor.
  */
  ~DynamicLibrary() = default;


  /**
  *  @brief Load the dll with name.
  */
  void
  load();

  /**
  *  @brief Unload the dll.
  */
  void
  unload();

  /**
  *  @brief Get desired symbol by its name.
  * 
  *  @param String& symbolName
  */
  void*
  getSymbol(const String& symbolName);

 private:
  /**
  *  @brief Dll name.
  */
  String m_name;

  /**
  *  @brief Dynamic library handle, it changes depending the platform.
  */
  DYNAMIC_LIBRARY_HANDLE m_dynLibHandler;

  /**
  *  @brief Static variable for dll extension.
  */
  static const char* EXTENSION;

  /**
  *  @brief Static variable for dll prefix.
  */
  static const char* PREFIX;
};
}
