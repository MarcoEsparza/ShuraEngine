/*****************************************************************************/
/*
*  @file    shChannel.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/27
*  @brief   Audio channel base class.
*
*  Audio channel base class.
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
/**
*  @brief Audio channel base class.
*/
class Channel
{
 public:
  /**
  *  @breif Default constructor.
  */
  Channel() = default;

  /**
  *  @breif Default destructor.
  */
  virtual ~Channel() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @breif Channel index in group.
  */
  uint32 index = 0;
};
}
