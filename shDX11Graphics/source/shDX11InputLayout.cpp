/*****************************************************************************/
/*
*  @file    shDX11InputLayout.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Input Layout class wrapper.
*
*  DirectX11 Input Layout class wrapper.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11InputLayout.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
DX11InputLayout::~DX11InputLayout() {
  SafeRelease(m_pLayout);
}
}

#endif
