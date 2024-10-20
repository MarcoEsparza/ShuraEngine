/*************************************************************/
/*
*  @file    shGraphicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   
*
*  
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
#include "shModule.h"
#include "shScreen.h"
#include "shLinearColor.h"

#include "shBuffers.h"
#include "shDepthStencil.h"
#include "shDevice.h"
#include "shInputLayout.h"
#include "shRenderTargetView.h"
#include "shSamplerState.h"
#include "shShader.h"
#include "shSwapChain.h"
#include "shTexture.h"

#include "shVector2.h"
#include "shVector3.h"
#include "shVector4.h"

namespace shEngineSDK {
/**
*  @brief Sample descriptor.
*/
typedef struct SAMPLE_DESC
{
  uint32 count = 0;
  uint32 quality = 0;
} SAMPLE_DESC;

/**
*  @brief InputLayout descriptor.
*/
struct InputLayoutDesc
{
  String semanticName;
  uint32 semanticIndex;
  uint32 format;
  uint32 inputSlot;
  uint32 aligenedByteOffset;
  uint32 inputSlotClass;
  uint32 instanceDataStepRate;
};

/**
*  @brief Vertex struct.
*/
struct VertexData
{
  Vector3 position;
  Vector3 normal;
  Vector2 tex;
  Vector4 boneIds;
  Vector4 boneWeights;
};

/**
*  @brief Module Graphis Manager.
*/
class GraphicsManager : public Module<GraphicsManager>
{
 public:
  /**
  *  @brief 
  */
  GraphicsManager() = default;

  /**
  *  @brief
  */
  virtual ~GraphicsManager() = default;

 public:
  /**
  *  @brief Initialize the graphics manager.
  *
  *  @param void* srcHandle
  *  @param bool bFullScreen
  *  @param bool bAntiliasing
  *  @param uint32 samplesPerPixel
  *  @param uint32 sampleQuality
  */
  void
  init(PlatformScreen srcHandle,
       bool bFullScreen,
       bool bAntiliasing,
       uint32 samplesPerPixel,
       uint32 sampleQuality);

 protected:
  /**
  *  @brief
  */
  virtual void
  internalInit(PlatformScreen srcHandle,
               bool bFullScreen,
               bool bAntiliasing,
               uint32 samplesPerPixel,
               uint32 sampleQuality) = 0;
};
}
