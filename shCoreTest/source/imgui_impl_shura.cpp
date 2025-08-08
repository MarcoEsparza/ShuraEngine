/*****************************************************************************/
/*
*  @file    imgui_impl_shura.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/20
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
//#include "imgui.h"
#include "imgui_impl_shura.h"
#ifndef IMGUI_DISABLE

#include <shGraphicsManager.h>
#include <shRenderManager.h>
#include <shScreen.h>

#include <shBuffers.h>
#include <shInputLayout.h>
#include <shShader.h>
#include <shTexture.h>
#include <shSamplerState.h>
#include <shRasterizerState.h>
#include <shBlendState.h>
#include <shDepthStencilState.h>
#include <shPass.h>

#define VertexBufferMaxSize                        5000
#define IndexBufferMaxSize                         10000

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
     vertexBufferSize = VertexBufferMaxSize;
     indexBufferSize = IndexBufferMaxSize;
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
ImGui_ImplShura_Init(const WPtr<Screen>& screenHandle)
{
  if(screenHandle.expired()) {
    SH_ASSERT(false && "Screen handle is expired!");
    return false;
  }

  if (ImGui::GetCurrentContext() == nullptr) {
    SH_ASSERT(false && "ImGui context is not initialized!");
    return false;
  }

  auto pScreen = screenHandle.lock();

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
  //io.BackendFlags |= ImGuiBackendFlags_HasKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  
  io.DisplaySize.x = static_cast<float>(pScreen->getWidth());
  io.DisplaySize.y = static_cast<float>(pScreen->getHeight());
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  viewport->PlatformHandle = reinterpret_cast<void*>(pScreen->getPlatformHandler());
  viewport->Size.x = static_cast<float>(pScreen->getWidth());
  viewport->Size.y = static_cast<float>(pScreen->getHeight());

  for (int32 key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
    io.KeysData[key - ImGuiKey_NamedKey_BEGIN].Down = false;
  }

  return true;
}

void
ImGui_ImplShura_Resize(const Vector2& newSize)
{
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = newSize.x;
  io.DisplaySize.y = newSize.y;

  ImGuiViewport* viewport = ImGui::GetMainViewport();
  viewport->Size.x = newSize.x;
  viewport->Size.y = newSize.y;
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
        graphMan.psSetShaderResourceView(pTexture);
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
                                              TEXTURE_FORMAT::kR8G8B8A8_UNORM,
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
  bd->pImGuiShuraProgram = sh_makeUnique<Pass>();
  bd->pImGuiShuraProgram->setVShaderInfo("resources/shaders/ImGuiShuraShader.hlsl",
                                         "main",
                                         "vs_5_0");
  bd->pImGuiShuraProgram->setPShaderInfo("resources/shaders/ImGuiShuraShader.hlsl",
                                         "mainPS",
                                         "ps_5_0");
  bd->pImGuiShuraProgram->compileShader();

  Vector<InputDesc> ilDesc;
  ilDesc.resize(3);

  ilDesc[0].format = TEXTURE_FORMAT::kR32G32_FLOAT;
  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].size = 8;

  ilDesc[1].format = TEXTURE_FORMAT::kR32G32_FLOAT;
  ilDesc[1].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[1].size = 8;

  ilDesc[2].format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;
  ilDesc[2].type = INPUT_LAYOUT_TYPES::kColor;
  ilDesc[2].size = 4;

  auto pIL = graphMan.createInputLayout(ilDesc, bd->pImGuiShuraProgram->getVertexShader());
  bd->pImGuiShuraProgram->setInputLayout(pIL);

  bd->pProjBuffer = graphMan.createConstantBuffer(sizeof(Matrix4));
  bd->pImGuiShuraProgram->addVSConstantBuffer(bd->pProjBuffer, 0);

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
getKeyData(KEY::E key, ImGuiKey& imKey, uint32& input, const ModifierState modifier)
{

  switch (key) {
  case KEY::kA:
  {
    imKey = ImGuiKey_A;
    if (modifier.shift) {
      input = 'A';
    }
    else {
      input = 'a';
    }
    break;
  }
  case KEY::kB:
  {
    imKey = ImGuiKey_B;
    if (modifier.shift) {
      input = 'B';
    }
    else {
      input = 'b';
    }
    break;
  }
  case KEY::kC:
  {
    imKey = ImGuiKey_C;
    if (modifier.shift) {
      input = 'C';
    }
    else {
      input = 'c';
    }
    break;
  }
  case KEY::kD:
  {
    imKey = ImGuiKey_D;
    if (modifier.shift) {
      input = 'D';
    }
    else {
      input = 'd';
    }
    break;
  }
  case KEY::kE:
  {
    imKey = ImGuiKey_E;
    if (modifier.shift) {
      input = 'E';
    }
    else {
      input = 'e';
    }
    break;
  }
  case KEY::kF:
  {
    imKey = ImGuiKey_F;
    if (modifier.shift) {
      input = 'F';
    }
    else {
      input = 'f';
    }
    break;
  }
  case KEY::kG:
  {
    imKey = ImGuiKey_G;
    if (modifier.shift) {
      input = 'G';
    }
    else {
      input = 'g';
    }
    break;
  }
  case KEY::kH:
  {
    imKey = ImGuiKey_H;
    if (modifier.shift) {
      input = 'H';
    }
    else {
      input = 'h';
    }
    break;
  }
  case KEY::kI:
  {
    imKey = ImGuiKey_I;
    if (modifier.shift) {
      input = 'I';
    }
    else {
      input = 'i';
    }
    break;
  }
  case KEY::kJ:
  {
    imKey = ImGuiKey_J;
    if (modifier.shift) {
      input = 'J';
    }
    else {
      input = 'j';
    }
    break;
  }
  case KEY::kK:
  {
    imKey = ImGuiKey_K;
    if (modifier.shift) {
      input = 'K';
    }
    else {
      input = 'k';
    }
    break;
  }
  case KEY::kL:
  {
    imKey = ImGuiKey_L;
    if (modifier.shift) {
      input = 'L';
    }
    else {
      input = 'l';
    }
    break;
  }
  case KEY::kM:
  {
    imKey = ImGuiKey_M;
    if (modifier.shift) {
      input = 'M';
    }
    else {
      input = 'm';
    }
    break;
  }
  case KEY::kN:
  {
    imKey = ImGuiKey_N;
    if (modifier.shift) {
      input = 'N';
    }
    else {
      input = 'n';
    }
    break;
  }
  case KEY::kO:
  {
    imKey = ImGuiKey_O;
    if (modifier.shift) {
      input = 'O';
    }
    else {
      input = 'o';
    }
    break;
  }
  case KEY::kP:
  {
    imKey = ImGuiKey_P;
    if (modifier.shift) {
      input = 'P';
    }
    else {
      input = 'p';
    }
    break;
  }
  case KEY::kQ:
  {
    imKey = ImGuiKey_Q;
    if (modifier.shift) {
      input = 'Q';
    }
    else {
      input = 'q';
    }
    break;
  }
  case KEY::kR:
  {
    imKey = ImGuiKey_R;
    if (modifier.shift) {
      input = 'R';
    }
    else {
      input = 'r';
    }
    break;
  }
  case KEY::kS:
  {
    imKey = ImGuiKey_S;
    if (modifier.shift) {
      input = 'S';
    }
    else {
      input = 's';
    }
    break;
  }
  case KEY::kT:
  {
    imKey = ImGuiKey_T;
    if (modifier.shift) {
      input = 'T';
    }
    else {
      input = 't';
    }
    break;
  }
  case KEY::kU:
  {
    imKey = ImGuiKey_U;
    if (modifier.shift) {
      input = 'U';
    }
    else {
      input = 'u';
    }
    break;
  }
  case KEY::kV:
  {
    imKey = ImGuiKey_V;
    if (modifier.shift) {
      input = 'V';
    }
    else {
      input = 'v';
    }
    break;
  }
  case KEY::kW:
  {
    imKey = ImGuiKey_W;
    if (modifier.shift) {
      input = 'W';
    }
    else {
      input = 'w';
    }
    break;
  }
  case KEY::kX:
  {
    imKey = ImGuiKey_X;
    if (modifier.shift) {
      input = 'X';
    }
    else {
      input = 'x';
    }
    break;
  }
  case KEY::kY:
  {
    imKey = ImGuiKey_Y;
    if (modifier.shift) {
      input = 'Y';
    }
    else {
      input = 'y';
    }
    break;
  }
  case KEY::kZ:
  {
    imKey = ImGuiKey_Z;
    if (modifier.shift) {
      input = 'Z';
    }
    else {
      input = 'z';
    }
    break;
  }
  case KEY::kNum0:
  {
    imKey = ImGuiKey_0;
    input = '0';
    break;
  }
  case KEY::kNum1:
  {
    imKey = ImGuiKey_1;
    input = '1';
    break;
  }
  case KEY::kNum2:
  {
    imKey = ImGuiKey_2;
    input = '2';
    break;
  }
  case KEY::kNum3:
  {
    imKey = ImGuiKey_3;
    input = '3';
    break;
  }
  case KEY::kNum4:
  {
    imKey = ImGuiKey_4;
    input = '4';
    break;
  }
  case KEY::kNum5:
  {
    imKey = ImGuiKey_5;
    input = '5';
    break;
  }
  case KEY::kNum6:
  {
    imKey = ImGuiKey_6;
    input = '6';
    break;
  }
  case KEY::kNum7:
  {
    imKey = ImGuiKey_7;
    input = '7';
    break;
  }
  case KEY::kNum8:
  {
    imKey = ImGuiKey_8;
    input = '8';
    break;
  }
  case KEY::kNum9:
  {
    imKey = ImGuiKey_9;
    input = '9';
    break;
  }
  case KEY::kBack:
  {
    imKey = ImGuiKey_Backspace;
    break;
  }
  case KEY::kSpace:
  {
    imKey = ImGuiKey_Space;
    input = ' ';
    break;
  }
  case KEY::kCapital:
  {
    imKey = ImGuiKey_CapsLock;
    break;
  }
  case KEY::kComma:
  {
    imKey = ImGuiKey_Comma;
    input = ',';
    break;
  }
  case KEY::kTab:
  {
    imKey = ImGuiKey_Tab;
    input = '    ';
    break;
  }
  case KEY::kKeysMax:
  {
    imKey = ImGuiKey_COUNT;
    break;
  }
  default:
    break;
  }
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
ImGui_ImplShura_AddKeyEvent(const KEY::E key,
                            const bool bPressed,
                            const ModifierState modifier)
{
  ImGuiIO& io = ImGui::GetIO();
  uint32 input = '\0';
  ImGuiKey imKey = ImGuiKey_None;
  getKeyData(key, imKey, input, modifier);

  if (imKey != ImGuiKey_None && imKey != ImGuiKey_COUNT) {
    io.AddKeyEvent(imKey, bPressed);
  }

  if (bPressed) {
    if (input != '\0') {
      io.AddInputCharacter(input);
    }
  }
}

void
ImGui_ImplShura_AddMousePosEvent(const Vector2& mousePos)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent(mousePos.x, mousePos.y);
}

void
ImGui_ImplShura_AddMouseButtonEvent(const bool bClicked, MOUSE_INPUT::E button)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMouseButtonEvent(static_cast<int>(button), bClicked);
}

void
ImGui_ImplShura_AddMouseWheelEvent(const float wheel, const float hWheel)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMouseWheelEvent(wheel, hWheel);
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
