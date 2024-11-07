/*************************************************************/
/*
*  @file    shDX11RenderTargetView.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   DirectX11 Render Target View class wrapper.
*
*  DirectX11 Render Target View class wrapper.
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
#include "shRenderTargetView.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Render Target View class wrapper.
*/
class DX11RenderTargetView : public RenderTargetView
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11RenderTargetView() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11RenderTargetView();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Render Target View pointer.
  */
  ID3D11RenderTargetView* m_pRenderTV = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11RenderTargetView::~DX11RenderTargetView()
{
  SafeRelease(m_pRenderTV);
}
}
