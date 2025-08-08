/*****************************************************************************/
/*
*  @file    shCapsule.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for capsule figure.
*
*  Math class for capsule figure.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Math class for capsule figure.
*/
class SH_UTILITY_EXPORT Capsule
{
 public:
  /**
  *  @brief Default constructor.
  */
  Capsule() = default;

  /**
  *  @brief Constructor that initialize the capsule values.
  * 
  *  @param const float _radius: The radius.
  *  @param const Vector3& pA: Point A.
  *  @param const Vector3& pB: Point B.
  */
  Capsule(const float _radius,
          const Vector3& _center,
          const Vector3& _direction,
          const float _height)
    : radius(_radius),
      center(_center),
      direction(_direction),
      height(_height)
  {}

  /**
  *  @brief Default destructor.
  */
  ~Capsule() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Project the capsule on the axis.
  *
  *  @param Vector3& axis
  *  @param float& min: Reference is needed, it will save the result.
  *  @param float& max: Reference is needed, it will save the result.
  */
  void
  projectOnAxis(const Vector3& axis, float& min, float& max) const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief The radius of the capsule figure.
  */
  float radius;

  /**
  *  @brief The center of the figure
  */
  Vector3 center;

  /**
  *  @brief The direction.
  */
  Vector3 direction;

  /**
  *  @brief The height between the two points of the capsule.
  */
  float height;
};
}
