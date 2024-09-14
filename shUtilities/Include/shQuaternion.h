/*************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/13
*  @brief   Quaternion for rotations
*
*  Quaternion for rotations
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
/*
*  Quaternion for rotations
* 
*  Sample usage:
*  
*/
class SH_UTILITY_EXPORT Quaternion
{
 public:
  /*
  *  Default constructor.
  */
  Quaternion() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  *  Recommended to initialize in radians.
  *  
  *  @param float _x: X value.
  *  @param float _y: Y value.
  *  @param float _z: Z value.
  *  @param float _w: W value.
  */
  Quaternion(float _x, float _y, float _z, float _w) :
             x(_x), y(_y), z(_z), w(_w) {}

  /*
  *  Copy constructor
  *
  *  @param Quaternion
  */
  Quaternion(const Quaternion& _other);

  /*
  *  Default destructor.
  */
  ~Quaternion() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  
  *
  *  @param
  * 
  *  @return
  */
  void
  eulerAngles();

  /*
  *
  *
  *  @param
  *
  *  @return
  */
  float
  mag();

  /*
  *
  *
  *  @param
  *
  *  @return
  */
  void
  normalized();

  /*
  *
  *
  *  @param
  *
  *  @return
  */
  float
  dot(const Quaternion& _other);

  /*
  *
  *
  *  @param
  *
  *  @return
  */
  Quaternion
  lerp(const Quaternion& _quatA,
       const Quaternion& _quatB,
       const float& _time);

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /*
  * Quaternion X value.
  */
  float x;
  /*
  * Quaternion Y value.
  */
  float y;
  /*
  * Quaternion Z value.
  */
  float z;
  /*
  * Quaternion W value.
  */
  float w;
};
}
