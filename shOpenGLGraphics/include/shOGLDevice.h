/*************************************************************/
/*
*  @file    shOGLDevice.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/27
*  @brief   Open GL Device and RenderContext.
*
*  Open GL Device and RenderContext.s
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
#include "shPrerequisitesOGLGraphics.h"
#include "shDevice.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32
struct HDC__;
using HandleDC = HDC__*;
struct HGLRC__;
using RenderingContext = HGLRC__*;
#else
using HandleDC = void*;
using RenderingContext = void*;
#endif

namespace shEngineSDK {
/**
*  @brief 
*/
class OGLDevice : public Device
{
 public:
  /**
  *  @brief Default constructor.
  */
  OGLDevice() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~OGLDevice();

  friend class OGLGraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  HandleDC m_device = nullptr;
};

class OGLRenderContext : public DeviceContext
{
 public:
  /**
  *  @brief Default constructor.
  */
  OGLRenderContext() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~OGLRenderContext();

  friend class OGLGraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  RenderingContext m_rContext = nullptr;
};

FORCEINLINE OGLDevice::~OGLDevice()
{
  
}

FORCEINLINE OGLRenderContext::~OGLRenderContext()
{
  
}
}
