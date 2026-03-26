/*****************************************************************************/
/*
*  @file    shDynamicLibrary.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/12
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
using LoadPluginFunc = void(*)(void);

# define DYNAMIC_LIBRARY_HANDLE hInstance
# define DYNAMIC_LIBRARY_LOAD(x) LoadLibraryA(x)
# define DYNAMIC_LIBRARY_GET_SYMBOL(x, y) GetProcAddress(x, y)
# define DYNAMIC_LIBRARY_UNLOAD(x) !FreeLibrary(x)
# define DYNAMIC_LIBRARY_ERROR() GetLastError() 

#elif SH_PLATFORM == SH_PLATFORM_LINUX

using hInstance = void*;
using LoadPluginFunc = void(*)(void);

# define DYNAMIC_LIBRARY_HANDLE hInstance
# define DYNAMIC_LIBRARY_LOAD(x) dlopen(x, RTLD_NOW)
# define DYNAMIC_LIBRARY_GET_SYMBOL(x, y) dlsym(x, y)
# define DYNAMIC_LIBRARY_UNLOAD(x) !dlclose(x)
# define DYNAMIC_LIBRARY_ERROR() dlerror()

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

  DynamicLibrary(DynamicLibrary& other);

  /**
  *  @brief Default destructor.
  */
  ~DynamicLibrary();


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

 public:
  DynamicLibrary& operator=(DynamicLibrary& other)
  {
    if (this != &other) {
      m_name = other.m_name;
      m_dynLibHandler = other.m_dynLibHandler;
      other.m_dynLibHandler = nullptr;
    }
    return *this;
  }

 private:
  /**
  *  @brief Dll name.
  */
  String m_name;

  /**
  *  @brief Dynamic library handle, it changes depending the platform.
  */
  DYNAMIC_LIBRARY_HANDLE m_dynLibHandler;

 public:
  /**
  *  @brief Static variable for dll extension.
  */
  static const String EXTENSION;

  /**
  *  @brief Static variable for dll prefix.
  */
  static const String PREFIX;
};
}
