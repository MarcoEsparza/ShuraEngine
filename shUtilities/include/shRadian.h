/*************************************************************/
/*
*  @file    shRadian.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/17
*  @brief   
*
*  
*
* @bug      No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
class Degree;

/**
*  @brief
*/
class Radian
{
 public:
  Radian() = default;
  Radian(const Radian&) = default;
  explicit Radian(const float r) : m_radian(r) {}
  Radian(const Degree& d);
  ~Radian() = default;

  float
  getValueOnDegrees() const;

 private:
   float m_radian = 0.0f;
};
}
