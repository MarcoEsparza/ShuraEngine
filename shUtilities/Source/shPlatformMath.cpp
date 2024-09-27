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
  return (_point.x >= _box.min.x &&
          _point.x <= _box.max.x &&
          _point.y >= _box.min.y &&
          _point.y <= _box.max.y &&
          _point.z >= _box.min.z &&
          _point.z <= _box.max.z);
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
  float dx = sqrt((_point.x - _cap.center.x) * (_point.x - _cap.center.x));
  float dy = sqrt((_point.y - _cap.center.y) * (_point.y - _cap.center.y));
  float dz = sqrt((_point.z - _cap.center.z) * (_point.z - _cap.center.z));

  return ((dx < _cap.radius) &&
          (dy < _cap.height) &&
          (dz < _cap.radius));
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
  /*return (_point.x >= _pln.min.x &&
          _point.x <= _pln.max.x &&
          _point.y >= _pln.min.y &&
          _point.y <= _pln.max.y);*/

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
  return (_box.min.x <= _box1.max.x &&
          _box.max.x >= _box1.min.x &&
          _box.min.y <= _box1.max.y &&
          _box.max.y >= _box1.min.y &&
          _box.min.z <= _box1.max.z &&
          _box.max.z >= _box1.min.z);
}

bool
PlatformMath::boxBoxIntersect(const shBoxAAB& _boxA, const shBoxOBB& _boxO)
{
  return (_boxA.min.x <= _boxO.max.x &&
          _boxA.max.x >= _boxO.min.x &&
          _boxA.min.y <= _boxO.max.y &&
          _boxA.max.y >= _boxO.min.y &&
          _boxA.min.z <= _boxO.max.z &&
          _boxA.max.z >= _boxO.min.z);
}

bool
PlatformMath::boxCapsuleIntersect(const shBoxAAB& _box, const shCapsule& _cap)
{
  float x = max(_box.min.x, min(_cap.center.x, _box.max.x));
  float y = max(_box.min.y, min(_cap.center.y, _box.max.y));
  float z = max(_box.min.z, min(_cap.center.z, _box.max.z));

  float dx = sqrt((x - _cap.center.x) * (x - _cap.center.x));
  float dy = sqrt((y - _cap.center.y) * (y - _cap.center.y));
  float dz = sqrt((z - _cap.center.z) * (z - _cap.center.z));

  return ((dx < _cap.radius) &&
          (dy < _cap.height) &&
          (dz < _cap.radius));
}

bool
PlatformMath::boxCapsuleIntersect(const shBoxOBB& _box, const shCapsule& _cap)
{
  float x = max(_box.min.x, min(_cap.center.x, _box.max.x));
  float y = max(_box.min.y, min(_cap.center.y, _box.max.y));
  float z = max(_box.min.z, min(_cap.center.z, _box.max.z));

  float dx = sqrt((x - _cap.center.x) * (x - _cap.center.x));
  float dy = sqrt((y - _cap.center.y) * (y - _cap.center.y));
  float dz = sqrt((z - _cap.center.z) * (z - _cap.center.z));

  return ((dx < _cap.radius) &&
          (dy < _cap.height) &&
          (dz < _cap.radius));
}

bool
PlatformMath::boxPlaneIntersect(const shBoxAAB& _box, const shPlane& _pln)
{
  /*return (_box.min.x <= _pln.max.x &&
          _box.max.x >= _pln.min.x &&
          _box.min.z <= _pln.max.y &&
          _box.max.z >= _pln.min.y);*/

  return false;
}

bool
PlatformMath::boxPlaneIntersect(const shBoxOBB& _box, const shPlane& _pln)
{
  /*return (_box.min.x <= _pln.max.x &&
          _box.max.x >= _pln.min.x &&
          _box.min.z <= _pln.max.y &&
          _box.max.z >= _pln.min.y);*/

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
  return (_box.min.x <= _r.max.x &&
          _box.max.x >= _r.min.x &&
          _box.min.y <= _r.max.y &&
          _box.max.y >= _r.min.y);
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
  float x = max(_box.min.x, min(_sph.center.x, _box.max.x));
  float y = max(_box.min.y, min(_sph.center.y, _box.max.y));
  float z = max(_box.min.z, min(_sph.center.z, _box.max.z));

  float distance = sqrt(((x - _sph.center.x) * (x - _sph.center.x)) +
                        ((y - _sph.center.y) * (y - _sph.center.y)) +
                        ((z - _sph.center.z) * (z - _sph.center.z)));

  return distance < _sph.radius;
}

bool
PlatformMath::sphereCapsuleIntersect(const shSphere& _sph, const shCapsule& _cap)
{
  float x = max(_sph.center.x, min(_cap.center.x, _sph.center.x));
  float y = max(_sph.center.y, min(_cap.center.y, _sph.center.y));
  float z = max(_sph.center.z, min(_cap.center.z, _sph.center.z));

  float dx = sqrt((x - _cap.center.x) * (x - _cap.center.x));
  float dy = sqrt((y - _cap.center.y) * (y - _cap.center.y));
  float dz = sqrt((z - _cap.center.z) * (z - _cap.center.z));

  return (((dx + _sph.radius) < _cap.radius) &&
          ((dy + _sph.radius) < _cap.height) &&
          ((dz + _sph.radius) < _cap.radius));
}

bool
PlatformMath::spherePlaneIntersect(const shSphere& _sph, const shPlane& _pln)
{
  /*float x = max(_pln.min.x, min(_sph.center.x, _pln.max.x));
  float z = max(_pln.min.y, min(_sph.center.z, _pln.max.y));

  float distance = sqrt(((x - _sph.center.x) * (x - _sph.center.x)) +
                        ((z - _sph.center.z) * (z - _sph.center.z)));

  return distance < _sph.radius;*/

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
  float x = max(_cap.center.x, min(_cap1.center.x, _cap.center.x));
  float y = max(_cap.center.y, min(_cap1.center.y, _cap.center.y));
  float z = max(_cap.center.z, min(_cap1.center.z, _cap.center.z));

  float dx = sqrt((x - _cap1.center.x) * (x - _cap1.center.x));
  float dy = sqrt((y - _cap1.center.y) * (y - _cap1.center.y));
  float dz = sqrt((z - _cap1.center.z) * (z - _cap1.center.z));

  return ((dx < _cap1.radius) &&
          (dy < _cap1.height) &&
          (dz < _cap1.radius));
}

bool
PlatformMath::capsulePlaneIntersect(const shCapsule& _cap, const shPlane& _pln)
{
  /*float x = max(_pln.min.x, min(_cap.center.x, _pln.max.x));
  float z = max(_pln.min.y, min(_cap.center.z, _pln.max.y));

  float dx = sqrt((x - _cap.center.x) * (x - _cap.center.x));
  float dz = sqrt((z - _cap.center.z) * (z - _cap.center.z));

  return ((dx < _cap.radius) &&
          (dz < _cap.radius));*/

  return false;
}

bool
PlatformMath::capsuleRectIntersect(const shCapsule& _cap, const shRect& _r)
{
  float x = max(_r.min.x, min(_cap.center.x, _r.max.x));
  float y = max(_r.min.y, min(_cap.center.z, _r.max.y));

  float dx = sqrt((x - _cap.center.x) * (x - _cap.center.x));
  float dy = sqrt((y - _cap.center.y) * (y - _cap.center.z));

  return ((dx < _cap.radius) &&
          (dy < _cap.height));
}

bool
PlatformMath::planePlaneIntersect(const shPlane& _pln, const shPlane& _pln1)
{
  /*return (_pln.min.x <= _pln1.max.x &&
          _pln.max.x >= _pln1.min.x &&
          _pln.min.y <= _pln1.max.y &&
          _pln.max.y >= _pln1.min.y);*/

  return false;
}

bool
PlatformMath::planeRectIntersect(const shPlane& _pln, const shRect& _r)
{
  /*return (_pln.min.x <= _r.max.x &&
          _pln.max.x >= _r.min.x &&
          _pln.min.y <= _r.max.y &&
          _pln.max.y >= _r.min.y);*/

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
