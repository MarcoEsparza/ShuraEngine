/*****************************************************************************/
/*
*  @file    shBivector.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/11
*  @brief
*
*
*
*  @bug     No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shBivector.h"
#include "shMath.h"
#include "shVector3.h"

namespace shEngineSDK {
float
Bivector::magnitude() const
{
  return Math::sqrt(yz * yz + zx * zx + xy * xy);
}

Bivector
Bivector::wedge(const Vector3& u, const Vector3& v)
{
  return Bivector(u.x * v.y - u.y * v.x,   // XY
                  u.x * v.z - u.z * v.x,   // XZ
                  u.y * v.z - u.z * v.y);  // YZ
}
}
