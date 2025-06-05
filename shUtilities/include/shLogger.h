/*****************************************************************************/
/*
*  @file    shLogger.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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
  *  @param String& logStr
  */
  void
  Log(const String& logStr);
};

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
