/*****************************************************************************/
/*
*  @file    shRenderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Render module.
*
*  Render module.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRenderManager.h"
#include "shGraphicsManager.h"
#include "shSceneGraph.h"
#include "shResourceManager.h"
#include "shMeshComponent.h"
#include "shSkyBoxComponent.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shPass.h"
#include "shMatrix4.h"
#include "shStringID.h"
#include "shPath.h"
#include "shTexture.h"
#include "shCubeMap.h"
#include "shMath.h"

#define PBRBufferSize 32 // Size of the PBR material constant buffer
#define LUMINANCE_MAP_SIZE 512.0f // Size of the luminance map
#define HISTOGRAM_MAP_SIZE 256.0f // Size of the histogram map
#define DEFAULT_THREADS 32 // Default threads for compute shaders
#define DEFAULT_NUM_MIP_LEVELS 4 // Number of mip levels for textures

#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define BLURV_THREADS_Y 4

#define TEXTURE8K_WIDTH 8192
#define TEXTURE4K_WIDTH 4096

#define CUBE_MAP_SIZE 1024 // Size of the cube map texture
#define NUM_CUBE_MAP_FACES 6 // Number of faces in a cube map

namespace shEngineSDK {
RenderManager::~RenderManager()
{
  for (auto& pass : m_passes) {
    pass.second.reset();
  }

  m_passes.clear();
}

void
RenderManager::onStartUp()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  // Blend state for alpha testing
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
  m_pAlphaTestBS = graphMan.createBlendState(blendDesc);

  // General blend state
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;
  m_pBasicBS = graphMan.createBlendState(blendDesc);

  // Create model transform constant buffer
  m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
  Matrix4 identity = Matrix4::IDENTITY;
  graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));

  auto fullUAVBindFlags = BIND_FLAGS::kRenderTarget |
                          BIND_FLAGS::kShaderResource |
                          BIND_FLAGS::kUnorderedAccess;

  m_pPBRData = graphMan.createConstantBuffer(PBRBufferSize);
  m_pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
  m_pMainBuffer = graphMan.createConstantBuffer(sizeof(MainBufferData));

  // Textures info
  m_renderTargetMap[StringID("DepthMap").getID()] = RenderTargetInfo("DepthMap",
                                                    TEXTURE_FORMAT::kR32G32B32A32_FLOAT);
  m_renderTargetMap[StringID("NormalMap").getID()] = RenderTargetInfo("NormalMap");
  m_renderTargetMap[StringID("ColorMap").getID()] = RenderTargetInfo("ColorMap");
  m_renderTargetMap[StringID("PropMap").getID()] = RenderTargetInfo("PropMap");
  m_renderTargetMap[StringID("AOMap").getID()] = RenderTargetInfo("AOMap",
                                                                  TEXTURE_FORMAT::kR16_FLOAT);

  m_renderTargetMap[StringID("HBlurMap").getID()] = RenderTargetInfo("HBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("VBlurMap").getID()] = RenderTargetInfo("VBlurMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("LightCMap").getID()] = RenderTargetInfo("LightCMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("SkyBoxMap").getID()] = RenderTargetInfo("SkyBoxMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT, fullUAVBindFlags);

  m_renderTargetMap[StringID("TempMap").getID()] = RenderTargetInfo("TempMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ToneMap").getID()] = RenderTargetInfo("ToneMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("PPMap").getID()] = RenderTargetInfo("PPMap",
    TEXTURE_FORMAT::kR8G8B8A8_UNORM, fullUAVBindFlags);

  m_renderTargetMap[StringID("ShadowMap").getID()] = RenderTargetInfo("ShadowMap",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("ShadowTemp").getID()] = RenderTargetInfo("ShadowTemp",
    TEXTURE_FORMAT::kR32_FLOAT,
    BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1, 1,
    m_shadowMapSize, m_shadowMapSize, false);

  m_renderTargetMap[StringID("LuminanceMap").getID()] = RenderTargetInfo("LuminanceMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 0, 1,
    LUMINANCE_MAP_SIZE, LUMINANCE_MAP_SIZE, false);

  m_renderTargetMap[StringID("BrightMap").getID()] = RenderTargetInfo("BrightMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS, 1,
    0.5f, 0.5f);

  m_renderTargetMap[StringID("AdditiveMap").getID()] = RenderTargetInfo("AdditiveMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("BHBlur").getID()] = RenderTargetInfo("BHBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("BVBlur").getID()] = RenderTargetInfo("BVBlur",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, DEFAULT_NUM_MIP_LEVELS);

  m_renderTargetMap[StringID("HistogramMap").getID()] = RenderTargetInfo("HistogramMap",
    TEXTURE_FORMAT::kR32_FLOAT,
    fullUAVBindFlags, USAGE::kDefault, 1, 1,
    HISTOGRAM_MAP_SIZE, 3, false);

  m_renderTargetMap[StringID("GbufferDepth").getID()] = RenderTargetInfo("GbufferDepth",
    TEXTURE_FORMAT::kR32_TYPELESS,
    BIND_FLAGS::kDepthStencil | BIND_FLAGS::kShaderResource, USAGE::kDefault, 1);

  /*m_renderTargetMap[StringID("CubeMap").getID()] = RenderTargetInfo("CubeMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 1, NUM_CUBE_MAP_FACES, CUBE_MAP_SIZE, CUBE_MAP_SIZE, false);

  m_renderTargetMap[StringID("DiffIrrMap").getID()] = RenderTargetInfo("DiffIrrMap",
    TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 0, 1, 256.0f, 128.0f, false);

  m_renderTargetMap[StringID("BRDFLut").getID()] = RenderTargetInfo("BRDFLut",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 1, 1, 256.0f, 256.0f, false);

  m_renderTargetMap[StringID("SpecPreMap").getID()] = RenderTargetInfo("SpecPreMap",
    TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
    BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
    USAGE::kDefault, 0, 1);*/

  m_pDiffIrr = graphMan.createTexture2D(256, 128,
                        TEXTURE_FORMAT::kR16G16B16A16_FLOAT,
                        USAGE::kDefault,
                        BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                        0, 1);

  auto pLut = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/Guardians-LogC4.cube"));
  if (pLut) {
    m_pLutTexture = cast::re_ptr<CubeMap>(pLut);
    m_shaderData.lutSize = static_cast<float>(m_pLutTexture->getLutSize());
  }

  auto pLut1 = resMan.loadResourceFromFile(Path("resources/Assets/LUTs/LBK-K-Tone_33.cube"));
  if (pLut1) {
    m_pLutLBK = cast::re_ptr<CubeMap>(pLut1);
    m_shaderData.lutSize = static_cast<float>(m_pLutLBK->getLutSize());
  }
}

void
RenderManager::onShutDown()
{
  cleanShaderObjects();
}

void
RenderManager::createRenderTextures()
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (auto& rti : m_renderTargetMap) {
    RenderTargetInfo& rtiInfo = rti.second;

    auto widthSize = rtiInfo.width;
    auto heightSize = rtiInfo.height;
    if (rtiInfo.bUseScaledSize)
    {
      widthSize *= m_screenDimension.x;
      heightSize *= m_screenDimension.y;
    }

    auto pTex = graphMan.createTexture2D(static_cast<uint32>(widthSize),
                                         static_cast<uint32>(heightSize),
                                         rtiInfo.format,
                                         rtiInfo.usage,
                                         rtiInfo.bFlags,
                                         rtiInfo.mipLevels,
                                         rtiInfo.arraySize);
    m_renderTargetMap[StringID(rtiInfo.name).getID()].pTexture = pTex;
  }
}

void
RenderManager::createPasses()
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Init pass shaders
  // GBuffer
  auto pGbufferShader = sh_makeShared<Pass>();
  pGbufferShader->setVShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "main",
                                 "vs_5_0");
  pGbufferShader->setPShaderInfo("resources/shaders/GBufferShader.hlsl",
                                 "mainPS",
                                 "ps_5_0");
  pGbufferShader->compileShader();

  // Lightning
  auto pLightCS = sh_makeShared<Pass>();
  pLightCS->setCShaderInfo("resources/shaders/LightCShader.hlsl",
                           "CSMain",
                           "cs_5_0");
  pLightCS->compileShader();

  // AO
  auto pAOShader = sh_makeShared<Pass>();
  pAOShader->setPShaderInfo("resources/shaders/AOShader.hlsl",
                            "mainPS",
                            "ps_5_0");
  pAOShader->compileShader();

  // HBlur
  auto pHBlurShader = sh_makeShared<Pass>();
  pHBlurShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                               "HBlurCS",
                               "cs_5_0");
  pHBlurShader->compileShader();

  // VBlur
  auto pVBlurShader = sh_makeShared<Pass>();
  pVBlurShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                               "VBlurCS",
                               "cs_5_0");
  pVBlurShader->compileShader();

  // HBlur
  auto pHBlurCS = sh_makeShared<Pass>();
  pHBlurCS->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                           "HBlur_CS",
                           "cs_5_0");
  pHBlurCS->compileShader();

  // VBlur
  auto pVBlurCS = sh_makeShared<Pass>();
  pVBlurCS->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "VBlur_CS",
                            "cs_5_0");
  pVBlurCS->compileShader();

  // Shadow map
  auto pSMapShader = sh_makeShared<Pass>();
  pSMapShader->setVShaderInfo("resources/shaders/SMapShader.hlsl",
                              "main",
                              "vs_5_0");
  pSMapShader->setPShaderInfo("resources/shaders/SMapShader.hlsl",
                              "mainPS",
                              "ps_5_0");
  pSMapShader->compileShader();

  // Skybox
  auto pSkyBoxShader = sh_makeShared<Pass>();
  /*pSkyBoxShader->setVShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "main",
                                "vs_5_0");
  pSkyBoxShader->setPShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "mainPS",
                                "ps_5_0");*/
  pSkyBoxShader->setCShaderInfo("resources/shaders/SkyBoxShader.hlsl",
                                "CSMain",
                                "cs_5_0");
  pSkyBoxShader->compileShader();

  // Final shader
  auto pFinalShader = sh_makeShared<Pass>();
  pFinalShader->setPShaderInfo("resources/shaders/FinalShader.hlsl",
                               "mainPS",
                               "ps_5_0");
  pFinalShader->compileShader();

  // Plane Vertex shader
  auto pPlaneVS = sh_makeShared<Pass>();
  pPlaneVS->setVShaderInfo("resources/shaders/PlaneVertexShader.hlsl",
                           "main",
                           "vs_5_0");
  pPlaneVS->compileShader();

  // Histogram shader
  auto pHistogramShader = sh_makeShared<Pass>();
  pHistogramShader->setCShaderInfo("resources/shaders/HistogramShader.hlsl",
                                   "CSMain",
                                   "cs_5_0");
  pHistogramShader->compileShader();

  // Add skybox shader
  auto pASBShader = sh_makeShared<Pass>();
  pASBShader->setCShaderInfo("resources/shaders/AddSkyboxShader.hlsl",
                             "CSMain",
                             "cs_5_0");
  pASBShader->compileShader();

  // Luminance shader
  auto pLuminanceShader = sh_makeShared<Pass>();
  pLuminanceShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                   "LuminanceCS",
                                   "cs_5_0");
  pLuminanceShader->compileShader();

  // Bright shader
  auto pBrightShader = sh_makeShared<Pass>();
  pBrightShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "BrightCS",
                                "cs_5_0");
  pBrightShader->compileShader();

  // ToneMap shader
  auto pToneMapShader = sh_makeShared<Pass>();
  pToneMapShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                                 "ToneMapCS",
                                 "cs_5_0");
  pToneMapShader->compileShader();

  // AddMix shader
  auto pAddMixShader = sh_makeShared<Pass>();
  pAddMixShader->setCShaderInfo("resources/shaders/LuminanceShader.hlsl",
                                "AddMixCS",
                                "cs_5_0");
  pAddMixShader->compileShader();

  // PostProcess shader
  auto pPPShader = sh_makeShared<Pass>();
  pPPShader->setCShaderInfo("resources/shaders/PostProcessShader.hlsl",
                            "PostProcessCS",
                            "cs_5_0");
  pPPShader->compileShader();

  // Shader to calculate cube maps
  auto pCubeMapShader = sh_makeShared<Pass>();
  pCubeMapShader->setCShaderInfo("resources/shaders/CubeMapShader.hlsl",
                                 "CSMain",
                                 "cs_5_0");
  pCubeMapShader->compileShader();

  // Irradiance cube shader
  auto pIrrCubeShader = sh_makeShared<Pass>();
  pIrrCubeShader->setCShaderInfo("resources/shaders/IrradianceCubeShader.hlsl",
                                 "CSMain",
                                 "cs_5_0");
  pIrrCubeShader->compileShader();

  // Specular Prefiltered Cubemap shader
  auto pSPreCubeMap = sh_makeShared<Pass>();
  pSPreCubeMap->setCShaderInfo("resources/shaders/SpecularPreMapShader.hlsl",
                               "CSMain",
                               "cs_5_0");
  pSPreCubeMap->compileShader();

  // BRDF LUT shader
  auto pBRDFShader = sh_makeShared<Pass>();
  pBRDFShader->setCShaderInfo("resources/shaders/BRDFShader.hlsl",
                              "CSMain",
                              "cs_5_0");
  pBRDFShader->compileShader();

  // Raster state
  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kSolid;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  // Blend state
  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  // Basic depth stencil state
  DepthStencilDesc depthSDesc = {};
  depthSDesc.depthEnable = true;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kLess;
  depthSDesc.stencilEnable = true;
  depthSDesc.stencilReadMask = 0xFF;
  depthSDesc.stencilWriteMask = 0xFF;
  depthSDesc.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  depthSDesc.frontFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.backFace.stencilFailOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  depthSDesc.backFace.stencilPassOp = STENCIL_OP::kKeep;
  depthSDesc.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Depth stencil state for sky box
  DepthStencilDesc skyBoxDepth = {};
  skyBoxDepth.depthEnable = false;
  skyBoxDepth.depthWriteMask = DEPTH_WRITE_MASK::kZero;
  skyBoxDepth.depthFunc = COMPARISON_FUNC::kLessEqual;
  skyBoxDepth.stencilEnable = true;
  skyBoxDepth.stencilReadMask = 0xFF;
  skyBoxDepth.stencilWriteMask = 0xFF;
  skyBoxDepth.frontFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.frontFace.stencilDepthFailOp = STENCIL_OP::kIncr;
  skyBoxDepth.frontFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.frontFace.stencilFunc = COMPARISON_FUNC::kAlways;
  skyBoxDepth.backFace.stencilFailOp = STENCIL_OP::kKeep;
  skyBoxDepth.backFace.stencilDepthFailOp = STENCIL_OP::kDecr;
  skyBoxDepth.backFace.stencilPassOp = STENCIL_OP::kReplace;
  skyBoxDepth.backFace.stencilFunc = COMPARISON_FUNC::kAlways;

  // Dpeth stencil state for planes
  DepthStencilDesc planeDepthSDesc = depthSDesc;
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.stencilEnable = false;

  // States creation
  m_pSamplerLinearWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipLinear,
                                                     TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerPointWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipPoint,
                                                    TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerAnisotropicWrap = graphMan.createSamplerState(SAMPLER_FILTER::kFilterAnisotropic,
                                                          TEXTURE_ADDRESS_MODE::kWrap);
  m_pSamplerLinearClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipLinear,
                                                      TEXTURE_ADDRESS_MODE::kClamp);
  m_pSamplerPointClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterMinMagMipPoint,
                                                     TEXTURE_ADDRESS_MODE::kClamp);
  m_pSamplerAnisotropicClamp = graphMan.createSamplerState(SAMPLER_FILTER::kFilterAnisotropic,
                                                           TEXTURE_ADDRESS_MODE::kClamp);

  auto pRasterState = graphMan.createRasterizerState(rasterDesc);
  auto pBlendState = graphMan.createBlendState(blendDesc);
  auto pDepthStencil = graphMan.createDepthStencilState(depthSDesc);

  // Fill pass info
  // GBuffer
  pGbufferShader->generateInputLayout();
  //pGbufferShader->setSamplerState(m_pSamplerLinearWrap);
  pGbufferShader->setRasterizerState(pRasterState);
  pGbufferShader->setBlendState(pBlendState);
  pGbufferShader->setDepthStencilState(pDepthStencil);

  // Lightning
  //pLightCS->setSamplerState(m_pSamplerLinearWrap);

  // AO
  //pAOShader->setSamplerState(m_pSamplerLinearWrap);

  // Shadow Map
  pSMapShader->generateInputLayout();
  //pSMapShader->setSamplerState(m_pSamplerLinearWrap);
  pSMapShader->setRasterizerState(pRasterState);
  pSMapShader->setBlendState(pBlendState);
  pSMapShader->setDepthStencilState(pDepthStencil);

  // SkyBox Map
  //pSkyBoxShader->generateInputLayout();
  //pSkyBoxShader->setSamplerState(m_pSamplerLinearWrap);
  //pSkyBoxShader->setRasterizerState(pRasterState);
  //pSkyBoxShader->setBlendState(pBlendState);
  //pSkyBoxShader->setDepthStencilStateFromDesc(skyBoxDepth);

  // Final
  //pFinalShader->setSamplerState(m_pSamplerLinearWrap);

  // Plane vs
  pPlaneVS->generateInputLayout();
  pPlaneVS->setDepthStencilStateFromDesc(planeDepthSDesc);

  // ToneMap
  //pToneMapShader->setSamplerState(m_pSamplerLinearWrap);

  // Bright
  //pBrightShader->setSamplerState(m_pSamplerLinearWrap);

  // Save passes
  m_passes[StringID("GBufferShader").getID()] = pGbufferShader;
  m_passes[StringID("AOShader").getID()] = pAOShader;
  m_passes[StringID("HBlurShader").getID()] = pHBlurShader;
  m_passes[StringID("VBlurShader").getID()] = pVBlurShader;
  m_passes[StringID("HBlurCS").getID()] = pVBlurShader;
  m_passes[StringID("VBlurCS").getID()] = pVBlurShader;
  m_passes[StringID("LightCS").getID()] = pLightCS;
  m_passes[StringID("SMapShader").getID()] = pSMapShader;
  m_passes[StringID("SkyBoxShader").getID()] = pSkyBoxShader;
  m_passes[StringID("FinalShader").getID()] = pFinalShader;
  m_passes[StringID("PlaneShader").getID()] = pPlaneVS;
  m_passes[StringID("HistogramShader").getID()] = pHistogramShader;
  m_passes[StringID("ASBShader").getID()] = pASBShader;
  m_passes[StringID("LuminanceShader").getID()] = pLuminanceShader;
  m_passes[StringID("BrightShader").getID()] = pBrightShader;
  m_passes[StringID("ToneMapShader").getID()] = pToneMapShader;
  m_passes[StringID("PPShader").getID()] = pPPShader;
  m_passes[StringID("AddMixShader").getID()] = pAddMixShader;
  m_passes[StringID("CubeMapShader").getID()] = pCubeMapShader;
  m_passes[StringID("IrrCubeShader").getID()] = pIrrCubeShader;
  m_passes[StringID("SpecularPreMapShader").getID()] = pSPreCubeMap;
  m_passes[StringID("BRDFShader").getID()] = pBRDFShader;
}

SPtr<Pass>
RenderManager::getPass(const String& passName)
{
  StringID strID(passName);
  auto pPass = m_passes.find(strID.getID());

  if (pPass != m_passes.end()) {
    return (*pPass).second;
  }

  return nullptr;
}

SPtr<Texture2D>
RenderManager::getTexture(const String& texName)
{
  StringID strID(texName);
  auto pTex = m_renderTargetMap.find(strID.getID());

  if (pTex != m_renderTargetMap.end()) {
    return (*pTex).second.pTexture;
  }

  return nullptr;
}

void RenderManager::recompileShaders()
{
  for(auto& pPass : m_passes) {
    pPass.second->compileShader();
  }
}

void
RenderManager::drawStaticMeshOnScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

  if (m_pModelTransform) {
    graphMan.vsSetConstantBuffers(m_pModelTransform, 2);
  }
  else {
    m_pModelTransform = graphMan.createConstantBuffer(sizeof(Matrix4));
    Matrix4 identity = Matrix4::IDENTITY;
    graphMan.updateConstantBuffer(m_pModelTransform, &identity, sizeof(Matrix4));
    graphMan.vsSetConstantBuffers(m_pModelTransform, 2);
  }

  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      drawStaticMesh(gameObject);
    }
  }
  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      drawTransparentStaticMesh(gameObject);
    }
  }
}

void
RenderManager::drawStaticMesh(const WPtr<GameObject> pGO)
{
  if( pGO.expired() ) {
    return;
  }
  GraphicsManager& graphMan = g_graphicsMan();

  auto pGameObject = pGO.lock();

  for (auto& component : pGameObject->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
        pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

      Transform modelT = pGameObject->transform.getTransform();
      graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        uint32 matIndex = mesh.materialIndex;
        bool bHasAlphaTest = meshResource->m_materials[matIndex]->m_properties.bHasAlphaTest;
        if (!bHasAlphaTest && mesh.bVisible) {
          setResourceViewFromPBRMaterial(meshResource->m_materials[mesh.materialIndex]);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->childs) {
    drawStaticMesh(pChild);
  }
}

void
RenderManager::drawTransparentStaticMesh(const WPtr<GameObject> pGO)
{
  if (pGO.expired()) {
    return;
  }
  GraphicsManager& graphMan = g_graphicsMan();

  auto pGameObject = pGO.lock();

  for (auto& component : pGameObject->components) {
    if (component->getType() == COMPONENT_TYPE::kStaticMesh &&
      pGameObject->m_bActive) {
      auto meshComponent = cast::re_ptr<StaticMeshComponent>(component);

      graphMan.setVertexBuffers(meshComponent->getVertexBuffer());
      graphMan.setIndexBuffers(meshComponent->getIndexBuffer());

      Transform modelT = pGameObject->transform.getTransform();
      graphMan.updateConstantBuffer(m_pModelTransform, &modelT, sizeof(Transform));

      uint32 indexCount = 0;
      uint32 vertexCount = 0;
      auto& meshResource = meshComponent->m_mesh;
      for (auto& mesh : meshResource->m_meshes) {
        uint32 matIndex = mesh.materialIndex;
        bool bHasAlphaTest = meshResource->m_materials[matIndex]->m_properties.bHasAlphaTest;
        if (bHasAlphaTest && mesh.bVisible) {
          setResourceViewFromPBRMaterial(meshResource->m_materials[mesh.materialIndex]);
          graphMan.drawIndexed(mesh.numIndices, indexCount, vertexCount);
        }
        indexCount += mesh.numIndices;
        vertexCount += mesh.numVertices;
      }
    }
  }

  for (auto& pChild : pGameObject->childs) {
    drawTransparentStaticMesh(pChild);
  }
}

void
RenderManager::setResourceViewFromPBRMaterial(const SPtr<Material>& pMat)
{
  GraphicsManager& graphMan = g_graphicsMan();

  if (pMat->m_type != MATERIAL_TYPE::kPBR) {
    return;
  }

  struct PBRMaterialData {
    Vector3 baseColorFactor;
    float unused0; // Padding to align to 16 bytes
    Vector2 metallicRoughnessFactor;
    MaterialProperties properties;
    float unused1; // Padding to align to 16 bytes
  };

  PBRMaterialData pbrData = {};
  pbrData.baseColorFactor = pMat->baseColorFactor;
  pbrData.metallicRoughnessFactor = pMat->metallicRoughnessFactor;
  pbrData.properties = pMat->m_properties;

  /*if (m_pPBRData == nullptr) {
    graphMan.createConstantBuffer(sizeof(PBRMaterialData));
  }*/
  graphMan.updateConstantBuffer(m_pPBRData, &pbrData, sizeof(PBRMaterialData));
  graphMan.psSetConstantBuffers(m_pPBRData, 3);

  if (pMat->baseColor) {
    graphMan.psSetShaderResourceView(pMat->baseColor);
  }

  if (pMat->normal) {
    graphMan.psSetShaderResourceView(pMat->normal, 1);
  }

  if (pMat->metallic) {
    graphMan.psSetShaderResourceView(pMat->metallic, 2);
  }

  if (pMat->roughness) {
    graphMan.psSetShaderResourceView(pMat->roughness, 3);
  }

  if (pMat->ao) {
    graphMan.psSetShaderResourceView(pMat->ao, 4);
  }

  if (pMat->m_properties.bHasAlphaTest) {
    graphMan.setBlendState(m_pAlphaTestBS);
  }
  else {
    graphMan.setBlendState(m_pBasicBS);
  }
}

static uint32
threadGroups(uint32 size, uint32 threads)
{
  uint32 groups = size / threads;
  if (size % threads > 0) {
    groups++;
  }
  return groups;
}

void
RenderManager::renderScene()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

  SPtr<Texture2D> pOutput;
  SPtr<Texture2D> pInput;

  auto pDepthSV = graphMan.getMainDepthStencil();

  // Get render targets
  auto pMainTarget = graphMan.getMainRenderTargetView();
  auto& pShadowMap = m_renderTargetMap[StringID("ShadowMap").getID()];
  auto& pShadowTemp = m_renderTargetMap[StringID("ShadowTemp").getID()];
  auto& pDepthMap = m_renderTargetMap[StringID("DepthMap").getID()];
  auto& pNormalMap = m_renderTargetMap[StringID("NormalMap").getID()];
  auto& pColorMap = m_renderTargetMap[StringID("ColorMap").getID()];
  auto& pPropMap = m_renderTargetMap[StringID("PropMap").getID()];
  auto& pAOMap = m_renderTargetMap[StringID("AOMap").getID()];
  auto& pHBlurMap = m_renderTargetMap[StringID("HBlurMap").getID()];
  auto& pVBlurMap = m_renderTargetMap[StringID("VBlurMap").getID()];
  auto& pSkyBoxMap = m_renderTargetMap[StringID("SkyBoxMap").getID()];
  auto& pLightCMap = m_renderTargetMap[StringID("LightCMap").getID()];
  auto& pToneMap = m_renderTargetMap[StringID("ToneMap").getID()];
  auto& pTempMap = m_renderTargetMap[StringID("TempMap").getID()];
  auto& pLuminance = m_renderTargetMap[StringID("LuminanceMap").getID()];
  auto& pBrightMap = m_renderTargetMap[StringID("BrightMap").getID()];
  auto& pPPMap = m_renderTargetMap[StringID("PPMap").getID()];
  auto& pAdditiveMap = m_renderTargetMap[StringID("AdditiveMap").getID()];
  auto& pBHBlur = m_renderTargetMap[StringID("BHBlur").getID()];
  auto& pBVBlur = m_renderTargetMap[StringID("BVBlur").getID()];
  auto& pGbufferDepth = m_renderTargetMap[StringID("GbufferDepth").getID()];
  //auto& pCubeMap = m_renderTargetMap[StringID("CubeMap").getID()];
  //auto& pDiffIrrMap = m_renderTargetMap[StringID("DiffIrrMap").getID()];
  //auto& pSpecPreMap = m_renderTargetMap[StringID("SpecPreMap").getID()];
  //auto& pBRDFMap = m_renderTargetMap[StringID("BRDFLut").getID()];
  //auto& pHistogramMap = m_renderTargetMap[StringID("HistogramMap").getID()];

  uint32 screenWidth = static_cast<uint32>(m_screenDimension.x);
  uint32 screenHeight = static_cast<uint32>(m_screenDimension.y);

  uint32 dispatchX = threadGroups(screenWidth, DEFAULT_THREADS);
  uint32 dispatchY = threadGroups(screenHeight, DEFAULT_THREADS);
  uint32 dispatchZ = 1;

  uint32 cubeDispatch = threadGroups(CUBE_MAP_SIZE, DEFAULT_THREADS);

  Viewport normalVP = {};
  normalVP.width = m_screenDimension.x;
  normalVP.height = m_screenDimension.y;
  normalVP.minDepth = 0.0f;
  normalVP.maxDepth = 1.0f;
  normalVP.topLeftX = 0.0f;
  normalVP.topLeftY = 0.0f;

  Viewport shadowVP = {};
  shadowVP.width = m_shadowMapSize;
  shadowVP.height = m_shadowMapSize;
  shadowVP.minDepth = 0.0f;
  shadowVP.maxDepth = 1.0f;
  shadowVP.topLeftX = 0.0f;
  shadowVP.topLeftY = 0.0f;

  Viewport lumVP = {};
  lumVP.width = LUMINANCE_MAP_SIZE;
  lumVP.height = LUMINANCE_MAP_SIZE;
  lumVP.minDepth = 0.0f;
  lumVP.maxDepth = 1.0f;
  lumVP.topLeftX = 0.0f;
  lumVP.topLeftY = 0.0f;

  Viewport quadVP = {};
  quadVP.width = m_screenDimension.x * 0.5f;
  quadVP.height = m_screenDimension.y * 0.5f;
  quadVP.minDepth = 0.0f;
  quadVP.maxDepth = 1.0f;
  quadVP.topLeftX = 0.0f;
  quadVP.topLeftY = 0.0f;

  /*************************************/
  /*          Shadow Mapping           */
  /*************************************/
  pOutput = pShadowMap.pTexture;
  graphMan.setViewport(shadowVP);
  graphMan.clearDepthStencil(pOutput);
  graphMan.clearRenderTarget(pShadowTemp.pTexture, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pShadowTemp.pTexture }}, pOutput);
  m_passes[StringID("SMapShader").getID()]->setPass();
  drawStaticMeshOnScene();
  cleanShaderObjects();

  /*************************************/
  /*              GBuffer              */
  /*************************************/
  graphMan.setViewport(normalVP);
  graphMan.clearDepthStencil(pGbufferDepth.pTexture);
  graphMan.clearRenderTarget(pDepthMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pNormalMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pColorMap.pTexture, LinearColor::BLACK);
  graphMan.clearRenderTarget(pPropMap.pTexture, LinearColor::BLACK);
  graphMan.clearDepthStencil(pDepthSV);
  graphMan.setRenderTargets({{pDepthMap.pTexture},
                             {pNormalMap.pTexture},
                             {pColorMap.pTexture},
                             {pPropMap.pTexture }}, pGbufferDepth.pTexture);
  m_passes[StringID("GBufferShader").getID()]->setPass();
  setSamplers();
  drawStaticMeshOnScene();
  cleanShaderObjects();

  /*************************************/
  /*         Ambient Occlusion         */
  /*************************************/
  pOutput = pAOMap.pTexture;
  graphMan.clearRenderTarget(pOutput, LinearColor::BLACK);
  graphMan.setRenderTargets({{ pOutput }}, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("AOShader").getID()]->setPass();
  setSamplers();
  graphMan.psSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.psSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.draw(3, 0);
  cleanShaderObjects();

  /*************************************/
  /*        AO Horizontal Blur         */
  /*************************************/
  m_shaderData.mipLevel0 = 1;
  graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));
  pInput = pAOMap.pTexture;
  pOutput = pHBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HBlurShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  /*graphMan.dispatch(threadGroups(screenWidth, BLURH_THREADS_X),
                    threadGroups(screenHeight, BLURH_THREADS_Y),
                    1);*/
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*          AO Vetical Blur          */
  /*************************************/
  pInput = pHBlurMap.pTexture;
  pOutput = pVBlurMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("VBlurShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  /*graphMan.dispatch(threadGroups(screenWidth, BLURV_THREADS_X),
                    threadGroups(screenHeight, BLURV_THREADS_Y),
                    1);*/
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*              Sky Box              */
  /*************************************/
  uint32 numMipLevels = 0;
  uint32 maxSize = Math::max(m_skyboxDimension.x, m_skyboxDimension.y);
  for (uint32 i = 0; i < maxSize; ++i) {
    if (maxSize >> i < 32) {
      break;
    }
    ++numMipLevels;
  }
  float normMip = Math::clamp(m_prefilteredCB.roughness, 0.0f, 1.0f);
  m_prefilteredCB.mipmapLevels = normMip * (numMipLevels - 1) + 0.5f;
  updatePrefilteredIBLBuffer();

  m_passes[StringID("SkyBoxShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetConstantBuffers(m_pPreCB, 2);
  graphMan.csSetShaderResourceView(m_pSpecularPreMap, 0);
  graphMan.setUnorderedAccessView({ pSkyBoxMap.pTexture }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  float tempRoughness = m_prefilteredCB.roughness;
  m_prefilteredCB.roughness = 0.0f;
  updatePrefilteredIBLBuffer();
  m_prefilteredCB.roughness = tempRoughness;

  /*************************************/
  /*             Lightning             */
  /*************************************/
  pOutput = pLightCMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LightCS").getID()]->setPass();
  setSamplers();
  graphMan.csSetConstantBuffers(m_pPreCB, 4);
  graphMan.csSetShaderResourceView(pDepthMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pNormalMap.pTexture, 1);
  graphMan.csSetShaderResourceView(pColorMap.pTexture, 2);
  graphMan.csSetShaderResourceView(pPropMap.pTexture, 3);
  graphMan.csSetShaderResourceView(pVBlurMap.pTexture, 4);
  graphMan.csSetShaderResourceView(pShadowTemp.pTexture, 5);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 6);
  //graphMan.csSetShaderResourceView(m_pEnvTexture, 7);
  graphMan.csSetShaderResourceView(m_pBRDF, 7);
  graphMan.csSetShaderResourceView(m_pDiffIrr, 8);
  graphMan.csSetShaderResourceView(m_pSpecularPreMap, 9);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*            Add Sky Box            */
  /*************************************/
  pOutput = pTempMap.pTexture;
  graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ASBShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pLightCMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pGbufferDepth.pTexture, 1);
  graphMan.csSetShaderResourceView(pSkyBoxMap.pTexture, 2);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();

  /*************************************/
  /*             Luminance             */
  /*************************************/
  pInput = pLightCMap.pTexture;
  pOutput = pLuminance.pTexture;
  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("LuminanceShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pInput, 0);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*               Bright              */
  /*************************************/
  pInput = pLuminance.pTexture;
  pOutput = pBrightMap.pTexture;
  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("BrightShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pInput, 1);
  graphMan.setUnorderedAccessView({ pOutput }, 0);
  graphMan.dispatch(threadGroups(static_cast<uint32>(screenWidth * 0.5f), DEFAULT_THREADS),
                    threadGroups(static_cast<uint32>(screenHeight * 0.5f), DEFAULT_THREADS),
                    1);
  cleanShaderObjects();
  graphMan.generateMips(pOutput);

  /*************************************/
  /*   Blur mips and do additive mix   */
  /*************************************/
  int32 startMip = 3;
  int32 mipCount = 4;
  for (int32 i = 0; i < mipCount; ++i) {
    int32 mipLevel0 = startMip - i;
    int32 mipLevel1 = mipLevel0 + 1;
    
    int32 width = static_cast<int32>(static_cast<int32>(screenWidth) >> (mipLevel0 + 1));
    int32 height = static_cast<int32>(static_cast<int32>(screenHeight) >> (mipLevel0 + 1));

    // Update buffers
    m_mainBufferData.screenSize.x = static_cast<float>(width);
    m_mainBufferData.screenSize.y = static_cast<float>(height);
    m_shaderData.mipLevel0 = static_cast<float>(mipLevel0);
    m_shaderData.mipLevel1 = static_cast<float>(mipLevel1);
    graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));
    graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));

    /*************************************/
    /*            Additive Mix           */
    /*************************************/
    pInput = pBrightMap.pTexture;
    if (i > 0) {
      pInput = pAdditiveMap.pTexture;
      //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
      m_passes[StringID("AddMixShader").getID()]->setPass();
      setSamplers();
      //graphMan.csSetSamplerState(m_pSamplerLinearClamp, 1);
      graphMan.csSetShaderResourceView(pBrightMap.pTexture, 0);
      graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
      graphMan.setUnorderedAccessView({ pAdditiveMap.pTexture, mipLevel0 }, 0);
      graphMan.dispatch(threadGroups(width, DEFAULT_THREADS),
                        threadGroups(height, DEFAULT_THREADS),
                        1);
      cleanShaderObjects();
      //graphMan.generateMips(pAdditiveMap.pTexture);
    }

    /*************************************/
    /*          Horizontal Blur          */
    /*************************************/
    //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    m_passes[StringID("HBlurShader").getID()]->setPass();
    //m_passes[StringID("HBlurCS").getID()]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pInput, 0);
    graphMan.setUnorderedAccessView({ pBHBlur.pTexture, mipLevel0 }, 0);
    /*graphMan.dispatch(threadGroups(width, BLURH_THREADS_X),
                      threadGroups(height, BLURH_THREADS_Y),
                      1);*/
    graphMan.dispatch(threadGroups(screenWidth, DEFAULT_THREADS),
                      threadGroups(screenHeight, DEFAULT_THREADS),
                      1);
    cleanShaderObjects();

    /*************************************/
    /*            Vetical Blur           */
    /*************************************/
    //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
    m_passes[StringID("VBlurShader").getID()]->setPass();
    //m_passes[StringID("VBlurCS").getID()]->setPass();
    setSamplers();
    graphMan.csSetShaderResourceView(pBHBlur.pTexture, 0);
    graphMan.setUnorderedAccessView({ pBVBlur.pTexture, mipLevel0 }, 0);
    /*graphMan.dispatch(threadGroups(width, BLURH_THREADS_X),
                      threadGroups(height, BLURH_THREADS_Y),
                      1);*/
    graphMan.dispatch(threadGroups(screenWidth, DEFAULT_THREADS),
                      threadGroups(screenHeight, DEFAULT_THREADS),
                      1);
    cleanShaderObjects();
  }

  /*************************************/
  /*             Tone Map              */
  /*************************************/
  m_mainBufferData.screenSize.x = m_screenDimension.x;
  m_mainBufferData.screenSize.y = m_screenDimension.y;
  graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));

  SPtr<Texture3D> pLutTex;
  int32 index = static_cast<int32>(m_shaderData.toneMappingIndex);
  if (index == 4) {
    m_shaderData.lutSize = m_pLutTexture->getLutSize();
    pLutTex = m_pLutTexture->getTexture();
  }
  else if (index == 5) {
    m_shaderData.lutSize = m_pLutLBK->getLutSize();
    pLutTex = m_pLutLBK->getTexture();
    m_shaderData.toneMappingIndex = 4.0f;
  }
  else {
    m_shaderData.lutSize = 0;
    pLutTex = nullptr;
  }
  updateShaderDataBuffer();

  //pInput = pVBlurMap.pTexture;
  //pOutput = pToneMap.pTexture;
  graphMan.setViewport(normalVP);

  //graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("ToneMapShader").getID()]->setPass();
  setSamplers();
  //graphMan.csSetSamplerState(m_pSamplerLinearClamp, 1);
  graphMan.csSetShaderResourceView(pTempMap.pTexture, 0);
  graphMan.csSetShaderResourceView(pBVBlur.pTexture, 1);
  graphMan.csSetShaderResourceView(pLuminance.pTexture, 2);
  graphMan.csSetShaderResourceView(pLutTex, 3);
  graphMan.setUnorderedAccessView({ pToneMap.pTexture }, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  m_shaderData.toneMappingIndex = cast::st<float>(index);
  updateShaderDataBuffer();

  /*************************************/
  /*            PostProcess            */
  /*************************************/
  m_passes[StringID("PPShader").getID()]->setPass();
  setSamplers();

  graphMan.csSetShaderResourceView(pToneMap.pTexture, 0);
  graphMan.setUnorderedAccessView({ pPPMap.pTexture }, 0);

  graphMan.dispatch(dispatchX, dispatchY, dispatchZ);

  cleanShaderObjects();

  /*************************************/
  /*         Add to backbuffer         */
  /*************************************/
  graphMan.setRenderTargets({{ pMainTarget }}, pDepthSV);
  m_passes[StringID("PlaneShader").getID()]->setPass();
  m_passes[StringID("FinalShader").getID()]->setPass();
  setSamplers();

  graphMan.psSetShaderResourceView(pPPMap.pTexture, 0);

  graphMan.draw(3, 0);

  cleanShaderObjects();

  /*************************************/
  /*             Histogram             */
  /*************************************/
  /*graphMan.setRenderTargets({ pMainTarget }, pDepthSV);
  m_passes[StringID("HistogramShader").getID()]->setPass();
  setSamplers();

  graphMan.csSetShaderResourceView(pPPMap.pTexture, 0);
  graphMan.setUnorderedAccessView(pHistogramMap.pTexture, 0);

  uint32 dx = static_cast<uint32>((HISTOGRAM_MAP_SIZE + DEFAULT_THREADS) / DEFAULT_THREADS);
  uint32 dy = 3;
  uint32 dz = 1;

  graphMan.dispatch(dx, dy, dz);

  cleanShaderObjects();*/
}

void
RenderManager::computeIBL()
{
  GraphicsManager& graphMan = g_graphicsMan();
  SceneGraph& scene = g_sceneGraph();

  m_pSpecularPreMap = nullptr;
  m_skyboxDimension.x = TEXTURE8K_WIDTH;
  m_skyboxDimension.y = TEXTURE4K_WIDTH;

  SPtr<ImageResource> pSbImg;
  for (auto& gameObject : scene.getGameObjectList()) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kSkyBox) {
        auto pSkyBox = sh_reinterpretPCast<SkyBoxComponent>(component);
        pSbImg = pSkyBox->getSkyBoxResource();
        break;
      }
    }
  }

  m_skyboxDimension.x = cast::st<int32>(pSbImg->width);
  m_skyboxDimension.y = cast::st<int32>(pSbImg->height);
  uint32 numMipLevels = 0;
  uint32 maxSize = Math::max(m_skyboxDimension.x, m_skyboxDimension.y);
  for (uint32 i = 0; i < maxSize; ++i) {
    if (maxSize >> i < 32) {
      break;
    }
    ++numMipLevels;
  }
  
  m_pSpecularPreMap = graphMan.createTexture2D(cast::st<float>(m_skyboxDimension.x),
                               cast::st<float>(m_skyboxDimension.y),
                               TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                               USAGE::kDefault,
                               BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                               numMipLevels);

  m_prefilteredCB.width = 256;
  m_prefilteredCB.height = 128;
  m_prefilteredCB.samples = TEXTURE4K_WIDTH;
  m_prefilteredCB.roughness = 0.0f;
  m_prefilteredCB.mipmapLevels = 0.0f;

  if(m_pPreCB == nullptr) {
    m_pPreCB = graphMan.createConstantBuffer(sizeof(PrefilteredCB));
  }
  graphMan.updateConstantBuffer(m_pPreCB, &m_prefilteredCB, sizeof(PrefilteredCB));

  /*************************************/
  /*           Diff Irradiance         */
  /*************************************/
  m_passes[StringID("IrrCubeShader").getID()]->setPass();
  setSamplers();
  graphMan.csSetConstantBuffers(m_pPreCB, 2);
  graphMan.csSetShaderResourceView(pSbImg->texture, 0);
  graphMan.setUnorderedAccessView({ m_pDiffIrr }, 0);
  graphMan.dispatch(threadGroups(256, 16),
                    threadGroups(128, 16),
                    1);

  cleanShaderObjects();

  graphMan.generateMips(m_pDiffIrr);

  /*************************************/
  /*      Specular Prefiltered Map     */
  /*************************************/
  PrefilteredCB specPreCB = {};

  for (int32 mip = 0; mip < numMipLevels; ++mip) {
    Vector2i preSize = Vector2i(m_skyboxDimension.x >> mip, m_skyboxDimension.y >> mip);
    Vector2i dispatchSize = Vector2i(threadGroups(preSize.x, 8),
                                     threadGroups(preSize.y, 8));

    specPreCB.width = preSize.x;
    specPreCB.height = preSize.y;
    specPreCB.samples = TEXTURE4K_WIDTH;
    specPreCB.mipmapLevels = cast::st<float>(mip);
    specPreCB.roughness = (mip == 0) ? 0.0f :
      Math::clamp(cast::st<float>(mip) / cast::st<float>(numMipLevels - 1), 0.0f, 1.0f);

    graphMan.updateConstantBuffer(m_pPreCB, &specPreCB, sizeof(PrefilteredCB));

    m_passes[StringID("SpecularPreMapShader").getID()]->setPass();
    setSamplers();
    graphMan.csSetConstantBuffers(m_pPreCB, 2);
    graphMan.csSetShaderResourceView(pSbImg->texture, 0);
    graphMan.setUnorderedAccessView({ m_pSpecularPreMap, mip }, 0);
    graphMan.dispatch(dispatchSize.x, dispatchSize.y, 1);

    cleanShaderObjects();
  }

  graphMan.updateConstantBuffer(m_pPreCB, &m_prefilteredCB, sizeof(PrefilteredCB));
}

void
RenderManager::computeBRDF()
{
  GraphicsManager& graphMan = g_graphicsMan();
  
  m_pBRDF = graphMan.createTexture2D(256, 256,
                                     TEXTURE_FORMAT::kR32G32B32A32_FLOAT,
                                     USAGE::kDefault,
                                     BIND_FLAGS::kShaderResource | BIND_FLAGS::kUnorderedAccess,
                                     1, 1);
  
  uint32 lutDispatch = threadGroups(256, 16);

  m_passes[StringID("BRDFShader").getID()]->setPass();
  setSamplers();
  graphMan.setUnorderedAccessView({ m_pBRDF }, 0);
  graphMan.dispatch(lutDispatch, lutDispatch, 1);

  cleanShaderObjects();
}

void
RenderManager::cleanPSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.psSetShaderResourceView(pSRV, i);
  }
}

void
RenderManager::cleanCSShaderResourceView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.csSetShaderResourceView(pSRV, i);
  }
}

void
RenderManager::cleanCSUAView(uint32 numSRV)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numSRV; ++i) {
    SPtr<Texture2D> pSRV = nullptr;
    graphMan.setUnorderedAccessView({pSRV}, i);
  }
}

void
RenderManager::cleanVSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.vsSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanPSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.psSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanGSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.gsSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanCSConstantBuffers(uint32 numCB)
{
  GraphicsManager& graphMan = g_graphicsMan();

  for (uint32 i = 0; i < numCB; ++i) {
    SPtr<ConstantBuffer> pBuff = nullptr;
    graphMan.csSetConstantBuffers(pBuff, i);
  }
}

void
RenderManager::cleanShaderObjects()
{
  cleanPSShaderResourceView();
  cleanCSShaderResourceView();
  cleanCSUAView(1);
  cleanVSConstantBuffers();
  cleanPSConstantBuffers();
  cleanGSConstantBuffers();
  cleanCSConstantBuffers();
}

void
RenderManager::updateShaderDataBuffer()
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (m_pShaderDataBuffer == nullptr) {
    m_pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
  }

  graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));
}

void
RenderManager::updatePrefilteredIBLBuffer()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pPreCB, &m_prefilteredCB, sizeof(PrefilteredCB));
}

void
RenderManager::setSamplers()
{
  GraphicsManager& graphMan = g_graphicsMan();

  graphMan.psSetSamplerState(m_pSamplerLinearWrap, 0);
  graphMan.psSetSamplerState(m_pSamplerPointWrap, 1);
  graphMan.psSetSamplerState(m_pSamplerAnisotropicWrap, 2);
  graphMan.psSetSamplerState(m_pSamplerLinearClamp, 3);
  graphMan.psSetSamplerState(m_pSamplerPointClamp, 4);
  graphMan.psSetSamplerState(m_pSamplerAnisotropicClamp, 5);

  graphMan.csSetSamplerState(m_pSamplerLinearWrap, 0);
  graphMan.csSetSamplerState(m_pSamplerPointWrap, 1);
  graphMan.csSetSamplerState(m_pSamplerAnisotropicWrap, 2);
  graphMan.csSetSamplerState(m_pSamplerLinearClamp, 3);
  graphMan.csSetSamplerState(m_pSamplerPointClamp, 4);
  graphMan.csSetSamplerState(m_pSamplerAnisotropicClamp, 5);
}

void
RenderManager::setShadowMapSize(const float size)
{
  m_shadowMapSize = size;
}

void
RenderManager::setScreenSize(const Vector2& screenD)
{
  m_screenDimension = screenD;
}

RenderManager& g_renderMan()
{
  return RenderManager::instance();
}
}
