/*************************************************************/
/*
*  @file    shSphere.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
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
  *  Default destructor.
  */
  ~shSphere() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Checks if another sphere is intersecting with this sphere.
  *
  *  @param shSphere _other: The other sphere.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  bool
  intersect(const shSphere& _other);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  The sphere radius
   */
   float radius;
   /*
   *  The position of the center of the sphere.
   */
   Vector3 center;
};
}
