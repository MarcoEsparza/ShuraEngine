/*****************************************************************************/
/*
*  @file    shSphere.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Math class for sphere figure.
*
*  Math class for sphere figure.
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
*  @brief Math class for sphere figure.
*/
class SH_UTILITY_EXPORT Sphere
{
 public:
  /**
  *  @brief Default constructor.
  */
  Sphere() = default;

  /**
  *  @brief Constructor to initialize center and radius values.
  * 
  *  @param const Vector3& center: Desired center.
  *  @param const float radius: Desired radius.
  */
  Sphere(const Vector3& center, const float radius)
    : radius(radius), center(center)
  {}

  /**
  *  @brief Default destructor.
  */
  ~Sphere() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
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
