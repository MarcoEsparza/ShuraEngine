/*************************************************************/
/*
*  @file    shVector2.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Vector2 math header
*
*  Vector2 with float
*/
/*************************************************************/

#pragma once
#include <math.h>

class  Vector2
{
public:
  Vector2();
  Vector2(float _x, float _y) : x(_x), y(_y){}
  ~Vector2() = default;

  float x;
  float y;

  float
  Dot(Vector2& vec);

  float
  Mag() const;

  Vector2
  Cross(Vector2& other);
};
