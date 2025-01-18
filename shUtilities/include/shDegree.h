/*************************************************************/
/*
*  @file    shDegree.h
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
class Radian;

/**
*  @brief
*/
class Degree
{
 public:
  Degree() = default;
  Degree(const Degree&) = default;
  explicit Degree(const float d) : m_degree(d) {}
  Degree(const Radian& r);
  ~Degree() = default;

  float
  getValueOnRadians() const;

  FORCEINLINE float
  getValueOnDegrees() const;

  Degree&
  operator=(const Degree&) = default;

  FORCEINLINE Degree&
  operator=(const Radian& r);

  FORCEINLINE Degree&
  operator=(const float f);

 private:
  float m_degree = 0.0f;
};

FORCEINLINE float
Degree::getValueOnDegrees() const
{
  return m_degree;
}

FORCEINLINE Degree&
Degree::operator=(const Radian& r)
{

}

FORCEINLINE Degree&
Degree::operator=(const float f)
{
  m_degree = f;
  return *this;
}
}
