/*****************************************************************************/
/*
*  @file    shResourceManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   Resource Manager module for loading all desired resources
*           from files.
*
*  Resource Manager module for loading all desired resources from files.
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
#include "shResource.h"
#include <shModule.h>
#include <shPath.h>

#define MODS_RESOURCES_PATH "resources/mods/"

namespace shEngineSDK {
/**
*  @brief Resource Manager module for loading all desired resources from files.
*/
class SH_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  ResourceManager() = default;

  /**
  *  @brief Default destructor.
  */
  ~ResourceManager();


  void
  onStartUp() override;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  void
  loadDefaultResources();

  /**
  *  @brief Load resource from desired file, clasify it and create a cache
  *         for optimizing the next time the resource will be loaded.
  *
  *  @param Path& filePath
  */
  SPtr<Resource>
  loadResourceFromFile(const Path& filePath);

  bool
  addResource(const SPtr<Resource> pRes);

  /**
  *  @brief Get the desired resource by its name.
  * 
  *  @param String& resourceName
  */
  SPtr<Resource>
  getResource(const String& resourceName);

  /**
  *  @brief Saves the given resource to an asset for better loading.
  *
  *  @param SPtr<Resource> pRes
  * 
  *  TODO: This function is not completed yet.
  */
  bool
  saveResourceToAsset(const SPtr<Resource> pRes, const String& path);

 private:
  /**
  *  @brief Checks if the resource is already loaded.
  * 
  *  @param String& fileName
  */
  bool
  isResourceLoaded(const String& fileName);

  /**
  *  @brief Checks if the resource is already loaded.
  *
  *  @param Path& fileName
  */
  SPtr<Resource>
  isResourceLoaded(const Path& fileName);

  /***************************************************************************/
  /*
  *  Texture
  */
  /***************************************************************************/

  SPtr<Resource>
  loadCubeMapFromFile(const Path& filePath);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief All the resources are storaged here.
  */
  UMap<String, SPtr<Resource>> m_loadedResources;
};

/**
*  @brief Easier way to access the Time module.
*/
SH_CORE_EXPORT ResourceManager&
g_resourceMan();
}
