/*************************************************************/
/*
*  @file    shLinearColor.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
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
    r = copy.r;
    g = copy.g;
    b = copy.b;
    a = copy.a;
  }

  LinearColor::LinearColor(const Color& color)
  {
    r = color.r / 255.0f;
    g = color.g / 255.0f;
    b = color.b / 255.0f;
    a = color.a / 255.0f;
  }

  Color
    LinearColor::toColor() const
  {
    return Color(static_cast<uint8>(Math::clamp(r * 255.0f, 0.0f, 255.0f)),
                 static_cast<uint8>(Math::clamp(g * 255.0f, 0.0f, 255.0f)),
                 static_cast<uint8>(Math::clamp(b * 255.0f, 0.0f, 255.0f)),
                 static_cast<uint8>(Math::clamp(a * 255.0f, 0.0f, 255.0f)));
  }
}
