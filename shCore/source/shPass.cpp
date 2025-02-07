/*****************************************************************************/
/*
*  @file    shPass.cpp
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
#include  "shPass.h"
#include "shGraphicsManager.h"
#include "shShader.h"

namespace shEngineSDK {
void
Pass::setShaderInfo(const String& shaderPath,
                    const String& vsEntry,
                    const String& psEntry,
                    const String& vsModel,
                    const String& psModel)
{
  m_shaderPath = shaderPath;
  m_vsEntryPoint = vsEntry;
  m_psEntryPoint = psEntry;
  m_vsShaderModel = vsModel;
  m_psShaderModel = psModel;
}

void
Pass::setRasterizerState(const RasterizerDesc& rasterDesc)
{
  m_rasterDesc = rasterDesc;
  m_pRasterState = g_graphicsMan().createRasterizerState(m_rasterDesc);
}

void
Pass::setBlendState(const BlendDesc& blendDesc)
{
  m_blendDesc = blendDesc;
  m_pBlendState = g_graphicsMan().createBlendState(m_blendDesc);
}

void
Pass::setDepthStencilState(const DepthStencilDesc& dsDesc)
{
  m_dsDesc = dsDesc;
  m_pDsState = g_graphicsMan().createDepthStencilState(m_dsDesc);
}

void
Pass::addVSConstantBuffer(const SPtr<ConstantBuffer>& buffer)
{
  m_vsCBuffers.push_back(buffer);
}

void
Pass::addPSConstantBuffer(const SPtr<ConstantBuffer>& buffer)
{
  m_psCBuffers.push_back(buffer);
}

void
Pass::compileShader()
{
  if (m_pShader) {
    m_pShader->~ProgramShader();
    m_pShader.reset();
  }

  m_pShader = g_graphicsMan().createProgramShader(m_shaderPath,
                                                  m_vsEntryPoint,
                                                  m_psEntryPoint,
                                                  m_vsShaderModel,
                                                  m_psShaderModel);
}

void
Pass::generateInputLayout()
{
  if (!m_pShader) {
    return;
  }

  m_pInputLayout = g_graphicsMan().createInputLayoutFromShader(m_pShader);
}

void
Pass::setPass() const
{
  g_graphicsMan().setProgramShader(m_pShader);
  g_graphicsMan().setInputLayout(m_pInputLayout);
  g_graphicsMan().setSamplerState(m_pSamplerState);

  if (m_pRasterState) {
    g_graphicsMan().setRasterizerState(m_pRasterState);
  }

  if (m_pBlendState) {
    g_graphicsMan().setBlendState(m_pBlendState);
  }

  if (m_pDsState) {
    g_graphicsMan().setDepthStencilState(m_pDsState);
  }

  for (uint32 i = 0; i < m_vsCBuffers.size(); ++i) {
    if (m_vsCBuffers[i]) {
      g_graphicsMan().vsSetConstantBuffers(m_vsCBuffers[i], i);
    }
  }

  for (uint32 i = 0; i < m_psCBuffers.size(); ++i) {
    if (m_psCBuffers[i]) {
      g_graphicsMan().psSetConstantBuffers(m_psCBuffers[i], i);
    }
  }
}
}
