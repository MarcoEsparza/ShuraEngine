/*****************************************************************************/
/*
*  @file    imgui_impl_shura.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/20
*  @brief   ImGui implementation for Shura Engine.
*
*  ImGui implementation for Shura Engine.
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
#include "shScreen.h"
#include "shVector2.h"
#include "imgui.h"

namespace shEngineSDK {

#ifndef IMGUI_DISABLE

/**
*  @brief Initialize ImGui implementation for Shura Engine.
*/
bool
ImGui_ImplShura_Init(const SPtr<Screen>& screenHandle);

/**
*  @brief Render call ImGui implementation for Shura Engine.
* 
*  @param ImDrawData* drawData
*/
void
ImGui_ImplShura_RenderDrawData(ImDrawData* drawData);

/**
*  @brief Creates necesary objects ImGui implementation for Shura Engine.
*/
bool
ImGui_ImplShura_CreateDeviceObjects();

/**
*  @brief Deletes objects ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_InvalidateDeviceObjects();

/**
*  @brief Updates ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_NewFrame();

void
addKeyEvent(const KEY::E key, const bool bPressed);

void
addMousePosEvent(const Vector2& mousePos);

void
addMouseButtonEvent(const bool bClicked);

void
addMouseWheelEvent(const float wheel,
                   const float hWheel);

/**
*  @brief Shuts down ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_Shutdown();

#endif
}
