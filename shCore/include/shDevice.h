/*************************************************************/
/*
*  @file    shDevice.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Device and Device Context classes.
*
*  Engine Device and Device Context classes.
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
*  @brief Engine Device class.
*/
class Device
{
 public:
  /**
  *  @brief Default constructor.
  */
  Device() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Device() = default;
};

/**
*  @brief Engine Device Context class.
*/
class DeviceContext
{
public:
  /**
  *  @brief Default constructor.
  */
  DeviceContext() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~DeviceContext() = default;
};
}
