/*****************************************************************************/
/*
*  @file    shBoxOBB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for Oriented Bounding-Box.
*
*  Math class for Oriented Bounding-Box.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shQuaternion.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Math class for Oriented Bounding-Box.
*/
class SH_UTILITY_EXPORT OBBox
{
 public:
  /**
  *  @brief Default constructor.
  */
  OBBox() = default;
  /**
  *  @brief Constructor that initialize box values.
  * 
  *  @param Vector3& center: Center values.
  *  @param Quaternion& rotation: Rotation values.
  *  @param Vector3& extent: Extent values.
  */
  OBBox(const Vector3& center,
        const Quaternion& rotation,
        const Vector3 extent)
    : center(center), rotation(rotation), extent(extent)
  {}
  
  /**
  *  @brief Default destructor.
  */
  ~OBBox() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Project the box corners on the axis.
  *
  *  @param Vector3& axis
  *  @param float& min: Reference is needed, it will save the result
  *  @param float& max: Reference is needed, it will save the result
  */
  void
  projectOnAxis(const Vector3& axis, float& min, float& max) const;

  /**
  *  @brief Checks for an intersection on the axis.
  *
  *  @param float min1
  *  @param float max1
  *  @param float min2
  *  @param float max2
  *
  *  @return bool
  */
  bool
  overlapOnProjection(float min1, float max1, float min2, float max2) const;

  /**
  *  @brief Calculate the box corners.
  *
  *  @return Array<Vector3, 8> Return the corners.
  */
  Array<Vector3, 8>
  getCorners() const;

  /**
  *  @brief Calculate the box vertices.
  * 
  *  @return Vector<Vector3> Return the vertices.
  */
  Vector<Vector3>
  getVertices() const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Box center.
  */
  Vector3 center = Vector3::ZERO;

  /**
  *  @brief Rotation in radians.
  */
  Quaternion rotation = Quaternion::IDENTITY;

  /**
  *  @brief Half size of the box, how much its "extending" in all axes.
  */
  Vector3 extent = Vector3::ZERO;
};
}
