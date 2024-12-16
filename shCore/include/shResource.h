/*************************************************************/
/*
*  @file    shResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Resource base class.
*
*  Resource base class.
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

namespace shEngineSDK {
/**
*  @brief Resource type.
*/
namespace RESOURCE_TYPE{
enum SH_CORE_EXPORT E
{
  kTexture = 0,
  kStaticMesh,
  kSkeletalMesh,
  kSkeleton,
  kAnmimation,
  kMeshUnion,

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
  Resource(const RESOURCE_TYPE::E rType) : m_type(rType) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Resource() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
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
  FORCEINLINE String
  getName() const { return m_name; }

  /**
  *  @brief Sets the resource name.
  *
  *  @param String& name
  */
  FORCEINLINE void
  setName(const String& name) { m_name = name; }

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Resource name.
  */
  String m_name;

  /**
  *  @brief Resource type.
  */
  RESOURCE_TYPE::E m_type;
};
}
