/*************************************************************/
/*
*  @file    shLinearColor.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   LinearColor class.
*
*  LinearColor class.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shLinearColor.h"
#include "shMath.h"

namespace shEngineSDK {
LinearColor::LinearColor(const LinearColor& copy)
{
  R = copy.R;
  G = copy.G;
  B = copy.B;
  A = copy.A;
}

LinearColor::LinearColor(const Color& color)
{
  R = color.R / 255.0f;
  G = color.G / 255.0f;
  B = color.B / 255.0f;
  A = color.A / 255.0f;
}

Color
LinearColor::toColor() const
{
  return Color(static_cast<uint8>(Math::clamp(R * 255.0f, 0.0f, 255.0f)),
               static_cast<uint8>(Math::clamp(G * 255.0f, 0.0f, 255.0f)),
               static_cast<uint8>(Math::clamp(B * 255.0f, 0.0f, 255.0f)),
               static_cast<uint8>(Math::clamp(A * 255.0f, 0.0f, 255.0f)));
}
}
