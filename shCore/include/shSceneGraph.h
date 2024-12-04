/*************************************************************/
/*
*  @file    shSceneGraph.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Scene graph class.
*
*  Scene graph class.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shGameObject.h"
#include "shBuffers.h"
#include "shGraphicTypes.h"
#include "shInputLayout.h"

namespace shEngineSDK {
/**
*  @brief Scene graph class.
*/
class SceneGraph
{
 public:
  /**
  *  @brief Default constructor.
  */
  SceneGraph();

  /**
  *  @brief Default destructor.
  */
  virtual ~SceneGraph() = default;

  /**
  *  @brief Creates and add a empty object to the scene.
  * 
  *  @param String& objectName
  */
  void
  createEmptyObject(const String& objectName);

  /**
  *  @brief Adds a object to the scene.
  * 
  *  @param SPtr<GameObject>& object
  */
  void
  addObject(const SPtr<GameObject>& object);

  /**
  *  @brief Gets the object list.
  * 
  *  @return const Vector<SPtr<GameObject>>&
  */
  const Vector<SPtr<GameObject>>&
  getGameObjectList() const;

  /**
  *  @brief Gets the Vertex Buffer for static meshes.
  * 
  *  @return SPtr<VertexBuffer>
  */
  SPtr<VertexBuffer>
  getStaticMeshesVertexBuffer();

  /**
  *  @brief Gets the Index Buffer for static meshes.
  * 
  *  @return SPtr<IndexBuffer>
  */
  SPtr<IndexBuffer>
  getStaticMeshesIndexBuffer();

  /**
  *  @brief Update the static buffers with all the static meshes in scene.
  */
  void
  updateBuffers();

  /**
  *  @brief Draw the static meshes in scene.
  * 
  *  @param SPtr<InputLayout>& inputLayout
  */
  void
  drawStaticMeshesInScene(const SPtr<InputLayout>& inputLayout);

  /**
  *  @brief Draw the skeletal meshes in scene.
  * 
  *  @param SPtr<InputLayout>& inputLayout
  */
  void
  drawSkeletalMeshesInScene(const SPtr<InputLayout>& inputLayout);

 private:
  /**
  *  @brief Game Objects storage.
  */
  Vector<SPtr<GameObject>> m_gameObjects;

  /**
  *  @brief Static Mesh Vertex Data.
  */
  Vector<VertexData> m_smVertexData;

  /**
  *  @brief Static Mesh Index Data.
  */
  Vector<uint32> m_smIndexData;

  /**
  *  @brief Static Mesh Vertex Buffer.
  */
  SPtr<VertexBuffer> m_staticVBuffer;

  /**
  *  @brief Static Mesh Index Buffer.
  */
  SPtr<IndexBuffer> m_staticIBuffer;

  /**
  *  @brief Shader Resource View Static Mesh Slot Count
  */
  uint32 m_srvSMSlotCount;
};
}
