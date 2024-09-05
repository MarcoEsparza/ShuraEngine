/*************************************************************/
/*
*  @file    shQuaternion.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Quaternion math header
*
*  
*/
/*************************************************************/

#pragma once

class Quaternion
{
public:
  Quaternion();
  Quaternion(float _x,
             float _y,
             float _z,
             float _w) : x(_x), y(_y), z(_z), w(_w) {}
  ~Quaternion() = default;

  float x;
  float y;
  float z;
  float w;
};
