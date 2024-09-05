/*************************************************************/
/*
*  @file    shVector4.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Vector4 math header
*
*  Vector4 with float
*/
/*************************************************************/

#pragma once

class  Vector4
{
public:
  Vector4();
  Vector4(float _x,
          float _y,
          float _z,
          float _w) : x(_x), y(_y), z(_z), w(_w){}
  ~Vector4() = default;

  float x;
  float y;
  float z;
  float w;


};
