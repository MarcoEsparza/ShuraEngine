/*****************************************************************************/
/*
*  @file    shFrustum.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Frustum class for camera view frustum calculations.
*
*  Frustum class for camera view frustum calculations.
*
*  @bug     No known bugs.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include <shPlane.h>
#include <shMatrix4.h>

namespace shEngineSDK {
/**
*  @brief Frustum class for camera view frustum calculations.
*/
class SH_CORE_EXPORT Frustum
{
 public:
  /**
  *  @brief Default constructor for the Frustum class.
  */
  Frustum() = default;

  /**
  *  @brief Destructor for the Frustum class.
  */
  ~Frustum() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Calculates the planes of the frustum based on the provided view and
  *         projection matrices.
  * 
  *  @param const Matrix4& view: The view matrix of the camera.
  *  @param const Matrix4& proj: The projection matrix of the camera.
  */
  void
  calculatePlanes(const Matrix4& view, const Matrix4& proj);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Near plane of the frustum.
  */
  Plane m_near;

  /**
  *  @brief Far plane of the frustum.
  */
  Plane m_far;

  /**
  *  @brief Left plane of the frustum.
  */
  Plane m_left;

  /**
  *  @brief Right plane of the frustum.
  */
  Plane m_right;

  /**
  *  @brief Top plane of the frustum.
  */
  Plane m_top;

  /**
  *  @brief Bottom plane of the frustum.
  */
  Plane m_bottom;
};
}
