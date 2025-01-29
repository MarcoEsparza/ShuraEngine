/*****************************************************************************/
/*
*  @file    shTime.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief   Module to calculate app time.
*
*  Module to calculate app time.
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
class Timer;

/**
*  @brief Module to calculate app time.
*/
class SH_UTILITY_EXPORT Time : public Module<Time>
{
 public:
  /**
  *  @brief Constructor to initialize the timer pointer.
  */
  Time();

  /**
  *  @brief Destructor to destroy the timer pointer.
  */
  ~Time();

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Returns the time since app started in seconds.
  * 
  *  @return float
  */
  FORCEINLINE float
  getTime() const;

  /**
  *  @brief Returns the delta time in seconds.
  * 
  *  @return float
  */
  FORCEINLINE float
  getFrameDeltaTime() const;

  /**
  *  @brief Update the time module.
  */
  void
  update();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Frame delta time in seconds.
  */
  float m_frameDelta = 0.0f;

  /**
  *  @brief Time since app start in seconds.
  */
  float m_timeSinceStart = 0.0f;

  /**
  *  @brief Module timer.
  */
  Timer* m_timer;

  /***************************************************************************/
  /*
  *  Static variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Constant time for fixed update.
  *  @brief 20ms = 50 times in a second
  */
  static const float FIXED_DELTA_TIME;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE float
Time::getTime() const
{
  return m_timeSinceStart;
}

FORCEINLINE float
Time::getFrameDeltaTime() const
{
  return m_frameDelta;
}

/**
*  @brief Easier way to access the Time module.
*/
SH_UTILITY_EXPORT Time&
g_Time()
{
  return Time::instance();
}
}
