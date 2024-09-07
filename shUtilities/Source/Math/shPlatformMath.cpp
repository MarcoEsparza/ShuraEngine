/*************************************************************/
/*
*  @file    shSPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX
*
*
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "Math/shPlatformMath.h"

namespace shEngineSDK {
 /*************************************************************/
 /*
 *  Static variables
 */
 /*************************************************************/
const float PlatformMath::PI = std::acosf(-1.0f);

const float PlatformMath::RAD2DEG = 180.0f / PI;

const float PlatformMath::DEG2RAD = PI / 180.0f;

const float PlatformMath::SMALL_NUMBER = (1.e-6f);

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

float
PlatformMath::sqrt(const float& value)
{
  return std::sqrt(value);
}

float
PlatformMath::sqrtf(const float& value)
{
  return std::sqrtf(value);
}

float
PlatformMath::pow(const float& value, const float vPow)
{
  return std::pow(value, vPow);
}

float
PlatformMath::min(const float& x, const float& y)
{
  return std::min(x, y);
}

float
PlatformMath::max(const float& x, const float& y)
{
  return std::max(x, y);
}
}
