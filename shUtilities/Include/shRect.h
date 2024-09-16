/*************************************************************/
/*
*  @file    shRect.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Math class for rectangle figure.
*
*  Math class for rectangle figure.
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
*  Math class for rectangle figure.
*/
class SH_UTILITY_EXPORT shRect
{
 public:
  /*
  *  Default constructor.
  */
  shRect() = default;
  /*
  *  Default destructor.
  */
  ~shRect() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Checks if another rect is intersecting with this rect.
  *
  *  @param shBoxAAB _other: The other rect.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  bool
  intersect(const shRect& _other) const;

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
