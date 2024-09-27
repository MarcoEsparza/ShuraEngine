/*************************************************************/
/*
*  @file    shBoxAAB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
  *  Constructor to initialize box values.
  * 
  *  @param const Vector3& _pos: Desired min value.
  * 
  *  @param const Vector3& _size: Compute max value with size and min value.
  */
  shBoxAAB(const Vector3& _pos, const Vector3& _size);
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
  *  Set the min values for the box.
  *
  *  @param const Vector3& _pos: Desired min value.
  *
  *  @return
  */
  void
  setPosition(const Vector3& _pos);

  /*
  *  Compute the max values with given size and the position.
  *
  *  @param const Vector3& _size: Set the max values with this plus position.
  *
  *  @return
  */
  void
  setSize(const Vector3& _size);

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
