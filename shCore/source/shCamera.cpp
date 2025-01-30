/*****************************************************************************/
/*
*  @file    shCamera.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/29
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
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
  m_up = upVector;
}

void
Camera::setPerspectiveData(const float halfFOV,
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
  m_proj = OrthographicProjectionMatrix(left, right, bottom, top, nearZ, farZ);
}

void
Camera::move(const Vector3& direction)
{
  Matrix4 transalation = Matrix4::IDENTITY;
  transalation = transalation.createTranslationMatrix(direction);

  m_view *= transalation;
}

void
Camera::rotateCam(const float yaw, const float pitch)
{
  Matrix4 rotation = Matrix4::IDENTITY;
  Matrix4 rotX = rotation.createRotationXMatrix(Radian(pitch));
  Matrix4 rotY = rotation.createRotationYMatrix(Radian(yaw));
  rotation = rotX * rotY;

  //Vector3 axis(pitch, yaw, 0.0f);
  //Matrix4 rotation = MatrixRotationAxis(axis, 0.0f);

  m_view *= rotation;
}
}
