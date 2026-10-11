// Original: d:\build\custom\temp\code\client\projectg\clientsetting.cpp
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
// This object keeps the out-of-line copy of MsgObject's default constructor.
#define CLIENTSETTING_ACTOR_INLINE
#define CLIENTSETTING_SHAREDDOC_INLINE
#define CLIENTSETTING_WMATH_INLINE
#include "NetworkSystem.h"
#include "ClientSetting.h"
#include "actor.h"
#include "InputManager.h"
#include "Packet.h"
#include "SoundManager.h"
#include "Shadow.h"
#include "wlocalize.h"
#include "wutil.h"
#include "wvideo.h"
#include "wdevmng.h"
#include "wview.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "frwnd.h"
#include "ProjectG.h"
#include "Browser.h"
#include "ChatMsg.h"
#include "scenemanager.h"
#include "polysoup.h"
#include "GolfDoc.h"
#include "frform.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frwndmanager.h"
#include <string>

#include <stdio.h>
#include <string.h>

// commonutil.cpp:1037 (not declared in commonutil.h yet)
void LimitText(char* str, int limit);
// The client version string (defined elsewhere).
extern char PY_PUBLIC_VERSION[];

// clientsetting.cpp:33
bool sOption::IsSupportShadow(void) {
	return sbRtSupport;
}

// clientsetting.cpp:38
bool sOption::IsSupportReflection(void) {
	return sbRtSupport && sbClipPlaneSupport && sbVsSupport && sbPsSupport;
}

// clientsetting.cpp:47
bool sOption::IsSupportLod(void) {
	return sbRtSupport;
}

// clientsetting.cpp:103
sOption::sOption() {
	m_windowed = 0;
	m_fillScreen = 0;
	m_wideMode = 0;
	m_overallLevel = -1;
	m_minScreenEnabled = 0;
	m_minScreenWidth = 800;
	m_minScreenHeight = 600;
	m_screenWidth = 800;
	m_screenHeight = 600;
	m_screenBpp = 16;
	m_tnlLevel = 1;
	m_mipmapLevel = 0;
	m_mipmapEnabled = 0;
	m_mipmapParam1 = 1;
	m_mipmapParam2 = 2;
	m_particleLevel = 1;
	m_shadowEnabled = 1;
	m_shadowmapEnabled = 1;
	m_reflectionEnabled = 1;
	m_lodEnabled = 0;
	m_npcEnabled = 1;
	m_gamma = 1.3f;
	m_audioOverall = 0;
	m_mssEnabled = 1;
	m_mssFrequency = 44100;
	m_mssBits = 16;
	m_mssChannels = 2;
	m_mssHwSoundEnabled = 1;
	m_mssBalance = 128;
	m_speakerType = 0;
	m_sfxVolume = 1.0f;
	m_bgmVolume = 1.0f;
	m_mouseSensitivity = 1.0f;
	m_whisperAllowed = 1;
	m_invitationAllowed = 1;
	m_friendConfirmNeeded = 1;
	m_chatWinTransparent = 1;
	m_uiEffect = 1;
	m_lastLoginID[0] = 0;
	m_identity = 0;
	m_chatLineExtended = 0;
	m_userSort = 0;
	m_pplEnable = 1;
	m_pplSize = 0;
	m_powerGauge = 1;
	m_captureLogo = 1;
	m_restore = 0;
	m_chatWndUp = 0;
	m_underExtBtn = 0;
	m_avatarNewBie = 1;
	m_captureHideGUI = 0;
	m_captureHidePI = 0;
	m_terrainTooltip = 1;
	m_cutinFlag = 1;

	for (int i = 0; i < 8; i++)
		m_macroText[i][0] = 0;
	m_saveMacros = 1;
}

// clientsetting.cpp:185
sOption::~sOption() {
}

// clientsetting.cpp:189
void sOption::vResetByOverall(const char* text) {
	if (strcmp(text, "\xB3\xF4\xC0\xBD") == 0) { // "High"
		m_overallLevel = 0;
		m_minScreenEnabled = 0;
		m_screenWidth = 1024;
		m_screenHeight = 768;
		m_screenBpp = 32;
		m_mipmapLevel = 0;
		m_particleLevel = 0;
		m_shadowEnabled = IsSupportShadow() ? 1 : 0;
		m_shadowmapEnabled = 1;
		m_reflectionEnabled = IsSupportReflection() ? 1 : 0;
		m_lodEnabled = 0;
		m_npcEnabled = 1;
	} else if (strcmp(text, "\xC1\xDF\xB0\xA3") == 0) { // "Medium"
		m_overallLevel = 1;
		m_minScreenEnabled = 0;
		m_screenWidth = 800;
		m_screenHeight = 600;
		m_screenBpp = 32;
		m_mipmapLevel = 1;
		m_particleLevel = 1;
		m_shadowEnabled = IsSupportShadow() ? 1 : 0;
		m_shadowmapEnabled = 1;
		m_reflectionEnabled = IsSupportReflection() ? 1 : 0;
		m_lodEnabled = IsSupportLod() ? 1 : 0;
		m_npcEnabled = 1;
	} else if (strcmp(text, "\xB3\xB7\xC0\xBD") == 0) { // "Low"
		m_overallLevel = 2;
		m_minScreenEnabled = 0;
		m_screenWidth = 640;
		m_screenHeight = 480;
		m_screenBpp = 16;
		m_mipmapLevel = 2;
		m_particleLevel = 2;
		m_shadowEnabled = 0;
		m_shadowmapEnabled = 0;
		m_reflectionEnabled = 0;
		m_lodEnabled = IsSupportLod() ? 1 : 0;
		m_npcEnabled = 0;
	}
}

// clientsetting.cpp:238
void sOption::vSetOverallByVidOptions(void) {
	if (m_screenWidth == 1024 && m_screenHeight == 768 && m_screenBpp == 32 && m_mipmapLevel == 0 && m_particleLevel == 0
		&& m_shadowEnabled == 1 && m_shadowmapEnabled == 1 && m_reflectionEnabled == 1 && m_lodEnabled == 0 && m_npcEnabled == 1) {
		m_overallLevel = 0;
	} else if (m_screenWidth == 800 && m_screenHeight == 600 && m_screenBpp == 32 && m_mipmapLevel == 1 && m_particleLevel == 1
		&& m_shadowEnabled == 1 && m_shadowmapEnabled == 1 && m_reflectionEnabled == 1 && m_lodEnabled == 1 && m_npcEnabled == 1) {
		m_overallLevel = 1;
	} else if (m_screenWidth == 640 && m_screenHeight == 480 && m_screenBpp == 16 && m_mipmapLevel == 2 && m_particleLevel == 2
		&& m_shadowEnabled == 0 && m_shadowmapEnabled == 0 && m_reflectionEnabled == 0 && m_lodEnabled == 1 && m_npcEnabled == 0) {
		m_overallLevel = 2;
	} else {
		m_overallLevel = -1;
	}
}

// clientsetting.cpp:285
const char* sOption::vGetOverallText(int level) {
	return GetThreeLvlText(level);
}

// clientsetting.cpp:290
int sOption::vGetOverallLvl(const char* text) {
	return vGetLvl3FromText(text);
}

// clientsetting.cpp:295
const char* sOption::vGetTnlText(int level) {
	if (level == 0)
		return "\xBC\xD2\xC7\xC1\xC6\xAE\xBF\xFE\xBE\xEE TnL"; // "Software TnL"
	else if (level == 1)
		return "\xC7\xCF\xB5\xE5\xBF\xFE\xBE\xEE TnL"; // "Hardware TnL"

	return "";
}

// clientsetting.cpp:308
int sOption::vGetTnlLvl(const char* text) {
	if (strcmp(text, "\xBC\xD2\xC7\xC1\xC6\xAE\xBF\xFE\xBE\xEE TnL") == 0)
		return 0;
	else if (strcmp(text, "\xC7\xCF\xB5\xE5\xBF\xFE\xBE\xEE TnL") == 0)
		return 1;

	return -1;
}

// clientsetting.cpp:321
const char* sOption::vGetTexResText(int level) {
	return GetThreeLvlText(level);
}

// clientsetting.cpp:326
int sOption::vGetTexResLvl(const char* text) {
	return vGetLvl3FromText(text);
}

// clientsetting.cpp:331
const char* sOption::vGetParticleText(int level) {
	return GetThreeLvlText(level);
}

// clientsetting.cpp:336
int sOption::vGetParticleLvl(const char* text) {
	return vGetLvl3FromText(text);
}

// clientsetting.cpp:341
const char* sOption::aGetOverallText(int level) {
	return GetThreeLvlText(level);
}

// clientsetting.cpp:346
int sOption::aGetOverallLvl(const char* text) {
	return vGetLvl3FromText(text);
}

// clientsetting.cpp:351
const char* sOption::aGetSpeakerText(int type) {
	switch (type) {
	case 0: return "2\xC3\xA4\xB3\xCE \xBD\xBA\xC7\xC7\xC4\xBF";   // "2 channel speakers"
	case 1: return "\xC7\xEC\xB5\xE5\xC6\xF9";                     // "Headphones"
	case 2: return "\xBC\xAD\xB6\xF3\xBF\xEE\xB5\xE5";             // "Surround"
	case 3: return "4\xC3\xA4\xB3\xCE \xBD\xBA\xC7\xC7\xC4\xBF";   // "4 channel speakers"
	case 4: return "5.1\xC3\xA4\xB3\xCE \xBD\xBA\xC7\xC7\xC4\xBF"; // "5.1 channel speakers"
	case 5: return "7.1\xC3\xA4\xB3\xCE \xBD\xBA\xC7\xC7\xC4\xBF"; // "7.1 channel speakers"
	}

	return "";
}

// clientsetting.cpp:366
int sOption::aGetSpeakerLvl(const char* text) {
	for (int i = 0; i < 6; i++) {
		if (strcmp(text, aGetSpeakerText(i)) == 0)
			return i;
	}

	return 0;
}

// clientsetting.cpp:377
const char* sOption::GetThreeLvlText(int level) {
	switch (level) {
	case 0: return "\xB3\xF4\xC0\xBD"; // "High"
	case 1: return "\xC1\xDF\xB0\xA3"; // "Medium"
	case 2: return "\xB3\xB7\xC0\xBD"; // "Low"
	}

	return "\xBB\xE7\xBF\xEB\xC0\xDA"; // "Custom"
}

// clientsetting.cpp:389
const char* sOption::GetTwoLvlText(int level) {
	switch (level) {
	case 0: return "\xB3\xF4\xC0\xBD"; // "High"
	case 1: return "\xB3\xB7\xC0\xBD"; // "Low"
	}

	return "\xBB\xE7\xBF\xEB\xC0\xDA"; // "Custom"
}

// clientsetting.cpp:400
int sOption::vGetLvl3FromText(const char* text) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(text, GetThreeLvlText(i)) == 0)
			return i;
	}

	return -1;
}

// The registry keys and their default values (clientsetting.cpp:455-518). The
// original initializes the tail from mMacro1's text on by code (sub_00989770,
// not reproduced yet: cl folds this form into static data).
static const char* const s_szDefaultMacro1 = "\xb8\xb8\xb3\xaa\xbc\xad \xb9\xdd\xb0\xa9\xbd\xc0\xb4\xcf\xb4\xd9.(\xc0\xce\xbb\xe7)";
static const char* const s_szDefaultMacro2 = "\xc7\xcf\xc0\xcc\xb7\xee~ \xb9\xdd\xb0\xa1\xbf\xf6\xbf\xe4~(\xc0\xae\xc5\xa9)";
static const char* const s_szDefaultMacro3 = "\xb7\xb9\xb5\xf0\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4~(\xc8\xad\xb3\xb2)";
static const char* const s_szDefaultMacro4 = "\xb3\xaa\xc0\xcc\xbd\xba~ \xbc\xa6!(\xb9\xda\xbc\xf6)";
static const char* const s_szDefaultMacro5 = "\xb5\xe9\xbe\xee\xb0\xa1\xb6\xf3~ \xb5\xe9\xbe\xee\xb0\xa1\xb6\xf3~ \xb5\xe9\xbe\xee\xb0\xa1\xb6\xf3~(\xb1\xe4\xc0\xe5)";
static const char* const s_szDefaultMacro6 = "\xc6\xc4\xc0\xcc\xc6\xc3~ \xc8\xfb\xb3\xbb\xbc\xbc\xbf\xe4~(\xc7\xec\xc7\xec)";
static const char* const s_szDefaultMacro7 = "\xbe\xd1! \xbd\xc7\xbc\xf6..(\xbe\xfb\xbe\xfb)";
static const char* const s_szDefaultMacro8 = "\xbc\xf6\xb0\xed\xc7\xcf\xbc\xcc\xbe\xee\xbf\xe4~(\xc0\xce\xbb\xe7)";

// clientsetting.cpp:455
const char* COption::ms_szReg[][2] = {
	{ "Ver", "1.xx" },
	{ "Option\\vWindowMode", "0" },
	{ "Option\\vFillMode", "0" },
	{ "Option\\vWideMode", "0" },
	{ "Option\\vOverall", "-1" },
	{ "Option\\vScreenWidth", "800" },
	{ "Option\\vScreenHeight", "600" },
	{ "Option\\vScreenColor", "32" },
	{ "Option\\vTnLMode", "1" },
	{ "Option\\Mip\\vDDSRes", "0" },
	{ "Option\\Mip\\vUseMipmap", "0" },
	{ "Option\\Mip\\vMaxMipLvl", "1" },
	{ "Option\\Mip\\vMipCreateFilter", "2" },
	{ "Option\\vEffectLevel", "1" },
	{ "Option\\vProjectionShadow", "1" },
	{ "Option\\vShadowmap", "1" },
	{ "Option\\vReflection", "0" },
	{ "Option\\vLod", "0" },
	{ "Option\\vNpc", "1" },
	{ "Option\\vCoordX", "0" },
	{ "Option\\vCoordY", "0" },
	{ "Option\\vGamma", "1.3" },
	{ "Option\\aMssOn", "1" },
	{ "Option\\aOverall", "0" },
	{ "Option\\aMssFreq", "44100" },
	{ "Option\\aMssBits", "16" },
	{ "Option\\aMssChannels", "2" },
	{ "Option\\aMssEnableHwSound", "1" },
	{ "Option\\aMssBalance", "128" },
	{ "Option\\aMssSpeaker", "0" },
	{ "Option\\aSfxVolume", "1.00" },
	{ "Option\\aBGMVolume", "1.00" },
	{ "Option\\gMouseSensitivity", "1.00" },
	{ "Option\\gWhisper", "1" },
	{ "Option\\gInvitation", "1" },
	{ "Option\\gFriendConfirm", "1" },
	{ "Option\\gTransChatWin", "1" },
	{ "Option\\gUiEffect", "1" },
	{ "Option\\gLastLoginID", "" },
	{ "Option\\gIdentity", "0" },
	{ "Option\\gExtendChatLine", "0" },
	{ "Option\\gUserSort", "0" },
	{ "Option\\gPPL_Enable", "1" },
	{ "Option\\gPPL_Size", "0" },
	{ "Option\\gPowerGauge", "1" },
	{ "Option\\gCaptureLogo", "1" },
	{ "Option\\gRestore", "0" },
	{ "Option\\gChatWnd", "0" },
	{ "Option\\gUnderTabBtn", "0" },
	{ "Option\\gAvatarNewbie", "1" },
	{ "Option\\gCaptureHideGUI", "0" },
	{ "Option\\gCaptureHidePI", "0" },
	{ "Option\\gTerrainTooltip", "1" },
	{ "Option\\gCutinDisplay", "1" },
	{ "Option\\mMacro1", s_szDefaultMacro1 },
	{ "Option\\mMacro2", s_szDefaultMacro2 },
	{ "Option\\mMacro3", s_szDefaultMacro3 },
	{ "Option\\mMacro4", s_szDefaultMacro4 },
	{ "Option\\mMacro5", s_szDefaultMacro5 },
	{ "Option\\mMacro6", s_szDefaultMacro6 },
	{ "Option\\mMacro7", s_szDefaultMacro7 },
	{ "Option\\mMacro8", s_szDefaultMacro8 },
	{ "Option\\mSaveMacros", "1" },
};

// clientsetting.cpp:559
COption::COption(HWND hWnd, WDeviceManager* deviceManager, WResourceManager* resourceManager)
	: m_hWnd(hWnd), m_deviceManager(deviceManager), m_resourceManager(resourceManager) {
	InitVars();
	Init();
}

// clientsetting.cpp:565
COption::~COption() {
	SaveReg();

	m_displayModes.clear();
}

// clientsetting.cpp:572
void COption::SaveReg(void) {
	WriteReg_I(1, m_option.m_windowed);

	WriteReg_I(2, m_option.m_fillScreen);

	WriteReg_I(3, m_option.m_wideMode);
	WriteReg_I(4, m_option.m_overallLevel);
	WriteReg_I(5, m_option.m_screenWidth);
	WriteReg_I(6, m_option.m_screenHeight);
	WriteReg_I(7, m_option.m_screenBpp);
	WriteReg_I(8, m_option.m_tnlLevel);
	WriteReg_I(9, m_option.m_mipmapLevel);
	WriteReg_I(10, m_option.m_mipmapEnabled);
	WriteReg_I(11, m_option.m_mipmapParam1);
	WriteReg_I(12, m_option.m_mipmapParam2);
	WriteReg_I(13, m_option.m_particleLevel);
	WriteReg_I(14, m_option.m_shadowEnabled);
	WriteReg_I(15, m_option.m_shadowmapEnabled);
	WriteReg_I(16, m_option.m_reflectionEnabled);
	WriteReg_I(17, m_option.m_lodEnabled);
	WriteReg_I(18, m_option.m_npcEnabled);
	WriteReg_I(19, m_option.m_coordX);
	WriteReg_I(20, m_option.m_coordY);

	WriteReg_I(22, m_option.m_mssEnabled);
	WriteReg_I(23, m_option.m_audioOverall);
	WriteReg_I(24, m_option.m_mssFrequency);
	WriteReg_I(25, m_option.m_mssBits);
	WriteReg_I(26, m_option.m_mssChannels);
	WriteReg_I(27, m_option.m_mssHwSoundEnabled);
	WriteReg_I(28, m_option.m_mssBalance);
	WriteReg_I(29, m_option.m_speakerType);
	WriteReg_F(30, m_option.m_sfxVolume);
	WriteReg_F(31, m_option.m_bgmVolume);

	WriteReg_F(32, m_option.m_mouseSensitivity);
	WriteReg_I(33, m_option.m_whisperAllowed);
	WriteReg_I(34, m_option.m_invitationAllowed);
	WriteReg_I(35, m_option.m_friendConfirmNeeded);
	WriteReg_I(36, m_option.m_chatWinTransparent);
	WriteReg_I(37, m_option.m_uiEffect);
	WriteReg_S(38, m_option.m_lastLoginID);
	WriteReg_I(39, m_option.m_identity);
	WriteReg_I(40, m_option.m_chatLineExtended);
	WriteReg_I(41, m_option.m_userSort);
	WriteReg_I(42, m_option.m_pplEnable);
	WriteReg_I(43, m_option.m_pplSize);
	WriteReg_I(44, m_option.m_powerGauge);
	WriteReg_I(45, m_option.m_captureLogo);
	WriteReg_I(46, m_option.m_restore);
	WriteReg_I(47, m_option.m_chatWndUp);
	WriteReg_I(48, m_option.m_underExtBtn);
	WriteReg_I(49, m_option.m_avatarNewBie);
	WriteReg_I(50, m_option.m_captureHideGUI);
	WriteReg_I(51, m_option.m_captureHidePI);
	WriteReg_I(52, m_option.m_terrainTooltip);
	WriteReg_I(53, m_option.m_cutinFlag);

	WriteReg_S(54, m_option.m_macroText[0]);
	WriteReg_S(55, m_option.m_macroText[1]);
	WriteReg_S(56, m_option.m_macroText[2]);
	WriteReg_S(57, m_option.m_macroText[3]);
	WriteReg_S(58, m_option.m_macroText[4]);
	WriteReg_S(59, m_option.m_macroText[5]);
	WriteReg_S(60, m_option.m_macroText[6]);
	WriteReg_S(61, m_option.m_macroText[7]);
	WriteReg_I(62, m_option.m_saveMacros);
}

// clientsetting.cpp:658
void COption::InitVars(void) {
	m_videoDevice = NULL;
	m_audioManager = NULL;
	m_inputManager = NULL;
	m_changePending = false;
	m_bar_Y = m_option.m_screenHeight - 62.0f;
	m_swapMouseButton = false;
	m_quitWindowed = false;
	m_captureFormat = 0;
	m_defaultCharacter = 0;
	m_defaultCaddie = 0;
	m_aviWidth = 320;
	m_aviHeight = 240;
	m_aviFps = 10;
	m_startMode = 0;
#ifdef PG_LOBBY_COURSE
	// CMake only: the course whose map is the lobby background instead of
	// "wizcity" (CProjectG::Ready reads it through dGetLobbyCourse).
	m_lobbyCourse = PG_LOBBY_COURSE;
#else
	m_lobbyCourse = -1;
#endif
	m_map = 0;
	m_startHole = 1;
	m_replayMode = 0;
	m_numPlayers = 1;
	m_shotTimeLimitMode = 1;
	m_weather = 0;
	m_clickSystem = 0;
	m_vtxClrTestEnabled = 0;
	m_baseMeshTestEnabled = 0;
}

// clientsetting.cpp:693
bool COption::ReadReg(int index, char* buffer, int size) {
	if (index < 0 || index >= 63) {
		return false;
	}

	return m_registry.Read((char*)ms_szReg[index][0], buffer, size);
}

// clientsetting.cpp:705
bool COption::ScanReg_I(int index, int* value) {
	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		return false;
	}

	return m_registry.Scan((char*)ms_szReg[index][0], "%d", value) ? true : false;
}

// clientsetting.cpp:718
bool COption::ScanReg_F(int index, float* value) {
	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		return false;
	}

	return m_registry.Scan((char*)ms_szReg[index][0], "%f", value) ? true : false;
}

// clientsetting.cpp:731
bool COption::ScanReg_S(int index, char* value) {
	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		value[0] = 0;
		return false;
	}

	if (m_registry.Scan((char*)ms_szReg[index][0], "%s", value)) {
		return true;
	}

	value[0] = 0;
	return false;
}

// clientsetting.cpp:750
void COption::WriteReg(int index, const char* value) {
	if (index >= 0 && index < 63 && ms_szReg[index][0] && strlen(ms_szReg[index][0]) != 0) {
		m_registry.Write((char*)ms_szReg[index][0], value);
	}
}

// clientsetting.cpp:763
bool COption::WriteReg_I(int index, int value) {
	static char szValue[256];

	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		return false;
	}

	sprintf(szValue, "%d", value);
	m_registry.Write((char*)ms_szReg[index][0], szValue);

	return true;
}

// clientsetting.cpp:781
bool COption::WriteReg_F(int index, float value) {
	static char szValue[256];

	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		return false;
	}

	sprintf(szValue, "%.2f", value);
	m_registry.Write((char*)ms_szReg[index][0], szValue);

	return true;
}

// clientsetting.cpp:799
bool COption::WriteReg_S(int index, const char* value) {
	static char szValue[256];

	if (index < 0 || index >= 63 || !ms_szReg[index][0] || strlen(ms_szReg[index][0]) == 0) {
		return false;
	}

	sprintf(szValue, "%s", value);
	m_registry.Write((char*)ms_szReg[index][0], szValue);

	return true;
}

// clientsetting.cpp:818
void COption::Init(void) {
	char szLocation[256];
	szLocation[0] = '\0';

	if (IsLocalContent((localContentType_t)0x10)) {
		char* cmdLine = GetCommandLineA();

		if (strstr(cmdLine, "-godhfma")) {
			sprintf(szLocation, "%s\\%s", "HKEY_LOCAL_MACHINE", "Software\\Ntreev\\Pangya_QA");
		} else if (strstr(cmdLine, "-vkfks")) {
			strcpy(szLocation, "HKEY_LOCAL_MACHINE\\Software\\ParanGame\\Pangya");
		} else {
			sprintf(szLocation, "%s\\%s", "HKEY_LOCAL_MACHINE", "Software\\Ntreev\\Pangya_QA");
		}
	} else {
		sprintf(szLocation, "%s\\%s", "HKEY_LOCAL_MACHINE", "Software\\Ntreev\\Pangya_QA");
	}

	m_registry.SetLocation(szLocation);

	// Write the default of every value missing from the registry.
	char szValue[128] = { 0 };
	for (int i = 0; i != 63; i++) {
		if (strlen(ms_szReg[i][0]) != 0 && !ReadReg(i, szValue, sizeof(szValue))) {
			WriteReg(i, ms_szReg[i][1]);
		}
	}

	ScanReg_S(0, PY_PUBLIC_VERSION);

	ScanReg_I(1, &m_option.m_windowed);
	ScanReg_I(2, &m_option.m_fillScreen);
	ScanReg_I(3, &m_option.m_wideMode);
	ScanReg_I(4, &m_option.m_overallLevel);
	ScanReg_I(5, &m_option.m_screenWidth);
	ScanReg_I(6, &m_option.m_screenHeight);
	ScanReg_I(7, &m_option.m_screenBpp);
	ScanReg_I(8, &m_option.m_tnlLevel);
	m_option.m_tnlLevel = 1;
	ScanReg_I(9, &m_option.m_mipmapLevel);
	ScanReg_I(10, &m_option.m_mipmapEnabled);
	ScanReg_I(11, &m_option.m_mipmapParam1);
	ScanReg_I(12, &m_option.m_mipmapParam2);
	ScanReg_I(13, &m_option.m_particleLevel);
	ScanReg_I(14, &m_option.m_shadowEnabled);
	ScanReg_I(15, &m_option.m_shadowmapEnabled);
	ScanReg_I(16, &m_option.m_reflectionEnabled);
	ScanReg_I(17, &m_option.m_lodEnabled);
	ScanReg_I(18, &m_option.m_npcEnabled);
	ScanReg_I(19, &m_option.m_coordX);
	ScanReg_I(20, &m_option.m_coordY);
	ScanReg_F(21, &m_option.m_gamma);

	ScanReg_I(22, &m_option.m_mssEnabled);
	ScanReg_I(23, &m_option.m_audioOverall);
	ScanReg_I(24, &m_option.m_mssFrequency);
	ScanReg_I(25, &m_option.m_mssBits);
	ScanReg_I(26, &m_option.m_mssChannels);
	ScanReg_I(27, &m_option.m_mssHwSoundEnabled);
	ScanReg_I(28, &m_option.m_mssBalance);
	ScanReg_I(29, &m_option.m_speakerType);
	ScanReg_F(30, &m_option.m_sfxVolume);
	ScanReg_F(31, &m_option.m_bgmVolume);

	ScanReg_F(32, &m_option.m_mouseSensitivity);
	ScanReg_I(33, &m_option.m_whisperAllowed);
	ScanReg_I(34, &m_option.m_invitationAllowed);
	ScanReg_I(35, &m_option.m_friendConfirmNeeded);
	ScanReg_I(36, &m_option.m_chatWinTransparent);
	ScanReg_I(37, &m_option.m_uiEffect);
	ScanReg_S(38, m_option.m_lastLoginID);
	ScanReg_I(39, &m_option.m_identity);
	ScanReg_I(40, &m_option.m_chatLineExtended);
	ScanReg_I(41, &m_option.m_userSort);
	ScanReg_I(42, &m_option.m_pplEnable);
	ScanReg_I(43, &m_option.m_pplSize);
	ScanReg_I(44, &m_option.m_powerGauge);
	ScanReg_I(45, &m_option.m_captureLogo);
	ScanReg_I(46, &m_option.m_restore);
	ScanReg_I(47, &m_option.m_chatWndUp);
	ScanReg_I(48, &m_option.m_underExtBtn);
	ScanReg_I(49, &m_option.m_avatarNewBie);
	ScanReg_I(50, &m_option.m_captureHideGUI);
	ScanReg_I(51, &m_option.m_captureHidePI);
	ScanReg_I(52, &m_option.m_terrainTooltip);
	ScanReg_I(53, &m_option.m_cutinFlag);

	char szMacro[8][260] = { 0 };
	ScanReg_S(54, szMacro[0]);
	ScanReg_S(55, szMacro[1]);
	ScanReg_S(56, szMacro[2]);
	ScanReg_S(57, szMacro[3]);
	ScanReg_S(58, szMacro[4]);
	ScanReg_S(59, szMacro[5]);
	ScanReg_S(60, szMacro[6]);
	ScanReg_S(61, szMacro[7]);
	ScanReg_I(62, &m_option.m_saveMacros);

	wnsprintfA(m_option.m_macroText[0], 64, "%s", szMacro[0]);
	wnsprintfA(m_option.m_macroText[1], 64, "%s", szMacro[1]);
	wnsprintfA(m_option.m_macroText[2], 64, "%s", szMacro[2]);
	wnsprintfA(m_option.m_macroText[3], 64, "%s", szMacro[3]);
	wnsprintfA(m_option.m_macroText[4], 64, "%s", szMacro[4]);
	wnsprintfA(m_option.m_macroText[5], 64, "%s", szMacro[5]);
	wnsprintfA(m_option.m_macroText[6], 64, "%s", szMacro[6]);
	wnsprintfA(m_option.m_macroText[7], 64, "%s", szMacro[7]);

	// Validate what was read.
#ifdef PG_WINDOWED
	// The CMake build starts windowed whatever the registry says (CMake
	// PG_WINDOWED); the in-game fullscreen toggle still works.
	m_option.m_windowed = 1;
#else
	m_option.m_windowed = m_option.m_windowed != 0;
#endif

	m_option.m_fillScreen = m_option.m_fillScreen != 0;

	m_option.m_wideMode = m_option.m_wideMode != 0;
	m_option.m_overallLevel = Between(0, m_option.m_overallLevel, 2);
	m_bar_Y = m_option.m_screenHeight - 62.0f;
	m_option.m_tnlLevel = Between(0, m_option.m_tnlLevel, 2);
	m_option.m_mipmapLevel = Max(0, m_option.m_mipmapLevel);
	m_option.m_mipmapEnabled = m_option.m_mipmapEnabled != 0;
	m_option.m_mipmapParam1 = Max(0, m_option.m_mipmapParam1);
	m_option.m_mipmapParam2 = Between(0, m_option.m_mipmapParam2, 5);
	m_option.m_particleLevel = Between(0, m_option.m_particleLevel, 2);
	m_option.m_shadowEnabled = m_option.m_shadowEnabled != 0;
	m_option.m_shadowmapEnabled = m_option.m_shadowmapEnabled != 0;
	m_option.m_reflectionEnabled = m_option.m_reflectionEnabled != 0;
	m_option.m_lodEnabled = m_option.m_lodEnabled != 0;
	m_option.m_npcEnabled = m_option.m_npcEnabled != 0;

	m_option.m_coordX = Between(0, m_option.m_coordX, GetSystemMetrics(SM_CXSCREEN) - m_option.m_screenWidth);
	m_option.m_coordY = Between(0, m_option.m_coordX, GetSystemMetrics(SM_CYSCREEN) - m_option.m_screenHeight);
	m_option.m_gamma = Between(0.5f, m_option.m_gamma, 3.0f);

	m_tnlMode = m_option.m_tnlLevel;

	m_option.m_mssEnabled = m_option.m_mssEnabled != 0;
	m_option.m_audioOverall = Between(0, m_option.m_audioOverall, 2);
	m_option.m_mssFrequency = m_option.m_mssFrequency == 44100 ? 44100 : 1;
	m_option.m_mssBits = m_option.m_mssBits == 16 ? 16 : 8;
	m_option.m_mssChannels = m_option.m_mssChannels == 2 ? 2 : 1;
	m_option.m_mssHwSoundEnabled = m_option.m_mssHwSoundEnabled != 0;
	m_option.m_mssBalance = Between(0, m_option.m_mssBalance, 256);
	m_option.m_speakerType = Between(0, m_option.m_speakerType, 5);
	m_option.m_sfxVolume = Between(0.0f, m_option.m_sfxVolume, 1.0f);
	m_option.m_bgmVolume = Between(0.0f, m_option.m_bgmVolume, 1.0f);

	m_option.m_mouseSensitivity = Between(0.5f, m_option.m_mouseSensitivity, 2.0f);
	m_option.m_whisperAllowed = m_option.m_whisperAllowed != 0;
	m_option.m_invitationAllowed = m_option.m_invitationAllowed != 0;
	m_option.m_chatWinTransparent = m_option.m_chatWinTransparent != 0;
	m_option.m_uiEffect = m_option.m_uiEffect != 0;
	// The byte past the 0x16 ID characters terminates an ID strncpy filled up.
	m_option.m_lastLoginID[sizeof(m_option.m_lastLoginID)] = '\0';
	m_option.m_chatLineExtended = m_option.m_chatLineExtended != 0;
	m_option.m_powerGauge = m_option.m_powerGauge != 0;
	m_option.m_captureLogo = m_option.m_captureLogo != 0;
	m_option.m_cutinFlag = m_option.m_cutinFlag != 0;

	// An empty or blank macro gets the default text.
	for (int i = 0; i < 8; i++) {
		std::string macro(m_option.m_macroText[i]);

		if (macro.length() == 0 || macro.find_first_not_of(' ') == std::string::npos) {
			sprintf(m_option.m_macroText[i], "\xc3\xa4\xc6\xc3 \xb8\xc5\xc5\xa9\xb7\xce\xb8\xa6 \xbc\xb3\xc1\xa4\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
		}

		LimitText(m_option.m_macroText[i], 64);
	}
	m_option.m_saveMacros = m_option.m_saveMacros != 0;

	WRegistry mouseRegistry;

	mouseRegistry.SetLocation("HKEY_CURRENT_USER\\Control Panel\\Mouse");

	char szSwap[128];
	if (mouseRegistry.Read("SwapMouseButtons", szSwap, sizeof(szSwap))) {
		int swap;

		sscanf(szSwap, "%d", &swap);
		m_swapMouseButton = swap != 0;
	}

	FrForm::EnableTail(m_option.m_uiEffect != 0);

	char szCmdLine[260];
	strcpy(szCmdLine, GetCommandLineA());
	strlwr(szCmdLine);

	if (strstr(szCmdLine, "-safe")) {
		// Safe mode: the most compatible settings.
		m_option.m_windowed = 1;
		m_option.m_wideMode = 0;
		m_option.m_overallLevel = 2;
		m_option.m_screenWidth = 800;
		m_option.m_screenHeight = 600;
		m_option.m_screenBpp = 16;
		m_bar_Y = m_option.m_screenHeight - 62.0f;
		m_option.m_tnlLevel = 0;
		m_option.m_mipmapLevel = 1;
		m_option.m_mipmapEnabled = 0;
		m_option.m_mipmapParam1 = 1;
		m_option.m_mipmapParam2 = 2;
		m_option.m_particleLevel = 2;
		m_option.m_shadowEnabled = 0;
		m_option.m_shadowmapEnabled = 0;
		m_option.m_reflectionEnabled = 0;
		m_option.m_lodEnabled = 0;
		m_option.m_npcEnabled = 0;
		m_option.m_coordX = (GetSystemMetrics(SM_CXSCREEN) - 800) / 2;
		m_option.m_coordY = (GetSystemMetrics(SM_CYSCREEN) - 600) / 2;

		m_tnlMode = m_option.m_tnlLevel;
	}

	strcpy(m_option.m_pcBangMascotMsg, "PangYa!");
}

// clientsetting.cpp:1143
void COption::CheckDebugRegValid(void) {
	m_captureFormat = Between(0, m_captureFormat, 2);
	m_defaultCharacter = Max(0, m_defaultCharacter);
	m_defaultCaddie = Max(0, m_defaultCaddie);
	m_aviWidth = (m_aviWidth == 640 || m_aviWidth == 800) ? m_aviWidth : 320;
	m_aviHeight = m_aviWidth == 640 ? 480 : (m_aviWidth == 800 ? 600 : 240);
	m_aviFps = Max(1, m_aviFps);

	m_startMode = Between(0, m_startMode, 3);

	m_lobbyCourse = Max(-1, m_lobbyCourse);
	m_map = Max(0, m_map);
	m_startHole = Between(1, m_startHole, 18);
	m_numPlayers = Between(1, m_numPlayers, 30);
	m_shotTimeLimitMode = Max(0, m_shotTimeLimitMode);
	m_weather = Between(0, m_weather, 4);
	m_chatGender = m_chatGender != 0;
	m_vtxClrTestEnabled = m_vtxClrTestEnabled != 0;
	m_baseMeshTestEnabled = m_baseMeshTestEnabled != 0;
	m_clickSystem = m_clickSystem != 0;
	m_replayMode = Between(0, m_replayMode, 3);
	m_gameType = Between(0, m_gameType, 16);
	m_versionInfoEnabled = m_versionInfoEnabled != 0;
}

// clientsetting.cpp:1171
void COption::SetVideoDevice(WVideoDev* videoDevice) {
	int caps;

	m_videoDevice = videoDevice;
	m_videoDevice->Command(WX_VDEV_GET_CAPS, 0, (intptr_t)&caps);
	sOption::sbHwTnlSupport = caps != 0;
	sOption::sbRtSupport = m_videoDevice->SupportRenderTargetFormat();

	sOption::sbVsSupport = m_videoDevice->IsSupportVS();
	sOption::sbPsSupport = m_videoDevice->IsSupportPS();
	sOption::sbMrtSupport = m_videoDevice->IsSupportMRT();
	sOption::sbClipPlaneSupport = m_videoDevice->IsSupportClipPlane();

	m_videoDevice->Command(WX_VDEV_GET_CAPS, 1, (intptr_t)&caps);
	m_tnlMode = m_option.m_tnlLevel = caps < 0 ? 0 : (caps > 2 ? 2 : caps);
	m_videoDevice->Command(W_VDEV_SET_CAPTURE_MODE, m_option.m_powerGauge, m_option.m_windowed);

	m_deviceManager->ResetVideoDevice(true, 800, 600, m_option.m_screenBpp, 0xC40000, 0);

	if (!sOption::sbRtSupport)
		m_option.m_shadowEnabled = 0;

	if (!sOption::sbRtSupport || !sOption::sbClipPlaneSupport || !sOption::sbVsSupport || !sOption::sbPsSupport)
		m_option.m_reflectionEnabled = 0;

	if (!sOption::sbRtSupport)
		m_option.m_lodEnabled = 0;

	m_option.vSetOverallByVidOptions();

	CheckDisplayModeList(videoDevice, m_option.m_windowed != 0, m_option.m_wideMode != 0);

	vApplyMipmap();
}

// clientsetting.cpp:1209
void COption::SetGlobalView(WView* view) {
	m_globalView = view;
}

// clientsetting.cpp:1214
void COption::SetAudioManager(CSoundManager* audioManager) {
	m_audioManager = audioManager;
}

// clientsetting.cpp:1219
void COption::SetInputManager(CInputManager* inputManager) {
	m_inputManager = inputManager;

	if (m_inputManager)
		m_inputManager->SwapMouseButton(m_swapMouseButton);

	gSetMouseSensitivity(m_option.m_mouseSensitivity);
}

// clientsetting.cpp:1231
void COption::vSetMipmap(int level, bool enable, int param1, int param2) {
	m_option.m_mipmapLevel = Max(0, level);
	m_option.m_mipmapEnabled = enable ? 1 : 0;
	m_option.m_mipmapParam1 = Max(0, param1);
	m_option.m_mipmapParam2 = Max(0, param2);
}

// clientsetting.cpp:1239
void COption::vApplyMipmap(void) {
	m_videoDevice->Command(W_VDEV_SET_DDS_RES, m_option.m_mipmapLevel, 0);
	m_videoDevice->Command(W_VDEV_USE_MIPMAP, m_option.m_mipmapEnabled, 0);
	m_videoDevice->Command(W_VDEV_MAX_MIPLVL, m_option.m_mipmapParam1, 0);
	m_videoDevice->Command(W_VDEV_MIP_CREATE_FILTER, m_option.m_mipmapParam2, 0);
}

// clientsetting.cpp:1249
void COption::vSetMinimumScreenSize(int enable, int width, int height) {
	m_option.m_minScreenEnabled = enable;
	m_option.m_minScreenWidth = width;
	m_option.m_minScreenHeight = height;
}

// clientsetting.cpp:1256
void COption::vChangeGameScreenSize(int width, int height, int bpp, bool check) {
	if (check) {
		vCheckScreenSize(width, height, bpp);
	} else {
		m_option.m_screenWidth = width;
		m_option.m_screenHeight = height;
		m_option.m_screenBpp = bpp;
	}

	m_bar_Y = m_option.m_screenHeight - 62.0f;
}

// clientsetting.cpp:1270
void COption::vApplyGameScreenSize(void) {
	if (m_option.m_minScreenEnabled) {
		if (m_option.m_screenWidth < m_option.m_minScreenWidth || m_option.m_screenHeight < m_option.m_minScreenHeight) {
			vChangeScreenSize(m_option.m_minScreenWidth, m_option.m_minScreenHeight, m_option.m_screenBpp);
			return;
		}
	}

	vChangeScreenSize(m_option.m_screenWidth, m_option.m_screenHeight, m_option.m_screenBpp);
}

// clientsetting.cpp:1285
void COption::vApplyLobbyScreenSize(void) {
	if (CShadowManager::Instance())
		CShadowManager::Instance()->Reset();

	vChangeScreenSize(800, 600, m_option.m_screenBpp);
}

// clientsetting.cpp:1293
bool COption::vChangeScreenSize(int width, int height, int bpp) {
	int captureMode;
	bool captureChanged = false;

	m_videoDevice->Command(W_VDEV_GET_CAPTURE_MODE, (intptr_t)&captureMode, 0);

	if (CSharedDoc::Instance()->m_roomType == 0xB || CSharedDoc::Instance()->m_roomType == 0xC) {
		if (captureMode != 0)
			captureChanged = true;
	} else {
		if (captureMode != m_option.m_powerGauge)
			captureChanged = true;
	}

	if (m_videoDevice->IsWindowed() == (m_option.m_windowed != 0 ? true : false)
		&& m_videoDevice->GetWidth() == width
		&& m_videoDevice->GetHeight() == height
		&& m_videoDevice->IsFillScreenMode() == (m_option.m_fillScreen != 0 ? true : false)
		&& (m_option.m_windowed || m_videoDevice->GetBackBufferBpp() == bpp)
		&& !captureChanged)
		return true;

	WView* view = m_globalView;
	float clipNear = view->GetClipNearValue();
	float fov = view->GetFOV();
	float clipFar = view->GetClipFarValue();

	CProjectG::Instance()->SetWindowed(m_option.m_windowed != 0);

	m_videoDevice->Command(W_VDEV_SET_CAPTURE_MODE, CSharedDoc::Instance()->m_roomType == 0xB || CSharedDoc::Instance()->m_roomType == 0xC ? 0 : m_option.m_powerGauge, m_option.m_windowed);

	bool result;
	if (m_option.m_windowed) {
		long style = m_option.m_fillScreen ? 0x90080000 : 0x00C40000;
		result = m_deviceManager->ResetVideoDevice(true, width, height, bpp, style, m_option.m_fillScreen);
	} else {
		result = m_deviceManager->ResetVideoDevice(false, width, height, bpp, 0x90080000, 0);
	}

	m_globalView->SetViewport((float)m_videoDevice->GetWidth(), (float)m_videoDevice->GetHeight());
	m_globalView->SetClip(clipNear, clipFar, false);
	m_globalView->SetFOV(fov);
	m_globalView->UpdateCamera();

	return result;
}

// clientsetting.cpp:1357
void COption::vSetParticleLevel(int level) {
	m_option.m_particleLevel = Between<int>(0, level, 2);
}

// clientsetting.cpp:1362
void COption::vEnableShadow(bool enable) {
	m_option.m_shadowEnabled = enable ? 1 : 0;
}

// clientsetting.cpp:1367
void COption::vEnableShadowmap(bool enable) {
	m_option.m_shadowmapEnabled = enable ? 1 : 0;
}

// clientsetting.cpp:1372
bool COption::vIsWindowed(void) {
	return m_option.m_windowed != 0 ? true : false;
}

// clientsetting.cpp:1377
bool COption::vIsWideMode(void) {
	return m_option.m_wideMode != 0 ? true : false;
}

// clientsetting.cpp:1382
bool COption::vIsShadowEnabled(void) {
	return m_option.m_shadowEnabled != 0 ? true : false;
}

// clientsetting.cpp:1387
bool COption::vIsShadowmapEnabled(void) {
	return m_option.m_shadowmapEnabled != 0 ? true : false;
}

// clientsetting.cpp:1392
bool COption::vIsReflectionEnabled(void) {
	return m_option.m_reflectionEnabled != 0 ? true : false;
}

// clientsetting.cpp:1397
bool COption::vIsLodEnabled(void) {
	return m_option.m_lodEnabled != 0 ? true : false;
}

// clientsetting.cpp:1402
bool COption::vIsNpcEnabled(void) {
	return m_option.m_npcEnabled != 0 ? true : false;
}

// clientsetting.cpp:1407
void COption::vToggleFullscreen(void) {
	m_newOption = m_option;
	m_newOption.m_windowed = !m_option.m_windowed;
	m_changePending = true;
	m_changeFlag = false;
}

// clientsetting.cpp:1414
void COption::aApplySfxVolume(float volume) {
	if (m_audioManager) {
		m_audioManager->SetVolume(Between<float>(0.0f, volume, 1.0f));
	}
}

// clientsetting.cpp:1422
void COption::aApplyBgmVolume(float volume) {
	if (m_audioManager) {
		m_audioManager->SetBGMVolume(Between<float>(0.0f, volume, 1.0f));
	}
}

// clientsetting.cpp:1430
void COption::aSetSfxVolume(float volume) {
	m_option.m_sfxVolume = Between<float>(0.0f, volume, 1.0f);
}

// clientsetting.cpp:1435
void COption::aSetBgmVolume(float volume) {
	m_option.m_bgmVolume = Between<float>(0.0f, volume, 1.0f);
}

// clientsetting.cpp:1440
void COption::aSetSpeakerType(int type) {
	m_option.m_speakerType = Between<int>(0, type, 5);
}

// clientsetting.cpp:1445
bool COption::aIsMssEnabled(void) {
	return m_option.m_mssEnabled != 0 ? true : false;
}

// clientsetting.cpp:1450
bool COption::aIsMssHwSoundEnabled(void) {
	return m_option.m_mssHwSoundEnabled != 0 ? true : false;
}

// clientsetting.cpp:1455
WMilesSoundSystem::w_speaker_type COption::aGetMssSpeaker(void) {
	return (WMilesSoundSystem::w_speaker_type)m_option.m_speakerType;
}

// clientsetting.cpp:1461
void COption::aApplyAudioSetting(void) {
	g_audio->SetSpeakerType(m_option.m_speakerType);
}

// clientsetting.cpp:1470
void COption::gSetMouseSensitivity(float sensitivity) {
	m_option.m_mouseSensitivity = Between<float>(0.5f, sensitivity, 2.0f);
	m_inputManager->SetSensitivity(sensitivity);
}

// clientsetting.cpp:1476
bool COption::gIsWhisperAllowed(void) {
	return m_option.m_whisperAllowed != 0 ? true : false;
}

// clientsetting.cpp:1481
bool COption::gIsInvitationAllowed(void) {
	return m_option.m_invitationAllowed != 0 ? true : false;
}

// clientsetting.cpp:1486
bool COption::gIsFriendConfirmNeeded(void) {
	return m_option.m_friendConfirmNeeded != 0 ? true : false;
}

// clientsetting.cpp:1491
bool COption::gIsChatWinTransparent(void) {
	return m_option.m_chatWinTransparent != 0 ? true : false;
}

// clientsetting.cpp:1496
bool COption::gIsChatLineEntended(void) {
	return m_option.m_chatLineExtended != 0 ? true : false;
}

// clientsetting.cpp:1501
bool COption::gIsPPLEnable(void) {
	return m_option.m_pplEnable != 0 ? true : false;
}

// clientsetting.cpp:1506
void COption::gSetPPLEnable(bool enable) {
	m_option.m_pplEnable = enable;
}

// clientsetting.cpp:1511
void COption::gSetPPLSize(int size) {
	m_option.m_pplSize = size;
}

// clientsetting.cpp:1516
int COption::gGetRestore(void) {
	if (m_option.m_restore == 0)
		return 0;

	char check = (char)(m_option.m_restore >> 30);
	check = (char)(check << 2 | ((m_option.m_restore >> 22) & 3));
	check = (char)(check << 2 | ((m_option.m_restore >> 14) & 3));
	check = (char)(check << 2 | ((m_option.m_restore >> 6) & 3));

	// The ID's bytes summed (MyId(), shareddoc.inl:487).
	const char* id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
	unsigned int len = strlen(id);
	char sum = 0;
	for (unsigned int i = 0; i < len; i++)
		sum += id[i];

	if (check != sum) {
		gResetRestore();
		return 0;
	}

	int restore = m_option.m_restore;

	unsigned char halfMinute = (unsigned char)(restore >> 24) & 0x3F;
	return (halfMinute << 25) | (restore & 0x3F3F3F);
}

// clientsetting.cpp:1551
void COption::gSetRestore(void) {
	if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10)
		return;

	// The ID's bytes summed (MyId(), shareddoc.inl:487).
	const char* id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
	unsigned int len = strlen(id);
	char sum = 0;
	for (unsigned int i = 0; i < len; i++)
		sum += id[i];

	SYSTEMTIME time;
	GetLocalTime(&time);
	m_option.m_restore = ((((((((sum >> 6) & 3) << 6 | (time.wMinute / 2)) << 2 | ((sum >> 4) & 3)) << 6 | time.wHour) << 2 | ((sum >> 2) & 3)) << 6 | time.wDay) << 2 | (sum & 3)) << 6 | time.wMonth;

	WriteReg_I(46, m_option.m_restore);
}

// clientsetting.cpp:1584
void COption::gResetRestore(void) {
	m_option.m_restore = 0;
	WriteReg_I(46, m_option.m_restore);
}

// clientsetting.cpp:1590
void COption::dApplyDebugSetting(void) {
	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
		return;

	SetCurMap((unsigned char)m_map);
	// SetHoleType(0) and SetGameTimeLimit(0) (shareddoc.inl:244, 426).
	WSingleton<CSharedDoc>::Instance()->m_gameKind = 0;
	WSingleton<CSharedDoc>::Instance()->m_gameTimeLimit = 0;

	// SetShotTimeLimit (shareddoc.inl:318).
	switch (m_shotTimeLimitMode) {
	case 0:
		WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = 0;
		break;

	case 1:
		WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = 40000;
		break;

	default:
		WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = m_shotTimeLimitMode * 1000;
		break;
	}
}

// clientsetting.cpp:1615
void COption::dSetNumPlayers(int numPlayers) {
	m_numPlayers = Between<int>(1, numPlayers, 30);
}

// clientsetting.cpp:1620
const char* COption::dGetCaptureFormat(void) {
	return ms_szCaptureFmt[m_captureFormat];
}

// clientsetting.cpp:1626
void COption::dSetCaptureFormat(int format) {
	m_captureFormat = format;
}

// clientsetting.cpp:1631
int COption::dGetDefaultCharacter(void) {
	return m_defaultCharacter;
}

// clientsetting.cpp:1636
int COption::dGetDefaultCaddie(void) {
	return m_defaultCaddie;
}

// clientsetting.cpp:1641
int COption::dGetDefaultClub(void) {
	return m_defaultClub;
}

// clientsetting.cpp:1646
int COption::dGetDefaultBall(void) {
	return m_defaultBall;
}

// clientsetting.cpp:1651
int COption::dGetMap(void) {
	return m_map;
}

// clientsetting.cpp:1656
int COption::dGetAviWidth(void) {
	return m_aviWidth;
}

// clientsetting.cpp:1661
int COption::dGetAviHeight(void) {
	return m_aviHeight;
}

// clientsetting.cpp:1666
int COption::dGetAviFps(void) {
	return m_aviFps;
}

// clientsetting.cpp:1671
int COption::dGetStartMode(void) {
	return m_startMode;
}

// clientsetting.cpp:1676
int COption::dGetLobbyCourse(void) {
	return m_lobbyCourse;
}

// clientsetting.cpp:1681
int COption::dGetNumPlayers(void) {
	return m_numPlayers;
}

// clientsetting.cpp:1686
unsigned char COption::dGetChatGender(void) {
	return m_chatGender;
}

// clientsetting.cpp:1691
bool COption::dIsVtxClrTestEnabled(void) {
	return m_vtxClrTestEnabled != 0 ? true : false;
}

// clientsetting.cpp:1696
bool COption::dIsBaseMeshTestEnabled(void) {
	return m_baseMeshTestEnabled != 0 ? true : false;
}

// clientsetting.cpp:1701
int COption::dGetClickSystem(void) {
	return m_clickSystem;
}

// clientsetting.cpp:1706
int COption::dGetReplayMode(void) {
	return m_replayMode;
}

// clientsetting.cpp:1712
void COption::dSetReplayMode(int mode) {
	m_replayMode = mode;
}

// clientsetting.cpp:1717
int COption::dGetGameType(void) {
	return m_gameType;
}

// clientsetting.cpp:1722
int COption::dGetWeather(void) {
	return m_weather;
}

// clientsetting.cpp:1727
unsigned char COption::dGetStartHole(void) {
	return m_startHole;
}

// clientsetting.cpp:1733
bool COption::dIsVersionInfoEnabled(void) {
	return m_versionInfoEnabled != 0 ? true : false;
}

// clientsetting.cpp:1739
float COption::gGetBar_Y(void) {
	return m_bar_Y;
}

// clientsetting.cpp:1744
void COption::ApplyChange(sOption& option, bool flag) {
	m_changePending = true;
	m_newOption = option;
	m_changeFlag = flag;
}

// clientsetting.cpp:1751
const sOption* COption::GetOption(void) {
	return &m_option;
}

// clientsetting.cpp:1756
void COption::CheckPendingChanges(void) {
	if (!m_changePending)
		return;

	m_changePending = false;

	if (m_option.m_windowed != m_newOption.m_windowed
		|| m_option.m_screenWidth != m_newOption.m_screenWidth
		|| m_option.m_screenHeight != m_newOption.m_screenHeight
		|| m_option.m_screenBpp != m_newOption.m_screenBpp
		|| m_option.m_fillScreen != m_newOption.m_fillScreen) {
		if (CBrowser::Instance())
			CBrowser::Instance()->ResetCount();

		if (m_changeFlag) {
			int oldWidth = m_option.m_screenWidth;
			int oldHeight = m_option.m_screenHeight;
			int oldBpp = m_option.m_screenBpp;

			m_option.m_fillScreen = m_newOption.m_fillScreen;
			m_option.m_wideMode = m_newOption.m_wideMode;
			m_option.m_windowed = m_newOption.m_windowed;
			vCheckScreenSize(m_newOption.m_screenWidth, m_newOption.m_screenHeight, m_newOption.m_screenBpp); m_bar_Y = m_option.m_screenHeight - 62.0f;
			vApplyGameScreenSize();

			if (CSceneManager::Instance())
				CSceneManager::Instance()->UpdateFog(false);

			if (oldWidth != m_option.m_screenWidth || oldHeight != m_option.m_screenHeight || oldBpp != m_option.m_screenBpp) {
				// Tells the task's actors the screen size changed (spin, letter_effect).
				MsgObject msg;
				msg.m_msg = 0x271B;
				msg.m_pSender = 0;
				msg.m_param1 = 0;
				msg.m_param2 = 0;
				msg.m_param3 = 0;
				msg.m_reserved = 0;
				AfxGetTask()->SendGlobalMsg(msg);

				if (g_pFresh)
					g_pFresh->GetDesktop()->ResetRect();
			}
		} else {
			m_option.m_windowed = m_newOption.m_windowed;
			m_option.m_fillScreen = m_newOption.m_fillScreen;
			m_option.m_wideMode = m_newOption.m_wideMode;
			vCheckScreenSize(m_newOption.m_screenWidth, m_newOption.m_screenHeight, m_newOption.m_screenBpp); m_bar_Y = m_option.m_screenHeight - 62.0f;
			if (CShadowManager::Instance())
				CShadowManager::Instance()->Reset();
			vChangeScreenSize(800, 600, m_option.m_screenBpp);
		}

		if (CChatMsg::Instance())
			CChatMsg::Instance()->ResetUiPosition();
	}

	m_option.m_overallLevel = m_newOption.m_overallLevel;
	m_option.m_tnlLevel = m_newOption.m_tnlLevel;
	m_option.m_mipmapLevel = m_newOption.m_mipmapLevel;
	m_option.m_mipmapEnabled = m_newOption.m_mipmapEnabled;
	m_option.m_mipmapParam1 = m_newOption.m_mipmapParam1;
	m_option.m_mipmapParam2 = m_newOption.m_mipmapParam2;
	m_option.m_particleLevel = m_newOption.m_particleLevel;

	if (!(WAbsBySign(m_option.m_gamma - m_newOption.m_gamma) < WMATH_EPSILON))
		m_option.m_gamma = m_newOption.m_gamma;

	if (m_option.m_shadowEnabled != m_newOption.m_shadowEnabled) {
		m_option.m_shadowEnabled = m_newOption.m_shadowEnabled;

		if (CShadowManager::Instance())
			CShadowManager::Instance()->Enable(m_option.m_shadowEnabled != 0);
	}

	if (m_option.m_shadowmapEnabled != m_newOption.m_shadowmapEnabled) {
		m_option.m_shadowmapEnabled = m_newOption.m_shadowmapEnabled;

		if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
			if (GOLFDOC() && GOLFDOC()->m_pPolySoup)
				GOLFDOC()->m_pPolySoup->EnableShadowmap(m_option.m_shadowmapEnabled != 0);
		}
	}

	if (m_option.m_reflectionEnabled != m_newOption.m_reflectionEnabled) {
		m_option.m_reflectionEnabled = m_newOption.m_reflectionEnabled;

		if (CSceneManager::Instance())
			CSceneManager::Instance()->SetReflection(m_option.m_reflectionEnabled != 0);
	}

	if (m_option.m_lodEnabled != m_newOption.m_lodEnabled) {
		m_option.m_lodEnabled = m_newOption.m_lodEnabled;

		if (CSceneManager::Instance())
			CSceneManager::Instance()->SetLOD(m_option.m_lodEnabled != 0, true);
	}

	m_option.m_npcEnabled = m_newOption.m_npcEnabled;

	m_option.m_audioOverall = m_newOption.m_audioOverall;

	if (m_option.m_mssEnabled != m_newOption.m_mssEnabled
		|| m_option.m_mssFrequency != m_newOption.m_mssFrequency
		|| m_option.m_mssBits != m_newOption.m_mssBits
		|| m_option.m_mssChannels != m_newOption.m_mssChannels
		|| m_option.m_mssHwSoundEnabled != m_newOption.m_mssHwSoundEnabled
		|| m_option.m_mssBalance != m_newOption.m_mssBalance
		|| m_option.m_speakerType != m_newOption.m_speakerType) {
		m_option.m_mssEnabled = m_newOption.m_mssEnabled;
		m_option.m_mssFrequency = m_newOption.m_mssFrequency;
		m_option.m_mssBits = m_newOption.m_mssBits;
		m_option.m_mssChannels = m_newOption.m_mssChannels;
		m_option.m_mssHwSoundEnabled = m_newOption.m_mssHwSoundEnabled;
		m_option.m_mssBalance = m_newOption.m_mssBalance;
		m_option.m_speakerType = m_newOption.m_speakerType;

		g_audio->SetSpeakerType(m_option.m_speakerType);
	}

	if (m_option.m_mssEnabled) {
		if (!(WAbsBySign(m_option.m_sfxVolume - m_newOption.m_sfxVolume) < WMATH_EPSILON))
			aSetSfxVolume(m_newOption.m_sfxVolume);

		if (!(WAbsBySign(m_option.m_bgmVolume - m_newOption.m_bgmVolume) < WMATH_EPSILON))
			aSetBgmVolume(m_newOption.m_bgmVolume);
	}

	if (!(WAbsBySign(m_option.m_mouseSensitivity - m_newOption.m_mouseSensitivity) < WMATH_EPSILON))
		gSetMouseSensitivity(m_newOption.m_mouseSensitivity);

	if (m_option.m_whisperAllowed != m_newOption.m_whisperAllowed) {
		m_option.m_whisperAllowed = m_newOption.m_whisperAllowed;

		WSendPacket packet((enumClientPacket)0x55);
		packet.Encode1((unsigned char)m_option.m_whisperAllowed);
		packet.Send((eSendTo)0);
	}

	m_option.m_invitationAllowed = m_newOption.m_invitationAllowed;

	if (m_option.m_friendConfirmNeeded != m_newOption.m_friendConfirmNeeded)
		m_option.m_friendConfirmNeeded = m_newOption.m_friendConfirmNeeded;

	m_option.m_chatWinTransparent = m_newOption.m_chatWinTransparent;

	if (m_option.m_uiEffect != m_newOption.m_uiEffect) {
		m_option.m_uiEffect = m_newOption.m_uiEffect;
		FrForm::EnableTail(m_option.m_uiEffect != 0);
	}

	if (m_option.m_chatLineExtended != m_newOption.m_chatLineExtended)
		m_option.m_chatLineExtended = m_newOption.m_chatLineExtended;

	if (m_option.m_powerGauge != m_newOption.m_powerGauge) {
		m_option.m_powerGauge = m_newOption.m_powerGauge;
		m_videoDevice->Command(W_VDEV_SET_CAPTURE_MODE, m_option.m_powerGauge, m_option.m_powerGauge);

		if (m_option.m_windowed) {
			long style = m_option.m_fillScreen ? 0x90080000 : 0x00C40000;
			m_deviceManager->ResetVideoDevice(true, m_videoDevice->GetWidth(), m_videoDevice->GetHeight(), m_option.m_screenBpp, style, m_option.m_fillScreen);
		} else {
			m_deviceManager->ResetVideoDevice(false, m_videoDevice->GetWidth(), m_videoDevice->GetHeight(), m_option.m_screenBpp, 0x90080000, 0);
		}
	}

	m_option.m_captureHideGUI = m_newOption.m_captureHideGUI;
	m_option.m_captureHidePI = m_newOption.m_captureHidePI;
	m_option.m_terrainTooltip = m_newOption.m_terrainTooltip;
	m_option.m_cutinFlag = m_newOption.m_cutinFlag;

	for (int i = 0; i < 8; i++) {
		std::string text(m_newOption.m_macroText[i]);

		if (text.length() == 0 || text.find_first_not_of(' ', 0) == std::string::npos) {
			// "Please set the chat macro."
			sprintf(m_option.m_macroText[i], "\xc3\xa4\xc6\xc3 \xb8\xc5\xc5\xa9\xb7\xce\xb8\xa6 \xbc\xb3\xc1\xa4\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
		} else {
			strcpy(m_option.m_macroText[i], m_newOption.m_macroText[i]);
		}
	}

	m_option.m_saveMacros = m_newOption.m_saveMacros;

	SaveReg();
}

// clientsetting.cpp:2004
const char* COption::vGetScrResText(int width, int height, int bpp, bool windowed) {
	const char* text = NULL;

	for (std::list<sDisplayMode>::iterator it = m_displayModes.begin(); it != m_displayModes.end(); ++it) {
		if (width == (*it).width && height == (*it).height) {
			if (windowed) {
				text = (*it).text;
				if (bpp == (*it).bpp)
					return text;
			} else if (bpp == (*it).bpp) {
				return (*it).text;
			}
		}
	}

	return text ? text : "";
}

// clientsetting.cpp:2035
const char* COption::vGetScrResText(int index) {
	int i = 0;

	for (std::list<sDisplayMode>::iterator it = m_displayModes.begin(); it != m_displayModes.end(); ++it) {
		if (i == index)
			return (*it).text;
		i++;
	}

	return "";
}

// clientsetting.cpp:2048
int COption::vGetScrResNum(void) {
	return m_displayModes.size();
}

// clientsetting.cpp:2053
void COption::vGetScrResLvl(const char* text, int* width, int* height, int* bpp) {
	for (std::list<sDisplayMode>::iterator it = m_displayModes.begin(); it != m_displayModes.end(); ++it) {
		if (_strcmpi(text, (*it).text) == 0) {
			*width = (*it).width;
			*height = (*it).height;
			*bpp = (*it).bpp;
			return;
		}
	}
}

// clientsetting.cpp:2068
void COption::vCheckScreenSize(int width, int height, int bpp) {
	for (std::list<sDisplayMode>::iterator it = m_displayModes.begin(); it != m_displayModes.end(); ++it) {
		if (width == (*it).width && height == (*it).height) {
			if (m_option.m_windowed) {
				m_option.m_screenWidth = width;
				m_option.m_screenHeight = height;
				m_option.m_screenBpp = (*it).bpp;
				return;
			}

			if (bpp == (*it).bpp) {
				m_option.m_screenWidth = width;
				m_option.m_screenHeight = height;
				m_option.m_screenBpp = bpp;
				return;
			}
		}
	}
}

// Display modes CheckDisplayModeList offers (file-local tables of the
// original, 0xA7C020 and 0xA7C0B0, no symbol).
struct sDisplayModeSize {
	int width;                     // 0x0
	int height;                    // 0x4
	int bpp;                       // 0x8
};
CHECK_SIZE(sDisplayModeSize, 0xC);

static sDisplayModeSize s_normalModes[12] = {
	{ 640, 480, 16 }, { 640, 480, 32 }, { 800, 600, 16 }, { 800, 600, 32 },
	{ 1024, 768, 16 }, { 1024, 768, 32 }, { 1280, 960, 16 }, { 1280, 960, 32 },
	{ 1280, 1024, 16 }, { 1280, 1024, 32 }, { 1600, 1200, 16 }, { 1600, 1200, 32 },
};

static sDisplayModeSize s_wideModes[28] = {
	{ 1024, 576, 16 }, { 1024, 576, 32 }, { 1280, 720, 16 }, { 1280, 720, 32 },
	{ 1280, 768, 16 }, { 1280, 768, 32 }, { 1280, 800, 16 }, { 1280, 800, 32 },
	{ 1366, 768, 16 }, { 1366, 768, 32 }, { 1400, 900, 16 }, { 1400, 900, 32 },
	{ 1440, 990, 16 }, { 1440, 990, 32 }, { 1400, 1050, 16 }, { 1400, 1050, 32 },
	{ 1440, 900, 16 }, { 1440, 900, 32 }, { 1440, 1050, 16 }, { 1440, 1050, 32 },
	{ 1600, 900, 16 }, { 1600, 900, 32 }, { 1680, 1050, 16 }, { 1680, 1050, 32 },
	{ 1920, 1080, 16 }, { 1920, 1080, 32 }, { 1920, 1200, 16 }, { 1920, 1200, 32 },
};

// clientsetting.cpp:2096
void COption::CheckDisplayModeList(WVideoDev* videoDevice, bool windowed, bool wide) {
	if (!videoDevice)
		return;

	m_displayModes.clear();

	if (wide) {
		for (int i = 0; i < sizeof(s_wideModes) / sizeof(s_wideModes[0]); i++) {
			sDisplayMode mode;
			mode.width = s_wideModes[i].width; mode.height = s_wideModes[i].height; mode.bpp = s_wideModes[i].bpp;

			sprintf(mode.text, "%dx%dx%d", mode.width, mode.height, mode.bpp);

			if (videoDevice->IsSupportedDisplayMode(windowed, mode.width, mode.height, mode.bpp))
				m_displayModes.push_back(mode);
		}
	} else {
		for (int i = 0; i < sizeof(s_normalModes) / sizeof(s_normalModes[0]); i++) {
			sDisplayMode mode;
			mode.width = s_normalModes[i].width; mode.height = s_normalModes[i].height; mode.bpp = s_normalModes[i].bpp;

			sprintf(mode.text, "%dx%dx%d", mode.width, mode.height, mode.bpp);

			if (videoDevice->IsSupportedDisplayMode(windowed, mode.width, mode.height, mode.bpp))
				m_displayModes.push_back(mode);
		}
	}
}

// clientsetting.cpp:2130
void COption::ChangeGameOption(sOption& option, sGameOptionInfo& info) {
	for (int i = 0; i < 8; i++)
		strcpy(info.m_macroText[i], option.m_macroText[i]);
}

// clientsetting.cpp:2137
void COption::ChangeGameOption(sGameOptionInfo& info, sOption& option) {
	for (int i = 0; i < 8; i++)
		strcpy(option.m_macroText[i], info.m_macroText[i]);
}

// clientsetting.cpp:2144
void COption::SendUpdateGameOptionPacket(void) {
	sGameOptionInfo info;

	COption::Instance()->ChangeGameOption(m_option, info);

	WSendPacket packet(0x69);
	packet.EncodeBuffer(&info, sizeof(info));
	packet.Send((eSendTo)0);
}
