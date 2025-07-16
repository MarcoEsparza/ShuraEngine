/*************************************************************/
/*
*  @file    shGUI.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/15
*  @brief   Graphical User Interface (GUI) system for editor.
*
*  Graphical User Interface (GUI) system for editor.
*
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
class Screen;

/**
*  @brief Graphical User Interface (GUI) system for editor.
*/
class GUI
{
 public:
  /**
  *  @brief Default constructor.
  */
  GUI() = default;

  /**
  *  @brief Default destructor.
  */
  ~GUI() = default;

  /**
  *  @brief Initialize the GUI system.
  */
  void
  init(const WPtr<Screen> pScreen);

  /**
  *  @brief Shutdown the GUI system.
  */
  void
  shutdown();

  /**
  *  @brief Render the GUI.
  */
  void
  render();

  /**
  *  @brief Update the GUI.
  */
  void
  update();
};
}
