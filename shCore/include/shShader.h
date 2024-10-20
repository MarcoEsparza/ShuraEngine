/*************************************************************/
/*
*  @file    shShader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Shader class wrappers.
*
*  Shader class wrappers.
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
class DX11Shader;

/**
*  @brief Shader wrapper.
*/
using Shader = DX11Shader;

class DX11VertexShader;

/**
*  @brief VertexShader wrapper.
*/
using VertexShader = DX11VertexShader;

class DX11PixelShader;

/**
*  @brief PixelShader wrapper.
*/
using PixelShader = DX11PixelShader;
#endif

}
