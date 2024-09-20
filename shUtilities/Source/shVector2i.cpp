/*************************************************************/
/*
*  @file    shVector2i.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
<<<<<<< Updated upstream
*  @date    2024/09/18
=======
*  @date    2024/09/20
>>>>>>> Stashed changes
*  @brief   Vector2 with integers
*
*  Vector2 with integers
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shVector2i.h"
#include "shMath.h"

namespace shEngineSDK {
Vector2i::Vector2i(const Vector2i& _other)
{
  x = _other.x;
  y = _other.y;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/


int32
Vector2i::dot(const Vector2i& _other) const
{
  return (x * _other.x + y * _other.y);
}

float
Vector2i::mag() const
{
  return Math::sqrtf(static_cast<float>(x * x + y * y));
}

<<<<<<< Updated upstream
float
Vector2i::scalarProjection(const Vector2i& _other) const
{
  return dot(_other) / _other.mag();
}

Vector2i
Vector2i::vectorProjection(const Vector2i& _other) const
{
  float scalar = dot(_other) / (_other.mag() * _other.mag());
  return Vector2i(_other.x * scalar, _other.y * scalar);
}

=======
>>>>>>> Stashed changes
Vector2i
Vector2i::lerp(const Vector2i& _other,
               const int32& _time) const
{
  return (*this + (_other + *this) * _time);
}
}
