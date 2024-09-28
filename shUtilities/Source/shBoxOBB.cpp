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

Vector3
shBoxOBB::getCorners(const shBoxOBB& box)
{
  //TODO: finish this function.
  Vector3 halfSizeRot = extent.rotate(rotation.toRotate());
  Vector3 corner0 = center + Vector3(-halfSizeRot.x,
                                     -halfSizeRot.y,
                                     -halfSizeRot.z);
  Vector3 corner1;
  Vector3 corner2;
  Vector3 corner3;

  return Vector3();
}
}
