/*****************************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Here is the main to test the window initialize.
*
*  Here is the main to test the window initialize.
* 
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shRendererApp.h"
#include <shScreen.h>

using namespace shEngineSDK;

int main(int argc, const char** argv)
{
  String graphicsName = "DX11";

  if (argc == 2) {
    graphicsName = argv[1];
  }

  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "Renderer App";
  desc.visible = true;
  desc.fullscreen = false;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 1920;
  desc.height = 1080;
  desc.iconPath = "resources/ShuraIcon.ico";

  GRAPHIC_API::E api = GRAPHIC_API::kDX11;


  if (graphicsName == "DX11") {
    api = GRAPHIC_API::kDX11;
  }
  else if (graphicsName == "OGL") {
    api = GRAPHIC_API::kOGL;
  }

  RendererApp app(desc, api);
  app.run();

  return 0;
}
