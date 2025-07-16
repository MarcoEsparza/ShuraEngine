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
  ImGui_ImplShura_NewFrame();
  ImGui::NewFrame();

  return;

  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  static bool built = false;
  if (!built) {
    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
    ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags);
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
    ImGui::DockBuilderDockWindow("RendererSettings", right);
    ImGui::DockBuilderDockWindow("Console", bottom);
    ImGui::DockBuilderDockWindow("Scene", center);

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

  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open Scene")) {
        // Handle open scene action
      }
      if (ImGui::MenuItem("Save Scene")) {
        // Handle save scene action
      }
      if (ImGui::MenuItem("Exit")) {
        // Handle exit action
      }
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo")) {
        // Handle undo action
      }
      if (ImGui::MenuItem("Redo")) {
        // Handle redo action
      }
      if(ImGui::MenuItem("Preferences")) {
        // Handle preferences action
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::End();

  //ImGui::ShowDemoWindow();

  ImGui::Begin("Scenegraph");
  ImGui::Text("Object1");
  ImGui::End();

  ImGui::Begin("RendererSettings");
  ImGui::Text("Object1");
  ImGui::End();

  ImGui::Begin("Console");
  ImGui::Text("Console output goes here.");
  ImGui::End();

  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
  ImGui::Begin("Scene");
  ImGui::End();
  ImGui::PopStyleColor();
}
}
