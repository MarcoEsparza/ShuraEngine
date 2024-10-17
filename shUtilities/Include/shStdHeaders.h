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
#include <utility>
#include <memory>

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
#include <queue>
#include <list>

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

namespace shEngineSDK {
 /**
 * @brief Fixed-size array sequence container class, holds its elements in a
 *        strict linear sequence.
 */
template <typename T, SIZE_T size>
using Array = std::array<T, size>;

/**
*  @brief Shared pointer that will be used for Shura Engine.
*/
template <typename T>
using SPtr = std::shared_ptr<T>;

/**
*  @brief Weak pointer that will be used for Shura Engine.
*/
template <typename T>
using WPtr = std::weak_ptr<T>;

/**
*  @brief Vector wrapper to use along the engine.
*/
template <typename T>
using Vector = std::vector<T>;

/**
*  @brief String wrapper to use along the engine.
*/
using String = std::string;

/**
*  @brief WString wrapper to use along the engine.
*/
using WString = std::wstring;

/**
*  @brief Stringstream wrapper to use along the engine.
*/
using StringStream = std::stringstream;

/**
*  @brief UnorderedMap to use along the engine.
*/
template <typename T, typename U>
using UMap = std::unordered_map<T, U>;

/**
*  @brief Queue wrapper to use along the engine.
*/
template <typename T>
using Queue = std::queue<T>;

using std::make_shared;
}
