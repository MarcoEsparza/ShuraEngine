/*************************************************************/
/*
*  @file    shBoxAAB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
*  @brief   Math class for Axis-Aligned Bounding-Box.
*
*  Math class for Axis-Aligned Bounding-Box.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shBoxAAB.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

bool
shBoxAAB::intersect(const shBoxAAB& _other) const
{
  return (min.x <= _other.max.x &&
          max.x >= _other.min.x &&
          min.y <= _other.max.y &&
          max.y <= _other.min.y &&
          min.z <= _other.max.z &&
          max.z <= _other.min.z);
}
}
