/*************************************************************/
/*
*  @file    shRect.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for rectangle figure.
*
*  Math class for rectangle figure.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shRect.h"

namespace shEngineSDK {

shRect::shRect(const Vector2& pos, const Vector2& size)
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
shRect::setPosition(const Vector2& pos)
{
  min = pos;
}

void
shRect::setSize(const Vector2& size)
{
  max = min + size;
}

Vector2
shRect::getMinPosition() const
{
  return min;
}

Vector2
shRect::getMaxPosition() const
{
  return max;
}
}
