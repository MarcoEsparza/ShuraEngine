/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shSceneGraph.h"
#include "shGraphicsManager.h"
#include "shMeshComponent.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
SceneGraph::SceneGraph()
{
  m_staticVBuffer = make_shared<VertexBuffer>();
  m_staticIBuffer = make_shared<IndexBuffer>();
}

void
SceneGraph::createEmptyObject(const String& objectName)
{
  auto newObject = make_shared<GameObject>();
  newObject->name = objectName;

  addObject(newObject);
}

void
SceneGraph::addObject(const SPtr<GameObject>& object)
{
  m_gameObjects.push_back(object);
}

const Vector<SPtr<GameObject>>&
SceneGraph::getGameObjectList() const
{
  return m_gameObjects;
}

SPtr<VertexBuffer>
SceneGraph::getStaticMeshesVertexBuffer()
{
  return m_staticVBuffer;
}

SPtr<IndexBuffer>
SceneGraph::getStaticMeshesIndexBuffer()
{
  return m_staticIBuffer;
}

void
SceneGraph::updateBuffers()
{
  for (auto& gObject : m_gameObjects) {
    for (auto& component : gObject->components) {
      if (component->type == COMPONENT_TYPE::kStaticMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
        for (auto& vertex : sMeshComponent->meshData->vertices) {
          m_smVertexData.push_back(vertex);
        }
        for (auto index : sMeshComponent->meshData->indices) {
          m_smIndexData.push_back(index);
        }
      }
    }
  }

  m_staticVBuffer = GraphicsManager::instance().createVertexBuffer(m_smVertexData);
  m_staticIBuffer = GraphicsManager::instance().createIndexBuffer(m_smIndexData);
}

void
SceneGraph::drawStaticMeshesInScene(const SPtr<InputLayout>& inputLayout)
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setInputLayout(inputLayout);
  gManager.setVertexBuffers(m_staticVBuffer);
  gManager.setIndexBuffers(m_staticIBuffer);
  
  uint32 vertexCount = 0;
  uint32 indexCount = 0;
  uint32 srvSlot = 0;
  for (auto& gObject : m_gameObjects) {
    for (auto& component : gObject->components) {
      if (component->type == COMPONENT_TYPE::kStaticMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<StaticMeshComponent>(component);
        auto meshMat = reinterpret_pointer_cast<PBRMaterial>(sMeshComponent->material);
        gManager.setProgramShader(meshMat->shader);
        gManager.setShaderResourceView(meshMat->baseColor, srvSlot);

        gManager.drawIndexed(sMeshComponent->meshData->numIndex,
                             indexCount,
                             vertexCount);

        indexCount += sMeshComponent->meshData->numIndex;
        vertexCount += sMeshComponent->meshData->numVertex;
        ++srvSlot;
      }
    }
  }

  m_srvSMSlotCount = srvSlot;
}

void
SceneGraph::drawSkeletalMeshesInScene(const SPtr<InputLayout>& inputLayout)
{
  GraphicsManager& gManager = GraphicsManager::instance();

  gManager.setInputLayout(inputLayout);

  uint32 vbSlot = 1;
  uint32 ibSlot = 1;
  uint32 srvSlot = m_srvSMSlotCount;
  for (auto& gObject : m_gameObjects) {
    for (auto& component : gObject->components) {
      if (component->type == COMPONENT_TYPE::kSkeletalMesh) {
        auto sMeshComponent = reinterpret_pointer_cast<SkeletalMeshComponent>(component);
        
        gManager.setVertexBuffers(sMeshComponent->m_vertexBuffer, vbSlot);
        gManager.setIndexBuffers(sMeshComponent->m_indexBuffer, ibSlot);
        
        uint32 vertexCount = 0;
        uint32 indexCount = 0;
        for (uint32 i = 0; i < sMeshComponent->meshData->numMeshes; ++i) {
          auto meshMat = reinterpret_pointer_cast<PBRMaterial>(sMeshComponent->materials[i]);
          gManager.setProgramShader(meshMat->shader);
          gManager.setShaderResourceView(meshMat->baseColor, srvSlot);

          gManager.drawIndexed(sMeshComponent->meshData->numIndices[i],
                               indexCount,
                               vertexCount);

          indexCount += sMeshComponent->meshData->numIndices[i];
          vertexCount += sMeshComponent->meshData->numVertices[i];
        }
        ++vbSlot;
        ++ibSlot;
        ++srvSlot;
      }
    }
  }
}
}