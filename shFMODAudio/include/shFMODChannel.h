/*****************************************************************************/
/*
*  @file    shFMODChannel.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Audio FMOD plugin channel.
*
*  Audio FMOD plugin channel.
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
#include "shChannel.h"

namespace FMOD
{
  class Channel;
}

namespace shEngineSDK {
/**
*  @brief Audio FMOD plugin channel.
*/
class FMODChannel : public Channel
{
 public:
  /**
  *  @brief Default constructor.
  */
  FMODChannel() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~FMODChannel();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief FMOD Channel.
  */
  FMOD::Channel* m_channel = nullptr;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FMODChannel::~FMODChannel()
{
#if USING_FMOD
  m_channel->stop();
#else
  SH_ASSERT("FMOD is not enabled in this build");
#endif // USING_FMOD
}
}
