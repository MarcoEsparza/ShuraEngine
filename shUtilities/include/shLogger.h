/*****************************************************************************/
/*
*  @file    shLogger.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Module to do debug logs on console.
*
*  Module to do debug logs on console.
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

namespace shEngineSDK {
enum class LogVerbosity : uint8
{
  kNone = 0,
  kFatal = 1,
  kError = 2,
  kWarning = 3,
  kInfo = 4,
  kDebug = 5,

  kAll = 255,
};

struct LogEntry
{
  LogVerbosity verbosity = LogVerbosity::kInfo;
  String message;
  String timestamp;
  String srcFile;
  uint32 srcLine = 0;
  String srcFunction;

  LogEntry() = default;
  LogEntry(LogVerbosity verbosity,
           const String& message,
           const String& timestamp,
           const String& srcFile,
           uint32 srcLine,
           const String& srcFunction)
    : verbosity(verbosity),
      message(message),
      timestamp(timestamp),
      srcFile(srcFile),
      srcLine(srcLine),
      srcFunction(srcFunction)
  {}
  ~LogEntry() = default;
};

/**
*  @brief Module to do debug logs on console.
*/
class SH_UTILITY_EXPORT Logger : public Module<Logger>
{
 public:
  /**
  *  @brief Default constructor.
  */
  Logger() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Logger() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Log into console.
  * 
  *  @param logStr String to log into console.
  */
  void
  consoleLog(const String& logStr);

  void
  log(LogVerbosity verbosity,
      const String& message,
      const String& timestamp,
      const String& srcFile,
      uint32 srcLine,
      const String& srcFunction);

  const Vector<LogEntry>&
  getLogs() const;

 private:
   Vector<LogEntry> m_logs;
};

#define SH_LOG(verbosity, message) \
  g_logger().log(verbosity, message, __TIME__, __FILE__, __LINE__, __FUNCTION__)

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

/**
*  @brief Easier way to access the Logger module.
*/
SH_UTILITY_EXPORT Logger&
g_logger();
}
