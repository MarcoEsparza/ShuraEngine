/*************************************************************/
/*
*  @file    shBoxOBB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
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
  *  @param Vector3 _min: Min values of X, Y and Z
  * 
  *  @param Vector3 _max: Max values of X, Y and Z
  */
  FORCEINLINE shBoxOBB(const Vector3& _min, const Vector3& _max) :
                       min(_min), max(_max)
  {
    extent = max - min;
  }
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
   *  Minimum values for X, Y and Z position
   */
   Vector3 min;
   /*
   *  Maximum values for X, Y and Z position
   */
   Vector3 max;
   /*
   *  Half lenghts of the box along each axis
   */
   Vector3 extent;
};
}
