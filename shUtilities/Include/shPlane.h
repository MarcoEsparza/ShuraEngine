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
  *  @param const Vector3& _norm: Desired normal.
  * 
  *  @param const float _d: Desired distance.
  */
  shPlane(const Vector3& _norm, const float _d)
          : normal(_norm), distance(_d) {}
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
  *  @param const Vector3& _norm: Desired normal value.
  *
  *  @return
  */
  void
  setNormal(const Vector3& _norm);

  /*
  *  Set the distance for the plane.
  *
  *  @param const float _d: Desired distance.
  *
  *  @return
  */
  void
  setDistance(const float _d);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  
   */
   Vector3 normal;
   /*
   *  
   */
   float distance;
};
}
