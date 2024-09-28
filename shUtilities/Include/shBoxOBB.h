/*************************************************************/
/*
*  @file    shBoxOBB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
/*
*  Math class for Oriented Bounding-Box.
*/
class SH_UTILITY_EXPORT shBoxOBB
{
 public:
  /*
  *  Default constructor.
  */
  shBoxOBB() = default;
  /*
  *  Constructor that initialize box values.
  * 
  *  @param Vector3& _center: Center values.
  * 
  *  @param Quaternion& _rotation: Rotation values.
  * 
  *  @param Vector3& _extent: Extent values.
  */
  shBoxOBB(const Vector3& _center,
           const Quaternion& _rotation,
           const Vector3 _extent)
           : center(_center), rotation(_rotation), extent(_extent) {}
  
  /*
  *  Default destructor.
  */
  ~shBoxOBB() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Sets box center.
  *
  *  @param Vector3& _center
  *
  *  @return
  */
  void
  setCenter(const Vector3& _center);

  /*
  *  Sets box rotation.
  *
  *  @param Quaternion& _rotation
  *
  *  @return
  */
  void
  setRotation(const Quaternion& _rotation);

  /*
  *  Sets box extent.
  *
  *  @param Vector3& _extent
  *
  *  @return
  */
  void
  setExtent(const Vector3& _extent);

  /*
  *  Returns box center.
  *
  *  @param
  *
  *  @return Vector3
  */
  Vector3
  getCenter() const;

  /*
  *  Returns box rotation.
  *
  *  @param
  *
  *  @return Quaternion
  */
  Quaternion
  getRotation() const;

  /*
  *  Returns box extent.
  *
  *  @param
  *
  *  @return Vector3
  */
  Vector3
  getExtent() const;

  /*
  *  Calculate corners for intersecction.
  *  // TODO: not finished yet.
  *
  *  @param shBoxOBB& box
  *
  *  @return
  */
  Vector3
  getCorners(const shBoxOBB& box);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  Box center.
   */
   Vector3 center;
   /*
   *  Rotation in radians.
   */
   Quaternion rotation;
   /*
   *  Half size.
   */
   Vector3 extent;
};
}
