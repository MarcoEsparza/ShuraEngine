#include "Math/shVector2.h"

Vector2::Vector2()
{
  x = 0;
  y = 0;
}

float
Vector2::Dot(Vector2& vec)
{
  return x * vec.x + y * vec.y;
}

float
Vector2::Mag() const
{
  return sqrtf(x * x + y * y);
}

Vector2
Vector2::Cross(Vector2& other)
{
  float scalar = Dot(other) / (other.Mag() * other.Mag());
  return Vector2(other.x * scalar, other.y * scalar);
}
