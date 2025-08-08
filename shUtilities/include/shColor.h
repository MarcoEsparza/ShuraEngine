/*****************************************************************************/
/*
*  @file    shColor.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/28
*  @brief   Color class.
*
*  Color class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
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
  *  @param uint8 R
  *  @param uint8 G
  *  @param uint8 B
  *  @param uint8 A
  */
  FORCEINLINE Color(const uint8 R, const uint8 G, const uint8 B, const uint8 A = 255)
                    : r(R), g(G), b(B), a(A) {}

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

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Red value.
  */
  uint8 r;

  /**
  *  @brief Green value.
  */
  uint8 g;

  /**
  *  @brief Blue value.
  */
  uint8 b;

  /**
  *  @brief Alpha value.
  */
  uint8 a;

  /***************************************************************************/
  /*
  *  Static variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Black = Color(0, 0, 0);
  */
  static const Color BLACK;

  /**
  *  @brief White = Color(255, 255, 255);
  */
  static const Color WHITE;

  /**
  *  @brief Red = Color(255, 0, 0);
  */
  static const Color RED;

  /**
  *  @brief Green = Color(0, 255, 0);
  */
  static const Color GREEN;

  /**
  *  @brief Blue = Color(0, 0, 255);
  */
  static const Color BLUE;

  /**
  *  @brief Yellow = Color(220, 220, 80);
  */
  static const Color YELLOW;

  /**
  *  @brief Gray = Color(150, 150, 150);
  */
  static const Color GRAY;

  /**
  *  @brief Purple = Color(165, 115, 206);
  */
  static const Color PURPLE;
};
}
