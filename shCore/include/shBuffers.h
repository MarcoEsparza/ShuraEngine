/*************************************************************/
/*
*  @file    shBuffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Buffer classes wrappers.
*
*  Buffer classes wrappers.
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
class DX11Buffer;

/**
*  @brief Buffer wrapper.
*/
using Buffer = DX11Buffer;

class DX11VertexBuffer;

/**
*  @brief VertexBuffer wrapper.
*/
using VertexBuffer = DX11VertexBuffer;

class DX11IndexBuffer;

/**
*  @brief IndexBuffer wrapper.
*/
using IndexBuffer = DX11IndexBuffer;

class DX11ConstantBuffer;

/**
*  @brief ConstantBuffer wrapper.
*/
using ConstantBuffer = DX11ConstantBuffer;
#endif

}
