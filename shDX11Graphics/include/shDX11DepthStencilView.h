/*************************************************************/
/*
*  @file    shDX11DepthStencilView.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   DirectX11 Depth Stencil View class wrapper.
*
*  DirectX11 Depth Stencil View class wrapper.
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
*  @brief DirectX11 Depth Stencil View class wrapper.
*/
class DX11DepthStencilView
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11DepthStencilView() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11DepthStencilView();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Depth Stencil View pointer.
  */
  ID3D11DepthStencilView* m_pDepthSV = nullptr;

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

FORCEINLINE DX11DepthStencilView::~DX11DepthStencilView()
{
  SafeRelease(m_pDepthSV);
  SafeRelease(m_pTexture2D);
}
}
