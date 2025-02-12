/*****************************************************************************/
/*
*  @file    imgui_impl_shura.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/11
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
#include "imgui.h"

namespace shEngineSDK {
class Device;
class DeviceContext;
class SwapChain;

#ifndef IMGUI_DISABLE

/**
*  @brief Initialize ImGui implementation for Shura Engine.
*/
IMGUI_IMPL_API bool ImGui_ImplShura_Init();

/**
*  @brief Render call ImGui implementation for Shura Engine.
* 
*  @param ImDrawData* drawData
*/
IMGUI_IMPL_API void ImGui_ImplShura_RenderDrawData(ImDrawData* drawData);

/**
*  @brief Creates necesary objects ImGui implementation for Shura Engine.
*/
IMGUI_IMPL_API bool ImGui_ImplShura_CreateDeviceObjects();

/**
*  @brief Deletes objects ImGui implementation for Shura Engine.
*/
IMGUI_IMPL_API void ImGui_ImplShura_InvalidateDeviceObjects();

/**
*  @brief Updates ImGui implementation for Shura Engine.
*/
IMGUI_IMPL_API void ImGui_ImplShura_NewFrame();

/**
*  @brief Shuts down ImGui implementation for Shura Engine.
*/
IMGUI_IMPL_API void ImGui_ImplShura_Shutdown();

#endif
}
