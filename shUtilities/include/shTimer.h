/*****************************************************************************/
/*
*  @file    shTimer.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief   Class to calculate time.
*
*  Class to calculate time.
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
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using DurationF = std::chrono::duration<float>;

namespace shEngineSDK {
/**
*  @brief Class to calculate time.
*/
class SH_UTILITY_EXPORT Timer
{
 public:
  /**
  *  @brief Constructor that starts the timer.
  */
  Timer();

  /**
  *  @brief Default destructor.
  */
  ~Timer() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Resets the timer.
  */
  void
  reset();

  /**
  *  @brief Returns the delta time in seconds.
  */
  float
  getTime();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Timer wrapper.
  */
  TimePoint m_timePoint;
};
}
