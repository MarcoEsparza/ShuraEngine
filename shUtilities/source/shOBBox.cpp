/*****************************************************************************/
/*
*  @file    shBoxOBB.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for Oriented Bounding-Box.
*
*  Math class for Oriented Bounding-Box.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shOBBox.h"
#include "shMatrix3.h"

namespace shEngineSDK {

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

void
OBBox::projectOnAxis(const Vector3& axis, float& min, float& max) const
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
OBBox::overlapOnProjection(float min1, float max1, float min2, float max2) const
{
  return !(max1 < min2 || max2 < min1);
}

Array<Vector3, 8>
OBBox::getCorners() const
{
  Array<Vector3, 8> corners;
  Vector<Vector3> axes = { Vector3(extent.x, 0.0f, 0.0f),
                           Vector3(0.0f, extent.y, 0.0f),
                           Vector3(0.0f, 0.0f, extent.z)};

  for (uint8 i = 0; i < 8; ++i) {
    Vector3 corner = center;
    for (uint8 j = 0; j < 3; ++j) {
      if (i & (1 << j)) {
        corner = corner + rotation.rotate(axes[j]);
      }
      else {
        corner = corner - rotation.rotate(axes[j]);
      }
    }
  }

  return corners;
}

Vector<Vector3>
OBBox::getVertices() const
{
  Vector<Vector3> vertices;
  vertices.reserve(8);

  Matrix3 rot = rotation.toMatrix3();

  Vector3 right = Vector3(rot.m[0][0], rot.m[0][1], rot.m[0][2]);
  Vector3 up = Vector3(rot.m[1][0], rot.m[1][1], rot.m[1][2]);
  Vector3 foward = Vector3(rot.m[2][0], rot.m[2][1], rot.m[2][2]);

  for (int32 x = -1; x <= 1; x+=2) {
    for (int32 y = -1; y <= 1; y += 2) {
      for (int32 z = -1; z <= 1; z += 2) {
        Vector3 corner = center +
                (right * (extent.x * x)) +
                (up * (extent.y * y)) +
                (foward * (extent.z * z));
        vertices.push_back(corner);
      }
    }
  }

  return vertices;
}
}
