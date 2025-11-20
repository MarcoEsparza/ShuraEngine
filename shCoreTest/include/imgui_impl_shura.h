/*****************************************************************************/
/*
*  @file    imgui_impl_shura.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/20
*  @brief   ImGui implementation for Shura Engine.
*
*  ImGui implementation for Shura Engine.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include <shPrerequisitesCore.h>
#include <shScreen.h>
#include <shVector2.h>
#include <shEventEnums.h>

#define IMGUI_ENABLE_VIEWPORTS
#define IMGUI_HAS_DOCK

#include "imgui.h"
#include "imgui_internal.h"

namespace shEngineSDK {

#ifndef IMGUI_DISABLE

namespace FONT_ICONS {
enum E {
  kMusicNote = 0xe800,
  kVideoCamera = 0xe801,
  kVideo = 0xe802,
  kStar = 0xe803,
  kEye = 0xe804,
  kEyeOff = 0xe805,
  kTag = 0xe806,
  kTh = 0xe807,
  kHelpCircled = 0xe808,
  kInfoCircled = 0xe809,
  kHome = 0xe80a,
  kLink = 0xe80b,
  kAttach = 0xe80c,
  kLock = 0xe80d,
  kPin = 0xe80e,
  kBookmark = 0xe80f,
  kFolder = 0xe810,
  kFolderOpen = 0xe811,
  kCogAlt = 0xe812,
  kWrench = 0xe813,
  kVolumeUp = 0xe814,
  kShare1 = 0xe815,
  kClock = 0xe816,
  kBlocked = 0xe817,
  kZoomIn = 0xe818,
  kZoomOut = 0xe819,
  kPlay = 0xe81a,
  kStop = 0xe81b,
  kPause = 0xe81c,
  kToEnd = 0xe81d,
  kToEndAlt = 0xe81e,
  kToStart = 0xe81f,
  kToStartAlt = 0xe820,
  kFastForward = 0xe821,
  kFastBackward = 0xe822,
  kEject = 0xe823,
  kInbox = 0xe824,
  kGlobe = 0xe825,
  kCloud = 0xe826,
  kFlash = 0xe827,
  kSearch = 0xe828,
  kPicture = 0xe829,
  kFont = 0xe82a,
  kBold = 0xe82b,
  kItalic = 0xe82c,
  kTextHeight = 0xe82d,
  kTextWidth = 0xe82e,
  kAlignLeft = 0xe82f,
  kAlignCenter = 0xe830,
  kAlignRight = 0xe831,
  kAlignJustify = 0xe832,
  kList = 0xe833,
  kIndentLeft = 0xe834,
  kIndentRight = 0xe835,
  kScissors = 0xe836,
  kBriefcase = 0xe837,
  kOff = 0xe838,
  kChartBar = 0xe839,
  kComment = 0xe83a,
  kHeart = 0xe83b,
  kForward = 0xe83c,
  kPencil = 0xe83d,
  kBasket = 0xe83e,
  kAward = 0xe83f,
  kRetweet = 0xe840,
  kCW = 0xe841,
  kCCW = 0xe842,
  kArrowsCW = 0xe843,
  kFlag = 0xe844,
  kResizeFull = 0xe845,
  kResizeSmall = 0xe846,
  kResizeVertical = 0xe847,
  kResizeHorizontal = 0xe848,
  kPitch = 0xe849,
  kFloppy = 0xe84a,
  kHeadphones = 0xe84b,
  kCloudSun = 0xe84c,
  kWrench1 = 0xe84d,
  kShuffle = 0xe84e,
  kTH = 0xe84f,
  kPicture1 = 0xe850,
  kFlowSplit = 0xe851,
  kFlowMerge = 0xe852,
  kFlowParallel = 0xe853,
  kFlowCross = 0xe854,
  kKey = 0xe855,
  kMove = 0xf047,
  kLinkExternal = 0xf08e,
  kCheckEmpty = 0xf096,
  kGithubCicled = 0xf09b,
  kRSS = 0xf09e,
  kLeftCircled = 0xf0a8,
  kRightCircled = 0xf0a9,
  kUpCircled = 0xf0aa,
  kDownCircled = 0xf0ab,
  kFilter = 0xf0b0,
  kResizeFullAlt = 0xf0b2,
  kBeaker = 0xf0c3,
  kDocs = 0xf0c5,
  kMenu = 0xf0c9,
  kListBullet = 0xf0ca,
  kListNumbered = 0xf0cb,
  kUnderline = 0xf0cd,
  kMagicWand = 0xf0d0,
  kMailAlt = 0xf0e0,
  kSiteMap = 0xf0e8,
  kPaste = 0xf0ea,
  kLightBulb = 0xf0eb,
  kBellAlt = 0xf0f3,
  kPlusSquared = 0xf0fe,
  kAngleDoubleLeft = 0xf100,
  kAngleDoubleRight = 0xf101,
  kDesktop = 0xf108,
  kLaptop = 0xf109,
  kTablet = 0xf10a,
  kReply = 0xf122,
  kGamepad = 0xf11b,
  kKeyboard = 0xf11c,
  kCode = 0xf121,
  kCrop = 0xf125,
  kFork = 0xf126,
  kUnlink = 0xf127,
  kEraser = 0xf12d,
  kMic = 0xf130,
  kMute = 0xf131,
  kShield = 0xf132,
  kCalendarEmpty = 0xf133,
  kLockOpenAlt = 0xf13e,
  kEllipsis = 0xf141,
  kEllipsisVert = 0xf142,
  kMinusSquared = 0xf146,
  kOKSquared = 0xf14a,
  kDocInv = 0xf15b,
  kDocTextInv = 0xf15c,
  kThumbsUpAlt = 0xf164,
  kThumbsDownAlt = 0xf165,
  kDown = 0xf175,
  kUp = 0xf176,
  kLeft = 0xf177,
  kRight = 0xf178,
  kApple = 0xf179,
  kWindows = 0xf17a,
  kAndroid = 0xf17b,
  kLinux = 0xf17c,
  kMale = 0xf183,
  kSun = 0xf185,
  kBox = 0xf187,
  kBug = 0xf188,
  kDotCircled = 0xf192,
  kDelicious = 0xf1a5,
  kLanguage = 0xf1ab,
  kChild = 0xf1ae,
  kCube = 0xf1b2,
  kCubes = 0xf1b3,
  kSteam = 0xf1b6,
  kSteamSquared = 0xf1b7,
  kRecycle = 0xf1b8,
  kDatabase = 0xf1c0,
  kFileImage = 0xf1c5,
  kFileCode = 0xf1c9,
  kHistory = 0xf1da,
  kShare = 0xf1e0,
  kPlug = 0xf1e6,
  kWifi = 0xf1eb,
  kBellOff = 0xf1f6,
  kTrash = 0xf1f8,
  kCopyright = 0xf1f9,
  kEyedropper = 0xf1fb,
  kBrush = 0xf1fc,
  kToogleOff = 0xf204,
  kToogleOn = 0xf205,
  kBattery4 = 0xf240,
  kBattery3 = 0xf241,
  kBattery2 = 0xf242,
  kBattery0 = 0xf244,
  kICursor = 0xf246,
  kObjectGroup = 0xf247,
  kObjectUngroup = 0xf248,
  kClone = 0xf24d,
  kHourglass = 0xf254,
  kTrademark = 0xf25c,
  kRegistered = 0xf25d,
  kUSB = 0xf287,
  kHashtag = 0xf292,
  kBluethooth = 0xf293,
  kUniversalAccess = 0xf29a,
  kWindowMaximize = 0xf2d0,
  kWindowMinimize = 0xf2d1,
  kWindowRestore = 0xf2d2,
  kWindowClose = 0xf2d3,
  kMicrochip = 0xf2db,
  kSpread = 0xf527,

  kIconMax
};
}

/**
*  @brief Initialize ImGui implementation for Shura Engine.
*/
bool
ImGui_ImplShura_Init(const WPtr<Screen>& screenHandle);

void
ImGui_ImplShura_Resize(const Vector2& newSize);

/**
*  @brief Render call ImGui implementation for Shura Engine.
* 
*  @param ImDrawData* drawData
*/
void
ImGui_ImplShura_RenderDrawData(ImDrawData* drawData);

/**
*  @brief Creates necesary objects ImGui implementation for Shura Engine.
*/
bool
ImGui_ImplShura_CreateDeviceObjects();

/**
*  @brief Deletes objects ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_InvalidateDeviceObjects();

/**
*  @brief Updates ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_NewFrame();

void
ImGui_ImplShura_AddKeyEvent(const KEY::E key,
                            const bool bPressed,
                            const ModifierState modifier);

void
ImGui_ImplShura_AddMousePosEvent(const Vector2& mousePos);

void
ImGui_ImplShura_AddMouseButtonEvent(const bool bClicked, MOUSE_INPUT::E button);

void
ImGui_ImplShura_AddMouseWheelEvent(const float wheel,
                   const float hWheel);

/**
*  @brief Shuts down ImGui implementation for Shura Engine.
*/
void
ImGui_ImplShura_Shutdown();

#endif
}
