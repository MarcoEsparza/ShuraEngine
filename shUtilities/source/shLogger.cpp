/*****************************************************************************/
/*
*  @file    shLogger.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
*  @brief   Module to do debug logs on console.
*
*  Module to do debug logs on console.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shLogger.h"
#include <iostream>

using std::cout;

namespace shEngineSDK {
void
Logger::Log(const String& logStr)
{
  cout << logStr << "\n";
}

SH_UTILITY_EXPORT Logger&
g_logger()
{
  return Logger::instance();
}
}
