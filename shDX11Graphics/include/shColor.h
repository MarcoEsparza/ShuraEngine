/*************************************************************/
/*
*  @file    shColor.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   Color class.
*
*  Color class.
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
#include "shPrerequisitesDX11Graphics.h"

namespace shEngineSDK {
/**
*  @brief Color class.
*/
class Color
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE Color() = default;

  /**
  *  @brief Constructor to initialize values, A = 255.
  * 
  *  @param uint8 r
  *  @param uint8 g
  *  @param uint8 b
  *  @param uint8 a
  */
  FORCEINLINE Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a = 255)
                    : R(r), G(g), B(b), A(a) {}

  /**
  *  @brief Copy constructor.
  * 
  *  @param Color& copy
  */
  Color(const Color& copy);

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~Color() = default;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Red value.
  */
  uint8 R;

  /**
  *  @brief Green value.
  */
  uint8 G;

  /**
  *  @brief Blue value.
  */
  uint8 B;

  /**
  *  @brief Alpha value.
  */
  uint8 A;

  /*************************************************************/
  /*
  *  Static variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Black = Color(0, 0, 0);
  */
  static const Color Black;

  /**
  *  @brief White = Color(255, 255, 255);
  */
  static const Color White;

  /**
  *  @brief Red = Color(255, 0, 0);
  */
  static const Color Red;

  /**
  *  @brief Green = Color(0, 255, 0);
  */
  static const Color Green;

  /**
  *  @brief Blue = Color(0, 0, 255);
  */
  static const Color Blue;

  /**
  *  @brief Yellow = Color(220, 220, 80);
  */
  static const Color Yellow;

  /**
  *  @brief Gray = Color(150, 150, 150);
  */
  static const Color Gray;

  /**
  *  @brief Purple = Color(165, 115, 206);
  */
  static const Color Purple;
};
}
