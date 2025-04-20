/*****************************************************************************/
/*
*  @file    imgui_impl_shura.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/20
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
ImGui_ImplShura_Init(const SPtr<Screen>& screenHandle)
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
  
  io.DisplaySize.x = static_cast<float>(screenHandle->getWidth());
  io.DisplaySize.y = static_cast<float>(screenHandle->getHeight());
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  viewport->PlatformHandle = reinterpret_cast<void*>(screenHandle->getPlatformHandler());
  viewport->Size.x = static_cast<float>(screenHandle->getWidth());
  viewport->Size.y = static_cast<float>(screenHandle->getHeight());

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
  /*Matrix4 orthoProj(2.0f / (R - L),    0.0f,              0.0f, 0.0f,
                    0.0f,              2.0f / (T - B),    0.0f, 0.0f,
                    0.0f,              0.0f,              0.5f, 0.0f,
                    (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f);*/
  Matrix4 orthoProj = OrthographicProjectionMatrix(L, R, B, T, 0.1f, 1.0f);
  graphMan.updateConstantBuffer(bd->pProjBuffer, &orthoProj, sizeof(Matrix4));

  ImGui_ImplShura_SetupRenderState(drawData);

  // Render command lists
  // (Because we merged all buffers into a single one, we maintain our own offset into them)
  int32 global_idx_offset = 0;
  int32 global_vtx_offset = 0;
  ImVec2 clip_off = drawData->DisplayPos;
  for (int32 i = 0; i < drawData->CmdListsCount; ++i) {
    const ImDrawList* cmd_list = drawData->CmdLists[i];
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

static void
ImGui_ImplShura_CreateFontsTexture()
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  ImFontAtlas* atlas = io.Fonts;
  ImFontConfig config;
  config.RasterizerDensity = 2.0f;
  auto font = atlas->AddFontFromFileTTF("resources/fonts/DroidSans.ttf", 18.0f, &config);
  
  if (font == nullptr) {
    SH_ASSERT(false && "Font loading failed");
  }

  io.FontDefault = font;
  atlas->Build();

  uint8* pixels;
  int32 width, height;
  atlas->GetTexDataAsRGBA32(&pixels, &width, &height);

  // Upload texture to graphics system
  bd->pFontTexture = graphMan.createTexture2D(width,
                                              height,
                                              TEXTURE_FORMAT::kR8G8B8A8_unorm,
                                              USAGE::kDefault,
                                              BIND_FLAGS::kShaderResource);

  graphMan.updateTexture2D(bd->pFontTexture, pixels, width, 4);

  // How do i pass the texture id from the shader resource view?
  atlas->SetTexID(reinterpret_cast<ImTextureID>(&bd->pFontTexture));

  auto pSampler = graphMan.createSamplerState();
  bd->pImGuiShuraProgram->setSamplerState(pSampler);
}

bool
ImGui_ImplShura_CreateDeviceObjects()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  
  if (bd->pImGuiShuraProgram) {
    ImGui_ImplShura_InvalidateDeviceObjects();
  }

  // Set pass
  bd->pImGuiShuraProgram = make_unique<Pass>();

  bd->pImGuiShuraProgram->setShaderInfo("resources/shaders/ImGuiShuraShader.hlsl",
                                        "main",
                                        "mainPS",
                                        "vs_5_0",
                                        "ps_5_0");
  bd->pImGuiShuraProgram->compileShader();

  Vector<InputDesc> ilDesc;
  ilDesc.resize(3);

  ilDesc[0].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].size = 8;

  ilDesc[1].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[1].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[1].size = 8;

  ilDesc[2].format = TEXTURE_FORMAT::kR8G8B8A8_unorm;
  ilDesc[2].type = INPUT_LAYOUT_TYPES::kColor;
  ilDesc[2].size = 4;

  auto pIL = graphMan.createInputLayout(ilDesc, bd->pImGuiShuraProgram->getShader());
  bd->pImGuiShuraProgram->setInputLayout(pIL);

  bd->pProjBuffer = graphMan.createConstantBuffer(sizeof(Matrix4));
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
  blendDesc.independentBlendEnable = false;
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
  depthSDesc.backFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilDepthFailOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  bd->pImGuiShuraProgram->setRasterizerStateFromDesc(rasterDesc);
  bd->pImGuiShuraProgram->setBlendStateFromDesc(blendDesc);
  bd->pImGuiShuraProgram->setDepthStencilStateFromDesc(depthSDesc);

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

static void
updateMouseData(const Vector2& mousePos,
                const bool clicked,
                const float wheel,
                const float hWheel)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent(mousePos.x, mousePos.y);
  
  if (clicked) {
    io.AddMouseButtonEvent(0, true);
  }
  else {
    io.AddMouseButtonEvent(0, false);
  }
  io.AddMouseWheelEvent(hWheel, wheel);
}

static ImGuiKey
getImGuiKey(KEY::E key)
{
  switch (key) {
  case KEY::kA:
  {
    return ImGuiKey_A;
    break;
  }
  case KEY::kNum1:
  {
    return ImGuiKey_1;
    break;
  }
  case KEY::kNum2:
  {
    return ImGuiKey_2;
    break;
  }
  case KEY::kNum3:
  {
    return ImGuiKey_3;
    break;
  }
  case KEY::kNum4:
  {
    return ImGuiKey_4;
    break;
  }
  case KEY::kNum5:
  {
    return ImGuiKey_5;
    break;
  }
  case KEY::kNum6:
  {
    return ImGuiKey_6;
    break;
  }
  case KEY::kNum7:
  {
    return ImGuiKey_7;
    break;
  }
  case KEY::kNum8:
  {
    return ImGuiKey_8;
    break;
  }
  case KEY::kNum9:
  {
    return ImGuiKey_9;
    break;
  }
  case KEY::kNum0:
  {
    return ImGuiKey_0;
    break;
  }
  case KEY::kKeysMax:
  {
    return ImGuiKey_COUNT;
    break;
  }
  default:
    break;
  }
}

static void
addKeyboardEvents(KEY::E key, bool pressed)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddKeyEvent(getImGuiKey(key), pressed);
}

void
ImGui_ImplShura_NewFrame(const Vector2& mousePos,
                         const bool clicked,
                         const float wheel,
                         const float hWheel,
                         const float btnPressed,
                         const KEY::E key)
{
  ImGui_ImplShura_RendererData* bd = ImGuiImplShura_BackendRendererData();
  IM_ASSERT(bd != nullptr &&
            "Context or backend not initialized! Did you call ImGui_ImplDX11_Init()?");

  updateMouseData(mousePos, clicked, wheel, hWheel);

  /*if (btnPressed) {
    addKeyboardEvents(key, btnPressed);
  }*/

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
