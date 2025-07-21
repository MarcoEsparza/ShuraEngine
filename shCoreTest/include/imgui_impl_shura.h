/*****************************************************************************/
/*
*  @file    imgui_impl_shura.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/20
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
#include <shPrerequisitesCore.h>
#include <shScreen.h>
#include <shVector2.h>
#include <shEventEnums.h>

#define IMGUI_ENABLE_VIEWPORTS
#define IMGUI_HAS_DOCK

#include "imgui.h"
#include "externals/imgui_internal.h"

namespace shEngineSDK {

#ifndef IMGUI_DISABLE

/**
*  @brief Initialize ImGui implementation for Shura Engine.
*/
bool
ImGui_ImplShura_Init(const WPtr<Screen>& screenHandle);

void
ImGui_ImplShura_Resize(const Vector2& newSize);

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
ImGui_ImplShura_AddKeyEvent(const KEY::E key,
                            const bool bPressed,
                            const ModifierState modifier);

void
ImGui_ImplShura_AddMousePosEvent(const Vector2& mousePos);

void
ImGui_ImplShura_AddMouseButtonEvent(const bool bClicked, MOUSE_INPUT::E button);

void
ImGui_ImplShura_AddMouseWheelEvent(const float wheel,
                   const float hWheel);

/**
*  @brief Shuts down ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_Shutdown();

#endif
}
