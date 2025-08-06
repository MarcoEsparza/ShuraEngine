/*************************************************************/
/*
*  @file    shImageResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Image Resource class.
*
*  Image Resource class.
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
#include "shResource.h"
#include "shTexture.h"

namespace shEngineSDK {
/**
*  @brief   Image Resource class.
*/
class ImageResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  ImageResource() : Resource(RESOURCE_TYPE::kTexture) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~ImageResource() = default;

  /*************************************************************/
  /*
  *  Variables.
  */
  /*************************************************************/
 public:
  /**
  *  @brief Image texture.
  */
  SPtr<Texture2D> texture;

  uint32 width = 0;
  uint32 height = 0;
};
}
