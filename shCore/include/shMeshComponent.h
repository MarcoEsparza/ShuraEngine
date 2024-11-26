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
#include "shComponent.h"
#include "shMaterial.h"

namespace shEngineSDK {
class SH_CORE_EXPORT MeshComponent : public Component
{
 public:
  MeshComponent() = default;
  virtual ~MeshComponent() = default;

  SPtr<Material> material;
};

class SH_CORE_EXPORT StaticMeshComponent : public MeshComponent
{
 public:
  StaticMeshComponent() = default;
  ~StaticMeshComponent() = default;


};

class SH_CORE_EXPORT SkeletalMeshComponent : public MeshComponent
{
 public:
  SkeletalMeshComponent() = default;
  ~SkeletalMeshComponent() = default;
};
}
