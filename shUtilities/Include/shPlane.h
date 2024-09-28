/*************************************************************/
/*
*  @file    shPlane.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for plane figure.
*
*  Math class for plane figure.
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
*  Math class for plane figure.
*/
class SH_UTILITY_EXPORT shPlane
{
 public:
  /*
  *  Default constructor.
  */
  shPlane() = default;
  /*
  *  Constructor to initialize plane values.
  * 
  *  @param const Vector3& _normal: Desired normal.
  * 
  *  @param const float _distance: Desired distance.
  */
  shPlane(const Vector3& _normal, const float _distance)
          : normal(_normal), distance(_distance) {}
  /*
  *  Default destructor.
  */
  ~shPlane() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Set the normal value for the plane.
  *
  *  @param const Vector3& _normal: Desired normal value.
  *
  *  @return
  */
  void
  setNormal(const Vector3& _normal);

  /*
  *  Set the distance for the plane.
  *
  *  @param const float _distance: Desired distance.
  *
  *  @return
  */
  void
  setDistance(const float _distance);

  /*
  *  Returns plane normal.
  *
  *  @param
  *
  *  @return Vector3
  */
  Vector3
  getNormal() const;

  /*
  *  Returns plane distance.
  *
  *  @param
  *
  *  @return float
  */
  float
  getDistance() const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  Plane normal
   */
   Vector3 normal;
   /*
   *  The distance
   */
   float distance;
};
}
