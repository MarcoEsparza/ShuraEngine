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

shBoxAAB::shBoxAAB(const Vector3& _pos, const Vector3& _size)
{
  min = _pos;
  max = min + _size;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shBoxAAB::setPosition(const Vector3& _pos)
{
  min = _pos;
}

void
shBoxAAB::setSize(const Vector3& _size)
{
  max = min + _size;
}

}
