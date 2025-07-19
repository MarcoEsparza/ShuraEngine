/*************************************************************/
/*
*  @file    shGUI.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Graphical User Interface (GUI) system for editor.
*
*  Graphical User Interface (GUI) system for editor.
*
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shGUI.h"
#include "imgui_impl_shura.h"
#include <shRenderManager.h>
#include <shSceneGraph.h>
#include <shTexture.h>
#include <shMath.h>

#include <shComponent.h>
#include <shTransformComponent.h>
#include <shMeshComponent.h>
#include <shSkyBoxComponent.h>
#include <shColliderComponent.h>
#include <shRigidbodyComponent.h>

#include <shMeshResource.h>

using std::remove;
using std::strncpy;
using std::remove_if;

namespace shEngineSDK {
void
GUI::init(const WPtr<Screen> pScreen)
{
  if(pScreen.expired()) {
    //SH_LOG_ERROR("GUI::init: Screen pointer is expired.");
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(pScreen);
  ImGui::StyleColorsDark();
}

void
GUI::shutdown()
{
  ImGui_ImplShura_Shutdown();
  ImGui::DestroyContext();
}

void
GUI::render()
{
  ImGui_ImplShura_NewFrame();
  ImGui::Render();
  ImGui_ImplShura_RenderDrawData(ImGui::GetDrawData());
}

void
GUI::update()
{
  //RenderManager& renderMan = g_renderMan();

  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();

  setDockSpace();

  setSceneGraph();
  setComponentInspector();

  ImGui::Begin("RendererSettings");
  ImGui::Text("Object1");
  ImGui::End();

  ImGui::Begin("ResourceManager");
  ImGui::Text("Resource1");
  ImGui::End();

  ImGui::Begin("Console");
  ImGui::Text("Message1");
  ImGui::End();

  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
  ImGui::Begin("Scene");
  ImGui::PopStyleColor();

  ImVec2 content_min = ImGui::GetWindowContentRegionMin();
  ImVec2 content_max = ImGui::GetWindowContentRegionMax();
  ImVec2 window_pos = ImGui::GetWindowPos();

  ImVec2 scene_pos = ImVec2(window_pos.x + content_min.x, window_pos.y + content_min.y);
  ImVec2 scene_size = ImVec2(content_max.x - content_min.x, content_max.y - content_min.y);

  ImGui::SetCursorScreenPos(scene_pos);

  //auto pScene = renderMan.getTexture("PPMap");
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pScene), scene_size);

  ImGui::End();
}

void
GUI::setDockSpace()
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  static bool built = false;
  if (!built) {
    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    ImGuiID main = dockspace_id;
    ImGuiID left;
    ImGuiID right;
    ImGuiID bottom;
    ImGuiID center;

    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, &left, &main);
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, &right, &main);
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, &bottom, &center);

    ImGui::DockBuilderDockWindow("Scenegraph", left);
    ImGui::DockBuilderDockWindow("Scene", center);
    ImGui::DockBuilderDockWindow("Inspector", right);
    ImGui::DockBuilderDockWindow("RendererSettings", right);
    ImGui::DockBuilderDockWindow("ResourceManager", bottom);
    ImGui::DockBuilderDockWindow("Console", bottom);

    ImGui::DockBuilderFinish(dockspace_id);
    built = true;
  }

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  window_flags |= ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoBringToFrontOnFocus |
                  ImGuiWindowFlags_NoNavFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();

  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Save Scene")) {
        // Handle save scene action
      }
      if (ImGui::MenuItem("Exit")) {
        // Handle exit action
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo")) {
        // Handle undo action
      }
      if (ImGui::MenuItem("Redo")) {
        // Handle redo action
      }
      if (ImGui::MenuItem("Preferences")) {
        // Handle preferences action
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::End();
}

static bool
IsDescendantOf(GameObject* child, GameObject* possibleAncestor) {
  GameObject* current = child;
  while (current) {
    if (current == possibleAncestor)
      return true;
    if (auto parent = current->parent.lock())
      current = parent.get();
    else
      break;
  }
  return false;
}

static SPtr<GameObject>
FindSharedPtrInTreeRecursive(GameObject* raw, const SPtr<GameObject>& node) {
  for (const auto& child : node->childs) {
    if (child.get() == raw)
      return child;
    auto found = FindSharedPtrInTreeRecursive(raw, child);
    if (found) return found;
  }
  return nullptr;
}

static SPtr<GameObject>
FindSharedPtrInTree(GameObject* raw, const Vector<SPtr<GameObject>>& roots) {
  for (const auto& root : roots) {
    if (root.get() == raw)
      return root;
    auto found = FindSharedPtrInTreeRecursive(raw, root);
    if (found) return found;
  }
  return nullptr;
}

static void
showSceneGraph(const SPtr<GameObject>& pNode,
               SPtr<GameObject>& pSelectedObj,
               SPtr<GameObject>& renamingObj)
{
  SceneGraph& scene = g_sceneGraph();

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                             ImGuiTreeNodeFlags_OpenOnDoubleClick;

  if (pNode->childs.empty()) {
    flags |= ImGuiTreeNodeFlags_Leaf;
  }

  if (pNode == pSelectedObj) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  ImGui::PushID(pNode.get());

  bool opened = false;

  if (renamingObj == pNode) {
    static char renameBuffer[256] = {};

    // Just initialize once
    if (!ImGui::IsAnyItemActive()) {
      strncpy(renameBuffer, pNode->name.c_str(), sizeof(renameBuffer));
    }

    // Show input text instead of name
    opened = ImGui::TreeNodeEx("", flags); // Empty label, we keep layout
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.0f);
    if (ImGui::InputText("##rename",
                         renameBuffer,
                         sizeof(renameBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue |
                         ImGuiInputTextFlags_AutoSelectAll)) {
      pNode->name = renameBuffer;
      renamingObj = nullptr;
    }
    if (!ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
      renamingObj = nullptr;
    }
  }
  else {
    opened = ImGui::TreeNodeEx(pNode->name.c_str(), flags);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      pSelectedObj = pNode;
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      renamingObj = pNode;
    }
  }

  // Context menu
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Add Child")) {
      SPtr<GameObject> child = sh_makeShared<GameObject>();
      child->name = "NewChild";
      child->parent = pNode;
      pNode->addChild(child);
    }
    ImGui::EndPopup();
  }

  // Drag and drop
  // Begin Drag Source
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("DND_GAMEOBJECT", &pNode, sizeof(GameObject*)); // Get pointer
    ImGui::Text("Mover %s", pNode->name.c_str());
    ImGui::EndDragDropSource();
  }

  // Begin Drop Target
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_GAMEOBJECT")) {
      auto dragged = *(GameObject**)payload->Data;

      // Avoid to be dragged into itself or its childs
      if (dragged != pNode.get() && !IsDescendantOf(dragged, pNode.get())) {
        // Find the original reference of the dragged object
        auto draggedShared = FindSharedPtrInTree(dragged, scene.getGameObjectList());
        if (draggedShared) {
          // Delete previous parent
          if (auto oldParent = draggedShared->parent.lock()) {
            auto& siblings = oldParent->childs;
            siblings.erase(remove_if(siblings.begin(), siblings.end(),
              [&](const SPtr<GameObject>& o) { return o.get() == dragged; }),
              siblings.end());
          }
          else {
            // If it was rooted, it gets deleted from the list
            scene.getGameObjectList().erase(remove_if(scene.getGameObjectList().begin(),
                                                      scene.getGameObjectList().end(),
                                                      [&](const SPtr<GameObject>& o) {
                                                        return o.get() == dragged;
                                                      }),
                                                      scene.getGameObjectList().end());
          }

          // Assign new parent
          draggedShared->parent = pNode;
          pNode->addChild(draggedShared);
        }
      }
    }
    ImGui::EndDragDropTarget();
  }

  if (opened) {
    for (auto& child : pNode->childs) {
      showSceneGraph(child, pSelectedObj, renamingObj);
    }
    ImGui::TreePop();
  }

  ImGui::PopID();
}

void
GUI::setSceneGraph()
{
  SceneGraph& scene = g_sceneGraph();

  static SPtr<GameObject> renamingObject;

  ImGui::Begin("Scenegraph");

  // Detect right clic on empty window
  if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    if (!ImGui::IsAnyItemHovered()) {
      ImGui::OpenPopup("HierarchyContextMenu");
    }
  }

  // Empty context menu
  if (ImGui::BeginPopup("HierarchyContextMenu")) {
    if (ImGui::MenuItem("Create GameObject")) {
      scene.createEmptyObject("NewObject");
    }
    ImGui::EndPopup();
  }

  // Draw hierarchy
  for (auto& gameObject : scene.getGameObjectList()) {
    showSceneGraph(gameObject, m_pModel, renamingObject);
  }

  ImGui::End();
}

void
GUI::setComponentInspector()
{
  ImGui::Begin("Inspector");
  if (m_pModel) {
    showTransformComponent();
    for (auto& pComponent : m_pModel->components) {
      COMPONENT_TYPE::E type = pComponent->getType();
      if (type == COMPONENT_TYPE::kStaticMesh) {
        showStaticMeshComponent(sh_reinterpretPCast<StaticMeshComponent>(pComponent));
      }
      else if (type == COMPONENT_TYPE::kSkeletalMesh) {

      }
      else if (type == COMPONENT_TYPE::kSkyBox) {

      }
      else if (type == COMPONENT_TYPE::kCamera) {

      }
      else if (type == COMPONENT_TYPE::kCollider) {

      }
      else if (type == COMPONENT_TYPE::kRigidbody) {

      }
      else if (type == COMPONENT_TYPE::kAnimator) {

      }
      else if (type == COMPONENT_TYPE::kLight) {

      }
    }
  }
  ImGui::End();
}

void
GUI::showTransformComponent()
{
  Vector3 modelPos = m_pModel->getPosition();
  Vector3 modelRot = m_pModel->getRotation() * Math::RAD2DEG;
  Vector3 modelScale = m_pModel->getScale();

  if (ImGui::CollapsingHeader("Transform")) {
    // Position
    ImGui::Text("Position:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##PosX", &modelPos.x, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##PosY", &modelPos.y, 0.01f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##PosZ", &modelPos.z, 0.01f);
    ImGui::PopStyleColor(3);

    // Rotation
    ImGui::Text("Rotation:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##RotX", &modelRot.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##RotY", &modelRot.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##RotZ", &modelRot.z, 0.1f);
    ImGui::PopStyleColor(3);

    // Scale
    ImGui::Text("Scale:");
    // Rotation X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##SclX", &modelScale.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##SclY", &modelScale.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Rotation Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##SclZ", &modelScale.z, 0.1f);
    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(220, 220, 220, 150));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 180, 180, 150));
    if (ImGui::Button("Reset")) {
      modelPos = { 0.0f, 0.0f, 0.0f };
      modelRot = { 0.0f, 0.0f, 0.0f };
      modelScale = { 1.0f, 1.0f, 1.0f };
    }
    ImGui::PopStyleColor(3);
  }

  if (modelPos != m_pModel->getPosition()) {
    m_pModel->setPosition(modelPos);
  }
  if (modelRot != m_pModel->getRotation()) {
    m_pModel->setRotation(modelRot * Math::DEG2RAD);
  }
  if (modelScale != m_pModel->getScale()) {
    m_pModel->setScale(modelScale);
  }
}

void
GUI::showStaticMeshComponent(const WPtr<StaticMeshComponent> wpSMesh)
{
  if (wpSMesh.expired()) {
    return;
  }

  auto pMesh = wpSMesh.lock();

  if (ImGui::CollapsingHeader("Static Mesh Component")) {
    ImGui::Text("Material Count: %d",
      static_cast<uint32>(pMesh->m_mesh->m_materials.size()));
    uint32 vertexCount = 0;
    uint32 indexCount = 0;
    for (uint32 j = 0; j < pMesh->m_mesh->m_meshes.size(); ++j) {
      vertexCount += pMesh->m_mesh->m_meshes[j].numVertices;
      indexCount += pMesh->m_mesh->m_meshes[j].numIndices;
    }
    ImGui::Text("Vertex Count: %d", vertexCount);
    ImGui::Text("Index Count: %d", indexCount);

    for (int32 j = 0; j < pMesh->m_mesh->m_materials.size(); ++j) {
      auto& currentMat = pMesh->m_mesh->m_materials[j];
      String matName = "Material:" + currentMat->name;
      //ImGui::Text(matName.c_str());
      if (ImGui::Button(matName.c_str())) {
        m_selectedMat = j;
        m_bTexColor = false;
      }

      if (m_selectedMat >= 0 && m_selectedMat == j) {
        //m_bTexColor = false;
        showMaterialInspector(currentMat);
      }
    }
  }
}

void
GUI::showMaterialInspector(const WPtr<Material> wpMat)
{
  if (wpMat.expired()) {
    return;
  }
  auto currentMat = wpMat.lock();

  auto& pBaseColor = currentMat->baseColor;
  auto& pNormal = currentMat->normal;
  auto& pMetallic = currentMat->metallic;
  auto& pRoughness = currentMat->roughness;

  // Base Color
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pBaseColor), ImVec2(64, 64));
  /*if(ImGui::ImageButton("##BaseColorSelection",
                        reinterpret_cast<ImTextureID*>(&pBaseColor),
                        ImVec2(64, 64))) {

  }*/
  ImGui::SameLine();
  String buttonID = "##ColorButton" + currentMat->name;
  Vector3& baseColor = currentMat->baseColorFactor;
  if (ImGui::ColorButton(buttonID.c_str(), ImVec4(baseColor.x,
    baseColor.y,
    baseColor.z,
    1.0f)))
  {
    m_bTexColor = m_bTexColor ? false : true;
  }
  ImGui::SameLine();
  bool bHasDiffuseMap = currentMat->m_properties.bHasDiffuseMap;
  ImGui::Checkbox("BaseColor", &bHasDiffuseMap);
  currentMat->m_properties.bHasDiffuseMap = bHasDiffuseMap;

  if (m_bTexColor) {
    float texColor[3] = { baseColor.x, baseColor.y, baseColor.z };
    ImGui::Begin("Color Picker", 0, ImGuiWindowFlags_NoTitleBar);
    texColor[0] = baseColor.x;
    texColor[1] = baseColor.y;
    texColor[2] = baseColor.z;
    ImGui::ColorPicker3("TexColor", texColor);
    baseColor.x = texColor[0];
    baseColor.y = texColor[1];
    baseColor.z = texColor[2];
    ImGui::End();
  }

  // Normal
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pNormal), ImVec2(64, 64));
  ImGui::SameLine();
  bool bHasNormalMap = currentMat->m_properties.bHasNormalMap;
  ImGui::Checkbox("Normal", &bHasNormalMap);
  currentMat->m_properties.bHasNormalMap = bHasNormalMap;

  // Metallic
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pMetallic), ImVec2(64, 64));
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Metallic Factor",
    &currentMat->metallicRoughnessFactor.x,
    0.01f,
    0.0f,
    1.0f);
  ImGui::SameLine();
  bool bHasMetallicMap = currentMat->m_properties.bHasMetalnessMap;
  ImGui::Checkbox("Metallic", &bHasMetallicMap);
  currentMat->m_properties.bHasMetalnessMap = bHasMetallicMap;

  // Roughness
  ImGui::Image(reinterpret_cast<ImTextureID*>(&pRoughness), ImVec2(64, 64));
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Roughness Factor",
    &currentMat->metallicRoughnessFactor.y,
    0.01f,
    0.0f,
    1.0f);
  ImGui::SameLine();
  bool bHasRoughnessMap = currentMat->m_properties.bHasRoughnessMap;
  ImGui::Checkbox("Roughness", &bHasRoughnessMap);
  currentMat->m_properties.bHasRoughnessMap = bHasRoughnessMap;
}

void
GUI::showSkyBoxComponent(const WPtr<SkyBoxComponent> wpSkyBox)
{

}

void
GUI::showLightComponent()
{

}

void
GUI::showCameraComponent()
{

}

void
GUI::showColliderComponent(const WPtr<ColliderComponent> wpCollider)
{

}

void
GUI::showRigidbodyComponent(const WPtr<RigidbodyComponent> wpRigidbody)
{

}
}
