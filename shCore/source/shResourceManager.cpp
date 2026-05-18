/*************************************************************/
/*
*  @file    shResourceManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   Resource Manager module for loading all desired resources
*           from files.
*
*  Resource Manager module for loading all desired resources from files.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shResourceManager.h"
#include "shGraphicsManager.h"
#include <shLogger.h>
#include <shTimer.h>
#include "shFileExplorer.h"
#include "shCodecManager.h"

#include "shMeshResource.h"
#include "shImageResource.h"
#include "shSkeletonResource.h"
#include "shAnimationResource.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Resource Manager functions
*/
/*************************************************************/

ResourceManager::~ResourceManager()
{
  for (auto& resource : m_loadedResources) {
    resource.second.reset();
  }

  m_loadedResources.clear();
}

void
ResourceManager::onStartUp()
{
  
}

void
ResourceManager::loadDefaultResources()
{
  GraphicsManager& graphMan = g_graphicsMan();

  auto pErrorTex = graphMan.createErrorTexture();
  auto pErrorImg = sh_makeShared<ImageResource>();
  pErrorImg->texture = pErrorTex;
  m_loadedResources["ErrorTexture"] = pErrorImg;

  auto pNormalTex = graphMan.createDefaultNormalTexture();
  auto pNormalImg = sh_makeShared<ImageResource>();
  pNormalImg->texture = pNormalTex;
  m_loadedResources["DefaultNormal"] = pNormalImg;

  auto pBlackTex = graphMan.createBlackTexture();
  auto pBlackImg = sh_makeShared<ImageResource>();
  pBlackImg->texture = pBlackTex;
  m_loadedResources["BlackTexture"] = pBlackImg;

  Path whiteTexPath(FileExplorer::DEFAULT_ASSETS_FOLDER + "Textures/White.png");
  auto white = cast::re_ptr<ImageResource>(loadResourceFromFile(whiteTexPath));
  /*auto normal = cast::re_ptr<ImageResource>(loadResourceFromFile(
                                              Path("resources/textures/normal.png")));*/

  Path cubePath(FileExplorer::DEFAULT_ASSETS_FOLDER + "Models/cube.fbx");
  auto pCube = cast::re_ptr<StaticMeshResource>(loadResourceFromFile(cubePath));
}

SPtr<Resource>
ResourceManager::loadResourceFromFile(const Path& filePath)
{
  CodecManager& codecMan = g_codecManager();

  // Check if resource is already on memory
  if (isResourceLoaded(filePath.filename())) {
    return m_loadedResources[filePath.filename()];
  }

  SPtr<Resource> resource = nullptr;
  auto codec = codecMan.getCodecByExtension(filePath.extension());
  if (codec != nullptr) {
    if (codec->decode(filePath)) {
      String resName = filePath.filename();
      resource = m_loadedResources[resName];
    }
  }
  else {
    String errString = "No codec found for file: " + filePath.string();
    SH_LOG_ERROR(errString);
  }

  return resource;
}

bool
ResourceManager::addResource(const SPtr<Resource> pRes)
{
  if(isResourceLoaded(pRes->getName())) {
    return false;
  }

  m_loadedResources[pRes->getName()] = pRes;
  if (isResourceLoaded(pRes->getName())) {
    return true;
  }

  return false;
}

SPtr<Resource>
ResourceManager::getResource(const String& resourceName)
{
  if (isResourceLoaded(resourceName)) {
    return m_loadedResources[resourceName];
  }
  return nullptr;
}

bool
ResourceManager::isResourceLoaded(const String& fileName)
{
  auto resObj = m_loadedResources.find(fileName);

  if (resObj != m_loadedResources.end()) {
    return true;
  }

  return false;
}

SPtr<Resource>
ResourceManager::isResourceLoaded(const Path& fileName)
{
  auto resObj = m_loadedResources.find(fileName.string());

  if (resObj != m_loadedResources.end()) {
    return (*resObj).second;
  }

  return nullptr;
}

ResourceManager& g_resourceMan()
{
  return ResourceManager::instance();
}
}
