/*************************************************************/
/*
*  @file    shRect.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
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
  *  Default destructor.
  */
  ~shRect() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  

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
