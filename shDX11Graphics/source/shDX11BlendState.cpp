/*****************************************************************************/
/*
*  @file    shDX11BlendState.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/06
*  @brief   DirectX11 Blend State.
*
*  DirectX11 Blend State.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11BlendState.h"

namespace shEngineSDK {
DX11BlendState::~DX11BlendState()
{
  SafeRelease(m_pBlendS);
}

void
DX11BlendState::setDebugName(const String& name)
{
  if (m_pBlendS) {
    m_pBlendS->SetPrivateData(WKPDID_D3DDebugObjectName,
                              cast::st<UINT>(name.size()),
                              name.c_str());
  }
}
}
