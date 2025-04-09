/*************************************************************/
/*
*  @file    shPhysicsTestUsage.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/06
*  @brief   Basic info for physics test.
*
*  Basic info for physics test.
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

namespace shEngineSDK {
/**
*  @brief Integration type.
*/
namespace INTEGRATION {
enum E
{
  kEuler = 0,
  kVerlet
};
}

namespace IK_ALGORITHM {
enum E
{
  kFabrik = 0,
  kCCD
};
}

namespace MOVEMENT_TYPE {
enum E
{
  kFoward = 0,
  kInverse
};
}
}
