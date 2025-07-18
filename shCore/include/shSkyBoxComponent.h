/*****************************************************************************/
/*
*  @file    shSkyBoxComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Skybox component to render a skybox in the scene.
*
*  Skybox component to render a skybox in the scene.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shComponent.h"
#include <shVector3.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward Declarations
*/
/*****************************************************************************/

class VertexBuffer;
class IndexBuffer;
class Material;

/**
*  @brief Skybox component to render a skybox in the scene.
*/
class SH_CORE_EXPORT SkyBoxComponent : public Component
{
 public:
  /**
  *  @brief Constructor for the SkyBoxComponent.
  */
  SkyBoxComponent()
    : Component(COMPONENT_TYPE::kSkyBox)
  {}

  /**
  *  @brief Destructor for the SkyBoxComponent.
  */
  virtual ~SkyBoxComponent() = default;

 public:
  /**
  *  @brief Sets the vertices for the skybox.
  * 
  *  @param Vector<Vector3>& vertices: The vertices to set for the skybox.
  */
  void
  setVertices(const Vector<Vector3>& vertices);

  /**
  *  @brief Sets the indices for the skybox.
  * 
  *  @param Vector<uint32>& indices: The indices to set for the skybox.
  */
  void
  setIndices(const Vector<uint32>& indices);

  /**
  *  @brief Sets the material for the skybox.
  * 
  *  @param SPtr<Material>& mat: The material to set for the skybox.
  */
  FORCEINLINE void
  setMaterial(const SPtr<Material>& mat);

  /**
  *  @brief Gets the vertex buffer of the skybox.
  * 
  *  @return SPtr<VertexBuffer>: The vertex buffer of the skybox.
  */
  FORCEINLINE const SPtr<VertexBuffer>
  getVertexBuffer() const;

  /**
  *  @brief Gets the index buffer of the skybox.
  *
  *  @return SPtr<IndexBuffer>: The index buffer of the skybox.
  */
  FORCEINLINE const SPtr<IndexBuffer>
  getIndexBuffer() const;

  /**
  *  @brief Gets the material of the skybox.
  *
  *  @return SPtr<Material>: The material of the skybox.
  */
  FORCEINLINE const SPtr<Material>
  getMaterial() const;

  /**
  *  @brief Gets the vertices of the skybox.
  *
  *  @return Vector<Vector3>: The vertices of the skybox.
  */
  FORCEINLINE const Vector<Vector3>
  getVertices() const;

  /**
  *  @brief Gets the indices of the skybox.
  *
  *  @return Vector<uint32>: The indices of the skybox.
  */
  FORCEINLINE const Vector<uint32>
  getIndices() const;

 private:
  /**
  *  @brief Vertex buffer for the skybox.
  */
  SPtr<VertexBuffer> m_vBuffer;

  /**
  *  @brief Index buffer for the skybox.
  */
  SPtr<IndexBuffer> m_iBuffer;

  /**
  *  @brief Material for the skybox.
  */
  SPtr<Material> m_material;

  /**
  *  @brief Vertices of the skybox.
  */
  Vector<Vector3> m_vertices;

  /**
  *  @brief Indices of the skybox.
  */
  Vector<uint32> m_indices;
};

FORCEINLINE void
SkyBoxComponent::setMaterial(const SPtr<Material>& mat)
{
  m_material = mat;
}

FORCEINLINE const SPtr<VertexBuffer>
SkyBoxComponent::getVertexBuffer() const
{
  return m_vBuffer;
}

FORCEINLINE const SPtr<IndexBuffer>
SkyBoxComponent::getIndexBuffer() const
{
  return m_iBuffer;
}

FORCEINLINE const SPtr<Material>
SkyBoxComponent::getMaterial() const
{
  return m_material;
}

FORCEINLINE const Vector<Vector3>
SkyBoxComponent::getVertices() const
{
  return m_vertices;
}

FORCEINLINE const Vector<uint32>
SkyBoxComponent::getIndices() const
{
  return m_indices;
}
}
