// Original: d:\build\custom\temp\code\client\projectg\taskmain.cpp
#define TASKMAIN_SHAREDDOC_INLINE
#define TASKMAIN_CARDSYSTEM_INLINE
#define TASKMAIN_TOOLTIP_INLINE
#define TASKMAIN_TASKMANAGER_INLINE
#define TASKMAIN_USER_INFO_INLINE
#include "NetworkSystem.h"
#include "TaskMain.h"
#include "actor.h"
#include "frwnd.h"
#include "ItemStorage.h"
#include "TaskManager.h"
#include "ClientSetting.h"
#include "logininfo.h"
#include "messengerdlg.h"
#include "onelineboard.h"
#include "cardsystem.h"
#include "itembuffactor.h"
#include "playerdlg.h"
#include "user_info.h"
#include "noticedlg.h"
#include "buddymanager.h"
#include "ToolTip.h"
#include "Treasure.h"
#include "avatar_avatar.h"
#include "birthdayeventdlg.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "scenemanager.h"
#include "RTTI.h"
#include "frcmdtarget.h"
#include "GiftDlg.h"
#include "FriendDlg.h"
#include "CardDlg.h"
#include "OnelineReqDlg.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Fresh/include/frbutton.h"
#include "../client/Fresh/include/frform.h"
#include "ObjectFactory.h"
#include "SoundManager.h"
#include "OfflineTradeDlg.h"
#include "Packet.h"
#include "fresh.h"
#include "Localize.h"
#include "InputManager.h"
#include "GiftAlarmDlg.h"
#include "s5/Utilities.h"
#include "LobbyTask.h"
#include "avatar_task.h"
#include "../client/Fresh/include/frarea.h"
#include "../client/Fresh/include/frgroupbox.h"
#include "s5/SkinnedTheme.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/Fresh/include/frstatic.h"
#include "OptionDlg.h"
#include "../client/Fresh/include/fredit.h"
#include "s5/classicserver/ClassicServer.h"
#include "../client/Fresh/include/frviewer.h"
#include "../client/Fresh/include/frgraphicinterface.h"
#include "mousecursor.h"
#include "binstr.h"
#include "PangGiftDlg.h"
#include "ProjectG.h"
#include "LoginRsDlg.h"
#include "Browser.h"
#include "RecycleDlg.h"
#include "ScratchDlg.h"

// The project's SDK has no <wininet.h>; the function is declared locally.
extern "C" __declspec(dllimport) BOOL __stdcall InternetSetCookieA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData);
#include "../client/Fresh/include/frtabbutton.h"
#include "KbdManualDlg.h"
#include "GolfTask.h"
#include "GhostTask.h"
#include "ShopTask.h"
#include "RealMyRoomTask.h"
#include "LogoutDlg.h"
#include "QuitWindow.h"
#include "TikiMagicBoxDlg.h"
#include "ServerDlg.h"
#include "CaddieInfoDlg.h"
#include "TutorialTask.h"
#include "BongdariShopDlg.h"
#include "LevelUpItemDlg.h"
#include "RealMyRoom_AuthorityDlg.h"
#include "InitDelegate.h"
#include "UccManager.h"
#include "../client/Fresh/include/frdesktop.h"

// The kind of BGM playing: 0 none, 1 random (SetRandomBGM), 3 constant
// (SetConstantBGM). File-local, no symbol.
static int data_af1a08;

// ShowOverAndUnderBar: whether the bars are shown, and each bar frame's
// visibility saved while they are hidden. File-local, no symbol.
static bool data_a7cea0 = true;
static bool data_af1a90[3];

// EnableOverAndUnderBar: whether the bars are enabled, and each control's
// enabled state saved while they are disabled. File-local, no symbol.
static bool data_a7cea1 = true;
static bool data_af1a94[35];

// wtypes.h:174 (inline in the original; this object kept the out-of-line copy)
bool WRect::IsInRect(const WPoint& pt) {
	if (pt.x >= x && pt.x <= x + w && pt.y >= y && pt.y <= y + h)
		return true;
	return false;
}

// clientsetting.h:406
int COption::gGetUnderExtBtn(void) {
	return m_option.m_underExtBtn;
}

// clientsetting.h:407
void COption::gSetUnderExtBtn(int extBtn) {
	m_option.m_underExtBtn = extBtn;
}

// avatar_avatar.h:280
void ntAvatarManager::SetActive(bool active) {
	m_active = active;
}

// birthdayeventdlg.h:147
void FrBirthdayEventMissionDlg::SetFadeOutFlag(bool fadeOut) {
	m_fadeOut = fadeOut;
}

// user_info.h:386
bool FrUserInfoForm::IsOpenAddFriendDlg(void) const {
	return m_addFriendDlg ? true : false;
}

// user_info.h:601: CUserInfo::GetDlg, inline in user_info.h (TASKMAIN_USER_INFO_INLINE).

// onelineboard.h:14
void COneLineBoard::SetRect(const WRect &rect) {
	m_rect = rect;
}

// onelineboard.h:15
void COneLineBoard::SetMessage(const char *message) {
	m_message = message;
}

// onelineboard.h:16
std::string &COneLineBoard::GetMessageA(void) {
	return m_message;
}

// onelineboard.h:17
void COneLineBoard::SetWaitMsgNum(int num) {
	m_waitMsgNum = num;
}

// onelineboard.h:18
void COneLineBoard::SetWaitMsgTime(int time) {
	m_waitMsgTime = time;
}

// noticedlg.h:29
unsigned long FrNoticeDlg::GetTypeId(void) const {
	return m_typeId;
}

// buddymanager.h:130
BuddyManager *Buddy(void) {
	return BuddyMGR;
}

// treasure.h:103
bool CTHunter::IsUsedTikiReport(void) {
	return m_usedTikiReport;
}

// treasure.h:104
void CTHunter::SetUsedTikiReport(bool used) {
	m_usedTikiReport = used;
}

// playerdlg.h:120
IFF_ITEM_COMMON *FrSubEquipBar::GetResult(void) {
	return m_result;
}

// playerdlg.h:121
unsigned long FrSubEquipBar::GetLastSelectedGuid(void) {
	return m_lastSelectedGuid;
}

// playerdlg.h:122
FrSubEquipBar::eCategory FrSubEquipBar::GetCategory(void) {
	return m_category;
}

// logininfo.h:121
CLoginInfo *CLoginInfo::Instance(void) {
	if (m_pInst == NULL)
		m_pInst = new CLoginInfo;
	return m_pInst;
}

// logininfo.h:475
CLoginInfo *LOGININFO(void) {
	return CLoginInfo::Instance();
}

// logininfo.h:480
const char *LOGINPW(void) {
	return LOGININFO()->Passwd();
}

// messengerdlg.h:435
CMessengerInfo *MESSENGER(void) {
	return CMessengerInfo::Instance();
}

// user_info.h:621: USERINFODLG, inline in user_info.h (TASKMAIN_USER_INFO_INLINE).

// taskmanager.h:1193
int AfxPostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4) {
	return CTaskManager::Instance()->PostMsg(pSender, pTarget, msg, param1, param2, param3, param4);
}

// taskmanager.h:1210 AfxGetPreservedTask: see TaskManager.h (TASKMAIN_TASKMANAGER_INLINE)

// tooltip.h:89 (see ToolTip.h)
CIconToolTip *CIconToolTip::GetInstance(void) {
	static CIconToolTip _Instance;

	return &_Instance;
}

// shareddoc.h:616
void CSharedDoc::SetRmrMode(unsigned char mode) {
	m_rmrMode = mode;
}

// shareddoc.h:617
unsigned char CSharedDoc::GetRmrMode(void) {
	return m_rmrMode;
}

// shareddoc.h:620
unsigned long CSharedDoc::GetFlagBlock(void) {
	return m_userInfo.flagBlock;
}

// shareddoc.h:1011
bool CSharedDoc::IsHaveHalloweenItem(void) {
	return m_haveHalloweenItem;
}

// shareddoc.h:1024
eReplayModeType CSharedDoc::GetPlayingMode(void) {
	return m_playingMode;
}

// shareddoc.h:1025
void CSharedDoc::SetPlayingMode(eReplayModeType mode) {
	m_playingMode = mode;
}

// shareddoc.h:1137
int CSharedDoc::GetNowGoWithMode(void) {
	return m_nowGoWithMode;
}

// shareddoc.h:1138
int CSharedDoc::GetGoWithFriendUID(void) {
	return m_goWithFriendUID;
}

// MyGuid(bool) is defined inline in shareddoc.h; do not emit a second body here.

// shareddoc.inl:466
int IsMassGame(unsigned char gameType) {
	switch (gameType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
	case 14:
		return 1;
	}

	return 0;
}

// shareddoc.inl:518
__int64 MyOutsideCash(void) {
	return WSingleton<CSharedDoc>::Instance()->m_outsideCash;
}

// shareddoc.inl:527: GalleryMode, inline in SharedDoc.h (TASKMAIN_SHAREDDOC_INLINE).

// onelinereqdlg.h:13
const char *FrOnelineReqDlg::GetInputString(void) {
	return m_pChatInput->GetLine(1, false);
}

// giftdlg.h:33
void FrGiftDlg::UpdateFriendList(void) {
	if (m_pFriendListDlg)
		m_pFriendListDlg->UpdateFriendList();
}

// carddlg.h:363
const WPoint &FrSPCardBuffDlg::GetPosition(void) {
	return m_Pos;
}

// frwnd.h:238
void FrWnd::OnResize(void) {
}

// taskmain.h:37
const WRTTI *CTaskMain::GetRTTI(void) const {
	return &m_RTTI;
}

// taskmain.cpp:91
const WRTTI CTaskMain::m_RTTI("CTaskMain", &IActor::m_RTTI);

IObject *CTaskMainMakeInstance(void) {
	return new CTaskMain;
}

struct __impCTaskMain {
	__impCTaskMain(void) {
		ObjectFactory().AddObjectFunctor(CTaskMainMakeInstance, "CTaskMain");
		ActorFactory().AddActorClass("CTaskMain");
	}
};
static __impCTaskMain s_impCTaskMain;

// taskmain.cpp:93
const sFRESH_MSGMAP *CTaskMain::GetMessageMap(void) const {
	return &CTaskMain::_MsgMap;
}

sFRESH_MSGMAP CTaskMain::_MsgMap = { &FrCmdTarget::_MsgMap, &CTaskMain::_MsgEntries[0] };

sFRESH_ENTRY CTaskMain::_MsgEntries[] = {
	// taskmain.cpp:96
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// taskmain.cpp:99
bool CTaskMain::IsActiveRecycleDlg(void) {
	return m_recycleDlg ? true : false;
}

// taskmain.cpp:100
bool CTaskMain::IsActiveScratchDlg(void) {
	return m_scratchDlg ? true : false;
}

// taskmain.cpp:105
void CTaskMain::OnPreLoadInit(void) {
	for (int i = 0; i < 0x15; i++)
		m_barButton[i] = NULL;

	InitUnderBar();
	InitOverBar();
	InitBongdariShop();
	InitMessenger();
	InitControls();
}

// taskmain.cpp:120
void CTaskMain::OnInit(void) {
	for (int i = 0; i < 0x15; i++)
		m_barButton[i] = NULL;

	InitUnderBar();
	InitOverBar();
	InitBongdariShop();
	InitMessenger();
	InitControls();

	m_rmrSettingDlg = NULL;

	for (int j = 0; j < 5; j++)
		m_completedMissionDlgs[j] = NULL;
}

// The title font's texture (one page; file-local in the original, no symbol).
extern "C" char *data_a7cea4[] = {
	"[font_wind.jpg"
};

// taskmain.cpp:142
void CTaskMain::OnLoad(void) {
	m_uiOffsetX = 0;
	m_uiOffsetY = 0;

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	m_screenWidth = 800.0f;
	m_screenHeight = 600.0f;
	info.filename = data_a7cea4;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,";

	m_titleFont = g_resrcmng->GetTitleFont();
	m_titleFont->Create(&info);
}

// taskmain.cpp:161
void CTaskMain::OnProcess(float dt) {
	ProcessOverBar();

	if (g_input->GetDown("FUNC_12", true)) {
		if (m_kbdManualDlg)
			m_kbdManualDlg->Close(true);
		else {
			m_kbdManualDlg = CreateForm<FrKbdManualDlg>(g_pFresh->GetManager(), this, "kbd_manual", 0);
			if (m_kbdManualDlg) {
				m_kbdManualDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnKbdManualDlgResult, 1);
				m_kbdManualDlg->EnableDrag(false);
			}
		}
	}

	if (g_input->GetDown("ESCAPE", true) && !g_pFresh->GetManager()->HasEscKeyWindow()
		&& !((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGolfTask::m_RTTI) : false)
		&& !((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		OnUnderBar_BackUp();

	if (m_waitDlgTime > -0.1f) {
		m_waitDlgTime += dt;

		if (m_waitDlgTime > (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI) : false) ? 1.0f : 0.5f)) {
			if (m_serverDlg == NULL) {
				if (m_waitDlg == NULL) {
					m_waitDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "wait", 0);
					m_waitDlg->SetMessage("\xbc\xad\xb9\xf6\xc0\xc7 \xc0\xc0\xb4\xe4\xc0\xbb \xb1\xe2\xb4\xd9\xb8\xae\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", false);
					m_waitDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnWaitDlgResult, 0);

					FrButton *cancel = DYNAMIC_CAST(FrButton, m_waitDlg->FindChildByName("cancel"));
					if (cancel)
						cancel->Enable(false);
					return;
				}
				if (!(m_waitDlgTime > 15.0f))
					return;

				FrButton *cancel = DYNAMIC_CAST(FrButton, m_waitDlg->FindChildByName("cancel"));
				if (cancel)
					cancel->Enable(true);
			}

			m_waitDlgTime = -10.0f;
		}
	}
}

// taskmain.cpp:227
void CTaskMain::OnDestroy(void) {
	if (g_resrcmng && m_titleFont) { g_resrcmng->Release(m_titleFont); m_titleFont = 0; }
}

// taskmain.cpp:232
void CTaskMain::OnPreserveBack(const char *) {
	if (g_resrcmng && m_titleFont) { g_resrcmng->Release(m_titleFont); m_titleFont = 0; }
}

// taskmain.cpp:237
void CTaskMain::OnRestorePreserved(void) {
	CTaskMain::OnLoad();
	CTaskMain::OnInit();
}

// taskmain.cpp:243
void CTaskMain::CalcUIOffset(bool fixed) {
	if (fixed) {
		m_screenWidth = 800.0f;
		m_screenHeight = 600.0f;
	} else {
		m_screenWidth = g_view->GetWidth();
		m_screenHeight = g_view->GetHeight();
	}

	m_uiOffsetX = (m_screenWidth - 800.0f) * 0.5f;
	m_uiOffsetY = m_screenHeight - 600.0f;
}

// taskmain.cpp:257
void CTaskMain::SetCenterOffset(FrWnd *wnd, float x, float y) {
	WRect rect = wnd->GetRect();

	rect.x += x;
	rect.y += y;

	wnd->SetRect(rect);
}

// taskmain.cpp:267
void CTaskMain::SetLeftOffset(FrWnd *wnd, float x) {
	WRect rect = wnd->GetRect();

	float right = 800.0f - rect.x;
	rect.x = x - right;

	wnd->SetRect(rect);
}

// taskmain.cpp:277
void CTaskMain::InitMessenger(void) {
	m_messengerBubbleEdit = NULL;
	memset(m_bubbleBitmap, 0, sizeof(m_bubbleBitmap));
	m_messengerAlarmBtn = NULL;
	m_messengerGiftAlarmBtn = NULL;

	m_treasureAlarm = NULL;

	m_messengerWnd = NULL;
	m_messengerBubble = NULL;
}

// taskmain.cpp:290
void CTaskMain::InitBongdariShop(void) {
	m_bongdariShop[0] = false;
	m_bongdariShop[1] = false;
	m_bongdariShop[2] = false;
}

// taskmain.cpp:297
void CTaskMain::InitControls(void) {
	memset(m_barFrame, 0, sizeof(m_barFrame));
	m_giftDlg = NULL;
	m_optionDlg = NULL;
	m_kbdManualDlg = NULL;
	m_serverDlg = NULL;
	m_loginRsDlg = NULL;
	m_recycleDlg = NULL;
	m_tikiMagicBoxDlg = NULL;
	m_scratchDlg = NULL;
	m_onelineReqDlg = NULL;
	m_waitDlg = NULL;
	m_noticeDlg = NULL;
	m_popupDlg = NULL;
	m_quitDlg = NULL;
	m_specialCardForm = NULL;
	m_subEquipBar = NULL;

	m_maketListDlg = NULL;

	m_waitDlgTime = -10.0f;
}

// taskmain.cpp:327
void CTaskMain::InitUnderBar(void) {
	m_underBar = NULL;
	m_mascotViewer = NULL;
	m_underTab = NULL;
	m_underBarEnabled = false;

	m_maketListDlg = NULL;
}

// taskmain.cpp:337
void CTaskMain::OnUnderBarInit(intptr_t param) {
	m_barFrame[2] = DYNAMIC_CAST(FrGroupBox, param);
	if (m_barFrame[2]) {
		SetCenterOffset(m_barFrame[2], m_uiOffsetX, m_uiOffsetY);
	}
}

// taskmain.cpp:346
void CTaskMain::OnUnderTab_Init(intptr_t param) {
	m_barFrame[1] = DYNAMIC_CAST(FrGroupBox, param);
	if (m_barFrame[1]) {
		m_barFrame[1]->SetVisible(false);

		SetCenterOffset(m_barFrame[1], m_uiOffsetX, m_uiOffsetY);
	}
}

// taskmain.cpp:360
void CTaskMain::OnOverBar_Init(intptr_t param) {
	FrGroupBox *groupBox = DYNAMIC_CAST(FrGroupBox, param);
	m_barFrame[0] = groupBox;
	if (groupBox) {
		S5::THEME::ChangeOverbarTheme(S5::THEME::GetCurrentSkinnedTheme(), groupBox);
	}
}

// taskmain.cpp:374
void CTaskMain::OnUnderBar_BackInit(intptr_t param) {
	m_barButton[BAR_BUTTON_NAVI_BACKWARD] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[BAR_BUTTON_NAVI_BACKWARD]) {
		m_barButton[BAR_BUTTON_NAVI_BACKWARD]->SetPushSound("ui_desktop_icon_click");
		m_barButton[BAR_BUTTON_NAVI_BACKWARD]->EnableHover(true, 0.0f);
	}
}

// The task preserved under the current one (the one the back button returns
// to) is exactly of class T.
#define PRESERVED_TASK_IS(T) ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&T::m_RTTI) : false)

// taskmain.cpp:384
void CTaskMain::OnUnderBar_BackUp(void) {
	BOOL sendBackPacket = FALSE;

	WSingleton<CSharedDoc>::Instance()->SetRmrMode(2);

	if (g_pFresh->IsCurrentLayout("FAMILY")) {
		WSingleton<CSharedDoc>::Instance()->m_gameMode = 0;
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
	} else if (g_pFresh->IsCurrentLayout("TOPPAGE") || g_pFresh->IsCurrentLayout("SERVERLIST")) {
		m_quitDlg = CreateForm<FrLogoutDlg>(g_pFresh->GetManager(), this, "quit", 0);
		m_quitDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnQuitDlgResult, 1);
	} else if (g_pFresh->IsCurrentLayout("ROOMLIST")) {
		SendRoomListExitPacket();
		m_waitedLayout = "TOPPAGE";
	} else if (g_pFresh->IsCurrentLayout("GAMEROOM") || g_pFresh->IsCurrentLayout("GAMEROOM_EXT")) {
		SendRoomExitPacket();
		m_waitedLayout = "ROOMLIST";
	} else if (g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES")) {
		SendMsg(NULL, "Lobby", 0x1F7, 0, 0, 0, 0);
	} else if (g_pFresh->IsCurrentLayout("SHOPMAIN")) {
		if (PRESERVED_TASK_IS(ntAvatarChatTask)) {
			CTaskManager::Instance()->ChangeTask("ntAvatarChatTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(NULL, "AvatarChat", 0, (intptr_t)"AVATARCHAT_MAIN", 0, 0, 0);
		} else if (PRESERVED_TASK_IS(CGhostTask)) {
			CTaskManager::Instance()->ChangeTask("CGhostTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
			CTaskManager::Instance()->PostMsg(NULL, "Ghost", 0, (intptr_t)"GHOST", 0, 0, 0);
			sendBackPacket = TRUE;
		} else if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2) {
			WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = false;
			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
		} else {
			if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
				sendBackPacket = TRUE;
			} else {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			}

			TurnOn3DBackground();
		}
	} else if (g_pFresh->IsCurrentLayout("RealMyRoom")) {
		if (PRESERVED_TASK_IS(ntAvatarChatTask)) {
			CTaskManager::Instance()->ChangeTask("ntAvatarChatTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(NULL, "AvatarChat", 0, (intptr_t)"AVATARCHAT_MAIN", 0, 0, 0);
		} else if (PRESERVED_TASK_IS(CGhostTask)) {
			CTaskManager::Instance()->ChangeTask("CGhostTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
			CTaskManager::Instance()->PostMsg(NULL, "Ghost", 0, (intptr_t)"GHOST", 0, 0, 0);
			sendBackPacket = TRUE;
		} else if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2) {
			WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = false;
			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
		} else {
			if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
				sendBackPacket = TRUE;
			} else if (IsLocalContent((localContentType_t)0x71) == true) {
				CItemStorage *itemStorage = WSingleton<CItemStorage>::Instance();
				if (itemStorage != NULL && !itemStorage->IsVisible()) {
					CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
					CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
				}
			} else {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			}

			TurnOn3DBackground();
		}
	} else if (g_pFresh->IsCurrentLayout("AVATARCHAT_MAIN")) {
		if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
			SendRoomExitPacket();
			m_waitedLayout = "ROOMLIST";
			sendBackPacket = TRUE;
		} else {
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
		}
	} else if (g_pFresh->IsCurrentLayout("TUTORIAL_MAIN")) {
		WSingleton<CSharedDoc>::Instance()->m_gameMode = 0;
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
	} else if (g_pFresh->IsCurrentLayout("UCCROOM")) {
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
	} else if (g_pFresh->IsCurrentLayout("REALMYROOM")) {
		if (PRESERVED_TASK_IS(ntAvatarChatTask)) {
			CTaskManager::Instance()->ChangeTask("ntAvatarChatTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(NULL, "AvatarChat", 0, (intptr_t)"AVATARCHAT_MAIN", 0, 0, 0);
		} else if (PRESERVED_TASK_IS(CGhostTask)) {
			CTaskManager::Instance()->ChangeTask("CGhostTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
			CTaskManager::Instance()->PostMsg(NULL, "Ghost", 0, (intptr_t)"GHOST", 0, 0, 0);
			sendBackPacket = TRUE;
		} else if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2) {
			WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = false;
			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
		} else {
			if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
				sendBackPacket = TRUE;
			} else if (IsLocalContent((localContentType_t)0x71) == true) {
				CItemStorage *itemStorage = WSingleton<CItemStorage>::Instance();
				if (itemStorage != NULL && !itemStorage->IsVisible()) {
					CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
					CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
				}
			} else {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			}

			TurnOn3DBackground();
		}
	}

	if (g_pFresh->IsCurrentLayout("REALMYROOMVISITOR")) {
		if (PRESERVED_TASK_IS(ntAvatarChatTask)) {
			CTaskManager::Instance()->ChangeTask("ntAvatarChatTask", "CGolfDoc", false);
			CTaskManager::Instance()->PostMsg(NULL, "AvatarChat", 0, (intptr_t)"AVATARCHAT_MAIN", 0, 0, 0);
		} else if (PRESERVED_TASK_IS(CGhostTask)) {
			CTaskManager::Instance()->ChangeTask("CGhostTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
			CTaskManager::Instance()->PostMsg(NULL, "Ghost", 0, (intptr_t)"GHOST", 0, 0, 0);
			sendBackPacket = TRUE;
		} else if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2) {
			WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = false;
			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
		} else {
			if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
				sendBackPacket = TRUE;
			} else {
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			}

			TurnOn3DBackground();
		}
	}

	if (IsLocalContent((localContentType_t)0x59)) {
		if (sendBackPacket) {
			WSendPacket packet((enumClientPacket)0xB9);
			packet.Encode1(1);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	OnBarHoverOff(0);
}

// taskmain.cpp:699
void CTaskMain::OnUnderBar_FrontInit(intptr_t param) {
	m_barButton[BAR_BUTTON_NAVI_FORWARD] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[BAR_BUTTON_NAVI_FORWARD]) {
		m_barButton[BAR_BUTTON_NAVI_FORWARD]->EnableHover(true, 0.0f);
		m_barButton[BAR_BUTTON_NAVI_FORWARD]->Enable(false);
	}
}

// taskmain.cpp:709
void CTaskMain::OnUnderBar_FrontUp(void) {
	OnBarHoverOff(0);
}

// taskmain.cpp:714
void CTaskMain::OnUnderBar_ShopInit(intptr_t param) {
	m_barButton[2] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[2]) {
		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CShopTask::m_RTTI) : false)
			m_barButton[2]->Enable(false);

		m_barButton[2]->SetPushSound("ui_desktop_icon_click");
		m_barButton[2]->EnableHover(true, 0.0f);

		if (IsLocalContent((localContentType_t)0x56) && g_pFresh->IsCurrentLayout("REALMYROOMVISITOR"))
			m_barButton[2]->Enable(false);
	}
}


// taskmain.cpp:733
void CTaskMain::OnUnderBar_ShopUp(void) {
	int slot;
	int param;

	if (!((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CShopTask::m_RTTI) : false)) {
		if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
			WSingleton<CProjectG>::Instance()->FromGolfToLobbySettings();
			CTaskManager::Instance()->ChangeTask("CShopTask", "", true);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);

			WSingleton<ntAvatarManager>::Instance()->SetActive(false);
		} else if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false)) {
			CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		} else if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CTutorialTask::m_RTTI) : false)) {
			WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = true;
			CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		} else if (g_pFresh->IsCurrentLayout("GHOST")) {
			CTaskManager::Instance()->ChangeTask("CShopTask", "", true);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		} else if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false) {
			WSingleton<CProjectG>::Instance()->FromGolfToLobbySettings();

			SendMsg(NULL, "RealMyRoom", 0xC3, (intptr_t)&slot, (intptr_t)&param, 0, 0);
			if (slot >= 8)
				slot = 0;
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0xC4, slot, param, 0, 0);

			CTaskManager::Instance()->ChangeTask("CShopTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		}
	}

	__rtti_obj = 0;
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:798
void CTaskMain::OnUnderBar_MyRoomInit(intptr_t param) {
	m_barButton[3] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[3]) {
		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false)
			m_barButton[3]->Enable(false);

		m_barButton[3]->SetPushSound("ui_desktop_icon_click");
		m_barButton[3]->EnableHover(true, 0.0f);

		if (IsLocalContent((localContentType_t)0x56) && g_pFresh->IsCurrentLayout("REALMYROOMVISITOR"))
			m_barButton[3]->Enable(false);
	}
}

// taskmain.cpp:816
void CTaskMain::OnUnderBar_MyRoomUp(void) {
	if (IsLocalContent((localContentType_t)0x56) && IsLocalContent((localContentType_t)0x61)) {
		WSingleton<CSharedDoc>::Instance()->SetRmrMode(1);
		WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID = WSingleton<CSharedDoc>::Instance()->m_myUID;
		MakeRealMyRoom();

		__rtti_obj = 0;
		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
		return;
	}

	if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false)) {
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0xC6, 1, 0, 0, 0);
	} else if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
		WSingleton<CProjectG>::Instance()->FromGolfToLobbySettings();
		CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", true);
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);

		WSingleton<ntAvatarManager>::Instance()->SetActive(false);
	} else if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false)) {
		CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", false);
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
	} else if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CTutorialTask::m_RTTI) : false)) {
		WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = true;
		CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", false);
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
	} else if (g_pFresh->IsCurrentLayout("GHOST")) {
		CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", true);
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
	} else {
		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CShopTask::m_RTTI) : false) {
			// carry the shop's current slot over to the room
			int slot;
			int param;
			SendMsg(NULL, "Shop", 0xC3, (intptr_t)&slot, (intptr_t)&param, 0, 0);
			CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0xC4, slot, param, 0, 0);
		}

		CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", CTaskManager::Instance()->GetPreservedTask() != 0);
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
	}

	__rtti_obj = 0;
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();

	if (IsLocalContent((localContentType_t)0x59)) {
		WSendPacket packet((enumClientPacket)0xB9);
		packet.Encode1(2);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// taskmain.cpp:873
void CTaskMain::OnUnderBar_ItemStorageInit(intptr_t param) {
	m_barButton[4] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[4]) {
		m_barButton[4]->SetPushSound("ui_desktop_icon_click");
		m_barButton[4]->EnableHover(true, 0.0f);

		m_itemStorageOpen = false;
		if (IsLocalContent((localContentType_t)0x56) && g_pFresh->IsCurrentLayout("REALMYROOMVISITOR"))
			m_barButton[4]->Enable(false);
	}
}

// taskmain.cpp:890
void CTaskMain::OnUnderBar_ItemStorageUp(void) {
	if (IsLocalContent((localContentType_t)0x71)) {
		WSingleton<CSharedDoc>::Instance()->SetRmrMode(4);
		WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID = MyUID();

		MakeRealMyRoom();
		OnBarHoverOff(0);
	}
}

// taskmain.cpp:902
void CTaskMain::OnUnderBar_GiftInit(intptr_t param) {
	m_barButton[5] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[5]) {
		m_barButton[5]->SetPushSound("ui_desktop_icon_click");
		m_barButton[5]->EnableHover(true, 0.0f);

		m_itemStorageOpen = false;
		if (IsLocalContent((localContentType_t)0x56) && g_pFresh->IsCurrentLayout("REALMYROOMVISITOR"))
			m_barButton[5]->Enable(false);
	}
}

// taskmain.cpp:919
void CTaskMain::OnUnderBar_GiftUp(void) {
	if (IsLocalContent((localContentType_t)0x56) && IsLocalContent((localContentType_t)0x61)) {
		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false) {
			m_itemStorageOpen = true;
			return;
		}

		WSingleton<CSharedDoc>::Instance()->SetRmrMode(3);
		WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID = WSingleton<CSharedDoc>::Instance()->m_myUID;

		MakeRealMyRoom();

		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
	} else if (IsLocalContent((localContentType_t)0x5B)) {
		WSendPacket packet((enumClientPacket)0x92);
		packet.Encode4(0);
		packet.Send(SEND_TO_DEFAULT);
	} else {
		if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false)) {
			if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) {
				CProjectG::Instance()->FromGolfToLobbySettings();
				CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", true);
				CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
				CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0xC6, 0, 0, 0, 0);

				WSingleton<ntAvatarManager>::Instance()->SetActive(false);
				goto done;
			}

			if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CTutorialTask::m_RTTI) : false) {
				WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag2 = true;
				CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", false);
			} else {
				CTaskManager::Instance()->ChangeTask("CRealMyRoomTask", "", AfxGetPreservedTask() != NULL);
			}
			CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0, (intptr_t)"RealMyRoom", 0, 0, 0);
		}
		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0xC6, 0, 0, 0, 0);
	}

done:
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:971
void CTaskMain::OnUnderBar_CookieInit(intptr_t param) {
	m_barButton[6] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[6]) {
		m_barButton[6]->SetPushSound("ui_desktop_icon_click");
		m_barButton[6]->EnableHover(true, 0.0f);
	}
}

// taskmain.cpp:981
void CTaskMain::OnUnderBar_CookieUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_value94) {
		this << MsgObject(0, 0x23, (intptr_t)"14\xBC\xBC \xB9\xCC\xB8\xB8\xC0\xCE \xBA\xD0\xC0\xBA\x0A\xC8\xB8\xBF\xF8\xB0\xA1\xC0\xD4(1) \xB9\xD7 \xB0\xE1\xC1\xA6(2)\xBF\xA1 \xB4\xEB\xC7\xD1\x0A\xBA\xCE\xB8\xF0\xB4\xD4\xC0\xC7 \xBD\xC2\xC0\xCE\xC0\xCC \xC0\xD6\xBE\xEE\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
	} else {
		WSingleton<COption>::Instance()->vApplyLobbyScreenSize();

		WSingleton<CBrowser>::Instance()->InitializePostArgument();

		switch (WSingleton<CSharedDoc>::Instance()->m_billingType) {
		case 2:
			WSingleton<CBrowser>::Instance()->AddPostArgument("sn", "9");
			WSingleton<CBrowser>::Instance()->AddPostArgument("mn", MakeStr("%d", CLoginInfo::Instance()->AuthUid()));
			WSingleton<CBrowser>::Instance()->OpenPost("https://qa.www.gametree.co.kr/InGame/Gateway.aspx", ((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false), true);
			break;

		case 4:
			if (!InternetSetCookieA("https://alpha-bill.hangame.com/", NULL, ((NhnLogin*)CLoginInfo::Instance()->QueryInterface())->GetSiteCookie())) {
				GetLastError();
				MessageBoxA(NULL, "Cookie Setting Fail!", "Error", MB_OK);
				return;
			}

			WSingleton<CBrowser>::Instance()->AddPostArgument("CHNL", "HG");
			WSingleton<CBrowser>::Instance()->OpenPost("https://alpha-bill.hangame.com/pay/charge.nhn", ((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false), true);
			break;
		}
	}

	if (IsLocalContent((localContentType_t)0x59)) {
		WSendPacket packet((enumClientPacket)0xB9);
		packet.Encode1(2);
		packet.Send(SEND_TO_DEFAULT);
	}

	__rtti_obj = 0;
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1094
void CTaskMain::OnUnderBar_GameInit(intptr_t param) {
	m_barButton[7] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[7]) {
		m_barButton[7]->SetPushSound("ui_desktop_icon_click");
		m_barButton[7]->EnableHover(true, 0.0f);

		m_barButton[7]->Enable(true);

		// No game list from the avatar chat or the ghost (replay) task.
		if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)
			|| ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)
			|| ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGhostTask::m_RTTI) : false)
			|| ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGhostTask::m_RTTI) : false))
			m_barButton[7]->Enable(false);
	}
}

// taskmain.cpp:1110
void CTaskMain::OnUnderBar_GameUp(void) {
	if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
		if (!CSharedDoc::Instance()->m_lobbyStateFlag) {
			WSendPacket packet((enumClientPacket)0x81);
			packet.Send(SEND_TO_DEFAULT);
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		} else {
			if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false))
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);

			CTaskManager::Instance()->PostMsg(NULL, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
		}
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1138
void CTaskMain::OnUnderBar_ServerInit(intptr_t param) {
	m_barButton[8] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[8]) {
		m_barButton[8]->SetPushSound("ui_desktop_icon_click");
		m_barButton[8]->EnableHover(true, 0.0f);

		if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI) : false)
			|| ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
			m_barButton[8]->Enable(false);
		else
			m_barButton[8]->Enable(true);
	}
}

// taskmain.cpp:1155
void CTaskMain::OnUnderBar_ServerUp(void) {
	if (!(((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI) : false)
		|| ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))) {
		IActor *room = AfxGetTask()->GetActor("RealMyRoom");
		if (room)
			room << MsgObject(this, 0x68, 0, 0, 0, 0, 0);

		CloseAllDialogs();

		WSendPacket packet((enumClientPacket)0x43);
		packet.Send(SEND_TO_DEFAULT);

		m_serverDlg = CreateForm<FrServerDlg>(g_pFresh->GetManager(), this, "server", 0);
		m_serverDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnServerDlgResult, WPoint(m_serverDlg->GetRect().x, 50.0f), 0x11);
		m_serverDlg->SetIconRect(m_barButton[8]->GetRect());
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1178
void CTaskMain::OnUnderBar_ExitInit(intptr_t param) {
	m_barButton[9] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[9]) {
		m_barButton[9]->SetPushSound("ui_desktop_icon_click");
		m_barButton[9]->EnableHover(true, 0.0f);
	}
}

// taskmain.cpp:1188
void CTaskMain::OnUnderBar_ExitUp(void) {
	CloseAllDialogs();

	if (g_pFresh->IsCurrentLayout("AVATARCHAT_MAIN")) {
		if (WSingleton<CQuitWindow>::Instance())
			WSingleton<CQuitWindow>::Instance()->Process();
	} else {
		m_quitDlg = CreateForm<FrLogoutDlg>(g_pFresh->GetManager(), this, "quit", 0);
		m_quitDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnQuitDlgResult, 0x11);

		if (m_barButton[9])
			m_quitDlg->SetIconRect(m_barButton[9]->GetRect());

		if (g_pFresh->IsCurrentLayout("GAMEROOM") || g_pFresh->IsCurrentLayout("GAMEROOM_EXT"))
			((FrLogoutDlg*)m_quitDlg)->DisableServerSelect();

		m_quitDlg->MoveCursor(g_pFresh->IsCurrentLayout("TOPPAGE") ? "logout" : "server");
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1222
void CTaskMain::OnUnderBar_TabToolTipInit(intptr_t param) {
	m_underTab = DYNAMIC_CAST(FrArea, param);
	if (m_underTab) {
		m_underTab->SetVisible(WSingleton<COption>::Instance()->gGetUnderExtBtn() == 0);
	}
}

// taskmain.cpp:1232
void CTaskMain::OnUnderBar_TabBtnInit(intptr_t param) {
	m_underBar = DYNAMIC_CAST(FrButton, param);
	if (m_underBar) {
		m_underBar->SetPushDelay(0.0f);

		if (WSingleton<COption>::Instance()->gGetUnderExtBtn() == 1)
			m_underBar->EnableBlink(false);
	}
}

// taskmain.cpp:1245
void CTaskMain::OnUnderBar_TabBtnUp(void) {
	if (m_underBarEnabled)
		TurnOffUnderTabBar();
	else
		TurnOnUnderTabBar();

	if (m_underTab && m_underTab->IsVisible()) {
		WSingleton<COption>::Instance()->gSetUnderExtBtn(1);
		m_underTab->SetVisible(false);
		m_underBar->EnableBlink(false);
	}

	OnBarHoverOff(0);
}

// taskmain.cpp:1263
void CTaskMain::TurnOnUnderTabBar(void) {
	if (!m_underBarEnabled) {
		m_underBarEnabled = true;
		m_barFrame[1]->SetVisible(true);

		m_barButton[0x0E]->Enable(IsLocalContent((localContentType_t)5));
		m_barButton[0x14]->Enable(IsLocalContent((localContentType_t)2));
		m_barButton[0x10]->Enable(!IsLocalCountry(0x10));

		WRect rect = m_underBar->GetRect();
		rect.y -= 53.0f;

		m_underBar->SetRect(rect);

		std::string name;
		m_underBar->GetWindowName(name);
		if (stricmp(name.c_str(), "tabbtn_b") == 0) {
			m_underBar->SetButtonImg("underbar_tab_bd_n", FrButton::NORMAL);
			m_underBar->SetButtonImg("underbar_tab_bd_o", FrButton::OVER);
		} else {
			m_underBar->SetButtonImg("underbar_tab_d_n", FrButton::NORMAL);
			m_underBar->SetButtonImg("underbar_tab_d_o", FrButton::OVER);
		}
	}
}

// taskmain.cpp:1304
void CTaskMain::TurnOffUnderTabBar(void) {
	if (m_underBarEnabled) {
		m_underBarEnabled = false;
		m_barFrame[1]->SetVisible(false);

		WRect rect = m_underBar->GetRect();
		rect.y += 53.0f;

		m_underBar->SetRect(rect);

		std::string name;
		m_underBar->GetWindowName(name);
		if (stricmp(name.c_str(), "tabbtn_b") == 0) {
			m_underBar->SetButtonImg("underbar_tab_bu_n", FrButton::NORMAL);
			m_underBar->SetButtonImg("underbar_tab_bu_o", FrButton::OVER);
		} else {
			m_underBar->SetButtonImg("underbar_tab_u_n", FrButton::NORMAL);
			m_underBar->SetButtonImg("underbar_tab_u_o", FrButton::OVER);
		}
	}
}

// taskmain.cpp:1333
void CTaskMain::OnUnderBar_MagicBoxInit(intptr_t param) {
	m_barButton[0x0E] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x0E]) {
		m_barButton[0x0E]->EnableHover(true, 0.0f);

		WRect rect = m_barButton[0x0E]->GetRect();
		rect.x += 74.0f;
		m_barButton[0x0E]->SetRect(rect);
	}
}

// taskmain.cpp:1348
void CTaskMain::OnUnderBar_MagicBoxUp(void) {
	const char *message;

	if (CSharedDoc::Instance()->IsControlServerService(0x8000)) {
		message = "\xB8\xB6\xB9\xFD\xBB\xF3\xC0\xDA \xB0\xFC\xB7\xC3 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.";
	} else if (WSingleton<CSharedDoc>::Instance()->m_userInfo.flagBlock & 0x40000000) {
		message = "\\c0xffff0000\\c \xC4\xAB\xB5\xE5\xC1\xA4\xBA\xB8 \xC0\xCC\xBB\xF3\xC0\xB8\xB7\xCE \xB8\xB6\xB9\xFD\xBB\xF3\xC0\xDA\\c0xff000000\\c\xB8\xA6 \xC0\xCC\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.";
	} else {
		if (IsLocalContent((localContentType_t)5) && m_recycleDlg == NULL) {
			m_recycleDlg = CreateForm<FrRecycleDlg>(g_pFresh->GetManager(), this, "recycledlg", 0);
			m_recycleDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnRecycleDlgResult, 0xC1);

			if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
				WSingleton<ntAvatarManager>::Instance()->SetActive(false);
		}

		__rtti_obj = 0;
		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
		return;
	}

	this << MsgObject(this, 0x23, (intptr_t)message, 0, 0, 0, 0);
}

// taskmain.cpp:1389
void CTaskMain::OnUnderBar_TikiMagicBoxInit(int) {
}

// taskmain.cpp:1393
void CTaskMain::OnUnderBar_TikiMagicBoxUp(void) {
	if (IsLocalContent((localContentType_t)0x79) == true && m_tikiMagicBoxDlg == NULL) {
		m_tikiMagicBoxDlg = CreateForm<FrTikiMagicBoxDlg>(g_pFresh->GetManager(), this, "tikimagicbox", 0);
		m_tikiMagicBoxDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnTikiMagicBoxDlgResult, 0xC1);

		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)
			WSingleton<ntAvatarManager>::Instance()->SetActive(false);
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1487
void CTaskMain::OnUnderBar_ScratchInit(intptr_t param) {
	m_barButton[0x14] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x14]) {
		m_barButton[0x14]->EnableHover(true, 0.0f);

		WRect rect = m_barButton[0x14]->GetRect();
		rect.x += 74.0f;
		m_barButton[0x14]->SetRect(rect);
	}
}

// taskmain.cpp:1506
void CTaskMain::OnUnderBar_ScratchUp(void) {
	if (CSharedDoc::Instance()->IsControlServerService(0x4000)) {
		this << MsgObject(this, 0x23, (intptr_t)"\xBD\xBA\xC5\xA9\xB7\xA1\xC4\xA1 \xB0\xFC\xB7\xC3 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		return;
	}

	if (IsLocalContent((localContentType_t)2)) {
		if (WSingleton<CSharedDoc>::Instance()->m_value5064 != 2) {
			if (m_scratchDlg == NULL) {
				m_scratchDlg = CreateForm<FrScratchDlg>(g_pFresh->GetManager(), this, "scratchdlg", 0);
				m_scratchDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnScratchDlgResult, 0xC1);

				if (((__rtti_obj = (CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL)) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
					WSingleton<ntAvatarManager>::Instance()->SetActive(false);
			}
		} else {
			FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			WRect rect = notify->GetRect();
			notify->SetRect(rect);
			notify->SetTimeLimit(12.0f);
			notify->SetMessage("\xC7\xF6\xC0\xE7 \xC7\xD1 \xC1\xBE\xB7\xF9\xC0\xC7 \xBE\xC6\xC0\xCC\xC5\xDB \xBA\xB8\xC0\xAF \xB0\xB3\xBC\xF6\xB0\xA1 20,000\xB0\xB3\xC0\xCC\xB9\xC7\xB7\xCE \xC7\xD8\xB4\xE7 \xC4\xC1\xC5\xD9\xC3\xF7\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			notify->Open(0, 0);
		}
	}

	__rtti_obj = 0;
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1563
void CTaskMain::OnUnderBar_BongdariShopInit(intptr_t param) {
	m_barButton[15] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[15]) {
		m_barButton[15]->SetPushSound("ui_desktop_icon_click");
		m_barButton[15]->EnableHover(true, 0.0f);

		WRect rect = m_barButton[15]->GetRect();
		rect.x += 74.0f;
		m_barButton[15]->SetRect(rect);
	}
}

// taskmain.cpp:1579
void CTaskMain::OnUnderBar_BongdariShopUp(void) {
	if (CSharedDoc::Instance()->IsControlServerService(0x10)) {
		this << MsgObject(this, 0x23, (intptr_t)"\xBA\xC0\xB4\xD9\xB8\xAE\xBC\xA5 \xB0\xFC\xB7\xC3 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_value5064 != 2) {
		if (!m_bongdariShop[1]) {
			FrWnd *shop = FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"));
			if (shop == NULL) {
				WSendPacket packet((enumClientPacket)0x95);
				packet.Send(SEND_TO_DEFAULT);

				WSingleton<CSharedDoc>::Instance()->m_pointEventState = 1;
			} else {
				shop->Close(true);
			}
		}
	} else {
		FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		WRect rect = notify->GetRect();
		notify->SetRect(rect);
		notify->SetTimeLimit(12.0f);
		notify->SetMessage("\xC7\xF6\xC0\xE7 \xC7\xD1 \xC1\xBE\xB7\xF9\xC0\xC7 \xBE\xC6\xC0\xCC\xC5\xDB \xBA\xB8\xC0\xAF \xB0\xB3\xBC\xF6\xB0\xA1 20,000\xB0\xB3\xC0\xCC\xB9\xC7\xB7\xCE \xC7\xD8\xB4\xE7 \xC4\xC1\xC5\xD9\xC3\xF7\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		notify->Open(0, 0);
	}

	m_bongdariShop[0] = false;
	m_bongdariShop[1] = false;
	__rtti_obj = 0;
	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1646
void CTaskMain::OnUnderBar_MaketInit(intptr_t param) {
	m_barButton[0x13] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x13]) {
		m_barButton[0x13]->EnableHover(true, 0.0f);
		m_barButton[0x13]->SetVisible(false);
	}
}

// taskmain.cpp:1661
void CTaskMain::OnUnderBar_MaketUp(void) {
	if (!IsLocalContent((localContentType_t)0x66)) {
		this << MsgObject(this, 0x23, (intptr_t)"\xC6\xCE\xBE\xDF\xB8\xB6\xC4\xCF \xB9\xD7 \xC1\xF8\xBF\xAD\xB4\xEB \xB1\xE2\xB4\xC9\xB0\xFC\xB7\xC3 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		return;
	}

	if (CSharedDoc::Instance()->IsControlServerService(0x80000)) {
		this << MsgObject(this, 0x23, (intptr_t)"\xC6\xCE\xBE\xDF\xB8\xB6\xC4\xCF \xB9\xD7 \xC1\xF8\xBF\xAD\xB4\xEB \xB1\xE2\xB4\xC9\xB0\xFC\xB7\xC3 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		return;
	}

	if (m_barButton[0x13]) {
		CloseAllDialogs();

		if (IsLocalContent((localContentType_t)0x66)) {
			WSendPacket packet((enumClientPacket)0xCA);
			packet.Encode2(1);
			packet.Send(SEND_TO_DEFAULT);
		}

		CTaskManager::Instance()->GetCurrentTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

		__rtti_obj = 0;
		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
	}
}

// taskmain.cpp:1717
void CTaskMain::CreateMaketDlg(void) {
	if (m_maketListDlg == NULL) {
		m_maketListDlg = CreateForm<COfflineTradeDlg>(g_pFresh->GetManager(), this, "offlinetraderanking", 0);
		m_maketListDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnMaketListDlgResult, 0x41);
	}
}

// taskmain.cpp:1727
void CTaskMain::OnUnderBar_RankingInit(intptr_t param) {
	m_barButton[0x10] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x10]) {
		m_barButton[0x10]->EnableHover(true, 0.0f);

		WRect rect = m_barButton[0x10]->GetRect();
		rect.x += 74.0f;
		m_barButton[0x10]->SetRect(rect);
	}
}

// taskmain.cpp:1747
void CTaskMain::OnUnderBar_RankingUp(void) {
	if (CSharedDoc::Instance()->IsControlServerService(0x10000)) {
		// the ranking service is closed by the server
		HandleMsg(MsgObject(this, 0x23, (intptr_t)"·©Å· °ü·Ã ºÎºÏ Á¡°ÊÁßÀÔ´Ï´Ù.", 0, 0, 0, 0));
		return;
	}

	CloseAllDialogs();

	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		WSendPacket packet((enumClientPacket)0x47);
		packet.Send(SEND_TO_DEFAULT);
	} else {
		WNetworkSystem::Instance()->Init(WNetworkSystem::NET_UNIT_RANK);
		WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_RANK, (NetworkUnit::eEvent)0);
	}

	if (m_loginRsDlg == NULL) {
		m_loginRsDlg = CreateForm<FrLoginRsDlg>(g_pFresh->GetManager(), this, "login_gs", 0);
		FrLoginRsDlg::SetState(LOGIN_TO_RS_CONNECTING);
		m_loginRsDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnLoginRsDlgResult, 0x41);
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:1783
void CTaskMain::OnUnderBar_GuildInit(intptr_t param) {
	m_barButton[0x11] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x11]) {
		m_barButton[0x11]->EnableHover(true, 0.0f);
		m_barButton[0x11]->SetVisible(false);
	}
}

// taskmain.cpp:1802
void CTaskMain::OnUnderBar_GuildUp(void) {
	CloseAllDialogs();
	WSingleton<COption>::Instance()->vApplyLobbyScreenSize();

	char url[1024] = { 0 };
	sprintf(url, "http://www.pangya.com/new3/800x600/index.asp?userid=%s&pwd=%s", WSingleton<CSharedDoc>::Instance()->m_myUID, LOGININFO()->Passwd());
	WSingleton<CBrowser>::Instance()->Open(url, ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) != 0, true);
	OnBarHoverOff(0);
}

// taskmain.cpp:1830
void CTaskMain::OnUnderBar_NoticeInit(intptr_t param) {
	m_barButton[0x12] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[0x12]) {
		m_barButton[0x12]->EnableHover(true, 0.0f);

		WRect rect = m_barButton[0x12]->GetRect();
		rect.x += 24.5f;
		m_barButton[0x12]->SetRect(rect);
	}
}

// taskmain.cpp:1845
void CTaskMain::OnUnderBar_NoticeUp(void) {
	CloseAllDialogs();

	m_noticeDlg = CreateForm<FrNoticeDlg>(g_pFresh->GetManager(), this, "notice_popup", 0);
	m_noticeDlg->LoadInitFile("notice_popup.ini");
	m_noticeDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnNoticeDlgResult, 0x11);

	if (m_barButton[0x12] && m_noticeDlg)
		m_noticeDlg->SetRect(m_barButton[0x12]->GetRect());

	OnBarHoverOff(0);
	WSingleton<CSharedDoc>::Instance()->m_noticeShown = false;
}

// taskmain.cpp:1917
void CTaskMain::InitOverBar(void) {
	m_overBarTab = NULL;

	m_charViewer = NULL;
	m_caddieViewer = NULL;
	m_aztecViewer = NULL;
	m_clubViewer = NULL;
	m_subSelectionWnd[0] = NULL;
	m_subSelectionWnd[1] = NULL;

	m_subSelectionWnd[3] = NULL;
	m_subSelectionWnd[2] = NULL;
	m_subSelectionWnd[4] = NULL;
	m_siteWnd = NULL;
	m_pangCookieWnd = NULL;
	m_titleArea = NULL;
	m_oneLineView = NULL;
	m_oneLineReqBtn = NULL;
	m_showSubSelection = false;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	m_overBarButtonDown = false;
	m_subSelectionPending = false;
}

// taskmain.cpp:1944
void CTaskMain::ProcessOverBar(void) {
	if (g_input->GetButton(LEFT_BUTTON) == 2)
		m_overBarButtonDown = true;

	if (m_subSelectionPending && m_overBarButtonDown) {
		if (g_input->GetButton(LEFT_BUTTON) == 1) {
			m_overBarButtonDown = false;
			m_subSelectionPending = false;
			SendSubSelectionPacket((PANGYA_ITEM_GROUP)0x20);
		}
	}
}

// taskmain.cpp:2047
void CTaskMain::ShowOverAndUnderBar(bool show) {
	if (data_a7cea0 != show) {
		data_a7cea0 = show;

		for (int i = 0; i < 3; i++) {
			if (m_barFrame[i]) {
				if (show) {
					m_barFrame[i]->SetVisible(data_af1a90[i]);
				} else {
					data_af1a90[i] = m_barFrame[i]->IsVisible();
					m_barFrame[i]->SetVisible(false);
				}
			}
		}

		if (m_barButton[0x13])
			m_barButton[0x13]->SetVisible(false);
	}
}

// taskmain.cpp:2084
void CTaskMain::EnableOverAndUnderBar(bool enable) {
	FrWnd *wnds[] = {
		m_charViewer, m_caddieViewer, m_aztecViewer, m_clubViewer,
		m_subSelectionWnd[0], m_subSelectionWnd[1], m_subSelectionWnd[2], m_subSelectionWnd[3], m_subSelectionWnd[4],
		m_pangCookieWnd, m_titleArea, m_oneLineView, m_oneLineReqBtn,
		m_barButton[0], m_barButton[1], m_barButton[2], m_barButton[3], m_barButton[4],
		m_barButton[5], m_barButton[6], m_barButton[15], m_barButton[7], m_barButton[8],
		m_barButton[9], m_underBar, m_barFrame[0], m_barFrame[2], m_barFrame[1],
		m_barButton[14], m_barButton[20], m_barButton[16], m_barButton[17], m_barButton[18],
		m_mascotViewer, m_barButton[19],
	};

	if (data_a7cea1 != enable) {
		data_a7cea1 = enable;

		for (unsigned int i = 0; i < 35; i++) {
			if (wnds[i]) {
				if (enable) {
					wnds[i]->Enable(data_af1a94[i]);
				} else {
					data_af1a94[i] = wnds[i]->IsEnabled();
					wnds[i]->Enable(false);
				}
			}
		}

		if (m_barButton[19])
			m_barButton[19]->SetVisible(false);
	}
}

// taskmain.cpp:2169
void CTaskMain::EnableUnderBar(bool enable) {
	m_barButton[2]->Enable(enable);
	m_barButton[3]->Enable(enable);
	m_barButton[4]->Enable(enable);
	m_barButton[5]->Enable(enable);
	m_barButton[6]->Enable(enable);
	m_barButton[15]->Enable(enable);
	m_barButton[7]->Enable(enable);
	m_barButton[8]->Enable(enable);
	m_underBar->Enable(enable);
}

// taskmain.cpp:2182
void CTaskMain::ShowAllSubSelection(void) {
	m_showSubSelection = true; m_subSelection = 0;
	m_subSelectionTab = 0;
}

// taskmain.cpp:2189
void CTaskMain::CloseAllSubSelection(void) {
	if (m_subSelectionWnd[0]) m_subSelectionWnd[0]->SetVisible(false);
	if (m_subSelectionWnd[1]) m_subSelectionWnd[1]->SetVisible(false);
	if (m_subSelectionWnd[1]) m_subSelectionWnd[1]->SetVisible(false);
	if (m_subSelectionWnd[2]) m_subSelectionWnd[2]->SetVisible(false);
	if (m_subSelectionWnd[3]) m_subSelectionWnd[3]->SetVisible(false);
	if (m_subSelectionWnd[4]) m_subSelectionWnd[4]->SetVisible(false);
	m_showSubSelection = false;
}

// taskmain.cpp:2200
void CTaskMain::OnOverBar_Bar1Init(int param) {
	m_overBarArea[0] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarArea[0]) {
		if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty()))
			m_overBarArea[0]->SetBgImg("classic_over_bar_01");
	}
}

// taskmain.cpp:2210
void CTaskMain::OnOverBar_Bar2Init(int param) {
	m_overBarArea[1] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarArea[1]) {
		float w = m_overBarArea[2]->GetRect().x - m_overBarArea[0]->GetRect().x - m_overBarArea[0]->GetRect().w;

		WRect rect = m_overBarArea[1]->GetRect();
		rect.w = w;
		m_overBarArea[1]->SetRect(rect);
	}
}

// taskmain.cpp:2223
void CTaskMain::OnOverBar_Bar3Init(int param) {
	m_overBarArea[2] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarArea[2]) {
		SetLeftOffset(m_overBarArea[2], m_screenWidth);
	}
}

// taskmain.cpp:2232
void CTaskMain::OnOverBar_TitleInit(intptr_t param) {
	m_titleArea = DYNAMIC_CAST(FrArea, param);
}

// taskmain.cpp:2239
void CTaskMain::OnOverBar_Frame1Init(int param) {
	m_overBarFrame[0] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarFrame[0]) {
		const WRect &cur = m_overBarFrame[0]->GetRect();
		if (m_screenWidth < 800.0f)
			m_overBarFrame[0]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_overBarFrame[0]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2252
void CTaskMain::OnOverBar_Frame2Init(int param) {
	m_overBarFrame[1] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarFrame[1]) {
		const WRect &cur = m_overBarFrame[1]->GetRect();
		if (m_screenWidth < 800.0f)
			m_overBarFrame[1]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_overBarFrame[1]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2265
void CTaskMain::OnOverBar_Frame3Init(int param) {
	m_overBarFrame[2] = DYNAMIC_CAST(FrArea, param);
	if (m_overBarFrame[2]) {
		const WRect &cur = m_overBarFrame[2]->GetRect();
		if (m_screenWidth < 800.0f)
			m_overBarFrame[2]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_overBarFrame[2]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2278
void CTaskMain::OnOverBar_TabInit(int param) {
	m_overBarTab = DYNAMIC_CAST(FrTabButton, param);
	if (m_overBarTab) {
		const WRect &cur = m_overBarTab->GetRect();
		if (m_screenWidth < 800.0f)
			m_overBarTab->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_overBarTab->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2291
void CTaskMain::OnOverBar_TabDown(void) {
}

// taskmain.cpp:2316
void CTaskMain::OnOverBar_CharInit(intptr_t param) {
	m_charViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_charViewer) {
		const WRect &cur = m_charViewer->GetRect();
		if (m_screenWidth < 800.0f)
			m_charViewer->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_charViewer->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));

		m_charViewer->Enable(!g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"));
	}
}

// taskmain.cpp:2331
void CTaskMain::OnOverBar_CharDown(void) {
	WRect rect = m_subSelectionWnd[0]->GetRect();

	WPoint pt(rect.x, rect.y + rect.h);
	// PANGYA_ITEM_GROUP has no enumerator names recovered yet.
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)1, 3);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();
		barRect.x = rect.x;
		m_subEquipBar->SetRect(barRect);
	}
}

// taskmain.cpp:2346
void CTaskMain::OnOverBar_CharOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	unsigned long color = m_charViewer->IsEnabled() ? 0xFFFFFFFF : 0x50B0B0B0;
	WRect rect = m_charViewer->GetRect();
	const Bitmap *bitmap = NULL;
	unsigned int characterId = WSingleton<CSharedDoc>::Instance()->m_myCharacterId;

	if (characterId == 0) {
		bitmap = g_pFresh->GetBitmap("bar_no");
	} else {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(WSingleton<CSharedDoc>::Instance()->m_characterMap.find(characterId)->second.typeId);
		if (item)
			bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
	}

	if (bitmap)
		gdi->DrawTexture(bitmap, rect, color, 0);

	_DrawSelection(m_charViewer, rect, (eBarButton)0x28, gdi);
}

// taskmain.cpp:2373
void CTaskMain::_DrawSelection(FrViewer *viewer, WRect &rect, eBarButton button, FrGraphicInterface *gdi) {
	if (gdi == NULL) {
		gdi = g_pFresh->GetManager()->GetGDI();
		if (gdi == NULL)
			return;
	}

	WVector mouse = g_input->GetMousePoint();
	WPoint pt(mouse.x, mouse.y);
	if (viewer->IsEnabled() && rect.IsInRect(pt)) {
		// hovered: frame the viewer and show its tooltip
		const Bitmap *bitmap = g_pFresh->GetBitmap("select_frame");
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);

		CIconToolTip::GetInstance()->SetToolTip(button);
	} else {
		if (CIconToolTip::GetInstance()->IsCurToolTip(button)) {
			m_hoverButton = NULL;
			CIconToolTip::GetInstance()->ClearToolTip();
		}
	}
}

// taskmain.cpp:2423
void CTaskMain::NotifyServerControlMsg(int property) {
	const char *message;
	if (_util::IsAvatarChatServer(0x40))
		message = _util::GetAvatarServerServiceMessage(property);
	else
		message = _util::GetNormalServerServiceMessage(property);

	if (message) {
		this << MsgObject(NULL, 0x23, (intptr_t)message, 0, 0, 0, 0);
	}
}

// taskmain.cpp:2446
void CTaskMain::OnOverBar_CaddieInit(intptr_t param) {
	m_caddieViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_caddieViewer) {
		const WRect &cur = m_caddieViewer->GetRect();
		if (m_screenWidth < 800.0f)
			m_caddieViewer->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_caddieViewer->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));

		m_caddieViewer->Enable(!g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"));
	}
}

// taskmain.cpp:2461
void CTaskMain::OnOverBar_CaddieDown(void) {
	WRect rect = m_subSelectionWnd[1]->GetRect();

	WPoint pt(rect.x, rect.y + rect.h);
	// PANGYA_ITEM_GROUP has no enumerator names recovered yet.
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)7, 3);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();
		barRect.x = rect.x;
		m_subEquipBar->SetRect(barRect);
	}
}

// taskmain.cpp:2476
void CTaskMain::OnOverBar_CaddieOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	unsigned long color = m_caddieViewer->IsEnabled() ? 0xFFFFFFFF : 0x50B0B0B0;
	WRect rect = m_caddieViewer->GetRect();
	const Bitmap *bitmap = NULL;
	bool expired = false;
	unsigned int caddieId = WSingleton<CSharedDoc>::Instance()->m_myCaddieId;

	if (caddieId == 0) {
		bitmap = g_pFresh->GetBitmap("bar_no");
	} else {
		std::map<unsigned int, sCaddieInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.find(caddieId);
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.typeId);
		if (item)
			bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));

		// a caddie on contract whose days ran out: crossed out in red
		IFF_STRUCT::sCaddie *caddie = ItemManager()->FindCaddie(it->second.typeId);
		if (caddie) {
			if ((it->second.flags & 2) && it->second.days <= 0 && it->second.cadItemTypeId == 0 && caddie->fee != 0)
				expired = true;
		}
	}

	if (bitmap)
		gdi->DrawTexture(bitmap, rect, color, 0);

	if (expired) {
		_WPOINT a, b;
		a.x = rect.x; a.y = rect.y + rect.h * 0.5f - 1.0f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
		g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
		a.x = rect.x; a.y = rect.y + rect.h * 0.5f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
		g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
		a.x = rect.x; a.y = rect.y + rect.h * 0.5f + 1.0f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
		g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
	}

	_DrawSelection(m_caddieViewer, rect, (eBarButton)0x29, gdi);
}

// taskmain.cpp:2520
void CTaskMain::OnOverBar_ClubInit(intptr_t param) {
	m_clubViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_clubViewer) {
		const WRect &cur = m_clubViewer->GetRect();
		if (m_screenWidth < 800.0f)
			m_clubViewer->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_clubViewer->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));

		m_clubViewer->Enable(!g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"));
	}
}

// taskmain.cpp:2535
void CTaskMain::OnOverBar_ClubDown(void) {
	WRect rect = m_subSelectionWnd[3]->GetRect();

	WPoint pt(rect.x, rect.y + rect.h);
	// PANGYA_ITEM_GROUP has no enumerator names recovered yet.
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)4, 3);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();
		barRect.x = rect.x;
		m_subEquipBar->SetRect(barRect);
	}
}

// taskmain.cpp:2550
void CTaskMain::OnOverBar_ClubOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	unsigned long color = m_clubViewer->IsEnabled() ? 0xFFFFFFFF : 0x50B0B0B0;
	WRect rect = m_clubViewer->GetRect();
	const Bitmap *bitmap = NULL;

	if (WSingleton<CSharedDoc>::Instance()->m_myClubId == 0) {
		bitmap = g_pFresh->GetBitmap("bar_no");
	} else {
		std::map<unsigned int, sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.find(WSingleton<CSharedDoc>::Instance()->m_myClubId);
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.m_typeId);
		if (item)
			bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
	}

	if (bitmap)
		gdi->DrawTexture(bitmap, rect, color, 0);

	_DrawSelection(m_clubViewer, rect, (eBarButton)0x2A, gdi);
}

// taskmain.cpp:2577
void CTaskMain::OnOverBar_AztecInit(intptr_t param) {
	m_aztecViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_aztecViewer) {
		const WRect &cur = m_aztecViewer->GetRect();
		if (m_screenWidth < 800.0f)
			m_aztecViewer->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_aztecViewer->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));

		m_aztecViewer->Enable(!g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"));
	}
}

// taskmain.cpp:2592
void CTaskMain::OnOverBar_AztecDown(void) {
	WRect rect = m_subSelectionWnd[2]->GetRect();

	WPoint pt(rect.x, rect.y + rect.h);
	// PANGYA_ITEM_GROUP has no enumerator names recovered yet.
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)5, 3);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();
		barRect.x = rect.x;
		m_subEquipBar->SetRect(barRect);
	}
}

// taskmain.cpp:2607
void CTaskMain::OpenSubEquipBar(const WPoint &pt, PANGYA_ITEM_GROUP group, int subType) {
	if (m_subEquipBar == NULL) {
		m_subEquipBar = CreateForm<FrSubEquipBar>(g_pFresh->GetManager(), this, "EQUIPBAR_SUB", 0);
		if (m_subEquipBar) {
			m_subEquipBar->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnResultSubEquipBar,
				(FrSubEquipBar::eCategory)group, pt, (FrSubEquipBar::eSubType)subType);
		}
	}
}

// taskmain.cpp:2677
bool CTaskMain::OnResultSubEquipBar(int result, FrForm *) {
	PANGYA_ITEM_GROUP group = (PANGYA_ITEM_GROUP)0x20;
	if (result == 1) {
		IFF_ITEM_COMMON *item = m_subEquipBar->GetResult();
		if (item != NULL) {
			unsigned long typeId = item->m_typeId;
			unsigned long itemGroup = typeId >> 26;
			if (itemGroup == 5) {
				// a ball: no item guid to equip
				CSharedDoc::Instance()->m_myBallId = typeId;
				m_subSelection = typeId;
				m_subSelectionTab = 0;
			} else {
				m_subSelection = typeId;
				m_subSelectionTab = m_subEquipBar->GetLastSelectedGuid();
				if (!m_subEquipBar->SetClientEquipInfo((PANGYA_ITEM_GROUP)itemGroup, m_subSelectionTab)) {
					m_subEquipBar = NULL;
					return true;
				}
			}
		} else {
			// nothing picked: unequip the caddie or the mascot
			switch (m_subEquipBar->GetCategory()) {
			case 0x10:
				CSharedDoc::Instance()->m_myMascotId = 0;
				group = (PANGYA_ITEM_GROUP)0x10;
				break;
			case 7:
				CSharedDoc::Instance()->m_myCaddieId = 0;
				group = (PANGYA_ITEM_GROUP)7;
				break;
			}
			m_subSelection = 0;
			m_subSelectionTab = 0;
		}
		SendSubSelectionPacket(group);
	}
	m_subEquipBar = NULL;
	return true;
}

// taskmain.cpp:2746
void CTaskMain::OnOverBar_AztecOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	unsigned long color = m_aztecViewer->IsEnabled() ? 0xFFFFFFFF : 0x50B0B0B0;
	WRect rect = m_aztecViewer->GetRect();
	const Bitmap *bitmap = NULL;

	if (WSingleton<CSharedDoc>::Instance()->m_myBallId == 0) {
		bitmap = g_pFresh->GetBitmap("bar_no");
	} else {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(WSingleton<CSharedDoc>::Instance()->m_myBallId);
		if (item)
			bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
	}

	if (bitmap)
		gdi->DrawTexture(bitmap, rect, color, 0);

	_DrawSelection(m_aztecViewer, rect, (eBarButton)0x2B, gdi);
}

// taskmain.cpp:2772
void CTaskMain::OnOverBar_MascotInit(intptr_t param) {
	m_mascotViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_mascotViewer) {
		const WRect &cur = m_mascotViewer->GetRect();
		if (m_screenWidth < 800.0f)
			m_mascotViewer->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_mascotViewer->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));

		m_mascotViewer->Enable(!g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"));
	}
}

// taskmain.cpp:2787
void CTaskMain::OnOverBar_MascotDown(void) {
	WRect rect = m_subSelectionWnd[4]->GetRect();

	WPoint pt(rect.x, rect.y + rect.h);
	// PANGYA_ITEM_GROUP has no enumerator names recovered yet.
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)0x10, 3);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();
		barRect.x = rect.x;
		m_subEquipBar->SetRect(barRect);
	}
}

// taskmain.cpp:2802
void CTaskMain::OnOverBar_MascotOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	unsigned long color = m_mascotViewer->IsEnabled() ? 0xFFFFFFFF : 0x50B0B0B0;
	WRect rect = m_mascotViewer->GetRect();
	const Bitmap *bitmap = NULL;

	if (WSingleton<CSharedDoc>::Instance()->m_myMascotId == 0) {
		bitmap = g_pFresh->GetBitmap("bar_no");
	} else {
		std::map<unsigned int, sMascotInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.find(WSingleton<CSharedDoc>::Instance()->m_myMascotId);
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.typeId);
		if (item)
			bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
	}

	if (bitmap)
		gdi->DrawTexture(bitmap, rect, color, 0);

	_DrawSelection(m_mascotViewer, rect, (eBarButton)0x2C, gdi);
}

// taskmain.cpp:2829
void CTaskMain::OnOverBar_CharSelInit(intptr_t param) {
	m_subSelectionWnd[0] = DYNAMIC_CAST(FrViewer, param);
	if (m_subSelectionWnd[0]) {
		m_subSelectionWnd[0]->SetVisible(false);

		const WRect &cur = m_subSelectionWnd[0]->GetRect();
		if (m_screenWidth < 800.0f)
			m_subSelectionWnd[0]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_subSelectionWnd[0]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2844
void CTaskMain::OnOverBar_CharSelOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	int count = (int)WSingleton<CSharedDoc>::Instance()->m_characterMap.size();
	if (count <= 1)
		return;

	// the bar: a left cap, a middle sized to the icons, a right cap
	float x = m_subSelectionWnd[0]->GetRect().x;
	float y = m_subSelectionWnd[0]->GetRect().y;
	float w = 8.0f;

	const Bitmap *bitmap = g_pFresh->GetBitmap("select_bar_01");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	x += w;
	float itemX = x;
	bitmap = g_pFresh->GetBitmap("select_bar_02");
	if (bitmap) {
		w = (count - 1) * 3.0f + count * 24.0f;
		gdi->DrawTexture(bitmap, WRect(x, y, w, (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}

	x += w;
	bitmap = g_pFresh->GetBitmap("select_bar_03");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	y += 7.0f;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	// the icons; the hovered one, if not the equipped character, is the selection
	for (std::map<unsigned int, sCharacterInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_characterMap.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_characterMap.end(); ++it) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.typeId);
		if (item == NULL)
			continue;

		WRect rect(itemX, y, 24.0f, 26.0f);
		bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
		if (bitmap)
			gdi->DrawTexture(bitmap, rect, 0xFFFFFFFF, 0);

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (rect.IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);

			if (WSingleton<CSharedDoc>::Instance()->m_myCharacterId != it->second.id) {
				m_subSelection = it->second.typeId;
				m_subSelectionTab = it->second.id;
			}
		}

		itemX += 27.0f;
	}
}

// taskmain.cpp:2920
void CTaskMain::OnOverBar_CaddieSelInit(intptr_t param) {
	m_subSelectionWnd[1] = DYNAMIC_CAST(FrViewer, param);
	if (m_subSelectionWnd[1]) {
		m_subSelectionWnd[1]->SetVisible(false);

		const WRect &cur = m_subSelectionWnd[1]->GetRect();
		if (m_screenWidth < 800.0f)
			m_subSelectionWnd[1]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_subSelectionWnd[1]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:2935
void CTaskMain::OnOverBar_CaddieSelOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	int count = (int)WSingleton<CSharedDoc>::Instance()->m_caddieMap.size();
	if (count < 1)
		return;

	// one more slot ("s_no") to take the equipped caddie off
	if (WSingleton<CSharedDoc>::Instance()->m_myCaddieId)
		count++;

	float x = m_subSelectionWnd[1]->GetRect().x;
	float y = m_subSelectionWnd[1]->GetRect().y;
	float w = 8.0f;

	const Bitmap *bitmap = g_pFresh->GetBitmap("select_bar_01");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	x += w;
	float itemX = x;
	bitmap = g_pFresh->GetBitmap("select_bar_02");
	if (bitmap) {
		w = (count - 1) * 3.0f + count * 24.0f;
		gdi->DrawTexture(bitmap, WRect(x, y, w, (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}

	x += w;
	bitmap = g_pFresh->GetBitmap("select_bar_03");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	y += 7.0f;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	for (std::map<unsigned int, sCaddieInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_caddieMap.end(); ++it) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.typeId);
		if (item == NULL)
			continue;

		WRect rect(itemX, y, 24.0f, 26.0f);
		bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
		if (bitmap)
			gdi->DrawTexture(bitmap, rect, 0xFFFFFFFF, 0);

		// a caddie on contract whose days ran out: crossed out in red
		IFF_STRUCT::sCaddie *caddie = ItemManager()->FindCaddie(it->second.typeId);
		if (caddie && (it->second.flags & 2) && it->second.days <= 0 && it->second.cadItemTypeId == 0 && caddie->fee != 0) {
			_WPOINT a, b;
			a.x = rect.x; a.y = rect.y + rect.h * 0.5f - 1.0f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
			a.x = rect.x; a.y = rect.y + rect.h * 0.5f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
			a.x = rect.x; a.y = rect.y + rect.h * 0.5f + 1.0f; b.x = rect.x + rect.w - 1.0f; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
		}

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (rect.IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);

			if (WSingleton<CSharedDoc>::Instance()->m_myCaddieId != it->second.id) {
				m_subSelection = it->second.typeId;
				m_subSelectionTab = it->second.id;
			}
		}

		itemX += 27.0f;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_myCaddieId) {
		WRect rect(itemX, y, 24.0f, 26.0f);
		bitmap = g_pFresh->GetBitmap("s_no");
		if (bitmap)
			gdi->DrawTexture(bitmap, rect, 0xFFFFFFFF, 0);

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (rect.IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);

			m_subSelection = WSingleton<CSharedDoc>::Instance()->m_caddieMap.begin()->second.typeId;
			m_subSelectionTab = 0;
		}
	}
}

// taskmain.cpp:3047
void CTaskMain::OnOverBar_ClubSelInit(intptr_t param) {
	m_subSelectionWnd[3] = DYNAMIC_CAST(FrViewer, param);
	if (m_subSelectionWnd[3]) {
		m_subSelectionWnd[3]->SetVisible(false);

		const WRect &cur = m_subSelectionWnd[3]->GetRect();
		if (m_screenWidth < 800.0f)
			m_subSelectionWnd[3]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_subSelectionWnd[3]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:3062
void CTaskMain::OnOverBar_ClubSelOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	int count = (int)WSingleton<CSharedDoc>::Instance()->m_myItemMap.size();
	if (count <= 1)
		return;

	float x = m_subSelectionWnd[3]->GetRect().x;
	float y = m_subSelectionWnd[3]->GetRect().y;
	float w = 8.0f;

	const Bitmap *bitmap = g_pFresh->GetBitmap("select_bar_01");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	x += w;
	float itemX = x;
	bitmap = g_pFresh->GetBitmap("select_bar_02");
	if (bitmap) {
		w = (count - 1) * 3.0f + count * 24.0f;
		gdi->DrawTexture(bitmap, WRect(x, y, w, (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}

	x += w;
	bitmap = g_pFresh->GetBitmap("select_bar_03");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	y += 7.0f;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	for (std::map<unsigned int, sItemInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_myItemMap.end(); ++it) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.m_typeId);
		if (item == NULL)
			continue;

		bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(itemX, y, 24.0f, 26.0f), 0xFFFFFFFF, 0);

		// a club set above the player's level: crossed out in red
		bool usable = WSingleton<CSharedDoc>::Instance()->CheckUnderLevelClubSet(it->second.m_typeId);
		if (!usable) {
			_WPOINT a, b;
			float right = itemX + 24.0f - 1.0f;
			float middle = y + 13.0f;
			a.x = itemX; a.y = middle - 1.0f; b.x = right; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
			a.x = itemX; a.y = middle; b.x = right; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
			a.x = itemX; a.y = middle + 1.0f; b.x = right; b.y = a.y;
			g_view->DrawLine2D(a, b, 0xFFFF0000, 0xFFFF0000, 0);
		}

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (WRect(itemX, y, 24.0f, 26.0f).IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(itemX - 3.0f, y - 3.0f, 30.0f, 32.0f), 0xFFFFFFFF, 0);

			if (WSingleton<CSharedDoc>::Instance()->m_myClubId != it->second.m_serial) {
				if (usable == true) {
					m_subSelection = it->second.m_typeId;
					m_subSelectionTab = it->second.m_serial;
				}
			}
		}

		itemX += 27.0f;
	}
}

// taskmain.cpp:3153
void CTaskMain::OnOverBar_AztecSelInit(intptr_t param) {
	m_subSelectionWnd[2] = DYNAMIC_CAST(FrViewer, param);
	if (m_subSelectionWnd[2]) {
		m_subSelectionWnd[2]->SetVisible(false);

		const WRect &cur = m_subSelectionWnd[2]->GetRect();
		if (m_screenWidth < 800.0f)
			m_subSelectionWnd[2]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_subSelectionWnd[2]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:3168
void CTaskMain::OnOverBar_AztecSelOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	if (ItemManager()->FindCommonItem(WSingleton<CSharedDoc>::Instance()->m_myBallId) == NULL)
		return;

	int count = (int)WSingleton<CSharedDoc>::Instance()->m_myGearList.size();
	if (count <= 1)
		return;

	float x = m_subSelectionWnd[2]->GetRect().x;
	float y = m_subSelectionWnd[2]->GetRect().y;
	float w = 8.0f;

	const Bitmap *bitmap = g_pFresh->GetBitmap("select_bar_01");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	x += w;
	float itemX = x;
	bitmap = g_pFresh->GetBitmap("select_bar_02");
	if (bitmap) {
		w = (count - 1) * 3.0f + count * 24.0f;
		gdi->DrawTexture(bitmap, WRect(x, y, w, (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}

	x += w;
	bitmap = g_pFresh->GetBitmap("select_bar_03");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	y += 7.0f;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	for (std::list<sItemInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_myGearList.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_myGearList.end(); ++it) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->m_typeId);
		if (item == NULL)
			continue;

		WRect rect(itemX, y, 24.0f, 26.0f);
		bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(itemX, y, 24.0f, 26.0f), 0xFFFFFFFF, 0);

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (rect.IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(itemX - 3.0f, y - 3.0f, 30.0f, 32.0f), 0xFFFFFFFF, 0);

			if (WSingleton<CSharedDoc>::Instance()->m_myBallId != it->m_typeId) {
				m_subSelection = it->m_typeId;
				m_subSelectionTab = it->m_serial;
			}
		}

		itemX += 27.0f;
	}
}

// taskmain.cpp:3248
void CTaskMain::OnOverBar_MascotSelInit(intptr_t param) {
	m_subSelectionWnd[4] = DYNAMIC_CAST(FrViewer, param);
	if (m_subSelectionWnd[4]) {
		m_subSelectionWnd[4]->SetVisible(false);

		const WRect &cur = m_subSelectionWnd[4]->GetRect();
		if (m_screenWidth < 800.0f)
			m_subSelectionWnd[4]->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_subSelectionWnd[4]->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:3263
void CTaskMain::OnOverBar_MascotSelOwnerDraw(int) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	int count = (int)WSingleton<CSharedDoc>::Instance()->m_myMascotMap.size();
	if (count < 1)
		return;

	// one more slot ("s_no") to take the equipped mascot off
	if (WSingleton<CSharedDoc>::Instance()->m_myMascotId)
		count++;

	float x = m_subSelectionWnd[4]->GetRect().x;
	float y = m_subSelectionWnd[4]->GetRect().y;
	float w = 8.0f;

	const Bitmap *bitmap = g_pFresh->GetBitmap("select_bar_01");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	x += w;
	float itemX = x;
	bitmap = g_pFresh->GetBitmap("select_bar_02");
	if (bitmap) {
		w = (count - 1) * 3.0f + count * 24.0f;
		gdi->DrawTexture(bitmap, WRect(x, y, w, (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}

	x += w;
	bitmap = g_pFresh->GetBitmap("select_bar_03");
	if (bitmap)
		gdi->DrawTexture(bitmap, WRect(x, y, 8.0f, (float)bitmap->Height()), 0xFFFFFFFF, 0);

	y += 7.0f;
	m_subSelection = 0;
	m_subSelectionTab = 0;

	for (std::map<unsigned int, sMascotInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_myMascotMap.end(); ++it) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(it->second.typeId);
		if (item == NULL)
			continue;

		bitmap = g_pFresh->GetBitmap(MakeStr("s_%s", item->m_icon));
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(itemX, y, 24.0f, 26.0f), 0xFFFFFFFF, 0);

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (WRect(itemX, y, 24.0f, 26.0f).IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(itemX - 3.0f, y - 3.0f, 30.0f, 32.0f), 0xFFFFFFFF, 0);

			if (WSingleton<CSharedDoc>::Instance()->m_myMascotId != it->second.id) {
				m_subSelection = it->second.typeId;
				m_subSelectionTab = it->second.id;
			}
		}

		itemX += 27.0f;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_myMascotId) {
		WRect rect(itemX, y, 24.0f, 26.0f);
		bitmap = g_pFresh->GetBitmap("s_no");
		if (bitmap)
			gdi->DrawTexture(bitmap, rect, 0xFFFFFFFF, 0);

		WVector mouse = g_input->GetMousePoint();
		WPoint pt(mouse.x, mouse.y);
		if (rect.IsInRect(pt)) {
			bitmap = g_pFresh->GetBitmap("select_frame");
			if (bitmap)
				gdi->DrawTexture(bitmap, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);

			m_subSelection = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.begin()->second.typeId;
			m_subSelectionTab = 0;
		}
	}
}

// taskmain.cpp:3463
void CTaskMain::OnOverBar_OptionInit(intptr_t param) {
	m_barButton[13] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[13]) {
		m_barButton[13]->EnableHover(true, 0.0f);
		m_barButton[13]->SetRect(WRect(m_screenWidth - (800.0f - m_barButton[13]->GetRect().x), m_barButton[13]->GetRect().y, m_barButton[13]->GetRect().w, m_barButton[13]->GetRect().h));
	}
}

// taskmain.cpp:3485
void CTaskMain::OnOverBar_MyInfoInit(intptr_t param) {
	m_barButton[12] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[12]) {
		m_barButton[12]->EnableHover(true, 0.0f);
		m_barButton[12]->SetRect(WRect(m_screenWidth - (800.0f - m_barButton[12]->GetRect().x), m_barButton[12]->GetRect().y, m_barButton[12]->GetRect().w, m_barButton[12]->GetRect().h));
	}
}

// taskmain.cpp:3828
void CTaskMain::OnTreasureAlarmInit(intptr_t param) {
	m_treasureAlarm = DYNAMIC_CAST(FrButton, param);
	if (m_treasureAlarm) {
		if (!WSingleton<CTHunter>::Instance()->IsUsedTikiReport()) {
			m_treasureAlarm->SetVisible(false);
			m_treasureAlarm->Enable(false);
			SetLeftOffset(m_treasureAlarm, m_screenWidth);
		}
	}
}

// taskmain.cpp:3978
bool CTaskMain::OnWaitDlgResult(int result, FrForm *) {
	if (result == 2) {
		WSingleton<CMouseCursor>::Instance()->SetActive(false);

		if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI))) {
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		}

		PostMsg(this, "Lobby", 0, (intptr_t)"LOGIN", 0, 0, 0);
	}

	m_waitDlg = NULL;
	return true;
}

// taskmain.cpp:4000
bool CTaskMain::OnServerDlgResult(int, FrForm *) {
	m_serverDlg = NULL;

	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false)) {
		if (WSingleton<CSharedDoc>::Instance()->m_logoutPending)
			GetActor("Lobby") << MsgObject(NULL, 0x56, 0, 0, 0, 0, 0);
	}

	return true;
}

// taskmain.cpp:4013
bool CTaskMain::OnQuitDlgResult(int, FrForm *) {
	m_quitDlg = NULL;
	return true;
}

// taskmain.cpp:4019
bool CTaskMain::OnOptionDlgResult(int, FrForm *) {
	m_optionDlg = NULL;
	return true;
}

// taskmain.cpp:4025
bool CTaskMain::OnNotifyDlgResult(int result, FrForm *) {
	if (result == 1) {
		if (IsLocalContent((localContentType_t)0x17) && !CSharedDoc::Instance()->IsHaveHalloweenItem()) {
			// the Halloween shop: off to the character shop
			CMouseCursor::Instance()->SetActive(false);

			CTaskManager::Instance()->ChangeTask("CShopTask", "",
				((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) != false);
			CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"CHARSHOP", 0, 0, 0);
		}
	}

	CSharedDoc::Instance()->m_giftList2.clear();
	return true;
}

// taskmain.cpp:4043
bool CTaskMain::OnNotifyOneDlgResult(int, FrForm *) {
	m_notifyOneDlg = NULL;
	return true;
}

// taskmain.cpp:4049
bool CTaskMain::OnRecycleDlgResult(int, FrForm *) {
	m_recycleDlg = NULL;

	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		WSingleton<ntAvatarManager>::Instance()->SetActive(true);

	return true;
}

// taskmain.cpp:4058
bool CTaskMain::OnTikiMagicBoxDlgResult(int, FrForm *) {
	m_tikiMagicBoxDlg = NULL;

	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		WSingleton<ntAvatarManager>::Instance()->SetActive(true);

	return true;
}

// taskmain.cpp:4075
bool CTaskMain::OnBongdariDlgResult(int, FrForm *) {
	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		WSingleton<ntAvatarManager>::Instance()->SetActive(true);

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CRealMyRoomTask::m_RTTI) : false) {
		MsgObject msg(0, 0xE1, 0, 0, 0, 0, 0);
		IActor *room = AfxGetTask()->GetActor("RealMyRoom");
		if (room)
			room << msg;
	}
	return true;
}

// taskmain.cpp:4088
bool CTaskMain::OnScratchDlgResult(int, FrForm *) {
	m_scratchDlg = NULL;

	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		WSingleton<ntAvatarManager>::Instance()->SetActive(true);

	return true;
}

// taskmain.cpp:4097
bool CTaskMain::OnLoginRsDlgResult(int, FrForm *) {
	m_loginRsDlg = NULL;
	return true;
}

// taskmain.cpp:4103
bool CTaskMain::OnMaketListDlgResult(int, FrForm *) {
	m_maketListDlg = NULL;
	return true;
}

// taskmain.cpp:4118
bool CTaskMain::OnOnelineReqDlgResult(int result, FrForm *pForm) {
	if (result == 1) {
		char *text = (char *)((FrOnelineReqDlg *)pForm)->GetInputString();

		if (WSingleton<CSharedDoc>::Instance()->m_chatManager.Filtering(text)) {
			// "contains an inappropriate word" (CP949)
			FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			notify->SetMessage("\xC0\xFB\xB4\xE7\xC7\xCF\xC1\xF6\xBE\xCA\xC0\xBA \xB4\xDC\xBE\xEE\xB0\xA1 \xC6\xF7\xC7\xD4\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			notify->Open(0, 3);
			m_onelineReqDlg = NULL;
			return true;
		}

		if (*text == '\0') {
			// "there is nothing to send" (CP949)
			FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			notify->SetMessage("\xBA\xB8\xB3\xBE \xB3\xBB\xBF\xEB\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			notify->Open(0, 3);
			m_onelineReqDlg = NULL;
			return true;
		}

		WSingleton<COneLineBoard>::Instance()->SetMessage(text);

		WSendPacket packet((enumClientPacket)0x67);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_onelineReqDlg = NULL;

	return true;
}

// taskmain.cpp:4247
bool CTaskMain::OnKbdManualDlgResult(int, FrForm *) {
	m_kbdManualDlg = NULL;
	return true;
}

// taskmain.cpp:4257
bool CTaskMain::OnLevelupFormResult(int, FrForm *) {
	if (!m_levelupForms.empty()) {
		m_levelupForms[m_levelupForms.size() - 1] = NULL;
		m_levelupForms.erase(m_levelupForms.end() - 1);
	}

	return true;
}

// taskmain.cpp:3364
void CTaskMain::OnOverBar_SiteInit(intptr_t param) {
	m_siteWnd = DYNAMIC_CAST(FrStatic, param);
	if (m_siteWnd) {
		UpdateSiteInfo();

		WRect rect = m_siteWnd->GetRect();
		rect.x += m_uiOffsetX;
		m_siteWnd->SetRect(rect);
	}
}

// taskmain.cpp:3374
void CTaskMain::OnOverBar_OneLineReqBtnInit(intptr_t param) {
	m_oneLineReqBtn = DYNAMIC_CAST(FrButton, param);
	if (m_oneLineReqBtn) {
		const WRect &cur = m_oneLineReqBtn->GetRect();
		if (m_screenWidth < 800.0f)
			m_oneLineReqBtn->SetRect(WRect(cur.x - 140.0f, cur.y, cur.w, cur.h));
		else
			m_oneLineReqBtn->SetRect(WRect(cur.x + m_uiOffsetX, cur.y, cur.w, cur.h));
	}
}

// taskmain.cpp:3442
void CTaskMain::OnOverBar_OneLineViewInit(intptr_t param) {
	m_oneLineView = DYNAMIC_CAST(FrArea, param);
	if (m_oneLineView) {
		WRect rect = m_oneLineView->GetRect();
		if (m_screenWidth < 800.0f)
			rect.x -= 140.0f;
		else
			rect.x += m_uiOffsetX;
		m_oneLineView->SetRect(rect);

		COneLineBoard::Instance()->SetRect(m_oneLineView->GetRect());
	}
}

// taskmain.cpp:3458
void CTaskMain::OnOverBar_OneLineViewOwnerDraw(int) {
	COneLineBoard::Instance()->Display();
}

// taskmain.cpp:3473
void CTaskMain::OnOverBar_OptionUp(void) {
	CloseAllDialogs();

	m_optionDlg = CreateForm<FrOptionDlg>(g_pFresh->GetManager(), this, "option", 0);
	m_optionDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnOptionDlgResult, 0x11);
	m_optionDlg->SetIconRect(m_barButton[0x0D]->GetRect());
	m_optionDlg->SetApplyScrSize(((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) != 0);

	OnBarHoverOff(0);
}

// taskmain.cpp:3495
void CTaskMain::OnOverBar_MyInfoUp(void) {
	CloseAllDialogs();
	WSingleton<CUserInfo>::Instance()->SetInfo(WSingleton<CSharedDoc>::Instance()->m_myUID,
		WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid, true, true, false, false,
		WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname);

	OnBarHoverOff(0);
}

// taskmain.cpp:3503
void CTaskMain::OnOverBar_PangCookieInit(intptr_t param) {
	m_pangCookieWnd = DYNAMIC_CAST(FrArea, param);
	if (m_pangCookieWnd) {
		WRect rect = m_pangCookieWnd->GetRect();
		rect.x = m_screenWidth - (800.0f - rect.x);
		m_pangCookieWnd->SetRect(rect);

		m_treeCashBitmap = m_treeFrameBitmap = NULL;
		m_treeCashBitmap = g_pFresh->GetBitmap("pang_bar_treecash");
		m_treeFrameBitmap = g_pFresh->GetBitmap("pang_bar_frame_tree");
		m_hanCoinBitmap = g_pFresh->GetBitmap("pang_bar_hancoin");
		m_hanFrameBitmap = g_pFresh->GetBitmap("pang_bar_frame_han");
	}
}

// taskmain.cpp:3520
void CTaskMain::OnOverBar_PangCookieOwnerDraw(int) {
	if (WSingleton<CProjectG>::Instance()->HidePrivacy())
		return;

	char text[32] = { 0 };
	const WRect &rect = m_pangCookieWnd->GetRect();

	MakePriceStrA(text, WSingleton<CSharedDoc>::Instance()->m_myPang);
	Print(rect.x + 113.0f, rect.y + 6.0f, text, 2.0f, 2, 1.0f, 0xFFFFD18E);

	__int64 cookie = S5::ExchangeOwnCashToCookie(WSingleton<CSharedDoc>::Instance()->m_outsideCash, 0, 100);
	if (cookie < 0)
		cookie = 0;
	MakePriceStrA(text, cookie);
	if (WSingleton<CSharedDoc>::Instance()->m_myBonusCash != 0) {
		char bonus[32] = { 0 };
		strcat(text, "/");

		MakePriceStrA(bonus, WSingleton<CSharedDoc>::Instance()->m_myBonusCash);
		strcat(text, bonus);
	}

	Print(rect.x + 218.0f, rect.y + 6.0f, text, 2.0f, 2, 1.0f, 0xFFFFD18E);

	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	const Bitmap *cashBitmap;
	const Bitmap *frameBitmap;
	switch (CLoginInfo::Instance()->ProvType()) {
	case 2:
		cashBitmap = m_treeCashBitmap;
		frameBitmap = m_treeFrameBitmap;
		break;
	case 4:
		cashBitmap = m_hanCoinBitmap;
		frameBitmap = m_hanFrameBitmap;
		break;
	default:
		return;
	}

	if (cashBitmap == NULL || frameBitmap == NULL)
		return;

	float width = (float)cashBitmap->Width();
	WRect cashRect(rect.x + rect.w - (width + 5.0f), rect.y + 3.0f, width, (float)cashBitmap->Height());
	gdi->DrawTexture(cashBitmap, cashRect, 0xFFFFFFFF, 0);

	cashRect.x -= 100.0f;
	cashRect.w += 150.0f;
	if (!WSingleton<CMouseCursor>::Instance()->InArea(cashRect))
		return;

	gdi->DrawTexture(frameBitmap, WRect(cashRect.x - 75.0f, cashRect.y + 10.0f, (float)frameBitmap->Width(), (float)frameBitmap->Height()), 0xFFFFFFFF, 0);
	MakePriceStrA(text, WSingleton<CSharedDoc>::Instance()->m_outsideCash);
	Print(cashRect.x + 75.0f, cashRect.y + 14.0f, text, 2.0f, 2, 1.0f, 0xFFFFD18E);
}

// taskmain.cpp:3710
void CTaskMain::OnBarHoverOn(intptr_t param) {
	m_hoverButton = DYNAMIC_CAST(FrButton, param);
	if (m_hoverButton) {
		int i;

		for (i = 0; i < 0x15; i++) {
			if (m_hoverButton == m_barButton[i]) {
				CIconToolTip::GetInstance()->SetToolTip((eBarButton)i);
				break;
			}
		}
	}
}

// taskmain.cpp:3726
void CTaskMain::OnBarHoverOff(intptr_t param) {
	FrButton *button = DYNAMIC_CAST(FrButton, (FrWnd *)param);

	if (button == NULL || button == m_hoverButton) {
		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
	}
}

// taskmain.cpp:3737
void CTaskMain::OnBarTooltipOwnerDraw(int) {
	TOOLTIP()->Render();
}

// taskmain.cpp:3742
void CTaskMain::OnOverBar_SpecialCardInit(intptr_t param) {
	m_barButton[10] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[10]) {
		m_barButton[10]->EnableHover(true, 0.0f);
		m_barButton[10]->EnableBlink(CCardManager::Instance()->ShowSPCardAlarm());

		WRect rect = m_barButton[10]->GetRect();
		rect.x = m_screenWidth - (800.0f - rect.x);
		m_barButton[10]->SetRect(rect);
	}
}

// taskmain.cpp:3755
void CTaskMain::OnOverBar_SpecialCardUp(void) {
	if (m_specialCardForm) {
		m_specialCardForm->Close(FrCANCEL, true);
	} else {
		m_barButton[10]->EnableBlink(false);
		CCardManager::Instance()->SetSPCardAlarm(false);

		m_specialCardForm = CreateForm<FrSPCardBuffDlg>(g_pFresh->GetManager(), this, "spcardbuff", 0);
		CCardManager::Instance()->SetCardBuffDlg(m_specialCardForm);
		CItemBuff::UpdateBuffDlg(m_specialCardForm);
		m_specialCardForm->SetVisible(false);
		m_specialCardForm->SetFixed(true);

		WPoint pos(m_specialCardForm->GetPosition().x, m_specialCardForm->GetPosition().y);
		pos.x = pos.x * g_view->GetWidth() * 0.00125f;
		pos.y = pos.y * g_view->GetHeight() * (1.0f / 600.0f);
		m_specialCardForm->EnableDrag(false);
		m_specialCardForm->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnSpecialCardFormResult, pos, 1);

		if (m_specialCardForm->GetSpecialCardNum() > 0) {
			m_specialCardForm->SetVisible(true);
		} else {
			m_specialCardForm->Close(FrCANCEL, true);

			WRect rect;
			rect.x = m_barButton[10]->GetRect().x - 86.0f;
			rect.y = m_messengerBubbleEdit->GetRect().y;
			MsgObject msg(NULL, 0xA8, (intptr_t)"\xbb\xe7\xbf\xeb\xc1\xdf\xc0\xce\x20\xc8\xbf\xb0\xfa\xb0\xa1\x20\xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9\x2e", 1, (intptr_t)&rect, 0, 0);
			AfxGetTask()->PassMsgToMainActor(msg);
		}
	}

	m_hoverButton = NULL;
	CIconToolTip::GetInstance()->ClearToolTip();
}

// taskmain.cpp:3801
void CTaskMain::OnOverBar_MessengerInit(intptr_t param) {
	m_barButton[11] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[11]) {
		m_barButton[11]->EnableHover(true, 0.0f);

		WSingleton<CMessengerInfo>::Instance()->m_rcAlarmBtn = m_barButton[11]->GetRect();
		if (!WSingleton<CSharedDoc>::Instance()->m_value4DBE)
			m_barButton[11]->EnableBlink(false);

		WRect rect = m_barButton[11]->GetRect();
		rect.x = m_screenWidth - (800.0f - rect.x);
		m_barButton[11]->SetRect(rect);
	}
}

// taskmain.cpp:3818
void CTaskMain::OnOverBar_MessengerUp(void) {
	CloseAllDialogs();

	MESSENGER()->Open(true);

	OnBarHoverOff(0);
}

// taskmain.cpp:3842
void CTaskMain::OnTreasureAlarmUp(void) {
	m_treasureAlarm->SetVisible(false);
	m_treasureAlarm->Enable(false);

	WSingleton<CTHunter>::Instance()->SetUsedTikiReport(false);

	CTreasureAlarmDlg *dlg = CreateForm<CTreasureAlarmDlg>(g_pFresh->GetManager(), this, "treasure_alarm", 0);
	dlg->Open((FRESH_RESULT_CALLBACK)0, 0x11);
	dlg->SetTopmost(true);
	dlg->FindNextTopFocus(false);
	dlg->SetRect(dlg->GetRect());

	OnBarHoverOff(0);
}

// taskmain.cpp:3858
void CTaskMain::OnMessengerBubbleInit(intptr_t param) {
	m_messengerBubble = DYNAMIC_CAST(FrArea, param);
	WRect rect = m_messengerBubble->GetRect();
	rect.x = m_screenWidth - (800.0f - rect.x);
	m_messengerBubble->SetRect(rect);
	for (int i = 0; i < 3; i++)
		m_bubbleBitmap[i] = g_pFresh->GetBitmap(MakeStr("login_%02d", i));
}

// taskmain.cpp:3866
void CTaskMain::OnMessengerBubbleOwnerDraw(int) {
	if (m_messengerBubbleEdit == NULL)
		return;

	if (!m_messengerBubbleEdit->IsVisible())
		return;

	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == NULL)
		return;

	FrEdit *edit = static_cast<FrEdit *>(m_messengerBubbleEdit);
	if (edit->GetLineNum() == 0)
		return;

	const WRect &rect = m_messengerBubbleEdit->GetRect();

	float x = rect.x - 2.0f;
	float y = rect.y - 8.0f;
	float height = edit->GetLineNum() * 14.0f;

	if (m_bubbleBitmap[0]) {
		gdi->DrawTexture(m_bubbleBitmap[0], WRect(x, y, (float)m_bubbleBitmap[0]->Width(), (float)m_bubbleBitmap[0]->Height()), 0xFFFFFFFF, 0);
		y += (float)m_bubbleBitmap[0]->Height();
	}

	if (m_bubbleBitmap[1]) {
		gdi->DrawTexture(m_bubbleBitmap[1], WRect(x, y, (float)m_bubbleBitmap[1]->Width(), height), 0xFFFFFFFF, 0);
		y += height;
	}

	if (m_bubbleBitmap[2])
		gdi->DrawTexture(m_bubbleBitmap[2], WRect(x, y, (float)m_bubbleBitmap[2]->Width(), (float)m_bubbleBitmap[2]->Height()), 0xFFFFFFFF, 0);
}

// taskmain.cpp:3911
void CTaskMain::OnMessengerBubbleEditInit(intptr_t param) {
	m_messengerBubbleEdit = DYNAMIC_CAST(FrEdit, param);
	m_messengerBubbleEdit->SetVisible(false);

	SetLeftOffset(m_messengerBubbleEdit, m_screenWidth);
}

// taskmain.cpp:3919
void CTaskMain::OnMessengerAlarmBtnInit(intptr_t param) {
	m_messengerAlarmBtn = DYNAMIC_CAST(FrButton, param);
	WRect rect = m_messengerAlarmBtn->GetRect();
	rect.x = m_screenWidth - (800.0f - rect.x);
	m_messengerAlarmBtn->SetRect(rect);
	if (m_messengerAlarmBtn && !WSingleton<CSharedDoc>::Instance()->m_value4DBD)
		m_messengerAlarmBtn->SetVisible(false);
}

// taskmain.cpp:3927
void CTaskMain::OnMessengerAlarmBtnUp(void) {
	MESSENGER()->AlarmBtnUp(0xFFFFFFFF);
}

// taskmain.cpp:3932
void CTaskMain::OnMessengerGiftAlarmBtnInit(intptr_t param) {
	m_messengerGiftAlarmBtn = DYNAMIC_CAST(FrButton, param);
	m_messengerGiftAlarmBtn->SetVisible(false);

	SetLeftOffset(m_messengerGiftAlarmBtn, m_screenWidth);
}

// taskmain.cpp:3939
void CTaskMain::OnMessengerGiftAlarmBtnUp(void) {
	m_messengerGiftAlarmBtn->SetVisible(false);

	FrGiftAlarmDlg *dlg = CreateForm<FrGiftAlarmDlg>(g_pFresh->GetManager(), this, "gift_alarm", 0);
	dlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnNotifyDlgResult, 0x11);
	dlg->SetTopmost(true);
	dlg->FindNextTopFocus(false);
	dlg->SetIconRect(m_messengerGiftAlarmBtn->GetRect());

	OnBarHoverOff(0);
}

// taskmain.cpp:3952
void CTaskMain::CloseAllDialogs(void) {
	if (m_noticeDlg)
		m_noticeDlg->Close(true);

	if (m_popupDlg)
		m_popupDlg->Close(true);

	if (!m_levelupForms.empty()) {
		int count = m_levelupForms.size();
		for (int i = 0; i < count; i++) {
			if (m_levelupForms[i]) {
				m_levelupForms[i]->Close(FrNONE, true);
				m_levelupForms[i] = NULL;
			}
		}

		m_levelupForms.clear();
	}
}

// taskmain.cpp:4268
bool CTaskMain::OnOnelineReqChkOutDlgResult(int result, FrForm *) {
	if (result == 1) {
		WSendPacket packet((enumClientPacket)0x66);
		packet.EncodeStr(WSingleton<COneLineBoard>::Instance()->GetMessageA());
		packet.Send(SEND_TO_DEFAULT);
	}

	return true;
}

// taskmain.cpp:4279
bool CTaskMain::OnSpecialCardFormResult(int, FrForm *) {
	CCardManager::Instance()->SetCardBuffDlg(NULL);
	m_specialCardForm = NULL;

	return true;
}

// taskmain.cpp:4287
float CTaskMain::Print(float x, float y, const char *text, float space, int align, float scale, unsigned long color) {
	float startX = x;

	if (text == NULL)
		return 0.0f;

	int len = strlen(text);
	if (len == 0)
		return 0.0f;

	WTitleFont *font = m_titleFont;

	float oldScale = font->GetScale();
	font->SetScale(scale);
	const char *charSet = font->GetFontInfo()->pCharSet;

	float width = 0.0f;
	const char *p;
	switch (align) {
	case 1:
		p = text;
		while (*p) {
			char c = *p++;

			int index = strchr(charSet, c) - charSet;

			width += font->GetCharWidthA(g_view, index) + space * scale;
		}
		width -= space;
		width *= 0.5f;
		break;
	case 2:
		p = text;
		while (*p) {
			char c = *p++;

			int index = strchr(charSet, c) - charSet;

			width += font->GetCharWidthA(g_view, index) + space * scale;
		}
		width -= space;
		break;
	}

	x -= width;

	p = text;
	while (*p) {
		char c = *p++;

		int index = strchr(charSet, c) - charSet;

		x += font->PutChar(g_view, x, y, index, 0, color) + space;
	}

	m_titleFont->SetScale(oldScale);
	m_titleFont->Flush(g_view);

	switch (align) {
	case 0:
		return x - startX;
	case 1:
	case 2:
		return width;
	}

	return startX;
}

// taskmain.cpp:4378
const char *CTaskMain::MakeStatusString(int slot, int value, int slotValue) {
	// power, control, accuracy, spin and curve (CP949)
	const char *names[5] = {
		"\xc6\xc4\xbf\xf6",
		"\xc4\xc1\xc6\xae\xb7\xd1",
		"\xc1\xa4\xc8\xae\xb5\xb5",
		"\xbd\xba\xc7\xc9",
		"\xc4\xbf\xba\xea",
	};

	if (value && slotValue)
		return MakeStr("%s+%d/%s+%d", names[slot], value, "\xbd\xbd\xb7\xd4", slotValue);
	else if (value)
		return MakeStr("%s+%d", names[slot], value);
	else if (slotValue)
		return MakeStr("%s%s+%d", names[slot], "\xbd\xbd\xb7\xd4", slotValue);

	return "";
}

// taskmain.cpp:4405
const char *CTaskMain::MakeStatusString2(int current, int total, int penalty) {
	if (current && total && penalty)
		return MakeStr("%d(-%d)/%d", current, penalty, total);
	else if (current && total)
		return MakeStr("%d/%d", current, total);
	else if (current)
		return MakeStr("%d", current);
	else if (total)
		return MakeStr("%d", total);

	return "";
}

// taskmain.cpp:4422
const char *CTaskMain::MakeStatusString3(int current, int total, int penalty) {
	if (penalty) {
		return MakeStr("%d(-%d)/%d", current, penalty, total);
	}

	return MakeStr("%d/%d", current, total);
}

// taskmain.cpp:4432
const char *CTaskMain::MakeCardStatusString(int slot, int value) {
	// power, control, accuracy, spin and curve slot (CP949)
	const char *slotNames[5] = {
		"ÆÄ¿ö½½·Ô",
		"ÄÁÆ®·Ñ½½·Ô",
		"Á¤È®µµ½½·Ô",
		"½ºÇÉ½½·Ô",
		"Ä¿ºê½½·Ô",
	};

	if (value)
		return MakeStr("%s+%d", slotNames[slot], value);
	return "";
}

// taskmain.cpp:4485
void CTaskMain::SetBGMNone(void) {
	if (data_af1a08 != 0) {
		g_audio->StopBGM(true);

		data_af1a08 = 0;
	}
}

// taskmain.cpp:4495
void CTaskMain::SetRandomBGM(void) {
	if (data_af1a08 != 1) {
		g_audio->m_fadeOutTime = 1.0f;

		g_audio->PlayBGM(randomBGMList[rand() % 2], false, true);
		data_af1a08 = 1;
	}
}

// taskmain.cpp:4506
void CTaskMain::SetTimeVariableBGM(void) {
	if (data_af1a08 != 2) {
		SYSTEMTIME time;
		GetLocalTime(&time);

		g_audio->m_fadeOutTime = 1.0f;

		if (time.wHour == 22 || time.wHour == 23)
			g_audio->PlayBGM("tea_time_vibra.mp3", false, true);
		else
			g_audio->PlayBGM("tea_time_piano.mp3", false, true);

		data_af1a08 = 2;
	}
}

// taskmain.cpp:4524
void CTaskMain::SetConstantBGM(const char* name, bool bOnce, bool bForce) {
	if (data_af1a08 != 3 || bForce) {
		g_audio->m_fadeOutTime = 1.0f;
		g_audio->PlayBGM(name, bOnce, true);
		data_af1a08 = 3;
	}
}

// taskmain.cpp:4534
void CTaskMain::TurnOn3DBackground(void) {
	WSingleton<CSharedDoc>::Instance()->m_weather = 0;

	if (WSingleton<CSceneManager>::Instance())
		WSingleton<CSceneManager>::Instance()->SetLobbyWeather(false);
	WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff = false;
}

// taskmain.cpp:4544
void CTaskMain::TurnOff3DBackground(void) {
	WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff = true;
}

// taskmain.cpp:4549
void CTaskMain::CheckNaviButtons(void) {
	if (m_barButton[BAR_BUTTON_NAVI_BACKWARD])
		m_barButton[BAR_BUTTON_NAVI_BACKWARD]->Enable(CTaskManager::Instance()->IsNaviBackwardEnable());

	if (m_barButton[BAR_BUTTON_NAVI_FORWARD])
		m_barButton[BAR_BUTTON_NAVI_FORWARD]->Enable(CTaskManager::Instance()->IsNaviForwardEnable());
}

// taskmain.cpp:4561
void CTaskMain::HandleMsgCommon(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x270:
		CInitDelegate::ExecuteInitFunctions();
		break;

	case 0x271:
		CInitDelegate::ExecuteReleaseFunctions();
		break;

	case 0x26B: {
		void *pMsg = (void *)msg.m_param1;
		AfxGetTask()->PushMsg(pMsg);
		break;
	}

	case 1:
		m_waitDlgTime = 0.0f;
		break;

	case 2:
		m_waitDlgTime = -30.0f;
		if (m_waitDlg) {
			m_waitDlg->Close((eFormRet)1, true);
			m_waitDlg = NULL;
		}
		break;

	case 0xEC:
		OpenWaitedLayout();
		break;

	case 0x19:
		if (m_serverDlg)
			m_serverDlg->BuildServerList();
		break;

	case 0x1B:
		if (m_serverDlg)
			m_serverDlg->BuildChannelList();
		break;

	case 0x23: {
		FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		notify->SetMessage((const char *)msg.m_param1, false);
		notify->Open(0, 3);
		notify->SetTopmost(true);
		notify->FindNextTopFocus(false);
		break;
	}

	case 0x24:
		if (m_notifyOneDlg == NULL) {
			m_notifyOneDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			m_notifyOneDlg->SetMessage((const char *)msg.m_param1, false);
			m_notifyOneDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnNotifyOneDlgResult, 3);
		}
		break;

	case 0x26: {
		// param1: the message, param2: the item's type id, or a pang amount
		// when no item has that id.
		unsigned long typeId = (unsigned long)msg.m_param2;
		IFF_ITEM_COMMON *item = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindCommonItem(typeId);
		FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_item", 0);
		if (notify == NULL)
			break;

		FrArea *icon = DYNAMIC_CAST(FrArea, notify->FindChildByName("itemicon"));
		FrEdit *name = DYNAMIC_CAST(FrEdit, notify->FindChildByName("itemname"));

		if (icon) {
			if (item) {
				icon->SetBgImg(item->m_icon);
			} else if (typeId) {
				WRect rect(icon->GetRect());
				icon->SetRect(WRect(rect.x + 15.0f, rect.y + 7.0f, rect.w, rect.h));
				icon->SetBgImg("item1_16");
			} else {
				icon->SetBgImg("hide");
			}
		}

		if (name) {
			if (item)
				name->AddText(item->m_name, false, true);
			else if (typeId)
				name->AddText(MakeStr("%d\xC6\xCE", typeId), false, true);   // "%d pang" (CP949)
			else
				name->AddText("none", false, true);

			name->AddText("\n", false, true);
		}

		notify->SetMessage((const char *)msg.m_param1, false);
		notify->Open(0, 2);
		break;
	}

	case 0x48:
		if (m_giftDlg)
			m_giftDlg->UpdateFriendList();
		break;

	case 0x50: {
		// param1: the receiver's uid, param2: his user info (the uid read at
		// +0x109 lies past sPangYaUserInfo: the record is a larger one).
		if (m_giftDlg)
			m_giftDlg->SetReceiverInfo((unsigned long)msg.m_param1, *(const sPangYaUserInfo *)msg.m_param2);

		if (CUserInfo::Instance()->IsVisible() == true) {
			FrUserInfoForm *dlg = USERINFODLG();
			if (dlg && dlg->m_addFriendDlg) {
				const char *info = (const char *)msg.m_param2;
				dlg->SetRequestFriendData(((const sPangYaUserInfo *)info)->nickname, *(const unsigned long *)(info + 0x109));
			}
		}
		break;
	}

	case 0x51: {
		FrUserInfoForm *dlg = USERINFODLG();
		if (dlg) {
			if (CUserInfo::Instance()->IsVisible() == true) {
				if (dlg->m_addFriendDlg)
					dlg->SetRequestFriendData((const char *)msg.m_param1, (unsigned long)msg.m_param2);
			}
		}
	}
	// fall through: the caddie info dialog ends its contract too
	case 0x5C: {
		FrCaddieInfoDlg *dlg = FrStaticCast<FrCaddieInfoDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("caddie_info"));
		if (dlg == NULL)
			break;

		dlg->EndContract(msg.m_param1);
		break;
	}

	case 0xA8:
		// The messenger bubble: param1 the text, param2 show/hide, param3
		// an optional position (a WPoint).
		if (m_messengerBubbleEdit == NULL)
			break;

		if (msg.m_param2) {
			const char *text = (const char *)msg.m_param1;
			if (msg.m_param3) {
				MESSENGER()->Notify2(text);

				WPoint pt = *(WPoint *)msg.m_param3;
				m_messengerBubbleEdit->MoveWindow(pt);
				break;
			}

			WPoint pt;
			pt.x = MESSENGER()->m_rcAlarmBtn.x - 86.0f;
			pt.y = m_messengerBubbleEdit->GetRect().y;
			m_messengerBubbleEdit->MoveWindow(pt);

			((FrEdit *)m_messengerBubbleEdit)->ClearLine();
			((FrEdit *)m_messengerBubbleEdit)->AddText(text, false, true);
			m_messengerBubbleEdit->SetVisible(true);
			break;
		}

		m_messengerBubbleEdit->SetVisible(false);
		break;

	case 0xAE:
		MESSENGER()->Close(true);
		break;

	case 0xA9:
		if (m_messengerAlarmBtn == NULL)
			break;

		if (msg.m_param1)
			m_messengerAlarmBtn->SetVisible(true);
		else
			m_messengerAlarmBtn->SetVisible(false);
		break;

	case 0xAC:
		if (m_barButton[11] == NULL)
			break;

		if (msg.m_param1)
			m_barButton[11]->EnableBlink(true);
		else
			m_barButton[11]->EnableBlink(false);
		break;

	case 0xAA:
		if (m_messengerGiftAlarmBtn == NULL)
			break;

		if (msg.m_param1) {
			if (IsLocalContent((localContentType_t)0x38) && CTaskManager::Instance()
				&& ((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&CTutorialTask::m_RTTI) : false))
				break;

			m_messengerGiftAlarmBtn->SetVisible(true);
			break;
		}

		m_messengerGiftAlarmBtn->SetVisible(false);
		break;

	case 0x6C:
		if (m_treasureAlarm == NULL)
			break;

		m_treasureAlarm->SetVisible(msg.m_param1 == 1);
		if (msg.m_param1 == 1) {
			m_treasureAlarm->Enable(true);
			m_treasureAlarm->SetVisible(true);
		} else {
			m_treasureAlarm->SetVisible(false);
			m_treasureAlarm->Enable(false);
		}
		break;

	case 0x1FA: {
		// Mark the equipped skins (type class 0x38000000) as kind 2 in the
		// skin list.
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		for (int i = 0; i < 6; i++) {
			unsigned long typeId = doc->m_equipSkin[i];
			if ((typeId & 0xFC000000) == 0x38000000) {
				for (std::list<sItemInfo>::iterator it = doc->m_mySkinList.begin(); it != doc->m_mySkinList.end(); ++it) {
					if (it->m_typeId == typeId) {
						it->m_stateBits.kind = 2;
						break;
					}
				}
			}
			doc = WSingleton<CSharedDoc>::Instance();
		}
		break;
	}

	case 0x1FB:
		OpenInformation((unsigned long)msg.m_param1);
		break;

	case 0xAD: {
		// An item was used (param1: its serial): take one off its count.
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		unsigned long serial = (unsigned long)msg.m_param1;
		unsigned long typeId = 0;
		std::list<sItemInfo>::iterator it;
		for (it = doc->m_itemInfoList.begin(); it != doc->m_itemInfoList.end(); ++it) {
			if (it->m_serial == serial) {
				typeId = it->m_typeId;
				break;
			}
		}

		if (it != doc->m_itemInfoList.end()) {
			if (it->m_stateBits.kind != 0) {
				if (doc->m_itemManager.GetCouponKind(it->m_typeId) == 3) {
					if (--it->m_count <= 0)
						WSingleton<CSharedDoc>::Instance()->m_itemInfoList.erase(it);
				} else if (WSingleton<CSharedDoc>::Instance()->m_itemManager.GetCouponKind(it->m_typeId) != 0) {
					WSingleton<CSharedDoc>::Instance()->m_itemInfoList.erase(it);
				} else if ((int)it->m_reserved08 <= 0) {
					WSingleton<CSharedDoc>::Instance()->m_itemInfoList.erase(it);
				}
			} else {
				if (--it->m_count <= 0)
					WSingleton<CSharedDoc>::Instance()->m_itemInfoList.erase(it);
			}
		}

		if (IsLocalContent((localContentType_t)1) && typeId
			&& WSingleton<CSharedDoc>::Instance()->m_itemManager.GetCouponKind(typeId) == 2) {
			FrBongdariShopDlg *shop = FrStaticCast<FrBongdariShopDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"));
			if (shop) {
				shop->InitCouponBongdariShop();
				shop->SetDataFee(1);
			}

			CTask *task = CTaskManager::Instance() ? CTaskManager::Instance()->GetCurrentTask() : NULL;
			IActor *room = task->GetActor("RealMyRoom");
			if (room)
				room << MsgObject(0, 0xE1, 0, 0, 0, 0, 0);
		}
		break;
	}

	case 0xAF:
		m_bongdariShop[2] = msg.m_param1 != 0;
		break;

	case 0x5D:
		WSingleton<COneLineBoard>::Instance()->SetWaitMsgNum(msg.m_param1);
		WSingleton<COneLineBoard>::Instance()->SetWaitMsgTime(msg.m_param2);

		if (msg.m_param1 >= 0x400) {
			// "Too many messages are booked. Please try again later." (CP949)
			this << MsgObject(this, 0x23, (intptr_t)"\xC7\xF6\xC0\xE7 \xBF\xB9\xBE\xE0\xB5\xC8 \xB8\xDE\xBD\xC3\xC1\xF6\xB0\xA1 \xB3\xCA\xB9\xAB \xB8\xB9\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xC0\xE1\xBD\xC3\xC8\xC4\xBF\xA1 \xB4\xD9\xBD\xC3 \xC7\xD8\xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0.", 0, 0, 0, 0);
		} else {
			FrOnelineReqChkOutDlg *dlg = CreateForm<FrOnelineReqChkOutDlg>(g_pFresh->GetManager(), this, "oneline_chkout", 0);
			if (dlg)
				dlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnOnelineReqChkOutDlgResult, 0x11);
		}
		break;

	case 0xEA:
		OnUnderBar_ServerUp();
		break;

	case 0xEB:
		UpdateSiteInfo();
		break;

	case 0x1C:
		// The channel was entered.
		this << MsgObject(0, 2, 0, 0, 0, 0, 0);

		WSingleton<CSharedDoc>::Instance()->m_currentChannel = WSingleton<CSharedDoc>::Instance()->m_selectedChannel;
		WSingleton<CSharedDoc>::Instance()->m_ignoreList.clear();
		UpdateSiteInfo();

		if (g_pFresh->IsCurrentLayout("SERVERLIST")) {
			CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			CTaskManager::Instance()->PostMsg(0, "Lobby", 0x35, 0, 0, 0, 0);
		}
		break;

	case 0x20:
		if (m_serverDlg) {
			m_serverDlg->Close((eFormRet)1, true);
			m_serverDlg = NULL;
		}
		break;

	case 0x22:
		if (m_serverDlg)
			m_serverDlg->SetLoginMessageAlert((const char *)msg.m_param1);
		break;

	case 0x200: {
		CloseAllDialogs();

		FrBongdariShopDlg *shop = CreateForm<FrBongdariShopDlg>(g_pFresh->GetManager(), this, "bongdari_shop", 0);
		if (shop == NULL)
			break;

		shop->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnBongdariDlgResult, 0x41);
		if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
			WSingleton<ntAvatarManager>::Instance()->SetActive(false);
		break;
	}

	case 0x12D: {
		FrLevelupItemForm *form = CreateForm<FrLevelupItemForm>(g_pFresh->GetManager(), this, "levelupitem", 0);
		if (form == NULL)
			break;

		form->SetData(*(const sLevelUpDone *)msg.m_param1);
		form->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnLevelupFormResult, 3);
		m_levelupForms.push_back(form);
		break;
	}

	case 0x216: {
		// Move straight to a room on another game server (following a
		// friend, or a room picked from a list).
		if (WSingleton<CSharedDoc>::Instance()->m_nowGoWithMode == 1) {
			int friendUID = WSingleton<CSharedDoc>::Instance()->m_goWithFriendUID;
			if (friendUID == -1)
				break;

			sFriend *buddy = BuddyMGR->GetBuddy(friendUID);
			if (buddy == NULL)
				break;

			WSingleton<CSharedDoc>::Instance()->SetDirectMoveRoomInfo(buddy->extra.index, buddy->extra.code, buddy->extra.id2);
		}

		sGameServerInfo server;
		server.name[0] = '\0';
		server.m_address[0] = '\0';

		unsigned char serverIndex = 0xFF;
		unsigned short room = 0xFFFF;
		unsigned long serverId = 0xFFFFFFFF;
		WSingleton<CSharedDoc>::Instance()->GetDirectMoveRoomInfo(&serverIndex, &room, &serverId);

		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		for (std::list<sGameServerInfo>::iterator it = doc->m_gameServerList.begin(); it != doc->m_gameServerList.end(); ++it) {
			server = *it;
			if (server.id == serverId)
				break;
		}

		if (server.m_maxUsers == 0)
			break;

		int mode = doc->m_nowGoWithMode;
		if (mode == 1) {
			if (serverIndex == 0xFF)
				break;
		} else if (mode == 2 || mode == 3) {
			if (serverIndex == 0xFF || room == 0xFFFF)
				break;
		}

		doc->m_gameServerAddress = server.m_address;
		WSingleton<CSharedDoc>::Instance()->m_gameServerPort = server.m_port;
		WSingleton<CSharedDoc>::Instance()->m_value84 = server.id;

		if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
			WSingleton<WNetworkSystem>::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_DEFAULT);

		WSingleton<WNetworkSystem>::Instance()->SendMessage(WNetworkSystem::NET_UNIT_DEFAULT, NetworkUnit::EVENT_NEXT);

		// m_serverName opens the connected server's whole sGameServerInfo record.
		memcpy(WSingleton<CSharedDoc>::Instance()->m_serverName, &server, sizeof(sGameServerInfo));
		WSingleton<CSharedDoc>::Instance()->SetDirectMoveRoomInfo(serverIndex, room, serverId);
		break;
	}

	case 0x222:
		if (IsLocalContent((localContentType_t)0x56))
			MakeRealMyRoom();
		break;

	case 0x21F:
		if (!IsLocalContent((localContentType_t)0x56))
			break;

		if (WSingleton<CSharedDoc>::Instance()->m_realMyRoomUID != WSingleton<CSharedDoc>::Instance()->m_myUID
			&& WSingleton<CSharedDoc>::Instance()->m_authSetting) {
			int result = 0;
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x231, (intptr_t)&result, 2, 3, 0, 0));

			if (result == 1) {
				CloseAllDialogs();
				if (m_rmrSettingDlg)
					break;

				m_rmrSettingDlg = CreateForm<CRealMyRoom_EnterSetting>(g_pFresh->GetManager(), this, "enter_setting", 0);
				FrForm::ms_bHasTail = false;
				m_rmrSettingDlg->Open((FRESH_RESULT_CALLBACK)&CTaskMain::OnRMR_ResultSetting, 0x81);
				m_rmrSettingDlg->SetFadeout(false);
				break;
			}

			// "The user is in a waiting room: you cannot visit another user's room." (CP949)
			this << MsgObject(this, 0x23, (intptr_t)"\xC7\xF6\xC0\xE7 \xC0\xAF\xC0\xFA\xB0\xA1 \xB4\xEB\xB1\xE2\xB9\xE6\xBF\xA1 \xC0\xD6\xBE\xEE \xB4\xD9\xB8\xA5 \xC0\xAF\xC0\xFA\xC0\xC7 \xB9\xE6\xBF\xA1 \xB9\xE6\xB9\xAE\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
			break;
		}

		ChangeRealMyRoom();
		break;

	case 0x228: {
		// A My Room / mail result (param1: the packet, param2: its result).
		int packet = msg.m_param1;
		FrForm *notify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

		switch (packet) {
		case 0x122:
			if (msg.m_param2)
				notify->SetMessage("\xB8\xB6\xC0\xCC\xB7\xEB\xC0\xC7 \xB1\xC7\xC7\xD1\xC0\xBB \xBC\xB3\xC1\xA4\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else
				notify->SetMessage("\xB8\xB6\xC0\xCC\xB7\xEB\xC0\xC7 \xB1\xC7\xC7\xD1\xC0\xBB \xBC\xB3\xC1\xA4\xC7\xCF\xB4\xC2\xB5\xA5 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x124:
			if (msg.m_param2)
				notify->SetMessage("\xC7\xF6\xC0\xE7 \xB9\xE8\xC4\xA1\xB8\xA6 \xC0\xFA\xC0\xE5\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else
				notify->SetMessage("\xB8\xB6\xC0\xCC\xB7\xEB\xBF\xA1 \xB0\xA1\xB1\xB8\xB8\xA6 \xB3\xF5\xC1\xF6 \xB8\xF8\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.. ", false);
			break;

		case 0x125:
			notify->SetMessage("\xB8\xB6\xC0\xCC\xB7\xEB\xBF\xA1 \xC0\xD6\xB4\xC2 \xB0\xA1\xB1\xB8 \xC1\xA4\xBA\xB8\xB8\xA6 \xB0\xA1\xC1\xAE\xBF\xC0\xC1\xF6 \xB8\xF8\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x123:
			notify->SetMessage("\xB8\xB6\xC0\xCC\xB7\xEB\xBF\xA1 \xB5\xE9\xBE\xEE\xB0\xA5 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x141:
			notify->SetMessage("\xBF\xEC\xC6\xED\xC7\xD4\xC0\xC7 \xC6\xED\xC1\xF6\xB5\xE9\xC0\xBB \xC0\xD0\xBE\xEE\xBF\xC0\xC1\xF6 \xB8\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x143:
			notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xC7 \xB3\xBB\xBF\xEB\xC0\xBB \xC0\xD0\xBE\xEE\xBF\xC0\xC1\xF6 \xB8\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x145:
			notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xBF\xA1 \xC3\xB7\xBA\xCE\xB5\xC8 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBF\xC5\xB1\xE2\xC1\xF6 \xB8\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x15D:
			notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xC1\xF6\xBF\xEC\xC1\xF6 \xB8\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;

		case 0x13F: {
			// Sending mail failed; param2 says why.
			int reason = msg.m_param2;
			if (reason == 1)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xCC \xC6\xED\xC1\xF6 \xBC\xF6\xBD\xC5\xC0\xBB \xB0\xC5\xBA\xCE\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 2)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xC0\xAF\xC0\xFA\xB8\xA6 \xC3\xA3\xC0\xBB \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 3)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xCC \xC0\xCC\xB9\xCC \xBC\xD2\xC0\xAF\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xD4\xB4\xCF\xB4\xD9.", false);
			else if (reason == 4)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBC\xD2\xC0\xAF\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB \xBC\xF6\xB7\xAE\xBA\xB8\xB4\xD9 \xB4\xF5 \xB8\xB9\xC0\xBA \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBA\xB8\xB3\xBB\xB7\xC1\xB0\xED \xBD\xC3\xB5\xB5\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9..", false);
			else if (reason == 5)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBC\xD2\xC0\xAF\xC7\xCF\xC1\xF6 \xBE\xCA\xC0\xBA \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBA\xB8\xB3\xBB\xB7\xC1\xB0\xED \xBD\xC3\xB5\xB5\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 6)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xCC \xC0\xCC\xB9\xCC \xC6\xED\xC1\xF6\xB7\xCE \xB9\xDE\xC0\xBA \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xD4\xB4\xCF\xB4\xD9.", false);
			else if (reason == 7)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xC3\xB7\xBA\xCE\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 8)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xC6\xED\xC1\xF6 \xB9\xDF\xBC\xDB\xC0\xBB \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 9)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xC7 \xBF\xEC\xC6\xED\xC7\xD4\xC0\xCC \xB0\xA1\xB5\xE6 \xC2\xF7\xBC\xAD \xC7\xF6\xC0\xE7\xB4\xC2 \xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xB9\xDE\xC0\xBB \xBC\xF6 \xBE\xF8\xB4\xC2 \xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9.", false);
			else if (reason == 10)
				notify->SetMessage("\xBF\xEC\xC6\xED\xC7\xD4 \xB1\xE2\xB4\xC9\xC0\xC7 \xBA\xCE\xBA\xD0 \xC1\xA1\xB0\xCB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", false);
			else if (reason == 11)
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \x0A\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xC7 \xC4\xB3\xB5\xF0\xB0\xA1 \xC8\xDE\xB0\xA1\xC1\xDF\xC0\xCC\xB0\xC5\xB3\xAA, \xBA\xB8\xC0\xAF\xC7\xD1 \xC4\xB3\xB5\xF0\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else if (reason == 20)
				notify->SetMessage("\xC3\xB3\xB8\xAE\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9. \xC0\xE1\xBD\xC3\xB8\xB8 \xB1\xE2\xB4\xD9\xB8\xAE\xBC\xBC\xBF\xE4.", false);
			else if (reason == 22)
				notify->SetMessage("\xB8\xDE\xC0\xCF \xC3\xB7\xBA\xCE \xB0\xA1\xB4\xC9\xC7\xD1 \xC3\xD6\xB4\xEB \xB0\xB3\xBC\xF6\xB8\xA6 \xC3\xCA\xB0\xFA\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			else
				notify->SetMessage("\xBF\xEC\xC6\xED\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBB\xC1\xF6 \xB8\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		}
		}

		notify->Open(0, 0);
		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 2, 0, 0, 0, 0, 0);
		break;
	}

	case 0x231:
		// The bar button state query: param1 receives the state, param2
		// the frame, param3 the button.
		if (IsLocalContent((localContentType_t)0x56)) {
			unsigned char frame = (unsigned char)msg.m_param2;
			*(int *)msg.m_param1 = GetBtnState(frame, (eBarButton)msg.m_param3);
		} else {
			*(bool *)msg.m_param3 = false;
		}
		break;

	case 0x252: {
		// A birthday event mission was completed (param1 points to its id).
		if (!IsLocalContent((localContentType_t)0x8E))
			break;

		std::string skin("BirthdayEventMissionSkin1");
		if (IsLocalContent((localContentType_t)0x9B))
			skin = "BirthdayEventMissionSkin2";

		for (int i = 0; i < 5; i++) {
			if (m_completedMissionDlgs[i])
				continue;

			m_completedMissionDlgs[i] = CreateForm<FrBirthdayEventMissionDlg>(g_pFresh->GetManager(), this, skin.c_str(), 0);
			if (m_completedMissionDlgs[i] == NULL)
				continue;

			((FrBirthdayEventMissionDlg *)m_completedMissionDlgs[i])->SetCompletedMissionInfo(*(const unsigned long *)msg.m_param1);
			for (int j = 0; j <= i; j++) {
				if (m_completedMissionDlgs[j])
					((FrBirthdayEventMissionDlg *)m_completedMissionDlgs[j])->CalculatePos(j, i + 1);
			}

			m_completedMissionDlgs[i]->Open((FRESH_RESULT_CALLBACK)&CTaskMain::CompletedMission_Result, 0);
			m_completedMissionDlgs[i]->m_nFlags.Disable(0x400);
			m_completedMissionDlgs[i]->SetFadeout(false);
			((FrBirthdayEventMissionDlg *)m_completedMissionDlgs[i])->m_fadeOut = true;
			break;
		}
		break;
	}

	case 0x272:
		NotifyServerControlMsg(msg.m_param1);
		break;
	}
}

// taskmain.cpp:5591
bool CTaskMain::CompletedMission_Result(int, FrForm *form) {
	for (int i = 0; i < 5; i++) {
		if (m_completedMissionDlgs[i] == form) {
			m_completedMissionDlgs[i] = NULL;
			break;
		}
	}

	return true;
}

// taskmain.cpp:5606
void CTaskMain::SendRoomExitPacket(void) {
	WSendPacket packet((enumClientPacket)0x0F);
	packet.Encode1(0);
	packet.Encode2(0xFFFF);
	packet.Encode8(0);
	packet.Encode8(0);
	packet.Send(SEND_TO_DEFAULT);

	WSingleton<CSharedDoc>::Instance()->m_mySlot = 0xFFFF;
	WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber = 0xFFFF;
	this << MsgObject(NULL, 1, 0, 0, 0, 0, 0);
}

// taskmain.cpp:5627
void CTaskMain::SendRoomListExitPacket(void) {
	WSendPacket packet((enumClientPacket)0x82);
	packet.Send(SEND_TO_DEFAULT);

	this << MsgObject(NULL, 1, 0, 0, 0, 0, 0);
}

// taskmain.cpp:5635
void CTaskMain::OpenWaitedLayout(void) {
	if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false))
		CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);

	AfxPostMsg(NULL, "Lobby", 0, (intptr_t)m_waitedLayout.c_str(), 0, 0, 0);
}

// taskmain.cpp:5643
void CTaskMain::UpdateSiteInfo(void) {
	if (m_siteWnd) {
		if (OfflinePlay()) {
			m_siteWnd->SetCaption("OFFLINE PLAY");
		} else if (strlen(WSingleton<CSharedDoc>::Instance()->m_currentChannel.name)) {
			m_siteWnd->SetCaption(MakeStr("%s > %s", WSingleton<CSharedDoc>::Instance()->m_serverName, WSingleton<CSharedDoc>::Instance()->m_currentChannel.name));
		} else {
			m_siteWnd->SetCaption(WSingleton<CSharedDoc>::Instance()->m_serverName);
		}
	}
}

// taskmain.cpp:5656
void CTaskMain::OpenInformation(unsigned long typeId) {
	if (g_pFresh->GetManager()->GetDesktop()->FindChildByName("information"))
		return;

	FrForm *form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "information", 0);

	IFF_ITEM_COMMON *item = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindCommonItem(typeId);

	FrArea *portrait = DYNAMIC_CAST(FrArea, form->FindChildByName("portrait"));
	if (portrait) {
		if (item)
			portrait->SetBgImg(item->m_icon);
		else
			portrait->SetBgImg("hide");
	}

	if (item) {
		FrEdit *name = DYNAMIC_CAST(FrEdit, form->FindChildByName("name"));
		FrArea *level = DYNAMIC_CAST(FrArea, form->FindChildByName("level"));
		if (name && level) {
			if (item->m_level & 0x80) {
				level->SetBgImg(MakeStr("level_%03d", (item->m_level & 0x7F) + 1));
				// "Name: %s\n\nLevel: ... or below" (CP949)
				name->AddText(MakeStr("\xC0\xCC\xB8\xA7: %s\n\n\xB7\xB9\xBA\xA7:              \xC0\xCC\xC7\xCF", item->m_name), false, true);
			} else {
				level->SetBgImg(MakeStr("level_%03d", (item->m_level & 0x7F) + 1));
				// "Name: %s\n\nLevel: ... or above" (CP949)
				name->AddText(MakeStr("\xC0\xCC\xB8\xA7: %s\n\n\xB7\xB9\xBA\xA7:              \xC0\xCC\xBB\xF3", item->m_name), false, true);
			}
		}

		IFF_STRUCT::sDesc *desc = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindDesc(item->m_typeId);
		if (desc)
			form->SetMessage(desc->m_text, false);
	} else {
		FrEdit *name = DYNAMIC_CAST(FrEdit, form->FindChildByName("name"));
		if (name)
			name->AddText("\xC0\xCC\xB8\xA7: \xBA\xD2\xB8\xED\n\n\xB7\xB9\xBA\xA7: \xBA\xD2\xB8\xED", false, true);   // "Name: unknown\n\nLevel: unknown" (CP949)

		form->SetMessage("\xBA\xA3\xC0\xCF\xBF\xA1 \xBD\xCE\xC0\xCE \xC1\xA4\xC3\xBC\xBA\xD2\xB8\xED\xC0\xC7 \xC1\xB8\xC0\xE7", false);   // "a being of unknown identity, veiled" (CP949)
	}

	form->Open(0, 3);
}

// taskmain.cpp:6194
bool CTaskMain::OnRMR_ResultSetting(int result, FrForm *form) {
	if (form) {
		FrForm::EnableTail(true);
	}

	if (result == 1) {
		WSingleton<CSharedDoc>::Instance()->m_rmrMode = 5;
		ChangeRealMyRoom();
	} else {
		m_rmrSettingDlg = NULL;
	}
	return true;
}

// taskmain.cpp:6214
unsigned long CTaskMain::GetCategoryFlag(int category) {
	switch (category) {
	case 0:
	case 13:
	case 22:
		return 0;
	case 3:
		return 0x10;
	case 4:
		return 0x2C;
	case 5:
		return 0x40;
	case 26:
		return 0x380;
	case 1:
	case 14:
		return 1;
	case 2:
	case 15:
		return 2;
	case 16:
		return 3;
	case 21:
		return 0x3BF;
	case 28:
		return 7;
	case 29:
		return 9;
	case 30:
		return 8;
	case 34:
		return 6;
	default:
		return category;
	}
}

// taskmain.cpp:6285
bool CTaskMain::GetBtnState(unsigned char frame, eBarButton button) {
	if (!IsLocalContent((localContentType_t)0x56))
		return false;

	if (m_barButton[button]
		&& m_barButton[button]->IsVisible()
		&& m_barButton[button]->IsEnabled()) {
		if (m_barFrame[frame]
			&& m_barFrame[frame]->IsVisible())
			return m_barFrame[frame]->IsEnabled();
	}

	return false;
}

// actor.h:121
void IActor::OnReload(void) {
}

// actor.h:123
void IActor::OnDisplay(void) {
}

// itemstorage.h:449
bool CItemStorage::IsVisible(void) const {
	return m_visible;
}

// taskmanager.h:66
IActor *CTask::GetMainActor(void) const {
	return m_mainActor;
}

// taskmanager.h:260 CTaskManager::GetPreservedTask: see TaskManager.h (TASKMAIN_TASKMANAGER_INLINE)
