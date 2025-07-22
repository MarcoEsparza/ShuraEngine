/*****************************************************************************/
/*
*  @file    shStdHeaders.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/22
*  @brief   Header for the STD libraries
*
*  This file includes the most common and required STL objects.
*  It takes account of the operating system of the build to modify
*  the required object.
* 
*  @bug     No bug known
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  C type objects
*/
/*****************************************************************************/
#include <cmath>
#include <assert.h>
#include <algorithm>

/*****************************************************************************/
/*
*  Types
*/
/*****************************************************************************/
#include "shPlatformTypes.h"

/*****************************************************************************/
/*
*  STL Containers
*/
/*****************************************************************************/
#include <array>
#include <vector>
#include <unordered_map>
#include <queue>

/*****************************************************************************/
/*
*  Standard Containers defined as own
*/
/*****************************************************************************/
#include <filesystem>

/*****************************************************************************/
/*
*  C++ Stream Stuff
*/
/*****************************************************************************/
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
*  @brief Shared pointer that will be used for Shura Engine.
*/
template <typename T>
using SPtr = std::shared_ptr<T>;

/*
*  @brief Weak pointer that will be used for Shura Engine.
*/
template <typename T>
using WPtr = std::weak_ptr<T>;

/*
*  @brief Unique pointer that will be used for Shura Engine.
*/
template <typename T>
using UPtr = std::unique_ptr<T>;

/*
*  @brief Vector wrapper to use along the engine.
*/
template <typename T>
using Vector = std::vector<T>;

/*
*  @brief String wrapper to use along the engine.
*/
using String = std::string;

/*
*  @brief WString wrapper to use along the engine.
*/
using WString = std::wstring;

/*
*  Stringstream wrapper to use along the engine.
*/
using StringStream = std::stringstream;

/*
*  @brief UnorderedMap to use along the engine.
*/
template <typename T, typename U>
using UMap = std::unordered_map<T, U>;

/*
*  @brief Pair to use along the engine.
*/
template <typename T, typename U>
using Pair = std::pair<T, U>;

/*
*  @brief Queue to use along the engine.
*/
template <typename T>
using Queue = std::queue<T>;

/*
*  @brief Function to create an engine shared pointer.
*/
template<typename T, typename... Args>
SPtr<T>
sh_makeShared(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

/*
*  @brief Function to create an engine unique pointer.
*/
template<typename T, typename... Args>
UPtr<T>
sh_makeUnique(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

/*
*  @brief Function to reinterpret a shared pointer.
*/
template<typename to, typename from>
SPtr<to>
sh_reinterpretPCast(const SPtr<from>& pFrom)
{
  return std::reinterpret_pointer_cast<to>(pFrom);
}

/**
*  @brief Byte wrapper to use along the engine.
*/
using Byte = std::byte;

/**
*  @breif File stream for Shura Engine.
*/
using sh_fstream = std::fstream;

/**
*  @breif This will change later to a wrapper.
*/
using std::ios;

/**
*  @brief File system path wrapper.
*/
using SystemPath = std::filesystem::path;

/**
*  @brief Hash wrapper.
*/
template<class T>
using sh_hash = std::hash<T>;
}

namespace std
{
template<>
struct hash<string>
{
  size_t operator()(const string& path) const
  {
    size_t hash = 0;
    for (char i : path)
    {
      hash = 65599 * hash + i;
    }

    return hash ^ (hash >> 16);
  }
};
}

namespace cast {
/*
*  @brief Function to reinterpret a shared pointer.
*/
template<typename to, typename from>
std::shared_ptr<to>
rePointer(const std::shared_ptr<from>& pFrom)
{
  return std::reinterpret_pointer_cast<to>(pFrom);
}

/*
*  @brief Function to reinterpret cast any object.
*/
template<typename to, typename from>
to
re(const from& fromObje)
{
  return reinterpret_cast<to>(fromObje);
}

/*
*  @brief Function to static cast any object.
*/
template<typename to, typename from>
to
st(const from& fromObje)
{
  return static_cast<to>(fromObje);
}
}
