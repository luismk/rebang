// Original: d:\build\custom\temp\code\client\projectg\projectg.cpp
#include <winsock2.h>
#include <windows.h>
#include <float.h>
#include <math.h>
#include <io.h>
#include <stdarg.h>
#include <stdio.h>

// direct.h (not in the trimmed toolchain include set)
extern "C" _CRTIMP int __cdecl _mkdir(const char* dirName);

#include "ClientSetting.h"
#include "DxDiagInfo.h"
#include "ProjectG.h"
#include "NetworkSystem.h"
#include "TaskManager.h"
#include "fpsmonitor.h"
#include "../client/Wangreal/include/westpak.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wdevmng.h"
#include "../client/Wangreal/include/wvideo.h"
#include "../client/Wangreal/include/winput.h"
#include "../client/Wangreal/include/wproc.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "fx.h"
#include "user_info.h"
#include "mousecursor.h"
#include "md5.h"
#include "SharedDoc.h"
#include "PangFBI.h"
#include "PowerGauge.h"
#include "ghostmanager.h"
#include "ghosthandler.h"
#include "voiceitem.h"
#include "ShuffleBonus.h"
#include "Intrusion.h"
#include "fresh.h"
#include "frform.h"
#include "logininfo.h"
#include "messengerdlg.h"
#include "wutil.h"
#include "MessageManager.h"
#include "hackingmanager.h"
#include "buddymanager.h"
#include "GMToolkit.h"
#include "ReplayDlg.h"
#include "cardsystem.h"
#include "commonutil.h"
#include "eventthread.h"
#include "Titles_Client.h"
#include "../shared/TitleManager.h"
#include "../shared/Localize.h"
#include "icontentsdoc.h"
#include "IntegrityCheck.h"
#include "ChatMsg.h"
#include "networkmonitor.h"
#include "headicon.h"
#include "NoticeBoard.h"
#include "WinnerNotice.h"
#include "EffectManager.h"
#include "NetResourceManager.h"
#include "onelineboard.h"
#include "RankingDlg.h"
#include "Treasure.h"
#include "DlgQuickStartSetting.h"
#include "gamedatadb.h"
#include "TopIconManager.h"
#include "lobbybg.h"
#include "frwndmanager.h"
#include "Browser.h"
#include "GolfTask.h"
#include "scenemanager.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/wview.h"
#include "Miles.h"
#include "../shared/luasystem/LuaScriptManager.h"
#include <algorithm>

extern bool g_bQuit;
extern HWND g_hwnd;
extern WMilesSoundSystem* g_miles;
extern unsigned long g_CurrentTime;
extern WFont* g_font;
extern WMatrix g_camera;
extern char PY_PUBLIC_VERSION[];
extern WInputDev* g_keyboard;
extern WInputDev* g_mouse;
extern WInputDev* g_ime;
extern TitleManager* g_TitleManager;

void __cdecl CloseCFile(cFile* file);
extern char g_executeDirectory[];
extern bool g_bFocus;
extern bool g_bOldFocus;
extern bool g_bAviCapture;
extern int g_iSendPacketType;
extern int g_iRecvPacketType;
extern int g_iRecvDisconCode;
extern int g_iSendFailType;
extern int g_last_screen_width;
extern int g_last_screen_height;

// commonutil.cpp
void InitPakedFile(void);
bool InitPakedFile2(void);
bool __stdcall GetCRCCheckList(std::list<std::pair<std::string, unsigned long> >& crcList);

void ExecuteStartMode(void);

// projectg.cpp:184 (windows.h maps TextOut to TextOutA)
void TextOut(WView* view, float x, float y, const char* format, ...) {
	char buffer[2048];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buffer, sizeof(buffer) - 1, format, args) == -1) {
		buffer[sizeof(buffer) - 1] = 0;
	}
	g_font->Print(view, x, y, buffer, 0, 0xFFFFFFFF, 0);
}

// projectg.cpp:198
CFpsMonitor g_FpsMonitor;
// projectg.cpp:199
CFpsMonitor g_VramMonitor;

// Owns a logo object, deleted at exit. The pointee's type is not known (the
// original only deletes it through the destructor in vtable slot 0).
class CTmpLogo {
public:
	// projectg.cpp:214
	CTmpLogo(void) : m_pLogo(0) {}
	// projectg.cpp:215
	~CTmpLogo(void) { if (m_pLogo) delete m_pLogo; }

	WOverlay* m_pLogo; // 0x0
};

// projectg.cpp:220
static CTmpLogo s_tmpLogo;

// projectg.cpp:236 (original sub_00797ff0): captures the back buffer into
// fileName (.jpg or .bmp); true when the file was written.
bool __stdcall SaveScreenShot(const char* fileName);


// dxdiaginfo.h:277
CDxDiagInfo::CDxDiagInfo() {
	Init();
	StartGathering();
}

// dxdiaginfo.h:278
CDxDiagInfo::~CDxDiagInfo() {
}

// dxdiaginfo.h:289
bool CDxDiagInfo::IsGathered(void) {
	return m_isGathered;
}

// clientsetting.h:345 (inlined into SetVideoDevice only, so not emitted here;
// GolfRule emits the out-of-line copy)
inline int COption::vGetTnLMode(void) const {
	return m_tnlMode;
}

// clientsetting.h:375
int COption::aGetMssFrequency(void) const {
	return m_option.m_mssFrequency;
}

// clientsetting.h:376
int COption::aGetMssBits(void) const {
	return m_option.m_mssBits;
}

// clientsetting.h:377
int COption::aGetMssChannels(void) const {
	return m_option.m_mssChannels;
}

// clientsetting.h:379
int COption::aGetMssBalance(void) const {
	return m_option.m_mssBalance;
}

// clientsetting.h:381 (inlined into SetAudioDevice only, so not emitted here;
// MagicCarpet emits the out-of-line copy)
inline float COption::aGetSfxVolume(void) const {
	return m_option.m_sfxVolume;
}

// clientsetting.h:382 (likewise)
inline float COption::aGetBgmVolume(void) const {
	return m_option.m_bgmVolume;
}

// clientsetting.h:410
int COption::gGetCaptureHideGUI(void) {
	return m_option.m_captureHideGUI;
}

// clientsetting.h:412
int COption::gGetCaptureHidePI(void) {
	return m_option.m_captureHidePI;
}

// user_info.h:682
CMatchHistory::CMatchHistory(void) : m_bUpdated(false) {
	memset(m_history, 0, sizeof(m_history));
}

// user_info.h:685
CMatchHistory::~CMatchHistory(void) {
}

// projectg.cpp:349
void InitSingletonClasses(void) {
	new CPowerGauge;

	new CMatchHistory;
	new CGhostManager;
	new CGhostHandler;
	new CSoundItemManager;
	new CShuffleBonus;
	new CIntrusion;
}

// projectg.cpp:361
void DeleteSingletonClasses(void) {
	delete CIntrusion::Instance();
	delete CShuffleBonus::Instance();
	delete CSoundItemManager::Instance();
	delete CGhostHandler::Instance();
	delete CGhostManager::Instance();
	delete CMatchHistory::Instance();

	delete CPowerGauge::Instance();
}

// projectg.cpp:374
CProjectG::CProjectG(HWND hWnd) : m_mainFlags(0), m_appFlags(0) {
	g_CurrentTime = timeGetTime();

	srand(g_CurrentTime);

	m_mainThreadId = GetCurrentThreadId();

	m_deviceManager = NULL;
	m_procManager = NULL;
	m_bMoveLoginServer = false;
	m_flagE4 = false;

	m_deviceManager = new WDeviceManager(NULL, 9);
	m_procManager = new WProcManager;
	g_resrcmng = new WResourceManager;

	WFile::SetResourceManager(g_resrcmng);

	m_createTime = g_CurrentTime;
	m_valueF0 = 4;

	new _systemmsg::CMessageManager;

	new CSharedDoc;
	new COption(hWnd, m_deviceManager, g_resrcmng);
	new HackingManager;

	ResetTimer();

	m_monitorFPS = 60.0f;
	new CTaskManager;

	new BuddyManager;
	new CGMToolkit;

	if (IsLocalContent((localContentType_t)0x4F))
		new CReplayControlManager;

	if (IsLocalContent((localContentType_t)0x51))
		new CCardManager;

	new CPangFBI;

	m_hidePrivacy = false;
	m_hideGUI = false;
	m_hidePI = false;
	m_dxDiagInfo = NULL;
	m_dxDiagInfo = new CDxDiagInfo;

	g_TitleManager = new TitleManager;
	RegisterTitles(g_TitleManager);

	new CEventThread;

	m_matrix.Reset();

	InitSingletonClasses();
}

// projectg.cpp:615
void CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT_PTR idTimer, DWORD time) {
}

// projectg.cpp:637
void CProjectG::FPU_Check(void) {
	m_fpuControl = _controlfp(0, 0);
	if (m_fpuControl != (_PC_24 | _MCW_EM)) {
		m_fpuControl = _controlfp(_PC_24, _MCW_PC);
	}
}

// projectg.cpp:647
void CProjectG::SetMoveLoginServer(const char* id, const char* password) {
	m_bMoveLoginServer = true;
	m_moveLoginId = id;
	m_moveLoginPassword = password;
}

// projectg.cpp:654
bool CProjectG::Ready(void) {
	FPU_Check();
	SetCursor(NULL);

#ifdef PG_OFFLINE
	// The offline CMake build (PG_OFFLINE) has no launcher and no update
	// list: it loads the projectg*.pak of its directory without the CRC check
	// of InitPakedFile2.
	InitPakedFile();
#else
	if (IsLocalContent((localContentType_t)0xE)) {
		if (!InitPakedFile2())
			return false;
	} else {
		InitPakedFile();
	}
#endif

	CSharedDoc::Instance()->LoadItemDb();
	CSharedDoc::Instance()->InitGolfDoc();
	CSharedDoc::Instance()->LoadChatFilter();
	CSharedDoc::Instance()->LoadVisGroup("visgroup.def");

	new CContentsDoc;
	if (!CContentsDoc::Instance()->Initialize())
		return false;

	DeleteFile("GameGuard/GameGuard.err");

	char commandLine[MAX_PATH];
	strcpy(commandLine, GetCommandLine());
	strlwr(commandLine);

	if (strstr(commandLine, "-integrity")) {
		CIntegrityCheck integrityCheck;
		if (!integrityCheck.Report("ResrcIntegrity.txt"))
			g_bQuit = true;
	}

	GetCurrentDirectory(MAX_PATH, g_executeDirectory);

	g_view = g_resrcmng->GetView(NULL);
	g_input = new CInputManager;

	COption::Instance()->CheckDebugRegValid();

	COption::Instance()->SetVideoDevice(g_resrcmng->VideoReference());
	COption::Instance()->SetGlobalView(g_view);
	COption::Instance()->SetAudioManager(g_audio);
	COption::Instance()->SetInputManager(g_input);

	UploadShaderSource();

	g_font = g_resrcmng->GetTexFont("data/Misc/font.bmp");
	LoadKeyLayout();

	new CChatMsg;
	new WNetworkSystem;
	new CNetworkMonitor;

	new CMouseCursor;
	new CHeadIcon;
	new CUserInfo;
	new CMessengerInfo;
	new CNoticeBoard;

	new CWinnerNotice;

	new CFx;
	new CAztecHoleinEffect;
	new NetResourceManager;
	new COneLineBoard;
	new CRankingInfo;

	// The original calls CAztecHoleinEffect's constructor here too (a class
	// of the same size whose identical constructor the linker folded).
	if (IsLocalContent((localContentType_t)0x55))
		new CAztecHoleinEffect;

	if (IsLocalContent((localContentType_t)0x58)) {
		new CTHunter;
		CTHunter::Instance()->Initialize(20);
	}

	if (IsLocalContent((localContentType_t)0x59))
		new CMatchingSystem;

	new CGameDataDB;

	new CIconManager;

	new CVoiceItem;
	CVoiceItem::Instance()->Initialize();

	g_pFresh = new Fresh;
	g_pFresh->Init("all.xml", true, NULL);
	g_pFresh->GetManager()->CreateToolTip();

	COption::Instance()->dApplyDebugSetting();

	CNetworkMonitor::Instance()->Init();

	g_FpsMonitor.Init(200, 200, WRect(10.0f, 300.0f, 200, 100));

	g_VramMonitor.Init(200, 600, WRect(10.0f, 300.0f, 200, 100));
	g_VramMonitor.SetColor(0, 0xFF0000FF);

	int lobbyCourse = COption::Instance()->dGetLobbyCourse();
	if (lobbyCourse >= 0) {
		LOBBYBG_MAP = lobbyCourse;
		IFF_STRUCT::sCourse* course = ItemManager()->FindCourse(0x28000000 | (lobbyCourse > 0x7F && lobbyCourse != 0xFD ? lobbyCourse - 0x80 : lobbyCourse));
		LOBBYBG_MAPNAME = course->m_mapName;
	}

	ExecuteStartMode();

#ifndef PG_OFFLINE
	// The offline CMake build (PG_OFFLINE) has no launcher registry key and
	// no update list: no background CRC check of the paks.
	SYSTEM_INFO systemInfo;
	memset(&systemInfo, 0, sizeof(systemInfo));
	GetSystemInfo(&systemInfo);
	if (systemInfo.dwNumberOfProcessors > 1) {
		std::list<std::pair<std::string, unsigned long> > crcList;
		if (!GetCRCCheckList(crcList))
			return false;

		for (std::list<std::pair<std::string, unsigned long> >::iterator it = crcList.begin(); it != crcList.end(); ++it)
			m_crcMT.AddWork(it->first.c_str(), it->second);

		m_crcMT.SetSleepTime(1);
		m_crcMT.Begin(g_hwnd);
	}
#endif

	return true;
}

// projectg.cpp:865
void ExecuteStartMode(void) {
	CTaskManager::Instance()->CreateTask("CLobbyTask", "CGolfDoc", false);
	CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"LOGIN", 0, 0, 0);
}

// projectg.cpp:950
void CProjectG::LoadKeyLayout(void) {
	// A key binding: its name, its key code, KEY_* flags and whether the
	// binding records its event time.
	struct KeyLayout {
		const char *name;          // 0x0
		int code;                  // 0x4
		unsigned long flags;       // 0x8, CInputManager::KEY_*
		int timed;                 // 0xC, 1: the binding records its event time
	};

	KeyLayout layout[] = {
		{ "\xBB\xF3", 0x06 },
		{ "\xC7\xCF", 0x07 },
		{ "\xC1\xC2", 0x04 },
		{ "\xBF\xEC", 0x05 },
		{ "\xBB\xF3", 0x57 },
		{ "\xC7\xCF", 0x53 },
		{ "\xC1\xC2", 0x41 },
		{ "\xBF\xEC", 0x44 },
		{ "LSHIFT", 0x81, 1 },
		{ "RSHIFT", 0x82, 1 },
		{ "\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9", 0x20 },
		{ "PAGE_DOWN", 0x03, 1 },
		{ "PAD_PLUS", 0x8C },
		{ "PAGE_UP", 0x02, 1 },
		{ "PAD_MINUS", 0x88 },
		{ "\xB5\xF0\xB9\xF6\xB1\xD7\xC5\xB0", 0x08 },
		{ "HOME", 0x0C },
		{ "END", 0x1A },
		{ "PAD_0", 0x90 },
		{ "NUM_0", 0x30 },
		{ "PAD_1", 0x8D },
		{ "NUM_1", 0x31 },
		{ "PAD_2", 0x8E },
		{ "NUM_2", 0x32 },
		{ "PAD_3", 0x8F },
		{ "NUM_3", 0x33 },
		{ "PAD_4", 0x89 },
		{ "NUM_4", 0x34 },
		{ "PAD_5", 0x8A },
		{ "NUM_5", 0x35 },
		{ "PAD_6", 0x8B },
		{ "NUM_6", 0x36 },
		{ "PAD_7", 0x85 },
		{ "NUM_7", 0x37 },
		{ "PAD_8", 0x86 },
		{ "NUM_8", 0x38 },
		{ "PAD_9", 0x87 },
		{ "NUM_9", 0x39 },
		{ "ITEMSLOT", 0x49 },
		{ "\xC1\xA4\xC1\xF6", 0x50 },
		{ "LALT", 0x1C, 1 },
		{ "RALT", 0x1D, 1 },
		{ "LCONTROL", 0x1E },
		{ "RCONTROL", 0x1F },
		{ "TAB", 0x09, 1 },
		{ "FASTFORWARD", 0x20 },
		{ "\xC5\xD7\xBD\xBA\xC6\xAE", 0x54 },
		{ "PARAM_UPDATE", 0x55 },
		{ "CONSOLE", 0x0A, 1 },
		{ "F15", 0x66, 1 },
		{ "FUNC_1", 0x0E, 1 },
		{ "FUNC_2", 0x0F, 1 },
		{ "FUNC_3", 0x10, 1 },
		{ "FUNC_4", 0x11, 1 },
		{ "FUNC_5", 0x12, 1 },
		{ "FUNC_6", 0x13, 1 },
		{ "FUNC_7", 0x14, 1 },
		{ "FUNC_8", 0x15, 1 },
		{ "FUNC_9", 0x16, 1 },
		{ "FUNC_10", 0x17, 1 },
		{ "FUNC_11", 0x18, 1 },
		{ "FUNC_12", 0x19, 1 },
		{ "FUNC_15", 0x66, 1 },
		{ "INSERT", 0x7F, 1 },
		{ "ESCAPE", 0x1B, 1 },
		{ "ENTER", 0x0D, 1 },
		{ "PADENTER", 0x91, 1 },
		{ "HIDE", 0x48 },
		{ "JUKEBOX", 0x4D },
		{ "REPLAY", 0x52 },
		{ "SKIP", 0x53 },
		{ "CANCEL", 0x43 },
		{ "FPS_SHOT", 0x46 },
		{ "BALL_TRACE", 0x47 },
		{ "PARTICLE_1", 0x2E },
		{ "PARTICLE_2", 0x2C },
		{ "\xB8\xC5\xC5\xA9\xB7\xCE\xC3\xA2", 0x60 },
		{ "\xC1\xA4\xC1\xF6" "2", 0x56 },
		{ "EMOTICON", 0x45 },
		{ "BANISH", 0x01, 1 },
		{ "RANK", 0x51 },
		{ "ROTATE_CW", 0x51 },
		{ "MESSENGER", 0x7F, 1 },
		{ "ROTATE_CCW", 0x45 },
		{ "DEBUG_INFO", 0x92, 1 },
		{ "PLAYER_INFO", 0x93, 0, 0 },
		{ "CALIPERS_RIGHT", 0x58 },
		{ "CALIPERS_LEFT", 0x5A },
		{ "\xB0\xED\xBC\xD3", 0x4E },
		{ "\xC0\xFA\xBC\xD3", 0x42 },
		{ "UNDO", 0x5A },
		{ "REDO", 0x59 },
		{ "FLIP", 0x46 },
		{ "SAVE", 0x53 },
		{ "PAINT", 0x50 },
		{ "GRID", 0x47 },
		{ "DRAG", 0x44 },
		{ NULL },
	};

	for (int i = 0; layout[i].name != NULL; i++) {
		g_input->Register(layout[i].name, layout[i].code, layout[i].flags, layout[i].timed == 1 ? 3 : 2);
	}

	g_input->ExcludeKey("BALL_TRACE");

	g_input->ExcludeKey("PARAM_UPDATE");
	g_input->ExcludeKey("FPS_SHOT");
	g_input->ExcludeKey("\xC5\xD7\xBD\xBA\xC6\xAE");
	g_input->ExcludeKey("\xB5\xF0\xB9\xF6\xB1\xD7\xC5\xB0");
	g_input->ExcludeKey("PARTICLE_1");
	g_input->ExcludeKey("PARTICLE_2");
	g_input->ExcludeKey("\xC1\xA4\xC1\xF6");
	g_input->ExcludeKey("\xC1\xA4\xC1\xF6" "2");
	g_input->ExcludeKey("\xB0\xED\xBC\xD3");
	g_input->ExcludeKey("\xC0\xFA\xBC\xD3");
}

// projectg.cpp:1112 (the original is inline asm: reads PEB->BeingDebugged)
int MainLoop(void) {
	int result = 1;

	__asm {
		mov eax, fs:[18h]
		mov eax, [eax+30h]
		movzx eax, byte ptr [eax+2]
		mov result, eax
	}

	return result;
}

// projectg.cpp:1126
int CProjectG::MainLoop(int) {
	FPU_Check();

	g_CurrentTime = timeGetTime();

	WNetworkSystem::Instance()->Process();

	ProcessRecvPacketQueue();

	bool active = ((m_appFlags >> 1) & 1) != 0;
	int flags = m_mainFlags;
	if (!active) flags |= 0x80;
	return Main(flags);
}

// The screenshot notice: CheckScreenShot writes the message and the time it
// was taken; Draw shows it for a second.
static char s_screenShotMessage[1024];        // 0xB7BB18
static unsigned long s_screenShotTime;        // 0xB7BF74

// projectg.cpp:1281
void CProjectG::CheckScreenShot(void) {
	// Frames left before the capture is taken (-1: no capture pending).
	static int s_captureDelay = -1;

	if (g_input->GetDown("FUNC_11", true) || CSharedDoc::Instance()->m_replayState == 6) {
		if (IsLocalContent((localContentType_t)0x4F) && (CSharedDoc::Instance()->m_replayState == 6 || CSharedDoc::Instance()->m_replayState == 9)) {
			__int64 freeSpace = CalcHddFreeSpace();
			if (freeSpace == 0) {
				CChatMsg::Instance()->AddChatMsg(std::string("\xc7\xcf\xb5\xe5\xc0\xc7 \xbf\xeb\xb7\xae\xc0\xbb \xc3\xbc\xc5\xa9 \xc7\xd2 \xbc\xf6 \xbe\xf8\xc0\xb8\xb9\xc7\xb7\xce \xb5\xbf\xbf\xb5\xbb\xf3 \xc6\xc4\xc0\xcf\xc0\xbb \xbb\xfd\xbc\xba \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9."), 0xFFFF7878, true, false);
				return;
			} else if (freeSpace < 0x40000000) {
				CChatMsg::Instance()->AddChatMsg(std::string("\xc7\xcf\xb5\xe5\xc0\xc7 \xbf\xeb\xb7\xae\xc0\xcc \xba\xce\xc1\xb7\xc7\xcf\xbf\xa9 avi\xc6\xc4\xc0\xcf\xc0\xbb \xbb\xfd\xbc\xba\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9."), 0xFFFF7878, true, false);
				return;
			}

			COption::Instance()->dSetCaptureFormat(2);

			if (CSharedDoc::Instance()->m_replayState == 6)
				CSharedDoc::Instance()->m_replayState = 7;
		} else {
			COption::Instance()->dSetCaptureFormat(0);
		}

		m_hideGUI = COption::Instance()->gGetCaptureHideGUI() == 1;
		m_hidePI = COption::Instance()->gGetCaptureHidePI() == 1;

		if (g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false) || m_hidePI) {
			m_hidePrivacy = true;
			g_pFresh->HidePrivacy(true);
		}

		s_captureDelay = 3;
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F9, 0, 0, 0, 0, 0));
	}

	if (s_captureDelay > 0)
		s_captureDelay--;

	if (s_captureDelay != 0)
		return;

	m_hidePrivacy = false;

	s_captureDelay = -1;
	g_pFresh->HidePrivacy(false);

	char tag[4] = "";
	char description[64] = "";
	if (m_hideGUI) {
		strcat(tag, "G");
		strcat(description, "(GUI");
		if (!m_hidePI) {
			strcat(description, "\xc1\xa6\xb0\xc5)");
		}
	}

	if (m_hidePI) {
		strcat(tag, "U");
		if (m_hideGUI)
			strcat(description, ", ");
		else
			strcat(description, "(");

		strcat(description, "\xb0\xb3\xc0\xce\xc1\xa4\xba\xb8\xc1\xa6\xb0\xc5)");
	}

	m_hideGUI = false;
	m_hidePI = false;

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F9, 0, 0, 0, 0, 0));

	const char* format = COption::Instance()->dGetCaptureFormat();

	_mkdir("capture");

	char fileName[64];
	do {
		sprintf(fileName, "capture/pangya%s_%03d.%s", tag, m_captureNum++, format);
	} while (_access(fileName, 0) != -1);

	if (_stricmp(format, "avi") == 0) {
		CSharedDoc::Instance()->m_replayState = 7;
		if (m_aviEncoder.IsOpened()) {
			m_aviEncoder.Close();

			m_mainFlags &= ~0x22;
			m_appFlags &= ~2;
			SetFPS(m_monitorFPS);

			if (IsLocalContent((localContentType_t)0x4F))
				CSharedDoc::Instance()->m_replayState = 0;

			SetGameSpeed(1.0f);
			COption::Instance()->dSetCaptureFormat(0);
		} else {
			int width = COption::Instance()->dGetAviWidth();
			int height = COption::Instance()->dGetAviHeight();
			int fps = COption::Instance()->dGetAviFps();

			if (m_aviEncoder.Open(fileName, width, height, fps, g_hwnd)) {
				g_input->SetFakeInput("\xbd\xba\xc6\xe4\xc0\xcc\xbd\xba\xb9\xd9", false);
				SetFPS((float)fps);
				m_appFlags |= 2;
				m_mainFlags |= 0x22;
			}
		}
		return;
	}

	if (COption::Instance()->gGetCaptureHidePI() == 0 && CSharedDoc::Instance()->m_playingMode != (eReplayModeType)2) {
		FILETIME fileTime;
		SystemTimeToFileTime(&CSharedDoc::Instance()->GetServerTime(), &fileTime);

		ULARGE_INTEGER time;
		time.LowPart = fileTime.dwLowDateTime;
		time.HighPart = fileTime.dwHighDateTime;
		time.QuadPart += (unsigned __int64)(GetTickCount() - CSharedDoc::Instance()->m_serverTimeTick) * 10000;
		fileTime.dwLowDateTime = time.LowPart;
		fileTime.dwHighDateTime = time.HighPart;

		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);

		char timeText[128];
		sprintf(timeText, "%d-%02d-%02d %02d:%02d:%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

		if (CSharedDoc::Instance()->m_loginConnectValue != 0) {
			if (CSharedDoc::Instance()->m_serverName[0] != '\0') {
				strcat(timeText, " (");
				strcat(timeText, CSharedDoc::Instance()->m_serverName);
				strcat(timeText, ")");
			}
		}

		Draw();

		FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
		if (gdi) {
			gdi->SetTextColor(0xFFFFFFFF, 0x88888888);
			gdi->SetTextStyle(2);
			gdi->Print(WPoint(5.0f, g_view->GetHeight() - 13.0f), 0, timeText);
			gdi->SetTextStyle(0);
		}

		g_view->EndScene();

		g_resrcmng->VideoReference()->EndScene();
	}

	if (SaveScreenShot(fileName))
		sprintf(s_screenShotMessage, "\xc8\xad\xb8\xe9%s\xc0\xcc %s\xbf\xa1 \xc0\xfa\xc0\xe5\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", description, fileName);
	else
		sprintf(s_screenShotMessage, "\xbd\xba\xc5\xa9\xb8\xb0\xbc\xa6 \xc0\xfa\xc0\xe5\xc0\xcc \xbd\xc7\xc6\xd0 \xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.");

	s_screenShotTime = timeGetTime();
}

// projectg.cpp:1531
void CheckQuickEscape(void) {
}

// projectg.cpp:1535
void CheckDebugKey(void) {
}

// projectg.cpp:1543
void CProjectG::OpenDisconDlg(const char* message, bool gameServer) {
	if (g_pFresh) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		if (pForm) {
			pForm->SetMessage(message, false);

			if (gameServer == true) {
				pForm->Open((FRESH_RESULT_CALLBACK)&CProjectG::OnGameServerResult, 0x203);
			} else {
				pForm->Open((FRESH_RESULT_CALLBACK)&CProjectG::OnLoginServerResult, 0x203);
			}
		}
	} else {
		MessageBox(0, message, "\xBE\xCB\xB8\xB2", 0);

		if (gameServer == true) {
			OnGameServerResult(1, 0);
		} else {
			g_bQuit = true;
		}
	}
}

// projectg.cpp:1577
void CProjectG::FromGolfToLobbySettings(void) {
	if (g_input) {
		g_input->FreeKey();
		g_input->FreeMouse();
	}

	if (g_audio)
		g_audio->StopAllSound();

	if (WSingleton<CFx>::IsInstantiated())
		WSingleton<CFx>::Instance()->ClearAll(false);

	if (WSingleton<CUserInfo>::IsInstantiated())
		WSingleton<CUserInfo>::Instance()->Close();

	if (WSingleton<CMouseCursor>::IsInstantiated())
		WSingleton<CMouseCursor>::Instance()->m_mode = CMouseCursor::SCREEN_LOBBY;

	g_resrcmng->VideoReference()->Command(W_VDEV_RELEASE_CAPTURERESOURCE, 0, 0);
}

// projectg.cpp:1600
void CProjectG::Process(float seconds) {
	g_input->Update();

	if (!WisEqual(m_gameSpeed, 1.0f, 0.00001f) && !((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
		m_gameSpeed = 1.0f;

	WSingleton<CNetworkMonitor>::Instance()->Process(seconds);

	g_FpsMonitor.Trace(seconds == 0.0f ? -1 : (int)(1.0f / seconds));
	g_FpsMonitor.Process(0);

	// Video memory used since the first frame, in units of 20000 bytes.
	int availVram = g_resrcmng->VideoReference()->Command(W_VDEV_GETAVAIL_VRAM, 0, 0);
	static int s_firstAvailVram = -1;
	if (s_firstAvailVram == -1)
		s_firstAvailVram = availVram;
	int usedVram = s_firstAvailVram - availVram;
	g_VramMonitor.Trace(usedVram / 20000);
	g_VramMonitor.Process(0);

	seconds *= m_gameSpeed;

	// AVI capture: the game advances by fixed 1/15 s steps.
	static float s_aviTime = 0.0f;
	if (IsLocalContent((localContentType_t)0x4F)) {
		bool active = ((m_appFlags >> 1) & 1) != 0;
		if (active) {
			s_aviTime += seconds;

			if (seconds > 1.0f / 15.0f) {
				seconds = 1.0f / 15.0f;
				m_gameSpeed = 0;
				s_aviTime = 0;
				g_bAviCapture = true;
			} else if (s_aviTime > 1.0f / 15.0f) {
				seconds = seconds - (s_aviTime - 1.0f / 15.0f);
				m_gameSpeed = 0;
				s_aviTime = 0;
				g_bAviCapture = true;
			}
		}
	}

	CTaskManager::Instance()->PreProcess();

	if (CSharedDoc::Instance()->m_replayState != 7) {
		if (g_input->GetDown("MESSENGER", true) && WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_DEFAULT)->m_bConnected != 0)
			CMessengerInfo::Instance()->Open(true);
	}

	CMouseCursor::Instance()->Process(seconds);
	CMessengerInfo::Instance()->Process(seconds);
	CTaskManager::Instance()->ProcessTask(seconds);
	CUserInfo::Instance()->Process(seconds);
	CRankingInfo::Instance()->Process(seconds);
	CNoticeBoard::Instance()->Process(seconds);
	CWinnerNotice::Instance()->Process(seconds);
	CFx::Instance()->Process(seconds);
	CBrowser::Instance()->Process(seconds);
	COneLineBoard::Instance()->Process(seconds);
	lua_system::lua_script_manger::GetInstance()->Update(seconds);

	g_audio->Process(seconds, g_view);
	if (g_miles)
		WMilesSoundSystem::Process();

	COption::Instance()->CheckPendingChanges();

	CheckScreenShot();

	if (m_dxDiagInfo && !m_dxDiagInfo->IsGathered())
		m_dxDiagInfo->GatherInfo();

	if (g_bQuit)
		Quit();

	static int s_frameCount = 0;
	s_frameCount++;
	if (g_bOldFocus != g_bFocus) {
		g_bOldFocus = g_bFocus;
		if (COption::Instance()->GetOption()->m_windowed == 0)
			g_mouse->InitDevice(g_hwnd, g_bFocus);
	}

	if (!g_bFocus) {
		if (WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_DEFAULT)->GetRcvCnt() < 5000)
			Sleep(50);
	} else if (CBrowser::Instance()->IsOpen()) {
		Sleep(10);
	}
}

// projectg.cpp:1973
void CProjectG::ReportError(int code, const char* format, ...) {
	char message[256] = { 0 };

	va_list args;
	va_start(args, format);

	if (_vsnprintf(message, sizeof(message) - 1, format, args) == -1)
		return;

	char report[512] = { 0 };
	const char* nickname = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname;
	_snprintf(report, sizeof(report) - 1, "%d %s %s %s %s", code, strlen(nickname) == 0 ? "unknown" : nickname, PY_PUBLIC_VERSION, "645.00", message);

	WSingleton<CPangFBI>::Instance()->ProcessReport("app_log", report);
}

// projectg.cpp:1991
void CProjectG::GameGuardLog(const char* format, ...) {
	char buffer[256];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buffer, sizeof(buffer) - 1, format, args) == -1) {
		buffer[sizeof(buffer) - 1] = 0;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);

	FILE* file = fopen("GameGuard/GameGuard.err", "a+");

	if (file) {
		fprintf(file, "%d-%02d-%02d %02d:%02d:%02d.%03d %s\n",
			time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
			buffer);
		fclose(file);
	}
}

// projectg.cpp:2015
bool CProjectG::HidePrivacy(void) {
	return m_hidePrivacy;
}

// projectg.cpp:2020
bool CProjectG::HideGUI(void) {
	return m_hideGUI;
}

// projectg.cpp:2025
bool CProjectG::HidePI(void) {
	return m_hidePI;
}

// A UI image whose content CheckUIHack verifies.
struct sUIHackFile {
	const char* fileName;      // 0x0
	unsigned char digest[16];  // 0x4
};

static sUIHackFile s_uiHackFiles[] = {
	{ "[left_ht.jpg", { 0x5b, 0xa3, 0x60, 0x07, 0x2f, 0x73, 0x87, 0x21, 0xef, 0xbd, 0xcd, 0xd4, 0x45, 0x23, 0xfc, 0xfa } },
	{ "[middle.jpg", { 0x7b, 0x95, 0xfc, 0xe6, 0x44, 0xab, 0xd1, 0x81, 0xa8, 0x53, 0xd1, 0x56, 0xf9, 0xf4, 0xa2, 0x8f } },
	{ "[right_wind_b.jpg", { 0xed, 0xdd, 0x43, 0xfd, 0x5e, 0x65, 0x91, 0x86, 0x15, 0x1c, 0x80, 0x79, 0x32, 0x41, 0x77, 0x1b } },
};

// projectg.cpp:2030 (original sub_007975d0): true when the resource file's
// MD5 equals the expected digest.
static bool __stdcall CheckFileDigest(const char* fileName, const unsigned char* digest) {
	unsigned char result[16];
	MD5Context context;

	cFile* file = g_resrcmng->GetCFile(fileName, 0xFFFF);

	if (file == NULL)
		return false;

	int length = file->Length();
	unsigned char* buffer = new unsigned char[length];
	file->Read(buffer, length);
	CloseCFile(file);

	MD5Init(&context);
	MD5Update(&context, buffer, length);
	MD5Final(result, &context);

	delete[] buffer;

	for (int i = 0; i < 16; i++) {
		if (digest[i] != result[i])
			return false;
	}

	return true;
}

// projectg.cpp:2062
bool CProjectG::CheckUIHack(void) {
#ifdef PG_FOREIGN_PAKS
	// The paks are another client's (CMake PG_PAK_REGION other than KR): its
	// GUI images differ from the 2011 KR ones the digests describe, so the
	// check would always report a modified file.
	return true;
#endif
	sUIHackFile* file = s_uiHackFiles;

	for (unsigned int i = 0; i < sizeof(s_uiHackFiles) / sizeof(s_uiHackFiles[0]); i++, file++) {
		if (!CheckFileDigest(file->fileName, file->digest))
			return false;
	}

	return true;
}

// projectg.cpp:2074
void CProjectG::ResetCaptuerNum(void) {
	m_captureNum = 0;
}

// Frames drawn since start.
static unsigned long s_drawnFrameCount;       // 0xB7BF58

// projectg.cpp:2079
void CProjectG::Draw(void) {
	if (CBrowser::Instance()->IsOpen())
		return;

	WMatrix camera = g_camera;

	unsigned char fadeAlpha = 0;
	if (CFx::Instance())
		CFx::Instance()->GetOutput(camera.pivot, fadeAlpha);

	g_view->SetCamera(camera);

	g_view->BeginScene();

	g_view->Clear(0, 2);

	g_view->SetPrevCamera(m_matrix);
	m_matrix = g_view->GetCamera();

	CTaskManager::Instance()->DisplayTask();

	CMouseCursor::Instance()->Display();
	CNoticeBoard::Instance()->Display();

	CWinnerNotice::Instance()->Display();

	if (!CSceneManager::Instance())
		CFx::Instance()->Display();
	CBrowser::Instance()->Display();

	if (IsLocalContent((localContentType_t)0x4F) && CSharedDoc::Instance()->m_playingMode == 1 && ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGolfTask::m_RTTI) : false) && (CSharedDoc::Instance()->m_replayPlayType == 1 || CSharedDoc::Instance()->m_replayPlayType == 2) && CSharedDoc::Instance()->m_roomInfo.m_roomMode != 14) {
		FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
		gdi->SetTextStyle(2);
		gdi->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
		if (CSharedDoc::Instance()->m_replayPlayType == 1)
			gdi->Print(WPoint(300.0f, g_view->GetHeight() - 40.0f), 0, "\xb8\xae\xc7\xc3\xb7\xb9\xc0\xcc\xb0\xa1 \xc0\xfa\xc0\xe5\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.(\xbc\xa6)");
		else if (CSharedDoc::Instance()->m_replayPlayType == 2)
			gdi->Print(WPoint(300.0f, g_view->GetHeight() - 40.0f), 0, "\xb8\xae\xc7\xc3\xb7\xb9\xc0\xcc\xb0\xa1 \xc0\xfa\xc0\xe5\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.");
		gdi->SetTextStyle(0);
	}

	if (timeGetTime() - s_screenShotTime < 1000) {
		if (CChatMsg::Instance()) {
			WFont* font = CChatMsg::Instance()->GetMaskedFont();
			font->Print(g_view, g_view->GetWidth() * 0.5f - font->GetTextWidth(g_view, s_screenShotMessage) * 0.5f, g_view->GetHeight() - 25.0f, s_screenShotMessage, 0, 0xFFFF0000, 0);
			CChatMsg::Instance()->GetMaskedFont()->Flush(g_view);
		}
	}

	if (fadeAlpha)
		WOverlay::DrawBox(g_view, WRect(0.0f, 0.0f, g_view->GetWidth(), g_view->GetHeight()), 0, (fadeAlpha << 24) | 0xFFFFFF, 0.001f);

	s_drawnFrameCount++;
}

// projectg.cpp:2218
void CProjectG::Paint(void) {
	if (CBrowser::Instance()->IsOpen())
		return;

	g_view->EndScene();

	g_view->Render();
	g_last_screen_width = (int)g_view->GetWidth();
	g_last_screen_height = (int)g_view->GetHeight();

	WVideoDev* videoDevice = g_view->GetVideoDevice();
	if (videoDevice) {
		int state = videoDevice->GetDeviceState();

		if (state == 1 && g_bFocus) {
			g_bFocus = false;
			m_flagE4 = true;
		} else if (m_flagE4 && state == 0 && !g_bFocus) {
			g_bFocus = true;
			m_flagE4 = false;
		}
	}

	bool active = ((m_appFlags >> 1) & 1) != 0;
	if (active && g_bAviCapture) {
		static Bitmap s_captureBitmap(g_last_screen_width, g_last_screen_height, 24);
		if (g_resrcmng->VideoReference()->Command(W_VDEV_CAPTURE_SCREEN, 0, (int)&s_captureBitmap) == 1) {
			m_aviEncoder.Write(&s_captureBitmap);
			SetGameSpeed(1.0f);
			g_bAviCapture = false;
		}
	}
}

// projectg.cpp:2358
long __stdcall CProjectG::WinProc(HWND hWnd, unsigned int message, unsigned int wParam, long lParam) {
	switch (message) {
	case 0xADB3:
		if (m_crcMT.IsWorking())
			m_crcMT.Pause();
		break;

	case 0xADB4:
		if (m_crcMT.IsWorking())
			m_crcMT.Resume();
		break;

	case 0xADB2:
		if (wParam == 0) {
			std::string fileName((const char*)lParam);
			// "%s file is damaged. Please run the game again." / "PangYa error"
			MessageBox(NULL, MakeStr("%s\xc6\xc4\xc0\xcf\xbf\xa1 \xbc\xd5\xbb\xf3\xc0\xcc \xb9\xdf\xb0\xdf\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xbd\xc3 \xbd\xc7\xc7\xe0\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", fileName.c_str()),
				"\xc6\xce\xbe\xdf\xbf\xa1\xb7\xaf", MB_ICONEXCLAMATION);
			PostQuitMessage(0);
		}
		break;

	// The game server's socket.
	case WM_USER + 10: {
		if (!WNetworkSystem::Instance())
			return 0;

		WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_DEFAULT);
		if (!socket)
			return 0;

		int reason = 0;
		if (WSAGETSELECTERROR(lParam) == 0) {
			switch (WSAGETSELECTEVENT(lParam)) {
			case FD_READ:
				if (socket->TryRead() == 0) {
					socket->Close();
					reason = 1;
				}
				break;
			case FD_WRITE:
				socket->Flush();
				break;
			case FD_CLOSE:
				reason = 2;
				break;
			}
		} else {
			reason = 3;
		}

		if (reason == 0)
			break;

		if (g_iRecvDisconCode) {
			g_iRecvDisconCode = 0;
		} else if (AfxGetTask()->GetActor("GolfRule")) {
			ReportError(4, "%d_%d_%d_%d_%d_%d(%d_%d)", reason, WSAGETSELECTERROR(lParam), g_iSendFailType, WSAGetLastError(),
				g_iSendPacketType, g_iRecvPacketType, CSharedDoc::Instance()->m_roomInfo.m_roomNumber, CSharedDoc::Instance()->m_userInfo.guid);
		} else {
			ReportError(4, "%d_%d_%d_%d_%d_%d", reason, WSAGETSELECTERROR(lParam), g_iSendFailType, WSAGetLastError(),
				g_iSendPacketType, g_iRecvPacketType);
		}

		if (IsLocalContent((localContentType_t)0x5A)) {
			// Reconnect to the least crowded game server not tried yet.
			CSharedDoc* doc = CSharedDoc::Instance();
			sGameServerInfo best;
			memset(&best, 0, sizeof(best));
			bool found = false;
			int minUsers = 2000;
			// The connected server's record starts at m_serverName.
			sGameServerInfo& current = *(sGameServerInfo*)doc->m_serverName;
			for (std::list<sGameServerInfo>::iterator it = doc->m_gameServerList.begin(); it != doc->m_gameServerList.end(); ++it) {
				sGameServerInfo info = *it;
				if (current.id != info.id) {
					minUsers = info.m_curUsers;
					best = info;
					found = true;
				}
				if (found == true) {
					for (std::list<unsigned long>::iterator tried = doc->m_list4B64.begin(); tried != doc->m_list4B64.end(); ++tried) {
						if (*tried == info.id) {
							memset(&best, 0, sizeof(best));
							found = false;
							break;
						}
					}
				}
				if (found == true && info.m_curUsers < minUsers && current.id != info.id) {
					minUsers = info.m_curUsers;
					best = info;
				}
			}

			if (found == false) {
				socket->Close();
				CSharedDoc::Instance()->m_list4B64.clear();
				SetMoveLoginServer(CSharedDoc::Instance()->m_userInfo.id, CSharedDoc::Instance()->m_loginServerAddress.c_str());
				CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"LOGIN", 0, 0, 0);
				break;
			}

			doc->m_gameServerAddress = best.m_address;
			CSharedDoc::Instance()->m_gameServerPort = best.m_port;
			CSharedDoc::Instance()->m_value84 = best.id;
			if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
				WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_DEFAULT);
			WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_DEFAULT, NetworkUnit::EVENT_NEXT);
			*(sGameServerInfo*)CSharedDoc::Instance()->m_serverName = best;
			CSharedDoc::Instance()->SetDirectMoveRoomInfo(CSharedDoc::Instance()->m_currentChannel.id, 0xFFFF, 0xFFFFFFFF);
			CSharedDoc::Instance()->m_list4B64.push_back(best.id);
		} else {
			// "The connection to the game server was lost."
			OpenDisconDlg("\xb0\xd4\xc0\xd3 \xbc\xad\xb9\xf6\xbf\xcd \xbf\xac\xb0\xe1\xc0\xcc \xb2\xf7\xbe\xee\xc1\xb3\xbd\xc0\xb4\xcf\xb4\xd9.", true);
		}
		return 1;
	}

	// The messenger server's socket.
	case WM_USER + 12:
		if (WSAGETSELECTERROR(lParam)) {
			if (WSAGETSELECTEVENT(lParam) == FD_CONNECT) {
				if (WSAGETSELECTERROR(lParam) == WSAEWOULDBLOCK)
					return 0;
				WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_MSN, NetworkUnit::EVENT_RESET);
				return 0;
			}
			CMessengerInfo::Instance()->Close(true);
			WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
			return 0;
		}
		if (WSAGETSELECTEVENT(lParam) == FD_CONNECT) {
			WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_MSN)->m_bConnected = 1;
			WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_MSN, NetworkUnit::EVENT_NEXT);
		} else if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_MSN);
			if (socket && socket->TryRead() == 0) {
				CMessengerInfo::Instance()->Close(true);
				WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
			}
		} else if (WSAGETSELECTEVENT(lParam) == FD_WRITE) {
			WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_MSN);
			if (socket)
				socket->Flush();
		} else if (WSAGETSELECTEVENT(lParam) == FD_CLOSE) {
			CMessengerInfo::Instance()->Close(true);
			WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
		}
		return 0;

	// The login server's socket.
	case WM_USER + 13:
		if (WSAGETSELECTERROR(lParam)) {
			WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_LOGIN)->m_bConnected = 0;
			// "The connection to the login server was lost."
			OpenDisconDlg("\xb7\xce\xb1\xd7\xc0\xce \xbc\xad\xb9\xf6\xbf\xcd \xbf\xac\xb0\xe1\xc0\xcc \xb2\xf7\xbe\xee\xc1\xb3\xbd\xc0\xb4\xcf\xb4\xd9.", false);
			return 0;
		}
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_LOGIN);
			if (socket && socket->TryRead() == 0)
				OpenDisconDlg("\xb7\xce\xb1\xd7\xc0\xce \xbc\xad\xb9\xf6\xbf\xcd \xbf\xac\xb0\xe1\xc0\xcc \xb2\xf7\xbe\xee\xc1\xb3\xbd\xc0\xb4\xcf\xb4\xd9.", false);
		} else if (WSAGETSELECTEVENT(lParam) == FD_WRITE) {
			WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_LOGIN);
			if (socket)
				socket->Flush();
		} else if (WSAGETSELECTEVENT(lParam) == FD_CLOSE) {
			OpenDisconDlg("\xb7\xce\xb1\xd7\xc0\xce \xbc\xad\xb9\xf6\xbf\xcd \xbf\xac\xb0\xe1\xc0\xcc \xb2\xf7\xbe\xee\xc1\xb3\xbd\xc0\xb4\xcf\xb4\xd9.", false);
		}
		return 0;

	// The rank server's socket: on loss, back to the lobby's room list.
	case WM_USER + 14:
		if (WSAGETSELECTERROR(lParam)) {
			WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_RANK)->m_bConnected = 0;
			CMouseCursor::Instance()->SetActive(false);
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
			CTaskManager::Instance()->PostMsg(0, "Lobby", 0x1A, 0, 0, 0, 0);
			return 0;
		}
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WClientSocket* socket = WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_RANK);
			if (socket == NULL || socket->TryRead() != 0)
				return 0;
		} else if (WSAGETSELECTEVENT(lParam) != FD_CLOSE) {
			return 0;
		}
		CMouseCursor::Instance()->SetActive(false);
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
		CTaskManager::Instance()->PostMsg(0, "Lobby", 0x1A, 0, 0, 0, 0);
		return 0;

	case WM_SETFOCUS:
		if (g_hwnd == GetForegroundWindow())
			g_bFocus = true;
		break;

	case WM_KILLFOCUS:
		if (!CBrowser::Instance() || !CBrowser::Instance()->IsOpen())
			g_bFocus = false;
		break;

	case WM_CLOSE:
		Quit();
		return 0;

	case WM_NCHITTEST:
		if (((m_appFlags >> 2) & 1) == 0)
			return 1;
		break;

	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_SIZE:
		case SC_MOVE:
		case SC_MAXIMIZE:
		case SC_KEYMENU:
		case SC_MONITORPOWER:
			if (((m_appFlags >> 2) & 1) == 0)
				return 1;
			break;
		}
		break;

	case WM_ACTIVATEAPP:
		if (((m_appFlags >> 2) & 1) == 0) {
			SetWindowLong(g_hwnd, GWL_STYLE, GetWindowLong(g_hwnd, GWL_STYLE) & ~WS_SYSMENU);
			ShowWindow(g_hwnd, wParam ? SW_RESTORE : SW_MINIMIZE);
			return 1;
		}
		if (g_miles)
			g_miles->Activate(wParam != 0);
		break;

	case WM_PAINT:
		if (CBrowser::Instance() && CBrowser::Instance()->IsOpen())
			return DefWindowProc(g_hwnd, message, wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
		// Alt+F4 during a round only quits where the content allows it.
		if (wParam == VK_F4) {
			if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
				if (!IsLocalContent((localContentType_t)0x4F) || CSharedDoc::Instance()->m_playingMode == 1)
					return 1;
			}
		}
		break;
	}

	return m_procManager ? m_procManager->WinProc(g_hwnd, message, wParam, lParam) : 0;
}

// projectg.cpp:2859
bool CProjectG::Init(void) {
	m_deviceManager->SetHWND(g_hwnd);

	if (!SetVideoDevice()) return false;
	if (!SetInputDevice()) return false;
	SetAudioDevice();

	UpdateWindow(g_hwnd);
	ShowWindow(g_hwnd, SW_SHOW);
	InvalidateRect(g_hwnd, NULL, FALSE);
	UpdateWindow(g_hwnd);

	ResetCaptuerNum();

	return true;
}

// projectg.cpp:2877
bool CProjectG::SetVideoDevice(void) {
	char caption[64] = "";

	int tnlMode = COption::Instance()->vGetTnLMode();

	RECT rect = { 0, 0, 800, 600 };

	AdjustWindowRectEx(&rect, 0xC40000, FALSE, 0);
	int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
	OffsetRect(&rect, x, y);

	MoveWindow(g_hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, FALSE);

	SetWindowed(true);
	WVideoDev* video = m_deviceManager->CreateVideoDevice(0, "Window", tnlMode);

	if (!video) {
		MessageBox(g_hwnd,
			"\xB4\xD9\xC0\xBD\xB0\xFA \xB0\xB0\xC0\xBA \xB9\xAE\xC1\xA6\xB7\xCE \xBD\xC7\xC7\xE0\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.\n\n"
			"* DirectX 9.0c \xC0\xCC\xBB\xF3\xC0\xCC \xBC\xB3\xC4\xA1 \xB5\xC7\xC1\xF6 \xBE\xCA\xBE\xD2\xB0\xC5\xB3\xAA\n"
			"* \xB5\xF0\xBD\xBA\xC7\xC3\xB7\xB9\xC0\xCC \xBE\xEE\xB4\xF0\xC5\xCD(\xBA\xF1\xB5\xF0\xBF\xC0\xC4\xAB\xB5\xE5)\xC0\xC7 \xB5\xE5\xB6\xF3\xC0\xCC\xB9\xF6\xB0\xA1 \xC3\xD6\xBD\xC5 \xB9\xF6\xC0\xFC\xC0\xCC \xBE\xC6\xB4\xCF\xB0\xC5\xB3\xAA\n"
			"* \xB5\xF0\xBD\xBA\xC7\xC3\xB7\xB9\xC0\xCC \xBE\xEE\xB4\xF0\xC5\xCD(\xBA\xF1\xB5\xF0\xBF\xC0\xC4\xAB\xB5\xE5)\xC0\xC7 \xBC\xBA\xB4\xC9\xBF\xA1 \xB9\xAE\xC1\xA6\xB0\xA1 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.\n\n"
			"\xC6\xCE\xBE\xDF \xBD\xC7\xC7\xE0\xBD\xC3 \xB3\xAA\xBF\xC0\xB4\xC2 \xC3\xB9 \xC8\xAD\xB8\xE9\xBF\xA1\xBC\xAD [\xBD\xC7\xC7\xE0\xB9\xAE\xC1\xA6\xBA\xB8\xB0\xED]\xB8\xA6 \xB4\xAD\xB7\xAF \xC4\xC4\xC7\xBB\xC5\xCD \xC1\xA4\xBA\xB8\xB8\xA6 \xB5\xEE\xB7\xCF\xC7\xCF\xBD\xC5 \xC8\xC4,  \n"
			"\xC6\xCE\xBE\xDF \xC8\xA8\xC6\xE4\xC0\xCC\xC1\xF6\xC0\xC7 [\xB0\xED\xB0\xB4\xC1\xF6\xBF\xF8->FAQ]\xB8\xA6 \xC0\xD0\xBE\xEE\xBA\xB8\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.",
			caption, MB_ICONERROR);

		return false;
	}

	video->SetMainThreadId(m_mainThreadId);
	SetProc(video);
	g_resrcmng->SetVideoReference(video);
	m_monitorFPS = (float)floor(video->GetMonitorSupportFPS());
	SetFPS(m_monitorFPS);

	return true;
}

// projectg.cpp:2940
bool CProjectG::SetAudioDevice(void) {
	bool mssEnabled = COption::Instance()->aIsMssEnabled();
	int frequency = COption::Instance()->aGetMssFrequency();
	int bits = COption::Instance()->aGetMssBits();
	int channels = COption::Instance()->aGetMssChannels();
	bool hwSound = COption::Instance()->aIsMssHwSoundEnabled();
	float sfxVolume = COption::Instance()->aGetSfxVolume();
	float bgmVolume = COption::Instance()->aGetBgmVolume();

	g_miles = new WMilesSoundSystem(g_hwnd, "mss", mssEnabled, frequency, bits, channels);
	g_audio = new CSoundManager;

	if (g_miles && g_miles->Init(g_hwnd, frequency, bits)) {
		WMilesSoundSystem::w_speaker_type speaker = COption::Instance()->aGetMssSpeaker();
		int balance = COption::Instance()->aGetMssBalance();

		g_miles->SpeakerType(speaker);
		g_miles->SetBalance(balance);
		g_miles->SetDistance(128.0f, 960.0f);

		g_audio->SetMSS(g_miles);

		if (mssEnabled) {
			g_audio->SetVolume(Between(0.0f, sfxVolume, 1.0f));
			g_audio->SetBGMVolume(Between(0.0f, bgmVolume, 1.0f));
		} else {
			g_audio->SetVolume(0);
			g_audio->SetBGMVolume(0);
		}
	} else {
		g_audio->SetMSS(NULL);
		return false;
	}

	return true;
}

// projectg.cpp:2997
void CProjectG::UploadShaderSource(void) {
	WVideoDev* video = g_resrcmng->VideoReference();

	if (video) {
		cFile* file = g_resrcmng->GetCFile("pangya.fx", -1);

		if (file) {
			char* source = new char[file->Length() + 1];

			file->Read(source, file->Length());
			source[file->Length()] = 0;

			video->SetShaderSource(source);

			delete[] source;
			CloseCFile(file);
		} else {
			video->SetShaderSource(0);
		}
	}
}

// projectg.cpp:3033
bool CProjectG::SetInputDevice(void) {
	g_keyboard = m_deviceManager->CreateInputDevice("DirectInput", "keyboard");
	SetProc(g_keyboard);
	m_inputDevList += g_keyboard;

	g_mouse = m_deviceManager->CreateInputDevice("DirectInput", "mouse");
	SetProc(g_mouse);
	m_inputDevList += g_mouse;

	g_ime = m_deviceManager->CreateInputDevice("Ime", "");
	SetProc(g_ime);
	m_inputDevList += g_ime;

	return true;
}

// projectg.cpp:3050
void CProjectG::SetProc(WDevice* device) {
	WProc* proc = device->ExternProc();

	if (proc)
		proc->SetProc(m_procManager, g_hwnd);
}

// projectg.cpp:3058
void CProjectG::Update(int time) {
	for (WInputDev* dev = m_inputDevList.Start(); dev; dev = m_inputDevList.Next()) {
		dev->Update(g_CurrentTime);
	}
}

// projectg.cpp:3068
void CProjectG::SetWindowed(bool windowed) {
	if (windowed) {
		m_appFlags |= 4;
	} else {
		m_appFlags &= ~4;
	}
}

// projectg.cpp:3076
void CProjectG::OnMsnPacket(WReceivedPacket& packet) {
}

// projectg.cpp:3081
void CProjectG::ProcessRecvPacketQueue(void) {
	CTask *pTask = CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : 0;
	if (pTask == 0) {
		return;
	}
	if (((pTask->m_stateFlags >> 1) & 1) == 0 || CTaskManager::Instance()->m_bReservedTask) {
		return;
	}
	while (!m_recvPacketQueue.empty()) {
		WReceivedPacket *pPacket = m_recvPacketQueue.front();

		if (pTask->OnPacket(*pPacket) == 0) {
			pPacket->Reset();
			pPacket->Decode2();
		}

		delete pPacket;
		m_recvPacketQueue.pop_front();
	}
}

// projectg.cpp:3108
// A packet from a network unit: queued while the current task is not ready
// (or a task change is pending), else the queued packets then this one go to
// the current task, falling back to the preserved task when it refuses one.
void CProjectG::OnPacket(WReceivedPacket& packet) {
	if (CTaskManager::Instance() == 0) {
		packet.Decode2();
		return;
	}
	CTask *pPreserved = CTaskManager::Instance()->m_preservedTask;
	CTask *pTask = CTaskManager::Instance()->GetCurrentTask();

	if (pTask == 0) {
		packet.Decode2();
		return;
	}

	if (((pTask->m_stateFlags >> 1) & 1) == 0 || CTaskManager::Instance()->m_bReservedTask) {
		if (packet.Decode2() == 0x60) {
			packet.Reset();
			packet.Decode2();
			return;
		}
		WReceivedPacket *pCopy = new WReceivedPacket;
		packet.Copy(pCopy);
		m_recvPacketQueue.push_back(pCopy);
		return;
	}

	while (!m_recvPacketQueue.empty()) {
		WReceivedPacket *pQueued = m_recvPacketQueue.front();

		if (pTask->OnPacket(*pQueued) == 0) {
			pQueued->Reset();
			if (pPreserved == 0 || pPreserved->OnPacket(*pQueued) == 0) {
				if (pPreserved) pQueued->Reset();
				pQueued->Decode2();
			}
		}

		delete pQueued;
		m_recvPacketQueue.pop_front();
	}

	if (pTask->OnPacket(packet) == 0) {
		packet.Reset();
		if (pPreserved == 0 || pPreserved->OnPacket(packet) == 0) {
			if (pPreserved) packet.Reset();
			packet.Decode2();
		}
	}
}

// projectg.cpp:3186
void CProjectG::ProcessPacket(WReceivedPacket& packet) {
	packet.Decode2();
}

// projectg.cpp:3219
void CProjectG::OnUDPPacket(WReceivedPacket& packet) {
}

// wtemplate.h:409-414 (not yet in src/client/ProjectG/wtemplate.h, which this
// unit does not own): finds a game server by its id.
struct _FindByGuid {
	unsigned long m_guid;

	_FindByGuid(unsigned long guid) : m_guid(guid) {}
	bool operator()(const sGameServerInfo& item) const { return m_guid == item.id; }
};

// projectg.cpp:3224
bool CProjectG::OnGameServerResult(int result, FrForm* pForm) {
	if (CLoginInfo::Instance()->IsWebLogin()) {
		g_bQuit = true;
	} else if (result) {
		FromGolfToLobbySettings();
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"SERVERLIST", 0, 0, 0);
		WSingleton<CMouseCursor>::Instance()->SetActive(true);

		if (WSingleton<CSharedDoc>::Instance()) {
			// The connected server's id: CSharedDoc keeps a sGameServerInfo at
			// 0x4B05 (m_serverName is its name), not yet declared as such.
			unsigned long serverId = ((const sGameServerInfo*)WSingleton<CSharedDoc>::Instance()->m_serverName)->id;
			std::list<sGameServerInfo>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_gameServerList.begin(), WSingleton<CSharedDoc>::Instance()->m_gameServerList.end(), _FindByGuid(serverId));

			if (it != WSingleton<CSharedDoc>::Instance()->m_gameServerList.end())
				WSingleton<CSharedDoc>::Instance()->m_gameServerList.erase(it);

			WSingleton<CSharedDoc>::Instance()->m_channelList.clear();
		}
	}

	return true;
}

// projectg.cpp:3257
bool CProjectG::OnLoginServerResult(int result, FrForm* pForm) {
	g_bQuit = true;
	return true;
}

// projectg.cpp:3274
__int64 CProjectG::CalcHddFreeSpace(void) {
	__int64 freeSpace = 0;
	char* hdd = GetCurrentHdd();

	typedef BOOL (WINAPI* GetDiskFreeSpaceExProc)(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
	GetDiskFreeSpaceExProc pGetDiskFreeSpaceEx = (GetDiskFreeSpaceExProc)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetDiskFreeSpaceExA");

	if (pGetDiskFreeSpaceEx) {
		ULARGE_INTEGER freeBytesAvailable;
		ULARGE_INTEGER totalBytes;
		ULARGE_INTEGER totalFreeBytes;
		BOOL result = pGetDiskFreeSpaceEx(hdd, &freeBytesAvailable, &totalBytes, &totalFreeBytes);

		if (result) {
			freeSpace = totalFreeBytes.QuadPart;
		} else {
			return 0;
		}
	}

	return freeSpace;
}
