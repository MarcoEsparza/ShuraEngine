/*************************************************************/
/*
*  @file    shCapsule.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for capsule figure.
*
*  Math class for capsule figure.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/*
*  Math class for capsule figure.
*/
class SH_UTILITY_EXPORT shCapsule
{
 public:
  /*
  *  Default constructor.
  */
  shCapsule() = default;
  /*
  *  Constructor that initialize the capsule values.
  * 
  *  @param const float _radius: The radius.
  * 
  *  @param const Vector3& pA: Point A.
  * 
  *  @param const Vector3& pB: Point B.
  */
  shCapsule(const float _radius, const Vector3& pA, const Vector3& pB)
            : radius(_radius), pointA(pA), pointB(pB) {}
  /*
  *  Default destructor.
  */
  ~shCapsule() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Set the min and max values for the capsule.
  * 
  *  @param const Vector3& pA: Desired first point value.
  * 
  *  @param const Vector3& pB: Desired second point value.
  * 
  *  @return
  */
  void
  setPosition(const Vector3& pA, const Vector3& pB);

  /*
  *  Set the capsule radius.
  *  
  *  @param const float& _radius: Set the radius.
  * 
  *  @return
  */
  void
  setRadius(const float _radius);

  /*
  *  Returns capsule point A.
  *
  *  @param
  *
  *  @return Vector3
  */
  Vector3
  getPointA() const;

  /*
  *  Returns capsule point B.
  *
  *  @param
  *
  *  @return Vector3
  */
  Vector3
  getPointB() const;

  /*
  * Returns capsule radius.
  *
  *  @param
  *
  *  @return float
  */
  float
  getRadius();

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  The radius of the capsule figure.
   */
   float radius;
   /*
   *  The min value of the capsule figure.
   */
   Vector3 pointA;
   /*
   *  The max value of the capsule figure.
   */
   Vector3 pointB;
};
}
