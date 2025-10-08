/*****************************************************************************/
/*
*  @file    shGUI.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/10/02
*  @brief   Graphical User Interface (GUI) system for editor.
*
*  Graphical User Interface (GUI) system for editor.
*
*  @bug     No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shGUI.h"
#include "imgui_impl_shura.h"

//#include <shGraphicsManager.h>
#include <shRenderManager.h>
#include <shShaderManager.h>
#include <shSceneGraph.h>
#include <shResourceManager.h>
#include <shFileExplorer.h>
//#include <shLogger.h>
#include <shTexture.h>
#include <shMath.h>
#include <shException.h>
#include <shTimer.h>

#include <shGameObject.h>
#include <shComponent.h>
#include <shTransformComponent.h>
#include <shMeshComponent.h>
#include <shSkyBoxComponent.h>
#include <shColliderComponent.h>
#include <shRigidbodyComponent.h>
#include <shLightComponent.h>

#include <shImageResource.h>

#include <shMeshResource.h>

#define COLOR_LIMIT                                           255.0f
#define NORM_COLOR                                            1.0f / 255.0f

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

  m_camSpeed = 100.0f;
  m_camFov = 30.0f;
  m_camNear = 0.1f;
  m_camFar = 2000.0f;
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
  setRendererSettings();

  ImGui::Begin("ResourceManager");
  ImGui::Text("Resource1");
  ImGui::End();

  setConsoleLogs();

  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
  ImGui::Begin("Scene");
  ImGui::PopStyleColor();

  if (ImGui::Button(m_bPlay ? "Stop" : "Play")) {
    m_bPlay = !m_bPlay;
  }

  /*ImVec2 content_min = ImGui::GetWindowContentRegionMin();
  ImVec2 content_max = ImGui::GetWindowContentRegionMax();
  ImVec2 window_pos = ImGui::GetWindowPos();

  ImVec2 scene_pos = ImVec2(window_pos.x + content_min.x, window_pos.y + content_min.y);
  ImVec2 scene_size = ImVec2(content_max.x - content_min.x, content_max.y - content_min.y);

  ImGui::SetCursorScreenPos(scene_pos);*/

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

    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, &left, &main);
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, &right, &main);
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.2f, &bottom, &center);

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

void
GUI::setConsoleLogs()
{
  ImGui::Begin("Console");
  
  for (const auto& log : m_logs) {
    ImGui::TextWrapped("%s", log.c_str());
  }

  ImGui::End();
}

void
GUI::setRendererSettings()
{
  //RenderManager& renderMan = g_renderMan();
  ShaderManager& shaderMan = g_shaderMan();
  auto& rendererSettings = shaderMan.m_shaderData;

  ImGui::Begin("RendererSettings");
  String strCount = std::to_string(m_fpsCountGUI);
  String text = strCount + ": fps";
  ImGui::Text(text.c_str());
  ImGui::SetNextItemWidth(150.0f);
  if (ImGui::Button("Recompile Shaders")) {
    shaderMan.recompileShaders();
  }
  if (ImGui::CollapsingHeader("SSAO")) {
    ImGui::Checkbox("Enable SSAO", &m_bSSAO);
    ImGui::DragFloat("Sampler radius", &rendererSettings.sampleRadius,
                     0.1f, 0.0f, 5.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat("Scale", &rendererSettings.aoScale, 0.1f, 0.0f, 5.0f);
    ImGui::DragFloat("Bias", &rendererSettings.aoBias, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Intensity", &rendererSettings.aoIntensity, 0.1f, 0.0f, 5.0f);
  }
  if (ImGui::CollapsingHeader("Tone Mapping")) {
    const char* toneMapType[] = {
      "Reinhard", "ACES", "Uncharted2", "AgX", "Guardians-LogC4(LUT)", "LBK-K-Tone",
    };
    int32 toneMapIndex = static_cast<int32>(rendererSettings.toneMappingIndex);
    ImGui::Combo("##ToneMappingCombo",
                 &toneMapIndex,
                 toneMapType,
                 IM_ARRAYSIZE(toneMapType));
    rendererSettings.toneMappingIndex = static_cast<float>(toneMapIndex);

    ImGui::Spacing();
    ImGui::DragFloat("Bright Threshold:",
                     &rendererSettings.brightThreshold,
                     0.01f, 0.0f, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloat("White Point:",
                     &rendererSettings.whitePoint,
                     0.01f, 0.5f, 11.2f);
    ImGui::Spacing();
    ImGui::DragFloat("Bloom Multiplier:",
                     &rendererSettings.bloomMultiplier,
                     0.01f, 0.5f, 2.0f);
    ImGui::Spacing();
    ImGui::DragFloat("MiddleGrey:",
                     &rendererSettings.middleGrey,
                     0.01f, 0.5f, 2.0f);
  }
  if (ImGui::CollapsingHeader("Post-Process")) {
    float minR = rendererSettings.minR * COLOR_LIMIT;
    float maxR = rendererSettings.maxR * COLOR_LIMIT;
    float minG = rendererSettings.minG * COLOR_LIMIT;
    float maxG = rendererSettings.maxG * COLOR_LIMIT;
    float minB = rendererSettings.minB * COLOR_LIMIT;
    float maxB = rendererSettings.maxB * COLOR_LIMIT;

    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Min R:", &minR, 1.0f, 0.0f, COLOR_LIMIT);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Max R:", &maxR, 1.0f, 0.0f, COLOR_LIMIT);
    ImGui::Spacing();
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Min G:", &minG, 1.0f, 0.0f, COLOR_LIMIT);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Max G:", &maxG, 1.0f, 0.0f, COLOR_LIMIT);
    ImGui::Spacing();
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Min B:", &minB, 1.0f, 0.0f, COLOR_LIMIT);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("Max B:", &maxB, 1.0f, 0.0f, COLOR_LIMIT);

    rendererSettings.minR = minR * NORM_COLOR;
    rendererSettings.maxR = maxR * NORM_COLOR;
    rendererSettings.minG = minG * NORM_COLOR;
    rendererSettings.maxG = maxG * NORM_COLOR;
    rendererSettings.minB = minB * NORM_COLOR;
    rendererSettings.maxB = maxB * NORM_COLOR;
  }

  if (ImGui::CollapsingHeader("Camera Settings")) {
    ImGui::DragFloat("Camera Speed", &m_camSpeed, 1.0f, 1.0f, 300.0f);
    ImGui::DragFloat("Camera FOV", &m_camFov, 1.0f, 1.0f, 180.0f);
    ImGui::DragFloat("Camera Near Plane", &m_camNear, 0.1f, 0.01f, 100.0f);
    ImGui::DragFloat("Camera Far Plane", &m_camFar, 1.0f, 100.0f, 10000.0f);
  }

  ImGui::End();

  if (m_bSSAO) {
    shaderMan.m_shaderData.ssaoEnabled = 1.0f;
  }
  else {
    shaderMan.m_shaderData.ssaoEnabled = 0.0f;
  }

  shaderMan.updateShaderDataCB();
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

void
GUI::showSceneGraph(const SPtr<GameObject>& pNode,
                    SPtr<GameObject>& pSelectedObj,
                    int32& matSelection)
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

  if (m_bRename && m_pRenamingGameObject == pNode) {
    if (ImGui::InputText("##rename", pSelectedObj->name.data(), 256,
      ImGuiInputTextFlags_AutoSelectAll |
      ImGuiInputTextFlags_EnterReturnsTrue)) {
      m_bRename = false;
      m_pRenamingGameObject = nullptr;
    }
  }
  else {
    opened = ImGui::TreeNodeEx(pNode->name.c_str(), flags);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      pSelectedObj = pNode;
      matSelection = -1; // Reset material selection when selecting a new object
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      m_pRenamingGameObject = pNode;
      m_bRename = true;
    }
  }

  // Context menu
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Add Child...")) {
      SPtr<GameObject> child = sh_makeShared<GameObject>();
      child->name = "NewChild";
      child->parent = pNode;
      pNode->addChild(child);
    }
    if (ImGui::MenuItem("Rename")) {
      //m_pRenamingGameObject = pNode;
      m_bRename = true;
      m_pRenamingGameObject = pSelectedObj;
    }
    if (ImGui::MenuItem("Delete")) {
      
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
      showSceneGraph(child, pSelectedObj, matSelection);
    }
    ImGui::TreePop();
  }

  ImGui::PopID();
}

void
GUI::setSceneGraph()
{
  SceneGraph& scene = g_sceneGraph();

  //static SPtr<GameObject> renamingObject;

  ImGui::Begin("Scenegraph");

  // Detect right clic on empty window
  if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    if (!ImGui::IsAnyItemHovered()) {
      ImGui::OpenPopup("HierarchyContextMenu");
    }
  }

  // Empty context menu
  if (ImGui::BeginPopup("HierarchyContextMenu")) {
    if (ImGui::MenuItem("Create Empty Object")) {
      scene.createEmptyObject("NewObject");
    }
    if (ImGui::MenuItem("Create Cube")) {
      scene.createCubeObject();
    }
    ImGui::EndPopup();
  }

  // Draw hierarchy
  for (auto& gameObject : scene.getGameObjectList()) {
    showSceneGraph(gameObject, m_pActiveGameObject, m_selectedMat);
  }

  ImGui::End();
}

void
GUI::setComponentInspector()
{
  ImGui::Begin("Inspector");
  if (m_pActiveGameObject) {
    ImGui::Checkbox("Active", &m_pActiveGameObject->m_bActive);
    showTransformComponent();
    for (auto& pComponent : m_pActiveGameObject->components) {
      COMPONENT_TYPE::E type = pComponent->getType();
      if (type == COMPONENT_TYPE::kStaticMesh) {
        showStaticMeshComponent(sh_reinterpretPCast<StaticMeshComponent>(pComponent));
      }
      else if (type == COMPONENT_TYPE::kSkeletalMesh) {

      }
      else if (type == COMPONENT_TYPE::kSkyBox) {
        showSkyBoxComponent(sh_reinterpretPCast<SkyBoxComponent>(pComponent));
      }
      else if (type == COMPONENT_TYPE::kCamera) {

      }
      else if (type == COMPONENT_TYPE::kCollider) {
        showColliderComponent(sh_reinterpretPCast<ColliderComponent>(pComponent));
      }
      else if (type == COMPONENT_TYPE::kRigidbody) {

      }
      else if (type == COMPONENT_TYPE::kAnimator) {

      }
      else if (type == COMPONENT_TYPE::kLight) {
        showLightComponent(cast::re_ptr<LightComponent>(pComponent));
      }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("ComponentMenu");
    }

    if (ImGui::BeginPopup("ComponentMenu")) {
      if (ImGui::MenuItem("StaticMesh")) {
        addStaticMeshComponentToObject(m_pActiveGameObject);
      }
      if (ImGui::MenuItem("SkeletalMesh")) {

      }
      if (ImGui::MenuItem("Animator")) {

      }
      if (ImGui::MenuItem("Audio Source")) {

      }
      if (ImGui::MenuItem("Audio Listener")) {

      }
      if (ImGui::MenuItem("Collider")) {

      }
      if (ImGui::MenuItem("Rigidbody")) {

      }
      if (ImGui::MenuItem("Camera")) {

      }
      if (ImGui::MenuItem("Light")) {

      }
      if (ImGui::MenuItem("Skybox")) {

      }
      ImGui::EndPopup();
    }
  }
  ImGui::End();
}

void
GUI::showTransformComponent()
{
  Vector3 modelPos = m_pActiveGameObject->getPosition();
  Vector3 modelRot = m_pActiveGameObject->getRotation() * Math::RAD2DEG;
  Vector3 modelScale = m_pActiveGameObject->getScale();

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

  if (modelPos != m_pActiveGameObject->getPosition()) {
    m_pActiveGameObject->setPosition(modelPos);
  }
  if (modelRot != m_pActiveGameObject->getRotation()) {
    m_pActiveGameObject->setRotation(modelRot * Math::DEG2RAD);
  }
  if (modelScale != m_pActiveGameObject->getScale()) {
    m_pActiveGameObject->setScale(modelScale);
  }
}

void
GUI::showStaticMeshComponent(const WPtr<StaticMeshComponent> wpSMesh)
{
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  if (wpSMesh.expired()) {
    return;
  }

  auto pMesh = wpSMesh.lock();

  if (ImGui::CollapsingHeader("Static Mesh Component")) {
    // Mesh selection
    if (ImGui::Button("Select Mesh")) {
      String filePath;
      if (fileExp.openFile(filePath, ".hdr", "resources/models/")) {
        Timer timer;
        float timeStart = timer.getTime();
        auto pRes = resMan.loadResourceFromFile(Path(filePath));
        auto pStaticMesh = cast::re_ptr<StaticMeshResource>(pRes);
        if (pStaticMesh) {
          pMesh->setMeshData(pStaticMesh);
          m_selectedMat = -1; // Reset material selection when changing mesh
        }
        float timeEnd = timer.getTime();
        float loadTime = timeEnd - timeStart;
        SystemPath pathObj(filePath);
        String log = "Loaded " +
                     pathObj.filename().string() +
                     " in " +
                     std::to_string(loadTime) +
                     " seconds.";
        m_logs.push_back(log);
      }
    }

    // Save to cache button
    if (ImGui::Button("Save Mesh to cache")) {
      if (!pMesh->m_mesh.expired()) {
        resMan.saveResourceToAsset(pMesh->m_mesh.lock());
      }
    }

    // If no mesh is assigned, show message and return
    if (pMesh->m_mesh.expired()) {
      // Show empty mesh info
      ImGui::Text("Material Count: 0");
      ImGui::Text("Vertex Count: 0");
      ImGui::Text("Index Count: 0");
      return;
    }
    auto pMeshRes = pMesh->m_mesh.lock();

    // Show mesh info
    ImGui::Text("Material Count: %d", static_cast<uint32>(pMeshRes->m_materials.size()));
    uint32 vertexCount = 0;
    uint32 indexCount = 0;
    for (uint32 i = 0; i < pMeshRes->m_meshes.size(); ++i) {
      vertexCount += pMeshRes->m_meshes[i].numVertices;
      indexCount += pMeshRes->m_meshes[i].numIndices;
    }
    ImGui::Text("Vertex Count: %d", vertexCount);
    ImGui::Text("Index Count: %d", indexCount);

    // Mesh visibility
    ImGui::Spacing();
    if(ImGui::CollapsingHeader("Mesh Visibility")) {
      for (auto& mesh : pMeshRes->m_meshes) {
        // Show visibility toggle for each mesh
        ImGui::Checkbox(mesh.name.c_str(), &mesh.bVisible);
      }
    }

    // Material inspector
    ImGui::Spacing();
    ImGui::Text("Materials:");
    ImGui::Spacing();
    for (int32 i = 0; i < pMeshRes->m_materials.size(); ++i) {
      auto currentMat = pMeshRes->m_materials[i].lock();
      String matName = "Material:" + currentMat->getName();
      bool bSelected = (m_selectedMat == i);

      ImGui::SetNextItemOpen(bSelected, ImGuiCond_Always);
      if (ImGui::CollapsingHeader(matName.c_str())) {
        if (!bSelected) {
          m_selectedMat = i;
        }
        showMaterialInspector(currentMat);
      }
      else {
        if (bSelected) {
          m_selectedMat = -1; // Deselect if the header is not open
        }
      }
    }
  }
}

void
GUI::showMaterialInspector(const WPtr<Material> wpMat)
{
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  if (wpMat.expired()) {
    return;
  }
  auto currentMat = wpMat.lock();

  auto& pBaseColorImg = currentMat->m_baseColor;
  auto& pNormalImg = currentMat->m_normal;
  auto& pMetalnessImg = currentMat->m_metalness;
  auto& pRoughnessImg = currentMat->m_roughness;
  auto& pAOImg = currentMat->m_ao;
  auto& pEmissiveImg = currentMat->m_emissive;

  auto& pBaseColor = currentMat->m_baseColor.lock()->texture;
  auto& pNormal = currentMat->m_normal.lock()->texture;
  auto& pMetalness = currentMat->m_metalness.lock()->texture;
  auto& pRoughness = currentMat->m_roughness.lock()->texture;
  auto& pAO = currentMat->m_ao.lock()->texture;
  auto& pEmissive = currentMat->m_emissive.lock()->texture;

  bool bHasAlpha = currentMat->m_properties.properties.flags.bHasAlphaTest;
  ImGui::Checkbox("Alpha testing", &bHasAlpha);
  currentMat->m_properties.properties.flags.bHasAlphaTest = bHasAlpha;

  // Base Color
  if(ImGui::ImageButton("##BaseColorSelection",
                        reinterpret_cast<ImTextureID*>(&pBaseColor),
                        ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg->texture) {
        pBaseColorImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  String buttonID = "##ColorButton" + currentMat->getName();
  Vector3& baseColor = currentMat->baseColorFactor;
  ImVec4 currentColor = ImVec4(baseColor.x,
                               baseColor.y,
                               baseColor.z,
                               1.0f);
  if (ImGui::ColorButton(buttonID.c_str(), currentColor))
  {
    ImGui::OpenPopup("ColorPickerPopup");
  }
  ImGui::SameLine();
  bool bHasDiffuseMap = currentMat->m_properties.properties.flags.bHasDiffuseMap;
  ImGui::Checkbox("BaseColor", &bHasDiffuseMap);
  currentMat->m_properties.properties.flags.bHasDiffuseMap = bHasDiffuseMap;

  if (ImGui::BeginPopup("ColorPickerPopup")) {
    ImGui::ColorPicker3("##picker", (float*)&currentColor);
    ImGui::EndPopup();
  }
  baseColor.x = currentColor.x;
  baseColor.y = currentColor.y;
  baseColor.z = currentColor.z;

  // Normal
  if (ImGui::ImageButton("##NormalSelection",
    reinterpret_cast<ImTextureID*>(&pNormal),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg) {
        pNormalImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  bool bHasNormalMap = currentMat->m_properties.properties.flags.bHasNormalMap;
  ImGui::Checkbox("Normal", &bHasNormalMap);
  currentMat->m_properties.properties.flags.bHasNormalMap = bHasNormalMap;

  // Metallic
  if (ImGui::ImageButton("##MetalnessSelection",
    reinterpret_cast<ImTextureID*>(&pMetalness),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg) {
        pMetalnessImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Metallic Factor",
                   &currentMat->metallicRoughnessFactor.x,
                   0.01f,
                   0.0f,
                   1.0f);
  ImGui::SameLine();
  bool bHasMetallicMap = currentMat->m_properties.properties.flags.bHasMetalnessMap;
  ImGui::Checkbox("Metallic", &bHasMetallicMap);
  currentMat->m_properties.properties.flags.bHasMetalnessMap = bHasMetallicMap;

  // Roughness
  if (ImGui::ImageButton("##RoughnessSelection",
    reinterpret_cast<ImTextureID*>(&pRoughness),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg) {
        pRoughnessImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat("##Roughness Factor",
                   &currentMat->metallicRoughnessFactor.y,
                   0.01f,
                   0.0f,
                   1.0f);
  ImGui::SameLine();
  bool bHasRoughnessMap = currentMat->m_properties.properties.flags.bHasRoughnessMap;
  ImGui::Checkbox("Roughness", &bHasRoughnessMap);
  currentMat->m_properties.properties.flags.bHasRoughnessMap = bHasRoughnessMap;

  // Occlusion
  if(ImGui::ImageButton("##OcclusionSelection",
                        reinterpret_cast<ImTextureID*>(&pAO),
                        ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg) {
        pAOImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  bool bHasAO = currentMat->m_properties.properties.flags.bHasAmbientOcclusionMap;
  ImGui::Checkbox("Ambient Occlusion", &bHasAO);
  currentMat->m_properties.properties.flags.bHasAmbientOcclusionMap = bHasAO;

  // Emissive
  if(ImGui::ImageButton("##EmissiveSelection",
                        reinterpret_cast<ImTextureID*>(&pEmissive),
                        ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg->texture) {
        pEmissiveImg = pImg;
      }
    }
  }
  ImGui::SameLine();
  String emmButtonID = "##EmmColorButton" + currentMat->getName();
  Vector3& emmisiveColor = currentMat->emissiveFactor;
  ImVec4 currentEmmColor = ImVec4(emmisiveColor.x,
                                  emmisiveColor.y,
                                  emmisiveColor.z,
                                  1.0f);
  if (ImGui::ColorButton(emmButtonID.c_str(), currentEmmColor))
  {
    ImGui::OpenPopup("EmmColorPickerPopup");
  }
  //ImGui::SameLine();
  bool bUseEmmision = currentMat->m_properties.properties.flags.bUseEmission;
  ImGui::Checkbox("Use Emmisive", &bUseEmmision);
  currentMat->m_properties.properties.flags.bUseEmission = bUseEmmision;
  ImGui::SameLine();
  bool bHasEmissiveMap = currentMat->m_properties.properties.flags.bHasEmissiveMap;
  ImGui::Checkbox("Use Emmisive Map", &bHasEmissiveMap);
  currentMat->m_properties.properties.flags.bHasEmissiveMap = bHasEmissiveMap;
  ImGui::DragFloat("Emissive Intensity",
                   &currentMat->emmisiveIntensity,
                   0.01f,
                   0.0f,
                   10.0f);

  if (ImGui::BeginPopup("EmmColorPickerPopup")) {
    ImGui::ColorPicker3("##picker", (float*)&currentEmmColor);
    ImGui::EndPopup();
  }
  emmisiveColor.x = currentEmmColor.x;
  emmisiveColor.y = currentEmmColor.y;
  emmisiveColor.z = currentEmmColor.z;
}

void
GUI::showSkyBoxComponent(const WPtr<SkyBoxComponent> wpSkyBox)
{
  RenderManager& renderMan = g_renderMan();
  ShaderManager& shaderMan = g_shaderMan();
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  if (wpSkyBox.expired()) {
    return;
  }
  auto pSkyBox = wpSkyBox.lock();
  if (ImGui::CollapsingHeader("SkyBox Component")) {
    if (ImGui::DragFloat("Skyblur",
                         &shaderMan.m_prefilteredData.roughness,
                         0.01f, 0.0f, 1.0f)) {
      shaderMan.updatePrefilterShaderCB();
    }

    if(ImGui::Button("Load Image")) {
      String filePath;
      if(fileExp.openFile(filePath, ".hdr", "resources/textures/")) {
        auto pRes = resMan.loadResourceFromFile(Path(filePath));
        auto pImg = cast::re_ptr<ImageResource>(pRes);
        if (pImg) {
          pSkyBox->setSkyBoxResource(pImg);
          renderMan.computeIBL();
        }
      }
    }
  }
}

void
GUI::showLightComponent(const WPtr<LightComponent> wpLight)
{
  ShaderManager& shaderMan = g_shaderMan();

  if(wpLight.expired()) {
    return;
  }
  auto pLight = wpLight.lock();

  if (ImGui::CollapsingHeader("Light Component")) {
    const char* lightType[] = {
      "Directional", "Point", "Spot"
    };
    int32 lightTypeIndex = static_cast<int32>(pLight->m_lightType);
    ImGui::Combo("##LightTypeCombo",
                 &lightTypeIndex,
                 lightType,
                 IM_ARRAYSIZE(lightType));
    pLight->m_lightType = static_cast<LIGHT_TYPE::E>(lightTypeIndex);
    
    ImGui::Spacing();
    // Position
    ImGui::Text("Position:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##LPosX", &pLight->m_position.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##LPosY", &pLight->m_position.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##LPosZ", &pLight->m_position.z, 0.1f);
    ImGui::PopStyleColor(3);

    // Target
    ImGui::Text("Target:");
    // Position X
    ImGui::SameLine(80.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("x##LTPosX", &pLight->m_target.x, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Y
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("y##LTPosY", &pLight->m_target.y, 0.1f);
    ImGui::PopStyleColor(3);
    // Position Z
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
    ImGui::SetNextItemWidth(50.0f);
    ImGui::DragFloat("z##LTPosZ", &pLight->m_target.z, 0.1f);
    ImGui::PopStyleColor(3);

    // Color
    ImVec4 currentColor = ImVec4(pLight->m_color.r,
                                 pLight->m_color.g,
                                 pLight->m_color.b,
                                 1.0f);
    if (ImGui::ColorButton("##LightColor", currentColor))
    {
      ImGui::OpenPopup("LightColorPickerPopup");
    }
    if (ImGui::BeginPopup("LightColorPickerPopup")) {
      ImGui::ColorPicker3("##picker", (float*)&currentColor);
      ImGui::EndPopup();
    }
    pLight->m_color.r = currentColor.x;
    pLight->m_color.g = currentColor.y;
    pLight->m_color.b = currentColor.z;

    // Intensity
    ImGui::DragFloat("Intensity", &pLight->m_intensity, 0.1f, 0.0f, 10.0f);
  }

  // Update light buffer if any property changed
  bool bUpdated = false;
  if(shaderMan.m_lightData.position != pLight->m_position) {
    shaderMan.m_lightData.position = pLight->m_position;
    pLight->m_lightCamera.setPosition(Vector3(pLight->m_position.x,
                                              pLight->m_position.y,
                                              pLight->m_position.z));
    bUpdated = true;
  }
  if(shaderMan.m_lightData.target != pLight->m_target) {
    shaderMan.m_lightData.target = pLight->m_target;
    pLight->m_lightCamera.setTarget(Vector3(pLight->m_target.x,
                                            pLight->m_target.y,
                                            pLight->m_target.z));
    bUpdated = true;
  }
  if(shaderMan.m_lightData.intensity != pLight->m_intensity) {
    shaderMan.m_lightData.intensity = pLight->m_intensity;
    bUpdated = true;
  }
  if(shaderMan.m_lightData.color != pLight->m_color) {
    shaderMan.m_lightData.color = pLight->m_color;
    bUpdated = true;
  }
  /*if(shaderMan.m_lightData.view != pLight->m_lightCamera.getView()) {
    shaderMan.m_lightData.view = pLight->m_lightCamera.getView().getTransposed();
    bUpdated = true;
  }
  if(shaderMan.m_lightData.proj != pLight->m_lightCamera.getProjection()) {
    shaderMan.m_lightData.proj = pLight->m_lightCamera.getProjection().getTransposed();
    bUpdated = true;
  }*/

  if(bUpdated) {
    shaderMan.m_lightData.view = pLight->m_lightCamera.getView().getTransposed();
    shaderMan.m_lightData.proj = pLight->m_lightCamera.getProjection().getTransposed();
    shaderMan.updateLightCB();
  }
}

void
GUI::showCameraComponent()
{

}

void
GUI::showColliderComponent(const WPtr<ColliderComponent> wpCollider)
{
  if (wpCollider.expired()) {
    return;
  }
  auto pCollider = wpCollider.lock();
  if (ImGui::CollapsingHeader("Collider Component")) {
    // Collider Type selection
    int32 colliderType = static_cast<int32>(pCollider->m_collider.m_type);
    ImGui::Text("Collider Type:");
    const char* colliderTypeNames[] = {
      "None", "Sphere", "Capsule", "AABBox", "OBBox", "ConvexMesh"
    };
    ImGui::Combo("##ColliderTypeCombo",
                 &colliderType,
                 colliderTypeNames,
                 IM_ARRAYSIZE(colliderTypeNames));
    pCollider->m_collider.m_type = static_cast<COLLIDER_TYPE::E>(colliderType);

    // Show collider properties based on type
    if (colliderType == COLLIDER_TYPE::kOBBox) {
      ImGui::Text("Offset:");
      ImGui::Text("X");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxXOffset",
                       &pCollider->m_collider.m_box.center.x,
                       0.01f);
      ImGui::SameLine();
      ImGui::Text("Y");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxYOffset",
                       &pCollider->m_collider.m_box.center.y,
                       0.01f);
      ImGui::SameLine();
      ImGui::Text("Z");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxZOffset",
                       &pCollider->m_collider.m_box.center.z,
                       0.01f);

      ImGui::Text("Size:");
      ImGui::Text("X");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxXSize",
                       &pCollider->m_collider.m_box.extent.x,
                       0.01f);
      ImGui::SameLine();
      ImGui::Text("Y");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxYSize",
                       &pCollider->m_collider.m_box.extent.y,
                       0.01f);
      ImGui::SameLine();
      ImGui::Text("Z");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("##OBBoxZSize",
                       &pCollider->m_collider.m_box.extent.z,
                       0.01f);
    }
  }
}

void
GUI::showRigidbodyComponent(const WPtr<RigidbodyComponent> wpRigidbody)
{
  SH_UNREFERENCED_PARAMETER(wpRigidbody);
}

void
GUI::addStaticMeshComponentToObject(SPtr<GameObject>& pObj)
{
  if (!pObj) {
    return;
  }
  bool bHasSMesh = false;
  for (auto& pComp : pObj->components) {
    if (pComp->getType() == COMPONENT_TYPE::kStaticMesh) {
      bHasSMesh = true;
      break;
    }
  }
  if (!bHasSMesh) {
    SPtr<StaticMeshComponent> pSMeshComp = sh_makeShared<StaticMeshComponent>();
    pObj->addComponent(pSMeshComp);
  }
}
}
