/*************************************************************/
/*
*  @file    shBoxAAB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/23
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
#include "shMath.h"

namespace shEngineSDK {

shBoxAAB::shBoxAAB(const Vector3& _min, const Vector3& _max)
{
  min = _min;
  max = _max;
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
Array<Vector3, 8>
shBoxAAB::getVertices() const
{
  return { min,
           Vector3(min.x, min.y, max.z),
           Vector3(min.x, max.y, min.z),
           Vector3(min.x, max.y, max.z),
           Vector3(max.x, min.y, min.z),
           Vector3(max.x, min.y, max.z),
           Vector3(max.x, max.y, min.z),
           max };
}

Array<uint32, 36>
shBoxAAB::getIndices() const
{
  static Array<uint32, 36> ret{
    0, 1, 2,
    0, 2, 3,
    4, 6, 5,
    4, 7, 6,
    4, 5, 1,
    4, 1, 0,
    3, 2, 6,
    3, 6, 7,
    1, 5, 6,
    1, 6, 2,
    4, 0, 3,
    4, 3, 7
  };

  return ret;
}

void
shBoxAAB::projectOnAxis(const Vector3& axis, float& _min, float& _max) const
{
  _min = _max = axis.dot(min);

  for (uint8 i = 0; i < 3; ++i) {
    float proj = axis.dot(Vector3(i == 0 ? min.x : (i == 0 ? max.x : 0),
                                  i == 1 ? min.y : (i == 1 ? max.y : 0),
                                  i == 2 ? min.z : (i == 2 ? max.z : 0)));

    _min = Math::min(_min, proj);
    _max = Math::max(_max, proj);
  }
}
}
