/*************************************************************/
/*
*  @file    shMesh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
*  @brief   Basic mesh structure for models.
*
*  Basic mesh structure for models.
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
#include "shTexture.h"

namespace shEngineSDK {
/**
*  @brief
*/
struct SH_CORE_EXPORT Material
{
  String name;
  UMap<TEXTURE_TYPE::E, SPtr<Texture2D>> textures;
};

/**
*  @brief
*/
struct SH_CORE_EXPORT Mesh
{
  uint32 baseVertex = 0;
  uint32 numVertex = 0;
  uint32 baseIndex = 0;
  uint32 numIndices = 0;
  uint32 matIndex = 0;
};
}
