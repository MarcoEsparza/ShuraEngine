/*************************************************************/
/*
*  @file    shSPlatformMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/12
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX
*
*  
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
  *  @param Value to calculate.
  * 
  *  @return float The result of the square root of value.
  */
  static float
  sqrt(const float& value);

  /*
  *  Calculate the square root of a given number.
  *
  *  @param Value to calculate.
  *
  *  @return float The result of the square root of value.
  */
  static float
  sqrtf(const float& value);

  /*
  *  Calculate the pow of a given number.
  *
  *  @param Value to calculate, number of times to be multiplied.
  *
  *  @return float The calculated pow.
  */
  static float
  pow(const float& value, const float vPow);

  /*
  *  Calculate the linear interpolation between two given values
  *
  *  @param First value, second value and alpha. First and second value must
  *         be the same type.
  *
  *  @return T The result of the linear interpolation.
  */
  FORCEINLINE static float
  lerp(const float& A, const float& B, const float& alpha);

  /*
  *  Calculate the minimum between two given numbers.
  *
  *  @param First value to compare, second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  min(const float& x, const float& y);

  /*
  *  Calculate the maximum between two given numbers.
  *
  *  @param First value to compare, second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  max(const float& x, const float& y);

  /*
  *  Clamp a given number with given min and max values
  *
  *  @param Value to calculate, min value and max value. All parameter must be
  *         the same type.
  *
  *  @return T The result of the clamp.
  */
  FORCEINLINE static float
  clamp(const float& x, const float& min, const float& max);

  /*
  *  Calculate the floating point reminder of the division of the given
  *  values
  *
  *  @param float Point value.
  * 
  *  @param float Point value.
  *
  *  @return float
  *          The reminder of the division
  */
  static float
  fmod(float _value1, float _value2);
  
  /*************************************************************/
  /*
  *  Trigonometric functions
  */
  /*************************************************************/

  /*
  *  Cosine result from Radian.
  *
  *  @param Radian Value to calculate cosine.
  *
  *  @return Radian Cosine from value.
  */
  static float
  cos(const float& _radian);

  /*
  *  Sine result from Radian.
  *
  *  @param Radian Value to calculate sine.
  *
  *  @return Radian Sine from value.
  */
  static float
  sin(const float& _radian);

  /*
  *  Tangent result from Radian.
  *
  *  @param Radian Value to calculate tangent.
  *
  *  @return Radian Tangent from value.
  */
  static float
  tan(const float& _radian);

  /*
  *  Hyperbolic cosine result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic cosine.
  *
  *  @return Radian Hyperbolic cosine from value.
  */
  static float
  cosh(const float& _radian);

  /*
  *  Hyperbolic sine result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic sine.
  *
  *  @return Radian Hyperbolic sine from value.
  */
  static float
  sinh(const float& _radian);

  /*
  *  Hyperbolic tangent result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic tangent.
  *
  *  @return Radian Hyperbolic tangent from value.
  */
  static float
  tanh(const float& _radian);

  /*
  *  Arch cosine result from Radian.
  *
  *  @param Radian Value to calculate arch cosine.
  *
  *  @return Radian Arch cosine from value.
  */
  static float
  acos(const float& _radian);

  /*
  *  Arch sine result from Radian.
  *
  *  @param Radian Value to calculate arch sine.
  *
  *  @return Radian Arch sine from value.
  */
  static float
  asin(const float& _radian);

  /*
  *  ArchTangent result from Radian.
  *
  *  @param Radian Value to calculate Arch tangent.
  *
  *  @return Radian Arch Tangent from value.
  */
  static float
  atan(const float& _radian);

  /*
  *  Hyperbolic arch cosine result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic arch cosine.
  *
  *  @return Radian Hyperbolic arch cosine from value.
  */
  static float
  acosh(const float& _radian);

  /*
  *  Hyperbolic arch sine result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic arch sine.
  *
  *  @return Radian Hyperbolic arch sine from value.
  */
  static float
  asinh(const float& _radian);

  /*
  *  Hyperbolic arch tangent result from Radian.
  *
  *  @param Radian Value to calculate hyperbolic arch tangent.
  *
  *  @return Radian Hyperbolic arch tangent from value.
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
