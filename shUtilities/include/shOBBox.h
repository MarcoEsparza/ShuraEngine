/*************************************************************/
/*
*  @file    shBoxOBB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/30
*  @brief   Math class for Oriented Bounding-Box.
*
*  Math class for Oriented Bounding-Box.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
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
  *  @param Vector3& _center: Center values.
  *  @param Quaternion& _rotation: Rotation values.
  *  @param Vector3& _extent: Extent values.
  */
  OBBox(const Vector3& _center,
           const Quaternion& _rotation,
           const Vector3 _extent)
           : center(_center), rotation(_rotation), extent(_extent) {}
  
  /**
  *  @brief Default destructor.
  */
  ~OBBox() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Sets box center.
  *
  *  @param Vector3& _center
  */
  void
  setCenter(const Vector3& _center);

  /**
  *  @brief Sets box rotation.
  *
  *  @param Quaternion& _rotation
  */
  void
  setRotation(const Quaternion& _rotation);

  /**
  *  @brief Sets box extent.
  *
  *  @param Vector3& _extent
  */
  void
  setExtent(const Vector3& _extent);

  /**
  *  @brief Returns box center.
  * 
  *  @return Vector3
  */
  Vector3
  getCenter() const;

  /**
  *  @brief Returns box rotation.
  *
  *  @return Quaternion
  */
  Quaternion
  getRotation() const;

  /**
  *  @brief Returns box extent.
  *
  *  @return Vector3
  */
  Vector3
  getExtent() const;

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

  Vector<Vector3>
  getVertices() const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Box center.
  */
  Vector3 center;

  /**
  *  @brief Rotation in radians.
  */
  Quaternion rotation;

  /**
  *  @brief Half size of the box, how much its "extending" in all axes.
  */
  Vector3 extent;
};
}
