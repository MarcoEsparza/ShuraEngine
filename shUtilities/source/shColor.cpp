/*************************************************************/
/*
*  @file    shColor.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/28
*  @brief   Color class.
*
*  Color class.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shColor.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Constant Variables
*/
/*************************************************************/
  const Color Color::BLACK = Color(0, 0, 0);

  const Color Color::WHITE = Color(255, 255, 255);

  const Color Color::RED = Color(255, 0, 0);

  const Color Color::GREEN = Color(0, 255, 0);

  const Color Color::BLUE = Color(0, 0, 255);

  const Color Color::YELLOW = Color(220, 220, 80);

  const Color Color::GRAY = Color(150, 150, 150);

  const Color Color::PURPLE = Color(165, 115, 206);

/*************************************************************/
/*
*  Constructors
*/
/*************************************************************/

  Color::Color(const Color& copy)
  {
    r = copy.r;
    g = copy.g;
    b = copy.b;
    a = copy.a;
  }
}
