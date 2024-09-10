/*************************************************************/
/*
*  @file    shMatrix4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
*  @brief   Matrix4x4, double array, use double brackets to access to the
*           matrix values.
*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
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
#include "shPrerequsitesUtilities.h"

namespace shEngineSDK {
/*
*  Matrix4x4, double array, use double brackets to access to the matrix values.
* 
*  Sample usage:
*  Matrix4 myMatrix;
*  myMatrix.m[0][0] = 1.0f;
*/
class SH_UTILITY_EXPORT Matrix4
{
  public:
  /*
  *  Default constructor.
  */
  Matrix4() = default;

  /*
  *  Constructor to initialize values to the given numbers.
  */
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);

  /*
  *  Copy constructor
  *
  *  @param Matrix4
  */
  Matrix4(const Matrix4& _other);

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
  *  Use double array to access values.
  */
  float m[4][4];
};
}
