/*****************************************************************************/
/*
*  @file    shRect.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for rectangle figure.
*
*  Math class for rectangle figure.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRect.h"

namespace shEngineSDK {

Rect::Rect(const Vector2& pos, const Vector2& size)
{
  min = pos;
  max = min + size;
}

/*****************************************************************************/
/*
*  Functions
*/
/*****************************************************************************/

Array<Vector3, 4> Rect::getVertices() const
{
  return { Vector3(min.x, min.y, 0.0f),
           Vector3(max.x, min.y, 0.0f),
           Vector3(max.x, max.y, 0.0f),
           Vector3(min.x, max.y, 0.0f) };
}

bool
Rect::lineIntersect(const Vector2& point1, const Vector2& point2) const
{
  return (pointIntersect(point1) || pointIntersect(point2) ||
          lineIntersectLine(point1, point2, min, Vector2(max.x, min.y)) ||
          lineIntersectLine(point1, point2, Vector2(min.x, max.y), max) ||
          lineIntersectLine(point1, point2, min, Vector2(min.x, max.y)) ||
          lineIntersectLine(point1, point2, Vector2(max.x, min.y), max));
}

bool
Rect::pointIntersect(const Vector2& point) const
{
  return (point.x >= min.x &&
          point.x <= max.x &&
          point.y >= min.y &&
          point.y <= max.y);
}

bool
Rect::lineIntersectLine(const Vector2& point1,
                          const Vector2& point2,
                          const Vector2& point3,
                          const Vector2& point4) const
{
  const float denom = (point4.y - point3.y) *
                      (point2.x - point1.x) -
                      (point4.x - point3.x) *
                      (point2.y - point1.y);

  if (denom == 0) {
    return false;
  }

  const float invDenom = 1 / denom;

  const float ua = ((point4.x - point3.x) *
                    (point1.y - point3.y) -
                    (point4.y - point3.y) *
                    (point1.x - point3.x)) * invDenom;
  const float ub = ((point2.x - point1.x) *
                    (point1.y - point3.y) -
                    (point2.y - point1.y) *
                    (point1.x - point3.x)) * invDenom;

  return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}
}
