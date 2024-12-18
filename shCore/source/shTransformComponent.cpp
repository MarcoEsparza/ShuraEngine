/*************************************************************/
/*
*  @file    shTransformComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief   Transform Component class.
*
*  Transform Component class.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shTransformComponent.h"

namespace shEngineSDK {
Transform&
TransformComponent::getTransform()
{
  return m_transform;
}
}
