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
Screen::init(const ScreenDesc& desc, const SPtr<ScreenEventHandle> eventHandler)
{
  m_eventQueue = eventHandler;
  m_width = desc.width;
  m_height = desc.height;
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  WNDCLASSEXA wc = { };
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
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
  }

  SH_ASSERT(RegisterClassExA(&wc));

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

  m_screenHandle = CreateWindowExA(0,
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

  SH_ASSERT(m_screenHandle != nullptr);

  m_eventQueue->m_initialized = true;

  if (desc.visible) {
    ShowWindow(m_screenHandle, SW_SHOW);

    SetForegroundWindow(m_screenHandle);

    SetFocus(m_screenHandle);

    SetWindowLongPtrA(m_screenHandle,
                      0,
                      reinterpret_cast<LONG_PTR>(m_eventQueue.get()));

    SetWindowLongPtrA(m_screenHandle,
                      1,
                      reinterpret_cast<LONG_PTR>(&m_prevMousePos));
  }

  return true;
}

void
Screen::close()
{
  DestroyWindow(m_screenHandle);
  PostQuitMessage(0);
}
}

#endif
