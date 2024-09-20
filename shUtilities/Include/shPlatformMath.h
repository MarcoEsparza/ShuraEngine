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
#include "shBoxAAB.h"
#include "shBoxOBB.h"
#include "shCapsule.h"
#include "shPlane.h"
#include "shRect.h"
#include "shSphere.h"

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
  *  Figures
  */
  /*************************************************************/

  /*
  *  Checks if a point is intersecting with an axis aligned bounding box.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shBoxAAB _box: The box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointBoxIntersect(const Vector3& _point, const shBoxAAB& _box);

  /*
  *  Checks if a point is intersecting with an oriented bounding box.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shBoxOBB _box: The box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointBoxIntersect(const Vector3& _point, const shBoxOBB& _box);

  /*
  *  Checks if a point is intersecting with a sphere.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointSphereIntersect(const Vector3& _point, const shSphere& _sph);

  /*
  *  Checks if a point is intersecting with a capsule.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointCapsuleIntersect(const Vector3& _point, const shCapsule& _cap);

  /*
  *  Checks if a point is intersecting with a rect.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointRectIntersect(const Vector2& _point, const shRect& _r);

  /*
  *  Checks if a point is intersecting with a plane.
  *
  *  @param Vector3 _point: The point.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointPlaneIntersect(const Vector2& _point, const shPlane& _pln);

  /*
  *  Checks if a box is intersecting with another box.
  *
  *  @param shBoxAAB _box: The first box.
  * 
  *  @param shBoxAAB _box1: The second box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const shBoxAAB& _box, const shBoxAAB& _box1);

  /*
  *  Checks if a box is intersecting with another box.
  *
  *  @param shBoxOBB _box: The first box.
  *
  *  @param shBoxOBB _box1: The second box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const shBoxOBB& _box, const shBoxOBB& _box1);

  /*
  *  Checks if a box is intersecting with another box.
  *
  *  @param shBoxAAB _boxA: The axis aligned bounding box.
  *
  *  @param shBoxOBB _boxO: The oriented bounding box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const shBoxAAB& _boxA, const shBoxOBB& _boxO);

  /*
  *  Checks if a box is intersecting with a capsule.
  *
  *  @param shBoxAAB _box: The axis aligned bounding box.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxCapsuleIntersect(const shBoxAAB& _box, const shCapsule& _cap);

  /*
  *  Checks if a box is intersecting with a capsule.
  *
  *  @param shBoxOBB _box: The oriented bounding box.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxCapsuleIntersect(const shBoxOBB& _box, const shCapsule& _cap);

  /*
  *  Checks if a box is intersecting with a plane.
  *
  *  @param shBoxAAB _box: The axis aligned bounding box.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxPlaneIntersect(const shBoxAAB& _box, const shPlane& _pln);

  /*
  *  Checks if a box is intersecting with a plane.
  *
  *  @param shBoxOBB _box: The oriented bounding box.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxPlaneIntersect(const shBoxOBB& _box, const shPlane& _pln);

  /*
  *  Checks if a box is intersecting with a rect.
  *
  *  @param shBoxAAB _box: The axis aligned bounding box.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxRectIntersect(const shBoxAAB& _box, const shRect& _r);

  /*
  *  Checks if a box is intersecting with a rect.
  *
  *  @param shBoxOBB _box: The oriented bounding box.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxRectIntersect(const shBoxOBB& _box, const shRect& _r);

  /*
  *  Checks if a sphere is intersecting with another sphere.
  *
  *  @param shSphere _sph: The first sphere.
  *
  *  @param shSphere _sph1: The second sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereSphereIntersect(const shSphere& _sph, const shSphere& _sph1);

  /*
  *  Checks if a box is intersecting with a sphere.
  *
  *  @param shBoxAAB _box: The axis aligned bounding box.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxSphereIntersect(const shBoxAAB& _box, const shSphere& _sph);

  /*
  *  Checks if a box is intersecting with a sphere.
  *
  *  @param shBoxOBB _box: The oriented bounding box.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxSphereIntersect(const shBoxOBB& _box, const shSphere& _sph);

  /*
  *  Checks if a sphere is intersecting with a capsule.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereCapsuleIntersect(const shSphere& _sph, const shCapsule& _cap);

  /*
  *  Checks if a sphere is intersecting with a plane.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  spherePlaneIntersect(const shSphere& _sph, const shPlane& _pln);

  /*
  *  Checks if a sphere is intersecting with a rect.
  *
  *  @param shSphere _sph: The sphere.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereRectIntersect(const shSphere& _sph, const shRect& _r);

  /*
  *  Checks if a capsule is intersecting with another capsule.
  *
  *  @param shCapsule _cap: The first capsule.
  *
  *  @param shCapsule _cap1: The second capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsuleCapsuleIntersect(const shCapsule& _cap, const shCapsule& _cap1);
  
  /*
  *  Checks if a capsule is intersecting with a plane.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsulePlaneIntersect(const shCapsule& _cap, const shPlane& _pln);

  /*
  *  Checks if a capsule is intersecting with a rect.
  *
  *  @param shCapsule _cap: The capsule.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsuleRectIntersect(const shCapsule& _cap, const shRect& _r);

  /*
  *  Checks if a plane is intersecting with another plane.
  *
  *  @param shPlane _pln: The first plane.
  *
  *  @param shPlane _pln1: The second plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  planePlaneIntersect(const shPlane& _pln, const shPlane& _pln1);

  /*
  *  Checks if a plane is intersecting with a rect.
  *
  *  @param shPlane _pln: The plane.
  *
  *  @param shRect _r: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  planeRectIntersect(const shPlane& _pln, const shRect& _r);

  /*
  *  Checks if a rect is intersecting with another rect.
  *
  *  @param shRect _r: The first rect.
  *
  *  @param shRect _r1: The second rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  rectRectIntersect(const shRect& _r, const shRect& _r1);

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
