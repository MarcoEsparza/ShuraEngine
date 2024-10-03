/*************************************************************/
/*
*  @file    shBoxAAB.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
*  @brief   Math class for Axis-Aligned Bounding-Box.
*
*  Math class for Axis-Aligned Bounding-Box.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Math class for Axis-Aligned Bounding-Box.
*/
class SH_UTILITY_EXPORT shBoxAAB
{
 public:
  /**
  *  @brief Default constructor.
  */
  shBoxAAB() = default;
  /**
  *  @brief Constructor to initialize box values.
  * 
  *  @param const Vector3& pos: Desired min value.
  *  @param const Vector3& size: Compute max value with size and min value.
  */
  shBoxAAB(const Vector3& pos, const Vector3& size);
  /**
  *  @brief Default destructor.
  */
  ~shBoxAAB() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Set the min values for the box.
  *
  *  @param const Vector3& pos: Desired min value.
  */
  void
  setPosition(const Vector3& pos);

  /**
  *  @brief Compute the max values with given size and the position.
  *
  *  @param const Vector3& size: Set the max values with this plus position.
  */
  void
  setSize(const Vector3& size);

  /**
  *  @brief Returns box min position values.
  *
  *  @return Vector3
  */
  Vector3
  getMinPosition() const;

  /**
  *  @brief Returns box max position values.
  * 
  *  @return Vector3
  */
  Vector3
  getMaxPosition() const;

  /**
  *  @brief Project the box corners on the axis.
  *
  *  @param Vector3& axis
  *  @param float& _min: Reference is needed, it will save the result
  *  @param float& _max: Reference is needed, it will save the result
  */
  void
  projectOnAxis(const Vector3& axis, float& _min, float& _max) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Minimum values for X, Y and Z position
  */
  Vector3 min;

  /**
  *  @brief Maximum values for X, Y and Z position
  */
  Vector3 max;
};
}
