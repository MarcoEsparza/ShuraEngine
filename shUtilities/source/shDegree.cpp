/*****************************************************************************/
/*
*  @file    shDegree.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
*  @brief   Class for angles which value is in degrees.
*
*  Class for angles which value is in degrees.
*
* @bug      No bug known
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDegree.h"
#include "shRadian.h"
#include "shMath.h"

namespace shEngineSDK {
Degree::Degree(const Radian& r)
{
  *this = r;
}

float
Degree::getValueOnRadians() const
{
  return m_degree * Math::DEG2RAD;
}

Degree&
Degree::operator=(const Radian& r)
{
  m_degree = r.getValueOnDegrees();
  return *this;
}

Degree
Degree::operator+(const Radian& r) const
{
  return Degree(m_degree + r.getValueOnDegrees());
}

Degree
Degree::operator-(const Radian& r) const
{
  return Degree(m_degree - r.getValueOnDegrees());
}

Degree
Degree::operator*(const Radian& r) const
{
  return Degree(m_degree * r.getValueOnDegrees());
}

Degree
Degree::operator/(const Radian& r) const
{
  return Degree(m_degree / r.getValueOnDegrees());
}

Degree&
Degree::operator+=(const Radian& r)
{
  m_degree += r.getValueOnDegrees();
  return *this;
}

Degree&
Degree::operator-=(const Radian& r)
{
  m_degree -= r.getValueOnDegrees();
  return *this;
}

Degree&
Degree::operator*=(const Radian& r)
{
  m_degree *= r.getValueOnDegrees();
  return *this;
}

Degree&
Degree::operator/=(const Radian& r)
{
  m_degree /= r.getValueOnDegrees();
  return *this;
}

bool
Degree::operator<(const Radian& r) const
{
  return m_degree < r.getValueOnDegrees();
}

bool
Degree::operator<=(const Radian& r) const
{
  return m_degree <= r.getValueOnDegrees();
}

bool
Degree::operator==(const Radian& r) const
{
  return m_degree == r.getValueOnDegrees();
}

bool
Degree::operator!=(const Radian& r) const
{
  return m_degree != r.getValueOnDegrees();
}

bool
Degree::operator>=(const Radian& r) const
{
  return m_degree >= r.getValueOnDegrees();
}

bool
Degree::operator>(const Radian& r) const
{
  return m_degree > r.getValueOnDegrees();
}
}
