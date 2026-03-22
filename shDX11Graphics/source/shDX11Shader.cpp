/*****************************************************************************/
/*
*  @file    shDX11Shader.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Program Shader class wrapper.
*
*  DirectX11 Program Shader class wrapper.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11Shader.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
DX11VertexShader::~DX11VertexShader() {
  SafeRelease(m_pBlob);
  SafeRelease(m_pVertexShader);
}

DX11PixelShader::~DX11PixelShader() {
  SafeRelease(m_pBlob);
  SafeRelease(m_pPixelShader);
}

DX11GeometryShader::~DX11GeometryShader() {
  SafeRelease(m_pBlob);
  SafeRelease(m_pGeometryShader);
}

DX11ComputeShader::~DX11ComputeShader() {
  SafeRelease(m_pBlob);
  SafeRelease(m_pComputeShader);
}
}

#endif
