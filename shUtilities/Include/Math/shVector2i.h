/*************************************************************/
/*
*  @file    shVector2i.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Vector2i math header
*
*  Vector2 with integers
*/
/*************************************************************/

#pragma once

class Vector2i
{
public:
  Vector2i();
  Vector2i(int _x, int _y) : x(_x), y(_y){}
  ~Vector2i() = default;
  
  int x;
  int y;
};
