/*****************************************************************************/
/*
*  @file    shPass.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
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

class VertexShader;
class PixelShader;
class GeometryShader;
class ComputeShader;
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
  FORCEINLINE Pass() = default;

  /**
  *  @brief Default destructor.
  */
  ~Pass();

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
  setInputLayout(const SPtr<InputLayout>& pInputLayout);

  /**
  *  @brief Sets the sampler state.
  *
  *  @param SPtr<SamplerState>& pSamplerLinear
  */
  FORCEINLINE void
  setSamplerState(const SPtr<SamplerState>& pSamplerLinear);

  /**
  *  @brief Sets the rasterizer state.
  *
  *  @param SPtr<RasterizerState>& pRaster
  */
  FORCEINLINE void
  setRasterizerState(const SPtr<RasterizerState>& pRaster);

  /**
  *  @brief Sets the blend state.
  *
  *  @param SPtr<BlendState>& pBlend
  */
  FORCEINLINE void
  setBlendState(const SPtr<BlendState>& pBlend);

  /**
  *  @brief Sets the depth stencil state.
  *
  *  @param SPtr<DepthStencilState>& pDepthS
  */
  FORCEINLINE void
  setDepthStencilState(const SPtr<DepthStencilState>& pDepthS);

  /**
  *  @brief Sets the info for the vertex shader compile.
  *
  *  @param String& shaderPath
  *  @param String& entry
  *  @param String& model
  *  @param Vector<ShaderMacro>& macros = {}
  */
  void
  setVShaderInfo(const String& shaderPath,
                 const String& entry,
                 const String& model,
                 const Vector<ShaderMacro>& macros = {});

  /**
  *  @brief Sets the info for the pixel shader compile.
  *
  *  @param String& shaderPath
  *  @param String& entry
  *  @param String& model
  *  @param Vector<ShaderMacro>& macros = {}
  */
  void
  setPShaderInfo(const String& shaderPath,
                 const String& entry,
                 const String& model,
                 const Vector<ShaderMacro>& macros = {});

  /**
  *  @brief Sets the info for the geometry shader compile.
  *
  *  @param String& shaderPath
  *  @param String& entry
  *  @param String& model
  *  @param Vector<ShaderMacro>& macros = {}
  */
  void
  setGShaderInfo(const String& shaderPath,
                 const String& entry,
                 const String& model,
                 const Vector<ShaderMacro>& macros = {});

  /**
  *  @brief Sets the info for the compute shader compile.
  *
  *  @param String& shaderPath
  *  @param String& entry
  *  @param String& model
  *  @param Vector<ShaderMacro>& macros = {}
  */
  void
  setCShaderInfo(const String& shaderPath,
                 const String& entry,
                 const String& model,
                 const Vector<ShaderMacro>& macros = {});

  /**
  *  @brief Sets the raster state from a descriptor.
  *
  *  @param RasterizerDesc& rasterDesc
  */
  void
  setRasterizerStateFromDesc(const RasterizerDesc& rasterDesc);

  /**
  *  @brief Sets the blend state from a descriptor.
  *
  *  @param BlendDesc& blendDesc
  */
  void
  setBlendStateFromDesc(const BlendDesc& blendDesc);

  /**
  *  @brief Sets the depth stencil state from a descriptor.
  *
  *  @param DepthStencilDesc& dsDesc
  */
  void
  setDepthStencilStateFromDesc(const DepthStencilDesc& dsDesc);

  /**
  *  @brief Gets the Vertex shader.
  *
  *  @return SPtr<VertexShader>
  */
  FORCEINLINE SPtr<VertexShader>
  getVertexShader() const;

  /**
  *  @brief Gets the Pixel shader.
  *
  *  @return SPtr<PixelShader>
  */
  FORCEINLINE SPtr<PixelShader>
  getPixelShader() const;

  /**
  *  @brief Gets the Geometry shader.
  *
  *  @return SPtr<GeometryShader>
  */
  FORCEINLINE SPtr<GeometryShader>
  getGeometryShader() const;

  /**
  *  @brief Gets the Compute shader.
  *
  *  @return SPtr<ComputeShader>
  */
  FORCEINLINE SPtr<ComputeShader>
  getComputeShader() const;

  /**
  *  @brief Add a constant buffer to the Vertex Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  *  @param uint32 slot
  */
  void
  addVSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot);

  /**
  *  @brief Add a constant buffer to the Pixel Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  */
  void
  addPSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot);

  /**
  *  @brief Add a constant buffer to the Pixel Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  */
  void
  addGSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot);

  /**
  *  @brief Add a constant buffer to the Pixel Shader Constant Buffer container.
  *
  *  @param SPtr<ConstantBuffer>& buffer
  */
  void
  addCSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot);

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
  *  @brief Vertex Shader pointer.
  */
  SPtr<VertexShader> m_pVShader;

  /**
  *  @brief Pixel Shader pointer.
  */
  SPtr<PixelShader> m_pPShader;

  /**
  *  @brief Geometry Shader pointer.
  */
  SPtr<GeometryShader> m_pGShader;

  /**
  *  @brief Compute Shader pointer.
  */
  SPtr<ComputeShader> m_pCShader;

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
  Vector<Pair<WPtr<ConstantBuffer>, uint32>> m_vsCBuffers;

  /**
  *  @brief Pixel Shader Constant Buffer container.
  */
  Vector<Pair<WPtr<ConstantBuffer>, uint32>> m_psCBuffers;

  /**
  *  @brief Geometry Shader Constant Buffer container.
  */
  Vector<Pair<WPtr<ConstantBuffer>, uint32>> m_gsCBuffers;

  /**
  *  @brief Compute Shader Constant Buffer container.
  */
  Vector<Pair<WPtr<ConstantBuffer>, uint32>> m_csCBuffers;

  /**
  *  @brief Path of the vertex shader file.
  */
  String m_vsPath = "";

  /**
  *  @brief Path of the pixel shader file.
  */
  String m_psPath = "";

  /**
  *  @brief Path of the geometry shader file.
  */
  String m_gsPath = "";

  /**
  *  @brief Path of the compute shader file.
  */
  String m_csPath = "";

  /**
  *  @brief Vertex Shader entry point.
  */
  String m_vsEntryPoint;

  /**
  *  @brief Pixel Shader entry point.
  */
  String m_psEntryPoint;

  /**
  *  @brief Geometry Shader entry point.
  */
  String m_gsEntryPoint;

  /**
  *  @brief Compute Shader entry point.
  */
  String m_csEntryPoint;

  /**
  *  @brief Vertex Shader model.
  */
  String m_vsModel;

  /**
  *  @brief Pixel Shader model.
  */
  String m_psModel;

  /**
  *  @brief Geometry Shader model.
  */
  String m_gsModel;

  /**
  *  @brief Compute Shader model.
  */
  String m_csModel;

 public:
  /**
  *  @brief Vertex Shader macros.
  */
  Vector<ShaderMacro> m_macros;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE void
Pass::setInputLayout(const SPtr<InputLayout>& pInputLayout)
{
  if (pInputLayout) {
    m_pInputLayout = pInputLayout;
  }
}

FORCEINLINE void
Pass::setSamplerState(const SPtr<SamplerState>& pSamplerLinear)
{
  if (pSamplerLinear) {
    m_pSamplerState = pSamplerLinear;
  }
}

FORCEINLINE void
Pass::setRasterizerState(const SPtr<RasterizerState>& pRaster)
{
  m_pRasterState = pRaster;
}

FORCEINLINE void
Pass::setBlendState(const SPtr<BlendState>& pBlend)
{
  m_pBlendState = pBlend;
}

FORCEINLINE void
Pass::setDepthStencilState(const SPtr<DepthStencilState>& pDepthS)
{
  m_pDsState = pDepthS;
}

FORCEINLINE SPtr<VertexShader>
Pass::getVertexShader() const
{
  return m_pVShader;
}

FORCEINLINE SPtr<PixelShader>
Pass::getPixelShader() const
{
  return m_pPShader;
}

FORCEINLINE SPtr<GeometryShader>
Pass::getGeometryShader() const
{
  return m_pGShader;
}

FORCEINLINE SPtr<ComputeShader>
Pass::getComputeShader() const
{
  return m_pCShader;
}
}
