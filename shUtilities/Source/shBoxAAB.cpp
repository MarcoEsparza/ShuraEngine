/*************************************************************/
/*
*  @file    shBoxAAB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
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
