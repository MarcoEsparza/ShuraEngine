/*************************************************************/
/*
*  @file    shRadian.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
*  @brief   Class for angles which value is in radians.
*
*  Class for angles which value is in radians.
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
  *this = d;
}

float
Radian::getValueOnDegrees() const
{
  return m_radian * Math::RAD2DEG;
}

Radian&
Radian::operator=(const Degree& d)
{
  m_radian = d.getValueOnRadians();
  return *this;
}

Radian
Radian::operator+(const Degree& d) const
{
  return Radian(m_radian + d.getValueOnRadians());
}

Radian
Radian::operator-(const Degree& d) const
{
  return Radian(m_radian - d.getValueOnRadians());
}

Radian
Radian::operator*(const Degree& d) const
{
  return Radian(m_radian * d.getValueOnRadians());
}

Radian
Radian::operator/(const Degree& d) const
{
  return Radian(m_radian / d.getValueOnRadians());
}

Radian&
Radian::operator+=(const Degree& d)
{
  m_radian += d.getValueOnRadians();
  return *this;
}

Radian&
Radian::operator-=(const Degree& d)
{
  m_radian -= d.getValueOnRadians();
  return *this;
}

Radian&
Radian::operator*=(const Degree& d)
{
  m_radian *= d.getValueOnRadians();
  return *this;
}

Radian&
Radian::operator/=(const Degree& d)
{
  m_radian /= d.getValueOnRadians();
  return *this;
}

bool
Radian::operator<(const Degree& d) const
{
  return m_radian < d.getValueOnRadians();
}

bool
Radian::operator<=(const Degree& d) const
{
  return m_radian <= d.getValueOnRadians();
}

bool
Radian::operator==(const Degree& d) const
{
  return m_radian == d.getValueOnRadians();
}

bool
Radian::operator!=(const Degree& d) const
{
  return m_radian != d.getValueOnRadians();
}

bool
Radian::operator>=(const Degree& d) const
{
  return m_radian >= d.getValueOnRadians();
}

bool
Radian::operator>(const Degree& d) const
{
  return m_radian > d.getValueOnRadians();
}
}
