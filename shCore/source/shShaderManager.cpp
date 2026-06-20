/*****************************************************************************/
/*
*  @file    shShaderManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
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
#include "shFileExplorer.h"
#include "shPass.h"
//#include "shMaterial.h"
#include "shBuffers.h"
#include "shRasterizerState.h"
#include "shBlendState.h"
#include "shDepthStencilState.h"
#include "shStringID.h"
#include <shPath.h>

namespace shEngineSDK {
const uint32 ShaderManager::SSAO_SHADER_ID = StringID("AOShader").getID();
const uint32 ShaderManager::HBLUR_SHADER_ID = StringID("HBlurShader").getID();
const uint32 ShaderManager::VBLUR_SHADER_ID = StringID("VBlurShader").getID();
const uint32 ShaderManager::LIGHT_CS_ID = StringID("LightCS").getID();
const uint32 ShaderManager::SHADOWMAP_SHADER_ID = StringID("ShadowMapShader").getID();
const uint32 ShaderManager::SKYBOX_SHADER_ID = StringID("SkyBoxShader").getID();
const uint32 ShaderManager::FINAL_SHADER_ID = StringID("FinalShader").getID();
const uint32 ShaderManager::PLANE_SHADER_ID = StringID("PlaneShader").getID();
//const uint32 ShaderManager::HISTOGRAM_SHADER_ID = StringID("HistogramShader").getID();
const uint32 ShaderManager::ADDSKYBOX_SHADER_ID = StringID("AddSkyboxShader").getID();
const uint32 ShaderManager::LUMINANCE_SHADER_ID = StringID("LuminanceShader").getID();
const uint32 ShaderManager::BRIGHT_SHADER_ID = StringID("BrightShader").getID();
const uint32 ShaderManager::TONEMAP_SHADER_ID = StringID("ToneMapShader").getID();
const uint32 ShaderManager::POSTPROCESS_SHADER_ID = StringID("PostProcessShader").getID();
const uint32 ShaderManager::ADDITIVE_SHADER_ID = StringID("AdditiveShader").getID();
const uint32 ShaderManager::CUBEMAP_SHADER_ID = StringID("CubeMapShader").getID();
const uint32 ShaderManager::DIFFUSE_IRR_SHADER_ID = StringID("DiffuseIrrShader").getID();
const uint32 ShaderManager::PREFILTERED_IRR_SHADER_ID =
                            StringID("PrefilteredIrrShader").getID();
const uint32 ShaderManager::BRDF_SHADER_ID = StringID("BRDFShader").getID();
const uint32 ShaderManager::EMMISIVE_SHADER_ID = StringID("EmmisiveShader").getID();

ShaderManager::~ShaderManager() {
  m_passes.clear();
  m_pMainBuffer.reset();
  m_pShaderDataBuffer.reset();
  m_pPrefilteredCB.reset();
  m_pLightBuffer.reset();
  m_pLCBuffer.reset();
  m_pModelTransformBuffer.reset();
  m_pPBRData.reset();
}

void
ShaderManager::onStartUp() {
  GraphicsManager& graphMan = g_graphicsMan();
  if (graphMan.getAPI() == GRAPHIC_API::kDX11) {
    m_shaderDirectory = FileExplorer::SHADERS_FOLDER + "DX11/";
  }
  else if (graphMan.getAPI() == GRAPHIC_API::kOGL) {
    m_shaderDirectory = FileExplorer::SHADERS_FOLDER + "OpenGL/";
  }
}

void
ShaderManager::createPipelinePasses()
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Lightning
  auto pLightCS = sh_makeShared<Pass>();
  pLightCS->setCShaderInfo(Path(m_shaderDirectory + "LightCShader"), "CSMain");
  pLightCS->compileShader();

  // AO
  auto pAOShader = sh_makeShared<Pass>();
  pAOShader->setPShaderInfo(Path(m_shaderDirectory + "AOShader"), "mainPS");
  pAOShader->compileShader();

  // HBlur
  auto pHBlurCS = sh_makeShared<Pass>();
  pHBlurCS->setCShaderInfo(Path(m_shaderDirectory + "PostProcessShader"), "HBlurCS");
  pHBlurCS->compileShader();

  // VBlur
  auto pVBlurCS = sh_makeShared<Pass>();
  pVBlurCS->setCShaderInfo(Path(m_shaderDirectory + "PostProcessShader"), "VBlurCS");
  pVBlurCS->compileShader();

  // Shadow map
  auto pSMapShader = sh_makeShared<Pass>();
  pSMapShader->setVShaderInfo(Path(m_shaderDirectory + "SMapShader"), "main");
  pSMapShader->setPShaderInfo(Path(m_shaderDirectory + "SMapShader"), "mainPS");
  pSMapShader->compileShader();

  // Skybox
  auto pSkyBoxShader = sh_makeShared<Pass>();
  pSkyBoxShader->setCShaderInfo(Path(m_shaderDirectory + "SkyBoxShader"), "CSMain");
  pSkyBoxShader->compileShader();

  // Final shader
  auto pFinalShader = sh_makeShared<Pass>();
  pFinalShader->setPShaderInfo(Path(m_shaderDirectory + "FinalShader"), "mainPS");
  pFinalShader->compileShader();

  // Plane Vertex shader
  auto pPlaneVS = sh_makeShared<Pass>();
  pPlaneVS->setVShaderInfo(Path(m_shaderDirectory + "PlaneVertexShader"), "main");
  pPlaneVS->compileShader();

  // Histogram shader
  /*auto pHistogramShader = sh_makeShared<Pass>();
  pHistogramShader->setCShaderInfo(m_shaderDirectory + "HistogramShader", "CSMain");
  pHistogramShader->compileShader();*/

  // Add skybox shader
  auto pASBShader = sh_makeShared<Pass>();
  pASBShader->setCShaderInfo(Path(m_shaderDirectory + "AddSkyboxShader"), "CSMain");
  pASBShader->compileShader();

  // Luminance shader
  auto pLuminanceShader = sh_makeShared<Pass>();
  pLuminanceShader->setCShaderInfo(Path(m_shaderDirectory + "LuminanceShader"),
                                   "LuminanceCS");
  pLuminanceShader->compileShader();

  // Bright shader
  auto pBrightShader = sh_makeShared<Pass>();
  pBrightShader->setCShaderInfo(Path(m_shaderDirectory + "LuminanceShader"), "BrightCS");
  pBrightShader->compileShader();

  // ToneMap shader
  auto pToneMapShader = sh_makeShared<Pass>();
  pToneMapShader->setCShaderInfo(Path(m_shaderDirectory + "PostProcessShader"), "ToneMapCS");
  pToneMapShader->compileShader();

  // AddMix shader
  auto pAddMixShader = sh_makeShared<Pass>();
  pAddMixShader->setCShaderInfo(Path(m_shaderDirectory + "LuminanceShader"), "AddMixCS");
  pAddMixShader->compileShader();

  // PostProcess shader
  auto pPPShader = sh_makeShared<Pass>();
  pPPShader->setCShaderInfo(Path(m_shaderDirectory + "PostProcessShader"), "PostProcessCS");
  pPPShader->compileShader();

  // Shader to calculate cube maps
  auto pCubeMapShader = sh_makeShared<Pass>();
  pCubeMapShader->setCShaderInfo(Path(m_shaderDirectory + "CubeMapShader"), "CSMain");
  pCubeMapShader->compileShader();

  // Irradiance cube shader
  auto pIrrCubeShader = sh_makeShared<Pass>();
  pIrrCubeShader->setCShaderInfo(Path(m_shaderDirectory + "IrradianceCubeShader"), "CSMain");
  pIrrCubeShader->compileShader();

  // Specular Prefiltered Cubemap shader
  auto pSPreCubeMap = sh_makeShared<Pass>();
  pSPreCubeMap->setCShaderInfo(Path(m_shaderDirectory + "SpecularPreMapShader"), "CSMain");
  pSPreCubeMap->compileShader();

  // BRDF LUT shader
  auto pBRDFShader = sh_makeShared<Pass>();
  pBRDFShader->setCShaderInfo(Path(m_shaderDirectory + "BRDFShader"), "CSMain");
  pBRDFShader->compileShader();

  // Emmisive pass
  auto pEmmisiveShader = sh_makeShared<Pass>();
  pEmmisiveShader->setCShaderInfo(Path(m_shaderDirectory + "PostProcessShader"),
                                  "EmmisiveCS");
  pEmmisiveShader->compileShader();

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
  m_pModelTransformBuffer = graphMan.createConstantBuffer(sizeof(Matrix4));
  m_pPBRData = graphMan.createConstantBuffer(sizeof(PBRMaterialData));

  // Shadow shader buffers
  pSMapShader->addVSConstantBuffer(m_pMainBuffer, 0);
  pSMapShader->addVSConstantBuffer(m_pShaderDataBuffer, 1);
  pSMapShader->addVSConstantBuffer(m_pModelTransformBuffer, 2);
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
  pHBlurCS->addCSConstantBuffer(m_pMainBuffer, 0);
  pHBlurCS->addCSConstantBuffer(m_pShaderDataBuffer, 1);
  pVBlurCS->addCSConstantBuffer(m_pMainBuffer, 0);
  pVBlurCS->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Specular prefiltered cubemap
  pSPreCubeMap->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  // Final shader buffers
  pFinalShader->addPSConstantBuffer(m_pMainBuffer, 0);

  // Histogram
  //pHistogramShader->addCSConstantBuffer(m_pMainBuffer, 0);

  // Add skybox
  pASBShader->addCSConstantBuffer(m_pMainBuffer, 0);

  pIrrCubeShader->addCSConstantBuffer(m_pPrefilteredCB, 2);
  pSPreCubeMap->addCSConstantBuffer(m_pPrefilteredCB, 2);

  // Emmisive pass
  pEmmisiveShader->addCSConstantBuffer(m_pMainBuffer, 0);
  pEmmisiveShader->addCSConstantBuffer(m_pShaderDataBuffer, 1);

  m_passes[SSAO_SHADER_ID] = pAOShader;
  m_passes[HBLUR_SHADER_ID] = pHBlurCS;
  m_passes[VBLUR_SHADER_ID] = pVBlurCS;
  m_passes[LIGHT_CS_ID] = pLightCS;
  m_passes[SHADOWMAP_SHADER_ID] = pSMapShader;
  m_passes[SKYBOX_SHADER_ID] = pSkyBoxShader;
  m_passes[FINAL_SHADER_ID] = pFinalShader;
  m_passes[PLANE_SHADER_ID] = pPlaneVS;
  //m_passes[HISTOGRAM_SHADER_ID] = pHistogramShader;
  m_passes[ADDSKYBOX_SHADER_ID] = pASBShader;
  m_passes[LUMINANCE_SHADER_ID] = pLuminanceShader;
  m_passes[BRIGHT_SHADER_ID] = pBrightShader;
  m_passes[TONEMAP_SHADER_ID] = pToneMapShader;
  m_passes[POSTPROCESS_SHADER_ID] = pPPShader;
  m_passes[ADDITIVE_SHADER_ID] = pAddMixShader;
  m_passes[CUBEMAP_SHADER_ID] = pCubeMapShader;
  m_passes[DIFFUSE_IRR_SHADER_ID] = pIrrCubeShader;
  m_passes[PREFILTERED_IRR_SHADER_ID] = pSPreCubeMap;
  m_passes[BRDF_SHADER_ID] = pBRDFShader;
  m_passes[EMMISIVE_SHADER_ID] = pEmmisiveShader;
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
  matPass->addVSConstantBuffer(m_pMainBuffer, 0);
  matPass->addVSConstantBuffer(m_pShaderDataBuffer, 1);
  matPass->addVSConstantBuffer(m_pModelTransformBuffer, 2);
  matPass->addPSConstantBuffer(m_pPBRData, 3);

  bool bWireframe = props.properties.flags.bWireframeEnabled;
  //bool bDoubleSided = props.properties.flags.bIsDoubleSided;
  bool bAlphaTest = props.properties.flags.bHasAlphaTest;
  bool bAlphaBlend = props.properties.flags.bHasAlphaBlend;

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
  if (props.properties.flags.bInvertNormals) {
    macros.push_back({ "INVERT_NORMALS", "1" });
  }
  if(props.properties.flags.bInvertRoughness) {
    macros.push_back({"INVERT_ROUGHNESS", "1"});
  }
  if(props.properties.flags.bInvertMetalness) {
    macros.push_back({"INVERT_METALNESS", "1"});
  }
  if(props.properties.flags.bHasOpacityMask) {
    macros.push_back({"HAS_OPACITY_MASK", "1"});
  }
  if(props.properties.flags.bUseEmission) {
    macros.push_back({"USE_EMISSION", "1"});
  }
  // Add more macros based on other properties as needed

  if (!bAlphaBlend) {
    matPass->setVShaderInfo(Path(m_shaderDirectory + "GBufferShader"), "main", macros);
    matPass->setPShaderInfo(Path(m_shaderDirectory + "GBufferShader"), "mainPS", macros);
    matPass->compileShader();
    matPass->generateInputLayout();
  }
  else {
    matPass->addPSConstantBuffer(m_pMainBuffer, 0);
    matPass->addPSConstantBuffer(m_pShaderDataBuffer, 1);
    matPass->addPSConstantBuffer(m_pLCBuffer, 4);
    matPass->addPSConstantBuffer(m_pPrefilteredCB, 5);

    matPass->setVShaderInfo(Path(m_shaderDirectory + "TransparentFowardShader"), "main", macros);
    matPass->setPShaderInfo(Path(m_shaderDirectory + "TransparentFowardShader"), "mainPS", macros);
    matPass->compileShader();
    matPass->generateInputLayout();
  }

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
  DepthStencilDesc depthSDesc = {};
  if (bAlphaTest && !bAlphaBlend) {
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

    depthSDesc.depthEnable = true;
    depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kZero;
    depthSDesc.depthFunc = COMPARISON_FUNC::kLess;
    depthSDesc.stencilEnable = false;
  }
  else if (!bAlphaTest && bAlphaBlend) {
    blendDesc.alphaToCoverageEnable = false;
    blendDesc.independentBlendEnable = false;
    blendDesc.renderTarget[0].blendEnable = true;
    blendDesc.renderTarget[0].srcBlend = BLEND::kSrcAlpha;
    blendDesc.renderTarget[0].destBlend = BLEND::kInvSrcAlpha;
    blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
    blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
    blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

    depthSDesc.depthEnable = true;
    depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kZero;
    depthSDesc.depthFunc = COMPARISON_FUNC::kLess;
    depthSDesc.stencilEnable = false;
  }
  else if (!bAlphaTest && !bAlphaBlend) {
    blendDesc.renderTarget[0].blendEnable = true;
    blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
    blendDesc.renderTarget[0].destBlend = BLEND::kZero;
    blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
    blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
    blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
    blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

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
  }
  matPass->setBlendStateFromDesc(blendDesc);
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

void
ShaderManager::updateMaterialCB()
{
  GraphicsManager& graphMan = g_graphicsMan();
  graphMan.updateConstantBuffer(m_pPBRData, &m_materialData, sizeof(PBRMaterialData));
}

String
ShaderManager::getVSShaderModel() const
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (graphMan.getAPI() == GRAPHIC_API::kDX11) {
    return "vs_5_0";
  }
  else if (graphMan.getAPI() == GRAPHIC_API::kOGL) {
    return "330";
  }
  return "";
}

String
ShaderManager::getPSShaderModel() const
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (graphMan.getAPI() == GRAPHIC_API::kDX11) {
    return "ps_5_0";
  }
  else if (graphMan.getAPI() == GRAPHIC_API::kOGL) {
    return "330";
  }
  return "";
}

String
ShaderManager::getGSShaderModel() const
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (graphMan.getAPI() == GRAPHIC_API::kDX11) {
    return "gs_5_0";
  }
  else if (graphMan.getAPI() == GRAPHIC_API::kOGL) {
    return "330";
  }
  return "";
}

String
ShaderManager::getCSShaderModel() const
{
  GraphicsManager& graphMan = g_graphicsMan();
  if (graphMan.getAPI() == GRAPHIC_API::kDX11) {
    return "cs_5_0";
  }
  else if (graphMan.getAPI() == GRAPHIC_API::kOGL) {
    return "330";
  }
  return "";
}

ShaderManager&
g_shaderMan()
{
  return ShaderManager::instance();
}
}
