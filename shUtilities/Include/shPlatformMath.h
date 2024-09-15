/*************************************************************/
/*
*  @file    shSPlatformMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/14
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX.
*
*  Math class wrapper, suing the STD. compatible with Windows, Linux and OSX.
* 
* @bug      No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
/*
*  Math class wrapper, using the STD. compatible with Windows, Linux and OSX.
*/
class SH_UTILITY_EXPORT PlatformMath
{
public:
  /*
  *  Default constructor
  */
  PlatformMath() = default;

  /*
  *  Default destructor
  */
  ~PlatformMath() = default;

  /*************************************************************/
  /*
  *  Algebra
  */
  /*************************************************************/

  /*
  *  Calculate the square root of a given number.
  * 
  *  @param float _value: Value to calculate.
  * 
  *  @return float The result of the square root of value.
  */
  static float
  sqrt(const float& _value);

  /*
  *  Calculate the square root of a given number.
  *
  *  @param float _value: Value to calculate.
  *
  *  @return float The result of the square root of value.
  */
  static float
  sqrtf(const float& _value);

  /*
  *  Calculate the pow of a given number.
  *
  *  @param float _value: Value to calculate.
  * 
  *  @param float _vPow: Number of times to be multiplied.
  *
  *  @return float The calculated pow.
  */
  static float
  pow(const float& _value, const float _vPow);

  /*
  *  Calculate the linear interpolation between two given values
  *
  *  @param float _A: First value.
  * 
  *  @param float _B: Second value.
  * 
  *  @param float _alpha: Alpha to calculate with previous numbers
  *
  *  @return T The result of the linear interpolation.
  */
  FORCEINLINE static float
  lerp(const float& _A, const float& _B, const float& _alpha);

  /*
  *  Calculate the minimum between two given numbers.
  *
  *  @param float _x: First value to compare.
  * 
  *  @param float _y: Second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  min(const float& x, const float& y);

  /*
  *  Calculate the maximum between two given numbers.
  *
  *  @param float _x: First value to compare.
  * 
  *  @param float _y: Second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  max(const float& _x, const float& _y);

  /*
  *  Clamp a given number with given min and max values
  *
  *  @param float _x: Value to calculate.
  * 
  *  @param float _min: Min value.
  * 
  *  @param float _max: Max value.
  *
  *  @return T The result of the clamp.
  */
  FORCEINLINE static float
  clamp(const float& _x, const float& _min, const float& _max);

  /*
  *  Calculate the floating point reminder of the division of the given
  *  values
  *
  *  @param float _value1: Dividend.
  * 
  *  @param float _value2: Divisor.
  *
  *  @return float The reminder of the division
  */
  static float
  fmod(const float& _value1, const float& _value2);

  /*
  *  Calculate the absolute of a given number
  *
  *  @param float _value
  *
  *  @return float
  */
  static float
  abs(const float& _value);
  
  /*************************************************************/
  /*
  *  Trigonometric functions
  */
  /*************************************************************/

  /*
  *  Cosine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Cosine from value.
  */
  static float
  cos(const float& _radian);

  /*
  *  Sine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Sine from value.
  */
  static float
  sin(const float& _radian);

  /*
  *  Tangent.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Tangent from value.
  */
  static float
  tan(const float& _radian);

  /*
  *  Hyperbolic cosine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic cosine from value.
  */
  static float
  cosh(const float& _radian);

  /*
  *  Hyperbolic sine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic sine from value.
  */
  static float
  sinh(const float& _radian);

  /*
  *  Hyperbolic tangent.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic tangent from value.
  */
  static float
  tanh(const float& _radian);

  /*
  *  Arch cosine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Arch cosine from value.
  */
  static float
  acos(const float& _radian);

  /*
  *  Arch sine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Arch sine from value.
  */
  static float
  asin(const float& _radian);

  /*
  *  Arch Tangent.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Arch Tangent from value.
  */
  static float
  atan(const float& _radian);

  /*
  *  Hyperbolic arch cosine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic arch cosine from value.
  */
  static float
  acosh(const float& _radian);

  /*
  *  Hyperbolic arch sine.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic arch sine from value.
  */
  static float
  asinh(const float& _radian);

  /*
  *  Hyperbolic arch tangent.
  *
  *  @param float _radian: Value must be in radians.
  *
  *  @return float Hyperbolic arch tangent from value.
  */
  static float
  atanh(const float& _radian);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
public:
  /*
  *  PI value.
  */
  static const float PI;

  /*
  *  Use to convert a Radian value to a Degree angle.
  */
  static const float RAD2DEG;

  /*
  *  Use to convert a Degree angle to a Radian value.
  */
  static const float DEG2RAD;

  /*
  *  Small number.
  */
  static const float SMALL_NUMBER;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE float
PlatformMath::lerp(const float& A, const float& B, const float& alpha)
{
  return (A + (B - A) * alpha);
}

FORCEINLINE float
PlatformMath::clamp(const float& X, const float& min, const float& max)
{
  return X < min ? min : X < max ? X : max;
}
}
