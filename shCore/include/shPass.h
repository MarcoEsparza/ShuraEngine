/*****************************************************************************/
/*
*  @file    shPass.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/07
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
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/

class ProgramShader;
class InputLayout;
class RasterizerState;
class BlendState;
class DepthStencilState;
class SamplerState;
class ConstantBuffer;

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

  /**
  *  @brief Sets the info for the shader compile.
  *
  *  @param String& shaderPath
  *  @param String& vsEntry
  *  @param String& psEntry
  *  @param String& vsModel
  *  @param String& psModel
  */
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

  /**
  *  @brief Add a constant buffer to the Vertex Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  */
  void
  addVSConstantBuffer(const SPtr<ConstantBuffer>& buffer);

  /**
  *  @brief Add a constant buffer to the Pixel Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  */
  void
  addPSConstantBuffer(const SPtr<ConstantBuffer>& buffer);

  /**
  *  @brief Compiles the shader.
  *
  *  @param DepthStencilDesc& dsDesc
  */
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
  
  /**
  *  @brief Vertex Shader Constant Buffer container.
  */
  Vector<SPtr<ConstantBuffer>> m_vsCBuffers;

  /**
  *  @brief Pixel Shader Constant Buffer container.
  */
  Vector<SPtr<ConstantBuffer>> m_psCBuffers;

  /**
  *  @brief Path of the shader file.
  */
  String m_shaderPath;

  /**
  *  @brief Vertex Shader entry point.
  */
  String m_vsEntryPoint;

  /**
  *  @brief Pixel Shader entry point.
  */
  String m_psEntryPoint;

  /**
  *  @brief Vertex Shader model.
  */
  String m_vsShaderModel;

  /**
  *  @brief Pixel Shader model.
  */
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
