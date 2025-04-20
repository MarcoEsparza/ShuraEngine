/*************************************************************/
/*
*  @file    shLinearColor.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/19
*  @brief   LinearColor class.
*
*  LinearColor class.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shColor.h"

namespace shEngineSDK {
/**
*  @brief LinearColor class.
*/
class SH_UTILITY_EXPORT LinearColor
{
public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE LinearColor() = default;

  /**
  *  @brief Constructor to initialize values, A = 1.0f.
  *
  *  @param float R
  *  @param float G
  *  @param float B
  *  @param float A
  */
  FORCEINLINE LinearColor(const float R, const float G, const float B, const float A = 1.0f)
                          : r(R), g(G), b(B), a(A) {}

  /**
  *  @brief Copy constructor.
  *
  *  @param LinearColor& copy
  */
  LinearColor(const LinearColor& copy);

  /**
  *  @brief
  */
  LinearColor(const Color& color);

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~LinearColor() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
public:
  /**
  *  @brief Transform this to a Color
  *
  *  @return Color
  */
  Color
  toColor() const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
public:
  /**
  *  @brief Operator that returns this and other LinearColor values sum.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor
  */
  FORCEINLINE LinearColor
  operator+(const LinearColor& other) const;

  /**
  *  @brief Operator that returns this and other LinearColor values substraction.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor
  */
  FORCEINLINE LinearColor
  operator-(const LinearColor& other) const;

  /**
  *  @brief Operator that returns this and other LinearColor values multiplication.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor
  */
  FORCEINLINE LinearColor
  operator*(const LinearColor& other) const;

  /**
  *  @brief Operator that returns this values and a scalar multiplication.
  *
  *  @param lValue-LinearColor
  *  @param rValue-float
  *
  *  @return LinearColor
  */
  FORCEINLINE LinearColor
  operator*(const float scalar) const;

  /**
  *  @brief Operator that add other LinearColor values to this.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor&
  */
  FORCEINLINE LinearColor&
  operator+=(const LinearColor& other);

  /**
  *  @brief Operator that substract other LinearColor values to this.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor&
  */
  FORCEINLINE LinearColor&
  operator-=(const LinearColor& other);

  /**
  *  @brief Operator that multiply other LinearColor values to this.
  *
  *  @param lValue-LinearColor
  *  @param rValue-LinearColor
  *
  *  @return LinearColor&
  */
  FORCEINLINE LinearColor&
  operator*=(const LinearColor& other);

  /**
  *  @brief Operator that multiply a scalar to this.
  *
  *  @param lValue-LinearColor
  *  @param rValue-float
  *
  *  @return LinearColor&
  */
  FORCEINLINE LinearColor&
  operator*=(const float scalar);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
public:
  /**
  *  @brief Red value.
  */
  float r;

  /**
  *  @brief Green value.
  */
  float g;

  /**
  *  @brief Blue value.
  */
  float b;

  /**
  *  @brief Alpha value.
  */
  float a;

  /*************************************************************/
  /*
  *  Static variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Black = LinearColor(0.0f, 0.0f, 0.0f);
  */
  static const LinearColor BLACK;

  /**
  *  @brief White = LinearColor(1.0f, 1.0f, 1.0f);
  */
  static const LinearColor WHITE;

  /**
  *  @brief Red = LinearColor(1.0f, 0.0f, 0.0f);
  */
  static const LinearColor RED;

  /**
  *  @brief Green = LinearColor(0.0f, 1.0f, 0.0f);
  */
  static const LinearColor GREEN;

  /**
  *  @brief Blue = LinearColor(0.0f, 0.0f, 1.0f);
  */
  static const LinearColor BLUE;

  /**
  *  @brief Yellow
  */
  static const LinearColor YELLOW;

  /**
  *  @brief Gray
  */
  static const LinearColor GRAY;

  /**
  *  @brief Puple
  */
  static const LinearColor PURPLE;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE LinearColor
LinearColor::operator+(const LinearColor& other) const
{
  return LinearColor(r + other.r,
                     g + other.g,
                     b + other.b,
                     a + other.a);
}

FORCEINLINE LinearColor
LinearColor::operator-(const LinearColor& other) const
{
  return LinearColor(r - other.r,
                     g - other.g,
                     b - other.b,
                     a - other.a);
}

FORCEINLINE LinearColor
LinearColor::operator*(const LinearColor& other) const
{
  return LinearColor(r * other.r,
                     g * other.g,
                     b * other.b,
                     a * other.a);
}

FORCEINLINE LinearColor
LinearColor::operator*(const float scalar) const
{
  return LinearColor(r * scalar,
                     g * scalar,
                     b * scalar,
                     a * scalar);
}

FORCEINLINE LinearColor&
LinearColor::operator+=(const LinearColor& other)
{
  r += other.r;
  g += other.g;
  b += other.b;
  a += other.a;

  return *this;
}

FORCEINLINE LinearColor&
LinearColor::operator-=(const LinearColor& other)
{
  r -= other.r;
  g -= other.g;
  b -= other.b;
  a -= other.a;

  return *this;
}

FORCEINLINE LinearColor&
LinearColor::operator*=(const LinearColor& other)
{
  r *= other.r;
  g *= other.g;
  b *= other.b;
  a *= other.a;

  return *this;
}

FORCEINLINE LinearColor&
LinearColor::operator*=(const float scalar)
{
  r *= scalar;
  g *= scalar;
  b *= scalar;
  a *= scalar;

  return *this;
}
}
