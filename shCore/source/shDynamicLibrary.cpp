/*************************************************************/
/*
*  @file    shDynamicLibrary.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Dynamic Library object.
*
*  Dynamic Library object.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shDynamicLibrary.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shEngineSDK {
#if SH_PLATFORM == SH_PLATFORM_WIN32

  const char* DynamicLibrary::EXTENSION = "dll";
  const char* DynamicLibrary::PREFIX = nullptr;

#endif

DynamicLibrary::DynamicLibrary(const String& name)
{
  m_name = name + "." + EXTENSION;
  m_dynLibHandler = nullptr;
  load();
}

void
DynamicLibrary::load()
{
  m_dynLibHandler = static_cast<DYNAMIC_LIBRARY_HANDLE>(DYNAMIC_LIBRARY_LOAD(m_name.c_str()));

  if (!m_dynLibHandler) {
    SH_ASSERT("Couldnt load dll");
  }
}

void
DynamicLibrary::unload()
{
  if (DYNAMIC_LIBRARY_UNLOAD(m_dynLibHandler)) {
    // TODO : Finish this function.
  }
}

void*
DynamicLibrary::getSymbol(const String& symbolName)
{
  return static_cast<void*>(DYNAMIC_LIBRARY_GET_SYMBOL(m_dynLibHandler, symbolName.c_str()));
}
}
