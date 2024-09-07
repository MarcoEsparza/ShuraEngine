/*************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Quaternion
*
*  
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

namespace shEngineSDK {
class Quaternion
{
public:
  /*
  *  Default constructor: initialize Quaternion values to 0.
  */
  Quaternion();
  /*
  *  Constructor to initialize values to the given numbers.
  *  
  *  @param X value, Y value, Z value, W value
  */
  Quaternion(float _x,
             float _y,
             float _z,
             float _w) : x(_x), y(_y), z(_z), w(_w) {}
  /*
  *  Default destructor.
  */
  ~Quaternion() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/



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
