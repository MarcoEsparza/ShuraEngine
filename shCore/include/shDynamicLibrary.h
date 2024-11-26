/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

struct HINSTANCE__;
using hInstance = HINSTANCE__*;

# define DYNAMIC_LIBRARY_HANDLE hInstance
# define DYNAMIC_LIBRARY_LOAD(x) LoadLibraryA(x)
# define DYNAMIC_LIBRARY_GET_SYMBOL(x, y) GetProcAddress(x, y)
# define DYNAMIC_LIBRARY_UNLOAD(x) !FreeLibrary(x)

#endif

namespace shEngineSDK {
class DynamicLibrary
{
 public:
  DynamicLibrary() = default;
  DynamicLibrary(const String& name);
  ~DynamicLibrary() = default;

  void
  load();

  void
  unload();

  void*
  getSymbol(const String& strName);

 private:

  String m_name;
  DYNAMIC_LIBRARY_HANDLE m_dynLibHandler;

  static const char* EXTENSION;
  static const char* PREFIX;
};
}
