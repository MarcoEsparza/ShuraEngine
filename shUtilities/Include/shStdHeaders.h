/*************************************************************/
/*
*  @file    shStdHeaders.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/09
*  @brief   Header for the STD libraries
*
*  This file includes the most common and required STL objects.
* It takes account of the operating system of the build to modify
* the required object.
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  C type objects
*/
/*************************************************************/
#include <cmath>
#include <assert.h>
#include <algorithm>

/*************************************************************/
/*
*  Types
*/
/*************************************************************/
#include "shPlatformTypes.h"

/*************************************************************/
/*
*  STL Containers
*/
/*************************************************************/
#include <array>
#include <vector>
#include <unordered_map>

/*************************************************************/
/*
*  Standard Containers defined as own
*/
/*************************************************************/


/*************************************************************/
/*
*  C++ Stream Stuff
*/
/*************************************************************/
#include <fstream>
#include <iostream>

//Missing one
#include <memory>

namespace shEngineSDK {
 /*
 * @brief Fixed-size array sequence container class, holds its elements in a
 * strict linear sequence.
 */
template <typename T, SIZE_T size>
using Array = std::array<T, size>;

/*
*  Shared pointer that will be used for Shura Engine.
*/
template <typename T>
using SPtr = std::shared_ptr<T>;

/*
*  Weak pointer that will be used for Shura Engine.
*/
template <typename T>
using WPtr = std::weak_ptr<T>;

/*
*  Vector wrapper to use along the engine.
*/
template <typename T>
using Vector = std::vector<T>;

/*
*  String wrapper to use along the engine.
*/
using String = std::string;

/*
*  WString wrapper to use along the engine.
*/
using WString = std::wstring;

/*
*  Stringstream wrapper to use along the engine.
*/
using StringStream = std::stringstream;

/*
*  UnorderedMap to use along the engine.
*/
template <typename T, typename U>
using UMap = std::unordered_map<T, U>;
}
