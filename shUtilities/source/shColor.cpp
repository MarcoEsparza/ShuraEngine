/*************************************************************/
/*
*  @file    shColor.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
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
  const Color Color::Black = Color(0, 0, 0);

  const Color Color::White = Color(255, 255, 255);

  const Color Color::Red = Color(255, 0, 0);

  const Color Color::Green = Color(0, 255, 0);

  const Color Color::Blue = Color(0, 0, 255);

  const Color Color::Yellow = Color(220, 220, 80);

  const Color Color::Gray = Color(150, 150, 150);

  const Color Color::Purple = Color(165, 115, 206);

/*************************************************************/
/*
*  Constructors
*/
/*************************************************************/

  Color::Color(const Color& copy)
  {
    R = copy.R;
    G = copy.G;
    B = copy.B;
    A = copy.A;
  }
}
