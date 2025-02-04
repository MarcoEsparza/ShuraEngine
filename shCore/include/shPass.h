/*****************************************************************************/
/*
*  @file    shPass.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/03
*  @brief   Pass for renderer.
*
*  Pass for renderer.
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
#include "shGraphicTypes.h"

namespace shEngineSDK {
class ProgramShader;
class InputLayout;
class RasterizerState;
class BlendState;
class DepthStencilState;
class SamplerState;

/**
*  @brief Pass for renderer.
*/
class SH_CORE_EXPORT Pass
{
 public:
  /**
  *  @brief Default constructor.
  */
  Pass() = default;

  /**
  *  @brief Default destructor.
  */
  ~Pass() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Sets the input layout.
  * 
  *  @param SPtr<InputLayout>& pInputLayout
  */
  FORCEINLINE void
  setInputLayout(SPtr<InputLayout>& pInputLayout);

  /**
  *  @brief Sets the sampler state.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  */
  FORCEINLINE void
  setSamplerState(SPtr<SamplerState>& pSamplerLinear);

  void
  setShaderInfo(const String& shaderPath,
                const String& vsEntry,
                const String& psEntry,
                const String& vsModel,
                const String& psModel);

  /**
  *  @brief Sets the raster state.
  *
  *  @param RasterizerDesc& rasterDesc
  */
  void
  setRasterizerState(const RasterizerDesc& rasterDesc);

  /**
  *  @brief Sets the blend state.
  *
  *  @param BlendDesc& blendDesc
  */
  void
  setBlendState(const BlendDesc& blendDesc);

  /**
  *  @brief Sets the depth stencil state.
  *
  *  @param DepthStencilDesc& dsDesc
  */
  void
  setDepthStencilState(const DepthStencilDesc& dsDesc);

  void
  compileShader();

  /**
  *  @brief Creates the input layout from vertex shader.
  */
  void
  generateInputLayout();

  /**
  *  @brief Sets the pass to the render pipeline.
  */
  void
  setPass() const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Shader.
  */
  SPtr<ProgramShader> m_pShader;

  /**
  *  @brief Input layout.
  */
  SPtr<InputLayout> m_pInputLayout;
  
  /**
  *  @brief Sampler state.
  */
  SPtr<SamplerState> m_pSamplerState;

  /**
  *  @brief Rasterizer state.
  */
  SPtr<RasterizerState> m_pRasterState;

  /**
  *  @brief Blend state.
  */
  SPtr<BlendState> m_pBlendState;
  
  /**
  *  @brief Depth Stencil state.
  */
  SPtr<DepthStencilState> m_pDsState;

  String m_shaderPath;
  String m_vsEntryPoint;
  String m_psEntryPoint;
  String m_vsShaderModel;
  String m_psShaderModel;

  /**
  *  @brief Descriptor for rasterizer state.
  */
  RasterizerDesc m_rasterDesc = {};

  /**
  *  @brief Descriptor for blend state.
  */
  BlendDesc m_blendDesc = {};

  /**
  *  @brief Descriptor for depth stencil state.
  */
  DepthStencilDesc m_dsDesc = {};
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE void
Pass::setInputLayout(SPtr<InputLayout>& pInputLayout)
{
  if (pInputLayout) {
    m_pInputLayout = pInputLayout;
  }
}

FORCEINLINE void
Pass::setSamplerState(SPtr<SamplerState>& pSamplerLinear)
{
  if (pSamplerLinear) {
    m_pSamplerState = pSamplerLinear;
  }
}
}
