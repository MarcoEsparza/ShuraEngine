/*****************************************************************************/
/*
*  @file    shImgCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   Image codec for loading and saving image files.
*
*  Image codec for loading and saving image files.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shImgCodec.h"
#include <shStringID.h>
#include <shResourceManager.h>
#include <shGraphicsManager.h>
#include <shLogger.h>
#include <shException.h>
#include <shImageResource.h>

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

namespace shEngineSDK {
UID
ImgCodec::getUID() const
{
  return StringID("ImgCodec").getUID();
}

Vector<String>
ImgCodec::getSupportedExtensions() const
{
  // Current supported extensions,
  // but this should be changed to be more dynamic in the future.
  return { ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".hdr", ".dds" };
}

bool
ImgCodec::decode(const Path& filePath) const
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  auto pImgRes = sh_makeShared<ImageResource>();
  int32 width = 0;
  int32 height = 0;
  int32 bpp = 0;
  int32 reqComp = STBI_rgb_alpha;

  if (filePath.extension().compare(".hdr") == 0) {
    float* data = stbi_loadf(filePath.c_str(), &width, &height, &bpp, reqComp);
    pImgRes->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);
    stbi_image_free(data);
  }
  else if(filePath.extension().compare(".dds") == 0) {
    pImgRes->texture = graphMan.createTextureFromDDS(filePath);
  }
  else {
    uint8* data = stbi_load(filePath.c_str(), &width, &height, &bpp, reqComp);
    pImgRes->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);
    stbi_image_free(data);
  }

  if (pImgRes->texture == nullptr) {
    SH_LOG_ERROR("Failed to load image: " + filePath.string());
    return false;
  }

  pImgRes->width = width;
  pImgRes->height = height;
  pImgRes->setName(filePath.filename());
  pImgRes->setPath(filePath);

  if(resMan.addResource(pImgRes)) {
    SH_LOG_INFO("Loaded image: " + filePath.string());
    return true;
  }

  SH_LOG_ERROR("Failed to load image: " + filePath.string());
  return false;
}

bool
ImgCodec::encode(const String& objName, const Path& saveFilePath) const
{
  SH_UNREFERENCED_PARAMETER(objName);
  SH_UNREFERENCED_PARAMETER(saveFilePath);
  return false;
}
}
