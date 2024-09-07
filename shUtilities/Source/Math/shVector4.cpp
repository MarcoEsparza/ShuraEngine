/*************************************************************/
/*
*  @file    shVector4.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/06
*  @brief   Vector4
*
*  float Vector4
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "Math/shVector4.h"
#include "Math/shPlatformMath.h"

namespace shEngineSDK {
Vector4::Vector4()
{
  x = 0;
  y = 0;
  z = 0;
  w = 0;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


float
Vector4::mag() const
{
  float lenght = (x * x) + (y * y) + (z * z) + (w * w);

  return PlatformMath::sqrt(lenght);
}

void
Vector4::normalize()
{
  float magnitude = mag();
  if (magnitude != 0.0f)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    w /= magnitude;
  }
  else
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }
}

Vector4
Vector4::lerp(const Vector4& vecA,
              const Vector4& vecB,
              float time) const
{
  return vecA + (vecB - vecA) * time;
}
}
