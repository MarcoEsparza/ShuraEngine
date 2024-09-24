/*************************************************************/
/*
*  @file    shPlane.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/20
*  @brief   Math class for plane figure.
*
*  Math class for plane figure.
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
*  Math class for plane figure.
*/
class SH_UTILITY_EXPORT shPlane
{
 public:
  /*
  *  Default constructor.
  */
  shPlane() = default;
  /*
  *  Default destructor.
  */
  ~shPlane() = default;
  
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
   *  Minimum values for X and Z (y value to access z).
   */
   Vector2 min;
   /*
   *  Maximum values for X and Z (y value to access z).
   */
   Vector2 max;
};
}
