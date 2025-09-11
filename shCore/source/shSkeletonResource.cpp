/*****************************************************************************/
/*
*  @file    shSkeletonResource.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/10
*  @brief   Skeleton info class.
*
*  Skeleton info class.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shSkeletonResource.h"

namespace shEngineSDK {
int32
SkeletonResource::getBoneID(const String& name) const
{
  auto it = m_boneMapping.find(name);
  if (it != m_boneMapping.end()) {
    return it->second;
  }
  return -1;
}
}
