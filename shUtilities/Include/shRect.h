/*************************************************************/
/*
*  @file    shRect.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
*  @brief   Math class for rectangle figure.
*
*  Math class for rectangle figure.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector2.h"

namespace shEngineSDK {
/**
*  @brief Math class for rectangle figure.
*/
class SH_UTILITY_EXPORT shRect
{
 public:
  /**
  *  @brief Default constructor.
  */
  shRect() = default;
  /**
  *  @brief Constructor to initialize rect values.
  * 
  *  @param const Vector2& pos: Desired min value.
  *  @param const Vector2& size: Compute max value with size and min value.
  */
  shRect(const Vector2& pos, const Vector2& size);
  /**
  *  @brief Default destructor.
  */
  ~shRect() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Set the min values for the rect.
  * 
  *  @param const Vector2& pos: Desired min value.
  */
  void
  setPosition(const Vector2& pos);

  /**
  *  @brief Compute the max values with given size and the position.
  *  
  *  @param const Vector2& size: Set the max values with this plus position.
  */
  void
  setSize(const Vector2& size);

  /**
  *  @brief Returns rect min position values.
  *
  *  @return Vector2
  */
  Vector2
  getMinPosition() const;

  /**
  *  @brief Returns rect max position values.
  *
  *  @return Vector2
  */
  Vector2
  getMaxPosition() const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Minimum values for X and Y.
  */
  Vector2 min;

  /**
  *  @brief Maximum values for X and Y.
  */
  Vector2 max;
};
}
