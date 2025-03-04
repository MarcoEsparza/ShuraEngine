/*************************************************************/
/*
*  @file    shPhysicsTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
*  @brief   Main for physics project.
*
*  Main for physics project.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesPhysics.h"
#include "shPrerequisitesCore.h"

#include "shPhysicsApp.h"

using namespace shEngineSDK;

int main()
{
  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "Graphics Test";
  desc.visible = true;
  desc.fullscreen = false;
  desc.anitaliasing = false;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 800;
  desc.height = 800;
  desc.iconPath = "resources/ShuraIcon.ico";

  PhysicsApp app(desc);
  app.run();

  return 0;
}
