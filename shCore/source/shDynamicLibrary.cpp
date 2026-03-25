/*****************************************************************************/
/*
*  @file    shDynamicLibrary.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/12
*  @brief   Dynamic Library object.
*
*  Dynamic Library object.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDynamicLibrary.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif SH_PLATFORM == SH_PLATFORM_LINUX
#include <dlfcn.h>
#endif

namespace shEngineSDK {
#if SH_PLATFORM == SH_PLATFORM_WIN32

const String DynamicLibrary::EXTENSION = ".dll";
const String DynamicLibrary::PREFIX = "";

#elif SH_PLATFORM == SH_PLATFORM_LINUX

const String DynamicLibrary::EXTENSION = ".so";
const String DynamicLibrary::PREFIX = "lib";

#endif

DynamicLibrary::DynamicLibrary(const String& name)
{
  m_name = name + EXTENSION;
  m_dynLibHandler = nullptr;
  load();
}

DynamicLibrary::DynamicLibrary(DynamicLibrary& other)
{
  m_name = other.m_name;
  m_dynLibHandler = other.m_dynLibHandler;
  other.m_dynLibHandler = nullptr;
}

DynamicLibrary::~DynamicLibrary()
{
  unload();
}

void
DynamicLibrary::load()
{
  m_dynLibHandler = cast::st<DYNAMIC_LIBRARY_HANDLE>(DYNAMIC_LIBRARY_LOAD(m_name.c_str()));

  if (!m_dynLibHandler) {
#if SH_PLATFORM == SH_PLATFORM_WIN32
    auto error = GetLastError();
    SH_ASSERT(false && error);
#elif SH_PLATFORM == SH_PLATFORM_LINUX
    auto error = dlerror();
    SH_ASSERT(false && error);
#endif
  }
}

void
DynamicLibrary::unload()
{
  if (DYNAMIC_LIBRARY_UNLOAD(m_dynLibHandler)) {
    SH_ASSERT("Couldnt unload dll");
  }
}

void*  
DynamicLibrary::getSymbol(const String& symbolName)  
{
  auto symbol = DYNAMIC_LIBRARY_GET_SYMBOL(m_dynLibHandler, symbolName.c_str());
  return reinterpret_cast<void*>(symbol);
}
}
