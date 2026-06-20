/*****************************************************************************/
/*
*  @file    shAssetCodec.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
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
#include "shAssetCodec.h"
#include "shResourceManager.h"
#include <shLogger.h>
#include <shFileExplorer.h>
#include <shSceneGraph.h>
#include <shStringID.h>
#include <shException.h>
#include "shMeshResource.h"
#include "shMaterial.h"
#include "shImageResource.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shSkyBoxComponent.h"
#include "shLightComponent.h"
#include "shCamera.h"

using std::getline;

namespace shEngineSDK {
const UID AssetCodec::SCENE_ASSET_UID = StringID("SceneAsset").getID();
const UID AssetCodec::GAMEOBJECT_ASSET_UID = StringID("GameObjectAsset").getID();

UID
AssetCodec::getUID() const
{
  return StringID("AssetCodec").getID();
}

Vector<String>
AssetCodec::getSupportedExtensions() const
{
  return { ".sha" };
}

bool
AssetCodec::decode(const Path& filePath) const
{
  SceneGraph& scene = g_sceneGraph();
  FILE* file = nullptr;

#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&file, filePath.string().c_str(), "rb");
#else
  file = fopen(filePath.string().c_str(), "rb");
#endif

  if (!file) {
    SH_LOG_ERROR("Failed to open file for reading: " + filePath.string());
    return false;
  }

  //ASSET_TYPE::E assetType;
  UID assetType;
  fread(&assetType, sizeof(UID), 1, file);

  if(assetType == SCENE_ASSET_UID) {
    return decodeScene(filePath, file);
  }
  else {
    SH_LOG_ERROR("Unsupported asset type for decoding: " + std::to_string(assetType));
    fclose(file);
    return false;
  }

  return false;
}

bool
AssetCodec::encode(const String& objName, const Path& saveFilePath) const
{
  UID assetUID = StringID(objName).getID();

  if(assetUID == SCENE_ASSET_UID) {
    return encodeScene(saveFilePath);
  }
  else {
    SH_LOG_ERROR("Unsupported asset type for encoding: " + objName);
    return false;
  }

  return false;
}

bool
AssetCodec::encodeScene(const Path& saveFilePath) const
{
  SceneGraph& scene = g_sceneGraph();
  FILE* file = nullptr;

#if SH_COMPILER == SH_COMPILER_MSVC
  fopen_s(&file, saveFilePath.string().c_str(), "wb");
#else
  file = fopen(saveFilePath.string().c_str(), "wb");
#endif

  if (!file) {
    SH_LOG_ERROR("Failed to open file for writing: " + saveFilePath.string());
    return false;
  }

  //ASSET_TYPE::E assetType = ASSET_TYPE::kScene;
  fwrite(&SCENE_ASSET_UID, sizeof(UID), 1, file);

  SceneAssetData sceneData;
  sceneData.numGameObjects = cast::st<uint32>(scene.getGameObjectList().size());
  fwrite(&sceneData, sizeof(SceneAssetData), 1, file);

  for (const auto& gameObject : scene.getGameObjectList()) {
    // Write the asset type for the game object
    //ASSET_TYPE::E gameObjectType = ASSET_TYPE::kGameObject;
    fwrite(&GAMEOBJECT_ASSET_UID, sizeof(UID), 1, file);
    
    // Write the game object name
    String name = gameObject->m_name;
    uint32 nameLength = cast::st<uint32>(name.size());
    fwrite(&nameLength, sizeof(uint32), 1, file);
    fwrite(name.c_str(), sizeof(char), nameLength, file);

    // Write the number of components in the game object
    uint32 numComponents = cast::st<uint32>(gameObject->m_componentList.size());
    fwrite(&numComponents, sizeof(uint32), 1, file);

    TransformComponentData transformData;
    transformData.position = gameObject->getPosition();
    transformData.rotation = gameObject->getRotation();
    transformData.scale = gameObject->getScale();
    fwrite(&transformData, sizeof(TransformComponentData), 1, file);

    // Write each component
    for (const auto& component : gameObject->m_componentList) {
      if (!encodeComponent(component, file)) {
        SH_LOG_ERROR("Failed to encode component for game object: " + name);
        fclose(file);
        return false;
      }
    }
  }

  fclose(file);

  SH_LOG_INFO("Scene encoded successfully to: " + saveFilePath.string());
  return true;
}

bool
AssetCodec::encodeComponent(const SPtr<Component>& component, FILE* file) const
{
  COMPONENT_TYPE::E type = component->getType();
  fwrite(&type, sizeof(COMPONENT_TYPE::E), 1, file);
  
    switch (type) {
      case COMPONENT_TYPE::kTexture:
        // Encode texture component data
        break;
      case COMPONENT_TYPE::kStaticMesh:
      {
        auto meshComp = cast::re_ptr<StaticMeshComponent>(component);
        auto& meshRes = meshComp->m_mesh;

        if (meshRes.expired()) {
          SH_LOG_ERROR("Mesh resource expired for static mesh component.");
          return false;
        }

        auto pMeshRes = meshRes.lock();
        Path meshPath = pMeshRes->getPath();
        uint32 pathLength = cast::st<uint32>(meshPath.string().size());
        fwrite(&pathLength, sizeof(uint32), 1, file);
        fwrite(meshPath.string().c_str(), sizeof(char), pathLength, file);

        break;
      }
      case COMPONENT_TYPE::kSkeletalMesh:
        // Encode skeletal mesh component data
        break;
      case COMPONENT_TYPE::kAnimator:
        // Encode animator component data
        break;
      case COMPONENT_TYPE::kSkyBox:
      {
        auto skyboxComp = cast::re_ptr<SkyBoxComponent>(component);
        auto& pSkyboxRes = skyboxComp->getSkyBoxResource();
        if (!pSkyboxRes) {
          SH_LOG_ERROR("Skybox resource expired for skybox component.");
          return false;
        }
        Path skyboxPath = pSkyboxRes->getPath();
        uint32 pathLength = cast::st<uint32>(skyboxPath.string().size());
        fwrite(&pathLength, sizeof(uint32), 1, file);
        fwrite(skyboxPath.string().c_str(), sizeof(char), pathLength, file);

        break;
      }
      case COMPONENT_TYPE::kRigidbody:
        // Encode rigidbody component data
        break;
      case COMPONENT_TYPE::kCollider:
        // Encode collider component data
        break;
      case COMPONENT_TYPE::kCamera:
        // Encode camera component data
        break;
      case COMPONENT_TYPE::kLight:
      {
        auto lightComp = cast::re_ptr<LightComponent>(component);
        LightComponentData lightData;
        lightData.color = lightComp->m_color;
        lightData.intensity = lightComp->m_intensity;
        lightData.lightType = lightComp->m_lightType;

        // Encode shadow camera data
        Camera& shadowCamera = lightComp->m_lightCamera;
        lightData.shadowCameraData.position = shadowCamera.getPosition();
        lightData.shadowCameraData.target = shadowCamera.getTarget();
        lightData.shadowCameraData.up = shadowCamera.getUp();
        lightData.shadowCameraData.halfFOV = shadowCamera.getHalfFOV();
        lightData.shadowCameraData.isOrthographic = shadowCamera.isOrtho();
        lightData.shadowCameraData.width = shadowCamera.getWidth();
        lightData.shadowCameraData.height = shadowCamera.getHeight();
        lightData.shadowCameraData.nearZ = shadowCamera.getNear();
        lightData.shadowCameraData.farZ = shadowCamera.getFar();
        fwrite(&lightData, sizeof(LightComponentData), 1, file);

        break;
      }
      default:
        SH_LOG_ERROR("Unsupported component type for encoding: " + std::to_string(type));
        return false;
    }

  return false;
}

bool
AssetCodec::decodeScene(const Path& filePath, FILE* file) const
{
  SceneGraph& scene = g_sceneGraph();

  SceneAssetData sceneData;
  fread(&sceneData, sizeof(SceneAssetData), 1, file);

  auto& gameObjectList = scene.getGameObjectList();
  gameObjectList.reserve(sceneData.numGameObjects);

  for (uint32 i = 0; i < sceneData.numGameObjects; ++i) {
    UID assetType;
    fread(&assetType, sizeof(UID), 1, file);
    if (assetType != GAMEOBJECT_ASSET_UID) {
      SH_LOG_ERROR("Unexpected asset type while decoding scene: " + std::to_string(assetType));
      fclose(file);
      return false;
    }
    GameObjectAssetData gameObjectData;

    uint32 nameLength;
    fread(&nameLength, sizeof(uint32), 1, file);
    gameObjectData.name.resize(nameLength);
    fread(gameObjectData.name.data(), sizeof(char), nameLength, file);
    fread(&gameObjectData.numComponents, sizeof(uint32), 1, file);
    SPtr<GameObject> gameObject = std::make_shared<GameObject>();
    gameObject->m_name = gameObjectData.name;
    TransformComponentData transformData;
    fread(&transformData, sizeof(TransformComponentData), 1, file);
    gameObject->setPosition(transformData.position);
    gameObject->setRotation(transformData.rotation);
    gameObject->setScale(transformData.scale);
    for (uint32 j = 0; j < gameObjectData.numComponents; ++j) {
      if (!decodeComponent(gameObject, file)) {
        SH_LOG_ERROR("Failed to decode component for game object: " + gameObjectData.name);
        fclose(file);
        return false;
      }
    }
    scene.addObject(gameObject);
  }

  SH_LOG_INFO("Scene decoded successfully from: " + filePath.string());
  return true;
}

bool
AssetCodec::decodeComponent(SPtr<GameObject>& gameObject, FILE* file) const
{


  return false;
}
}
