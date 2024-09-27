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

shRect::shRect(const Vector2& _pos, const Vector2& _size)
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
shRect::setPosition(const Vector2& _pos)
{
  min = _pos;
}

void
shRect::setSize(const Vector2& _size)
{
  max = min + _size;
}

}
