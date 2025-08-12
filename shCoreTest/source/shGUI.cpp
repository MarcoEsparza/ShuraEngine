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

//#include <shGraphicsManager.h>
#include <shRenderManager.h>
#include <shSceneGraph.h>
#include <shResourceManager.h>
#include <shFileExplorer.h>
#include <shTexture.h>
#include <shMath.h>

#include <shComponent.h>
#include <shTransformComponent.h>
#include <shMeshComponent.h>
#include <shSkyBoxComponent.h>
#include <shColliderComponent.h>
#include <shRigidbodyComponent.h>

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
  m_lightPos = { 0.0f, 100.0f, 0.0f, 1.0f };
  m_lightTarget = Vector3::ZERO;
  m_lcamNear = 0.1f;
  m_lcamFar = 1000.0f;
  m_lcamSize = 1000.0f;
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

  ImGui::Begin("Console");
  ImGui::Text("Message1");
  /*static char buffer[256] = "This is a test";
  if (ImGui::InputText("##TestText",
                       buffer,
                       IM_ARRAYSIZE(buffer),
                       ImGuiInputTextFlags_EnterReturnsTrue |
                       ImGuiInputTextFlags_AutoSelectAll)) {
  }
  ImGui::Text(buffer);*/
  ImGui::End();

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
GUI::setRendererSettings()
{
  RenderManager& renderMan = g_renderMan();
  auto& rendererSettings = renderMan.getShaderData();

  ImGui::Begin("RendererSettings");
  String strCount = std::to_string(m_fpsCountGUI);
  String text = strCount + ": fps";
  ImGui::Text(text.c_str());
  ImGui::SetNextItemWidth(150.0f);
  if (ImGui::Button("Recompile Shaders")) {
    renderMan.recompileShaders();
  }
  if (ImGui::CollapsingHeader("Ambient Occlusion")) {
    ImGui::DragFloat("AO sampler rad", &rendererSettings.sampleRadius, 0.1f, 0.0f, 5.0f);
    ImGui::DragFloat("AO scale", &rendererSettings.aoScale, 0.1f, 0.0f, 5.0f);
    ImGui::DragFloat("AO bias", &rendererSettings.aoBias, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("AO intensity", &rendererSettings.aoIntensity, 0.1f, 0.0f, 5.0f);
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

  if (ImGui::CollapsingHeader("Shadows")) {
    if (ImGui::CollapsingHeader("Light settings")) {
      // Light Position
      ImGui::Text("Light Pos:");
      // Position X
      ImGui::SameLine(80.0f);
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("x##LPosX", &m_lightPos.x, 1.0f);
      ImGui::PopStyleColor(3);
      // Position Y
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("y##LPosY", &m_lightPos.y, 1.0f);
      ImGui::PopStyleColor(3);
      // Position Z
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("z##LPosZ", &m_lightPos.z, 1.0f);
      ImGui::PopStyleColor(3);

      // Light Target
      ImGui::Text("Light Target:");
      // Position X
      ImGui::SameLine(80.0f);
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(180, 50, 50, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(200, 70, 70, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(200, 70, 70, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("x##LTarX", &m_lightTarget.x, 1.0f);
      ImGui::PopStyleColor(3);
      // Position Y
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 50, 150, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 70, 170, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 70, 170, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("y##LTarY", &m_lightTarget.y, 1.0f);
      ImGui::PopStyleColor(3);
      // Position Z
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(50, 150, 50, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(70, 170, 70, 150));
      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(70, 170, 70, 150));
      ImGui::SetNextItemWidth(50.0f);
      ImGui::DragFloat("z##LTarZ", &m_lightTarget.z, 1.0f);
      ImGui::PopStyleColor(3);

      ImGui::Spacing();
      ImGui::DragFloat("Light Cam Near:", &m_lcamNear, 0.1f);
      ImGui::Spacing();
      ImGui::DragFloat("Light Cam Far:", &m_lcamFar, 1.0f);
      ImGui::Spacing();
      ImGui::DragFloat("Light Cam Size:", &m_lcamSize, 1.0f);
    }
  }

  if (ImGui::CollapsingHeader("Camera Settings")) {
    ImGui::DragFloat("Camera Speed", &m_camSpeed, 1.0f, 1.0f, 300.0f);
    ImGui::DragFloat("Camera FOV", &m_camFov, 1.0f, 1.0f, 180.0f);
  }

  ImGui::End();

  renderMan.updateShaderDataBuffer();
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
    if (ImGui::MenuItem("Add Child...")) {
      SPtr<GameObject> child = sh_makeShared<GameObject>();
      child->name = "NewChild";
      child->parent = pNode;
      pNode->addChild(child);
      //if (ImGui::BeginPopupContextItem()) {
      //  if (ImGui::MenuItem("Create empty child")) {
      //    SPtr<GameObject> child = sh_makeShared<GameObject>();
      //    child->name = "NewChild";
      //    child->parent = pNode;
      //    pNode->addChild(child);
      //  }
      //  if (ImGui::MenuItem("Create Cube")) {
      //    /*SPtr<GameObject> child = sh_makeShared<GameObject>();
      //    child->name = "Cube";
      //    child->parent = pNode;
      //    pNode->addChild(child);*/
      //  }
      //  ImGui::EndPopup();
      //}
    }
    if (ImGui::MenuItem("Rename")) {
      
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
    showSceneGraph(gameObject, m_pActiveGameObject, renamingObject);
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

  if (wpSMesh.expired()) {
    return;
  }

  auto pMesh = wpSMesh.lock();

  if (ImGui::CollapsingHeader("Static Mesh Component")) {
    if (ImGui::Button("Save Mesh to cache")) {
      resMan.saveResourceToAsset(pMesh->m_mesh);
    }
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
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  if (wpMat.expired()) {
    return;
  }
  auto currentMat = wpMat.lock();

  auto& pBaseColor = currentMat->baseColor;
  auto& pNormal = currentMat->normal;
  auto& pMetallic = currentMat->metallic;
  auto& pRoughness = currentMat->roughness;

  bool bHasAlpha = currentMat->m_properties.bHasAlphaTest;
  ImGui::Checkbox("Alpha testing", &bHasAlpha);
  currentMat->m_properties.bHasAlphaTest = bHasAlpha;

  // Base Color
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pBaseColor), ImVec2(64, 64));
  if(ImGui::ImageButton("##BaseColorSelection",
                        reinterpret_cast<ImTextureID*>(&pBaseColor),
                        ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::rePointer<ImageResource>(pRes);
      if (pImg) {
        pBaseColor = pImg->texture;
      }
    }
  }
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
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pNormal), ImVec2(64, 64));
  if (ImGui::ImageButton("##NormalSelection",
    reinterpret_cast<ImTextureID*>(&pNormal),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::rePointer<ImageResource>(pRes);
      if (pImg) {
        pNormal = pImg->texture;
      }
    }
  }
  ImGui::SameLine();
  bool bHasNormalMap = currentMat->m_properties.bHasNormalMap;
  ImGui::Checkbox("Normal", &bHasNormalMap);
  currentMat->m_properties.bHasNormalMap = bHasNormalMap;

  // Metallic
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pMetallic), ImVec2(64, 64));
  if (ImGui::ImageButton("##MetallicSelection",
    reinterpret_cast<ImTextureID*>(&pMetallic),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::rePointer<ImageResource>(pRes);
      if (pImg) {
        pMetallic = pImg->texture;
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
  bool bHasMetallicMap = currentMat->m_properties.bHasMetalnessMap;
  ImGui::Checkbox("Metallic", &bHasMetallicMap);
  currentMat->m_properties.bHasMetalnessMap = bHasMetallicMap;

  // Roughness
  //ImGui::Image(reinterpret_cast<ImTextureID*>(&pRoughness), ImVec2(64, 64));
  if (ImGui::ImageButton("##RoughnessSelection",
    reinterpret_cast<ImTextureID*>(&pRoughness),
    ImVec2(64, 64))) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::rePointer<ImageResource>(pRes);
      if (pImg) {
        pRoughness = pImg->texture;
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
  bool bHasRoughnessMap = currentMat->m_properties.bHasRoughnessMap;
  ImGui::Checkbox("Roughness", &bHasRoughnessMap);
  currentMat->m_properties.bHasRoughnessMap = bHasRoughnessMap;
}

void
GUI::showSkyBoxComponent(const WPtr<SkyBoxComponent> wpSkyBox)
{
  RenderManager& renderMan = g_renderMan();
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  if (wpSkyBox.expired()) {
    return;
  }
  auto pSkyBox = wpSkyBox.lock();
  if (ImGui::CollapsingHeader("SkyBox Component")) {
    if (ImGui::DragFloat("Skyblur",
                         &renderMan.getPrefilteredIBLData().roughness,
                         0.01, 0.0f, 1.0f)) {
      renderMan.updatePrefilteredIBLBuffer();
    }

    if(ImGui::Button("Load Image")) {
      String filePath;
      if(fileExp.openFile(filePath, ".hdr", "resources/textures/")) {
        auto pRes = resMan.loadResourceFromFile(Path(filePath));
        auto pImg = cast::rePointer<ImageResource>(pRes);
        if (pImg) {
          pSkyBox->setSkyBoxResource(pImg);
          renderMan.computeIBL();
        }
      }
    }
  }
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

}
}
