#include "shDX11Texture.h"

namespace shEngineSDK
{
  DX11Texture::~DX11Texture()
  {
    SafeRelease(m_pShaderRV);
  }

  DX11Texture2D::~DX11Texture2D()
  {
    SafeRelease(m_pShaderRV);
    SafeRelease(m_pDepthSV);
    SafeRelease(m_pRenderTV);
    SafeRelease(m_pTexture2D);
  }
}