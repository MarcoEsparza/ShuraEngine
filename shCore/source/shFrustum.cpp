/*****************************************************************************/
/*
*  @file    shFrustum.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/30
*  @brief
*
*
*
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shFrustum.h"

namespace shEngineSDK {
void
Frustum::calculatePlanes(const Matrix4& view, const Matrix4& proj)
{
  // Calculate the inverse of the projection and view matrix
  const Matrix4 invP = proj.getInversed();
  const Matrix4 invV = view.getInversed();

  // Set the 8 frustum vertices
  Vector3 p1(-1.0f, -1.0f, 0.0f);
  Vector3 p2(1.0f, -1.0f, 0.0f);
  Vector3 p3(-1.0f, 1.0f, 0.0f);
  Vector3 p4(1.0f, 1.0f, 0.0f);
  Vector3 p5(-1.0f, -1.0f, 1.0f);
  Vector3 p6(1.0f, -1.0f, 1.0f);
  Vector3 p7(-1.0f, 1.0f, 1.0f);
  Vector3 p8(1.0f, 1.0f, 1.0f);

  // Calculate the world position of the vertices
  p1 = invP * p1;
  p1 = invV * p1;

  p2 = invP * p2;
  p2 = invV * p2;

  p3 = invP * p3;
  p3 = invV * p3;

  p4 = invP * p4;
  p4 = invV * p4;

  p5 = invP * p5;
  p5 = invV * p5;

  p6 = invP * p6;
  p6 = invV * p6;

  p7 = invP * p7;
  p7 = invV * p7;

  p8 = invP * p8;
  p8 = invV * p8;

  // Calculate the plane normals
  // Near p1 -> p2 -> p3
  Vector3 nearNormal = ((p2 - p1).cross(p3 - p1)).getNormalized();
  
  // Far p5 -> p6 -> p7
  Vector3 farNormal = ((p6 - p5).cross(p7 - p5)).getNormalized();
  
  // Left p5 -> p1 -> p7
  Vector3 leftNormal = ((p1 - p5).cross(p7 - p5)).getNormalized();

  // Right p2 -> p6 -> p4
  Vector3 rightNormal = ((p6 - p2).cross(p4 - p2)).getNormalized();

  // Top p5 -> p6 -> p1
  Vector3 topNormal = ((p6 - p5).cross(p1 - p5)).getNormalized();
  
  // Bottom p7 -> p8 -> p3
  Vector3 bottomNormal = ((p8 - p7).cross(p3 - p7)).getNormalized();

  // Set planes
  m_near.setNormal(nearNormal);
  m_near.setDistance(-((nearNormal.x * p1.x) +
                     (nearNormal.y * p1.y) +
                     (nearNormal.z * p1.z)));

  m_far.setNormal(farNormal);
  m_far.setDistance(-((farNormal.x * p5.x) +
                    (farNormal.y * p5.y) +
                    (farNormal.z * p5.z)));

  m_left.setNormal(leftNormal);
  m_left.setDistance(-((leftNormal.x * p5.x) +
                     (leftNormal.y * p5.y) +
                     (leftNormal.z * p5.z)));

  m_right.setNormal(rightNormal);
  m_right.setDistance(-((rightNormal.x * p2.x) +
                      (rightNormal.y * p2.y) +
                      (rightNormal.z * p2.z)));

  m_top.setNormal(topNormal);
  m_top.setDistance(-((topNormal.x * p5.x) +
                    (topNormal.y * p5.y) +
                    (topNormal.z * p5.z)));

  m_bottom.setNormal(bottomNormal);
  m_bottom.setDistance(-((bottomNormal.x * p7.x) +
                       (bottomNormal.y * p7.y) +
                       (bottomNormal.z * p7.z)));
}
}
