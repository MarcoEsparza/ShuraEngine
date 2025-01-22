/*************************************************************/
/*
*  @file    shCamera.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/21
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     Camera movement not working properly.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shMatrix4.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Engine Camera class.
*/
class SH_CORE_EXPORT Camera
{
 public:
  /**
  *  @brief Default constructor.
  */
  Camera() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Camera() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Sets view matrix.
  * 
  *  @param Vector3& camPos
  *  @param Vector3& targetPos
  *  @param Vector3& upVector
  */
  void
  setViewData(const Vector3& camPos,
              const Vector3& targetPos,
              const Vector3& upVector);

  /**
  *  @brief Gets the view matrix.
  *
  *  @return const ViewMatrix&
  */
  FORCEINLINE const ViewMatrix&
  getView() const { return m_view; }

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief The view matrix.
  */
  ViewMatrix m_view;

  /**
  *  @brief Camera position.
  */
  Vector3 m_position;

  /**
  *  @brief Camera look target.
  */
  Vector3 m_target;

  /**
  *  @brief Camera up direction vector.
  */
  Vector3 m_upVector;

  /**
  *  @brief Camera rigth direction.
  */
  Vector3 m_right = { 1.0f, 0.0f, 0.0f };
};

class SH_CORE_EXPORT FPSCamera : public Camera
{
 public:
  /**
  *  @brief  Default constructor.
  */
  FPSCamera() = default;

  /**
  *  @brief Default destructor.
  */
  ~FPSCamera() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Sets projection matrix.
  *
  *  @param float halfFOV
  *  @param float width
  *  @param float height
  *  @param float minZ
  *  @param float maxZ
  */
  void
  setProjectionData(const float halfFOV,
                    const float width,
                    const float height,
                    const float minZ,
                    const float maxZ);

  // TODO: Change the move functions.
  /**
  *  @brief Move camera in the X axis.
  * 
  *  @param float dir
  */
  void
  moveX(const float dir);

  /**
  *  @brief Move camera in the X axis.
  * 
  *  @param float dir
  */
  void
  moveY(const float dir);

  /**
  *  @brief Move camera in the X axis.
  * 
  *  @param float dir
  */
  void
  moveZ(const float dir);

  /**
  *  @brief Move camera.
  *
  *  @param Vector3& direction
  */
  void
  move(const Vector3& direction);

  /**
  *  @brief Rotate camera in the X and Y axes.
  * 
  *  @param float yaw
  *  @param float pitch
  */
  void
  rotateCam(const float yaw, const float pitch);

  /**
  *  @brief Gets the projection matrix.
  *
  *  @return const ProjectionMatrix&
  */
  FORCEINLINE const ProjectionMatrix&
  getProjection() const { return m_proj; }

 private:
  /**
  *  @brief The projection matrix.
  */
  ProjectionMatrix m_proj;
};

class SH_CORE_EXPORT OrthographicCamera : public Camera
{
 public:
  /**
  *  @brief  Default constructor.
  */
  OrthographicCamera() = default;

  /**
  *  @brief Default destructor.
  */
  ~OrthographicCamera() = default;

  /**
  *  @brief Sets the orthographic projection matrix.
  *
  *  @param const float left
  *  @param const float right
  *  @param const float bottom
  *  @param const float top
  *  @param const float nearZ
  *  @param const float farZ
  */
  void
  setOrthographicProjData(const float left,
                          const float right,
                          const float bottom,
                          const float top,
                          const float nearZ,
                          const float farZ);

  /**
  *  @brief Gets the orthographic projection matrix.
  *
  *  @return const OrthographicProjectionMatrix&
  */
  FORCEINLINE const OrthographicProjectionMatrix&
  getOrthographicProjection() const { return m_orthoProj; }

 private:
  /**
  *  @brief The orthogonal projection matrix.
  */
  OrthographicProjectionMatrix m_orthoProj;
};
}
