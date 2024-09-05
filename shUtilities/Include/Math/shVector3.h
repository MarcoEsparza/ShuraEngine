/*************************************************************/
/*
*  @file    shVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Vector3 math header
*
*  Vector3 with float
*/
/*************************************************************/

#pragma once

class  Vector3
{
public:
  Vector3();
  Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
  ~Vector3() = default;

  float x;
  float y;
  float z;


};
