/*************************************************************/
/*
*  @file    shCamera.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/20
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     Camera movement not working properly.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shCamera.h"
#include "shMath.h"
#include "shRadian.h"

namespace shEngineSDK {
void
Camera::setViewData(const Vector3& camPos,
                    const Vector3& targetPos,
                    const Vector3& upVector)
{
  m_view = ViewMatrix(camPos, targetPos, upVector);

  m_position = camPos;
  m_target = targetPos;
  m_upVector = upVector;
}

void
Camera::setProjectionData(const float halfFOV,
                          const float width,
                          const float height,
                          const float minZ,
                          const float maxZ)
{
  m_proj = ProjectionMatrix(halfFOV, width, height, minZ, maxZ);
}

void
Camera::setOrthographicProjData(const float left,
                                const float right,
                                const float bottom,
                                const float top,
                                const float nearZ,
                                const float farZ)
{
  m_orthoProj = OrthographicProjectionMatrix(left, right, bottom, top, nearZ, farZ);
}

void
FPSCamera::moveX(const float dir)
{
  const Vector3 forward = m_target - m_position;
  m_right = m_upVector.cross(forward);
  m_right.normalize();

  m_position += m_right * dir;
  m_target += m_right * dir;

  setViewData(m_position, m_target, m_upVector);
}

void
FPSCamera::moveY(const float dir)
{
  const Vector3 forward = m_target - m_position;
  m_upVector = forward.cross(m_right);
  m_upVector.normalize();

  m_position += m_upVector * dir;
  m_target += m_upVector * dir;

  setViewData(m_position, m_target, m_upVector);
}

void
FPSCamera::moveZ(const float dir)
{
  const Vector3 temp = m_right.cross(m_upVector);
  const Vector3 forward = temp.getNormalized();

  m_position += forward * dir;
  m_target += forward * dir;

  setViewData(m_position, m_target, m_upVector);
}

// TODO: Change above functions to this.
void
FPSCamera::move(const Vector3&)
{

}

void
FPSCamera::rotateCam(const float yaw, const float pitch)
{
  Matrix4 rotation = Matrix4::IDENTITY;
  Matrix4 rotX = rotation.createRotationXMatrix(Radian(pitch));
  Matrix4 rotY = rotation.createRotationYMatrix(Radian(yaw));
  rotation = rotX * rotY;

  m_view *= rotation;
}
}
