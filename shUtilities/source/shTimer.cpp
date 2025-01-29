/*****************************************************************************/
/*
*  @file    shTimer.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief   Class to calculate time.
*
*  Class to calculate time.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shTimer.h"

namespace shEngineSDK {
Timer::Timer()
{
  m_timePoint = Clock::now();
}

void
Timer::reset()
{
  m_timePoint = Clock::now();
}

float
Timer::getTime()
{
  TimePoint currentTime = Clock::now();
  DurationF deltaTime = currentTime - m_timePoint;
  m_timePoint = currentTime;

  return deltaTime.count();
}
}
