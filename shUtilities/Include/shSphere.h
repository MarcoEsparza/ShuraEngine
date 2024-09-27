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
  *  @param const float _r: Desired radius.
  */
  shSphere(const Vector3& _center, const float _r)
           : center(_center), radius(_r) {}
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
  *  @param const Vector3& _center: Desired center.
  * 
  *  @return
  */
  void
  setCenter(const Vector3& _center);

  /*
  *  Set the radius of the sphere.
  *  
  *  @param const float _radius: Desired radius.
  * 
  *  @return
  */
  void
  setSize(const float _radius);

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
