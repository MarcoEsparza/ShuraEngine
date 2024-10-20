/*************************************************************/
/*
*  @file    shRenderTargetView.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   RenderTargetView class wrapper.
*
*  RenderTargetView class wrapper.
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

#if SH_PLATFORM == SH_PLATFORM_WIN32
class DX11RenderTargetView;

/**
*  @brief RenderTargetView wrapper.
*/
using RenderTargetView = DX11RenderTargetView;
#endif

}
