/*************************************************************/
/*
*  @file    shDX11Texture.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   DirectX11 Texture and Texture2D class wrappers.
*
*  DirectX11 Texture and Texture2D class wrappers.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesDX11Graphics.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Texture class wrapper.
*/
class DX11Texture
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11Texture() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11Texture();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Shader Resource View pointer.
  */
  ID3D11ShaderResourceView* m_pShaderRV = nullptr;

  /**
  *  @brief DirectX11 Depth Stencil View pointer.
  */
  ID3D11DepthStencilView* m_pDepthSV = nullptr;
};

/**
*  @brief DirectX11 Texture2D class wrapper.
*/
class DX11Texture2D : public DX11Texture
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11Texture2D() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11Texture2D();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Texture2D pointer.
  */
  ID3D11Texture2D* m_pTexture2D = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11Texture::~DX11Texture()
{
  SafeRelease(m_pShaderRV);
  SafeRelease(m_pDepthSV);
}

FORCEINLINE DX11Texture2D::~DX11Texture2D()
{
  SafeRelease(m_pTexture2D);
}
}
