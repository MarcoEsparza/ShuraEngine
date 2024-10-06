/*************************************************************/
/*
*  @file    shSphere.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/03
*  @brief   Math class for sphere figure.
*
*  Math class for sphere figure.
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
*  @brief Math class for sphere figure.
*/
class SH_UTILITY_EXPORT shSphere
{
 public:
  /**
  *  @brief Default constructor.
  */
  shSphere() = default;
  /**
  *  @brief Constructor to initialize center and radius values.
  * 
  *  @param const Vector3& _center: Desired center.
  *  @param const float _radius: Desired radius.
  */
  shSphere(const Vector3& _center, const float _radius)
           : center(_center), radius(_radius) {}
  /**
  *  @brief Default destructor.
  */
  ~shSphere() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/

  /**
  *  @brief Set the center of the sphere.
  * 
  *  @param Vector3& _center: Desired center.
  */
  void
  setCenter(const Vector3& _center);

  /**
  *  @brief Set the radius of the sphere.
  *  
  *  @param float _radius: Desired radius.
  */
  void
  setRadius(const float _radius);

  /**
  *  @brief Returns sphere center.
  *
  *  @return Vector3
  */
  Vector3
  getCenter() const;

  /**
  *  @brief Returns sphere radius.
  *
  *  @return float
  */
  float
  getRadius() const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
public:
  /**
  *  @brief The sphere radius.
  */
  float radius;

  /**
  *  @brief The position of the center of the sphere.
  */
  Vector3 center;
};
}
