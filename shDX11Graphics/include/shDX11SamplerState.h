/*****************************************************************************/
/*
*  @file    shDX11SamplerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   DirectX11 Sampler State class wrapper.
*
*  DirectX11 Sampler State class wrapper.
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
#include "shPrerequisitesDX11Graphics.h"
#include <shSamplerState.h>

namespace shEngineSDK {
/**
*  @brief DirectX11 Sampler State class wrapper.
*/
class DX11SamplerState : public SamplerState
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11SamplerState() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11SamplerState();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Sampler State pointer.
  */
  ID3D11SamplerState* m_pSamplerLinear = nullptr;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE DX11SamplerState::~DX11SamplerState()
{
  SafeRelease(m_pSamplerLinear);
}
}
