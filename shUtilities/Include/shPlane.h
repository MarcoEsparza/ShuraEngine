/*************************************************************/
/*
*  @file    shPlane.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/05
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
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Math class for plane figure.
*/
class SH_UTILITY_EXPORT shPlane
{
 public:
  /**
  *  @brief Default constructor.
  */
  shPlane() = default;
  /**
  *  @brief Constructor to initialize plane values.
  * 
  *  @param const Vector3& _normal: Desired normal.
  *  @param const float _distance: Desired distance.
  */
  shPlane(const Vector3& _normal, const float _distance)
          : normal(_normal), distance(_distance) {}
  /**
  *  @brief Default destructor.
  */
  ~shPlane() = default;
  
  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Set the normal value for the plane.
  *
  *  @param const Vector3& _normal: Desired normal value.
  */
  void
  setNormal(const Vector3& _normal);

  /**
  *  @brief Set the distance for the plane.
  *
  *  @param const float _distance: Desired distance.
  */
  void
  setDistance(const float _distance);

  /**
  *  @brief Returns plane normal.
  *
  *  @return Vector3
  */
  Vector3
  getNormal() const;

  /**
  *  @brief Returns plane distance.
  *
  *  @return float
  */
  float
  getDistance() const;

  /**
  *  @brief Projects a point to the plane distance.
  *
  *  @param Vector3& point
  *
  *  @return float
  */
  float
  pointToPlaneDistance(const Vector3& point) const;

  /**
  *  @brief Evaluates a point with the plane.
  *
  *  @param Vector3& point
  *
  *  @return float
  */
  float
  evaluate(const Vector3& point) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
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
