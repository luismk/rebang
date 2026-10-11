// Original: d:\build\custom\temp\code\client\projectg\clientsetting.h
#ifndef PG_CLIENT_PROJECTG_CLIENTSETTING_H
#define PG_CLIENT_PROJECTG_CLIENTSETTING_H

#include "..\..\defines.h"
#include "singleton.h"
#include "Miles.h"
#include "wreg.h"

#include <list>
#include <string.h>

// Game option block. Its layout is only partly transcribed; its size is
// 0x318 (COption copies it with 0xC6 dwords in ApplyChange), and its
// destructor is trivial (??1sOption@@QAE@XZ is a single `ret`).
struct sOption {
	static bool IsSupportShadow(void);
	static bool IsSupportReflection(void);
	static bool IsSupportLod(void);

	sOption();
	~sOption();

	const char* vGetOverallText(int level);
	int vGetOverallLvl(const char* text);
	const char* vGetTnlText(int level);
	int vGetTnlLvl(const char* text);
	const char* vGetTexResText(int level);
	const char* vGetParticleText(int level);
	const char* aGetOverallText(int level);
	const char* aGetSpeakerText(int type);
	int aGetSpeakerLvl(const char* text);
	const char* GetThreeLvlText(int level);
	const char* GetTwoLvlText(int level);
	int vGetLvl3FromText(const char* text);
	int vGetTexResLvl(const char* text);
	int vGetParticleLvl(const char* text);
	int aGetOverallLvl(const char* text);
	void vResetByOverall(const char* text);
	void vSetOverallByVidOptions(void);

	// Device capabilities (0xAC6D5D..0xAC6D62 in the original's .data,
	// filled by COption::SetVideoDevice).
	static bool sbHwTnlSupport;
	static bool sbRtSupport;
	static bool sbVsSupport;
	static bool sbPsSupport;
	static bool sbMrtSupport;
	static bool sbClipPlaneSupport;

	int m_windowed;                // 0x0
	int m_fillScreen;              // 0x4 (CBrowser::PrepareOpen forces it off)
	int m_wideMode;                // 0x8
	int m_screenWidth;             // 0xC
	int m_screenHeight;            // 0x10
	int m_minScreenEnabled;        // 0x14
	int m_minScreenWidth;          // 0x18
	int m_minScreenHeight;         // 0x1C
	int m_screenBpp;               // 0x20
	int m_overallLevel;            // 0x24 (overall video quality, -1 = custom)
	int m_tnlLevel;                // 0x28 (registry vTnLMode; SetVideoDevice clamps the device's to 0..2)
	int m_mipmapLevel;             // 0x2C (vApplyMipmap: W_VDEV_SET_DDS_RES)
	int m_mipmapEnabled;           // 0x30
	int m_mipmapParam1;            // 0x34
	int m_mipmapParam2;            // 0x38
	int m_particleLevel;           // 0x3C
	int m_shadowEnabled;           // 0x40
	int m_shadowmapEnabled;        // 0x44
	int m_reflectionEnabled;       // 0x48
	int m_lodEnabled;              // 0x4C
	int m_npcEnabled;              // 0x50
	int m_coordX;                  // 0x54 (registry vCoordX)
	int m_coordY;                  // 0x58 (registry vCoordY)
	float m_gamma;                 // 0x5C (registry vGamma, default 1.3)
	int m_mssEnabled;              // 0x60
	int m_audioOverall;            // 0x64 (registry aOverall)
	int m_mssFrequency;            // 0x68
	int m_mssBits;                 // 0x6C
	int m_mssChannels;             // 0x70
	int m_mssHwSoundEnabled;       // 0x74
	int m_mssBalance;              // 0x78
	int m_speakerType;             // 0x7C
	float m_sfxVolume;             // 0x80
	float m_bgmVolume;             // 0x84
	float m_mouseSensitivity;      // 0x88
	int m_whisperAllowed;          // 0x8C
	int m_invitationAllowed;       // 0x90
	int m_friendConfirmNeeded;     // 0x94
	int m_chatWinTransparent;      // 0x98
	int m_uiEffect;                // 0x9C
	char m_lastLoginID[0x16];      // 0xA0 (strncpy'd with its size)
	UNKNOWN(0xB6, 0xB8);
	int m_identity;                // 0xB8 (registry gIdentity)
	int m_chatLineExtended;        // 0xBC
	int m_userSort;                // 0xC0 (registry gUserSort)
	int m_pplEnable;               // 0xC4
	int m_pplSize;                 // 0xC8
	int m_powerGauge;              // 0xCC (FrOptionDlg::gOnPowerGaugeBtnUp; also the W_VDEV_SET_CAPTURE_MODE value)
	int m_captureLogo;             // 0xD0 (registry gCaptureLogo)
	int m_restore;                 // 0xD4 (registry value 46): packed month, day, hour, minute / 2 and a checksum of the ID (gSetRestore)
	int m_chatWndUp;               // 0xD8
	int m_underExtBtn;             // 0xDC
	int m_avatarNewBie;            // 0xE0
	int m_captureHideGUI;          // 0xE4
	int m_captureHidePI;           // 0xE8
	int m_terrainTooltip;          // 0xEC
	int m_cutinFlag;               // 0xF0
	char m_macroText[8][0x40];     // 0xF4
	int m_saveMacros;              // 0x2F4
	char m_pcBangMascotMsg[0x1D];  // 0x2F8 (strncpy'd with its size)
	UNKNOWN(0x315, 0x318);
};
CHECK_SIZE(sOption, 0x318);
BEGIN_LAYOUT_CHECK(sOption)
	CHECK_FIELD(sOption, m_windowed, 0x0);
	CHECK_FIELD(sOption, m_fillScreen, 0x4);
	CHECK_FIELD(sOption, m_wideMode, 0x8);
	CHECK_FIELD(sOption, m_screenWidth, 0xC);
	CHECK_FIELD(sOption, m_screenHeight, 0x10);
	CHECK_FIELD(sOption, m_minScreenEnabled, 0x14);
	CHECK_FIELD(sOption, m_minScreenWidth, 0x18);
	CHECK_FIELD(sOption, m_minScreenHeight, 0x1C);
	CHECK_FIELD(sOption, m_screenBpp, 0x20);
	CHECK_FIELD(sOption, m_overallLevel, 0x24);
	CHECK_FIELD(sOption, m_tnlLevel, 0x28);
	CHECK_FIELD(sOption, m_mipmapLevel, 0x2C);
	CHECK_FIELD(sOption, m_mipmapEnabled, 0x30);
	CHECK_FIELD(sOption, m_mipmapParam1, 0x34);
	CHECK_FIELD(sOption, m_mipmapParam2, 0x38);
	CHECK_FIELD(sOption, m_particleLevel, 0x3C);
	CHECK_FIELD(sOption, m_shadowEnabled, 0x40);
	CHECK_FIELD(sOption, m_shadowmapEnabled, 0x44);
	CHECK_FIELD(sOption, m_reflectionEnabled, 0x48);
	CHECK_FIELD(sOption, m_lodEnabled, 0x4C);
	CHECK_FIELD(sOption, m_npcEnabled, 0x50);
	CHECK_FIELD(sOption, m_coordX, 0x54);
	CHECK_FIELD(sOption, m_coordY, 0x58);
	CHECK_FIELD(sOption, m_gamma, 0x5C);
	CHECK_FIELD(sOption, m_mssEnabled, 0x60);
	CHECK_FIELD(sOption, m_audioOverall, 0x64);
	CHECK_FIELD(sOption, m_mssFrequency, 0x68);
	CHECK_FIELD(sOption, m_mssBits, 0x6C);
	CHECK_FIELD(sOption, m_mssChannels, 0x70);
	CHECK_FIELD(sOption, m_mssHwSoundEnabled, 0x74);
	CHECK_FIELD(sOption, m_mssBalance, 0x78);
	CHECK_FIELD(sOption, m_speakerType, 0x7C);
	CHECK_FIELD(sOption, m_sfxVolume, 0x80);
	CHECK_FIELD(sOption, m_bgmVolume, 0x84);
	CHECK_FIELD(sOption, m_mouseSensitivity, 0x88);
	CHECK_FIELD(sOption, m_whisperAllowed, 0x8C);
	CHECK_FIELD(sOption, m_invitationAllowed, 0x90);
	CHECK_FIELD(sOption, m_friendConfirmNeeded, 0x94);
	CHECK_FIELD(sOption, m_chatWinTransparent, 0x98);
	CHECK_FIELD(sOption, m_uiEffect, 0x9C);
	CHECK_FIELD(sOption, m_lastLoginID, 0xA0);
	CHECK_FIELD(sOption, m_identity, 0xB8);
	CHECK_FIELD(sOption, m_chatLineExtended, 0xBC);
	CHECK_FIELD(sOption, m_userSort, 0xC0);
	CHECK_FIELD(sOption, m_pplEnable, 0xC4);
	CHECK_FIELD(sOption, m_pplSize, 0xC8);
	CHECK_FIELD(sOption, m_powerGauge, 0xCC);
	CHECK_FIELD(sOption, m_captureLogo, 0xD0);
	CHECK_FIELD(sOption, m_restore, 0xD4);
	CHECK_FIELD(sOption, m_chatWndUp, 0xD8);
	CHECK_FIELD(sOption, m_underExtBtn, 0xDC);
	CHECK_FIELD(sOption, m_avatarNewBie, 0xE0);
	CHECK_FIELD(sOption, m_captureHideGUI, 0xE4);
	CHECK_FIELD(sOption, m_captureHidePI, 0xE8);
	CHECK_FIELD(sOption, m_terrainTooltip, 0xEC);
	CHECK_FIELD(sOption, m_cutinFlag, 0xF0);
	CHECK_FIELD(sOption, m_macroText, 0xF4);
	CHECK_FIELD(sOption, m_saveMacros, 0x2F4);
	CHECK_FIELD(sOption, m_pcBangMascotMsg, 0x2F8);
END_LAYOUT_CHECK

// Game option block exchanged with COption::ChangeGameOption. Only the
// members the transcribed functions use are named. Its size is 0x240
// (SendUpdateGameOptionPacket encodes it whole).
struct sGameOptionInfo {
	char m_macroText[8][0x40]; // 0x0
	char m_pcBangMascotMsg[0x40]; // 0x200, the PC bang mascot's message (COption::mSetPCBangMascotMsg)
};
CHECK_SIZE(sGameOptionInfo, 0x240);
CHECK_FIELD(sGameOptionInfo, m_macroText, 0x0);

class WView;
class WVideoDev;
class WDeviceManager;
class WResourceManager;
class CSoundManager;
class CInputManager;

// Client option manager (registry-backed settings). Derives from
// WSingleton<COption> (WSingleton<COption>::Instance is emitted by this
// unit). Only the members read by the transcribed accessors are named.
class COption : public WSingleton<COption> {
public:
	// One display mode of the device (built by CheckDisplayModeList).
	struct sDisplayMode {
		int width;                 // 0x0
		int height;                // 0x4
		int bpp;                   // 0x8
		char text[0x10];           // 0xC "<width>x<height>x<bpp>"
	};

	COption(HWND hWnd, WDeviceManager* deviceManager, WResourceManager* resourceManager);
	virtual ~COption();

	int vGetScrResNum(void);
	int dGetDefaultCharacter(void);
	int dGetDefaultCaddie(void);
	int dGetDefaultClub(void);
	int dGetDefaultBall(void);
	int dGetMap(void);
	int dGetAviWidth(void);
	int dGetAviHeight(void);
	int dGetAviFps(void);
	int dGetStartMode(void);
	int dGetLobbyCourse(void);
	int dGetNumPlayers(void);
	unsigned char dGetChatGender(void);
	int dGetClickSystem(void);
	int dGetReplayMode(void);
	int dGetGameType(void);
	int dGetWeather(void);
	unsigned char dGetStartHole(void);
	float gGetBar_Y(void);
	const sOption* GetOption(void);
	void SetVideoDevice(WVideoDev* videoDevice);
	void CheckDisplayModeList(WVideoDev* videoDevice, bool windowed, bool wide);
	void CheckDebugRegValid(void);
	void CheckPendingChanges(void);
	void dApplyDebugSetting(void);
	int gGetRestore(void);
	void gSetRestore(void);

	void vSetMipmap(int level, bool enable, int param1, int param2);
	void vSetMinimumScreenSize(int enable, int width, int height);
	void vChangeGameScreenSize(int width, int height, int bpp, bool check);
	void vApplyGameScreenSize(void);
	void vApplyLobbyScreenSize(void);
	void vSetParticleLevel(int level);
	void vToggleFullscreen(void);
	void aApplySfxVolume(float volume);
	void aApplyBgmVolume(float volume);
	void aSetSfxVolume(float volume);
	void aSetBgmVolume(float volume);
	void aSetSpeakerType(int type);
	void dSetNumPlayers(int numPlayers);
	void ApplyChange(sOption& option, bool flag);
	void ChangeGameOption(sOption& option, sGameOptionInfo& info);
	void ChangeGameOption(sGameOptionInfo& info, sOption& option);
	void vCheckScreenSize(int width, int height, int bpp);
	const char* vGetScrResText(int width, int height, int bpp, bool windowed);
	const char* vGetScrResText(int index);
	void vGetScrResLvl(const char* text, int* width, int* height, int* bpp);

	void SetGlobalView(WView* view);
	void SetAudioManager(CSoundManager* audioManager);
	void SetInputManager(CInputManager* inputManager);
	WMilesSoundSystem::w_speaker_type aGetMssSpeaker(void);
	void gSetMouseSensitivity(float sensitivity);
	void vEnableShadow(bool enable);
	void vEnableShadowmap(bool enable);
	bool vIsWindowed(void);
	bool vIsWideMode(void);
	bool vIsShadowEnabled(void);
	bool vIsShadowmapEnabled(void);
	bool vIsReflectionEnabled(void);
	bool vIsLodEnabled(void);
	bool vIsNpcEnabled(void);
	bool aIsMssEnabled(void);
	bool aIsMssHwSoundEnabled(void);
	bool gIsWhisperAllowed(void);
	bool gIsInvitationAllowed(void);
	bool gIsFriendConfirmNeeded(void);
	bool gIsChatWinTransparent(void);
	bool gIsChatLineEntended(void);
	bool gIsPPLEnable(void);
	void gSetPPLEnable(bool enable);
	void gSetPPLSize(int size);
	void gResetRestore(void);
	void SendUpdateGameOptionPacket(void);
	const char* dGetCaptureFormat(void);
	void dSetCaptureFormat(int format);
	bool dIsVtxClrTestEnabled(void);
	bool dIsBaseMeshTestEnabled(void);
	void dSetReplayMode(int mode);
	bool dIsVersionInfoEnabled(void);

	// Inline accessors of clientsetting.h, emitted by the units using them.
	int vGetTnLMode(void) const;
	int vGetParticleLevel(void) const;
	int aGetMssFrequency(void) const;
	int aGetMssBits(void) const;
	int aGetMssChannels(void) const;
	int aGetMssBalance(void) const;
	float aGetSfxVolume(void) const;
	float aGetBgmVolume(void) const;
	float gGetMouseSensitivity(void) const;
	void gSetLastLoginID(const char* id);
	const char* gGetLastLoginID(void) const;
	int gGetPPLSize(void);
	int gGetChatWndUp(void);
	void gSetChatWndUp(int up);
	int gGetUnderExtBtn(void);
	void gSetUnderExtBtn(int extBtn);
	int gGetAvatarNewBie(void);
	void gSetAvatarNewBie(int newBie);
	int gGetCaptureHideGUI(void);
	int gGetCaptureHidePI(void);
	int gGetTerrainTooltip(void);
	const char* mGetMacroText(int index);
	void mSetMacroText(int index, const char* text);
	int mGetSaveMacros(void);
	void mSetPCBangMascotMsg(const char* msg);
	int gGetCutinFlag(void);
	void SetQuitWindowed(bool windowed);
	bool GetQuitWindowed(void);

protected:
	void InitVars(void);
	void Init(void);
	bool ReadReg(int index, char* buffer, int size);
	void WriteReg(int index, const char* value);
	bool ScanReg_I(int index, int* value);
	bool ScanReg_F(int index, float* value);
	bool ScanReg_S(int index, char* value);
	bool WriteReg_I(int index, int value);
	bool WriteReg_F(int index, float value);
	bool WriteReg_S(int index, const char* value);
	bool vChangeScreenSize(int width, int height, int bpp);
	void aApplyAudioSetting(void);
	void SaveReg(void);
	void vApplyMipmap(void);

	static const char* ms_szCaptureFmt[];
	// Registry value names, indexed by ReadReg/WriteReg (63 entries).
	static const char* ms_szReg[][2];

	WRegistry m_registry;          // 0x28
	HWND m_hWnd;                   // 0xB0
	WDeviceManager* m_deviceManager; // 0xB4
	WResourceManager* m_resourceManager; // 0xB8
	WVideoDev* m_videoDevice;      // 0xBC (SetVideoDevice)
	WView* m_globalView;           // 0xC0
	CSoundManager* m_audioManager; // 0xC4
	CInputManager* m_inputManager; // 0xC8
	sOption m_option;              // 0xCC
	int m_tnlMode;                 // 0x3E4
	bool m_changePending;          // 0x3E8
	sOption m_newOption;           // 0x3EC
	bool m_changeFlag;             // 0x704
	bool m_swapMouseButton;        // 0x705
	bool m_quitWindowed;           // 0x706
	PADDING(0x707, 0x708);
	std::list<sDisplayMode> m_displayModes; // 0x708
	int m_captureFormat;           // 0x714
	int m_aviWidth;                // 0x718
	int m_aviHeight;               // 0x71C
	int m_aviFps;                  // 0x720
	int m_startMode;               // 0x724
	int m_lobbyCourse;             // 0x728
	int m_defaultCharacter;        // 0x72C
	int m_defaultCaddie;           // 0x730
	int m_defaultClub;             // 0x734
	int m_defaultBall;             // 0x738
	int m_map;                     // 0x73C
	int m_startHole;               // 0x740 (stored as a dword, read as a byte)
	int m_replayMode;              // 0x744
	int m_numPlayers;              // 0x748
	int m_shotTimeLimitMode;       // 0x74C (dApplyDebugSetting: 0 none, 1 40 s, else seconds)
	int m_weather;                 // 0x750
	int m_chatGender;              // 0x754 (stored as a dword, read as a byte)
	int m_vtxClrTestEnabled;       // 0x758
	int m_baseMeshTestEnabled;     // 0x75C
	int m_clickSystem;             // 0x760
	int m_gameType;                // 0x764
	int m_versionInfoEnabled;      // 0x768
	float m_bar_Y;                 // 0x76C

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(COption)
	CHECK_FIELD(COption, m_registry, 0x28);
	CHECK_FIELD(COption, m_hWnd, 0xB0);
	CHECK_FIELD(COption, m_deviceManager, 0xB4);
	CHECK_FIELD(COption, m_resourceManager, 0xB8);
	CHECK_FIELD(COption, m_videoDevice, 0xBC);
	CHECK_FIELD(COption, m_globalView, 0xC0);
	CHECK_FIELD(COption, m_audioManager, 0xC4);
	CHECK_FIELD(COption, m_inputManager, 0xC8);
	CHECK_FIELD(COption, m_option, 0xCC);
	CHECK_FIELD(COption, m_tnlMode, 0x3E4);
	CHECK_FIELD(COption, m_changePending, 0x3E8);
	CHECK_FIELD(COption, m_newOption, 0x3EC);
	CHECK_FIELD(COption, m_changeFlag, 0x704);
	CHECK_FIELD(COption, m_swapMouseButton, 0x705);
	CHECK_FIELD(COption, m_quitWindowed, 0x706);
	CHECK_FIELD(COption, m_displayModes, 0x708);
	CHECK_FIELD(COption, m_captureFormat, 0x714);
	CHECK_FIELD(COption, m_aviWidth, 0x718);
	CHECK_FIELD(COption, m_aviHeight, 0x71C);
	CHECK_FIELD(COption, m_aviFps, 0x720);
	CHECK_FIELD(COption, m_startMode, 0x724);
	CHECK_FIELD(COption, m_lobbyCourse, 0x728);
	CHECK_FIELD(COption, m_defaultCharacter, 0x72C);
	CHECK_FIELD(COption, m_defaultCaddie, 0x730);
	CHECK_FIELD(COption, m_defaultClub, 0x734);
	CHECK_FIELD(COption, m_defaultBall, 0x738);
	CHECK_FIELD(COption, m_map, 0x73C);
	CHECK_FIELD(COption, m_startHole, 0x740);
	CHECK_FIELD(COption, m_replayMode, 0x744);
	CHECK_FIELD(COption, m_numPlayers, 0x748);
	CHECK_FIELD(COption, m_shotTimeLimitMode, 0x74C);
	CHECK_FIELD(COption, m_weather, 0x750);
	CHECK_FIELD(COption, m_chatGender, 0x754);
	CHECK_FIELD(COption, m_vtxClrTestEnabled, 0x758);
	CHECK_FIELD(COption, m_baseMeshTestEnabled, 0x75C);
	CHECK_FIELD(COption, m_clickSystem, 0x760);
	CHECK_FIELD(COption, m_gameType, 0x764);
	CHECK_FIELD(COption, m_versionInfoEnabled, 0x768);
	CHECK_FIELD(COption, m_bar_Y, 0x76C);
END_LAYOUT_CHECK
CHECK_SIZE(COption::sDisplayMode, 0x1C);

#endif
