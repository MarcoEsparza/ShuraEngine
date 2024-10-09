/*************************************************************/
/*
*  @file    shScreenEventHandle.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/08
*  @brief   
*
*  
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shScreenEventHandle.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
void
ScreenEventHandle::update()
{
  MSG msg = { };

  for (;;) {
    if (m_processingMode == shProcessingMode::E::kPoll) {
      if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        break;
      }
    }
    else {
      GetMessage(&msg, NULL, 0, 0);
    }

    if (msg.message == WM_QUIT) {
      return;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

Queue<Event>
ScreenEventHandle::getQueue()
{
  return m_queue;
}

bool
ScreenEventHandle::empty()
{
  return m_queue.empty();
}

Event
ScreenEventHandle::front()
{
  return m_queue.front();
}

void
ScreenEventHandle::pop()
{
  m_queue.pop();
}

void
ScreenEventHandle::setProcessingNode(shProcessingMode::E mode)
{
  m_processingMode = mode;
}

SIZE_T
ScreenEventHandle::getSize()
{
  return m_queue.size();
}

// TODO: finish this
long long
ScreenEventHandle::pushEvent(MSG msg, WPtr<Screen> screen)
{
  auto pScreen = screen.lock();

  uint32 message = msg.message;
  LRESULT result = 0;
  RECT currenScreenRect = { -1,-1,-1,-1 };

  Event ev = Event(shEventType::E::kNone, pScreen);

  switch (message) {
  case WM_CREATE:
  {
    ev = Event(shEventType::E::kCreate, pScreen);
    break;
  }
  case WM_PAINT:
  {

    break;
  }
  case WM_ERASEBKGND:
  {
    break;
  }
  case WM_CLOSE:
  case WM_DESTROY:
  {
    ev = Event(shEventType::E::kClose, pScreen);
    break;
  }
  case WM_SETFOCUS:
  {

    break;
  }
  case WM_KILLFOCUS:
  {

    break;
  }
  case WM_MOUSEWHEEL:
  {

    break;
  }
  case WM_LBUTTONDOWN:
  {

    break;
  }
  case WM_LBUTTONDBLCLK:
  {

    break;
  }
  case WM_LBUTTONUP:
  {

    break;
  }
  case WM_MBUTTONDOWN:
  {

    break;
  }
  case WM_MBUTTONDBLCLK:
  {

    break;
  }
  case WM_MBUTTONUP:
  {

    break;
  }
  case WM_RBUTTONDOWN:
  {

    break;
  }
  case WM_RBUTTONDBLCLK:
  {

    break;
  }
  case WM_RBUTTONUP:
  {

    break;
  }
  case WM_XBUTTONDOWN:
  {

    break;
  }
  case WM_XBUTTONDBLCLK:
  {

    break;
  }
  case WM_XBUTTONUP:
  {

    break;
  }
  case WM_INPUT:
  {

    break;
  }
  case WM_MOUSEMOVE:
  {

    break;
  }
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_CHAR:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  {
    shKey::E key;

    switch (msg.wParam)
    {
    case VK_ESCAPE:
      key = shKey::E::kEscape;
      break;
    case 0x30:
      key = shKey::E::kNum0;
      break;
    }
    break;
  }
  case WM_SIZE:
  {

    break;
  }
  case WM_SIZING:
  {

    break;
  }
  case WM_MOVING:
  {

    break;
  }
  case WM_NCHITTEST:
  {

    break;
  }
  case WM_DPICHANGED:
  {

    break;
  }
  case WM_NCCALCSIZE:
  {

    break;
  }
  case WM_GETMINMAXINFO:
  {

    break;
  }
  default:
    break;
  }

  return result;
}
}

#endif
