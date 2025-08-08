/*****************************************************************************/
/*
*  @file    shDX11Texture.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/21
*  @brief   DirectX11 Texture2D and Texture3D class wrappers.
*
*  DirectX11 Texture2D and Texture3D class wrappers.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shDX11Texture.h"

namespace shEngineSDK
{
DX11Texture2D::~DX11Texture2D()
{
  SafeRelease(m_pShaderRV);
  SafeRelease(m_pDepthSV);
  for(auto& it : m_pRenderTV) {
    SafeRelease(it);
  }
  for(auto& it : m_pUnorderedAV) {
    SafeRelease(it);
  }
  SafeRelease(m_pTexture2D);
}

DX11Texture3D::~DX11Texture3D()
{
  SafeRelease(m_pTexture3D);
  SafeRelease(m_pShaderRV);
}
}