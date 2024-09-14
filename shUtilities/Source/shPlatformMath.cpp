/*************************************************************/
/*
*  @file    shSPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX.
*
*  Math class wrapper, suing the STD. compatible with Windows, Linux and OSX.
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlatformMath.h"

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

float
PlatformMath::fmod(const float& _value1, const float& _value2)
{
  return std::fmod(_value1, _value2);
}

float
PlatformMath::cos(const float& _radian)
{
  return std::cos(_radian);
}

float
PlatformMath::sin(const float& _radian)
{
  return std::sin(_radian);
}

float
PlatformMath::tan(const float& _radian)
{
  return std::tan(_radian);
}

float
PlatformMath::cosh(const float& _radian)
{
  return std::cosh(_radian);
}

float
PlatformMath::sinh(const float& _radian)
{
  return std::sinh(_radian);
}

float
PlatformMath::tanh(const float& _radian)
{
  return std::tanh(_radian);
}

float
PlatformMath::acos(const float& _radian)
{
  return std::acos(_radian);
}

float
PlatformMath::asin(const float& _radian)
{
  return std::asin(_radian);
}

float
PlatformMath::atan(const float& _radian)
{
  return std::atan(_radian);
}

float
PlatformMath::acosh(const float& _radian)
{
  return std::acosh(_radian);
}

float
PlatformMath::asinh(const float& _radian)
{
  return std::asinh(_radian);
}

float
PlatformMath::atanh(const float& _radian)
{
  return std::atanh(_radian);
}
}
