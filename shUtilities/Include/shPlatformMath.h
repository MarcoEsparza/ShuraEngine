/*************************************************************/
/*
*  @file    shPlatformMath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
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
class Vector3;
class Vector2;
class BoxAAB;
class BoxOBB;
class Capsule;
class Plane;
class Rect;
class Sphere;
class Degree;
class Radian;

/**
*  @brief Math class wrapper, using the STD. compatible with Windows, Linux and OSX.
*/
class SH_UTILITY_EXPORT PlatformMath
{
public:
  /**
  *  @brief Default constructor.
  */
  PlatformMath() = default;

  /**
  *  @brief Default destructor.
  */
  ~PlatformMath() = default;

  /*************************************************************/
  /*
  *  Algebra
  */
  /*************************************************************/

  /**
  *  @brief Calculate the square root of a given number.
  * 
  *  @param float value: Value to calculate.
  * 
  *  @return float The result of the square root of value.
  */
  static float
  sqrt(const float value);

  /**
  *  @brief Calculate the pow of a given number.
  *
  *  @param float value: Value to calculate.
  *  @param float vPow: Number of times to be multiplied.
  *
  *  @return float The calculated pow.
  */
  static float
  pow(const float value, const float vPow);

  /**
  *  @brief Calculate the linear interpolation between two given values.
  *
  *  @param float A: First value.
  *  @param float B: Second value.
  *  @param float alpha: Alpha to calculate with previous numbers.
  *
  *  @return float The result of the linear interpolation.
  */
  static float
  lerp(const float A, const float B, const float alpha);

  /**
  *  @brief Calculate the minimum between two given numbers.
  *
  *  @param float x: First value to compare.
  *  @param float y: Second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  min(const float x, const float y);

  /**
  *  @brief Calculate the maximum between two given numbers.
  *
  *  @param float x: First value to compare.
  *  @param float y: Second value.
  *
  *  @return float The result of the comparison.
  */
  static float
  max(const float x, const float y);

  /**
  *  @brief Clamp a given number with given min and max values.
  *
  *  @param float x: Value to calculate.
  *  @param float min: Min value.
  *  @param float max: Max value.
  *
  *  @return float The result of the clamp.
  */
  static float
  clamp(const float x, const float min, const float max);

  /**
  *  @brief Calculate the floating point reminder of the division of the given
  *         values.
  *
  *  @param float value1: Dividend.
  *  @param float value2: Divisor.
  *
  *  @return float The reminder of the division.
  */
  static float
  fmod(const float value1, const float value2);

  /**
  *  @brief Calculate the absolute of a given number.
  *
  *  @param float value.
  *
  *  @return float
  */
  static float
  abs(const float value);

  static float
  log2(const float value);

  /**
  *  @brief Computes the min element of two given arrays.
  * 
  *  @param const Array<float, 4>::iterator first
  *  @param const Array<float, 4>::iterator last
  * 
  *  @return const Array<float, 4>::iterator
  */
  static Array<float, 4>::iterator
  min_element(const Array<float, 4>::iterator first, const Array<float, 4>::iterator last);

  /**
  *  @brief Computes the max element of two given arrays.
  *
  *  @param const Array<float, 4>::iterator first
  *  @param const Array<float, 4>::iterator last
  *
  *  @return const Array<float, 4>::iterator
  */
  static Array<float, 4>::iterator
  max_element(const Array<float, 4>::iterator first, const Array<float, 4>::iterator last);
  
  /**
  *  @brief Checks if a value is NaN.
  *
  *  @param const float value
  *
  *  @return bool
  */
  static bool
  is_NaN(const float value);

  /*************************************************************/
  /*
  *  Trigonometric functions
  */
  /*************************************************************/

  /**
  *  @brief Cosine.
  *
  *  @param Radian& radian
  *
  *  @return float Cosine from value.
  */
  static float
  cos(const Radian& radian);

  /**
  *  @brief Sine.
  *
  *  @param Radian& radian
  *
  *  @return float Sine from value.
  */
  static float
  sin(const Radian& radian);

  /**
  *  @brief Tangent.
  *
  *  @param Radian& radian
  *
  *  @return float Tangent from value.
  */
  static float
  tan(const Radian& radian);

  /**
  *  @brief Hyperbolic cosine.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic cosine from value.
  */
  static float
  cosh(const Radian& radian);

  /**
  *  @brief Hyperbolic sine.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic sine from value.
  */
  static float
  sinh(const Radian& radian);

  /**
  *  @brief Hyperbolic tangent.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic tangent from value.
  */
  static float
  tanh(const Radian& radian);

  /**
  *  @brief Arch cosine.
  *
  *  @param Radian& radian
  *
  *  @return float Arch cosine from value.
  */
  static float
  acos(const Radian& radian);

  /**
  *  @brief Arch sine.
  *
  *  @param Radian& radian
  *
  *  @return float Arch sine from value.
  */
  static float
  asin(const Radian& radian);

  /**
  *  @brief Arch Tangent.
  *
  *  @param Radian& radian
  *
  *  @return float Arch Tangent from value.
  */
  static float
  atan(const Radian& radian);

  /**
  *  @brief Arch Tangent 2.
  *
  *  @param Radian& rad1
  *  @param Radian& rad2
  *
  *  @return float Arch Tangent from value.
  */
  static float
  atan2(const Radian& rad1, const Radian& rad2);

  /**
  *  @brief Hyperbolic arch cosine.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic arch cosine from value.
  */
  static float
  acosh(const Radian& radian);

  /**
  *  @brief Hyperbolic arch sine.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic arch sine from value.
  */
  static float
  asinh(const Radian& radian);

  /**
  *  @brief Hyperbolic arch tangent.
  *
  *  @param Radian& radian
  *
  *  @return float Hyperbolic arch tangent from value.
  */
  static float
  atanh(const Radian& radian);

  /*************************************************************/
  /*
  *  Figures
  */
  /*************************************************************/

  /**
  *  @brief Checks if a point is intersecting with an axis aligned bounding box.
  *
  *  @param Vector3 point: The point.
  *  @param shBoxAAB box: The box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointBoxIntersect(const Vector3& point, const BoxAAB& box);

  /**
  *  @brief Checks if a point is intersecting with an oriented bounding box.
  *
  *  @param Vector3 point: The point.
  *  @param shBoxOBB box: The box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointBoxIntersect(const Vector3& point, const BoxOBB& box);

  /**
  *  @brief Checks if a point is intersecting with a sphere.
  *
  *  @param Vector3 point: The point.
  *  @param shSphere sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointSphereIntersect(const Vector3& point, const Sphere& sph);

  /**
  *  @brief Checks if a point is intersecting with a capsule.
  *
  *  @param Vector3 point: The point.
  *  @param shCapsule cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointCapsuleIntersect(const Vector3& point, const Capsule& cap);

  /**
  *  @brief Checks if a point is intersecting with a rect.
  *
  *  @param Vector3 point: The point.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointRectIntersect(const Vector2& point, const Rect& rect);

  /**
  *  @brief Checks if a point is intersecting with a plane.
  *
  *  @param Vector3 point: The point.
  *  @param shPlane plane: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  pointPlaneIntersect(const Vector3& point, const Plane& plane);

  /**
  *  @brief Checks if a box is intersecting with another box.
  *
  *  @param shBoxAAB box: The first box.
  *  @param shBoxAAB box1: The second box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const BoxAAB& box, const BoxAAB& box1);

  /**
  *  @brief Checks if a box is intersecting with another box.
  *
  *  @param shBoxOBB box: The first box.
  *  @param shBoxOBB box1: The second box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const BoxOBB& box, const BoxOBB& box1);

  /**
  *  @brief Checks if a box is intersecting with another box.
  *
  *  @param shBoxAAB boxA: The axis aligned bounding box.
  *  @param shBoxOBB boxO: The oriented bounding box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxBoxIntersect(const BoxAAB& boxA, const BoxOBB& boxO);

  /**
  *  @brief Checks if a box is intersecting with a capsule.
  *
  *  @param shBoxAAB box: The axis aligned bounding box.
  *  @param shCapsule cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxCapsuleIntersect(const BoxAAB& box, const Capsule& cap);

  /**
  *  @brief Checks if a box is intersecting with a capsule.
  *
  *  @param shBoxOBB box: The oriented bounding box.
  *  @param shCapsule cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxCapsuleIntersect(const BoxOBB& box, const Capsule& cap);

  /**
  *  @brief Checks if a box is intersecting with a plane.
  *
  *  @param shBoxAAB box: The axis aligned bounding box.
  *  @param shPlane plane: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxPlaneIntersect(const BoxAAB& box, const Plane& plane);

  /**
  *  @brief Checks if a box is intersecting with a plane.
  *
  *  @param shBoxOBB box: The oriented bounding box.
  *  @param shPlane plane: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxPlaneIntersect(const BoxOBB& box, const Plane& plane);

  /**
  *  @brief Checks if a box is intersecting with a rect.
  *
  *  @param shBoxAAB box: The axis aligned bounding box.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxRectIntersect(const BoxAAB& box, const Rect& rect);

  /**
  *  @brief Checks if a box is intersecting with a rect.
  *
  *  @param shBoxOBB box: The oriented bounding box.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxRectIntersect(const BoxOBB& box, const Rect& rect);

  /**
  *  @brief Checks if a sphere is intersecting with another sphere.
  *
  *  @param shSphere sph: The first sphere.
  *  @param shSphere sph1: The second sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereSphereIntersect(const Sphere& sph, const Sphere& sph1);

  /**
  *  @brief Checks if a box is intersecting with a sphere.
  *
  *  @param shBoxAAB box: The axis aligned bounding box.
  *  @param shSphere sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxSphereIntersect(const BoxAAB& box, const Sphere& sph);

  /**
  *  @brief Checks if a box is intersecting with a sphere.
  *
  *  @param shBoxOBB box: The oriented bounding box.
  *  @param shSphere sph: The sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  boxSphereIntersect(const BoxOBB& box, const Sphere& sph);

  /**
  *  @brief Checks if a sphere is intersecting with a capsule.
  *
  *  @param shSphere sph: The sphere.
  *  @param shCapsule cap: The capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereCapsuleIntersect(const Sphere& sph, const Capsule& cap);

  /**
  *  @brief Checks if a sphere is intersecting with a plane.
  *
  *  @param shSphere sph: The sphere.
  *  @param shPlane plane: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  spherePlaneIntersect(const Sphere& sph, const Plane& plane);

  /**
  *  @brief Checks if a sphere is intersecting with a rect.
  *
  *  @param shSphere sph: The sphere.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  sphereRectIntersect(const Sphere& sph, const Rect& rect);

  /**
  *  @brief Checks if a capsule is intersecting with another capsule.
  *
  *  @param shCapsule cap: The first capsule.
  *  @param shCapsule cap1: The second capsule.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsuleCapsuleIntersect(const Capsule& cap, const Capsule& cap1);
  
  /**
  *  @brief Checks if a capsule is intersecting with a plane.
  *
  *  @param shCapsule cap: The capsule.
  *  @param shPlane plane: The plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsulePlaneIntersect(const Capsule& cap, const Plane& pln);

  /**
  *  @brief Checks if a capsule is intersecting with a rect.
  *
  *  @param shCapsule cap: The capsule.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  capsuleRectIntersect(const Capsule& cap, const Rect& rect);

  /**
  *  @brief Checks if a plane is intersecting with another plane.
  *
  *  @param shPlane plane: The first plane.
  *  @param shPlane plane1: The second plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  planePlaneIntersect(const Plane& plane, const Plane& plane1);

  /**
  *  @brief Checks if a plane is intersecting with a rect.
  *
  *  @param shPlane plane: The plane.
  *  @param shRect rect: The rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  planeRectIntersect(const Plane& plane, const Rect& rect);

  /**
  *  @brief Checks if a rect is intersecting with another rect.
  *
  *  @param shRect rect: The first rect.
  *  @param shRect rect1: The second rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  static bool
  rectRectIntersect(const Rect& rect, const Rect& rect1);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
public:
  /**
  *  @brief PI value.
  */
  static const float PI;

  /**
  *  @brief Use to convert a Radian value to a Degree angle.
  */
  static const float RAD2DEG;

  /**
  *  @brief Use to convert a Degree angle to a Radian value.
  */
  static const float DEG2RAD;

  /**
  *  @brief Small number.
  */
  static const float SMALL_NUMBER;
};
}
