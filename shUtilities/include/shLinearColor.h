/*************************************************************/
/*
*  @file    shLinearColor.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
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
  class LinearColor
  {
  public:
    /**
    *  @brief Default constructor.
    */
    FORCEINLINE LinearColor() = default;

    /**
    *  @brief Constructor to initialize values, A = 1.0f.
    *
    *  @param float r
    *  @param float g
    *  @param float b
    *  @param float a
    */
    FORCEINLINE LinearColor(const float r, const float g, const float b, const float a = 1.0f)
                            : R(r), G(g), B(b), A(a) {}

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
    float R;

    /**
    *  @brief Green value.
    */
    float G;

    /**
    *  @brief Blue value.
    */
    float B;

    /**
    *  @brief Alpha value.
    */
    float A;
  };

  /*************************************************************/
  /*
  *  Implementations
  */
  /*************************************************************/

  FORCEINLINE LinearColor
    LinearColor::operator+(const LinearColor& other) const
  {
    return LinearColor(R + other.R,
      G + other.G,
      B + other.B,
      A + other.A);
  }

  FORCEINLINE LinearColor
    LinearColor::operator-(const LinearColor& other) const
  {
    return LinearColor(R - other.R,
      G - other.G,
      B - other.B,
      A - other.A);
  }

  FORCEINLINE LinearColor
    LinearColor::operator*(const LinearColor& other) const
  {
    return LinearColor(R * other.R,
      G * other.G,
      B * other.B,
      A * other.A);
  }

  FORCEINLINE LinearColor
    LinearColor::operator*(const float scalar) const
  {
    return LinearColor(R * scalar,
      G * scalar,
      B * scalar,
      A * scalar);
  }

  FORCEINLINE LinearColor&
    LinearColor::operator+=(const LinearColor& other)
  {
    R += other.R;
    G += other.G;
    B += other.B;
    A += other.A;

    return *this;
  }

  FORCEINLINE LinearColor&
    LinearColor::operator-=(const LinearColor& other)
  {
    R -= other.R;
    G -= other.G;
    B -= other.B;
    A -= other.A;

    return *this;
  }

  FORCEINLINE LinearColor&
    LinearColor::operator*=(const LinearColor& other)
  {
    R *= other.R;
    G *= other.G;
    B *= other.B;
    A *= other.A;

    return *this;
  }

  FORCEINLINE LinearColor&
    LinearColor::operator*=(const float scalar)
  {
    R *= scalar;
    G *= scalar;
    B *= scalar;
    A *= scalar;

    return *this;
  }
}
