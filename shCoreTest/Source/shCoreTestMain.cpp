/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
*  @brief   Here is the main to test the window initialize.
*
*  Here is the main to test the window initialize.
* 
*  @bug     Camera movement not working properly.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"
#include "shBaseApp.h"

using namespace shEngineSDK;

int main(int argc, const char** argv)
{
  if (argc < 2) {
    return -1;
  }

  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "Graphics Test";
  desc.visible = true;
  desc.fullscreen = false;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 1000;
  desc.height = 600;
  desc.iconPath = "M:/ShuraEngine/Repo/ShuraEngine/resources/ShuraIcon.ico";

  GRAPHIC_API::E api = GRAPHIC_API::kDX11;

  String graphicsName = argv[1];

  if (graphicsName == "DX11") {
    api = GRAPHIC_API::kDX11;
  }
  else if (graphicsName == "OGL") {
    api = GRAPHIC_API::kOGL;
  }

  BaseApp app(desc, api);
  app.run();

  return 0;
}
