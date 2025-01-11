/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/06
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

  String dllName;

  String graphicsName = argv[1];

#ifdef SH_DEBUG_MODE

  if (graphicsName == "DX11") {
    dllName = "shDX11Graphicsd";
  }
  else if (graphicsName == "OGL") {
    dllName = "shOGLGraphicsd";
  }

#else

  if (graphicsName == "DX11") {
    dllName = "shDX11Graphics";
  }
  else if (graphicsName == "OGL") {
    dllName = "shOGLGraphics";
  }

#endif

  BaseApp app(desc, dllName);
  app.run();

  return 0;
}
