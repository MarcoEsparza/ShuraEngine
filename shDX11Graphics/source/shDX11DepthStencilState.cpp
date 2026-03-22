/*****************************************************************************/
/*
*  @file    shDX11DepthStencilState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Depth Stencil State.
*
*  DirectX11 Depth Stencil State.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11DepthStencilState.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
DX11DepthStencilState::~DX11DepthStencilState() {
  SafeRelease(m_pDepthSS);
}
}

#endif
