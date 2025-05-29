/*****************************************************************************/
/*
*  @file    shSkyBoxComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shSkyBoxComponent.h"
#include "shGraphicsManager.h"

namespace shEngineSDK {
void
SkyBoxComponent::setVertices(const Vector<Vector3>& vertices)
{
  GraphicsManager& graphMan = g_graphicsMan();
  m_vertices = vertices;
  m_vBuffer = graphMan.createVertexBuffer(m_vertices);
}

void
SkyBoxComponent::setIndices(const Vector<uint32>& indices)
{
  GraphicsManager& graphMan = g_graphicsMan();
  m_indices = indices;
  m_iBuffer = graphMan.createIndexBuffer(m_indices);
}
}
