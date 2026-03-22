/*****************************************************************************/
/*
*  @file    shDX11SamplerState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Sampler State class wrapper.
*
*  DirectX11 Sampler State class wrapper.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11SamplerState.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
DX11SamplerState::~DX11SamplerState() {
  SafeRelease(m_pSamplerLinear);
}
}

#endif
