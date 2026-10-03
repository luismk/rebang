#pragma once

#include <string.h>
#include <list>
#include "wreg.h"
#include "miles.h"

class CSoundManager;
class CInputManager;
struct sGameOptionInfo;

struct sOption
{
	sOption();
	~sOption();

	static bool IsSupportShadow();
	static bool IsSupportReflection();
	static bool IsSupportLod();

	void vSetOverallByVidOptions();
	void vResetByOverall(const char* text);

	const char* GetThreeLvlText(int lvl);
	const char* GetTwoLvlText(int lvl);
	int vGetLvl3FromText(const char* text);

	const char* vGetOverallText(int lvl);
	int vGetOverallLvl(const char* text);
	const char* vGetTnlText(int lvl);
	int vGetTnlLvl(const char* text);
	const char* vGetTexResText(int lvl);
	int vGetTexResLvl(const char* text);
	const char* vGetParticleText(int lvl);
	int vGetParticleLvl(const char* text);

	const char* aGetOverallText(int lvl);
	int aGetOverallLvl(const char* text);
	const char* aGetSpeakerText(int lvl);
	int aGetSpeakerLvl(const char* text);

	static bool sbHwTnlSupport;
	static bool sbRtSupport;
	static bool sbVsSupport;
	static bool sbPsSupport;
	static bool sbMrtSupport;
	static bool sbClipPlaneSupport;

	int vWindowMode;
	int vFillMode;
	int vWideMode;
	int vScreenWidth;
	int vScreenHeight;
	int vUseMinScreen;
	int vMinScreenWidth;
	int vMinScreenHeight;
	int vScreenColor;
	int vOverall;
	int vTnLMode;
	int vDDSRes;
	int vUseMipmap;
	int vMaxMipLvl;
	int vMipCreateFilter;
	int vEffectLevel;
	int vProjectionShadow;
	int vShadowmap;
	int vReflection;
	int vLod;
	int vNpc;
	int vCoordX;
	int vCoordY;
	float vGamma;

	int aMssOn;
	int aOverall;
	int aMssFreq;
	int aMssBits;
	int aMssChannels;
	int aMssEnableHwSound;
	int aMssBalance;
	int aMssSpeaker;
	float aSfxVolume;
	float aBGMVolume;

	float gMouseSensitivity;
	int gWhisper;
	int gInvitation;
	int gFriendConfirm;
	int gTransChatWin;
	int gUiEffect;
	char gLastLoginID[23];
	int gIdentity;
	int gExtendChatLine;
	int gUserSort;
	int gPPL_Enable;
	int gPPL_Size;
	int gPowerGauge;
	int gCaptureLogo;
	int gRestore;
	int gChatWnd;
	int gUnderTabBtn;
	int gAvatarNewbie;
	int gCaptureHideGUI;
	int gCaptureHidePI;
	int gTerrainTooltip;
	int gCutinDisplay;

	char mMacro[8][64];
	int mSaveMacros;
	char mPCBangMascotMsg[30];
};

class COption : public WSingleton<COption>
{
public:
	struct sDisplayMode
	{
		int width;
		int height;
		int color;
		char text[16];
	};

	COption(HWND__* hWnd, WDeviceManager* pDeviceManager,
		WResourceManager* pResourceManager);
	virtual ~COption();

	void SetVideoDevice(WVideoDev* pVideoDev);
	void SetGlobalView(WView* pView);
	void SetAudioManager(CSoundManager* pAudioManager);
	void SetInputManager(CInputManager* pInputManager);

	const sOption* GetOption();
	void ApplyChange(sOption& option, bool bGameScreen);
	void CheckPendingChanges();
	void ChangeGameOption(sOption& option, sGameOptionInfo& info);
	void ChangeGameOption(sGameOptionInfo& info, sOption& option);
	void SendUpdateGameOptionPacket();
	void CheckDebugRegValid();
	void CheckDisplayModeList(WVideoDev* pVideoDev, bool bWindowed, bool bWide);

	void vToggleFullscreen();
	void vApplyGameScreenSize();
	void vApplyLobbyScreenSize();
	void vChangeGameScreenSize(int width, int height, int color, bool bCheck);
	void vCheckScreenSize(int width, int height, int color);
	void vSetMinimumScreenSize(int bUse, int width, int height);
	int vGetScrResNum();
	const char* vGetScrResText(int index);
	const char* vGetScrResText(int width, int height, int color,
		bool bWindowed);
	void vGetScrResLvl(const char* text, int* width, int* height, int* color);
	void vSetMipmap(int ddsRes, bool bUseMipmap, int maxMipLvl,
		int mipCreateFilter);
	bool vIsWindowed();
	bool vIsWideMode();

	int vGetTnLMode() const { return m_nTnLMode; }
	int vGetParticleLevel() const { return m_opt.vEffectLevel; }
	void vSetParticleLevel(int level);
	void vEnableShadow(bool bEnable);
	void vEnableShadowmap(bool bEnable);
	bool vIsShadowEnabled();
	bool vIsShadowmapEnabled();
	bool vIsReflectionEnabled();
	bool vIsLodEnabled();
	bool vIsNpcEnabled();

	bool aIsMssEnabled();
	bool aIsMssHwSoundEnabled();
	void aApplySfxVolume(float volume);
	void aApplyBgmVolume(float volume);

	int aGetMssFrequency() const { return m_opt.aMssFreq; }
	int aGetMssBits() const { return m_opt.aMssBits; }
	int aGetMssChannels() const { return m_opt.aMssChannels; }
	WMilesSoundSystem::w_speaker_type aGetMssSpeaker();
	int aGetMssBalance() const { return m_opt.aMssBalance; }
	void aSetSpeakerType(int type);
	float aGetSfxVolume() const { return m_opt.aSfxVolume; }
	float aGetBgmVolume() const { return m_opt.aBGMVolume; }
	void aSetSfxVolume(float volume);
	void aSetBgmVolume(float volume);

	float gGetMouseSensitivity() const { return m_opt.gMouseSensitivity; }
	void gSetMouseSensitivity(float sensitivity);
	bool gIsWhisperAllowed();
	bool gIsInvitationAllowed();
	bool gIsFriendConfirmNeeded();
	void gSetLastLoginID(const char* id)
	{
		strncpy(m_opt.gLastLoginID, id, 22);
	}
	const char* gGetLastLoginID() const { return m_opt.gLastLoginID; }
	bool gIsChatWinTransparent();
	bool gIsChatLineEntended();
	bool gIsPPLEnable();
	void gSetPPLEnable(bool bEnable);
	void gSetPPLSize(int size);

	int gGetPPLSize() { return m_opt.gPPL_Size; }
	int gGetRestore();
	void gSetRestore();
	void gResetRestore();
	int gGetChatWndUp() { return m_opt.gChatWnd; }
	void gSetChatWndUp(int up) { m_opt.gChatWnd = up; }
	int gGetUnderExtBtn() { return m_opt.gUnderTabBtn; }
	void gSetUnderExtBtn(int btn) { m_opt.gUnderTabBtn = btn; }
	int gGetAvatarNewBie() { return m_opt.gAvatarNewbie; }
	void gSetAvatarNewBie(int newbie) { m_opt.gAvatarNewbie = newbie; }
	int gGetCaptureHideGUI() { return m_opt.gCaptureHideGUI; }

	int gGetCaptureHidePI() { return m_opt.gCaptureHidePI; }

	float gGetBar_Y();
	int gGetTerrainTooltip() { return m_opt.gTerrainTooltip; }

	const char* mGetMacroText(int index) { return m_opt.mMacro[index]; }
	void mSetMacroText(int index, const char* text)
	{
		strcpy(m_opt.mMacro[index], text);
	}
	int mGetSaveMacros() { return m_opt.mSaveMacros; }
	void mSetPCBangMascotMsg(const char* msg)
	{
		strncpy(m_opt.mPCBangMascotMsg, msg, 29);
	}

	int gGetCutinFlag() { return m_opt.gCutinDisplay; }

	void dApplyDebugSetting();
	const char* dGetCaptureFormat();
	void dSetCaptureFormat(int format);
	int dGetAviWidth();
	int dGetAviHeight();
	int dGetAviFps();
	int dGetStartMode();
	int dGetLobbyCourse();
	int dGetDefaultCharacter();
	int dGetDefaultCaddie();
	int dGetDefaultClub();
	int dGetDefaultBall();
	int dGetMap();
	unsigned char dGetStartHole();
	int dGetReplayMode();
	void dSetReplayMode(int mode);
	int dGetNumPlayers();
	void dSetNumPlayers(int num);
	int dGetWeather();
	void SetQuitWindowed(bool bQuitWindowed)
	{
		m_bQuitWindowed = bQuitWindowed;
	}
	bool GetQuitWindowed() { return m_bQuitWindowed; }
	unsigned char dGetChatGender();
	bool dIsVtxClrTestEnabled();
	bool dIsBaseMeshTestEnabled();
	int dGetClickSystem();
	int dGetGameType();
	bool dIsVersionInfoEnabled();

protected:
	void InitVars();
	void Init();
	void SaveReg();
	bool ReadReg(int index, char* buf, int size);
	void WriteReg(int index, const char* value);
	bool ScanReg_I(int index, int* value);
	bool ScanReg_F(int index, float* value);
	bool ScanReg_S(int index, char* value);
	bool WriteReg_I(int index, int value);
	bool WriteReg_F(int index, float value);
	bool WriteReg_S(int index, const char* value);

	bool vChangeScreenSize(int width, int height, int color);
	void vApplyMipmap();
	void aApplyAudioSetting();

	static const char* ms_szReg[][2];
	static const char* ms_szCaptureFmt[];

	WRegistry m_reg;
	HWND__* m_hWnd;
	WDeviceManager* m_pDeviceManager;
	WResourceManager* m_pResourceManager;
	WVideoDev* m_pVideoDev;
	WView* m_pGlobalView;
	CSoundManager* m_pAudioManager;
	CInputManager* m_pInputManager;
	sOption m_opt;
	int m_nTnLMode;
	bool m_bPendingChange;
	sOption m_newOpt;
	bool m_bGameScreen;
	bool m_bSwapMouseButton;
	bool m_bQuitWindowed;
	std::list<sDisplayMode> m_displayModeList;

	int m_nCaptureFmt;
	int m_nAviWidth;
	int m_nAviHeight;
	int m_nAviFps;
	int m_nStartMode;
	int m_nLobbyCourse;
	int m_nDefaultCharacter;
	int m_nDefaultCaddie;
	int m_nDefaultClub;
	int m_nDefaultBall;
	int m_nMap;
	int m_nStartHole;
	int m_nReplayMode;
	int m_nNumPlayers;
	int m_nGameTime;
	int m_nWeather;
	int m_nChatGender;
	int m_bVtxClrTest;
	int m_bBaseMeshTest;
	int m_nClickSystem;
	int m_nGameType;
	int m_bVersionInfo;
	float m_fBarY;
};

struct sGameOptionInfo
{
	char macro[8][64];
	char reserved[64];
};
