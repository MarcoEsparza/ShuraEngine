/*****************************************************************************/
/*
*  @file    shOGLTexture.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/16
*  @brief   Open GL texture class.
*
*  Open GL texture class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesOGLGraphics.h"
#include "shTexture.h"

namespace shEngineSDK {
class OGLTexture2D final : public Texture2D
{
 public:
   OGLTexture2D() = default;
  ~OGLTexture2D();

  friend class OGLGraphicsManager;

  void
  setDebugName(const String& name) override;

 protected:
  uint32 m_texture = 0;
  uint32 m_shaderResourceView = 0;
  uint32 m_depthStencilView = 0;
  Vector<uint32> m_renderTargetViews;
  Vector<uint32> m_unorderedAccessViews;
};

class OGLTexture3D final : public Texture3D
{
 public:
  OGLTexture3D() = default;
  ~OGLTexture3D();
  friend class OGLGraphicsManager;
 protected:
  uint32 m_texture = 0;
  uint32 m_shaderResourceView = 0;
};
}
