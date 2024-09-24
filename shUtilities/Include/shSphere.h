/*************************************************************/
/*
*  @file    shSphere.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
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
