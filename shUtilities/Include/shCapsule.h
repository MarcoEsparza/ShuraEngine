/*************************************************************/
/*
*  @file    shCapsule.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
*  @brief   Math class for capsule figure.
*
*  Math class for capsule figure.
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
  Capsule(const float _radius, const Vector3& pA, const Vector3& pB)
            : radius(_radius), pointA(pA), pointB(pB) {}
  /**
  *  @brief Default destructor.
  */
  ~Capsule() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Set the min and max values for the capsule.
  * 
  *  @param const Vector3& pA: Desired first point value.
  *  @param const Vector3& pB: Desired second point value.
  */
  void
  setPosition(const Vector3& pA, const Vector3& pB);

  /**
  *  @brief Set the capsule radius.
  *  
  *  @param const float& _radius: Set the radius.
  */
  void
  setRadius(const float _radius);

  /**
  *  @brief Returns capsule point A.
  *
  *  @return Vector3
  */
  Vector3
  getPointA() const;

  /**
  *  @brief Returns capsule point B.
  *
  *  @return Vector3
  */
  Vector3
  getPointB() const;

  /**
  *  @brief Returns capsule radius.
  *
  *  @return float
  */
  float
  getRadius();

  /**
  *  @brief Project the capsule on the axis.
  *
  *  @param Vector3& axis
  *  @param float& min: Reference is needed, it will save the result.
  *  @param float& max: Reference is needed, it will save the result.
  */
  void
  projectOnAxis(const Vector3& axis, float& min, float& max) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief The radius of the capsule figure.
  */
  float radius;

  /**
  *  @brief The min value of the capsule figure.
  */
  Vector3 pointA;

  /**
  *  @brief The max value of the capsule figure.
  */
  Vector3 pointB;

  Vector3 center;
  Vector3 direction;
  float height;
};
}
