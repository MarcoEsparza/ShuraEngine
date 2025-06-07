/*****************************************************************************/
/*
*  @file    shFMODChannelGroup.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Audio FMOD plugin Channel group.
*
*  Audio FMOD plugin Channel group.
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
#include "shPrerequisitesFMODAudio.h"
#include "shChannelGroup.h"

namespace FMOD
{
  class ChannelGroup;
}

namespace shEngineSDK {
/**
*  @brief Audio FMOD plugin Channel group.
*/
class FMODChannelGroup : public ChannelGroup
{
 public:
  /**
  *  @brief Default constructor.
  */
  FMODChannelGroup() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~FMODChannelGroup();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief FMOD ChannelGroup.
  */
  FMOD::ChannelGroup* m_channelGroup = nullptr;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FMODChannelGroup::~FMODChannelGroup()
{
#if USING_FMOD
  safeRelease(m_channelGroup);
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}
}
