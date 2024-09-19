/*************************************************************/
/*
*  @file    shPlane.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
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
#include "shVector2.h"

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
  *  Default destructor.
  */
  ~shPlane() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Checks if another plane is intersecting with this plane.
  *
  *  @param shBoxAAB _other: The other plane.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  bool
  intersect(const shPlane& _other) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  Minimum values for X, Y and Z position
   */
   Vector2 min;
   /*
   *  Maximum values for X, Y and Z position
   */
   Vector2 max;
};
}
