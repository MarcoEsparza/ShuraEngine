/*****************************************************************************/
/*
*  @file    shSkyBoxComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/06
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
#include "shSkyBoxComponent.h"
#include "shGraphicsManager.h"
#include "shImageResource.h"

namespace shEngineSDK {
void
SkyBoxComponent::setSkyBoxResource(const WPtr<ImageResource>& skyBoxResource)
{
  //GraphicsManager& graphMan = g_graphicsMan();

  if (auto res = skyBoxResource.lock()) {
    if (m_skyBoxResource) {
      //m_skyBoxResource.reset();
      m_skyBoxResource = nullptr;
    }
    m_skyBoxResource = sh_reinterpretPCast<ImageResource>(res);
    //graphMan.generateMips(m_skyBoxResource->texture);
  }
  else {
    //SH_LOG_ERROR("Failed to set skybox resource: Resource is expired or invalid.");
  }
}
const SPtr<ImageResource>&
SkyBoxComponent::getSkyBoxResource() const
{
  return m_skyBoxResource;
}
}
