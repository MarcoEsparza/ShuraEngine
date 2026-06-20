/*****************************************************************************/
/*
*  @file    shAssetCodec.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/17
*  @brief
*
*
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
#include "shCodec.h"
#include <shPath.h>
#include "shComponent.h"
#include <shVector3.h>
#include <shQuaternion.h>
#include <shCollider.h>
#include <shLightComponent.h>

namespace shEngineSDK {
class GameObject;

struct SceneAssetData
{
  uint32 numGameObjects;
};

struct GameObjectAssetData
{
  String name;
  uint32 numComponents;
};

struct TransformComponentData
{
  Vector3 position;
  Quaternion rotation;
  Vector3 scale;
};

struct SkyboxComponentData
{
  Path skyboxPath;
};

struct RigidbodyComponentData
{
  float mass;
  bool isKinematic;
};

struct ColliderComponentData
{
  COLLIDER_TYPE::E type;
  Vector3 size;
};

struct CameraComponentData
{
  Vector3 position;
  Vector3 target;
  Vector3 up;
  float halfFOV;
  float width;
  float height;
  float nearZ;
  float farZ;
  bool isOrthographic;
};

struct LightComponentData
{
  LinearColor color;
  float intensity;
  LIGHT_TYPE::E lightType;
  CameraComponentData shadowCameraData;
};

class AssetCodec : public Codec
{
 public:
  /**
   *  @brief Default constructor.
   */
  AssetCodec() = default;
  /**
   *  @brief Default destructor.
   */
  ~AssetCodec() = default;

  UID
  getUID() const override;

  /**
  *  @brief Gets the list of supported file extensions by the codec.
  *
  *  @return Vector<String>: List of supported file extensions by the codec,
  *                          including the dot.
  */
  Vector<String>
  getSupportedExtensions() const override;

  bool
  decode(const Path& filePath) const override;

  bool
  encode(const String& objName, const Path& saveFilePath) const override;

 private:
  bool
  encodeScene(const Path& saveFilePath) const;

  bool
  encodeComponent(const SPtr<Component>& component, FILE* file) const;

  bool
  decodeScene(const Path& filePath, FILE* file) const;
  
  bool
  decodeComponent(SPtr<GameObject>& gameObject, FILE* file) const;

 private:
  const static UID SCENE_ASSET_UID;
  const static UID GAMEOBJECT_ASSET_UID;
};
}
