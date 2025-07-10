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
    for(auto& it : m_pRenderTV) {
      SafeRelease(it);
    }
    for(auto& it : m_pUnorderedAV) {
      SafeRelease(it);
    }
    SafeRelease(m_pTexture2D);
  }
}