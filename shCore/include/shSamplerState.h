/*************************************************************/
/*
*  @file    shSamplerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Sampler State class.
*
*  Engine Sampler State class.
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
#include "shPrerequisitesCore.h"

namespace shEngineSDK {
/**
*  @brief Engine Sampler State class.
*/
class SamplerState
{
 public:
  /**
  *  @brief Default constructor.
  */
  SamplerState() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~SamplerState() = default;
};
}
