// Original: d:\build\custom\temp\code\client\projectg\golfrule.cpp
#define GOLFRULE_GOLFDOC_INLINE
#define GOLFRULE_CHATMSG_INLINE
#define GOLFRULE_LOGININFO_INLINE
#define GOLFRULE_CLUB_INLINE
#define GOLFRULE_GOLFBALL_INLINE
#define GOLFRULE_MOUSECURSOR_INLINE
#define GOLFRULE_SHAREDDOC_INLINE
#define GOLFRULE_WMATH_INLINE
#include "GolfRule.h"
#include "encryption.h"
#include "ClientSetting.h"
#include <mmsystem.h>
#include "GolfDoc.h"
#include "Fx.h"
#include "club.h"
#include "quadtree.h"
#include "wutil.h"
#include "mousecursor.h"
#include "headicon.h"
#include "messengerdlg.h"
#include "logininfo.h"
#include "ChatMsg.h"
#include "frwnd.h"
#include "frbutton.h"
#include "onelineboard.h"
#include "InputManager.h"
#include "fresh.h"
#include "Treasure.h"
#include "wind.h"
#include "cardsystem.h"
#include "globalgamedefine.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "ObjectFactory.h"
#include "QuitWindow.h"
#include "hackingmanager.h"
#include "user_info.h"
#include "wmath.h"
#include "Localize.h"
#include "NetResourceManager.h"
#include "Packet.h"
#include "golfball.h"
#include "winput.h"
#include "HelpMsg.h"
#include "CapturedBg.h"
#include "rival.h"
#include "RandomHouse.h"
#include "fredit.h"
#include "frcombobox.h"
#include "parttidlist.h"
#include "camerawork.h"
#include "SoundManager.h"
#include "ProjectG.h"
#include "scenemanager.h"
#include "ReplayDlg.h"
#include "GiftAlarmDlg.h"
#include "EmoticonDlg.h"
#include "frarea.h"
#include "ApproachResultDlg.h"
#include <io.h>
#include <stdio.h>
#include "commonutil.h"
#include "ToolTip.h"
#include "SimpleOverlay.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "../client/Wangreal/include/wview.h"
#include "Registry.h"

// Defined in another unit (TutorialTask also uses them).
// Name of the gateway actor (file-local data).
static const char* s_gatewayActorName = "Gateway";

extern int g_iSendPacketType;
extern int g_iRecvPacketType;
extern int g_iSendPacketGR;
extern int g_iRecvPacketGR;
extern bool g_bQuit;   // ProjectG.cpp
extern WMatrix g_camera;
// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev *g_ime;
// The bug report file of a ball stop replay (public ?ball_stop_replay_data_name@@3PBDB).
extern const char *const ball_stop_replay_data_name;
// The putting green musics (public ?g_bgmPutting@@3PAPBDA).
extern const char *g_bgmPutting[];
// The character motion names by shot timing (publics ?g_szCommonCharMotion@@3PAPBDA, ?g_szLuciaCharMotion@@3PAPBDA).
extern const char *g_szCommonCharMotion[];
extern const char *g_szLuciaCharMotion[];
// commonutil.cpp (commonutil.h does not declare it yet).
std::string GetMyComputerName();
// <direct.h>'s mkdir (the toolchain has no direct.h yet).
extern "C" __declspec(dllimport) int __cdecl mkdir(const char *);

// clientsetting.h:345
int COption::vGetTnLMode(void) const {
	return m_tnlMode;
}

// encryption.h:134
CEncryption::CEncryption() {
	(void)&CEncryption::Instance;	// odr-use: emit Instance's local-static helper (SendShotResult's call site)
}

// encryption.h:135
CEncryption::~CEncryption() {
}

// headicon.h:30
bool CHeadIcon::GetVisible(void) const {
	return m_visible;
}

// headicon.h:31
void CHeadIcon::Show(void) {
	m_visible = true;
}

// headicon.h:32
void CHeadIcon::Hide(void) {
	m_visible = false;
}

// messengerdlg.h:369
bool CMessengerInfo::IsGameFocus(void) {
	return m_gameFocus;
}

// logininfo.h:481
bool IsWebLogin(void) {
	return CLoginInfo::Instance()->IsWebLogin() ? true : false;
}

// quadtree.h:112
void CQuadTree::SetSlopeFactor(float factor) {
	m_slopeFactor = factor;
}

// quadtree.h:157
float CQuadTree::GetLowest(void) const {
	return m_lowest;
}

// quadtree.h:423
WVector CQuadTree::GetPenaltyPos(void) {
	return m_penaltyPos;
}

// quadtree.h:497
void CQuadTree::SetFrame(unsigned long frame) {
	m_frame = frame;
	m_frameTime = 0.0f;
}

// quadtree.h:505
unsigned long CQuadTree::GetFrame(void) {
	return m_frame;
}

// club.h:203
bool CClub::BarIs(int bar) {
	return m_bar == bar;
}

// club.h:209
void CClub::SetRandomPower(float power) {
	m_randomPower = power;
}

// club.h:210
float CClub::GetRandomPower(void) const {
	return m_randomPower;
}

// club.h:211
void CClub::SetCalcRandomPower(bool calc) {
	m_calcRandomPower = calc;
}

// golfdoc.h:298
float sBar::Persent(void) const {
	return (m_power - BAR_START) / BAR_LENGTH;
}

// golfdoc.h:300
float sBar::Delta(void) const {
	return m_position - m_startPosition;
}

// golfdoc.h:512
sHoleData &CGolfDoc::GetHoleData(unsigned char hole) {
	return m_holeData[hole - 1];
}

// golfdoc.h:650
void CGolfDoc::SetStatUpdated(bool updated) {
	m_statUpdated = updated;
}

// golfdoc.h:651
bool CGolfDoc::GetStatUpdated(void) const {
	return m_statUpdated;
}

// golfdoc.h:671
bool CGolfDoc::GetApproachReloaded(void) {
	return m_approachReloaded;
}

// golfdoc.h:672
void CGolfDoc::SetApproachReloaded(bool reloaded) {
	m_approachReloaded = reloaded;
}

// logininfo.h:482
int IsPcBang(void) {
	return CLoginInfo::Instance()->IsPcBang();
}

// treasure.h:97
bool CTHunter::IsReturnLobby() {
	return m_bReturnLobby;
}

// treasure.h:98
void CTHunter::SetReturnLobby(bool returnLobby) {
	m_bReturnLobby = returnLobby;
}

// treasure.h:101
void CTHunter::SetReceiveGiftPacket(bool receive) {
	m_bReceiveGiftPacket = receive;
}

// wind.h:57
bool CWind::IsActiveSpecialWind(void) {
	return m_activeSpecialWind;
}

// cardsystem.h:115
std::list<unsigned long> &CCardManager::GetActiveCardList(void) {
	return m_activeCardList;
}

// onelineboard.h:22
void COneLineBoard::ShowBg(bool show) {
	m_showBg = show;
}
 

// frwnd.h:213
bool FrWnd::HasKeyFocus(void) {
	return m_nFlags.GetFlag(0x10) ? true : false;
}

// globalgamedefine.h:1132
unsigned char EncryptKey::GetKey(int index) {
	return m_key[index];
}

// taskmanager.h:106
void CTask::SetPause(bool pause) {
	m_pause = pause;
}

// shareddoc.h:1018
void CSharedDoc::SetReplayState(int state) {
	m_replayState = state;
}

// shareddoc.h:1019
int CSharedDoc::GetReplayState(void) {
	return m_replayState;
}

// shareddoc.h:1021
char *CSharedDoc::GetFileName(void) {
	return m_fileName;
}

// shareddoc.h:1022
int CSharedDoc::GetReplayPlayType(void) {
	return m_replayPlayType;
}

// shareddoc.h:1023
void CSharedDoc::SetReplayPlayType(int type) {
	m_replayPlayType = type;
}

// shareddoc.h:1150
void CSharedDoc::SetGoStopMode(bool goStop) {
	m_goStopMode = goStop;
}

// shareddoc.h:1151
bool CSharedDoc::GetGoStopMode(void) {
	return m_goStopMode;
}

// shareddoc.inl:134
bool IsOthersTurn(bool b) {
	return !IsMyTurn(b);
}

// shareddoc.inl:224
unsigned char GetHoles(void) {
	return WSingleton<CSharedDoc>::Instance()->m_holes;
}

// shareddoc.inl:234
unsigned char GetStartHole(void) {
	return WSingleton<CSharedDoc>::Instance()->m_holeOrder[0];
}

// shareddoc.inl:249
unsigned char GetEndHole(void) {
	CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
	return doc->m_holeOrder[doc->m_holes - 1];
}

// shareddoc.inl:416
void SetGameOver(bool gameOver) {
	WSingleton<CSharedDoc>::Instance()->m_gameOver = gameOver;
}

// shareddoc.inl:421
bool IsGameOver(void) {
	return WSingleton<CSharedDoc>::Instance()->m_gameOver;
}

// shareddoc.inl:503
// This client's player index: a gallery member's by its gallery GUID (in
// rooms 4 to 6, 9 and 10) or the golf doc's, else by its own GUID.
unsigned char MyIndex(void) {
	CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
	if (doc->m_userInfo.IsIdentity(2)) {
		switch (doc->m_roomType) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10:
			return doc->GetIndex(doc->m_userInfo.galleryGuid);
		default:
			return doc->m_pGolfDoc->m_myPlayerIndex;
		}
	}

	return doc->GetIndex(doc->m_userInfo.guid);
}

// shareddoc.inl:540
int IsObserver(unsigned long identity) {
	if (identity == 0xFFFFFFFF)
		identity = WSingleton<CSharedDoc>::Instance()->m_userInfo.identity;

	return (identity & 0xE) == 0xE;
}

// golfrule.h:617
void CGolfRule::SetFlagCutin(bool flagCutin) {
	m_flagCutin = flagCutin;
}

// golfrule.h:618
bool CGolfRule::GetFlagCutin(void) {
	return m_flagCutin;
}

// golfrule.cpp:258
// The motion grade of a shot: power shot 4, else 2 for the second flag, plus
// one for the bonus flag (at most 5), looked up in the special or normal table.
static int __stdcall GetShotMotionGrade(bool special, bool powerShot, bool secondFlag, bool bonus) {
	int grade = 0;

	if (powerShot)
		grade = 4;
	else if (secondFlag)
		grade = 2;

	if (bonus)
		grade++;

	if (grade >= 5)
		grade = 5;

	if (special) {
		int grades[6] = { 12, 3, 14, 3, 3, 3 };
		return grades[grade];
	} else {
		int grades[6] = { 4, 3, 4, 3, 3, 3 };
		return grades[grade];
	}
}

// golfrule.cpp:296
// The character motion name of a shot timing (-3..3, GetBarClickTiming).
static const char *GetCharMotionByTiming(int timing, int, int charType, int motionMode) {
	int index = 0;

	if (motionMode != 1) {
		if (timing < -3 || timing > 3)
			index = 7;
		else
			index = timing + 4;

		if (index >= 7)
			index = 7;
	}

	bool lucia = charType == 8;

	return lucia ? g_szLuciaCharMotion[index] : g_szCommonCharMotion[index];
}

namespace {

// golfrule.cpp:370
int GetBarClickTiming(void) {
	switch (GOLFDOC()->GetBar().m_accuracyGrade) {
	case 3:
		return GOLFDOC()->GetBar().m_position - GOLFDOC()->GetBar().m_startPosition > 0.0f ? 1 : -1;
	case 2:
		return GOLFDOC()->GetBar().m_position > 140.0f ? 2 : -2;
	case 1:
	case 0:
		return GOLFDOC()->GetBar().m_position - GOLFDOC()->GetBar().m_startPosition > 0.0f ? 3 : -3;
	}
	return 0;
}

// golfrule.cpp:392
// The power factor of a bar click: full power for a putter or an unknown
// grade, else the root of the ground power blended by the click's error.
float GetBarClickPowerFactor(void) {
	if (GolfClub().GetType() != 3) {
		CGolfDoc *doc = GOLFDOC();

		switch (doc->GetBar().m_accuracyGrade) {
		case 4:
			return sqrtf(GetPVS().GetMaxPowerOnGround());
		case 3: {
			float delta = doc->GetBar().m_position - doc->GetBar().m_startPosition;
			float ratio;
			if (delta > 0.0f)
				ratio = delta / (GolfClub().GetNarrowAccuracy() + 140.0f - doc->GetBar().m_startPosition);
			else
				ratio = delta / (doc->GetBar().m_startPosition - (140.0f - GolfClub().GetNarrowAccuracy()));

			return sqrtf(GetPVS().GetMaxPowerOnGround() * (1.0f - Abs(ratio)) + GetPVS().GetMinPowerOnGround() * Abs(ratio));
		}
		case 2: {
			float ratio = (Abs(doc->GetBar().m_position - 140.0f) - GolfClub().GetNarrowAccuracy()) / (GolfClub().GetWideAccuracy() - GolfClub().GetNarrowAccuracy());

			return sqrtf(GetPVS().GetMinPowerOnGround() * (1.0f - ratio) + GetPVS().GetMinPowerOnGround() * ratio);
		}
		}
	}

	return 1.0f;
}

} // namespace

// golfrule.cpp:870 (run-time type and factory registration of CGolfRule)
const WRTTI CGolfRule::m_RTTI("CGolfRule", &CTaskMain::m_RTTI);

IObject *CGolfRuleMakeInstance(void);

struct __impCGolfRule {
	__impCGolfRule(void) {
		ObjectFactory().AddObjectFunctor(CGolfRuleMakeInstance, "CGolfRule");
		ActorFactory().AddActorClass("CGolfRule");
	}
};
static __impCGolfRule s_impCGolfRule;

// golfrule.cpp:870
IObject *CGolfRuleMakeInstance(void) {
	return new CGolfRule;
}

// golfrule.cpp:872
const sFRESH_MSGMAP *CGolfRule::GetMessageMap(void) const {
	return &CGolfRule::_MsgMap;
}

// golfrule.cpp:998
void CGolfRule::OnEmoticonInit(intptr_t param) {
	m_emoticonBtn = DYNAMIC_CAST(FrButton, param);
	if (m_emoticonBtn) {
		m_emoticonBtn->SetVisible(false);

		WPoint pt;
		pt.x = m_emoticonBtn->GetRect().x;
		pt.y = COption::Instance()->gGetBar_Y() + 16.0f;
		m_emoticonBtn->MoveWindow(pt);
	}
}

// golfrule.cpp:1015
void CGolfRule::OnEmoticonBtnUp(void) {
	if (m_emoticonDlg == NULL) {
		m_emoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (m_emoticonDlg) {
			WPoint pt(0.0f, 0.0f);
			if (m_emoticonBtn) {
				pt.x = m_emoticonBtn->GetRect().x - 200.0f;
				pt.y = m_emoticonBtn->GetRect().y - 260.0f;
			}

			if (!g_pFresh->GetManager()->IsKeyExclusive())
				g_pFresh->GetManager()->SetExclusiveKey(true);

			m_emoticonDlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnEmoticonResult, pt, 3);
		}
	} else {
		m_emoticonDlg->Close(FrCANCEL, true);
	}
}

// golfrule.cpp:1076
void CGolfRule::OnReportInit(int param) {
	m_reportBtn = DYNAMIC_CAST(FrButton, param);
	if (m_reportBtn) {
		m_reportBtn->SetVisible(false);

		WPoint pt;
		pt.x = m_reportBtn->GetRect().x;
		pt.y = COption::Instance()->gGetBar_Y() + 16.0f;
		m_reportBtn->MoveWindow(pt);
	}
}

// golfrule.cpp:1092
void CGolfRule::OnReportBtnUp(void) {
	if (GOLFDOC()->m_reportSent) {
		FrForm *form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "A report on the current game has already been received."
		form->SetMessage("\xC7\xF6\xC0\xE7 \xB0\xD4\xC0\xD3\xBF\xA1 \xB4\xEB\xC7\xD8\xBC\xAD \xC0\xCC\xB9\xCC \xBD\xC5\xB0\xED\xB0\xA1 \xC1\xA2\xBC\xF6\xB5\xC7\xBE\xEE \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		form->Open((FRESH_RESULT_CALLBACK)0, 0);
		return;
	}

	if (m_reportDlg == NULL) {
		m_reportDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);

		// "Reporting sends the whole chat of the current game to the operators..."
		m_reportDlg->SetMessage("\xBD\xC5\xB0\xED\xB8\xA6 \xC7\xCF\xBD\xC3\xB8\xE9 \xC7\xF6\xC0\xE7\xB0\xD4\xC0\xD3\xC0\xC7 \xB4\xEB\xC8\xAD \xB3\xBB\xBF\xEB\xC0\xCC \xB8\xF0\xB5\xCE \xBF\xEE\xBF\xB5\xC0\xDA\xBF\xA1\xB0\xD4 \xBD\xC5\xB0\xED\xB0\xA1 \xB5\xCB\xB4\xCF\xB4\xD9.\n\n\xB4\xEB\xC8\xAD \xB3\xBB\xBF\xEB \xC1\xDF \xC3\xB3\xB9\xFA\xC7\xD8\xB4\xE7\xBB\xE7\xC7\xD7\xC0\xCC \xC0\xD6\xB4\xC2 \\c0xffff0000\\c\xB8\xF0\xB5\xE7 \xBE\xC6\xC0\xCC\xB5\xF0\\c0xff000000\\c\xB4\xC2 \xBA\xED\xB7\xB0 \xC3\xB3\xB8\xAE\xB8\xA6 \xB4\xE7\xC7\xCF\xB0\xD4 \xB5\xCB\xB4\xCF\xB4\xD9.\n\n\xBD\xC5\xB0\xED \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		m_reportDlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnReportResult, 0);
	}
}

// golfrule.cpp:1112
void CGolfRule::OnChatTargetInit(intptr_t param) {
	m_chatEdit = DYNAMIC_CAST(FrComboBox, param);
	if (m_chatEdit) {
		m_chatEdit->SetVisible(false);
		WSingleton<CSharedDoc>::Instance()->InitReservedChatList(m_chatEdit,
			WSingleton<CSharedDoc>::Instance()->m_roomType == 5 || WSingleton<CSharedDoc>::Instance()->m_roomType == 6
				? (eReservedChatPartner)(RESERVED_CHAT_PARTNER_TEAM | RESERVED_CHAT_PARTNER_GUILD)
				: RESERVED_CHAT_PARTNER_GUILD);

		WPoint pt;
		pt.x = m_chatEdit->GetRect().x;
		pt.y = COption::Instance()->gGetBar_Y() + 14.0f;
		m_chatEdit->MoveWindow(pt);
	}
}

// golfrule.cpp:1131
void CGolfRule::OnChatTargetBtnDown(int) {
	if (m_chatInput)
		m_chatInput->SetKeyFocus(true);
}

// golfrule.cpp:1138
bool CGolfRule::OnChatTargetEnterKey(int) {
	if (m_chatEdit && strlen(m_chatEdit->GetLine(1, false)) == 0)
		m_chatEdit->SetLine(1, "\xB8\xF0\xB5\xCE\xBF\xA1\xB0\xD4", 0, false, 0);

	if (m_chatInput)
		m_chatInput->SetKeyFocus(true);

	return false;
}

// golfrule.cpp:1150
void CGolfRule::OnChatInputInit(intptr_t param) {
	m_chatInput = DYNAMIC_CAST(FrEdit, param);
	if (m_chatInput) {
		m_chatInput->SetVisible(false);

		WPoint pt;
		pt.x = m_chatInput->GetRect().x;
		pt.y = COption::Instance()->gGetBar_Y() + 14.0f;
		m_chatInput->MoveWindow(pt);
	}
}

// golfrule.cpp:1240
void CGolfRule::OnSpecialCardInit(int param) {
	m_barButton[10] = DYNAMIC_CAST(FrButton, param);
	if (m_barButton[10]) {
		m_barButton[10]->SetVisible(false);
		m_barButton[10]->EnableHover(true, 0.0f);
		m_barButton[10]->EnableBlink(CCardManager::Instance()->ShowSPCardAlarm());

		WPoint pt;
		pt.x = g_view->GetWidth() - 106.0f;
		pt.y = m_barButton[10]->GetRect().y;
		m_barButton[10]->MoveWindow(pt);
	}
}

// golfrule.cpp:1256
void CGolfRule::OnSpecialCardBtnUp(void) {
	CTaskMain::OnOverBar_SpecialCardUp();
}

// golfrule.cpp:1261
void CGolfRule::OnMyInfoInit(int param) {
	m_myInfoBtn = DYNAMIC_CAST(FrButton, param);
	if (m_myInfoBtn) {
		m_myInfoBtn->SetVisible(false);
		m_myInfoBtn->EnableHover(true, 0.0f);

		WPoint pt;
		pt.x = g_view->GetWidth() - 54.0f;
		pt.y = m_myInfoBtn->GetRect().y;
		m_myInfoBtn->MoveWindow(pt);
	}
}

// golfrule.cpp:1276
void CGolfRule::OnMyInfoBtnUp(void) {
	if (CUserInfo::Instance()) {
		CUserInfo::Instance()->SetInfo(MyUID(), CSharedDoc::Instance()->GetPangyaUserInfo().guid, true, true, false, false, CSharedDoc::Instance()->GetPangyaUserInfo().nickname);
	}
	OnBarHoverOff(0);
}

// golfrule.cpp:1286
void CGolfRule::OnOptionInit(int param) {
	m_optionBtn = DYNAMIC_CAST(FrButton, param);
	if (m_optionBtn) {
		m_optionBtn->SetVisible(false);
		m_optionBtn->EnableHover(true, 0.0f);

		WPoint pt;
		pt.x = g_view->GetWidth() - 29.0f;
		pt.y = m_myInfoBtn->GetRect().y;
		m_optionBtn->MoveWindow(pt);
	}
}

// golfrule.cpp:1301
void CGolfRule::OnOptionBtnUp(void) {
	if (CQuitWindow::Instance()) {
		CQuitWindow::Instance()->CreateOptionWindow();
	}
	OnBarHoverOff(0);
}

// golfrule.cpp:1311
void CGolfRule::OnCaddieBookInit(int param) {
	m_commandBtns[0] = DYNAMIC_CAST(FrButton, param);
	if (m_commandBtns[0]) {
		m_commandBtns[0]->SetVisible(false);

		WPoint pt;
		pt.x = g_view->GetWidth() - 423.0f;
		pt.y = m_commandBtns[0]->GetRect().y;
		m_commandBtns[0]->MoveWindow(pt);

		ResetUiPos_Board();
	}
}

// golfrule.cpp:1327
void CGolfRule::OnCaddieBookBtnUp(void) {
	SendMsg(this, "GuideBook", 0x12A, 0, 0, 0, 0);
}

// golfrule.cpp:1332
void CGolfRule::OnUnplayableInit(int param) {
	m_commandBtns[1] = DYNAMIC_CAST(FrButton, param);
	if (m_commandBtns[1]) {
		m_commandBtns[1]->SetVisible(false);

		WPoint pt;
		pt.x = g_view->GetWidth() - 398.0f;
		pt.y = m_commandBtns[1]->GetRect().y;
		m_commandBtns[1]->MoveWindow(pt);
	}
}

// golfrule.cpp:1346
void CGolfRule::OnUnplayableBtnUp(void) {
	if (GOLFDOC()->m_approachMode) {
		if (CHelpMsg::Instance())
			CHelpMsg::Instance()->SetHelpMsg(0x17, 0);
		return;
	}

	if (GOLFDOC()->GameMode() & 0x48108)
		SendMsg(this, "Screen", 0x1CB, 0, 0, 0, 0);
}

// golfrule.cpp:1360
void CGolfRule::OnPauseInit(int param) {
	m_commandBtns[2] = DYNAMIC_CAST(FrButton, param);
	if (m_commandBtns[2]) {
		m_commandBtns[2]->SetVisible(false);

		WPoint pt;
		pt.x = g_view->GetWidth() - 373.0f;
		pt.y = m_commandBtns[2]->GetRect().y;
		m_commandBtns[2]->MoveWindow(pt);
	}
}

// golfrule.cpp:1374
void CGolfRule::OnPauseBtnUp(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 9:
		if (CHelpMsg::Instance())
			CHelpMsg::Instance()->SetHelpMsg(0x18, 0);
		break;

	default:
		if (GOLFDOC()->GameMode() & 0x48108)
			SendMsg(this, "Screen", 0x1CC, 0, 0, 0, 0);
		break;
	}
}

// golfrule.cpp:1388
void CGolfRule::OnChatRecordInit(int param) {
	m_commandBtns[3] = DYNAMIC_CAST(FrButton, param);
	if (m_commandBtns[3]) {
		m_commandBtns[3]->SetVisible(false);

		WPoint pt;
		pt.x = g_view->GetWidth() - 348.0f;
		pt.y = m_commandBtns[3]->GetRect().y;
		m_commandBtns[3]->MoveWindow(pt);
	}
}

// golfrule.cpp:1402
void CGolfRule::OnChatRecordBtnUp(void) {
	SendMsg(this, "Screen", 0x1CD, 0, 0, 0, 0);
}

// golfrule.cpp:1407
void CGolfRule::OnOnelineViewInit(int param) {
	m_onelineBoardWnd = DYNAMIC_CAST(FrArea, param);
	m_onelineBoardWnd->SetVisible(false);

	COneLineBoard::Instance()->SetRect(m_onelineBoardWnd->GetRect());
	COneLineBoard::Instance()->LoadBg();
}

// golfrule.cpp:1417
void CGolfRule::OnOnelineViewOwnerDraw(int) {
	COneLineBoard::Instance()->Display();
}

// golfrule.cpp:1436
bool CGolfRule::OnOnelineReqChkOutDlgResult(int result, FrForm *) {
	if (result == 1) {
		WSendPacket packet((enumClientPacket)0x66);
		packet.EncodeStr(COneLineBoard::Instance()->GetMessageA());
		packet.Send(SEND_TO_DEFAULT);
	}
	return true;
}

// golfrule.cpp:1519
void CGolfRule::OnTopBarHoverOn(int param) {
	m_hoverButton = DYNAMIC_CAST(FrButton, param);

	if (m_hoverButton) {
		if (m_hoverButton == m_messengerWnd)
			CIconToolTip::GetInstance()->SetToolTip((eBarButton)11);
		else if (m_hoverButton == m_myInfoBtn)
			CIconToolTip::GetInstance()->SetToolTip((eBarButton)12);
		else if (m_hoverButton == m_optionBtn)
			CIconToolTip::GetInstance()->SetToolTip((eBarButton)13);
		else if (m_hoverButton == m_barButton[10])
			CIconToolTip::GetInstance()->SetToolTip((eBarButton)10);
	}
}

// golfrule.cpp:1536
void CGolfRule::OnTopBarHoverOff(int param) {
	FrButton *btn = DYNAMIC_CAST(FrButton, param);

	if (btn == NULL || btn == m_hoverButton) {
		m_hoverButton = NULL;
		CIconToolTip::GetInstance()->ClearToolTip();
	}
}

// golfrule.cpp:1548
void CGolfRule::InitRadioPredictShotButton(FrButton **button, int wnd, float offset) {
	FrButton *btn = DYNAMIC_CAST(FrButton, wnd);

	if (btn) {
		WPoint pt(g_view->GetWidth() - 180.0f, g_view->GetHeight() - offset);
		btn->MoveWindow(pt);
		btn->SetPushDelay(0.1f);
	}
	*button = btn;
}

// golfrule.cpp:1568
void CGolfRule::SetRadioPredictShotPress(FrButton *button) {
	if (m_predictNormalShotBtn) m_predictNormalShotBtn->SetStatus(FrButton::NORMAL);
	if (m_predictTomahwakShotBtn) m_predictTomahwakShotBtn->SetStatus(FrButton::NORMAL);
	if (m_predictCobraShotBtn) m_predictCobraShotBtn->SetStatus(FrButton::NORMAL);
	if (m_predictSpikeShotBtn) m_predictSpikeShotBtn->SetStatus(FrButton::NORMAL);

	if (button) button->SetStatus(FrButton::PRESSED);

	int type = 0;
	if (button == m_predictTomahwakShotBtn) type = 1;
	if (button == m_predictCobraShotBtn) type = 2;
	if (button == m_predictSpikeShotBtn) type = 3;
	SendMsg(this, "ShotPreview", -1, (intptr_t)"CHANGE_PREDICT_TYPE", type, 0, 0);
}

// golfrule.cpp:1584
void CGolfRule::UpdateRadioPredictShotButtonStatus(void) {
	bool visible = GolfClub().GetType() != 3;

	if (m_predictNormalShotBtn) m_predictNormalShotBtn->SetVisible(visible);
	if (m_predictTomahwakShotBtn) m_predictTomahwakShotBtn->SetVisible(visible);
	if (m_predictCobraShotBtn) m_predictCobraShotBtn->SetVisible(visible);
	if (m_predictSpikeShotBtn) m_predictSpikeShotBtn->SetVisible(visible);
}

// golfrule.cpp:1596
void CGolfRule::OnRadioPredictNormalShotInit(int wnd) {
	InitRadioPredictShotButton(&m_predictNormalShotBtn, wnd, 100.0f);
	if (m_predictNormalShotBtn)
		m_predictNormalShotBtn->SetStatus(FrButton::PRESSED);
}

// golfrule.cpp:1600
void CGolfRule::OnRadioPredictTomahwakShotInit(int wnd) {
	InitRadioPredictShotButton(&m_predictTomahwakShotBtn, wnd, 80.0f);
}

// golfrule.cpp:1603
void CGolfRule::OnRadioPredictCobraShotInit(int wnd) {
	InitRadioPredictShotButton(&m_predictCobraShotBtn, wnd, 60.0f);
}

// golfrule.cpp:1606
void CGolfRule::OnRadioPredictSpikeShotInit(int wnd) {
	InitRadioPredictShotButton(&m_predictSpikeShotBtn, wnd, 40.0f);
}

// golfrule.cpp:1613
void CGolfRule::OnRadioPredictNormalShotDown(void) {
	if (m_predictNormalShotBtn) {
		if (m_predictNormalShotBtn->GetStatus() != FrButton::PRESSED)
			SetRadioPredictShotPress(m_predictNormalShotBtn);
	}
}

// golfrule.cpp:1618
void CGolfRule::OnRadioPredictTomahwakShotDown(void) {
	if (m_predictTomahwakShotBtn) {
		if (GolfClub().PowerShotType()) SetRadioPredictShotPress(m_predictTomahwakShotBtn);
		else m_predictTomahwakShotBtn->SetStatus(FrButton::NORMAL);
	}
}

// golfrule.cpp:1623
void CGolfRule::OnRadioPredictCobraShotDown(void) {
	if (m_predictCobraShotBtn) {
		if (GolfClub().PowerShotType()) SetRadioPredictShotPress(m_predictCobraShotBtn);
		else m_predictCobraShotBtn->SetStatus(FrButton::NORMAL);
	}
}

// golfrule.cpp:1628
void CGolfRule::OnRadioPredictSpikeShotDown(void) {
	if (m_predictSpikeShotBtn) {
		if (GolfClub().PowerShotType()) SetRadioPredictShotPress(m_predictSpikeShotBtn);
		else m_predictSpikeShotBtn->SetStatus(FrButton::NORMAL);
	}
}

// golfrule.cpp:1638
void CGolfRule::OnRadioPredictNormalShotDraw(int) {
	FrButton *button = m_predictNormalShotBtn;
	if (button) {
		button->MoveWindow(WPoint(g_view->GetWidth() - 180.0f, g_view->GetHeight() - 100.0f));

		WRect rect = button->GetRect();
		float x = rect.x + rect.w - 35.0f;
		if (button->GetStatus() == FrButton::PRESSED)
			m_predictShotUI->SetColor(0xFFFFFFFF);
		else
			m_predictShotUI->SetColor(0x80808080);

		m_predictShotUI->Render("Pangya", x, rect.y, 1.0f, 0);
	}
}

// golfrule.cpp:1653
void CGolfRule::OnRadioPredictTomahwakShotDraw(int) {
	FrButton *button = m_predictTomahwakShotBtn;
	if (button) {
		if (!GolfClub().PowerShotType())
			SetRadioPredictShotPress(m_predictNormalShotBtn);

		button->MoveWindow(WPoint(g_view->GetWidth() - 180.0f, g_view->GetHeight() - 80.0f));
		button->Enable(GolfClub().PowerShotType() != 0);

		WRect rect = button->GetRect();
		float x = rect.x + rect.w - 23.0f;
		if (button->GetStatus() == FrButton::PRESSED)
			m_predictShotUI->SetColor(0xFFFFFFFF);
		else
			m_predictShotUI->SetColor(0x80808080);

		m_predictShotUI->Render("Tomahawk", x, rect.y, 1.0f, 0);
	}
}

// golfrule.cpp:1671
void CGolfRule::OnRadioPredictCobraShotDraw(int) {
	FrButton *button = m_predictCobraShotBtn;
	if (button) {
		if (!GolfClub().PowerShotType())
			SetRadioPredictShotPress(m_predictNormalShotBtn);

		button->MoveWindow(WPoint(g_view->GetWidth() - 180.0f, g_view->GetHeight() - 60.0f));
		button->Enable(GolfClub().PowerShotType() != 0);

		WRect rect = button->GetRect();
		float x = rect.x + rect.w - 40.0f;
		if (button->GetStatus() == FrButton::PRESSED)
			m_predictShotUI->SetColor(0xFFFFFFFF);
		else
			m_predictShotUI->SetColor(0x80808080);

		m_predictShotUI->Render("Cobra", x, rect.y, 1.0f, 0);
	}
}

// golfrule.cpp:1689
void CGolfRule::OnRadioPredictSpikeShotDraw(int) {
	FrButton *button = m_predictSpikeShotBtn;
	if (button) {
		if (!GolfClub().PowerShotType())
			SetRadioPredictShotPress(m_predictNormalShotBtn);

		button->MoveWindow(WPoint(g_view->GetWidth() - 180.0f, g_view->GetHeight() - 40.0f));
		button->Enable(GolfClub().PowerShotType() != 0);

		WRect rect = button->GetRect();
		float x = rect.x + rect.w - 40.0f;
		if (button->GetStatus() == FrButton::PRESSED)
			m_predictShotUI->SetColor(0xFFFFFFFF);
		else
			m_predictShotUI->SetColor(0x80808080);

		m_predictShotUI->Render("Spike", x, rect.y, 1.0f, 0);
	}
}

// golfrule.cpp:1708
CGolfRule::CGolfRule(void) {
	m_priority = 0;
	m_chatViewWnd = NULL;
	m_emoticonBtn = NULL;
	m_wnd47C = NULL;
	m_reportBtn = NULL;
	m_chatEdit = NULL;
	m_chatInput = NULL;
	m_emoticonDlg = NULL;
	m_reportDlg = NULL;
	m_myInfoBtn = NULL;
	m_optionBtn = NULL;
	m_commandBtns[0] = NULL;
	m_commandBtns[1] = NULL;
	m_commandBtns[2] = NULL;
	m_commandBtns[3] = NULL;
	m_onelineBoardWnd = NULL;
	m_boardWnd = NULL;
	m_forgetDlg = NULL;
	m_forgetPending = false;
	m_forgetElapsed = 0.0f;
	m_cancelShotItemSlot = 0x1800002D;
	m_playerInfoDlg = NULL;
	m_form494 = NULL;
	m_approachResultDlg = NULL;
	m_approachEndDlg = NULL;
	m_flag4C4 = false;
	memset(m_text4C5, 0, sizeof(m_text4C5));
	m_pRule = NULL;

	if (IsLocalContent((localContentType_t)0x50)) {
		m_value1E4 = m_value1E0 = 0;
		m_value1E8 = 0;
	}

	if (IsLocalContent((localContentType_t)0x4F)) {
		m_replayBox = NULL;
		m_recordAviDlg = NULL;

		CReplayControlManager::Instance()->Init();
		m_value59C = 0;
		m_value5A0 = 0;
	}

	CSharedDoc::Instance()->SetGoStopMode(false);

	m_predictShotUI = new CSimpleUI("[impact.png");
	m_predictShotUI->Add("Pangya", WRect(0.0f, 18.0f, 128.0f, 18.0f), false);
	m_predictShotUI->Add("Tomahawk", WRect(0.0f, 72.0f, 128.0f, 18.0f), false);
	m_predictShotUI->Add("Cobra", WRect(0.0f, 126.0f, 128.0f, 18.0f), false);
	m_predictShotUI->Add("Spike", WRect(0.0f, 144.0f, 128.0f, 18.0f), false);

	m_predictNormalShotBtn = NULL;
	m_predictTomahwakShotBtn = NULL;
	m_predictCobraShotBtn = NULL;
	m_predictSpikeShotBtn = NULL;
	m_value5BC = 0;
}

// golfrule.cpp:1787
CGolfRule::~CGolfRule(void) {
	if (m_predictShotUI) { delete m_predictShotUI; m_predictShotUI = NULL; }

	if (m_pRule) { delete m_pRule; m_pRule = NULL; }
}

// golfrule.cpp:2074
void CGolfRule::OnLoad(void) {
	CTaskMain::OnLoad();
	OnReload();
	CTaskMain::OnInit();
	g_pFresh->OpenLayout("MAIN", this, true);
}

// golfrule.cpp:2109
void CGolfRule::OnInit(void) {
	if (CSharedDoc::Instance()->m_pCurCourse) {
		GetPVS().SetSlopeFactor(CSharedDoc::Instance()->m_pCurCourse->m_rate);
	}
	CSceneManager::Instance()->UpdateWind();

	m_flag42C = false;
	CalcUIOffset(false);

	SendMsg(this, "Score", 0x12F, 0, 0, 0, 0);

	m_replayBox = NULL;
	m_recordAviDlg = NULL;
	m_flag5B8 = false;
	m_waitingForStart = false;
	IActor *pGateway = GetActor(s_gatewayActorName);
	pGateway << MsgObject(0, 0x26F, 0xC, 0, 0, 0, 0);
}

// golfrule.cpp:2265
void CGolfRule::SetPlayer(void) {
	if (HackingManager::Instance()) {
		HackingManager::Instance()->ClearCrypticList();
	}
	if (m_pRule) {
		m_pRule->SetPlayer();
	}
}

// golfrule.cpp:2285
void CGolfRule::SetGrade(void) {
	if (m_pRule != NULL)
		m_pRule->SetGrade();
}

// golfrule.cpp:2295
void CGolfRule::SetReady(void) {
	if (CSharedDoc::Instance()->m_playingMode == 2) {
		ChangeGameMode(0x10000);
	} else {
		if (m_pRule) {
			m_pRule->SetReady();
		}
	}
}

// golfrule.cpp:2350
void CGolfRule::SetVisibleUpperBtns(bool visible) {
	if (m_chatViewWnd)
		m_chatViewWnd->SetVisible(visible);

	if (m_boardWnd)
		m_boardWnd->SetVisible(visible);

	if (m_myInfoBtn)
		m_myInfoBtn->SetVisible(visible);

	if (m_optionBtn)
		m_optionBtn->SetVisible(visible);

	if (m_messengerWnd)
		m_messengerWnd->SetVisible(visible);
}

// golfrule.cpp:2410
void CGolfRule::OnDestroy(void) {
	CTaskMain::OnDestroy();

	delete CCameraWork::Instance();
	delete CRival::Instance();
	delete CRandomHouse::Instance();
	delete CCapturedBg::Instance();

	while (!m_backUps.empty())
		m_backUps.pop_back();
}

// golfrule.cpp:3210
void CGolfRule::DoFixedCamera(float) {
	if (g_input->GetDown("\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9", true)) {
		CMouseCursor::Instance()->Store();
		ChangeGameMode(8);
	} else if (TopViewClick()) {
		ChangeGameMode(0x200);
	}
}

// golfrule.cpp:3226
void CGolfRule::DoSpotCamera(float) {
	if (g_input->GetDown("\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9", true)) {
		CMouseCursor::Instance()->Store();
		ChangeGameMode(8);
	} else if (TopViewClick()) {
		ChangeGameMode(0x200);
	}
}

// golfrule.cpp:3243
bool CGolfRule::CheckDropOut(void) {
	if (m_pRule) {
		int numTimeout = m_pRule->GetNumTimeout();

		if (numTimeout >= 3) {
			if (!OnlinePlay())
				SendMsg(this, "GolfRule", 0x82, PLAYER(GOLFDOC()->m_myPlayerIndex)->m_uid, 0, 0, 0);

			return true;
		}
	}

	return false;
}

// golfrule.cpp:3268
bool CGolfRule::CheckGiveUp(void) {
	if (m_pRule)
		return m_pRule->CheckGiveUp();

	return false;
}

// golfrule.cpp:3641
void CGolfRule::RecordTeamData(unsigned char team) {
	TEAM(team)->m_totalScore += TEAM(team)->m_holes[GOLFDOC()->m_currentHole - 1].GetScore();
	TEAM(team)->m_state = 1;
}

// golfrule.cpp:4010
void CGolfRule::RecordScoreByPlayer(unsigned char player) {
	if (player < GOLFDOC()->GetPlayerNum()) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1)
			RecordTeamData(GOLFDOC()->GetPlayer(player)->m_attribute);

		RecordPlayerData(player);

		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 && GOLFDOC()->GetPlayer(player)->m_partnerIndex != 0xFF)
			RecordPlayerData(GOLFDOC()->GetPlayer(player)->m_partnerIndex);

		SendHoleStat();
	}
}

// golfrule.cpp:4045
void CGolfRule::RecordScoreByTeam(unsigned char team) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 && team < 2) {
		for (int i = 0; i < 4; i++) {
			if (GOLFDOC()->GetPlayer(i)->m_attribute == team) {
				RecordScoreByPlayer(i);
				return;
			}
		}
	}
}

// golfrule.cpp:4128
unsigned long CGolfRule::GetHolePang(unsigned char hole) {
	CSharedDoc *sharedDoc = WSingleton<CSharedDoc>::Instance();
	float channelRate;

	if (sharedDoc->m_currentChannel.flags & 0x800)
		channelRate = 1.0f;
	else if (sharedDoc->m_currentChannel.flags & 0x1000)
		channelRate = 2.5f;
	else if (sharedDoc->m_currentChannel.flags & 0x2000)
		channelRate = 5.0f;
	else
		channelRate = 1.0f;

	int step = (hole - 1) / 3;

	float pang = channelRate * (step * 0.5f + 1.0f) * 20.0f;

	if (hole == sharedDoc->m_holes)
		pang += pang;

	return (unsigned long)pang;
}

// golfrule.cpp:4387
void CGolfRule::DoWaitForStart(float elapsed) {
	FlyBy(elapsed);

	m_waitingForStart = true;

	if (GOLFDOC()->m_currentHole != GetStartHole()) {
		ChangeGameMode(2);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.size() == 1) {
		ReturnLobby(false);
	}
}

// golfrule.cpp:4487
void CGolfRule::DoCloseupCaddie(float elapsed) {
	if (m_modeTime < 3.0f) {
		CCameraWork::Instance()->Process(elapsed, 0.0f);
	} else {
		CMouseCursor::Instance()->Reset(true);
		SendMsg(this, "Clock", 0x1E9, 0, 0, 0, 0);
		g_audio->PlaySfx("play_my_turn", 0, 0, 0, 0, 0.5f, 200.0f);
		ChangeGameMode(8);
	}
}

// golfrule.cpp:4501
void CGolfRule::ReadyForNextTurn(void) {
	CMouseCursor::Instance()->Reset(true);

	SendMsg(this, "Clock", 0x1E9, 0, 0, 0, 0);

	ChangeTurn();

	GOLFDOC()->MakeDefaultCamera(true);
	WPuppet *ball = CSceneManager::Instance()->GetPuppet("\xB0\xF8");

	if (ball && ball->GetRootBone())
		ball->SetAlpha(0xFF);
	g_audio->StopSfx("shot_dist_count");
	g_audio->PlaySfx("play_my_turn", 0, 0, 0, 0, 0.5f, 200.0f);
	CCameraWork::Instance()->StopBallSound();
	ChangeGameMode(8);
}

// golfrule.cpp:6190
// FIRST PASS IN PROGRESS (23 KB, golfrule.cpp:6190-8679): the prologue, the
// mode switch and the short cases are written; each empty case names the
// source lines still to write.
void CGolfRule::ChangeGameMode(unsigned long mode) {
	unsigned long oldMode = GOLFDOC()->GameMode();

	if (oldMode == 0x200 && GOLFDOC()->m_dropPending) {
		if (mode == 0x4000) {
			GOLFDOC()->m_dropPending = false;

			if (m_dropDlg != NULL)
				m_dropDlg->Close(FrNONE, false);
		} else {
			if (m_dropDlg == NULL) {
				m_dropDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
				m_dropDlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::CancelDropDlgResult, 0);
				// "Cancel the ball moving mode."
				m_dropDlg->SetMessage("\xB0\xF8 \xBF\xC5\xB1\xE2\xB1\xE2 \xB8\xF0\xB5\xE5\xB8\xA6 \xC3\xEB\xBC\xD2\xC7\xD5\xB4\xCF\xB4\xD9.", false);
				m_dropDlg->EnableDrag(false);
			}

			return;
		}
	}

	GOLFDOC()->GameMode() = mode;
	m_modeTime = 0.0f;

	CGolfBall &ball = GolfBall();

	switch (mode) {
	case 0x200000:
		// golfrule.cpp:6224-6235 not yet written (Random clip, SetClip, PLAYER_%d visibility, "Score" 0x13B)
		break;
	case 0x40000:
		GOLFDOC()->GetUIAlpha() = 0x80;
		CChatMsg::Instance()->ReplaceBackupUiAlpha();
		break;
	case 1:
		// golfrule.cpp:6245-6279 not yet written
		break;
	case 0x10000:
		// golfrule.cpp:6280-6328 not yet written
		break;
	case 0x100000:
		// golfrule.cpp:6329-6419 not yet written
		break;
	case 2:
		// golfrule.cpp:6420-6650 not yet written
		break;
	case 4:
		// golfrule.cpp:6651-6675 not yet written
		break;
	case 8:
		// golfrule.cpp:6676-6816 not yet written
		break;
	case 0x10:
		// golfrule.cpp:6817-6850 not yet written
		break;
	case 0x20:
		// golfrule.cpp:6851-6866 not yet written (members 0x267, 0x3BC-0x3C4; CCapturedBg byte 0x28)
		break;
	case 0x100:
		GOLFDOC()->GetUIAlpha() = 0x80;
		CChatMsg::Instance()->ReplaceBackupUiAlpha();
		break;
	case 0x400:
		// golfrule.cpp:6874-6909 not yet written
		break;
	case 0x800:
		// golfrule.cpp:6910-6933 not yet written
		break;
	case 0x200:
		// golfrule.cpp:6934-6991 not yet written
		break;
	case 0x40:
		// golfrule.cpp:6992-7946 not yet written (calls GetShotMotionGrade at :7707 and :7803)
		break;
	case 0x80:
		// golfrule.cpp:7947-8297 not yet written
		break;
	case 0x2000:
		// golfrule.cpp:8298-8382 not yet written
		break;
	case 0x400000:
		// golfrule.cpp:8383-8399 not yet written
		break;
	case 0x1000:
		// golfrule.cpp:8400-8465 not yet written
		break;
	case 0x4000:
		// golfrule.cpp:8466-8534 not yet written
		break;
	case 0x20000:
		// golfrule.cpp:8535-8616 not yet written
		break;
	case 0x80000:
		// golfrule.cpp:8617-8644 not yet written
		break;
	case 0x800000:
		// golfrule.cpp:8645-8678 not yet written
		break;
	}
	(void)oldMode;
	(void)ball;
}

// golfrule.cpp:8682
// A random variant name of a motion: "<name>_01" to "<name>_04".
static char *__stdcall MakeRandomMotionName(const char *name) {
	return MakeStr("%s_%02d", name, rand() % 4 + 1);
}

// golfrule.cpp:11169
void CGolfRule::DoGetReadyToCaptureBG(float) {
	if (CCapturedBg::Instance()->IsCaptured()) {
		if (COption::Instance()->dGetClickSystem()) {
			if ((g_input->GetButton(LEFT_BUTTON) == 2) ^ g_input->GetUp("\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9")) {
				CancelShot();
				return;
			}
		}

		ChangeGameMode(0x20);
	}
}

// golfrule.cpp:11491
void CGolfRule::ToggleCommandBtns(bool show) {
	if (m_commandBtns[0])
		m_commandBtns[0]->SetVisible(show);

	if (m_commandBtns[1])
		m_commandBtns[1]->SetVisible(show);

	if (m_commandBtns[2])
		m_commandBtns[2]->SetVisible(show);

	if (m_commandBtns[3])
		m_commandBtns[3]->SetVisible(show);
}

// golfrule.cpp:12413
void CGolfRule::DoNavigateCamera(float) {
	if (g_input->GetDown("\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9", true)) {
		CMouseCursor::Instance()->Store();
		ChangeGameMode(8);
		SendMsg(this, "Camera", 0xF0, 0, 0, 0, 0);
	}

	if (TopViewClick())
		ChangeGameMode(0x200);
}

// golfrule.cpp:12462
bool CGolfRule::StandViewClick(void) {
	return g_input->GetDown("�����̽���", true) ? true : false;
}

// golfrule.cpp:12677
void CGolfRule::SetNetMsgWait(bool send, int type, char *msg, float timeout) {
	if (send) {
		m_netMsgWaitSend = true;
		m_netMsgWaitType = type;
		m_netMsgWaitTimeout = timeout;
		m_netMsgWait = msg;
		m_netMsgSendTime = GetTickCount();

		g_iSendPacketGR = g_iSendPacketType;
		g_iRecvPacketGR = 0;
		g_iRecvPacketType = 0;
	} else {
		m_netMsgWaitSend = false;
		m_netMsgWaitRecv = msg;
		m_netMsgRecvTime = GetTickCount();

		g_iRecvPacketGR = g_iRecvPacketType;
	}
}

// golfrule.cpp:12706
void CGolfRule::CheckNetMsgWait_Send(char *msg) {
	if (m_netMsgWait && !_strcmpi(m_netMsgWait, msg)) {
		g_iSendPacketGR = g_iSendPacketType;
	}
}

// golfrule.cpp:12714
void CGolfRule::CheckNetMsgWait_Recv(char *msg) {
	if (m_netMsgWait && !_strcmpi(m_netMsgWait, msg)) {
		g_iRecvPacketGR = g_iRecvPacketType;
	}
}

// golfrule.cpp:13531
void CGolfRule::SetAngle(float angle) {
	SendMsg(this, "Camera", 0x2718, (intptr_t)&angle, 0, 0, 0);
	SendMsg(this, "Spot", 0x2718, (intptr_t)&angle, 0, 0, 0);
	SendMsg(this, "Screen", 0x2718, (intptr_t)&angle, 0, 0, 0);
	SendMsg(this, "Grid", 0x2718, (intptr_t)&angle, 0, 0, 0);
	SendMsg(this, "Caddie", 0x2718, (intptr_t)&angle, 0, 0, 0);
}

// golfrule.cpp:13540
WMatrix CGolfRule::BankCamera(float angle) {
	return RotMat(angle, 2) * g_camera;
}

// golfrule.cpp:13859
void CGolfRule::Warp(const WVector &pos) {
	int meshIndex = GetPVS().GetMeshIndex(pos, true);

	if (meshIndex >= 0 && !GOLFDOC()->m_pPolySoup->IsOutOfBound(pos)) {
		PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos = pos;
		PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel = 0;
		GolfBall().m_pos = pos;
		GolfBall().m_groundArea = meshIndex;

		SetEnvironmentVariableA();
		SetTurnChangeEvent();
		ReadyForNextTurn();
	}
}

// golfrule.cpp:14066
void CGolfRule::AllReplay(void) {
	m_replayFlags &= ~4;
	Replay(false);
}

// golfrule.cpp:14644
void CGolfRule::QuickSave(const char *name) {
	FILE *fp = fopen(MakeStr("%s.sav", name), "wb");

	unsigned char map = GetCurMap();
	fwrite(&map, 1, 1, fp);
	fwrite(&GOLFDOC()->m_currentHole, 1, 1, fp);

	unsigned char playerNum = GOLFDOC()->GetPlayerNum();
	fwrite(&playerNum, 1, 1, fp);
	fwrite(GOLFDOC()->GetPlayer(0), 0xB98, playerNum, fp);
	fwrite(&GOLFDOC()->m_myPlayerIndex, 1, 1, fp);

	fclose(fp);
}

// golfrule.cpp:15229
void CGolfRule::GetWinningPrize(int prize) {
	AddPrize(prize);
	AddExtPrize();
}

// golfrule.cpp:15526
void CGolfRule::CancelShot(void) {
	timeEndPeriod(1);
	CFx::Instance()->SetActive(true);

	if (OnlinePlay() && IsMyTurn(false)) {
		struct {
			unsigned char type;
			float startPosition;
		} __declspec(align(1)) data;
		data.type = 0;
		data.startPosition = GOLFDOC()->GetBar().m_startPosition;

		WSendPacket packet((enumClientPacket)0x14);
		packet.EncodeBuffer(&data, 5);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_shotReady = false;
	GOLFDOC()->m_clockHold = 0;
	GOLFDOC()->GetBar().m_accuracy = GolfClub().GetType() == 3 ? 140.0f : 105.0f;
	GOLFDOC()->GetBar().m_prevAccuracy = GOLFDOC()->GetBar().m_accuracy;
	GOLFDOC()->GetBar().m_power = 140.0f;
	GOLFDOC()->GetBar().m_state = 0;
	GOLFDOC()->GetBar().m_stopped = false;
	CMouseCursor::Instance()->SetActive(true);

	if (CCapturedBg::Instance() != NULL)
		CCapturedBg::Instance()->Reset();

	SendMsg(this, "Camera", 0x19A, 0, 0, 0, 0);
	GOLFDOC()->GameMode() = 8;

	if (m_chatViewWnd != NULL)
		m_chatViewWnd->SetVisible(true);
}

// golfrule.cpp:15578
void CGolfRule::SaveErrorFile(void) {
	if (IsMyTurn(false)) {
		mkdir("error");
		int index = 0;

		do {
			sprintf(m_saveFileName, "error/%s_%03d.rep", GetMyComputerName().c_str(), index++);
		} while (_access(m_saveFileName, 0) != -1);

		SaveShot(m_saveFileName);
	}
}

// golfrule.cpp:15704
// Leaving the round early: a web login quits the client; otherwise the
// player's game pang is kept for the lobby (0 without a player).
bool CGolfRule::ReturnToLobbyDlgResult(int, FrForm *) {
	if (CLoginInfo::Instance()->IsWebLogin()) {
		g_bQuit = true;
	} else {
		if (PLAYER(MyIndex()))
			CSharedDoc::Instance()->m_quitGamePang = PLAYER(MyIndex())->m_ghostAward;
		else
			CSharedDoc::Instance()->m_quitGamePang = 0;

		ReturnLobby(true);
	}

	return true;
}

// golfrule.cpp:15720
void CGolfRule::ToggleGameUI(void) {
	SendMsg(this, "Screen", 0x190, m_showGameUI ? 1 : 0, 0, 0, 0);
	SendMsg(this, "Letter", 0x190, m_showGameUI ? 1 : 0, 0, 0, 0);

	SendMsg(this, "VideoRecorder", 0x190, m_showGameUI ? 1 : 0, 0, 0, 0);

	if (g_pFresh)
		g_pFresh->Toggle(m_showGameUI);

	CMouseCursor::Instance()->Toggle(m_showGameUI);
	CChatMsg::Instance()->Toggle(m_showGameUI);
}

// golfrule.cpp:15739
bool CGolfRule::CheckAllHoleOut(void) {
	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (GOLFDOC()->GetPlayer(i)->m_state == 0)
			return false;
	}
	return true;
}

// golfrule.cpp:15750
bool CGolfRule::PauseDlgResult(int result, FrForm *) {
	CMouseCursor::Instance()->m_uiEnable[3] = true;

	m_pauseDlg = NULL;

	if (OnlinePlay()) {
		if (result) {
			WSendPacket packet((enumClientPacket)0x30);
			packet.Encode1(0);
			packet.Send(SEND_TO_DEFAULT);
		}
	} else {
		GOLFDOC()->m_pause = false;
	}

	return true;
}

// golfrule.cpp:15827
bool CGolfRule::CancelDropDlgResult(int result, FrForm *) {
	m_dropDlg = NULL;
	if (result == 1) {
		GOLFDOC()->m_dropPending = false;
		ChangeGameMode(8);
	}
	return true;
}

// golfrule.cpp:15840
bool CGolfRule::GoStopDlgResult(int result, FrForm *) {
	m_goStopDlg = NULL;
	if (result) {
		WSendPacket packet((enumClientPacket)0x36);
		packet.Encode1(result == 1);
		packet.Send(SEND_TO_DEFAULT);

		GOLFDOC()->m_clockStopped = false;
		m_goStopSent = true;
		SetNetMsgWait(true, 0x3DEB, "GoStopDlgResult", 20.0f);
	}

	return true;
}

// golfrule.cpp:15909
bool CGolfRule::OnReportResult(int result, FrForm *) {
	if (result == 1) {
		GOLFDOC()->SetReport(true);

		WSendPacket packet((enumClientPacket)0x3A);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_reportDlg = NULL;
	return true;
}

// golfrule.cpp:15923
bool CGolfRule::OnReturnLobbyResult(int, FrForm *) {
	ReturnLobby(false);
	return true;
}

// golfrule.cpp:15929
bool CGolfRule::OnForgetDlgResult(int result, FrForm *) {
	if (result == 1) {
		SendMsg(this, "Item", 0x19E, m_cancelShotItemSlot, 0, 0, 0);
		SetNetMsgWait(true, 0x3E3D, "PM_NET_CANCEL_SHOT", 20.0f);
	} else if (result == 2) {
		SendShotResult();
		SendMsg(this, "Item", 0x195, 0, 0, 0, 0);
		SendMsg(this, "Screen", 0x1AF, 0, 0, 0, 0);
	}

	m_forgetPending = false;
	m_forgetElapsed = 0;
	m_forgetDlg = NULL;

	return true;
}

// golfrule.cpp:15952
bool CGolfRule::OnPlayerInfoDlgResult(int, FrForm *) {
	m_playerInfoDlg = NULL;
	return true;
}

// golfrule.cpp:16009
bool CGolfRule::OnApproachDlgResult(int, FrForm *) {
	if (m_approachResultDlg) {
		if (m_approachResultDlg->GetApproachEnd()) {
			if (m_approachEndDlg == NULL) {
				m_approachEndDlg = CreateForm<FrApproachEndDlg>(g_pFresh->GetManager(), this, "approach_end", 0);
				m_approachEndDlg->EnableDrag(false);

				float w = m_approachEndDlg->GetRect().w;
				WPoint pt(g_view->GetWidth() * 0.5f - w * 0.5f, 35.0f);
				m_approachEndDlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnApproachEndDlgResult, pt, 0);
				m_approachEndDlg->SetTimeLimit(15.0f);
				// "Approach final result"
				m_approachEndDlg->SetCaption("\xBE\xEE\xC7\xC1\xB7\xCE\xC4\xA1 \xC3\xD6\xC1\xBE\xB0\xE1\xB0\xFA");
			}
		} else {
			ChangeGameMode(0x800000);
		}

		m_approachResultDlg = NULL;
	}

	return true;
}

// golfrule.cpp:16177
bool CGolfRule::CanCancelShot(void) {
	for (int i = 0; i < 10; i++) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_equipItem[i] == 0x1800000E) {
			m_cancelShotItemSlot = i;
			return true;
		}
	}
	return false;
}

// golfrule.cpp:16321
void CGolfRule::ResetUiPos_Board(void) {
	WRect rect = m_boardWnd->GetRect();
	rect.x = g_view->GetWidth() - 140.0f;
	m_boardWnd->SetRect(rect);

	rect = m_onelineBoardWnd->GetRect();
	rect.x = g_view->GetWidth() - 317.0f;
	m_onelineBoardWnd->SetRect(rect);
	COneLineBoard::Instance()->SetRect(m_onelineBoardWnd->GetRect());
}

// golfrule.cpp:16333
void CGolfRule::PlayBGM(void) {
	if (GolfBall().m_groundType == 2)
		SetConstantBGM(g_bgmPutting[rand() % 2], false, true);
	else
		SetConstantBGM(m_bgmList[m_bgmIndex].c_str(), false, true);
}

// golfrule.cpp:16354
int CGolfRule::HoleReplay2(void) {
	return 0;
}

// golfrule.cpp:16381
void CGolfRule::OnMessengerBtnUp(void) {
	CMessengerInfo::Instance()->Open(true);

	OnBarHoverOff(0);
}

// golfrule.cpp:16424
void CGolfRule::OnMessenger_AlarmBtnUp(void) {
	CMessengerInfo::Instance()->AlarmBtnUp(0xFFFFFFFF);
}

// golfrule.cpp:16430
void CGolfRule::OnMessenger_GiftAlarmBtnInit(int param) {
	m_messengerGiftAlarmBtn = DYNAMIC_CAST(FrButton, param);
	if (m_messengerGiftAlarmBtn) {
		m_messengerGiftAlarmBtn->SetVisible(false);
		m_messengerGiftAlarmBtn->SetTopmost(false);

		WPoint pt;
		pt.x = g_view->GetWidth() - 75.0f;
		pt.y = m_messengerGiftAlarmBtn->GetRect().y;
		m_messengerGiftAlarmBtn->MoveWindow(pt);
	}
}

// golfrule.cpp:16445
void CGolfRule::OnMessenger_GiftAlarmBtnUp(void) {
	m_messengerGiftAlarmBtn->SetVisible(false);
	m_messengerGiftAlarmBtn->SetTopmost(false);

	FrGiftAlarmDlg *dlg = CreateForm<FrGiftAlarmDlg>(g_pFresh->GetManager(), this, "gift_alarm", 0);
	dlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnNotifyDlgResult, 0x11);
	dlg->SetTopmost(true);
	dlg->FindNextTopFocus(false);
	dlg->SetIconRect(m_messengerGiftAlarmBtn->GetRect());
}

// golfrule.cpp:16458
void CGolfRule::OnReplayStatusInit(int param) {
	m_replayBoxWnd = DYNAMIC_CAST(FrArea, param);

	if (!IsLocalContent((localContentType_t)0x4F))
		m_replayBoxWnd->SetVisible(false);

	if (m_replayBoxWnd) {
		WRect rect = m_replayBoxWnd->GetRect();
		rect.x = g_view->GetWidth() - (800.0f - rect.x);
		m_replayBoxWnd->SetRect(rect);
	}
}

// golfrule.cpp:16547
bool CGolfRule::IsMaxWithHGParts(unsigned char index) {
	// The four parts of the HG set.
	static const unsigned long s_hgParts[] = {
		0x08104011, 0x0810A00C, 0x08100008, 0x08110152
	};

	CPartTidList *partList = &GOLFDOC()->GetPartTidList(index);

	if (partList == NULL)
		return false;

	bool equipped = false;
	for (int i = 0; i < sizeof(s_hgParts) / sizeof(s_hgParts[0]); i++) {
		if (partList->IsEquipped(s_hgParts[i], 0)) {
			equipped = true;
			break;
		}
	}

	return equipped;
}

// golfrule.cpp:16582
void CGolfRule::_ImeReset(void) {
	if (IsLocalContent((localContentType_t)0x2A)) {
		if (!CChatMsg::Instance()->IsActive()) {
			g_ime->SetActive(true);
			g_ime->SetActive(false);
			g_ime->Reset();
		}
	}
}

// golfrule.cpp:16596
void CGolfRule::ReplayBoxOpen(void) {
	if (!m_replayBox) {
		float x = 570.0f - (800.0f - g_view->GetWidth());

		m_replayBox = CreateForm<FrReplayControlDlg>(g_pFresh->GetManager(), this, "replaycontroldlg", 0);
		m_replayBox->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnReplayModeDlgResult, WPoint(x, 55.0f), 0x11);
	} else {
		m_replayBox->SetVisible(true);
	}

	if (m_replayBoxWnd)
		m_replayBoxWnd->SetVisible(false);
}

// golfrule.cpp:16616
void CGolfRule::ReplayBoxClose(void) {
	if (m_replayBox) {
		m_replayBox->Close(true);
	}
	if (m_replayBoxWnd) {
		m_replayBoxWnd->SetVisible(true);
	}
}

// golfrule.cpp:16627
bool CGolfRule::OnReplayModeDlgResult(int, FrForm *) {
	if (WSingleton<CSharedDoc>::Instance()->m_playingMode == 2 && WSingleton<CSharedDoc>::Instance()->m_replayState == 5) {
		WSingleton<CSharedDoc>::Instance()->m_replayState = 0;

		WSingleton<CSharedDoc>::Instance()->SetRmrMode(1);
		WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID = MyUID();
		WSingleton<CSharedDoc>::Instance()->m_playingMode = (eReplayModeType)3;

		SetBGMNone();
		CProjectG::Instance()->FromGolfToLobbySettings();
		CTaskMain::OnUnderBar_MyRoomUp();

		CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0xC6, 2, 0, 0, 0);
	}

	m_replayBox = NULL;
	return true;
}

// golfrule.cpp:16773
void CGolfRule::OnRecordAviBtnUp(void) {
	if (m_recordAviDlg == NULL) {
		char *installDir = GetPangyaInstallDic();
		char text[255] = "";
		m_recordAviDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);

		// "Save the replay as a video file?  Save path: %s/replay"
		sprintf(text, "\xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC \xC5\xD7\xC0\xD9\xC0\xC7 \xB3\xBB\xBF\xEB\xC0\xBB\n\xB5\xBF\xBF\xB5\xBB\xF3 \xC6\xC4\xC0\xCF\xB7\xCE \xC0\xFA\xC0\xE5\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?\n\n\t\t\t\t\t\t   \xC0\xFA\xC0\xE5 \xB0\xE6\xB7\xCE\n\t\t\t\t\t\t   %s/replay", installDir);
		m_recordAviDlg->SetMessage(text, false);
		m_recordAviDlg->Open((FRESH_RESULT_CALLBACK)&CGolfRule::OnRecordAviResult, 0);
	}
}

// golfrule.cpp:16788
bool CGolfRule::OnRecordAviResult(int result, FrForm *) {
	if (result == 1)
		WSingleton<CSharedDoc>::Instance()->SetReplayState(9);

	m_recordAviDlg = NULL;
	return true;
}

// golfrule.cpp:16803
char *CGolfRule::GetPangyaInstallDic(void) {
	static char s_installDir[MAX_PATH];

	CRegistry registry;
	registry.SetRootKey(HKEY_LOCAL_MACHINE);
	registry.SetKey("Software\\Ntreev\\Pangya_QA", TRUE);

	WTL::CString installDir = "";
	installDir = registry.ReadString("Install_Dir", "");
	sprintf(s_installDir, "%s", (const char *)installDir);

	return s_installDir;
}

// golfrule.cpp:17182
bool CGolfRule::IsLastShotRelplay(void) {
	return m_replayMode == 2 ? true : false;
}

// golfrule.cpp:17267
void CGolfRule::CollectBallStopData(void) {
	int playType = CSharedDoc::Instance()->GetReplayPlayType();
	CSharedDoc::Instance()->SetReplayPlayType(2);
	if (ShotDataSave(ball_stop_replay_data_name, false) == 1) {
		NetResourceManager::Instance()->UploadBugReportData(ball_stop_replay_data_name, 1, (GolfBall().m_eventPreDataIndex[7] != -1) + 1, true);
	}
	CSharedDoc::Instance()->SetReplayPlayType(playType);
}

// wutil.h:218: the original holds the out-of-line Min<float> in GolfRule's object.
template float Min<float>(float a, float b);
