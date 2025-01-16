/*************************************************************/
/*
*  @file    shDX11BlendState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/14
*  @brief   DirectX11 Blend State.
*
*  DirectX11 Blend State.
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
#include "shBlendState.h"
#include "shLinearColor.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Blend State.
*/
class DX11BlendState : public BlendState
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11BlendState() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11BlendState();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blend State pointer.
  */
  ID3D11BlendState* m_pBlendS = nullptr;

  /**
  *  @brief Blend factor color.
  */
  LinearColor m_blendFactor;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11BlendState::~DX11BlendState()
{
  SafeRelease(m_pBlendS);
}
}
