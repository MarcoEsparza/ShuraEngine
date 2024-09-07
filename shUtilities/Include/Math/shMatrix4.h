/*************************************************************/
/*
*  @file    shMatrix4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Matrix4
*
*  
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

namespace shEngineSDK {
class Matrix4
{
public:
  /*
  *  Default constructor: initialize Matrix4 values to 0.
  */
  Matrix4();
  /*
  *  Constructor to initialize values to the given numbers.
  */
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);
  /*
  *  Default destructor.
  */
  ~Matrix4() = default;

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
  *  Double array for values.
  */
  float m[4][4];
};
}
