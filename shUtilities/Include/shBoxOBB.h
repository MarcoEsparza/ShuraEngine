/*************************************************************/
/*
*  @file    shBoxOBB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for Oriented Bounding-Box.
*
*  Math class for Oriented Bounding-Box.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shQuaternion.h"
#include "shVector3.h"

namespace shEngineSDK {
/*
*  Math class for Oriented Bounding-Box.
*/
class SH_UTILITY_EXPORT shBoxOBB
{
 public:
  /*
  *  Default constructor.
  */
  shBoxOBB() = default;
  /*
  *  Constructor that initialize min and max value with the given Vector3
  *  and calculate the extent.
  * 
  *  @param Vector3& _center: Center values.
  * 
  *  @param VQuaternion& _rot: Rotation values.
  */
  
  /*
  *  Default destructor.
  */
  ~shBoxOBB() = default;

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
   *  
   */
   Vector3 center;
   /*
   *  
   */
   Quaternion rotation;
   /*
   *  
   */
   Vector3 extent;
};
}
