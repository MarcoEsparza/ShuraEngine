/*****************************************************************************/
/*
*  @file    imgui_impl_shura.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/04
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "imgui_impl_shura.h"
#include "shGraphicsManager.h"
#include "shRenderManager.h"
#include "shScreen.h"
#include "shDevice.h"
#include "shBuffers.h"
#include "shInputLayout.h"
#include "shShader.h"
#include "shTexture.h"
#include "shSamplerState.h"
#include "shRasterizerState.h"
#include "shBlendState.h"
#include "shDepthStencilState.h"

namespace shEngineSDK {
struct ImGui_ImplShura_Data
{
  SPtr<VertexBuffer>         pVB;
  SPtr<IndexBuffer>          pIB;
  SPtr<ConstantBuffer>       pBuffer;
  SPtr<ProgramShader>        pProgram;
  SPtr<SamplerState>         pSampler;
  SPtr<RasterizerState>      pRaster;
  SPtr<BlendState>           pBlend;
  SPtr<DepthStencilState>    pDepthStencil;
  SPtr<Texture>              pResourceView;
  uint32                     vertexBufferSize = 0;
  uint32                     indexBufferSize = 0;
};

bool
ImGui_ImplShura_Init(SPtr<Device>& pDevice, SPtr<DeviceContext>& pContext)
{
  ImGuiIO& io = ImGui::GetIO();
  IMGUI_CHECKVERSION();
  IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend");

  ImGui_ImplShura_Data* bd = IM_NEW(ImGui_ImplShura_Data)();
  

  return true;
}

void
ImGui_ImplShura_RenderDrawData(ImDrawData* drawData)
{
  
}

void
ImGui_ImplShura_CreateDeviceObjects()
{
  
}

void
ImGui_ImplShura_InvalidateDeviceObjects()
{
  
}

void
ImGui_ImplShura_NewFrame()
{
  
}

void
ImGui_ImplShura_Shutdown()
{
  
}
}
