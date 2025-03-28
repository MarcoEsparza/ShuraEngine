/*****************************************************************************/
/*
*  @file    shChannelGroup.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Audio Channel group base class.
*
*  Audio Channel group base class.
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

namespace shEngineSDK {
class Channel;

/**
*  @brief Audio Channel group base class.
*/
class ChannelGroup
{
 public:
  /**
  *  @brief Default constructor.
  */
  ChannelGroup() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~ChannelGroup() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Channels array.
  */
  Vector<SPtr<Channel>> m_channels;
};
}
