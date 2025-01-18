/*************************************************************/
/*
*  @file    shDegree.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/17
*  @brief
*
*
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shDegree.h"
#include "shRadian.h"
#include "shMath.h"

namespace shEngineSDK {
Degree::Degree(const Radian& r)
{
  m_degree = r.getValueOnDegrees();
}

float
Degree::getValueOnRadians() const
{
  return m_degree * Math::DEG2RAD;
}
}
