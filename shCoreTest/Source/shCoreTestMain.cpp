/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Here is the main to test the window initialize.
*
*  Here is the main to test the window initialize.
* 
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"
#include "shGraphicsManager.h"
#include "shBoxAAB.h"
#include "shMath.h"

#include "functional"

using namespace shEngineSDK;

#include <Windows.h>

SPtr<VertexShader> g_pVertexShader;
SPtr<PixelShader> g_pPixelShader;
SPtr<InputLayout> g_pInputLayout;
SPtr<Texture2D> g_pTexture;
SPtr<VertexBuffer> g_pVertexBuffer;
SPtr<IndexBuffer> g_pIndexBuffer;
SPtr<ConstantBuffer> g_pWVP;
SPtr<SamplerState> g_pSamplerLinear;
SPtr<DepthStencilView> g_pDepthSV;

Vector<VertexData> g_mesh;
Vector<uint32> g_index;

void
initGraphicAssets(const Screen& _screen);

void
render();

int main()
{
  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "Graphics Test";
  desc.visible = true;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 1000;
  desc.height = 600;
  desc.iconPath = "resources/ShuraIcon.ico";

  Screen mainScreen;
  SPtr<ScreenEventHandle> eventQ = make_shared<ScreenEventHandle>();

  if (!mainScreen.init(desc, eventQ)) {
    return -1;
  }

  HINSTANCE hGetProcIDDLL = LoadLibrary("shDX11Graphicsd.dll");
  SH_ASSERT(hGetProcIDDLL && "Could not load dll");

  auto loadPlugin = reinterpret_cast<void(*)()>(GetProcAddress(hGetProcIDDLL, "loadPlugin"));
  SH_ASSERT(loadPlugin && "Could not load function");
  loadPlugin();

  GraphicsManager::instance().initManager(mainScreen.getPlatformHandler(),
                                          false,
                                          false,
                                          1,
                                          1);
  
  initGraphicAssets(mainScreen);

  bool isRunning = true;

  while (isRunning) {
    eventQ->update();

    while (!eventQ->empty()) {
      auto ev = eventQ->front();

      if (ev.type == EVENT_TYPE::E::kMouseInput) {
        const MouseInputData mouse = ev.data.mouseInput;
      }
      if (ev.type == EVENT_TYPE::E::kClose) {
        mainScreen.close();
        isRunning = false;
      }

      eventQ->pop();
    }

    render();
  }

  //GraphicsManager::shutDown();

  return 0;
}

void
initGraphicAssets(const Screen& _screen)
{
  /********************
  *  Shaders
  ********************/

  g_pVertexShader = GraphicsManager::instance().createVertexShader("resources/BasicShader.hlsl",
                                                                   "main",
                                                                   "vs_5_0");
  SH_ASSERT(g_pVertexShader);

  g_pPixelShader = GraphicsManager::instance().createPixelShader("resources/BasicShader.hlsl",
                                                                 "mainPS",
                                                                 "ps_5_0");
  SH_ASSERT(g_pPixelShader);

  /********************
  *  Input layout
  ********************/

  Vector<shInputLayoutTypes::E> ilTypes;

  ilTypes.push_back(shInputLayoutTypes::E::kPOSITION);
  ilTypes.push_back(shInputLayoutTypes::E::kNORMAL);
  ilTypes.push_back(shInputLayoutTypes::E::kTEXCOORD);
  ilTypes.push_back(shInputLayoutTypes::E::kBONEINDICES);
  ilTypes.push_back(shInputLayoutTypes::E::kBONEWEIGHTS);

  g_pInputLayout = GraphicsManager::instance().createInputLayout(ilTypes, g_pVertexShader);
  SH_ASSERT(g_pInputLayout);

  /*****************************
  *  Vertex and Index buffers
  *****************************/

  /*shBoxAAB box(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  auto boxVertex = box.getVertices();
  auto boxIndices = box.getIndices();

  Vector<VertexData> cube;
  cube.resize(8);

  for (uint8 i = 0; i < 8; ++i) {
    auto& vertex = cube[i];
    vertex.position = boxVertex[i];
  }

  Vector<uint32> cubeIndices;
  cubeIndices.resize(36);

  for (uint8 i = 0; i < 36; ++i) {
    auto& index = cubeIndices[i];
    index = boxIndices[i];
  }

  g_mesh = cube;
  g_index = cubeIndices;*/

  Vector<VertexData> cube;
  cube.resize(24);

  cube[0].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[0].tex = Vector2(0.0f, 0.0f);
  cube[1].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[1].tex = Vector2(1.0f, 0.0f);
  cube[2].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[2].tex = Vector2(1.0f, 1.0f);
  cube[3].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[3].tex = Vector2(0.0f, 1.0f);
  cube[4].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[4].tex = Vector2(0.0f, 0.0f);
  cube[5].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[5].tex = Vector2(1.0f, 0.0f);
  cube[6].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[6].tex = Vector2(1.0f, 1.0f);
  cube[7].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[7].tex = Vector2(0.0f, 1.0f);
  cube[8].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[8].tex = Vector2(0.0f, 0.0f);
  cube[9].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[9].tex = Vector2(1.0f, 0.0f);
  cube[10].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[10].tex = Vector2(1.0f, 1.0f);
  cube[11].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[11].tex = Vector2(0.0f, 1.0f);
  cube[12].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[12].tex = Vector2(0.0f, 0.0f);
  cube[13].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[13].tex = Vector2(1.0f, 0.0f);
  cube[14].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[14].tex = Vector2(1.0f, 1.0f);
  cube[15].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[15].tex = Vector2(0.0f, 1.0f);
  cube[16].position = Vector3(-1.0f, -1.0f, -1.0f);
  cube[16].tex = Vector2(0.0f, 0.0f);
  cube[17].position = Vector3(1.0f, -1.0f, -1.0f);
  cube[17].tex = Vector2(1.0f, 0.0f);
  cube[18].position = Vector3(1.0f, 1.0f, -1.0f);
  cube[18].tex = Vector2(1.0f, 1.0f);
  cube[19].position = Vector3(-1.0f, 1.0f, -1.0f);
  cube[19].tex = Vector2(0.0f, 1.0f);
  cube[20].position = Vector3(-1.0f, -1.0f, 1.0f);
  cube[20].tex = Vector2(0.0f, 0.0f);
  cube[21].position = Vector3(1.0f, -1.0f, 1.0f);
  cube[21].tex = Vector2(1.0f, 0.0f);
  cube[22].position = Vector3(1.0f, 1.0f, 1.0f);
  cube[22].tex = Vector2(1.0f, 1.0f);
  cube[23].position = Vector3(-1.0f, 1.0f, 1.0f);
  cube[23].tex = Vector2(0.0f, 1.0f);

  Vector<uint32> indices = { 3,1,0,
                             2,1,3,
                             
                             6,4,5,
                             7,4,6,
                             
                             11,9,8,
                             10,9,11,
                             
                             14,12,13,
                             15,12,14,
                             
                             19,17,16,
                             18,17,19,
                             
                             22,20,21,
                             23,20,22 };

  g_mesh = cube;
  g_index = indices;

  g_pVertexBuffer = GraphicsManager::instance().createVertexBuffer(g_mesh);
  SH_ASSERT(g_pVertexBuffer);

  g_pIndexBuffer = GraphicsManager::instance().createIndexBuffer(g_index);
  SH_ASSERT(g_pIndexBuffer);

  /********************
  *  Sampler state
  ********************/

  g_pSamplerLinear = GraphicsManager::instance().createSamplerState();
  SH_ASSERT(g_pSamplerLinear);

  /********************
  *  Texture
  ********************/

  String path = "resources/ShuraIconOption.png";
  g_pTexture = GraphicsManager::instance().createTextureFromFile(path);
  SH_ASSERT(g_pTexture);

  /********************
  *  View-Projection
  ********************/

  uint32 sizeWVP = sizeof(Matrix4);
  g_pWVP = GraphicsManager::instance().createConstantBuffer(sizeWVP * 2);
  SH_ASSERT(g_pWVP);

  Vector3 eye(0.0f, 0.0f, -4.0f);
  Vector3 at(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);

  WorldViewProjection myWVP;
  myWVP.view = ViewMatrix(eye, at, up);
  myWVP.proj = ProjectionMatrix(Math::PI / 4.0f,
                                static_cast<float>(_screen.getWidth()),
                                static_cast<float>(_screen.getHeight()),
                                0.1f,
                                100.0f);
  
  myWVP.view.getTransposed();
  myWVP.proj.getTransposed();

  GraphicsManager::instance().updateConstantBuffer(g_pWVP, &myWVP, sizeof(myWVP));
  SH_ASSERT(g_pWVP);
}

void
render()
{
  auto pMainRTV = GraphicsManager::instance().getMainRenderTargetView();
  LinearColor color(0.2f, 0.6f, 0.2f);
  GraphicsManager::instance().clearRenderTarget(pMainRTV, color);

  auto pDepthStencil = GraphicsManager::instance().getMainDepthStencil();
  GraphicsManager::instance().clearDepthStencil(pDepthStencil);

  GraphicsManager::instance().setRenderTargets(pMainRTV, pDepthStencil, 1);

  GraphicsManager::instance().setSamplerState(g_pSamplerLinear);
  GraphicsManager::instance().setShaderResourceView(g_pTexture);

  GraphicsManager::instance().setVertexShader(g_pVertexShader);
  GraphicsManager::instance().setPixelShader(g_pPixelShader);

  GraphicsManager::instance().setInputLayout(g_pInputLayout);
  GraphicsManager::instance().setVertexBuffers(g_pVertexBuffer);
  GraphicsManager::instance().setIndexBuffers(g_pIndexBuffer);
  GraphicsManager::instance().vsSetConstantBuffers(g_pWVP);
  GraphicsManager::instance().setPrimitiveTopology();
  
  GraphicsManager::instance().drawIndexed(static_cast<uint32>(g_index.size()), 0, 0);

  GraphicsManager::instance().present();
}
