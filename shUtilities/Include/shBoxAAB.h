/*************************************************************/
/*
*  @file    shBoxAAB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
*  @brief   Math class for Axis-Aligned Bounding-Box.
*
*  Math class for Axis-Aligned Bounding-Box.
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
*  Math class for Axis-Aligned Bounding-Box.
*/
class SH_UTILITY_EXPORT shBoxAAB
{
 public:
  /*
  *  Default constructor.
  */
  shBoxAAB() = default;
  /*
  *  Default destructor.
  */
  ~shBoxAAB() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Checks if another box is intersecting with this box.
  *
  *  @param shBoxAAB _other: The other box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  bool
  intersect(const shBoxAAB& _other) const;

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
};
}
