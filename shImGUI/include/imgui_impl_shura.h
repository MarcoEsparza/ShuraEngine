/*****************************************************************************/
/*
*  @file    imgui_impl_shura.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/04
*  @brief
*
*
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

IMGUI_IMPL_API bool ImGui_ImplShura_Init(SPtr<Device>& pDevice, SPtr<DeviceContext>& pContext);
IMGUI_IMPL_API void ImGui_ImplShura_RenderDrawData(ImDrawData* drawData);
IMGUI_IMPL_API void ImGui_ImplShura_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplShura_InvalidateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplShura_NewFrame();
IMGUI_IMPL_API void ImGui_ImplShura_Shutdown();

#endif
}
