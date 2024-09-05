/*************************************************************/
/*
*  @file    shMatrix4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Matrix4 math header
*
*  
*/
/*************************************************************/

#pragma once

class Matrix4
{
public:
  Matrix4();
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);
  ~Matrix4() = default;

  float m[4][4];
};
