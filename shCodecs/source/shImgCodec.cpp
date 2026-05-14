/*****************************************************************************/
/*
*  @file    shImgCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/13
*  @brief
*
*
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
#include <shImageResource.h>

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

namespace shEngineSDK {
UID
ImgCodec::getUID() const
{
  return StringID("ImgCodec").getUID();
}

bool
ImgCodec::canDecodeExtension(const String& extension) const
{
  Vector<String> supportedExtensions = getSupportedExtensions();
  for (const String& suppExt : supportedExtensions) {
    if (extension.compare(suppExt) == 0) {
      return true;
    }
  }
  return false;
}

Vector<String>
ImgCodec::getSupportedExtensions() const
{
  // Current supported extensions,
  // but this should be changed to be more dynamic in the future.
  return { ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".hdr" };
}

bool
ImgCodec::decode(const String& filePath) const
{
  GraphicsManager& graphMan = g_graphicsMan();
  ResourceManager& resMan = g_resourceMan();

  auto pImgRes = sh_makeShared<ImageResource>();
  int32 width = 0;
  int32 height = 0;
  int32 bpp = 0;
  int32 reqComp = STBI_rgb_alpha;

  SystemPath path = filePath;

  if (path.extension().compare(".hdr") == 0) {
    float* data = stbi_loadf(filePath.c_str(), &width, &height, &bpp, reqComp);
    pImgRes->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);
    stbi_image_free(data);
  }
  else {
    uint8* data = stbi_load(filePath.c_str(), &width, &height, &bpp, reqComp);
    pImgRes->texture = graphMan.createTextureFromFile(filePath, data, width, height, bpp);
    stbi_image_free(data);
  }

  if (pImgRes->texture == nullptr) {
    SH_LOG_ERROR("Failed to load image: " + filePath);
    return false;
  }

  pImgRes->width = width;
  pImgRes->height = height;
  pImgRes->setName(path.filename().string());
  pImgRes->setPath(Path(filePath));

  if(resMan.addResource(pImgRes)) {
    SH_LOG_INFO("Loaded image: " + filePath);
    return true;
  }

  SH_LOG_ERROR("Failed to load image: " + filePath);
  return false;
}

bool
ImgCodec::encode(const String& filePath) const
{
  return false;
}
}
