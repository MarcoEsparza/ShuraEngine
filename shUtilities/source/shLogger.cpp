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

#define MAX_LOG_BUFFER_SIZE 255

using std::cout;

namespace shEngineSDK {
void
Logger::consoleLog(const String& logStr)
{
  cout << logStr << "\n";
}

void
Logger::log(LogVerbosity verbosity,
            const String& message,
            const String& timestamp,
            const String& srcFile,
            uint32 srcLine,
            const String& srcFunction)
{
  if (m_logs.size() >= MAX_LOG_BUFFER_SIZE) {
    m_logs.erase(m_logs.begin());
  }

  LogEntry entry(verbosity, message, timestamp, srcFile, srcLine, srcFunction);
  m_logs.push_back(entry);
}

const
Vector<LogEntry>& Logger::getLogs() const
{
  return m_logs;
}

SH_UTILITY_EXPORT Logger&
g_logger()
{
  return Logger::instance();
}
}
