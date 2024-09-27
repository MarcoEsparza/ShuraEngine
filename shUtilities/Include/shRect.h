/*************************************************************/
/*
*  @file    shRect.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/27
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
/*
*  Math class for rectangle figure.
*/
class SH_UTILITY_EXPORT shRect
{
 public:
  /*
  *  Default constructor.
  */
  shRect() = default;
  /*
  *  Constructor to initialize rect values.
  * 
  *  @param const Vector2& _pos: Desired min value.
  * 
  *  @param const Vector2& _size: Compute max value with size and min value.
  */
  shRect(const Vector2& _pos, const Vector2& _size);
  /*
  *  Default destructor.
  */
  ~shRect() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /*
  *  Set the min values for the rect.
  * 
  *  @param const Vector2& _pos: Desired min value.
  * 
  *  @return
  */
  void
  setPosition(const Vector2& _pos);

  /*
  *  Compute the max values with given size and the position.
  *  
  *  @param const Vector2& _size: Set the max values with this plus position.
  * 
  *  @return
  */
  void
  setSize(const Vector2& _size);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
   /*
   *  Minimum values for X and Y.
   */
   Vector2 min;
   /*
   *  Maximum values for X and Y.
   */
   Vector2 max;
};
}
