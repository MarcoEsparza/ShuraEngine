/*****************************************************************************/
/*
*  @file    shPlane.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for plane figure.
*
*  Math class for plane figure.
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
*  @brief Math class for plane figure.
*/
class SH_UTILITY_EXPORT Plane
{
 public:
  /**
  *  @brief Default constructor.
  */
  Plane() = default;

  /**
  *  @brief Constructor to initialize plane values.
  * 
  *  @param const Vector3& normal: Desired normal.
  *  @param const float distance: Desired distance.
  */
  Plane(const Vector3& normal, const float distance)
    : normal(normal), distance(distance)
  {}
  /**
  *  @brief Default destructor.
  */
  ~Plane() = default;
  
  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Projects a point to the plane distance.
  *
  *  @param Vector3& point: Point to project.
  *
  *  @return float: Projected point.
  */
  float
  pointToPlaneDistance(const Vector3& point) const;

  /**
  *  @brief Evaluates a point with the plane.
  *
  *  @param Vector3& point: Point to evaluate.
  *
  *  @return float: Evaluation result.
  */
  float
  evaluate(const Vector3& point) const;

  /**
  *  @brief Normalize the plane.
  */
  void
  normalize();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Plane normal
  */
  Vector3 normal;

  /**
  *  @brief The distance
  */
  float distance;
};
}
