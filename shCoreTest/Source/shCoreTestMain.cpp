/*************************************************************/
/*
*  @file    shCoreTestMain.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Here is the main to test the window initialize.
*
*  Here is the main to test the window initialize.
* 
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"

using namespace shEngineSDK;

int main()
{
  ScreenDesc desc;
  desc.name = "Test";
  desc.title = "My title";
  desc.visible = true;
  desc.positionX = 0;
  desc.positionY = 0;
  desc.width = 1000;
  desc.height = 600;
  desc.iconPath = "resources/ShuraIcon.ico";

  Screen mainScreen;
  SPtr<ScreenEventHandle> eventQ = make_shared<ScreenEventHandle>();

  if (!mainScreen.init(desc, eventQ)) {
    return -1;
  }

  bool isRunning = true;

  while (isRunning) {
    eventQ->update();

    while (!eventQ->empty()) {
      auto ev = eventQ->front();

      if (ev.type == shEventType::E::kMouseInput) {
        const MouseInputData mouse = ev.data.mouseInput;
      }
      if (ev.type == shEventType::E::kClose) {
        mainScreen.close();
        isRunning = false;
      }

      eventQ->pop();
    }
  }

  return 0;
}
