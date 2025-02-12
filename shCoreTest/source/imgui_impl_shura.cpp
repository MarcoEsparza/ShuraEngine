/*****************************************************************************/
/*
*  @file    imgui_impl_shura.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/11
*  @brief   ImGui implementation for Shura Engine.
*
*  ImGui implementation for Shura Engine.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "imgui.h"
#ifndef IMGUI_DISABLE

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
#include "shPass.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
struct ImGui_ImplShura_RendererData
{
  SPtr<VertexBuffer>         pVB;
  SPtr<IndexBuffer>          pIB;
  SPtr<ConstantBuffer>       pProjBuffer;
  UPtr<Pass>                 pImGuiShuraProgram;
  SPtr<Texture2D>            pFontTexture;
  uint32                     vertexBufferSize = 0;
  uint32                     indexBufferSize = 0;

  ImGui_ImplShura_RendererData()
  { 
     memset((void*)this, 0, sizeof(this));
     vertexBufferSize = 5000;
     indexBufferSize = 10000;
  }
};

static ImGui_ImplShura_RendererData*
ImGuiImplShura_BackendRendererData()
{
  return ImGui::GetCurrentContext() ?
         reinterpret_cast<ImGui_ImplShura_RendererData*>
         (ImGui::GetIO().BackendRendererUserData) : nullptr;
}

bool
ImGui_ImplShura_Init()
{
  ImGuiIO& io = ImGui::GetIO();
  IMGUI_CHECKVERSION();
  IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend");
  IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend");

  ImGui_ImplShura_RendererData* bd = IM_NEW(ImGui_ImplShura_RendererData)();
  io.BackendRendererUserData = reinterpret_cast<void*>(bd);
  io.BackendRendererName = "ImGui_impl_Shura_Renderer";
  io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
  io.BackendPlatformName = "ImGui_impl_Shura_Platform";
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  return true;
}

static void
ImGui_ImplShura_SetupRenderState(ImDrawData* drawData)
{
  GraphicsManager& graphMan = g_graphicsMan();
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();

  // Setup viewport
  Viewport vp = {};
  vp.width = drawData->DisplaySize.x;
  vp.height = drawData->DisplaySize.y;
  vp.minDepth = 0.0f;
  vp.maxDepth = 1.0f;
  vp.topLeftX = vp.topLeftY = 0.0f;
  graphMan.setViewport(vp);

  // Set Pass
  graphMan.setPrimitiveTopology();
  bd->pImGuiShuraProgram->setPass();
  graphMan.setVertexBuffers(bd->pVB);
  graphMan.setIndexBuffers(bd->pIB);
}

void
ImGui_ImplShura_RenderDrawData(ImDrawData* drawData)
{
  if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f) {
    return;
  }

  GraphicsManager& graphMan = g_graphicsMan();
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();

  Vector<ImDrawVert> vertList;
  Vector<ImDrawIdx> idxList;

  for (int32 i = 0; i < drawData->CmdListsCount; ++i) {
    const ImDrawList* drawList = drawData->CmdLists[i];
    vertList.insert(vertList.end(),
                    drawList->VtxBuffer.Data,
                    drawList->VtxBuffer.Data + drawList->VtxBuffer.size());
    idxList.insert(idxList.end(),
                   drawList->IdxBuffer.Data,
                   drawList->IdxBuffer.Data + drawList->IdxBuffer.size());

    if (vertList.size() == 0) {
      return;
    }

    if (bd->pVB) {
      bd->pVB.reset();
    }
    Vector<GUIVertexData> vertVec;
    for (auto& imVert : vertList) {
      GUIVertexData vertex = {};
      vertex.position.x = imVert.pos.x;
      vertex.position.y = imVert.pos.y;
      vertex.texcoord.x = imVert.uv.x;
      vertex.texcoord.y = imVert.uv.y;
      vertex.color = imVert.col;
      vertVec.push_back(vertex);
    }
    bd->pVB = graphMan.createVertexBuffer(vertVec);

    if (bd->pIB) {
      bd->pIB.reset();
    }
    Vector<uint32> idxVec;
    for (auto& idx : idxList) {
      idxVec.push_back(static_cast<uint32>(idx));
    }
    bd->pIB = graphMan.createIndexBuffer(idxVec);

    // Constant buffer
    float L = drawData->DisplayPos.x;
    float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    float T = drawData->DisplayPos.y;
    float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
    Matrix4 orthoProj(2.0f / (R - L),    0.0f,              0.0f, 0.0f,
                      0.0f,              2.0f / (T - B),    0.0f, 0.0f,
                      0.0f,              0.0f,              0.5f, 0.0f,
                      (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f);
    graphMan.updateConstantBuffer(bd->pProjBuffer, &orthoProj, sizeof(Matrix4));

    ImGui_ImplShura_SetupRenderState(drawData);

    // Render command lists
    // (Because we merged all buffers into a single one, we maintain our own offset into them)
    int32 global_idx_offset = 0;
    int32 global_vtx_offset = 0;
    ImVec2 clip_off = drawData->DisplayPos;
    for (int32 j = 0; j < drawData->CmdListsCount; ++j) {
      const ImDrawList* cmd_list = drawData->CmdLists[j];
      for (int32 cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i) {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
        if (pcmd->UserCallback != nullptr)
        {
          // User callback, registered via ImDrawList::AddCallback()
          // (ImDrawCallback_ResetRenderState is a special callback value used by the
          // user to request the renderer to reset render state.)
          if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
            ImGui_ImplShura_SetupRenderState(drawData);
          }
          else {
            pcmd->UserCallback(cmd_list, pcmd);
          }
        }
        else
        {
          // Project scissor/clipping rectangles into framebuffer space
          ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
          ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
          if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) {
            continue;
          }

          // Apply scissor/clipping rectangle
          Rect scissorClip = {};
          scissorClip.min.x = clip_min.x;
          scissorClip.min.y = clip_min.y;
          scissorClip.max.x = clip_max.x;
          scissorClip.max.y = clip_max.y;
          graphMan.setScissorRects(scissorClip);

          // Bind texture, Draw
          SPtr<Texture2D>& pTexture = *reinterpret_cast<SPtr<Texture2D>*>(pcmd->GetTexID());
          graphMan.setShaderResourceView(pTexture);
          graphMan.drawIndexed(pcmd->ElemCount,
                               pcmd->IdxOffset + global_idx_offset,
                               pcmd->VtxOffset + global_vtx_offset);
        }
      }
      global_idx_offset += cmd_list->IdxBuffer.Size;
      global_vtx_offset += cmd_list->VtxBuffer.Size;
    }
  }
}

static void
ImGui_ImplShura_CreateFontsTexture()
{
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  unsigned char* pixels;
  int32 width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  // Upload texture to graphics system
  bd->pFontTexture = g_graphicsMan().createTexture2D(width,
                                                     height,
                                                     TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                                     USAGE::kDefault,
                                                     BIND_FLAGS::kShaderResource);

  // How do i pass the texture id from the shader resource view?
  io.Fonts->SetTexID(static_cast<ImTextureID>(0));

  auto pSampler = g_graphicsMan().createSamplerState();
  bd->pImGuiShuraProgram->setSamplerState(pSampler);
}

bool
ImGui_ImplShura_CreateDeviceObjects()
{
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  
  if (bd->pImGuiShuraProgram) {
    ImGui_ImplShura_InvalidateDeviceObjects();
  }

  // Set pass
  bd->pImGuiShuraProgram = make_unique<Pass>();

  bd->pImGuiShuraProgram->setShaderInfo("resources/ImGuiShuraShader.hlsl",
                                        "main",
                                        "mainPS",
                                        "vs_5_0",
                                        "ps_5_0");
  bd->pImGuiShuraProgram->compileShader();
  bd->pImGuiShuraProgram->generateInputLayout();

  bd->pProjBuffer = g_graphicsMan().createConstantBuffer(sizeof(Matrix4));
  bd->pImGuiShuraProgram->addVSConstantBuffer(bd->pProjBuffer);

  // Set states
  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kSolid;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = true;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  BlendDesc blendDesc = {};
  blendDesc.alphaToCoverageEnable = false;
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kSrcAlpha;
  blendDesc.renderTarget[0].destBlend = BLEND::kInvSrcAlpha;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kInvSrcAlpha;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  DepthStencilDesc depthSDesc = {};
  depthSDesc.depthEnable = false;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.stencilEnable = false;
  depthSDesc.stencilReadMask = 0;
  depthSDesc.stencilWriteMask = 0;
  depthSDesc.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilDepthFailOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.backFace = depthSDesc.frontFace;

  bd->pImGuiShuraProgram->setRasterizerState(rasterDesc);
  bd->pImGuiShuraProgram->setBlendState(blendDesc);
  bd->pImGuiShuraProgram->setDepthStencilState(depthSDesc);

  // Create texture and sampler state
  ImGui_ImplShura_CreateFontsTexture();

  return true;
}

void
ImGui_ImplShura_InvalidateDeviceObjects()
{
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();

  bd->pVB.reset();
  bd->pIB.reset();
  bd->pProjBuffer.reset();
  bd->pImGuiShuraProgram.reset();
  bd->pFontTexture.reset();
}

void
ImGui_ImplShura_NewFrame()
{
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  IM_ASSERT(bd != nullptr &&
            "Context or backend not initialized! Did you call ImGui_ImplDX11_Init()?");

  if (!bd->pFontTexture) {
    ImGui_ImplShura_CreateDeviceObjects();
  }
}

void
ImGui_ImplShura_Shutdown()
{
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  auto& io = ImGui::GetIO();

  ImGui_ImplShura_InvalidateDeviceObjects();
  io.BackendRendererName = nullptr;
  io.BackendRendererUserData = nullptr;
  io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
  IM_DELETE(bd);
}
}

#endif
