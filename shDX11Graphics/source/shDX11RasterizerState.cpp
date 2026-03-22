/*****************************************************************************/
/*
*  @file    shDX11RasterizerState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Rasterizer State.
*
*  DirectX11 Rasterizer State.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11RasterizerState.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
DX11RasterizerState::~DX11RasterizerState() {
  SafeRelease(m_pRasterS);
}
}

#endif
