/*************************************************************/
/*
*  @file    shSPlatformMath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/14
*  @brief   Math class wrapper, suing the STD. compatible with Windows, Linux
*           and OSX.
*
*  Math class wrapper, suing the STD. compatible with Windows, Linux and OSX.
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPlatformMath.h"
#include "shBoxAAB.h"
#include "shBoxOBB.h"
#include "shSphere.h"

namespace shEngineSDK {
 /*************************************************************/
 /*
 *  Static variables
 */
 /*************************************************************/

const float PlatformMath::PI = acos(-1.0f);

const float PlatformMath::RAD2DEG = 180.0f / PI;

const float PlatformMath::DEG2RAD = PI / 180.0f;

const float PlatformMath::SMALL_NUMBER = (1.e-6f);

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

float
PlatformMath::sqrt(const float& _value)
{
  return std::sqrt(_value);
}

float
PlatformMath::sqrtf(const float& _value)
{
  return std::sqrtf(_value);
}

float
PlatformMath::pow(const float& _value, const float _vPow)
{
  return std::pow(_value, _vPow);
}

float
PlatformMath::min(const float& _x, const float& _y)
{
  return std::min(_x, _y);
}

float
PlatformMath::max(const float& _x, const float& _y)
{
  return std::max(_x, _y);
}

float
PlatformMath::fmod(const float& _value1, const float& _value2)
{
  return std::fmod(_value1, _value2);
}

float
PlatformMath::abs(const float& _value)
{
  return std::abs(_value);
}

/*************************************************************/
/*
*  Trigonometric functions
*/
/*************************************************************/

float
PlatformMath::cos(const float& _radian)
{
  return std::cos(_radian);
}

float
PlatformMath::sin(const float& _radian)
{
  return std::sin(_radian);
}

float
PlatformMath::tan(const float& _radian)
{
  return std::tan(_radian);
}

float
PlatformMath::cosh(const float& _radian)
{
  return std::cosh(_radian);
}

float
PlatformMath::sinh(const float& _radian)
{
  return std::sinh(_radian);
}

float
PlatformMath::tanh(const float& _radian)
{
  return std::tanh(_radian);
}

float
PlatformMath::acos(const float& _radian)
{
  return std::acos(_radian);
}

float
PlatformMath::asin(const float& _radian)
{
  return std::asin(_radian);
}

float
PlatformMath::atan(const float& _radian)
{
  return std::atan(_radian);
}

float
PlatformMath::acosh(const float& _radian)
{
  return std::acosh(_radian);
}

float
PlatformMath::asinh(const float& _radian)
{
  return std::asinh(_radian);
}

float
PlatformMath::atanh(const float& _radian)
{
  return std::atanh(_radian);
}

/*************************************************************/
/*
*  Figures
*/
/*************************************************************/

bool
PlatformMath::pointBoxIntersect(const Vector3& _point, const shBoxAAB& _box)
{
  return (_point.x >= _box.min.x &&
          _point.x <= _box.max.x &&
          _point.y >= _box.min.y &&
          _point.y <= _box.max.y &&
          _point.z >= _box.min.z &&
          _point.z <= _box.max.z);
}

bool
PlatformMath::pointBoxIntersect(const Vector3& _point, const shBoxOBB& _box)
{
  return false;
}

bool
PlatformMath::pointSphereIntersect(const Vector3& _point, const shSphere& _sph)
{
  float distance = sqrt(((_point.x - _sph.center.x) *
                        (_point.x - _sph.center.x)) +
                        ((_point.y - _sph.center.y) *
                        (_point.y - _sph.center.y)) +
                        ((_point.z - _sph.center.z) *
                        (_point.z - _sph.center.z)));

  return distance < _sph.radius;
}

bool
PlatformMath::pointCapsuleIntersect(const Vector3& _point, const shCapsule& _cap)
{
  return false;
}

bool
PlatformMath::pointRectIntersect(const Vector2& _point, const shRect& _r)
{
  return (_point.x >= _r.min.x &&
          _point.x <= _r.max.x &&
          _point.y >= _r.min.y &&
          _point.y <= _r.max.y);
}

bool
PlatformMath::pointPlaneIntersect(const Vector2& _point, const shPlane& _pln)
{
  return false;
}

bool
PlatformMath::boxBoxIntersect(const shBoxAAB& _box, const shBoxAAB& _box1)
{
  return (_box.min.x <= _box1.max.x &&
          _box.max.x >= _box1.min.x &&
          _box.min.y <= _box1.max.y &&
          _box.max.y >= _box1.min.y &&
          _box.min.z <= _box1.max.z &&
          _box.max.z >= _box1.min.z);
}

bool
PlatformMath::boxBoxIntersect(const shBoxOBB& _box, const shBoxOBB& _box1)
{
  return false;
}

bool
PlatformMath::boxBoxIntersect(const shBoxAAB& _boxA, const shBoxOBB& _boxO)
{
  return false;
}

bool
PlatformMath::boxCapsuleIntersect(const shBoxAAB& _box, const shCapsule& _cap)
{
  return false;
}

bool
PlatformMath::boxCapsuleIntersect(const shBoxOBB& _box, const shCapsule& _cap)
{
  return false;
}

bool
PlatformMath::boxPlaneIntersect(const shBoxAAB& _box, const shPlane& _pln)
{
  float d = _pln.normal.dot(_box.max - _box.min) - _pln.distance;

  float projRadius = _box.max.x - _box.min.x + abs(_pln.normal.x) +
                     _box.max.y - _box.min.y + abs(_pln.normal.y) +
                     _box.max.z - _box.min.z + abs(_pln.normal.z);

  return abs(d) <= projRadius;
}

bool
PlatformMath::boxPlaneIntersect(const shBoxOBB& _box, const shPlane& _pln)
{
  return false;
}

bool
PlatformMath::boxRectIntersect(const shBoxAAB& _box, const shRect& _r)
{
  return (_box.min.x <= _r.max.x &&
          _box.max.x >= _r.min.x &&
          _box.min.y <= _r.max.y &&
          _box.max.y >= _r.min.y);
}

bool
PlatformMath::boxRectIntersect(const shBoxOBB& _box, const shRect& _r)
{
  return false;
}

bool
PlatformMath::sphereSphereIntersect(const shSphere& _sph, const shSphere& _sph1)
{
  float distance = sqrt((_sph.center.x - _sph1.center.x) +
                        (_sph.center.y - _sph1.center.y) +
                        (_sph.center.z - _sph1.center.z));

  return (distance < (_sph.radius + _sph1.radius));
}

bool
PlatformMath::boxSphereIntersect(const shBoxAAB& _box, const shSphere& _sph)
{
  float x = max(_box.min.x, min(_sph.center.x, _box.max.x));
  float y = max(_box.min.y, min(_sph.center.y, _box.max.y));
  float z = max(_box.min.z, min(_sph.center.z, _box.max.z));

  float distance = sqrt(((x - _sph.center.x) * (x - _sph.center.x)) +
                        ((y - _sph.center.y) * (y - _sph.center.y)) +
                        ((z - _sph.center.z) * (z - _sph.center.z)));

  return distance < _sph.radius;
}

bool
PlatformMath::boxSphereIntersect(const shBoxOBB& _box, const shSphere& _sph)
{
  return false;
}

bool
PlatformMath::sphereCapsuleIntersect(const shSphere& _sph, const shCapsule& _cap)
{
  return false;
}

bool
PlatformMath::spherePlaneIntersect(const shSphere& _sph, const shPlane& _pln)
{
  return false;
}

bool
PlatformMath::sphereRectIntersect(const shSphere& _sph, const shRect& _r)
{
  float x = max(_r.min.x, min(_sph.center.x, _r.max.x));
  float y = max(_r.min.y, min(_sph.center.y, _r.max.y));

  float distance = sqrt(((x - _sph.center.x) * (x - _sph.center.x)) +
                        ((y - _sph.center.y) * (y - _sph.center.y)));

  return distance < _sph.radius;
}

bool
PlatformMath::capsuleCapsuleIntersect(const shCapsule& _cap, const shCapsule& _cap1)
{
  Vector3 c1Normal = (_cap.pointB - _cap.pointA).getNormalized();
  Vector3 c1EndOffset = c1Normal + _cap.radius;
  Vector3 c1A = _cap.pointA + c1EndOffset;
  Vector3 c1B = _cap.pointB + c1EndOffset;

  Vector3 c2Normal = (_cap1.pointB - _cap1.pointA).getNormalized();
  Vector3 c2EndOffset = c1Normal + _cap1.radius;
  Vector3 c2A = _cap1.pointA + c1EndOffset;
  Vector3 c2B = _cap1.pointB + c1EndOffset;

  Vector3 vec0 = c2A - c1A;
  Vector3 vec1 = c2B - c1A;
  Vector3 vec2 = c2A - c1B;
  Vector3 vec3 = c2B - c1B;

  float d0 = vec0.dot(vec0);
  float d1 = vec1.dot(vec1);
  float d2 = vec2.dot(vec2);
  float d3 = vec3.dot(vec3);

  Vector3 bestA = c1A;
  if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
    bestA = c1B;
  }

  Vector3 AB = c2B - c2A;
  float t = clamp((bestA - c2A).dot(AB) / AB.dot(AB), 0.0f, 1.0f);
  Vector3 bestB = c2A + (AB * t);

  AB = c1B - c1A;
  t = clamp((bestB - c1A).dot(AB) / AB.dot(AB), 0.0f, 1.0f);
  bestA = c1A + (AB * t);

  float dist = (bestA - bestB).mag();
  float depth = _cap.radius + _cap1.radius - dist;
  
  return depth > 0;
}

bool
PlatformMath::capsulePlaneIntersect(const shCapsule& _cap, const shPlane& _pln)
{
  return false;
}

bool
PlatformMath::capsuleRectIntersect(const shCapsule& _cap, const shRect& _r)
{
  return false;
}

bool
PlatformMath::planePlaneIntersect(const shPlane& _pln, const shPlane& _pln1)
{
  return false;
}

bool
PlatformMath::planeRectIntersect(const shPlane& _pln, const shRect& _r)
{
  return false;
}

bool
PlatformMath::rectRectIntersect(const shRect& _r, const shRect& _r1)
{
  return (_r.min.x <= _r1.max.x &&
          _r.max.x >= _r1.min.x &&
          _r.min.y <= _r1.max.y &&
          _r.max.y >= _r1.min.y);
}
}
