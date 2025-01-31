/*************************************************************/
/*
*  @file    shDegree.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/19
*  @brief   Class for angles which value is in degrees.
*
*  Class for angles which value is in degrees.
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
*  @brief Class for angles which value is in degrees.
*/
class SH_UTILITY_EXPORT Degree
{
 public:
  /**
  *  @brief Default constructor.
  */
  Degree() = default;

  /**
  *  @brief Copy constructor.
  * 
  *  @param Degree&
  */
  Degree(const Degree&) = default;

  /**
  *  @brief Constructor with a float value, the value is assumed to be on
  *         degrees, be aware of that.
  * 
  *  @param float f
  */
  explicit Degree(const float f) : m_degree(f) {}

  /**
  *  @brief Constructor for degrees with a Radian.
  * 
  *  @param Radian& r
  */
  Degree(const Radian& r);

  /**
  *  @brief Default destructor.
  */
  ~Degree() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Return the value in radians.
  * 
  *  @return float
  */
  float
  getValueOnRadians() const;

  /**
  *  @brief Return the value on degrees.
  * 
  *  @return float
  */
  FORCEINLINE float
  getValueOnDegrees() const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Assign other Degree value to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree&
  * 
  *  @return Degree&
  */
  Degree&
  operator=(const Degree&) = default;

  /**
  *  @brief Assign a Radian value to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree&
  */
  Degree&
  operator=(const Radian& r);

  /**
  *  @brief Assign a float value to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator=(const float f);

  /**
  *  @brief Adds this value and another Degree value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator+(const Degree& d) const;

  /**
  *  @brief Return this.
  * 
  *  @return Degree&
  */
  FORCEINLINE const Degree&
  operator+() const;

  /**
  *  @brief Subtracts Adds this value and another Degree value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator-(const Degree& d) const;

  /**
  *  @brief Change this Degree value sign.
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator-() const;

  /**
  *  @brief Multiply this value and another Degree value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator*(const Degree& d) const;

  /**
  *  @brief Divide this value and another Degree value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator/(const Degree& d) const;

  /**
  *  @brief Adds this value and a Radian value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator+(const Radian& r) const;

  /**
  *  @brief Subtract this value and a Radian value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator-(const Radian& r) const;

  /**
  *  @brief Multiply this value and a Radian value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator*(const Radian& r) const;

  /**
  *  @brief Divide this value and a Radian value.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator/(const Radian& r) const;

  /**
  *  @brief Adds this value and a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator+(float f) const;

  /**
  *  @brief Subtracts this value and a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator-(float f) const;

  /**
  *  @brief Multiply this value and a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator*(float f) const;

  /**
  *  @brief Divide this value and a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree
  */
  FORCEINLINE Degree
  operator/(float f) const;

  /**
  *  @brief Adds another Degree to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator+=(const Degree& d);

  /**
  *  @brief Subtratcs another Degree to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator-=(const Degree& d);

  /**
  *  @brief Multiply another Degree to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator*=(const Degree& d);

  /**
  *  @brief Divide another Degree to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator/=(const Degree& d);

  /**
  *  @brief Adds a Radian to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator+=(const Radian& r);

  /**
  *  @brief Subtratcs a Radian to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator-=(const Radian& r);

  /**
  *  @brief Multiply a Radian to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator*=(const Radian& r);

  /**
  *  @brief Divide a Radian to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator/=(const Radian& r);

  /**
  *  @brief Adds a float to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator+=(const float f);

  /**
  *  @brief Subtracts a float to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator-=(const float f);

  /**
  *  @brief Multiply a float to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator*=(const float f);

  /**
  *  @brief Divide a float to this.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return Degree&
  */
  FORCEINLINE Degree&
  operator/=(const float f);

  /**
  *  @brief Compare if this is smaller than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const Degree& d) const;

  /**
  *  @brief Compare if this is smaller or equal than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const Degree& d) const;

  /**
  *  @brief Compare if this is equal than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const Degree& d) const;

  /**
  *  @brief Compare if this is different than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const Degree& d) const;

  /**
  *  @brief Compare if this is bigger or equal than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const Degree& d) const;

  /**
  *  @brief Compare if this is bigger than another Degree.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Degree& d
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const Degree& d) const;

  /**
  *  @brief Compare if this is smaller than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const Radian& r) const;

  /**
  *  @brief Compare if this is smaller or equal than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const Radian& r) const;

  /**
  *  @brief Compare if this is equal than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const Radian& r) const;

  /**
  *  @brief Compare if this is different than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const Radian& r) const;

  /**
  *  @brief Compare if this is bigger or equal than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const Radian& r) const;

  /**
  *  @brief Compare if this is bigger than a Radian.
  * 
  *  @param lValue-Degree&
  *  @param rValue-Radian& r
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const Radian& r) const;

  /**
  *  @brief Compare if this is smaller than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<(const float f) const;

  /**
  *  @brief Compare if this is smaller or equal than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator<=(const float f) const;

  /**
  *  @brief Compare if this is equal than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator==(const float f) const;

  /**
  *  @brief Compare if this is different than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator!=(const float f) const;

  /**
  *  @brief Compare if this is bigger or equal than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>=(const float f) const;

  /**
  *  @brief Compare if this is bigger than a float.
  * 
  *  @param lValue-Degree&
  *  @param rValue-float f
  * 
  *  @return bool
  */
  FORCEINLINE bool
  operator>(const float f) const;

  /**
  *  @brief Adds a float and a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return Degree
  */
  FORCEINLINE friend Degree
  operator+(float lf, const Degree& rd)
  {
    return Degree(lf + rd.m_degree);
  }

  /**
  *  @brief Subtracts a float and a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return Degree
  */
  FORCEINLINE friend Degree
  operator-(float lf, const Degree& rd)
  {
    return Degree(lf - rd.m_degree);
  }

  /**
  *  @brief Multiply a float and a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return Degree
  */
  FORCEINLINE friend Degree
  operator*(float lf, const Degree& rd)
  {
    return Degree(lf * rd.m_degree);
  }

  /**
  *  @brief Divide a float and a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return Degree
  */
  FORCEINLINE friend Degree
  operator/(float lf, const Degree& rd)
  {
    return Degree(lf / rd.m_degree);
  }

  /**
  *  @brief Compare if a float is smaller than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator<(float lf, const Degree& rd)
  {
    return lf < rd.m_degree;
  }

  /**
  *  @brief Compare if a float is smaller or equal than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator<=(float lf, const Degree& rd)
  {
    return lf <= rd.m_degree;
  }

  /**
  *  @brief Compare if a float is equal than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator==(float lf, const Degree& rd)
  {
    return lf == rd.m_degree;
  }

  /**
  *  @brief Compare if a float is different than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator!=(float lf, const Degree& rd)
  {
    return lf != rd.m_degree;
  }

  /**
  *  @brief Compare if a float is bigger or equal than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator>=(float lf, const Degree& rd)
  {
    return lf >= rd.m_degree;
  }

  /**
  *  @brief Compare if a float is bigger than a Degree.
  * 
  *  @param lValue-float f
  *  @param rValue-Degree&
  * 
  *  @return bool
  */
  FORCEINLINE friend bool
  operator>(float lf, const Degree& rd)
  {
    return lf > rd.m_degree;
  }

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief Value on degrees.
  */
  float m_degree = 0.0f;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE float
Degree::getValueOnDegrees() const
{
  return m_degree;
}

FORCEINLINE Degree&
Degree::operator=(const float f)
{
  m_degree = f;
  return *this;
}

FORCEINLINE Degree
Degree::operator+(const Degree& d) const
{
  return Degree(m_degree + d.m_degree);
}

FORCEINLINE const Degree&
Degree::operator+() const
{
  return *this;
}

FORCEINLINE Degree
Degree::operator-(const Degree& d) const
{
  return Degree(m_degree - d.m_degree);
}

FORCEINLINE Degree
Degree::operator-() const
{
  return Degree(-m_degree);
}

FORCEINLINE Degree
Degree::operator*(const Degree& d) const
{
  return Degree(m_degree * d.m_degree);
}

FORCEINLINE Degree
Degree::operator/(const Degree& d) const
{
  return Degree(m_degree / d.m_degree);
}

FORCEINLINE Degree
Degree::operator+(float f) const
{
  return Degree(m_degree + f);
}

FORCEINLINE Degree
Degree::operator-(float f) const
{
  return Degree(m_degree + f);
}

FORCEINLINE Degree
Degree::operator*(float f) const
{
  return Degree(m_degree + f);
}

FORCEINLINE Degree
Degree::operator/(float f) const
{
  return Degree(m_degree + f);
}

FORCEINLINE Degree&
Degree::operator+=(const Degree& d)
{
  m_degree += d.m_degree;
  return *this;
}

FORCEINLINE Degree&
Degree::operator-=(const Degree& d)
{
  m_degree -= d.m_degree;
  return *this;
}

FORCEINLINE Degree&
Degree::operator*=(const Degree& d)
{
  m_degree *= d.m_degree;
  return *this;
}

FORCEINLINE Degree&
Degree::operator/=(const Degree& d)
{
  m_degree /= d.m_degree;
  return *this;
}

FORCEINLINE Degree&
Degree::operator+=(const float f)
{
  m_degree += f;
  return *this;
}

FORCEINLINE Degree&
Degree::operator-=(const float f)
{
  m_degree -= f;
  return *this;
}

FORCEINLINE Degree&
Degree::operator*=(const float f)
{
  m_degree *= f;
  return *this;
}

FORCEINLINE Degree&
Degree::operator/=(const float f)
{
  m_degree /= f;
  return *this;
}

FORCEINLINE bool
Degree::operator<(const Degree& d) const
{
  return m_degree < d.m_degree;
}

FORCEINLINE bool
Degree::operator<=(const Degree& d) const
{
  return m_degree <= d.m_degree;
}

FORCEINLINE bool
Degree::operator==(const Degree& d) const
{
  return m_degree == d.m_degree;
}

FORCEINLINE bool
Degree::operator!=(const Degree& d) const
{
  return m_degree != d.m_degree;
}

FORCEINLINE bool
Degree::operator>=(const Degree& d) const
{
  return m_degree >= d.m_degree;
}

FORCEINLINE bool
Degree::operator>(const Degree& d) const
{
  return m_degree > d.m_degree;
}

FORCEINLINE bool
Degree::operator<(const float f) const
{
  return m_degree < f;
}

FORCEINLINE bool
Degree::operator<=(const float f) const
{
  return m_degree <= f;
}

FORCEINLINE bool
Degree::operator==(const float f) const
{
  return m_degree == f;
}

FORCEINLINE bool
Degree::operator!=(const float f) const
{
  return m_degree != f;
}

FORCEINLINE bool
Degree::operator>=(const float f) const
{
  return m_degree >= f;
}

FORCEINLINE bool
Degree::operator>(const float f) const
{
  return m_degree > f;
}
}
