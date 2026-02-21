/*****************************************************************************/
/*
*  @file    shDX11Buffers.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Vertex Buffer, Index Buffer and Constant Buffer class wrappers.
*
*  DirectX11 Vertex Buffer, Index Buffer and Constant Buffer class wrappers.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11Buffers.h"

namespace shEngineSDK {
DX11VertexBuffer::~DX11VertexBuffer() {
  SafeRelease(m_pBuffer);
}

DX11IndexBuffer::~DX11IndexBuffer() {
  SafeRelease(m_pBuffer);
}

DX11ConstantBuffer::~DX11ConstantBuffer() {
  SafeRelease(m_pBuffer);
}
}
