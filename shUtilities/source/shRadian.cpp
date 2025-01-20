/*************************************************************/
/*
*  @file    shRadian.cpp
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
#include "shRadian.h"
#include "shDegree.h"
#include "shMath.h"

namespace shEngineSDK {
Radian::Radian(const Degree& d)
{
  
}

float
Radian::getValueOnDegrees() const
{
  return m_radian * Math::RAD2DEG;
}
}
