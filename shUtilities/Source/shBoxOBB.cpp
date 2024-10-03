/*************************************************************/
/*
*  @file    shBoxOBB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
*  @brief   Math class for Oriented Bounding-Box.
*
*  Math class for Oriented Bounding-Box.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shBoxOBB.h"

namespace shEngineSDK {

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

void
shBoxOBB::setCenter(const Vector3& _center)
{
  center = _center;
}

void
shBoxOBB::setRotation(const Quaternion& _rotation)
{
  rotation = _rotation;
}

void
shBoxOBB::setExtent(const Vector3& _extent)
{
  extent = _extent;
}

Vector3
shBoxOBB::getCenter() const
{
  return center;
}

Quaternion
shBoxOBB::getRotation() const
{
  return rotation;
}

Vector3
shBoxOBB::getExtent() const
{
  return extent;
}

void
shBoxOBB::projectOnAxis(const Vector3& axis, float& min, float& max) const
{
  min = max = center.dot(axis);

  Vector3 right, up, forward;
  rotation.toAxes(right, up, forward);

  float projRight = right.dot(axis) * extent.x;
  float projUp = up.dot(axis) * extent.y;
  float projForward = forward.dot(axis) * extent.z;

  min -= projRight - projUp - projForward;
  min += projRight + projUp + projForward;
}

bool
shBoxOBB::overlapOnProjection(float min1, float max1, float min2, float max2) const
{
  return !(max1 < min2 || max2 < min1);
}

Array<Vector3, 8>
shBoxOBB::getCorners() const
{
  Array<Vector3, 8> corners;
  Vector3 right, up, forward;
  rotation.toAxes(right, up, forward);

  for (int8 x = 0; x <= 1; ++x) {
    for (int8 y = 0; y <= 1; ++y) {
      for (int8 z = 0; z <= 1; ++z) {
        corners[x * 4 + y * 2 + z] = {
            center.x + (x * 2 - 1) * extent.x * right.x +
            (y * 2 - 1) * extent.y * up.x +
            (z * 2 - 1) * extent.z * forward.x,
            center.y + (x * 2 - 1) * extent.x * right.y +
            (y * 2 - 1) * extent.y * up.y +
            (z * 2 - 1) * extent.z * forward.y,
            center.z + (x * 2 - 1) * extent.x * right.z +
            (y * 2 - 1) * extent.y * up.z +
            (z * 2 - 1) * extent.z * forward.z
        };
      }
    }
  }

  return corners;
}
}
