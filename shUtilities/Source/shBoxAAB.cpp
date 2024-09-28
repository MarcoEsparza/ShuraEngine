/*************************************************************/
/*
*  @file    shBoxAAB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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

shBoxAAB::shBoxAAB(const Vector3& pos, const Vector3& size)
{
  min = pos;
  max = min + size;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shBoxAAB::setPosition(const Vector3& pos)
{
  min = pos;
}

void
shBoxAAB::setSize(const Vector3& size)
{
  max = min + size;
}

Vector3
shBoxAAB::getMinPosition() const
{
  return min;
}

Vector3
shBoxAAB::getMaxPosition() const
{
  return max;
}
}
