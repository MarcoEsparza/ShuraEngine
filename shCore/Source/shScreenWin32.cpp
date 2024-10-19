/*************************************************************/
/*
*  @file    shScreenWin32.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/19
*  @brief   Base screen
*
*  Base screen
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shScreen.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
LRESULT CALLBACK
windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool
Screen::init(ScreenDesc& desc, SPtr<ScreenEventHandle> eventHandler)
{
  m_eventQueue = eventHandler;
  m_width = desc.width;
  m_height = desc.height;
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  WNDCLASSEX wc = { };
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = windowProc;
  wc.cbWndExtra = 0;
  wc.cbClsExtra = 0;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = desc.name.c_str();
  wc.cbWndExtra = sizeof(ScreenEventHandle*);

  if (desc.iconPath != "") {
    HICON hIcon = reinterpret_cast<HICON>(::LoadImage(hInstance,
                                          desc.iconPath.c_str(),
                                          IMAGE_ICON,
                                          32,
                                          32,
                                          LR_LOADFROMFILE));
    if (!hIcon) {
      MessageBox(nullptr, "Couldn't load image", "Error", MB_ICONERROR);
    }

    wc.hIcon = hIcon;
    wc.hIconSm = hIcon;
  }
  else {
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
  }

  SH_ASSERT(RegisterClassEx(&wc));

  uint32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
  uint32 screenHeight = GetSystemMetrics(SM_CYSCREEN);

  if (desc.fullscreen) {
    DEVMODE dmScreenSettings;
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = screenWidth;
    dmScreenSettings.dmPelsHeight = screenHeight;
    dmScreenSettings.dmBitsPerPel = 32;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if ((desc.width != screenWidth) && (desc.height != screenHeight)) {
      if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        // Stay windowed
      }
    }
  }

  m_screenHandle = CreateWindowEx(0,
                                  wc.lpszClassName,
                                  desc.title.c_str(),
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  m_width,
                                  m_height,
                                  nullptr,
                                  nullptr,
                                  hInstance,
                                  nullptr);

  DWORD error = GetLastError();

  SH_ASSERT(m_screenHandle != nullptr);

  if (desc.visible) {
    ShowWindow(reinterpret_cast<HWND>(m_screenHandle), SW_SHOW);

    SetForegroundWindow(reinterpret_cast<HWND>(m_screenHandle));

    SetFocus(reinterpret_cast<HWND>(m_screenHandle));

    SetWindowLongPtrW(reinterpret_cast<HWND>(m_screenHandle),
                                             0,
                                             reinterpret_cast<LONG_PTR>(m_eventQueue.get()));
  }

  return true;
}

void
Screen::close()
{
  DestroyWindow(reinterpret_cast<HWND>(m_screenHandle));
  PostQuitMessage(0);
}
}

#endif
