/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
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
#include "shResource.h"
#include "shTexture.h"

namespace shEngineSDK {
class ImageResource : public Resource
{
 public:
  ImageResource() = default;
  virtual ~ImageResource() = default;

  SPtr<Texture2D> texture;
};
}
