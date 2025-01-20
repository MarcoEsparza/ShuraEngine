/*************************************************************/
/*
*  @file    shRadian.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
*  @brief   Class for angles which value is in radians.
*
*  Class for angles which value is in radians.
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
*  @brief Class for angles which value is in radians.
*/
class Radian
{
 public:
  /**
  *  @brief Default constructor.
  */
  Radian() = default;

  /**
  *  @brief Copy constructor.
  */
  Radian(const Radian&) = default;

  /**
  *  @brief Constructor with a float value, the value is assumed to be on
  *         radians, be aware of that.
  * 
  *  @param float f
  */
  explicit Radian(const float f) : m_radian(f) {}

  /**
  *  @brief Constructor for Radians with a Degree.
  * 
  *  @param Degree& d
  */
  Radian(const Degree& d);

  /**
  *  @brief Default destructor.
  */
  ~Radian() = default;

  /**
  *  @brief Return the value on Degrees.
  * 
  *  @return float
  */
  float
  getValueOnDegrees() const;

  /**
  *  @brief Return the value on Radians.
  *
  *  @return float
  */
  FORCEINLINE float
  getValueOnRadians() const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Assign other Radian value to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian&
  * 
  *  @return Degree&
  */
  Radian&
  operator=(const Radian&) = default;

  /**
  *  @brief Assign a Degree value to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Degree&
  */
  Radian&
  operator=(const Degree& d);

  /**
  *  @brief Assign a float value to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Radian&
  operator=(const float f);

  /**
  *  @brief Adds this value and another Radian value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator+(const Radian& r) const;

  /**
  *  @brief Return this.
  * 
  *  @return Radian&
  */
  FORCEINLINE const Radian&
  operator+() const;

  /**
  *  @brief Subtracts Adds this value and another Radian value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator-(const Radian& r) const;

  /**
  *  @brief Change this Radian value sign.
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator-() const;

  /**
  *  @brief Multiply this value and another Radian value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator*(const Radian& r) const;

  /**
  *  @brief Divide this value and another Radian value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator/(const Radian& r) const;

  /**
  *  @brief Adds this value and a Degree value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator+(const Degree& d) const;

  /**
  *  @brief Subtract this value and a Degree value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator-(const Degree& d) const;

  /**
  *  @brief Multiply this value and a Degree value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator*(const Degree& d) const;

  /**
  *  @brief Divide this value and a Degree value.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator/(const Degree& d) const;

  /**
  *  @brief Adds this value and a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator+(float f) const;

  /**
  *  @brief Subtracts this value and a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator-(float f) const;

  /**
  *  @brief Multiply this value and a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator*(float f) const;

  /**
  *  @brief Divide this value and a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian
  */
  FORCEINLINE Radian
  operator/(float f) const;

  /**
  *  @brief Adds another Radian to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& d
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator+=(const Radian& r);

  /**
  *  @brief Subtratcs another Radian to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator-=(const Radian& r);

  /**
  *  @brief Multiply another Radian to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator*=(const Radian& r);

  /**
  *  @brief Divide another Radian to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator/=(const Radian& r);

  /**
  *  @brief Adds a Degree to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator+=(const Degree& d);

  /**
  *  @brief Subtratcs a Degree to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator-=(const Degree& d);

  /**
  *  @brief Multiply a Degree to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator*=(const Degree& d);

  /**
  *  @brief Divide a Degree to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator/=(const Degree& d);

  /**
  *  @brief Adds a float to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator+=(const float f);

  /**
  *  @brief Subtracts a float to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator-=(const float f);

  /**
  *  @brief Multiply a float to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator*=(const float f);

  /**
  *  @brief Divide a float to this.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return Radian&
  */
  FORCEINLINE Radian&
  operator/=(const float f);

  /**
  *  @brief Compare if this is smaller than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const Radian& r) const;

  /**
  *  @brief Compare if this is smaller or equal than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const Radian& r) const;

  /**
  *  @brief Compare if this is equal than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const Radian& r) const;

  /**
  *  @brief Compare if this is different than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const Radian& r) const;

  /**
  *  @brief Compare if this is bigger or equal than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const Radian& r) const;

  /**
  *  @brief Compare if this is bigger than another Radian.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const Radian& r) const;

  /**
  *  @brief Compare if this is smaller than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const Degree& d) const;

  /**
  *  @brief Compare if this is smaller or equal than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const Degree& d) const;

  /**
  *  @brief Compare if this is equal than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const Degree& d) const;

  /**
  *  @brief Compare if this is different than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const Degree& d) const;

  /**
  *  @brief Compare if this is bigger or equal than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const Degree& d) const;

  /**
  *  @brief Compare if this is bigger than a Degree.
  * 
  *  @param lValue-Radian&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const Degree& d) const;

  /**
  *  @brief Compare if this is smaller than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const float f) const;

  /**
  *  @brief Compare if this is smaller or equal than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const float f) const;

  /**
  *  @brief Compare if this is equal than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const float f) const;

  /**
  *  @brief Compare if this is different than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const float f) const;

  /**
  *  @brief Compare if this is bigger or equal than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const float f) const;

  /**
  *  @brief Compare if this is bigger than a float.
  * 
  *  @param lValue-Radian&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const float f) const;

  /**
  *  @brief Adds a float and a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return Radian
  */
  FORCEINLINE friend Radian
  operator+(float lf, const Radian& rr)
  {
    return Radian(lf + rr.m_radian);
  }

  /**
  *  @brief Subtracts a float and a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return Radian
  */
  FORCEINLINE friend Radian
  operator-(float lf, const Radian& rr)
  {
    return Radian(lf - rr.m_radian);
  }

  /**
  *  @brief Multiply a float and a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return Radian
  */
  FORCEINLINE friend Radian
  operator*(float lf, const Radian& rr)
  {
    return Radian(lf * rr.m_radian);
  }

  /**
  *  @brief Divide a float and a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return Radian
  */
  FORCEINLINE friend Radian
  operator/(float lf, const Radian& rr)
  {
    return Radian(lf / rr.m_radian);
  }

  /**
  *  @brief Compare if a float is smaller than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator<(float lf, const Radian& rr)
  {
    return lf < rr.m_radian;
  }

  /**
  *  @brief Compare if a float is smaller or equal than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator<=(float lf, const Radian& rr)
  {
    return lf <= rr.m_radian;
  }

  /**
  *  @brief Compare if a float is equal than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator==(float lf, const Radian& rr)
  {
    return lf == rr.m_radian;
  }

  /**
  *  @brief Compare if a float is different than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator!=(float lf, const Radian& rr)
  {
    return lf != rr.m_radian;
  }

  /**
  *  @brief Compare if a float is bigger or equal than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator>=(float lf, const Radian& rr)
  {
    return lf >= rr.m_radian;
  }

  /**
  *  @brief Compare if a float is bigger than a Radian.
  * 
  *  @param lValue-float f
  *  @param rValue-Radian& rr
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator>(float lf, const Radian& rr)
  {
    return lf > rr.m_radian;
  }

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief Value on radians.
  */
  float m_radian = 0.0f;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE float
Radian::getValueOnRadians() const
{
  return m_radian;
}

FORCEINLINE Radian&
Radian::operator=(const float f)
{
  m_radian = f;
  return *this;
}

FORCEINLINE Radian
Radian::operator+(const Radian& r) const
{
  return Radian(m_radian + r.m_radian);
}

FORCEINLINE const Radian&
Radian::operator+() const
{
  return *this;
}

FORCEINLINE Radian
Radian::operator-(const Radian& r) const
{
  return Radian(m_radian - r.m_radian);
}

FORCEINLINE Radian
Radian::operator-() const
{
  return Radian(-m_radian);
}

FORCEINLINE Radian
Radian::operator*(const Radian& r) const
{
  return Radian(m_radian * r.m_radian);
}

FORCEINLINE Radian
Radian::operator/(const Radian& r) const
{
  return Radian(m_radian / r.m_radian);
}

FORCEINLINE Radian
Radian::operator+(float f) const
{
  return Radian(m_radian + f);
}

FORCEINLINE Radian
Radian::operator-(float f) const
{
  return Radian(m_radian - f);
}

FORCEINLINE Radian
Radian::operator*(float f) const
{
  return Radian(m_radian * f);
}

FORCEINLINE Radian
Radian::operator/(float f) const
{
  return Radian(m_radian / f);
}

FORCEINLINE Radian&
Radian::operator+=(const Radian& r)
{
  m_radian += r.m_radian;
  return *this;
}

FORCEINLINE Radian&
Radian::operator-=(const Radian& r)
{
  m_radian -= r.m_radian;
  return *this;
}

FORCEINLINE Radian&
Radian::operator*=(const Radian& r)
{
  m_radian *= r.m_radian;
  return *this;
}

FORCEINLINE Radian&
Radian::operator/=(const Radian& r)
{
  m_radian /= r.m_radian;
  return *this;
}

FORCEINLINE Radian&
Radian::operator+=(const float f)
{
  m_radian += f;
  return *this;
}

FORCEINLINE Radian&
Radian::operator-=(const float f)
{
  m_radian -= f;
  return *this;
}

FORCEINLINE Radian&
Radian::operator*=(const float f)
{
  m_radian *= f;
  return *this;
}

FORCEINLINE Radian&
Radian::operator/=(const float f)
{
  m_radian /= f;
  return *this;
}

FORCEINLINE bool
Radian::operator<(const Radian& r) const
{
  return m_radian < r.m_radian;
}

FORCEINLINE bool
Radian::operator<=(const Radian& r) const
{
  return m_radian <= r.m_radian;
}

FORCEINLINE bool
Radian::operator==(const Radian& r) const
{
  return m_radian == r.m_radian;
}

FORCEINLINE bool
Radian::operator!=(const Radian& r) const
{
  return m_radian != r.m_radian;
}

FORCEINLINE bool
Radian::operator>=(const Radian& r) const
{
  return m_radian >= r.m_radian;
}

FORCEINLINE bool
Radian::operator>(const Radian& r) const
{
  return m_radian > r.m_radian;
}

FORCEINLINE bool
Radian::operator<(const float f) const
{
  return m_radian < f;
}

FORCEINLINE bool
Radian::operator<=(const float f) const
{
  return m_radian <= f;
}

FORCEINLINE bool
Radian::operator==(const float f) const
{
  return m_radian == f;
}

FORCEINLINE bool
Radian::operator!=(const float f) const
{
  return m_radian != f;
}

FORCEINLINE bool
Radian::operator>=(const float f) const
{
  return m_radian >= f;
}

FORCEINLINE bool
Radian::operator>(const float f) const
{
  return m_radian > f;
}
}
