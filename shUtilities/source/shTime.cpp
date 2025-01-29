/*****************************************************************************/
/*
*  @file    shTime.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief   Module to calculate app time.
*
*  Module to calculate time.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shTime.h"
#include "shTimer.h"

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static variables
*/
/*****************************************************************************/

const float Time::FIXED_DELTA_TIME = 0.02f;

Time::Time()
{
  m_timer = new Timer();
}

Time::~Time()
{
  if (m_timer != nullptr) {
    delete m_timer;
    m_timer = nullptr;
  }
}

void
Time::update()
{
  m_frameDelta = m_timer->getTime();
  m_timeSinceStart += m_frameDelta;
}

Time& g_time()
{
  return Time::instance();
}
}
