/*************************************************************/
/*
*  @file    shDX11InputLayout.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   DirectX11 Input Layout class wrapper.
*
*  DirectX11 Input Layout class wrapper.
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
*  @brief DirectX11 Input Layout class wrapper.
*/
class DX11InputLayout
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11InputLayout() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11InputLayout();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Input Layout pointer.
  */
  ID3D11InputLayout* m_pLayout = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11InputLayout::~DX11InputLayout()
{
  SafeRelease(m_pLayout);
}
}
