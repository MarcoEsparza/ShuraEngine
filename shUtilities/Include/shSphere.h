/*************************************************************/
/*
*  @file    shSphere.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for sphere figure.
*
*  Math class for sphere figure.
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
*  Math class for sphere figure.
*/
class SH_UTILITY_EXPORT shSphere
{
 public:
  /*
  *  Default constructor.
  */
  shSphere() = default;
  /*
  *  Constructor to initialize center and radius values.
  * 
  *  @param const Vector3& _center: Desired center.
  * 
  *  @param const float _radius: Desired radius.
  */
  shSphere(const Vector3& _center, const float _radius)
           : center(_center), radius(_radius) {}
  /*
  *  Default destructor.
  */
  ~shSphere() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Set the center of the sphere.
  * 
  *  @param Vector3& _center: Desired center.
  * 
  *  @return
  */
  void
  setCenter(const Vector3& _center);

  /*
  *  Set the radius of the sphere.
  *  
  *  @param float _radius: Desired radius.
  * 
  *  @return
  */
  void
  setRadius(const float _radius);

  /*
  *  Returns sphere center.
  *
  *  @param 
  *
  *  @return Vector3
  */
  Vector3
  getCenter() const;

  /*
  *  Returns sphere radius.
  *
  *  @param
  *
  *  @return float
  */
  float
  getRadius() const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
public:
   /*
   *  The sphere radius.
   */
   float radius;
   /*
   *  The position of the center of the sphere.
   */
   Vector3 center;
};
}
