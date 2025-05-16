/*****************************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/15
*  @brief   
*
*  
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesPhysics.h"
#include "shVector3.h"

namespace shEngineSDK {
struct CollisionInfo
{
  float m_penDist;
  float normal;
  Vector3 m_contactPoint;
};
}
