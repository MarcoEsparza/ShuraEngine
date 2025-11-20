/*****************************************************************************/
/*
*  @file    shGUI.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/11/14
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
#include <shCameraComponent.h>
#include <shAnimatorComponent.h>

#include <shImageResource.h>

#include <shMeshResource.h>

#define COLOR_LIMIT                                           255.0f
#define NORM_COLOR                                            1.0f / 255.0f
#define ICON_SIZE                                             50.0f
#define ICONS_PER_ROW                                         10

using std::remove;
using std::strncpy;
using std::remove_if;

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Static Functions
*/
/*****************************************************************************/

static void
dragVector3(const String& label, Vector3& vec, float speed)
{
  ImVec4 red = ImVec4(0.8f, 0.2f, 0.2f, 0.6f);
  ImVec4 hoveredRed = ImVec4(0.9f, 0.3f, 0.3f, 0.8f);
  ImVec4 activeRed = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
  ImVec4 green = ImVec4(0.2f, 0.5f, 0.2f, 0.6f);
  ImVec4 hoveredGreen = ImVec4(0.3f, 0.65f, 0.3f, 0.8f);
  ImVec4 activeGreen = ImVec4(0.35f, 0.7f, 0.35f, 1.0f);
  ImVec4 blue = ImVec4(0.2f, 0.2f, 0.8f, 0.6f);
  ImVec4 hoveredBlue = ImVec4(0.3f, 0.3f, 0.9f, 0.8f);
  ImVec4 activeBlue = ImVec4(0.4f, 0.4f, 1.0f, 1.0f);

  String xLabel = "x##" + label;
  String yLabel = "y##" + label;
  String zLabel = "z##" + label;

  ImGui::PushStyleColor(ImGuiCol_FrameBg, red);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, activeRed);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, hoveredRed);
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat(xLabel.c_str(), &vec.x, speed);
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_FrameBg, green);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, activeGreen);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, hoveredGreen);
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat(yLabel.c_str(), &vec.y, speed);
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_FrameBg, blue);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, activeBlue);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, hoveredBlue);
  ImGui::SetNextItemWidth(50.0f);
  ImGui::DragFloat(zLabel.c_str(), &vec.z, speed);
  ImGui::PopStyleColor(3);
}

static bool
tablePropertyDragFloat(const String& label,
                       float& value,
                       float speed,
                       float min,
                       float max,
                       const char* format = "%.3f")
{
  bool changed = false;

  ImGui::TableNextRow();
  ImGui::TableSetColumnIndex(0);
  ImGui::Text(label.c_str());
  ImGui::TableNextColumn();
  changed = ImGui::DragFloat(("##" + label).c_str(), &value, speed, min, max, format);

  return changed;
}

static bool
tablePropertyCheckbox(const String& label, bool& value)
{
  bool changed = false;

  ImGui::TableNextRow();
  ImGui::TableSetColumnIndex(0);
  ImGui::Text(label.c_str());
  ImGui::TableNextColumn();
  changed = ImGui::Checkbox(("##" + label).c_str(), &value);

  return changed;
}

static bool
tablePropertyEnumCombo(const String& label,
                       int32& currentItem,
                       const char* const* items,
                       int32 arraySize)
{
  bool changed = false;

  ImGui::TableNextRow();
  ImGui::TableSetColumnIndex(0);
  ImGui::TextUnformatted(label.c_str());
  ImGui::TableNextColumn();
  String completeLabel = "##" + label;
  changed = ImGui::Combo(completeLabel.c_str(), &currentItem, items, arraySize);

  return changed;
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
textureFileButton(const String& buttonID,
                  WPtr<ImageResource>& textureImg,
                  ImVec2& buttonSize)
{
  ResourceManager& resMan = g_resourceMan();
  FileExplorer& fileExp = g_fileExplorer();

  auto& pTexture = textureImg.lock()->texture;
  ImTextureID* texID = cast::re<ImTextureID*>(&pTexture);

  if (ImGui::ImageButton(buttonID.c_str(), texID, buttonSize)) {
    String filePath;
    if (fileExp.openFile(filePath,
                         "PNGs(*.png)\0*.png\0",
                         "resources/textures/")) {
      auto pRes = resMan.loadResourceFromFile(Path(filePath));
      auto pImg = cast::re_ptr<ImageResource>(pRes);
      if (pImg->texture) {
        textureImg = pImg;
      }
    }
  }
}

static String
iconToStr(FONT_ICONS::E icon)
{
  static char buffer[8];
  ImTextCharToUtf8(buffer, cast::st<ImWchar>(icon));
  String result(buffer);
  return result;
}

/*****************************************************************************/
/*
*  Class Functions
*/
/*****************************************************************************/

void
GUI::init(const WPtr<Screen> pScreen)
{
  if(pScreen.expired()) {
    //SH_LOG_ERROR("GUI::init: Screen pointer is expired.");
    return;
  }
  auto& resMan = g_resourceMan();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplShura_Init(pScreen);
  setStyle();

  m_camSpeed = 100.0f;
  m_camFov = 30.0f;
  m_camNear = 0.1f;
  m_camFar = 2000.0f;

  Path resPath("resources/data/ImGui/ShuraIconsTexture.png");
  auto pRes = cast::re_ptr<ImageResource>(resMan.loadResourceFromFile(resPath));
  m_iconsTexture = pRes->texture;
  m_iconsSize = Vector2(cast::st<float>(pRes->width), cast::st<float>(pRes->height));

  m_sceneGraphWindowStr = iconToStr(FONT_ICONS::kSiteMap) + " Scenegraph";
  m_sceneWindowStr = iconToStr(FONT_ICONS::kPicture) + " Scene";
  m_inspectorWindowStr = iconToStr(FONT_ICONS::kInfoCircled) + " Inspector";
  m_renderSettWindowStr = iconToStr(FONT_ICONS::kCogAlt) + " RendererSettings";
  m_projectResWindowStr = iconToStr(FONT_ICONS::kFolderOpen) + " Project";
  m_consoleWindowStr = iconToStr(FONT_ICONS::kDocTextInv) + " Console";
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
  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();

  setDockSpace();

  setSceneGraph();
  setInspector();
  setRendererSettings();

  ImGui::Begin(m_projectResWindowStr.c_str());
  ImGui::Text("Resource1");
  ImGui::End();

  setConsoleLogs();

  setScene();
}

void
GUI::setDockSpace()
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  window_flags |= ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoBringToFrontOnFocus |
                  ImGuiWindowFlags_NoNavFocus;

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  if (!m_bOpenDockSpace) {
    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    ImGuiID mainID = dockspace_id;
    ImGuiID leftID = ImGui::DockBuilderSplitNode(mainID,
                                                 ImGuiDir_Left, 0.125f, NULL, &mainID);
    ImGuiID rightID = ImGui::DockBuilderSplitNode(mainID,
                                                  ImGuiDir_Right, 0.225f, NULL, &mainID);
    ImGuiID bottomID = ImGui::DockBuilderSplitNode(mainID,
                                                   ImGuiDir_Down, 0.25f, NULL, &mainID);

    ImGui::DockBuilderDockWindow(m_sceneGraphWindowStr.c_str(), leftID);
    ImGui::DockBuilderDockWindow(m_sceneWindowStr.c_str(), mainID);
    ImGui::DockBuilderDockWindow(m_inspectorWindowStr.c_str(), rightID);
    ImGui::DockBuilderDockWindow(m_renderSettWindowStr.c_str(), rightID);
    ImGui::DockBuilderDockWindow(m_projectResWindowStr.c_str(), bottomID);
    ImGui::DockBuilderDockWindow(m_consoleWindowStr.c_str(), bottomID);

    ImGui::DockBuilderFinish(dockspace_id);
    m_bOpenDockSpace = true;
  }

  ImGui::Begin("DockSpace", &m_bOpenDockSpace, window_flags);

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
    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Scene")) {
        // Handle scene window action
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("Documentation")) {
        // Handle documentation action
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
      if (ImGui::MenuItem("Shura Engine ver 0.4.3")) {
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

void
GUI::setScene()
{
  RenderManager& renderMan = g_renderMan();

  ImGui::Begin(m_sceneWindowStr.c_str(), &m_bOpenDockSpace, 0);

  /*if (ImGui::Button(m_bPlay ? "Stop" : "Play")) {
    m_bPlay = !m_bPlay;
  }*/

  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  m_sceneWindowSize = Vector2(windowSize.x, windowSize.y);
  renderMan.setScreenSize(m_sceneWindowSize);
  auto& pSceneTex = renderMan.createSceneTexture(m_sceneWindowSize);

  if (pSceneTex != nullptr) {
    ImGui::Image(cast::re<ImTextureID*>(&pSceneTex), windowSize);
  }

  m_bSceneWindowFocused = ImGui::IsWindowFocused();

  ImGui::End();
}

void
GUI::setConsoleLogs()
{
  ImGui::Begin(m_consoleWindowStr.c_str());
  
  for (const auto& log : m_logs) {
    ImGui::TextWrapped("%s", log.c_str());
  }

  ImGui::End();
}

void
GUI::setRendererSettings()
{
  ShaderManager& shaderMan = g_shaderMan();
  auto& rendererSettings = shaderMan.m_shaderData;

  ImGui::Begin(m_renderSettWindowStr.c_str());
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  float width = windowSize.x;
  float col0 = width * 0.35f;

  String strCount = std::to_string(m_fpsCountGUI);
  String text = strCount + ": fps";
  ImGui::Text(text.c_str());
  ImGui::SetNextItemWidth(150.0f);
  if (ImGui::Button("Recompile Shaders")) {
    shaderMan.recompileShaders();
  }
  if (ImGui::CollapsingHeader("SSAO")) {
    if(ImGui::BeginTable("SSAO Settings", 2)) {
      ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthFixed, col0);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      tablePropertyCheckbox("Enable SSAO:", m_bSSAO);
      tablePropertyDragFloat("Sampler Radius:", rendererSettings.sampleRadius,
                             0.1f, 0.0f, 5.0f);
      tablePropertyDragFloat("Scale:", rendererSettings.aoScale, 0.1f, 0.0f, 5.0f);
      tablePropertyDragFloat("Bias:", rendererSettings.aoBias, 0.01f, 0.0f, 1.0f);
      tablePropertyDragFloat("Intensity:", rendererSettings.aoIntensity, 0.1f, 0.0f, 5.0f);

      ImGui::EndTable();
    }
  }
  if (ImGui::CollapsingHeader("Tone Mapping")) {
    const char* toneMappingOptions[] = {
      "Reinhard", "ACES", "Uncharted2", "AgX", "Guardians-LogC4(LUT)", "LBK-K-Tone",
    };

    if (ImGui::BeginTable("Tone Mapping Settings", 2)) {
      ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthFixed, col0);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      int32 toneMappingIndex = cast::st<int32>(rendererSettings.toneMappingIndex);
      tablePropertyEnumCombo("Tone Mapping Type:",
                             toneMappingIndex,
                             toneMappingOptions,
                             IM_ARRAYSIZE(toneMappingOptions));
      rendererSettings.toneMappingIndex = cast::st<float>(toneMappingIndex);
      tablePropertyDragFloat("Bright Threshold:", rendererSettings.brightThreshold,
                             0.01f, 0.0f, 1.0f);
      tablePropertyDragFloat("White Point:",
                             rendererSettings.whitePoint,
                             0.01f, 0.5f, 11.0f);
      tablePropertyDragFloat("Bloom Multiplier:", rendererSettings.bloomMultiplier,
                             0.01f, 0.5f, 5.0f);
      tablePropertyDragFloat("MiddleGrey:", rendererSettings.middleGrey,
                             0.01f, 0.5f, 2.0f);
      tablePropertyDragFloat("Emissive Intensity:", rendererSettings.emmisiveIntensity,
                             0.1f, 0.0f, 10.0f);

      ImGui::EndTable();
    }
  }
  if (ImGui::CollapsingHeader("Post-Process")) {
    float minR = rendererSettings.minR * COLOR_LIMIT;
    float maxR = rendererSettings.maxR * COLOR_LIMIT;
    float minG = rendererSettings.minG * COLOR_LIMIT;
    float maxG = rendererSettings.maxG * COLOR_LIMIT;
    float minB = rendererSettings.minB * COLOR_LIMIT;
    float maxB = rendererSettings.maxB * COLOR_LIMIT;

    if(ImGui::BeginTable("Post-Process Settings", 4)) {
      ImGui::TableSetupColumn("Parameter1", ImGuiTableColumnFlags_WidthStretch, 1.0f);
      ImGui::TableSetupColumn("Value1", ImGuiTableColumnFlags_WidthStretch, 1.0f);
      ImGui::TableSetupColumn("Parameter2", ImGuiTableColumnFlags_WidthStretch, 1.0f);
      ImGui::TableSetupColumn("Value2", ImGuiTableColumnFlags_WidthStretch, 1.0f);

      tablePropertyDragFloat("Min red:", minR, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");
      ImGui::TableNextColumn();
      ImGui::Text("Max red:");
      ImGui::TableNextColumn();
      ImGui::DragFloat("##Max red:", &maxR, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");

      tablePropertyDragFloat("Min green:", minG, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");
      ImGui::TableNextColumn();
      ImGui::Text("Max green:");
      ImGui::TableNextColumn();
      ImGui::DragFloat("##Max green:", &maxG, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");

      tablePropertyDragFloat("Min blue:", minB, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");
      ImGui::TableNextColumn();
      ImGui::Text("Max blue:");
      ImGui::TableNextColumn();
      ImGui::DragFloat("##Max blue:", &maxB, 1.0f, 0.0f, COLOR_LIMIT, "%.1f");

      ImGui::EndTable();
    }

    rendererSettings.minR = minR * NORM_COLOR;
    rendererSettings.maxR = maxR * NORM_COLOR;
    rendererSettings.minG = minG * NORM_COLOR;
    rendererSettings.maxG = maxG * NORM_COLOR;
    rendererSettings.minB = minB * NORM_COLOR;
    rendererSettings.maxB = maxB * NORM_COLOR;
  }

  if (ImGui::CollapsingHeader("Camera Settings")) {
    if(ImGui::BeginTable("Camera Settings Table", 2)) {
      ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthFixed, col0);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      tablePropertyDragFloat("Speed:", m_camSpeed, 1.0f, 1.0f, 300.0f, "%.1f");
      tablePropertyDragFloat("FOV:", m_camFov, 1.0f, 1.0f, 180.0f, "%.1f");
      tablePropertyDragFloat("Near Plane:", m_camNear, 0.1f, 0.01f, 100.0f, "%.2f");
      tablePropertyDragFloat("Far Plane:", m_camFar, 1.0f, 100.0f, 10000.0f, "%.1f");

      ImGui::EndTable();
    }
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

  ImGui::Begin(m_sceneGraphWindowStr.c_str());
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("##ReadOnlyBox",
                    ImVec2(windowSize.x, ImGui::GetFrameHeight() + 5.0f), true,
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImGui::TextUnformatted((iconToStr(FONT_ICONS::kCubes) + " DefaultScene").c_str());
  ImGui::EndChild();

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
GUI::setInspector()
{
  ImGui::Begin(m_inspectorWindowStr.c_str());
  if (m_pActiveGameObject && !m_pActiveResource) {
    showGameObjectInspector();
  }
  else if(m_pActiveResource && !m_pActiveGameObject) {
    showResourceInspector();
  }
  ImGui::End();
}

void
GUI::showGameObjectInspector()
{
  float windowWidth = ImGui::GetContentRegionAvail().x;

  ImGui::Checkbox("Active", &m_pActiveGameObject->m_bActive);
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  showTransformComponent();
  for (auto& pComponent : m_pActiveGameObject->components) {
    COMPONENT_TYPE::E type = pComponent->getType();
    if (type == COMPONENT_TYPE::kStaticMesh) {
      showStaticMeshComponent(cast::re_ptr<StaticMeshComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kSkeletalMesh) {
      showSkeletalMeshComponent(cast::re_ptr<SkeletalMeshComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kSkyBox) {
      showSkyBoxComponent(cast::re_ptr<SkyBoxComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kCamera) {
      showCameraComponent(cast::re_ptr<CameraComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kCollider) {
      showColliderComponent(cast::re_ptr<ColliderComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kRigidbody) {
      showRigidbodyComponent(cast::re_ptr<RigidbodyComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kAnimator) {
      showAnimatorComponent(cast::re_ptr<AnimatorComponent>(pComponent));
    }
    else if (type == COMPONENT_TYPE::kLight) {
      showLightComponent(cast::re_ptr<LightComponent>(pComponent));
    }
  }

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  //ImGui::SetNextItemWidth(windowWidth);
  ImVec2 buttonSize = ImVec2(300.0f, 30.0f);
  ImGui::SetCursorPosX((windowWidth - buttonSize.x) * 0.5f);
  if (ImGui::Button("Add Component", buttonSize)) {
    ImGui::OpenPopup("ComponentMenu");
  }

  if (ImGui::BeginPopup("ComponentMenu")) {
    String staticMeshIcon = iconToStr(FONT_ICONS::kCube) + " StaticMesh";
    if (ImGui::MenuItem(staticMeshIcon.c_str())) {
      addStaticMeshComponentToObject(m_pActiveGameObject);
    }

    String skeletalMeshIcon = iconToStr(FONT_ICONS::kMale) + " SkeletalMesh";
    if (ImGui::MenuItem(skeletalMeshIcon.c_str())) {

    }

    String animatorIcon = iconToStr(FONT_ICONS::kChild) + " Animator";
    if (ImGui::MenuItem(animatorIcon.c_str())) {

    }

    String audioIcon = iconToStr(FONT_ICONS::kVolumeUp) + " Audio";
    if (ImGui::MenuItem(audioIcon.c_str())) {

    }

    String audioListenerIcon = iconToStr(FONT_ICONS::kHeadphones) + " Audio Listener";
    if (ImGui::MenuItem(audioListenerIcon.c_str())) {

    }

    String colliderIcon = iconToStr(FONT_ICONS::kCheckEmpty) + " Collider";
    if (ImGui::MenuItem(colliderIcon.c_str())) {

    }

    String rigidbodyIcon = iconToStr(FONT_ICONS::kShield) + " Rigidbody";
    if (ImGui::MenuItem(rigidbodyIcon.c_str())) {

    }

    String cameraIcon = iconToStr(FONT_ICONS::kVideoCamera) + " Camera";
    if (ImGui::MenuItem(cameraIcon.c_str())) {

    }

    String lightIcon = iconToStr(FONT_ICONS::kLightBulb) + " Light";
    if (ImGui::MenuItem(lightIcon.c_str())) {

    }

    String skyboxIcon = iconToStr(FONT_ICONS::kCloudSun) + " Skybox";
    if (ImGui::MenuItem(skyboxIcon.c_str())) {

    }
    ImGui::EndPopup();
  }
}

void
GUI::showResourceInspector()
{
  auto resType = m_pActiveResource->getType();

  switch (resType)
  { 
  case shEngineSDK::RESOURCE_TYPE::kTexture:
    break;
  case shEngineSDK::RESOURCE_TYPE::kStaticMesh:
    break;
  case shEngineSDK::RESOURCE_TYPE::kSkeletalMesh:
    break;
  case shEngineSDK::RESOURCE_TYPE::kSkeleton:
    break;
  case shEngineSDK::RESOURCE_TYPE::kAnmimation:
    break;
  case shEngineSDK::RESOURCE_TYPE::kCubeMap:
    break;
  case shEngineSDK::RESOURCE_TYPE::kMaterial:
    break;
  case shEngineSDK::RESOURCE_TYPE::kCount:
    break;
  default:
    break;
  }
}

void
GUI::showTransformComponent()
{
  Vector3 modelPos = m_pActiveGameObject->getPosition();
  Vector3 modelRot = m_pActiveGameObject->getRotation() * Math::RAD2DEG;
  Vector3 modelScale = m_pActiveGameObject->getScale();
  
  float windowWidth = ImGui::GetContentRegionAvail().x;

  String headerStr = iconToStr(FONT_ICONS::kShare) + " Transform";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
    // Position
    String posIcon = iconToStr(FONT_ICONS::kMove) + " Position:";
    ImGui::Text(posIcon.c_str());
    ImGui::SameLine(110.0f);
    dragVector3("Position", modelPos, 0.01f);

    // Rotation
    String rotIcon = iconToStr(FONT_ICONS::kArrowsCW) + " Rotation:";
    ImGui::Text(rotIcon.c_str());
    ImGui::SameLine(110.0f);
    dragVector3("Rotation", modelRot, 0.1f);

    // Scale
    String sclIcon = iconToStr(FONT_ICONS::kResizeFullAlt) + " Scale:";
    ImGui::Text(sclIcon.c_str());
    ImGui::SameLine(110.0f);
    dragVector3("Scale", modelScale, 0.01f);

    ImGui::Spacing();
    ImGui::Spacing();
    float bottonWidth = 150.0f;
    ImGui::SetCursorPosX((windowWidth - bottonWidth) * 0.5f);
    if (ImGui::Button("Reset", ImVec2(bottonWidth, 25.0f))) {
      modelPos = { 0.0f, 0.0f, 0.0f };
      modelRot = { 0.0f, 0.0f, 0.0f };
      modelScale = { 1.0f, 1.0f, 1.0f };
    }
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

  String headerStr = iconToStr(FONT_ICONS::kCube) + " Static Mesh Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
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
GUI::showSkeletalMeshComponent(const WPtr<SkeletalMeshComponent> wpSkelMesh)
{
  if(wpSkelMesh.expired()) {
    return;
  }

  String headerStr = iconToStr(FONT_ICONS::kMale) + " Skeletal Mesh Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {

  }
}

void
GUI::showMaterialInspector(const WPtr<Material> wpMat)
{
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
  auto& pOpacityImg = currentMat->m_opacityMask;

  float windowWidth = ImGui::GetContentRegionAvail().x;
  ImVec2 buttonSize = ImVec2(32.0f, 32.0f);

  // Base Color
  textureFileButton("##BaseColorSelection", pBaseColorImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pBaseColorImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pBaseColorImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  String buttonID = "##ColorButton" + currentMat->getName();
  Vector3& baseColor = currentMat->baseColorFactor;
  bool bHasDiffuseMap = currentMat->m_properties.properties.flags.bHasDiffuseMap;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  ImGui::Checkbox("BaseColor", &bHasDiffuseMap);
  currentMat->m_properties.properties.flags.bHasDiffuseMap = bHasDiffuseMap;

  ImVec4 currentColor = ImVec4(baseColor.x, baseColor.y, baseColor.z, 1.0f);
  ImGui::SameLine();
  float textHeight = ImGui::GetTextLineHeight();
  float frameHeight = ImGui::GetFrameHeight();
  float offsetY = (frameHeight - textHeight) * 0.5f;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY + 4.0f);
  if (ImGui::ColorButton(buttonID.c_str(),
                         currentColor,
                         ImGuiColorEditFlags_None,
                         ImVec2(windowWidth * 0.52f, 20.0f))) {
    ImGui::OpenPopup("ColorPickerPopup");
  }

  if (ImGui::BeginPopup("ColorPickerPopup")) {
    ImGui::ColorPicker3("##picker", (float*)&currentColor);
    ImGui::EndPopup();
  }
  baseColor.x = currentColor.x;
  baseColor.y = currentColor.y;
  baseColor.z = currentColor.z;

  // Normal
  textureFileButton("##NormalSelection", pNormalImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pNormalImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pNormalImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bHasNormalMap = currentMat->m_properties.properties.flags.bHasNormalMap;
  ImGui::Checkbox("Normal", &bHasNormalMap);
  currentMat->m_properties.properties.flags.bHasNormalMap = bHasNormalMap;

  // Metallic
  textureFileButton("##MetalnessSelection", pMetalnessImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pMetalnessImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pMetalnessImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bHasMetallicMap = currentMat->m_properties.properties.flags.bHasMetalnessMap;
  ImGui::Checkbox("Metallic", &bHasMetallicMap);
  currentMat->m_properties.properties.flags.bHasMetalnessMap = bHasMetallicMap;
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  ImGui::SetNextItemWidth(windowWidth * 0.55f);
  ImGui::DragFloat("##Metallic Factor", &currentMat->metallicRoughnessFactor.x,
                   0.01f, 0.0f, 1.0f);

  // Roughness
  textureFileButton("##RoughnessSelection", pRoughnessImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pRoughnessImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pRoughnessImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bHasRoughnessMap = currentMat->m_properties.properties.flags.bHasRoughnessMap;
  ImGui::Checkbox("Roughness", &bHasRoughnessMap);
  currentMat->m_properties.properties.flags.bHasRoughnessMap = bHasRoughnessMap;
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  ImGui::SetNextItemWidth(windowWidth * 0.55f);
  ImGui::DragFloat("##Roughness Factor", &currentMat->metallicRoughnessFactor.y,
                   0.01f, 0.0f, 1.0f);

  // Occlusion
  textureFileButton("##AOSelection", pAOImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pAOImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pAOImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bHasAO = currentMat->m_properties.properties.flags.bHasAmbientOcclusionMap;
  ImGui::Checkbox("Ambient Occlusion", &bHasAO);
  currentMat->m_properties.properties.flags.bHasAmbientOcclusionMap = bHasAO;

  // Emissive
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bUseEmmision = currentMat->m_properties.properties.flags.bUseEmission;
  ImGui::Checkbox("Emmision", &bUseEmmision);
  currentMat->m_properties.properties.flags.bUseEmission = bUseEmmision;

  String emmButtonID = "##EmmColorButton" + currentMat->getName();
  Vector3& emmisiveColor = currentMat->emissiveFactor;
  ImVec4 currentEmmColor = ImVec4(emmisiveColor.x,
                                  emmisiveColor.y,
                                  emmisiveColor.z,
                                  1.0f);
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  if (ImGui::ColorButton(emmButtonID.c_str(),
                         currentEmmColor,
                         ImGuiColorEditFlags_None,
                         ImVec2(windowWidth * 0.6f, 20.0f))) {
    ImGui::OpenPopup("EmmColorPickerPopup");
  }

  textureFileButton("##EmissiveSelection", pEmissiveImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pEmissiveImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pEmissiveImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  bool bHasEmissiveMap = currentMat->m_properties.properties.flags.bHasEmissiveMap;
  ImGui::Checkbox("Emmisive texture", &bHasEmissiveMap);
  currentMat->m_properties.properties.flags.bHasEmissiveMap = bHasEmissiveMap;

  if (ImGui::BeginPopup("EmmColorPickerPopup")) {
    ImGui::ColorPicker3("##picker", (float*)&currentEmmColor);
    ImGui::EndPopup();
  }
  emmisiveColor.x = currentEmmColor.x;
  emmisiveColor.y = currentEmmColor.y;
  emmisiveColor.z = currentEmmColor.z;

  // Opacity mask
  bool bHasAlpha = currentMat->m_properties.properties.flags.bHasAlphaTest;
  ImGui::Checkbox("Alpha testing", &bHasAlpha);
  currentMat->m_properties.properties.flags.bHasAlphaTest = bHasAlpha;
  ImGui::SameLine();
  bool bHasOpacityMask = currentMat->m_properties.properties.flags.bHasOpacityMask;
  ImGui::Checkbox("Use Opacity Mask Map", &bHasOpacityMask);
  currentMat->m_properties.properties.flags.bHasOpacityMask = bHasOpacityMask;

  textureFileButton("##OpacityMaskSelection", pOpacityImg, buttonSize);
  if (ImGui::IsItemHovered()) {
    if (!pOpacityImg.expired()) {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted(pOpacityImg.lock()->getPath().toString().c_str());
      ImGui::EndTooltip();
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
  ImGui::Text("Alpha Cutoff:");
  ImGui::SameLine();
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
  ImGui::SetNextItemWidth(windowWidth * 0.5f);
  ImGui::DragFloat("##Alpha cutoff", &currentMat->alphaCutoff, 0.01f, 0.0f, 1.0f);
  
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
  String headerStr = iconToStr(FONT_ICONS::kCloudSun) + " SkyBox Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    float width = windowSize.x;
    float col0 = width * 0.20f;

    if (ImGui::BeginTable("SkyBox Settings", 2)) {
      ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthFixed, col0);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      if(tablePropertyDragFloat("SkyBlur:", shaderMan.m_prefilteredData.roughness,
                                0.01f, 0.0f, 1.0f)) {
        shaderMan.updatePrefilterShaderCB();
      }

      ImTextureID* pSkyBoxImg = cast::re<ImTextureID*>(&pSkyBox->getSkyBoxResource()->texture);
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      if (ImGui::ImageButton("##SkyboxThumbnail", pSkyBoxImg, ImVec2(64.0f, 64.0f))) {
        String filePath;
        if (fileExp.openFile(filePath, ".hdr", "resources/textures/")) {
          auto pRes = resMan.loadResourceFromFile(Path(filePath));
          auto pImg = cast::re_ptr<ImageResource>(pRes);
          if (pImg) {
            pSkyBox->setSkyBoxResource(pImg);
            renderMan.computeIBL();
          }
        }
      }
      ImGui::TableNextColumn();

      ImGui::BeginChild("##ReadOnlyBox",
                        ImVec2(windowSize.x, ImGui::GetFrameHeight()),
                        true, ImGuiWindowFlags_NoScrollWithMouse);
      String textPath = pSkyBox->getSkyBoxResource()->getPath().toString();
      ImGui::TextUnformatted((iconToStr(FONT_ICONS::kFolder) + " " + textPath).c_str());
      ImGui::EndChild();

      if(ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(textPath.c_str());
        ImGui::EndTooltip();
      }

      ImGui::EndTable();
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

  String headerStr = iconToStr(FONT_ICONS::kLightBulb) + " Light Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
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
    Vector3 lightPos = Vector3(pLight->m_position.x,
                               pLight->m_position.y,
                               pLight->m_position.z);
    ImGui::Text("Position:");
    ImGui::SameLine(80.0f);
    dragVector3("LightPosition", lightPos, 0.1f);
    pLight->m_position.x = lightPos.x;
    pLight->m_position.y = lightPos.y;
    pLight->m_position.z = lightPos.z;

    // Target
    ImGui::Text("Target:");
    ImGui::SameLine(80.0f);
    dragVector3("LightTarget", pLight->m_target, 0.1f);

    // Color
    ImGui::Text("Color:");
    ImGui::SameLine();
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
    ImGui::Text("Light intensity:");
    ImGui::SameLine();
    ImGui::DragFloat("##Intensity", &pLight->m_intensity, 0.1f, 0.0f, 10.0f);
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
GUI::showCameraComponent(const WPtr<CameraComponent> wpCamera)
{
  if(wpCamera.expired()) {
    return;
  }

  String headerStr = iconToStr(FONT_ICONS::kVideoCamera) + " Camera Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
    // Camera properties can be added here
  }
}

void
GUI::showColliderComponent(const WPtr<ColliderComponent> wpCollider)
{
  if (wpCollider.expired()) {
    return;
  }
  auto pCollider = wpCollider.lock();
  String headerStr = iconToStr(FONT_ICONS::kCheckEmpty) + " Collider Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
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
  if(wpRigidbody.expired()) {
    return;
  }

  String headerStr = iconToStr(FONT_ICONS::kShield) + " Rigidbody Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
    // Rigidbody properties can be added here
  }
}

void
GUI::showAnimatorComponent(const WPtr<AnimatorComponent> wpAnimator)
{
  if(wpAnimator.expired()) {
    return;
  }

  String headerStr = iconToStr(FONT_ICONS::kChild) + " Animator Component";
  if (ImGui::CollapsingHeader(headerStr.c_str())) {
    // Animator properties can be added here
  }
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

void
GUI::setProjectResourceViewer()
{

}

void
GUI::setIcon(const ICONS::E icon, const Vector2& size)
{
  ImTextureID* iconTex = cast::re<ImTextureID*>(&m_iconsTexture);

  int32 index = cast::st<int32>(icon);
  int32 row = index / ICONS_PER_ROW;
  int32 col = index % ICONS_PER_ROW;

  float u0 = cast::st<float>(col * ICON_SIZE) / m_iconsSize.x;
  float v0 = cast::st<float>(row * ICON_SIZE) / m_iconsSize.y;
  float u1 = cast::st<float>((col + 1) * ICON_SIZE) / m_iconsSize.x;
  float v1 = cast::st<float>((row + 1) * ICON_SIZE) / m_iconsSize.y;

  ImGui::Image(iconTex,
               ImVec2(size.x, size.y),
               ImVec2(u0, v0),
               ImVec2(u1, v1));
}

bool
GUI::iconCollapsingHeader(const ICONS::E icon, const String& label)
{
  bool opened = false;
  float y = ImGui::GetCursorPosY() + 2.0f;

  ImGui::SetCursorPosY(y);
  setIcon(icon);
  ImGui::SameLine();

  opened = ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);

  return opened;
}

void
GUI::setStyle()
{
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 1.0f;
  style.DisabledAlpha = 0.6f;
  style.WindowPadding = ImVec2(10.0f, 10.0f);
  style.WindowRounding = 5.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(20.0f, 20.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 5.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 7.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(5.0f, 5.0f);
  style.FrameRounding = 7.5f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(8.0f, 5.0f);
  style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
  style.CellPadding = ImVec2(5.0f, 5.0f);
  style.IndentSpacing = 10.0f;
  style.ColumnsMinSpacing = 5.0f;
  style.ScrollbarSize = 12.5f;
  style.ScrollbarRounding = 5.0f;
  style.GrabMinSize = 10.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 10.0f;
  style.TabBorderSize = 0.0f;
  style.TabCloseButtonMinWidthSelected = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Left;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(0.78431374f, 0.78431374f, 0.78431374f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 0.9411765f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.42918456f, 0.42918026f, 0.42918026f, 0.54f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.652354f, 0.65235764f, 0.65236056f, 0.4f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6351931f, 0.63518673f, 0.63518673f, 0.67f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.13733906f, 0.13733768f, 0.13733768f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22317594f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.55364805f, 0.0f, 0.0f, 0.51f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] =
              ImVec4(0.019607844f, 0.019607844f, 0.019607844f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
              ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
              ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9828326f, 0.40494397f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.40952373f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.5137255f, 0.1764706f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.30042917f, 0.30042616f, 0.30042616f, 0.4f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3991416f, 0.39913762f, 0.39913762f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5064327f, 0.50643575f, 0.5064378f, 1.0f);
  style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.40784314f, 0.0f, 0.64705884f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.48327434f, 0.120171666f, 0.64705884f);
  style.Colors[ImGuiCol_HeaderActive] =
              ImVec4(0.99607843f, 0.5294118f, 0.20392157f, 0.64705884f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.99570817f, 0.71793544f, 0.0f, 0.5f);
  style.Colors[ImGuiCol_SeparatorHovered] =
              ImVec4(0.76824033f, 0.5925562f, 0.13848108f, 0.78f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.8969957f, 0.68286604f, 0.13089205f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.70385563f, 0.7038591f, 0.70386267f, 0.2f);
  style.Colors[ImGuiCol_ResizeGripHovered] =
              ImVec4(0.8283179f, 0.82832193f, 0.82832617f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive] =
              ImVec4(0.21888411f, 0.1117906f, 0.1117906f, 0.95f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.46351933f, 0.1452228f, 0.1452228f, 0.862f);
  style.Colors[ImGuiCol_TabHovered] =
              ImVec4(0.64377683f, 0.10223065f, 0.10223065f, 0.8627451f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.4509804f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] =
              ImVec4(0.14509805f, 0.06666667f, 0.06666667f, 0.9724f);
  style.Colors[ImGuiCol_TabUnfocusedActive] =
              ImVec4(0.42352942f, 0.13333336f, 0.13333336f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7253219f, 0.42958978f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.62231755f, 0.4487076f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] =
              ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] =
              ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.53648067f, 0.536478f, 0.5364753f, 0.35f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.7467811f, 0.42306852f, 0.0f, 0.9f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
  style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.35f, 0.35f, 0.35f, 0.7f);
  style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.35f, 0.1f, 0.1f, 1.0f);
}
}
