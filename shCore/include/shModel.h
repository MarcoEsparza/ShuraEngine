/*************************************************************/
/*
*  @file    shModel.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/06
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
#include "shGraphicTypes.h"
#include "shMesh.h"
#include "shBone.h"
#include "shAnimator.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT Model
{
  Vector<Mesh> meshes;
  Vector<VertexData> vertices;
  Vector<uint32> indices;
  UMap<uint32, SPtr<Material>> materials;
  Skeleton skeletalMesh;
  Vector<Animation> animations;
};
}
