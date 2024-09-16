/*************************************************************/
/*
*  @file    shCapsule.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/16
*  @brief   Math class for capsule figure.
*
*  Math class for capsule figure.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/*
*  Math class for capsule figure.
*/
class SH_UTILITY_EXPORT shCapsule
{
 public:
  /*
  *  Default constructor.
  */
  shCapsule() = default;
  /*
  *  Constructor that initialize the capsule with the given radius and height.
  *  @brief The center is initialized in x = 0.0f, y = 0.0f, z = 0.0f.
  * 
  *  @param float _r: The radius.
  * 
  *  @param float _h: The height.
  */
  shCapsule(const float& _r, const float& _h) :
            radius(_r),
            height(_h),
            center(Vector3(0.0f,0.0f,0.0f)) {}
  /*
  *  Default destructor.
  */
  ~shCapsule() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Checks if another box is intersecting with this box.
  *
  *  @param shBoxAAB _other: The other box.
  *
  *  @return bool True if is intersecting, false is otherwise.
  */
  bool
  intersect(const shCapsule& _other) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  The radius of the capsule figure.
   */
   float radius;
   /*
   *  The height of the capsule figure.
   */
   float height;
   /*
   *  The position of the center of the capsule.
   */
   Vector3 center;
};
}
