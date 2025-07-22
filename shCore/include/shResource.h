/*****************************************************************************/
/*
*  @file    shResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Resource base class.
*
*  Resource base class.
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
#include "shPath.h"

namespace shEngineSDK {
/**
*  @brief Resource type.
*/
namespace RESOURCE_TYPE{
enum E
{
  kTexture = 0,
  kStaticMesh,
  kSkeletalMesh,
  kSkeleton,
  kAnmimation,
  kCubeMap,

  kCount
};
}

/**
*  @brief Resource base class.
*/
class SH_CORE_EXPORT Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  Resource() = default;

  /**
  *  @brief Constructor that initialize the type variable
  * 
  *  @param RESOURCE_TYPE::E rType
  */
  Resource(const RESOURCE_TYPE::E rType)
    : m_type(rType)
  {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Resource() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Gets the resource type.
  * 
  *  @return RESOURCE_TYPE::E
  */
  FORCEINLINE RESOURCE_TYPE::E
  getType() const { return m_type; }

  /**
  *  @brief Gets the resource name.
  *
  *  @return String
  */
  FORCEINLINE const String&
  getName() const { return m_name; }

  /**
  *  @brief Sets the resource name.
  *
  *  @param String& name
  */
  FORCEINLINE void
  setName(const String& name) { m_name = name; }

  /**
  *  @brief Gets the resource name.
  *
  *  @return String
  */
  FORCEINLINE const Path&
  getPath() const { return m_path; }

  /**
  *  @brief Sets the resource name.
  *
  *  @param String& name
  */
  FORCEINLINE void
  setPath(const Path& path) { m_path = path; }

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief Resource name.
  */
  String m_name;

  /**
  *  @brief Resource path.
  */
  Path m_path;

  /**
  *  @brief Resource type.
  */
  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::kCount;
};
}
