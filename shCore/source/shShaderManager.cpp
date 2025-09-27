/*****************************************************************************/
/*
*  @file    shShaderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/24
*  @brief   Shader managment module.
*
*  Shader managment module.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shShaderManager.h"
#include "shGraphicsManager.h"
#include "shPass.h"
//#include "shMaterial.h"
#include "shBuffers.h"
#include "shRasterizerState.h"
#include "shBlendState.h"
#include "shDepthStencilState.h"
#include "shStringID.h"

namespace shEngineSDK {
void
ShaderManager::createPipelinePasses()
{
  GraphicsManager& graphMan = g_graphicsMan();

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

  DepthStencilDesc planeDepthSDesc = depthSDesc;
  planeDepthSDesc.depthEnable = false;
  planeDepthSDesc.stencilEnable = false;

  // Shadow Map
  pSMapShader->generateInputLayout();
  pSMapShader->setRasterizerStateFromDesc(rasterDesc);
  pSMapShader->setBlendStateFromDesc(blendDesc);
  pSMapShader->setDepthStencilStateFromDesc(depthSDesc);

  // Plane vs
  pPlaneVS->generateInputLayout();
  pPlaneVS->setDepthStencilStateFromDesc(planeDepthSDesc);

  m_pShaderDataBuffer = graphMan.createConstantBuffer(sizeof(ShaderData));
  m_pMainBuffer = graphMan.createConstantBuffer(sizeof(MainBufferData));
  m_pPrefilteredCB = graphMan.createConstantBuffer(sizeof(PrefilteredCB));
  m_pLightBuffer = graphMan.createConstantBuffer(sizeof(LightCB));

  // Shadow shader buffers
  pSMapShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pSMapShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);
  pSMapShader->addVSConstantBuffer(m_pLightBuffer, 3);

  // Skybox shader buffers
  pSkyBoxShader->addCSConstantBuffer(m_pMainBuffer, 0);

  // Lightning shader buffers
  pLightCS->addCSConstantBuffer(m_pMainBuffer, 0);
  pLightCS->addCSConstantBuffer(m_pShaderDataBuffer, 1);
  pLightCS->addCSConstantBuffer(m_pLightBuffer, 2);
  pLightCS->addCSConstantBuffer(m_pPrefilteredCB, 3);

  // Ambient occlusion buffers
  pAOShader->addPSConstantBuffer(m_pMainBuffer, 0);
  pAOShader->addPSConstantBuffer(m_pShaderDataBuffer, 1);
  // Post process buffers
  pPPShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pPPShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Tone map buffers
  pToneMapShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pToneMapShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Luminance buffers
  pLuminanceShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pLuminanceShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Bright buffers
  pBrightShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pBrightShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // AddMix buffers
  pAddMixShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pAddMixShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Blur buffers
  pHBlurShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pHBlurShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);
  pVBlurShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pVBlurShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Blur buffers
  pHBlurCS->addCSConstantBuffer(m_pMainBuffer, 0);
  pHBlurCS->addCSConstantBuffer(m_pShaderDataBuffer, 1);
  pVBlurCS->addCSConstantBuffer(m_pMainBuffer, 0);
  pVBlurCS->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Specular prefiltered cubemap
  pSPreCubeMap->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Final shader buffers
  pFinalShader->addPSConstantBuffer(m_pMainBuffer, 0);

  // Histogram
  pHistogramShader->addCSConstantBuffer(m_pMainBuffer, 0);

  // Add skybox
  pASBShader->addCSConstantBuffer(m_pMainBuffer, 0);

  pIrrCubeShader->addCSConstantBuffer(m_pPrefilteredCB, 2);
  pSPreCubeMap->addCSConstantBuffer(m_pPrefilteredCB, 2);

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
  m_passes[StringID("DiffIrrShader").getID()] = pIrrCubeShader;
  m_passes[StringID("SpecularPreMapShader").getID()] = pSPreCubeMap;
  m_passes[StringID("BRDFShader").getID()] = pBRDFShader;
}

SPtr<Pass>
ShaderManager::getPassFromMaterial(const MaterialProperties& props)
{
  uint32 shaderID = props.properties.value;
  auto passIt = m_passes.find(shaderID);
  if(passIt != m_passes.end()) {
    return (*passIt).second;
  }

  auto matPass = sh_makeShared<Pass>();
  bool bWireframe = props.properties.flags.bWireframeEnabled;
  //bool bDoubleSided = props.properties.flags.bIsDoubleSided;
  bool bAlphaTest = props.properties.flags.bHasAlphaTest;

  // Create and configure the Pass based on MaterialProperties (props)
  Vector<ShaderMacro>& macros = matPass->m_macros;

  if(props.properties.flags.bHasDiffuseMap) {
    macros.push_back({"HAS_DIFFUSE_MAP", "1"});
  }
  if(props.properties.flags.bHasSpecularMap) {
    macros.push_back({"HAS_SPECULAR_MAP", "1"});
  }
  if(props.properties.flags.bHasNormalMap) {
    macros.push_back({"HAS_NORMAL_MAP", "1"});
  }
  if(props.properties.flags.bHasMetalnessMap) {
    macros.push_back({"HAS_METALNESS_MAP", "1"});
  }
  if(props.properties.flags.bHasRoughnessMap) {
    macros.push_back({"HAS_ROUGHNESS_MAP", "1"});
  }
  if(props.properties.flags.bHasAmbientOcclusionMap) {
    macros.push_back({"HAS_AO_MAP", "1"});
  }
  if(props.properties.flags.bHasEmissiveMap) {
    macros.push_back({"HAS_EMISSIVE_MAP", "1"});
  }
  if(props.properties.flags.bIsOpaque) {
    macros.push_back({"IS_OPAQUE", "1"});
  }
  if(props.properties.flags.bHasAlphaTest) {
    macros.push_back({"HAS_ALPHA_TEST", "1"});
  }
  if(props.properties.flags.bHasAlphaBlend) {
    macros.push_back({"HAS_ALPHA_BLEND", "1"});
  }
  if(props.properties.flags.bIsDoubleSided) {
    macros.push_back({"IS_DOUBLE_SIDED", "1"});
  }
  if(props.properties.flags.bWireframeEnabled) {
    macros.push_back({"WIREFRAME_ENABLED", "1"});
  }
  if(props.properties.flags.bCanCastShadows) {
    macros.push_back({"CAN_CAST_SHADOWS", "1"});
  }
  if(props.properties.flags.bCanReceiveShadows) {
    macros.push_back({"CAN_RECEIVE_SHADOWS", "1"});
  }
  if(props.properties.flags.bInvertRoughness) {
    macros.push_back({"INVERT_ROUGHNESS", "1"});
  }
  // Add more macros based on other properties as needed

  matPass->setVShaderInfo("resources/shaders/GBufferShader.hlsl", "main", "vs_5_0", macros);
  matPass->setPShaderInfo("resources/shaders/GBufferShader.hlsl", "mainPS", "ps_5_0", macros);
  matPass->compileShader();
  matPass->generateInputLayout();

  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = bWireframe ? FILL_MODE::kWireframe : FILL_MODE::kSolid;
  // TODO : Implement double and single sided materials
  //rasterDesc.cullMode = bDoubleSided ? CULL_MODE::kNone : CULL_MODE::kFront;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;
  matPass->setRasterizerStateFromDesc(rasterDesc);

  BlendDesc blendDesc = {};
  if (bAlphaTest) {
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
  }
  else {
    blendDesc.renderTarget[0].blendEnable = true;
    blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
    blendDesc.renderTarget[0].destBlend = BLEND::kZero;
    blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
    blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
    blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;
  }
  matPass->setBlendStateFromDesc(blendDesc);

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
  matPass->setDepthStencilStateFromDesc(depthSDesc);

  m_passes[shaderID] = matPass;

  return m_passes[shaderID];
}

void
ShaderManager::recompileShaders()
{
  for (auto& pass : m_passes) {
    pass.second->compileShader();
    //pass.second->generateInputLayout();
  }
}

void
ShaderManager::updateMainCB()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pMainBuffer, &m_mainBufferData, sizeof(MainBufferData));
}

void
ShaderManager::updateShaderDataCB()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pShaderDataBuffer, &m_shaderData, sizeof(ShaderData));
}

void
ShaderManager::updatePrefilterShaderCB()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pPrefilteredCB, &m_prefilteredData, sizeof(PrefilteredCB));
}

void
ShaderManager::updateLightCB()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pLightBuffer, &m_lightData, sizeof(LightCB));
}

ShaderManager&
g_shaderMan()
{
  return ShaderManager::instance();
}
}
