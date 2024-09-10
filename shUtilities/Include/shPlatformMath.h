/*************************************************************/
/*
*  @file    shSPlatformMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
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
#include "shPrerequsitesUtilities.h"

namespace shEngineSDK {
using Degree = float;
using Radian = float;

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
  template <class T, class U>
  FORCEINLINE static T
  lerp(const T& A, const T& B, const U& alpha);

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
  template <class T>
  FORCEINLINE static T
  clamp(const T& X, const T& min, const T& max);
  
  /*************************************************************/
  /*
  *  Trigonometry
  */
  /*************************************************************/

  /*
  *  
  *
  *  @param 
  *
  *  @return 
  */
  static Degree
  cos(const Degree& value);

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

  /*************************************************************/
  /*
  *  Implementations
  */
  /*************************************************************/
  

  /*
  *  Calculate the linear interpolation between two given values
  *
  *  @param First value, second value and alpha. First and second value must
  *         be the same type.
  *
  *  @return T The result of the linear interpolation.
  */
  template <class T, class U>
  FORCEINLINE static T
  PlatformMath::lerp(const T& A, const T& B, const U& alpha)
  {
    return static_cast<T>(A + (B - A) * alpha);
  }
  
  /*
  *  Clamp a given number with given min and max values
  *
  *  @param Value to calculate, min value and max value. All parameter must be
  *         the same type.
  *
  *  @return T The result of the clamp.
  */
  template <class T>
  FORCEINLINE static T
  PlatformMath::clamp(const T& X, const T& min, const T& max)
  {
    return X < min ? min : X < max ? X : max;
  }
};
}
