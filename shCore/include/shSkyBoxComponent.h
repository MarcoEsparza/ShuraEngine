/*****************************************************************************/
/*
*  @file    shSkyBoxComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/06
*  @brief   Skybox component to render a skybox in the scene.
*
*  Skybox component to render a skybox in the scene.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shComponent.h"
#include <shVector3.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward Declarations
*/
/*****************************************************************************/

class ImageResource;

/**
*  @brief Skybox component to render a skybox in the scene.
*/
class SH_CORE_EXPORT SkyBoxComponent : public Component
{
 public:
  /**
  *  @brief Constructor for the SkyBoxComponent.
  */
  SkyBoxComponent()
    : Component(COMPONENT_TYPE::kSkyBox)
  {}

  /**
  *  @brief Destructor for the SkyBoxComponent.
  */
  virtual ~SkyBoxComponent() = default;

 public:
  void
  setSkyBoxResource(const WPtr<ImageResource>& skyBoxResource);

  const SPtr<ImageResource>&
  getSkyBoxResource() const;

 protected:
  void
  onUpdate() override {}

 private:
  /**
  *  @brief Resource for the skybox texture
  */
  SPtr<ImageResource> m_skyBoxResource;
};
}
