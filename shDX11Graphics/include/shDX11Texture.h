/*************************************************************/
/*
*  @file    shDX11Texture.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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
#include "shTexture.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Texture class wrapper.
*/
class DX11Texture : public Texture
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11Texture() = default;

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
};

/**
*  @brief DirectX11 Texture2D class wrapper.
*/
class DX11Texture2D final : public Texture2D
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11Texture2D() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  ~DX11Texture2D();

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

  /**
  *  @brief DirectX11 Render Target View pointer.
  */
  ID3D11RenderTargetView* m_pRenderTV = nullptr;

  /**
  *  @brief DirectX11 Texture2D pointer.
  */
  ID3D11Texture2D* m_pTexture2D = nullptr;

  /**
  *  @brief DirectX11 Unordered Access View pointer.
  */
  ID3D11UnorderedAccessView* m_pUnorderedAV = nullptr;
};

}
