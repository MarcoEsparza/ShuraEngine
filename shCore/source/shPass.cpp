/*****************************************************************************/
/*
*  @file    shPass.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Pass for renderer.
*
*  Pass for renderer.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include  "shPass.h"
#include "shGraphicsManager.h"
#include "shShaderManager.h"
#include "shShader.h"
#include "shBlendState.h"
#include <shLogger.h>

namespace shEngineSDK {
Pass::~Pass() {
  m_pVShader.reset();
  m_pPShader.reset();
  m_pGShader.reset();
  m_pCShader.reset();
  m_pInputLayout.reset();
  m_pRasterState.reset();
  m_pBlendState.reset();
  m_pDsState.reset();
  m_vsCBuffers.clear();
  m_psCBuffers.clear();
  m_gsCBuffers.clear();
  m_csCBuffers.clear();
}

void
Pass::setVShaderInfo(const String& shaderPath,
                     const String& entry,
                     const Vector<ShaderMacro>& macros)
{
  m_vsPath = shaderPath;
  m_vsEntryPoint = entry;
  m_macros = macros;
}

void
Pass::setPShaderInfo(const String& shaderPath,
                     const String& entry,
                     const Vector<ShaderMacro>& macros)
{
  m_psPath = shaderPath;
  m_psEntryPoint = entry;
  m_macros = macros;
}

void
Pass::setGShaderInfo(const String& shaderPath,
                     const String& entry,
                     const Vector<ShaderMacro>& macros)
{
  m_gsPath = shaderPath;
  m_gsEntryPoint = entry;
  m_macros = macros;
}

void
Pass::setCShaderInfo(const String& shaderPath,
                     const String& entry,
                     const Vector<ShaderMacro>& macros)
{
  m_csPath = shaderPath;
  m_csEntryPoint = entry;
  m_macros = macros;
}

void
Pass::setRasterizerStateFromDesc(const RasterizerDesc& rasterDesc)
{
  m_pRasterState = g_graphicsMan().createRasterizerState(rasterDesc);
}

void
Pass::setBlendStateFromDesc(const BlendDesc& blendDesc)
{
  m_pBlendState = g_graphicsMan().createBlendState(blendDesc);
  if (m_vsPath != "") {
    String name = m_vsPath + "_BlendState";
    m_pBlendState->setDebugName(name);
  }
}

void
Pass::setDepthStencilStateFromDesc(const DepthStencilDesc& dsDesc)
{
  m_pDsState = g_graphicsMan().createDepthStencilState(dsDesc);
}

void
Pass::addVSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot)
{
  m_vsCBuffers.push_back({ buffer, slot });
}

void
Pass::addPSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot)
{
  m_psCBuffers.push_back({ buffer, slot });
}

void
Pass::addGSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot)
{
  m_gsCBuffers.push_back({ buffer, slot });
}

void
Pass::addCSConstantBuffer(const SPtr<ConstantBuffer>& buffer, const uint32 slot)
{
  m_csCBuffers.push_back({ buffer, slot });
}

void
Pass::compileShader()
{
  GraphicsManager& graphMan = g_graphicsMan();
  ShaderManager& shaderMan = g_shaderMan();

  // Reset pointer if there's already a Vertex Shader in it.
  if (m_pVShader) {
    m_pVShader.reset();
  }
  // It will not compile if there's no path for shader
  if (m_vsPath != "") {
    m_pVShader = graphMan.createVertexShader(m_vsPath,
                                             m_vsEntryPoint,
                                             shaderMan.getVSShaderModel(),
                                             m_macros);
  }

  // Reset pointer if there's already a Pixel Shader in it.
  if (m_pPShader) {
    m_pPShader.reset();
  }
  // It will not compile if there's no path for shader
  if (m_psPath != "") {
    m_pPShader = graphMan.createPixelShader(m_psPath,
                                            m_psEntryPoint,
                                            shaderMan.getPSShaderModel(),
                                            m_macros);
  }

  // Reset pointer if there's already a Geometry Shader in it.
  if (m_pGShader) {
    m_pGShader.reset();
  }
  // It will not compile if there's no path for shader
  if (m_gsPath != "") {
    m_pGShader = graphMan.createGeometryShader(m_gsPath,
                                               m_gsEntryPoint,
                                               shaderMan.getGSShaderModel(),
                                               m_macros);
  }

  // Reset pointer if there's already a Compute Shader in it.
  if (m_pCShader) {
    m_pCShader.reset();
  }
  // It will not compile if there's no path for shader
  if (m_csPath != "") {
    m_pCShader = graphMan.createComputeShader(m_csPath,
                                              m_csEntryPoint,
                                              shaderMan.getCSShaderModel(),
                                              m_macros);
  }
}

void
Pass::generateInputLayout()
{
  if (!m_pVShader) {
    return;
  }

  m_pInputLayout = g_graphicsMan().createInputLayoutFromShader(m_pVShader);
}

void
Pass::setPass() const
{
  GraphicsManager& graphMan = g_graphicsMan();

  // Set shaders
  if (m_pVShader) {
    graphMan.setVertexShader(m_pVShader);
  }
  if (m_pPShader) {
    graphMan.setPixelShader(m_pPShader);
  }
  if (m_pGShader) {
    graphMan.setGeometryShader(m_pGShader);
  }
  if (m_pCShader) {
    graphMan.setComputeShader(m_pCShader);
  }

  // Set input layout
  if (m_pInputLayout) {
    graphMan.setInputLayout(m_pInputLayout);
  }

  // Set states
  if (m_pSamplerState) {
    graphMan.psSetSamplerState(m_pSamplerState);
    graphMan.csSetSamplerState(m_pSamplerState);
  }
  if (m_pRasterState) {
    graphMan.setRasterizerState(m_pRasterState);
  }
  if (m_pBlendState) {
    graphMan.setBlendState(m_pBlendState);
  }
  if (m_pDsState) {
    graphMan.setDepthStencilState(m_pDsState);
  }

  // Set constant buffers
  for (auto& pVSCBufferPair : m_vsCBuffers) {
    if (!pVSCBufferPair.first.expired()) {
      graphMan.vsSetConstantBuffers(pVSCBufferPair.first, pVSCBufferPair.second);
    }
  }
  for (auto& pPSCBufferPair : m_psCBuffers) {
    if (!pPSCBufferPair.first.expired()) {
      graphMan.psSetConstantBuffers(pPSCBufferPair.first, pPSCBufferPair.second);
    }
  }
  for (auto& pGSCBufferPair : m_gsCBuffers) {
    if (!pGSCBufferPair.first.expired()) {
      graphMan.gsSetConstantBuffers(pGSCBufferPair.first, pGSCBufferPair.second);
    }
  }
  for (auto& pCSCBufferPair : m_csCBuffers) {
    if (!pCSCBufferPair.first.expired()) {
      graphMan.csSetConstantBuffers(pCSCBufferPair.first, pCSCBufferPair.second);
    }
  }
}
}
