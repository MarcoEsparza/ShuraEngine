/*************************************************************/
/*
*  @file    shBoxOBB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
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
  Vector<Vector3> axes = { Vector3(extent.x, 0.0f, 0.0f),
                           Vector3(0.0f, extent.y, 0.0f),
                           Vector3(0.0f, 0.0f, extent.z)};

  for (uint8 i = 0; i < 8; ++i) {
    Vector3 corner = center;
    for (uint8 j = 0; j < 3; ++j) {
      if (i & (1 << j)) {
        corner = corner + rotation.toRotate(axes[j]);
      }
      else {
        corner = corner - rotation.toRotate(axes[j]);
      }
    }
  }

  return corners;
}
}
