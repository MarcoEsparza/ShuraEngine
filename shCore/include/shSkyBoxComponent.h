/*****************************************************************************/
/*
*  @file    shSkyBoxComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
*  @brief   
*
*  
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
#include "shVector3.h"

namespace shEngineSDK {
class VertexBuffer;
class IndexBuffer;
class Material;

class SH_CORE_EXPORT SkyBoxComponent : public Component
{
 public:
  SkyBoxComponent() : Component(COMPONENT_TYPE::kSkyBox) {}
  virtual ~SkyBoxComponent() = default;

 public:
  void
  setVertices(const Vector<Vector3>& vertices);

  void
  setIndices(const Vector<uint32>& indices);

  FORCEINLINE void
  setMaterial(const SPtr<Material>& mat);

  FORCEINLINE const SPtr<VertexBuffer>
  getVertexBuffer() const;

  FORCEINLINE const SPtr<IndexBuffer>
  getIndexBuffer() const;

  FORCEINLINE const SPtr<Material>
  getMaterial() const;

  FORCEINLINE const Vector<Vector3>
  getVertices() const;

  FORCEINLINE const Vector<uint32>
  getIndices() const;

 private:
  SPtr<VertexBuffer> m_vBuffer;
  SPtr<IndexBuffer> m_iBuffer;
  SPtr<Material> m_material;
  Vector<Vector3> m_vertices;
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
