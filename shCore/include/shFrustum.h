/*****************************************************************************/
/*
*  @file    shFrustum.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/30
*  @brief   
*
*  
*
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shPlane.h"
#include "shMatrix4.h"

namespace shEngineSDK {
class SH_CORE_EXPORT Frustum
{
 public:
  Frustum() = default;
  ~Frustum() = default;

  void
  calculatePlanes(const Matrix4& view, const Matrix4& proj);

 private:
  Plane m_near;
  Plane m_far;
  Plane m_left;
  Plane m_right;
  Plane m_top;
  Plane m_bottom;
};
}
