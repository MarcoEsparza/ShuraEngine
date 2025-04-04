/*****************************************************************************/
/*
*  @file    shScriptVector3.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/17
*  @brief   Script for Vector3.
*
*  Script for Vector3.
*
*  @bug     No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesUtilities.h"
#include "shScriptManager.h"
#include "shVector3.h"

namespace shEngineSDK {
FORCEINLINE static void
registerVector3(ScriptState* state)
{
  state->new_usertype<Vector3>(
  // Typename
  "Vector3",

  // Constructors
  sol::constructors<
  Vector3(),
  Vector3(float _x, float _y, float _z),
  Vector3(const Vector3 & other),
  >(),

  // Members
  "x", &Vector3::x,
  "y", &Vector3::y,
  "z", &Vector3::z,

  // Methods
  "dot", &Vector3::dot,

  // Operators
  sol::meta_function::multiplication, &Vector3::operator*
  );
}
}
