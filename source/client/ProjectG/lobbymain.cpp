// Original: d:\build\custom\temp\code\client\projectg\lobbymain.cpp
#define LOBBYMAIN_SHAREDDOC_INLINE
#include "NetworkSystem.h"
#include "SoundManager.h"
#include <windows.h>
#include <shellapi.h>
#include "ClientSetting.h"
#include "LobbyMain.h"
#include "EventPrizeDlg.h"
#include "Localize.h"
#include "TopIconManager.h"
#include "logininfo.h"
#include "frbutton.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "CreateNickDlg.h"
#include "buddymanager.h"
#include "mousecursor.h"
#include "frarea.h"
#include "TaskManager.h"
#include "globalgamedefine.h"
#include "../shared/globalgamedefine.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "frlistbox.h"
#include "LoginDlg.h"
#include "EquipDlg.h"
#include "MakeRoomDlg.h"
#include "exhibition.h"
#include "noticedlg.h"
#include "s5/SkinnedTheme.h"
#include "playerdlg.h"
#include "messengerdlg.h"
#include "frcombobox.h"
#include "Intrusion.h"
#include "user_info.h"
#include "frviewer.h"
#include "fredit.h"
#include "frstatic.h"
#include "frgaugebar.h"
#include "frframe.h"
#include "ServerDlg.h"
#include "UserListDlg.h"
#include "MailBoxDlg.h"
#include "ParanNickDlg.h"
#include "GuildNoticeDlg.h"
#include "NewsReader.h"
#include "winput.h"
#include "icontentsdoc.h"
#include "contentsdoc.h"
#include "ScoreDlg.h"
#include "DetailedRoomInfoDlg.h"
#include "CaddieWarningDlg.h"
#include "frscrollbar.h"
#include "frdesktop.h"
#include "ToolTip.h"
#include "OptionDlg.h"
#include "ProjectG.h"
#include "GolfDoc.h"
#include "polysoup.h"
#include "wview.h"
#include "cardsystem.h"
#include "itembuffactor.h"
#include "ValentineEventDlg.h"
#include "bitmap.h"
#include "WorldTourEventDlg.h"
#include "FrFindEbortEventDlg.h"
#include "FrHalloweenDlg.h"
#include "ChristamsEventDlg.h"
#include "FrTicketExchangeDlg.h"
#include "gamedatadb.h"
#include "gdbitmapbox.h"
#include "InputManager.h"
#include "ResultDlg.h"
#include "TreasureDlg.h"
#include "Treasure.h"
#include "RankingDlg.h"
#include "OnelineReqDlg.h"
#include "ChatMsg.h"
#include "UccManager.h"
#include "NetResourceManager.h"
#include "../shared/s5/SharedUtilities.h"
#include <algorithm>
#include "pg_boot_todo.h"

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev *g_ime;
// The mouse input device (public ?g_mouse@@3PAVWInputDev@@A).
extern WInputDev *g_mouse;
// The camera matrix of the 3D view (public ?g_camera@@3VWMatrix@@A).
extern WMatrix g_camera;

// The room list's game type names: VS [0..1], Mass [2..4], Battle [5..6]
// (public ?g_strGameType@@3PAV?$basic_string@...@A, defined elsewhere).
extern std::string g_strGameType[];

extern bool g_bQuit;

// Unnamed data; its fourth int is the room list selection the category
// tabs reset to -1.
extern "C" int data_aa9eac[];

// Unnamed data: the IME mode (alphanumeric) the chat language mark shows.
extern "C" bool data_ab2830;

// clientsetting.h:404
int COption::gGetChatWndUp(void) {
	return m_option.m_chatWndUp;
}

// clientsetting.h:405
void COption::gSetChatWndUp(int up) {
	m_option.m_chatWndUp = up;
}

// eventprizedlg.h:11
FrEventPrizeDlg::FrEventPrizeDlg(void) {
	m_cancelBtn = 0;
	m_viewBtn = 0;
}

// eventprizedlg.h:13
void FrEventPrizeDlg::SetEvent(unsigned char event) {
	m_event = event;
}

// eventprizedlg.h:14
unsigned char FrEventPrizeDlg::GetEvent(void) const {
	return m_event;
}

// topiconmanager.h:77
void CIconManager::AddTopIconFromOutSide(void) {
	m_outSideIconCount++;
}

// topiconmanager.h:78
void CIconManager::ClearInformation(void) {
	m_outSideIconCount = 0;
}

// Top-page event icon slot (values only known by number so far).
enum eTopIconID {
	TOPICON_0 = 0
};

// One icon button of the top page, placed by its slot.
class cTopEventIcon {
public:
	cTopEventIcon(eTopIconID id);
	void Prepare(FrButton* button);

private:
	eTopIconID m_id;    // 0x0
};

// lobbymain.cpp:185
cTopEventIcon::cTopEventIcon(eTopIconID id) {
	m_id = id;
}

// lobbymain.cpp:187
void cTopEventIcon::Prepare(FrButton* button) {
	if (button != 0) {
		button->SetPushSound("ui_desktop_icon_click");
		button->EnableHover(true, 0.5f);
		int slot = CLoginInfo::Instance()->IsPcBang() ? m_id : m_id - 1;
		button->SetRect(WRect((float)(slot * 80) + 1.0f, 50.0f, 80.0f, 80.0f));
	}
}

// lobbymain.cpp:217
// Room list category names, indexed like the category tabs (VS, Mass,
// Battle, Chat, All).
std::string g_strCate[5] = {
	"\xB4\xEB\xC0\xFC",
	"\xB4\xEB\xC8\xB8",
	"\xB9\xE8\xC6\xB2",
	"\xB4\xEB\xC8\xAD",
	"\xB8\xF0\xB5\xCE"
};

// lobbymain.cpp:241
int GetCateByGameType(int type) {
	switch (type) {
	case 4:
		return 1;
	case 5:
		return 1;
	case 6:
		return 1;
	case 7:
		return 2;
	case 9:
		return 2;
	case 10:
		return 2;
	case 2:
		return 3;
	case 0:
		return 0;
	case 1:
		return 0;
	case 3:
		return 0;
	case 8:
		return 0;
	}
	return 0;
}

// lobbymain.cpp:259
long GetShotTimeLimitByGameType(int type) {
	switch (type) {
	case 0:
		return 40000;
	case 1:
		return 40000;
	case 2:
		return 0;
	case 3:
		return 40000;
	case 4:
		return 0;
	case 5:
		return 0;
	case 6:
		return 0;
	case 7:
		return 40000;
	case 8:
		return 40000;
	case 9:
		return 40000;
	case 10:
		return 40000;
	}
	return 40000;
}

// lobbymain.cpp:277
long GetGameTimeLimitByGameType(int type) {
	switch (type) {
	case 0:
		return 0;
	case 1:
		return 0;
	case 2:
		return 0;
	case 3:
		return 0;
	case 4:
		return 2400000;
	case 5:
		return 2400000;
	case 6:
		return 2400000;
	case 7:
		return 0;
	case 8:
		return 0;
	case 9:
		return 40000;
	case 10:
		return 40000;
	}
	return 0;
}

// lobbymain.cpp:296
unsigned char GetRandGameTypeByCate(int cate) {
	switch (cate) {
	case 0:
		return 0;
	case 1:
		return 4;
	case 2:
		return 10;
	case 3:
		return 2;
	case 4:
		return 0;
	}
	return 0;
}

// lobbymain.cpp:393 (run-time type and factory registration of CLobbyMain)
const WRTTI CLobbyMain::m_RTTI("CLobbyMain", &CTaskMain::m_RTTI);

IObject* CLobbyMainMakeInstance(void) {
	return new CLobbyMain;
}

struct __impCLobbyMain {
	__impCLobbyMain(void) {
		ObjectFactory().AddObjectFunctor(CLobbyMainMakeInstance, "CLobbyMain");
		ActorFactory().AddActorClass("CLobbyMain");
	}
};
static __impCLobbyMain s_impCLobbyMain;

// lobbymain.cpp:394
const sFRESH_MSGMAP* CLobbyMain::GetMessageMap(void) const {
	return &CLobbyMain::_MsgMap;
}

sFRESH_MSGMAP CLobbyMain::_MsgMap = { &CTaskMain::_MsgMap, &CLobbyMain::_MsgEntries[0] };

sFRESH_ENTRY CLobbyMain::_MsgEntries[] = {
	// lobbymain.cpp:408
	{ "LOGIN", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnLogin_Init },
	// lobbymain.cpp:409
	{ "LOGIN", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnLogin_Finish },
	// lobbymain.cpp:399
	{ "BLANK", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnBlank_Init },
	// lobbymain.cpp:400
	{ "BLANK", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnBlank_Finish },
	// lobbymain.cpp:403
	{ "SECOND_PWD", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnSecond_Pwd_Init },
	// lobbymain.cpp:404
	{ "SECOND_PWD", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnSecond_Pwd_Finish },
	// lobbymain.cpp:408
	{ "TOPPAGE", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_Init },
	// lobbymain.cpp:409
	{ "TOPPAGE", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_Finish },
	// lobbymain.cpp:419
	{ "TOPPAGE", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_Destroy },
	// lobbymain.cpp:420
	{ "TOPPAGE.gameplay", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_GameInit },
	// lobbymain.cpp:412
	{ "TOPPAGE.gameplay", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_GameUp },
	// lobbymain.cpp:413
	{ "TOPPAGE.family", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_FamilyInit },
	// lobbymain.cpp:414
	{ "TOPPAGE.family", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_FamilyUp },
	// lobbymain.cpp:415
	{ "TOPPAGE.pangyastudy", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_PangysStudyInit },
	// lobbymain.cpp:416
	{ "TOPPAGE.pangyastudy", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_PangysStudyUp },
	// lobbymain.cpp:419
	{ "TOPPAGE.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_NoticeInit },
	// lobbymain.cpp:420
	{ "TOPPAGE.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_NoticeUp },
	// lobbymain.cpp:421
	{ "TOPPAGE.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_ServerInit },
	// lobbymain.cpp:422
	{ "TOPPAGE.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_ServerUp },
	// lobbymain.cpp:423
	{ "TOPPAGE.pcbang", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_PcbangInit },
	// lobbymain.cpp:424
	{ "TOPPAGE.pcbang", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_PcbangUp },
	// lobbymain.cpp:425
	{ "TOPPAGE.rental_expired", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_RentalExpiredAlramInit },
	// lobbymain.cpp:426
	{ "TOPPAGE.rental_expired", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_RentalExpiredAlramUp },
	// lobbymain.cpp:427
	{ "TOPPAGE.event1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_Event1Init },
	// lobbymain.cpp:428
	{ "TOPPAGE.event1", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_Event1Up },
	// lobbymain.cpp:430
	{ "TOPPAGE.new_2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_NewInit },
	// lobbymain.cpp:431
	{ "TOPPAGE.new_2", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_NewOwnerDraw },
	// lobbymain.cpp:451
	{ "TOPPAGE.TopIcon1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon1Init },
	// lobbymain.cpp:452
	{ "TOPPAGE.TopIcon1", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon1Up },
	// lobbymain.cpp:454
	{ "TOPPAGE.TopIcon2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon2Init },
	// lobbymain.cpp:455
	{ "TOPPAGE.TopIcon2", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon2Up },
	// lobbymain.cpp:457
	{ "TOPPAGE.TopIcon3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon3Init },
	// lobbymain.cpp:458
	{ "TOPPAGE.TopIcon3", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon3Up },
	// lobbymain.cpp:460
	{ "TOPPAGE.TopIcon4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon4Init },
	// lobbymain.cpp:461
	{ "TOPPAGE.TopIcon4", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon4Up },
	// lobbymain.cpp:463
	{ "TOPPAGE.TopIcon5", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon5Init },
	// lobbymain.cpp:464
	{ "TOPPAGE.TopIcon5", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon5Up },
	// lobbymain.cpp:466
	{ "TOPPAGE.TopIcon6", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TopIcon6Init },
	// lobbymain.cpp:467
	{ "TOPPAGE.TopIcon6", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TopIcon6Up },
	// lobbymain.cpp:476
	{ "TOPPAGE.pointevent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_PointEventInit },
	// lobbymain.cpp:477
	{ "TOPPAGE.pointevent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_PointEventDown },
	// lobbymain.cpp:480
	{ "TOPPAGE.valevent_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_ValEventInit },
	// lobbymain.cpp:481
	{ "TOPPAGE.valevent_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_ValEventDown },
	// lobbymain.cpp:484
	{ "TOPPAGE.worldtourevent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_WorldTourEventInit },
	// lobbymain.cpp:485
	{ "TOPPAGE.worldtourevent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_WorldTourEventDown },
	// lobbymain.cpp:488
	{ "TOPPAGE.findebortevent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_FindEbortEventInit },
	// lobbymain.cpp:489
	{ "TOPPAGE.findebortevent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_FindEbortEventDown },
	// lobbymain.cpp:492
	{ "TOPPAGE.halloween2007event", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_Halloween2007Init },
	// lobbymain.cpp:493
	{ "TOPPAGE.halloween2007event", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_Halloween2007EvenDown },
	// lobbymain.cpp:496
	{ "TOPPAGE.Christmas2007Event", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_Christmas2007EventInit },
	// lobbymain.cpp:497
	{ "TOPPAGE.Christmas2007Event", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_Christmas2007EventDown },
	// lobbymain.cpp:506
	{ "TOPPAGE.ticketexchage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TicketExchangeInit },
	// lobbymain.cpp:507
	{ "TOPPAGE.ticketexchage", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TicketExchangeDown },
	// lobbymain.cpp:510
	{ "TOPPAGE.MissionEvent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_MissionEventInit },
	// lobbymain.cpp:511
	{ "TOPPAGE.MissionEvent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_MissionEventDown },
	// lobbymain.cpp:514
	{ "TOPPAGE.BingoEvent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_BingoEventInit },
	// lobbymain.cpp:515
	{ "TOPPAGE.BingoEvent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_BingoEventDown },
	// lobbymain.cpp:518
	{ "TOPPAGE.KoohBirthdayEvent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_KoohBirthdayEventInit },
	// lobbymain.cpp:519
	{ "TOPPAGE.KoohBirthdayEvent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_KoohBirthdayEventDown },
	// lobbymain.cpp:522
	{ "TOPPAGE.treasureisland", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnToppage_TreasureIslandInit },
	// lobbymain.cpp:523
	{ "TOPPAGE.treasureisland", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_TreasureIslandLBDown },
	// lobbymain.cpp:559
	{ "TOPPAGE.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	// lobbymain.cpp:560
	{ "TOPPAGE.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	// lobbymain.cpp:561
	{ "TOPPAGE.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	// lobbymain.cpp:562
	{ "TOPPAGE.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:563
	{ "TOPPAGE.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:564
	{ "TOPPAGE.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	// lobbymain.cpp:565
	{ "TOPPAGE.ubar.front", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	// lobbymain.cpp:566
	{ "TOPPAGE.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:567
	{ "TOPPAGE.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:568
	{ "TOPPAGE.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	// lobbymain.cpp:569
	{ "TOPPAGE.ubar.shop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	// lobbymain.cpp:570
	{ "TOPPAGE.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:571
	{ "TOPPAGE.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:572
	{ "TOPPAGE.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	// lobbymain.cpp:573
	{ "TOPPAGE.ubar.myroom", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	// lobbymain.cpp:574
	{ "TOPPAGE.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:575
	{ "TOPPAGE.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:576
	{ "TOPPAGE.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	// lobbymain.cpp:577
	{ "TOPPAGE.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	// lobbymain.cpp:578
	{ "TOPPAGE.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:579
	{ "TOPPAGE.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:580
	{ "TOPPAGE.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	// lobbymain.cpp:581
	{ "TOPPAGE.ubar.gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	// lobbymain.cpp:582
	{ "TOPPAGE.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:583
	{ "TOPPAGE.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:584
	{ "TOPPAGE.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	// lobbymain.cpp:585
	{ "TOPPAGE.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	// lobbymain.cpp:586
	{ "TOPPAGE.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:587
	{ "TOPPAGE.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:588
	{ "TOPPAGE.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	// lobbymain.cpp:589
	{ "TOPPAGE.ubar.game", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	// lobbymain.cpp:590
	{ "TOPPAGE.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:591
	{ "TOPPAGE.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:592
	{ "TOPPAGE.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	// lobbymain.cpp:593
	{ "TOPPAGE.ubar.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	// lobbymain.cpp:594
	{ "TOPPAGE.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:595
	{ "TOPPAGE.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:596
	{ "TOPPAGE.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	// lobbymain.cpp:597
	{ "TOPPAGE.ubar.exit", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	// lobbymain.cpp:598
	{ "TOPPAGE.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:599
	{ "TOPPAGE.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:600
	{ "TOPPAGE.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	// lobbymain.cpp:601
	{ "TOPPAGE.ubar.tabbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	// lobbymain.cpp:604
	{ "TOPPAGE.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	// lobbymain.cpp:606
	{ "TOPPAGE.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	// lobbymain.cpp:607
	{ "TOPPAGE.utab.magicbox", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	// lobbymain.cpp:608
	{ "TOPPAGE.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:609
	{ "TOPPAGE.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:611
	{ "TOPPAGE.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	// lobbymain.cpp:612
	{ "TOPPAGE.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	// lobbymain.cpp:613
	{ "TOPPAGE.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:614
	{ "TOPPAGE.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:636
	{ "TOPPAGE.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	// lobbymain.cpp:637
	{ "TOPPAGE.utab.scratch", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	// lobbymain.cpp:638
	{ "TOPPAGE.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:639
	{ "TOPPAGE.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:641
	{ "TOPPAGE.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	// lobbymain.cpp:642
	{ "TOPPAGE.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	// lobbymain.cpp:643
	{ "TOPPAGE.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:644
	{ "TOPPAGE.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:645
	{ "TOPPAGE.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	// lobbymain.cpp:646
	{ "TOPPAGE.utab.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	// lobbymain.cpp:647
	{ "TOPPAGE.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:648
	{ "TOPPAGE.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:649
	{ "TOPPAGE.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	// lobbymain.cpp:650
	{ "TOPPAGE.utab.guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	// lobbymain.cpp:651
	{ "TOPPAGE.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:652
	{ "TOPPAGE.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:653
	{ "TOPPAGE.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	// lobbymain.cpp:654
	{ "TOPPAGE.utab.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	// lobbymain.cpp:655
	{ "TOPPAGE.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:656
	{ "TOPPAGE.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:659
	{ "TOPPAGE.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	// lobbymain.cpp:660
	{ "TOPPAGE.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	// lobbymain.cpp:661
	{ "TOPPAGE.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:662
	{ "TOPPAGE.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:673
	{ "TOPPAGE.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	// lobbymain.cpp:674
	{ "TOPPAGE.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	// lobbymain.cpp:675
	{ "TOPPAGE.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	// lobbymain.cpp:676
	{ "TOPPAGE.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	// lobbymain.cpp:677
	{ "TOPPAGE.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	// lobbymain.cpp:678
	{ "TOPPAGE.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	// lobbymain.cpp:679
	{ "TOPPAGE.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	// lobbymain.cpp:680
	{ "TOPPAGE.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	// lobbymain.cpp:681
	{ "TOPPAGE.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	// lobbymain.cpp:682
	{ "TOPPAGE.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	// lobbymain.cpp:683
	{ "TOPPAGE.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	// lobbymain.cpp:684
	{ "TOPPAGE.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	// lobbymain.cpp:685
	{ "TOPPAGE.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	// lobbymain.cpp:686
	{ "TOPPAGE.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	// lobbymain.cpp:687
	{ "TOPPAGE.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	// lobbymain.cpp:688
	{ "TOPPAGE.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	// lobbymain.cpp:689
	{ "TOPPAGE.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	// lobbymain.cpp:690
	{ "TOPPAGE.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	// lobbymain.cpp:691
	{ "TOPPAGE.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	// lobbymain.cpp:692
	{ "TOPPAGE.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	// lobbymain.cpp:693
	{ "TOPPAGE.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	// lobbymain.cpp:694
	{ "TOPPAGE.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	// lobbymain.cpp:695
	{ "TOPPAGE.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	// lobbymain.cpp:696
	{ "TOPPAGE.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	// lobbymain.cpp:697
	{ "TOPPAGE.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	// lobbymain.cpp:698
	{ "TOPPAGE.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	// lobbymain.cpp:699
	{ "TOPPAGE.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	// lobbymain.cpp:700
	{ "TOPPAGE.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	// lobbymain.cpp:701
	{ "TOPPAGE.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	// lobbymain.cpp:702
	{ "TOPPAGE.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	// lobbymain.cpp:703
	{ "TOPPAGE.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	// lobbymain.cpp:704
	{ "TOPPAGE.obar.onelinemsg_req", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	// lobbymain.cpp:705
	{ "TOPPAGE.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	// lobbymain.cpp:706
	{ "TOPPAGE.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	// lobbymain.cpp:707
	{ "TOPPAGE.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	// lobbymain.cpp:708
	{ "TOPPAGE.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	// lobbymain.cpp:709
	{ "TOPPAGE.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:710
	{ "TOPPAGE.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:711
	{ "TOPPAGE.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	// lobbymain.cpp:712
	{ "TOPPAGE.obar.messenger", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	// lobbymain.cpp:713
	{ "TOPPAGE.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:714
	{ "TOPPAGE.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:715
	{ "TOPPAGE.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	// lobbymain.cpp:716
	{ "TOPPAGE.obar.myinfo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	// lobbymain.cpp:717
	{ "TOPPAGE.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:718
	{ "TOPPAGE.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:719
	{ "TOPPAGE.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	// lobbymain.cpp:720
	{ "TOPPAGE.obar.option", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	// lobbymain.cpp:721
	{ "TOPPAGE.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:722
	{ "TOPPAGE.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:725
	{ "TOPPAGE.ubar.undertab_tooltip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabToolTipInit },
	// lobbymain.cpp:726
	{ "TOPPAGE.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	// lobbymain.cpp:727
	{ "TOPPAGE.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	// lobbymain.cpp:728
	{ "TOPPAGE.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	// lobbymain.cpp:729
	{ "TOPPAGE.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	// lobbymain.cpp:730
	{ "TOPPAGE.obar.messenger_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	// lobbymain.cpp:731
	{ "TOPPAGE.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	// lobbymain.cpp:732
	{ "TOPPAGE.obar.gift_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	// lobbymain.cpp:733
	{ "TOPPAGE.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	// lobbymain.cpp:734
	{ "TOPPAGE.treasure_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnTreasureAlarmInit },
	// lobbymain.cpp:735
	{ "TOPPAGE.treasure_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnTreasureAlarmUp },
	// lobbymain.cpp:738
	{ "TOPPAGE.FirstConnect", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnFirstConnectOwnerDraw },
	// lobbymain.cpp:741
	{ "TOPPAGE.Go_UCCShop", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_UCCShopBtnUp },
	// lobbymain.cpp:742
	{ "TOPPAGE.Go_CardShop", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnToppage_CardShopBtnUp },
	// lobbymain.cpp:745
	{ "SERVERLIST", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnServer_Init },
	// lobbymain.cpp:746
	{ "SERVERLIST", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnServer_Finish },
	// lobbymain.cpp:749
	{ "ROOMLIST", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_Init },
	// lobbymain.cpp:750
	{ "ROOMLIST", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_Finish },
	// lobbymain.cpp:751
	{ "ROOMLIST", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_Destroy },
	// lobbymain.cpp:753
	{ "ROOMLIST.roomlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomListInit },
	// lobbymain.cpp:754
	{ "ROOMLIST.roomlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomListOwnerDraw },
	// lobbymain.cpp:755
	{ "ROOMLIST.roomlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomListLDown },
	// lobbymain.cpp:756
	{ "ROOMLIST.roomlist", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomListRUp },
	// lobbymain.cpp:757
	{ "ROOMLIST.roomlist", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomListDClick },
	// lobbymain.cpp:759
	{ "ROOMLIST.room_no", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortNoInit },
	// lobbymain.cpp:760
	{ "ROOMLIST.room_no", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortNoDown },
	// lobbymain.cpp:761
	{ "ROOMLIST.room_no", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortNoOwnerDraw },
	// lobbymain.cpp:763
	{ "ROOMLIST.btn_cate_total", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateAll_BtnInit },
	// lobbymain.cpp:764
	{ "ROOMLIST.btn_cate_total", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateAll_BtnDown },
	// lobbymain.cpp:765
	{ "ROOMLIST.btn_cate_vs", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateVS_BtnInit },
	// lobbymain.cpp:766
	{ "ROOMLIST.btn_cate_vs", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateVS_BtnDown },
	// lobbymain.cpp:767
	{ "ROOMLIST.btn_cate_mass", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateMass_BtnInit },
	// lobbymain.cpp:768
	{ "ROOMLIST.btn_cate_mass", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateMass_BtnDown },
	// lobbymain.cpp:769
	{ "ROOMLIST.btn_cate_battle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateBattle_BtnInit },
	// lobbymain.cpp:770
	{ "ROOMLIST.btn_cate_battle", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateBattle_BtnDown },
	// lobbymain.cpp:771
	{ "ROOMLIST.btn_cate_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateChat_BtnInit },
	// lobbymain.cpp:772
	{ "ROOMLIST.btn_cate_chat", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateChat_BtnDown },
	// lobbymain.cpp:780
	{ "ROOMLIST.room_hole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortHoleInit },
	// lobbymain.cpp:781
	{ "ROOMLIST.room_hole", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortHoleDown },
	// lobbymain.cpp:782
	{ "ROOMLIST.room_hole", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortHoleOwnerDraw },
	// lobbymain.cpp:783
	{ "ROOMLIST.room_course", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCourseInit },
	// lobbymain.cpp:784
	{ "ROOMLIST.room_course", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCourseDown },
	// lobbymain.cpp:785
	{ "ROOMLIST.room_course", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCourseOwnerDraw },
	// lobbymain.cpp:786
	{ "ROOMLIST.room_state", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortStateInit },
	// lobbymain.cpp:787
	{ "ROOMLIST.room_state", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortStateDown },
	// lobbymain.cpp:788
	{ "ROOMLIST.room_state", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortStateOwnerDraw },
	// lobbymain.cpp:790
	{ "ROOMLIST.cate_vs_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateVS_ListInit },
	// lobbymain.cpp:791
	{ "ROOMLIST.cate_vs_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateVS_ListLUp },
	// lobbymain.cpp:792
	{ "ROOMLIST.cate_vs_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateVS_ListOwnerDraw },
	// lobbymain.cpp:793
	{ "ROOMLIST.cate_mass_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateMass_ListInit },
	// lobbymain.cpp:794
	{ "ROOMLIST.cate_mass_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateMass_ListLUp },
	// lobbymain.cpp:795
	{ "ROOMLIST.cate_mass_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateMass_ListOwnerDraw },
	// lobbymain.cpp:796
	{ "ROOMLIST.cate_battle_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateBattle_ListInit },
	// lobbymain.cpp:797
	{ "ROOMLIST.cate_battle_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_RoomSortCateBattle_ListLUp },
	// lobbymain.cpp:798
	{ "ROOMLIST.cate_battle_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_RoomSortCateBattle_ListOwnerDraw },
	// lobbymain.cpp:801
	{ "ROOMLIST.chatbg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgInit },
	// lobbymain.cpp:802
	{ "ROOMLIST.chatbg", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgOwnerDraw },
	// lobbymain.cpp:803
	{ "ROOMLIST.chatwndbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatWndBtnInit },
	// lobbymain.cpp:804
	{ "ROOMLIST.chatwndbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnChatWndBtnUp },
	// lobbymain.cpp:805
	{ "ROOMLIST.emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnEmoticonInit },
	// lobbymain.cpp:806
	{ "ROOMLIST.emoticon", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnEmoticonBtnUp },
	// lobbymain.cpp:807
	{ "ROOMLIST.language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnLanguageInit },
	// lobbymain.cpp:808
	{ "ROOMLIST.chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatInputInit },
	// lobbymain.cpp:809
	{ "ROOMLIST.chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatInputEnterKey },
	// lobbymain.cpp:810
	{ "ROOMLIST.chattarget", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetInit },
	// lobbymain.cpp:811
	{ "ROOMLIST.chattarget", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetBtnDown },
	// lobbymain.cpp:812
	{ "ROOMLIST.chattarget", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatTargetEnterKey },
	// lobbymain.cpp:813
	{ "ROOMLIST.chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_ChatViewInit },
	// lobbymain.cpp:814
	{ "ROOMLIST.report", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnReportBtnUp },
	// lobbymain.cpp:820
	{ "ROOMLIST.makeroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_MakeRoomInit },
	// lobbymain.cpp:821
	{ "ROOMLIST.makeroom", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_MakeRoomUp },
	// lobbymain.cpp:824
	{ "ROOMLIST.guildinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_GuildInfoInit },
	// lobbymain.cpp:825
	{ "ROOMLIST.guildinfo", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_GuildInfoLButtonUp },
	// lobbymain.cpp:836
	{ "ROOMLIST.userlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserListInit },
	// lobbymain.cpp:837
	{ "ROOMLIST.userlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserListLBtnUp },
	// lobbymain.cpp:838
	{ "ROOMLIST.userlist", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserListRBtnUp },
	// lobbymain.cpp:839
	{ "ROOMLIST.userlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserListOwnerDraw },
	// lobbymain.cpp:841
	{ "ROOMLIST.user_gender", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortGenderInit },
	// lobbymain.cpp:842
	{ "ROOMLIST.user_gender", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserSortGenderUp },
	// lobbymain.cpp:843
	{ "ROOMLIST.user_gender", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortGenderOwnerDraw },
	// lobbymain.cpp:844
	{ "ROOMLIST.user_level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortLevelInit },
	// lobbymain.cpp:845
	{ "ROOMLIST.user_level", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserSortLevelUp },
	// lobbymain.cpp:846
	{ "ROOMLIST.user_level", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortLevelOwnerDraw },
	// lobbymain.cpp:852
	{ "ROOMLIST.user_guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortGuildInit },
	// lobbymain.cpp:853
	{ "ROOMLIST.user_guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserSortGuildUp },
	// lobbymain.cpp:854
	{ "ROOMLIST.user_guild", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortGuildOwnerDraw },
	// lobbymain.cpp:855
	{ "ROOMLIST.user_nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortNickInit },
	// lobbymain.cpp:856
	{ "ROOMLIST.user_nick", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnRoomList_UserSortNickUp },
	// lobbymain.cpp:857
	{ "ROOMLIST.user_nick", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_UserSortNickOwnerDraw },
	// lobbymain.cpp:859
	{ "ROOMLIST.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	// lobbymain.cpp:860
	{ "ROOMLIST.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	// lobbymain.cpp:861
	{ "ROOMLIST.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	// lobbymain.cpp:862
	{ "ROOMLIST.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:863
	{ "ROOMLIST.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:864
	{ "ROOMLIST.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	// lobbymain.cpp:865
	{ "ROOMLIST.ubar.front", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	// lobbymain.cpp:866
	{ "ROOMLIST.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:867
	{ "ROOMLIST.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:868
	{ "ROOMLIST.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	// lobbymain.cpp:869
	{ "ROOMLIST.ubar.shop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	// lobbymain.cpp:870
	{ "ROOMLIST.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:871
	{ "ROOMLIST.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:872
	{ "ROOMLIST.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	// lobbymain.cpp:873
	{ "ROOMLIST.ubar.myroom", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	// lobbymain.cpp:874
	{ "ROOMLIST.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:875
	{ "ROOMLIST.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:876
	{ "ROOMLIST.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	// lobbymain.cpp:877
	{ "ROOMLIST.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	// lobbymain.cpp:878
	{ "ROOMLIST.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:879
	{ "ROOMLIST.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:880
	{ "ROOMLIST.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	// lobbymain.cpp:881
	{ "ROOMLIST.ubar.gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	// lobbymain.cpp:882
	{ "ROOMLIST.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:883
	{ "ROOMLIST.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:884
	{ "ROOMLIST.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	// lobbymain.cpp:885
	{ "ROOMLIST.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	// lobbymain.cpp:886
	{ "ROOMLIST.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:887
	{ "ROOMLIST.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:888
	{ "ROOMLIST.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	// lobbymain.cpp:889
	{ "ROOMLIST.ubar.game", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	// lobbymain.cpp:890
	{ "ROOMLIST.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:891
	{ "ROOMLIST.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:892
	{ "ROOMLIST.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	// lobbymain.cpp:893
	{ "ROOMLIST.ubar.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	// lobbymain.cpp:894
	{ "ROOMLIST.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:895
	{ "ROOMLIST.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:896
	{ "ROOMLIST.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	// lobbymain.cpp:897
	{ "ROOMLIST.ubar.exit", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	// lobbymain.cpp:898
	{ "ROOMLIST.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:899
	{ "ROOMLIST.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:900
	{ "ROOMLIST.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	// lobbymain.cpp:901
	{ "ROOMLIST.ubar.tabbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	// lobbymain.cpp:904
	{ "ROOMLIST.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	// lobbymain.cpp:906
	{ "ROOMLIST.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	// lobbymain.cpp:907
	{ "ROOMLIST.utab.magicbox", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	// lobbymain.cpp:908
	{ "ROOMLIST.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:909
	{ "ROOMLIST.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:911
	{ "ROOMLIST.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	// lobbymain.cpp:912
	{ "ROOMLIST.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	// lobbymain.cpp:913
	{ "ROOMLIST.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:914
	{ "ROOMLIST.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:936
	{ "ROOMLIST.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	// lobbymain.cpp:937
	{ "ROOMLIST.utab.scratch", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	// lobbymain.cpp:938
	{ "ROOMLIST.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:939
	{ "ROOMLIST.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:941
	{ "ROOMLIST.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	// lobbymain.cpp:942
	{ "ROOMLIST.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	// lobbymain.cpp:943
	{ "ROOMLIST.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:944
	{ "ROOMLIST.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:945
	{ "ROOMLIST.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	// lobbymain.cpp:946
	{ "ROOMLIST.utab.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	// lobbymain.cpp:947
	{ "ROOMLIST.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:948
	{ "ROOMLIST.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:949
	{ "ROOMLIST.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	// lobbymain.cpp:950
	{ "ROOMLIST.utab.guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	// lobbymain.cpp:951
	{ "ROOMLIST.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:952
	{ "ROOMLIST.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:953
	{ "ROOMLIST.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	// lobbymain.cpp:954
	{ "ROOMLIST.utab.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	// lobbymain.cpp:955
	{ "ROOMLIST.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:956
	{ "ROOMLIST.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:959
	{ "ROOMLIST.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	// lobbymain.cpp:960
	{ "ROOMLIST.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	// lobbymain.cpp:961
	{ "ROOMLIST.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:962
	{ "ROOMLIST.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:973
	{ "ROOMLIST.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	// lobbymain.cpp:974
	{ "ROOMLIST.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	// lobbymain.cpp:975
	{ "ROOMLIST.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	// lobbymain.cpp:976
	{ "ROOMLIST.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	// lobbymain.cpp:977
	{ "ROOMLIST.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	// lobbymain.cpp:978
	{ "ROOMLIST.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	// lobbymain.cpp:979
	{ "ROOMLIST.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	// lobbymain.cpp:980
	{ "ROOMLIST.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	// lobbymain.cpp:981
	{ "ROOMLIST.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	// lobbymain.cpp:982
	{ "ROOMLIST.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	// lobbymain.cpp:983
	{ "ROOMLIST.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	// lobbymain.cpp:984
	{ "ROOMLIST.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	// lobbymain.cpp:985
	{ "ROOMLIST.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	// lobbymain.cpp:986
	{ "ROOMLIST.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	// lobbymain.cpp:987
	{ "ROOMLIST.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	// lobbymain.cpp:988
	{ "ROOMLIST.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	// lobbymain.cpp:989
	{ "ROOMLIST.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	// lobbymain.cpp:990
	{ "ROOMLIST.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	// lobbymain.cpp:991
	{ "ROOMLIST.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	// lobbymain.cpp:992
	{ "ROOMLIST.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	// lobbymain.cpp:993
	{ "ROOMLIST.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	// lobbymain.cpp:994
	{ "ROOMLIST.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	// lobbymain.cpp:995
	{ "ROOMLIST.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	// lobbymain.cpp:996
	{ "ROOMLIST.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	// lobbymain.cpp:997
	{ "ROOMLIST.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	// lobbymain.cpp:998
	{ "ROOMLIST.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	// lobbymain.cpp:999
	{ "ROOMLIST.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	// lobbymain.cpp:1000
	{ "ROOMLIST.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	// lobbymain.cpp:1001
	{ "ROOMLIST.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	// lobbymain.cpp:1002
	{ "ROOMLIST.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	// lobbymain.cpp:1003
	{ "ROOMLIST.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	// lobbymain.cpp:1004
	{ "ROOMLIST.obar.onelinemsg_req", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	// lobbymain.cpp:1005
	{ "ROOMLIST.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	// lobbymain.cpp:1006
	{ "ROOMLIST.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	// lobbymain.cpp:1007
	{ "ROOMLIST.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	// lobbymain.cpp:1008
	{ "ROOMLIST.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	// lobbymain.cpp:1009
	{ "ROOMLIST.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1010
	{ "ROOMLIST.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1011
	{ "ROOMLIST.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	// lobbymain.cpp:1012
	{ "ROOMLIST.obar.messenger", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	// lobbymain.cpp:1013
	{ "ROOMLIST.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1014
	{ "ROOMLIST.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1015
	{ "ROOMLIST.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	// lobbymain.cpp:1016
	{ "ROOMLIST.obar.myinfo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	// lobbymain.cpp:1017
	{ "ROOMLIST.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1018
	{ "ROOMLIST.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1019
	{ "ROOMLIST.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	// lobbymain.cpp:1020
	{ "ROOMLIST.obar.option", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	// lobbymain.cpp:1021
	{ "ROOMLIST.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1022
	{ "ROOMLIST.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1024
	{ "ROOMLIST.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	// lobbymain.cpp:1025
	{ "ROOMLIST.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	// lobbymain.cpp:1026
	{ "ROOMLIST.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	// lobbymain.cpp:1027
	{ "ROOMLIST.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	// lobbymain.cpp:1028
	{ "ROOMLIST.obar.messenger_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	// lobbymain.cpp:1029
	{ "ROOMLIST.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	// lobbymain.cpp:1030
	{ "ROOMLIST.obar.gift_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	// lobbymain.cpp:1031
	{ "ROOMLIST.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	// lobbymain.cpp:1032
	{ "ROOMLIST.treasure_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnTreasureAlarmInit },
	// lobbymain.cpp:1033
	{ "ROOMLIST.treasure_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnTreasureAlarmUp },
	// lobbymain.cpp:1036
	{ "GAMEROOM", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_Init },
	// lobbymain.cpp:1037
	{ "GAMEROOM", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_Finish },
	// lobbymain.cpp:1038
	{ "GAMEROOM", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_Destroy },
	// lobbymain.cpp:1041
	{ "GAMEROOM.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_TitleInit },
	// lobbymain.cpp:1043
	{ "GAMEROOM.background", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_BackGroundInit },
	// lobbymain.cpp:1044
	{ "GAMEROOM.pet0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_Pet0Init },
	// lobbymain.cpp:1045
	{ "GAMEROOM.pet1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_Pet1Init },
	// lobbymain.cpp:1046
	{ "GAMEROOM.pet2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_Pet2Init },
	// lobbymain.cpp:1047
	{ "GAMEROOM.pet3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_Pet3Init },
	// lobbymain.cpp:1048
	{ "GAMEROOM.user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_RoomUserInit },
	// lobbymain.cpp:1049
	{ "GAMEROOM.user", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_RoomUserOwnerDraw },
	// lobbymain.cpp:1050
	{ "GAMEROOM.user", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserLBtnUp },
	// lobbymain.cpp:1051
	{ "GAMEROOM.user", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserRBtnUp },
	// lobbymain.cpp:1052
	{ "GAMEROOM.user", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserDClick },
	// lobbymain.cpp:1054
	{ "GAMEROOM.chatbg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgInit },
	// lobbymain.cpp:1055
	{ "GAMEROOM.chatbg", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgOwnerDraw },
	// lobbymain.cpp:1056
	{ "GAMEROOM.chatwndbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatWndBtnInit },
	// lobbymain.cpp:1057
	{ "GAMEROOM.chatwndbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnChatWndBtnUp },
	// lobbymain.cpp:1058
	{ "GAMEROOM.emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnEmoticonInit },
	// lobbymain.cpp:1059
	{ "GAMEROOM.emoticon", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnEmoticonBtnUp },
	// lobbymain.cpp:1060
	{ "GAMEROOM.language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnLanguageInit },
	// lobbymain.cpp:1061
	{ "GAMEROOM.chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatInputInit },
	// lobbymain.cpp:1062
	{ "GAMEROOM.chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatInputEnterKey },
	// lobbymain.cpp:1063
	{ "GAMEROOM.chattarget", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetInit },
	// lobbymain.cpp:1064
	{ "GAMEROOM.chattarget", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetBtnDown },
	// lobbymain.cpp:1065
	{ "GAMEROOM.chattarget", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatTargetEnterKey },
	// lobbymain.cpp:1066
	{ "GAMEROOM.chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_ChatViewInit },
	// lobbymain.cpp:1067
	{ "GAMEROOM.report", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnReportBtnUp },
	// lobbymain.cpp:1069
	{ "GAMEROOM.map_background", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapBgInit },
	// lobbymain.cpp:1070
	{ "GAMEROOM.map", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapInit },
	// lobbymain.cpp:1071
	{ "GAMEROOM.map", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapBtnUp },
	// lobbymain.cpp:1072
	{ "GAMEROOM.mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapPrevInit },
	// lobbymain.cpp:1073
	{ "GAMEROOM.mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapNextInit },
	// lobbymain.cpp:1074
	{ "GAMEROOM.mapprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapPrevBtnUp },
	// lobbymain.cpp:1075
	{ "GAMEROOM.mapnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapNextBtnUp },
	// lobbymain.cpp:1076
	{ "GAMEROOM.mapcomboex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapHoleComboInit },
	// lobbymain.cpp:1077
	{ "GAMEROOM.mapcomboex", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapHoleComboDown },
	// lobbymain.cpp:1078
	{ "GAMEROOM.overgauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapGaugeInit },
	// lobbymain.cpp:1079
	{ "GAMEROOM.maplock", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapLockInit },
	// lobbymain.cpp:1081
	{ "GAMEROOM.start_tip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_StartTipInit },
	// lobbymain.cpp:1082
	{ "GAMEROOM.ready_tip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ReadyTipInit },
	// lobbymain.cpp:1083
	{ "GAMEROOM.start", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_StartInit },
	// lobbymain.cpp:1084
	{ "GAMEROOM.start", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_StartBtnUp },
	// lobbymain.cpp:1086
	{ "GAMEROOM.ready", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ReadyInit },
	// lobbymain.cpp:1087
	{ "GAMEROOM.ready", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ReadyBtnUp },
	// lobbymain.cpp:1089
	{ "GAMEROOM.change_team", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ChangeTeamBtnInit },
	// lobbymain.cpp:1090
	{ "GAMEROOM.change_team", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ChangeTeamBtnUp },
	// lobbymain.cpp:1091
	{ "GAMEROOM.invite", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_UserListInit },
	// lobbymain.cpp:1092
	{ "GAMEROOM.invite", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_UserListBtnUp },
	// lobbymain.cpp:1093
	{ "GAMEROOM.modify", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_RoomOptionInit },
	// lobbymain.cpp:1094
	{ "GAMEROOM.modify", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomOptionBtnUp },
	// lobbymain.cpp:1096
	{ "GAMEROOM.powerbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_PowerBarInit },
	// lobbymain.cpp:1097
	{ "GAMEROOM.controlbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ControlBarInit },
	// lobbymain.cpp:1098
	{ "GAMEROOM.accuracybar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AccuracyBarInit },
	// lobbymain.cpp:1099
	{ "GAMEROOM.spinbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_SpinBarInit },
	// lobbymain.cpp:1100
	{ "GAMEROOM.curvebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CurveBarInit },
	// lobbymain.cpp:1101
	{ "GAMEROOM.epower", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_PowerEditInit },
	// lobbymain.cpp:1102
	{ "GAMEROOM.econtrol", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ControlEditInit },
	// lobbymain.cpp:1103
	{ "GAMEROOM.eaccuracy", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AccuracyEditInit },
	// lobbymain.cpp:1104
	{ "GAMEROOM.espin", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_SpinEditInit },
	// lobbymain.cpp:1105
	{ "GAMEROOM.ecurve", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CurveEditInit },
	// lobbymain.cpp:1108
	{ "GAMEROOM.cur_char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharInit },
	// lobbymain.cpp:1109
	{ "GAMEROOM.cur_char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CharDown },
	// lobbymain.cpp:1110
	{ "GAMEROOM.cur_char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharOwnerDraw },
	// lobbymain.cpp:1111
	{ "GAMEROOM.cur_caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieInit },
	// lobbymain.cpp:1112
	{ "GAMEROOM.cur_caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CaddieDown },
	// lobbymain.cpp:1113
	{ "GAMEROOM.cur_caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieOwnerDraw },
	// lobbymain.cpp:1114
	{ "GAMEROOM.cur_club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubInit },
	// lobbymain.cpp:1115
	{ "GAMEROOM.cur_club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ClubDown },
	// lobbymain.cpp:1116
	{ "GAMEROOM.cur_club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubOwnerDraw },
	// lobbymain.cpp:1117
	{ "GAMEROOM.cur_aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecInit },
	// lobbymain.cpp:1118
	{ "GAMEROOM.cur_aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_AztecDown },
	// lobbymain.cpp:1119
	{ "GAMEROOM.cur_aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecOwnerDraw },
	// lobbymain.cpp:1120
	{ "GAMEROOM.cur_mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotInit },
	// lobbymain.cpp:1121
	{ "GAMEROOM.cur_mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MascotDown },
	// lobbymain.cpp:1122
	{ "GAMEROOM.cur_mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotOwnerDraw },
	// lobbymain.cpp:1123
	{ "GAMEROOM.cur_equip_item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EquipItemInit },
	// lobbymain.cpp:1124
	{ "GAMEROOM.cur_equip_item", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_EquipItemDown },
	// lobbymain.cpp:1125
	{ "GAMEROOM.cur_equip_item", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EquipItemOwnerDraw },
	// lobbymain.cpp:1127
	{ "GAMEROOM.cur_char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharSelInit },
	// lobbymain.cpp:1128
	{ "GAMEROOM.cur_char_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CharSelDown },
	// lobbymain.cpp:1129
	{ "GAMEROOM.cur_char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharSelOwnerDraw },
	// lobbymain.cpp:1130
	{ "GAMEROOM.cur_caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieSelInit },
	// lobbymain.cpp:1131
	{ "GAMEROOM.cur_caddie_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CaddieSelDown },
	// lobbymain.cpp:1132
	{ "GAMEROOM.cur_caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieSelOwnerDraw },
	// lobbymain.cpp:1133
	{ "GAMEROOM.cur_club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubSelInit },
	// lobbymain.cpp:1134
	{ "GAMEROOM.cur_club_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ClubSelDown },
	// lobbymain.cpp:1135
	{ "GAMEROOM.cur_club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubSelOwnerDraw },
	// lobbymain.cpp:1136
	{ "GAMEROOM.cur_aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecSelInit },
	// lobbymain.cpp:1137
	{ "GAMEROOM.cur_aztec_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_AztecSelDown },
	// lobbymain.cpp:1138
	{ "GAMEROOM.cur_aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecSelOwnerDraw },
	// lobbymain.cpp:1139
	{ "GAMEROOM.cur_mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotSelInit },
	// lobbymain.cpp:1140
	{ "GAMEROOM.cur_mascot_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MascotSelDown },
	// lobbymain.cpp:1141
	{ "GAMEROOM.cur_mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotSelOwnerDraw },
	// lobbymain.cpp:1143
	{ "GAMEROOM.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	// lobbymain.cpp:1144
	{ "GAMEROOM.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	// lobbymain.cpp:1145
	{ "GAMEROOM.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	// lobbymain.cpp:1146
	{ "GAMEROOM.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1147
	{ "GAMEROOM.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1148
	{ "GAMEROOM.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	// lobbymain.cpp:1149
	{ "GAMEROOM.ubar.front", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	// lobbymain.cpp:1150
	{ "GAMEROOM.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1151
	{ "GAMEROOM.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1152
	{ "GAMEROOM.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	// lobbymain.cpp:1153
	{ "GAMEROOM.ubar.shop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	// lobbymain.cpp:1154
	{ "GAMEROOM.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1155
	{ "GAMEROOM.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1156
	{ "GAMEROOM.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	// lobbymain.cpp:1157
	{ "GAMEROOM.ubar.myroom", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	// lobbymain.cpp:1158
	{ "GAMEROOM.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1159
	{ "GAMEROOM.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1160
	{ "GAMEROOM.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	// lobbymain.cpp:1161
	{ "GAMEROOM.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	// lobbymain.cpp:1162
	{ "GAMEROOM.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1163
	{ "GAMEROOM.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1164
	{ "GAMEROOM.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	// lobbymain.cpp:1165
	{ "GAMEROOM.ubar.gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	// lobbymain.cpp:1166
	{ "GAMEROOM.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1167
	{ "GAMEROOM.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1168
	{ "GAMEROOM.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	// lobbymain.cpp:1169
	{ "GAMEROOM.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	// lobbymain.cpp:1170
	{ "GAMEROOM.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1171
	{ "GAMEROOM.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1172
	{ "GAMEROOM.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	// lobbymain.cpp:1173
	{ "GAMEROOM.ubar.game", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	// lobbymain.cpp:1174
	{ "GAMEROOM.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1175
	{ "GAMEROOM.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1176
	{ "GAMEROOM.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	// lobbymain.cpp:1177
	{ "GAMEROOM.ubar.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	// lobbymain.cpp:1178
	{ "GAMEROOM.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1179
	{ "GAMEROOM.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1180
	{ "GAMEROOM.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	// lobbymain.cpp:1181
	{ "GAMEROOM.ubar.exit", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	// lobbymain.cpp:1182
	{ "GAMEROOM.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1183
	{ "GAMEROOM.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1184
	{ "GAMEROOM.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	// lobbymain.cpp:1185
	{ "GAMEROOM.ubar.tabbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	// lobbymain.cpp:1188
	{ "GAMEROOM.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	// lobbymain.cpp:1190
	{ "GAMEROOM.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	// lobbymain.cpp:1191
	{ "GAMEROOM.utab.magicbox", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	// lobbymain.cpp:1192
	{ "GAMEROOM.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1193
	{ "GAMEROOM.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1195
	{ "GAMEROOM.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	// lobbymain.cpp:1196
	{ "GAMEROOM.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	// lobbymain.cpp:1197
	{ "GAMEROOM.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1198
	{ "GAMEROOM.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1220
	{ "GAMEROOM.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	// lobbymain.cpp:1221
	{ "GAMEROOM.utab.scratch", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	// lobbymain.cpp:1222
	{ "GAMEROOM.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1223
	{ "GAMEROOM.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1225
	{ "GAMEROOM.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	// lobbymain.cpp:1226
	{ "GAMEROOM.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	// lobbymain.cpp:1227
	{ "GAMEROOM.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1228
	{ "GAMEROOM.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1229
	{ "GAMEROOM.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	// lobbymain.cpp:1230
	{ "GAMEROOM.utab.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	// lobbymain.cpp:1231
	{ "GAMEROOM.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1232
	{ "GAMEROOM.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1233
	{ "GAMEROOM.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	// lobbymain.cpp:1234
	{ "GAMEROOM.utab.guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	// lobbymain.cpp:1235
	{ "GAMEROOM.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1236
	{ "GAMEROOM.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1237
	{ "GAMEROOM.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	// lobbymain.cpp:1238
	{ "GAMEROOM.utab.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	// lobbymain.cpp:1239
	{ "GAMEROOM.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1240
	{ "GAMEROOM.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1243
	{ "GAMEROOM.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	// lobbymain.cpp:1244
	{ "GAMEROOM.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	// lobbymain.cpp:1245
	{ "GAMEROOM.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1246
	{ "GAMEROOM.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1257
	{ "GAMEROOM.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	// lobbymain.cpp:1258
	{ "GAMEROOM.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	// lobbymain.cpp:1259
	{ "GAMEROOM.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	// lobbymain.cpp:1260
	{ "GAMEROOM.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	// lobbymain.cpp:1261
	{ "GAMEROOM.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	// lobbymain.cpp:1262
	{ "GAMEROOM.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	// lobbymain.cpp:1263
	{ "GAMEROOM.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	// lobbymain.cpp:1264
	{ "GAMEROOM.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	// lobbymain.cpp:1265
	{ "GAMEROOM.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	// lobbymain.cpp:1266
	{ "GAMEROOM.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	// lobbymain.cpp:1267
	{ "GAMEROOM.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	// lobbymain.cpp:1268
	{ "GAMEROOM.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	// lobbymain.cpp:1269
	{ "GAMEROOM.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	// lobbymain.cpp:1270
	{ "GAMEROOM.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	// lobbymain.cpp:1271
	{ "GAMEROOM.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	// lobbymain.cpp:1272
	{ "GAMEROOM.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	// lobbymain.cpp:1273
	{ "GAMEROOM.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	// lobbymain.cpp:1274
	{ "GAMEROOM.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	// lobbymain.cpp:1275
	{ "GAMEROOM.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	// lobbymain.cpp:1276
	{ "GAMEROOM.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	// lobbymain.cpp:1277
	{ "GAMEROOM.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	// lobbymain.cpp:1278
	{ "GAMEROOM.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	// lobbymain.cpp:1279
	{ "GAMEROOM.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	// lobbymain.cpp:1280
	{ "GAMEROOM.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	// lobbymain.cpp:1281
	{ "GAMEROOM.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	// lobbymain.cpp:1282
	{ "GAMEROOM.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	// lobbymain.cpp:1283
	{ "GAMEROOM.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	// lobbymain.cpp:1284
	{ "GAMEROOM.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	// lobbymain.cpp:1285
	{ "GAMEROOM.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	// lobbymain.cpp:1286
	{ "GAMEROOM.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	// lobbymain.cpp:1287
	{ "GAMEROOM.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	// lobbymain.cpp:1288
	{ "GAMEROOM.obar.onelinemsg_req", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	// lobbymain.cpp:1289
	{ "GAMEROOM.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	// lobbymain.cpp:1290
	{ "GAMEROOM.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	// lobbymain.cpp:1291
	{ "GAMEROOM.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	// lobbymain.cpp:1292
	{ "GAMEROOM.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	// lobbymain.cpp:1293
	{ "GAMEROOM.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1294
	{ "GAMEROOM.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1295
	{ "GAMEROOM.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	// lobbymain.cpp:1296
	{ "GAMEROOM.obar.messenger", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	// lobbymain.cpp:1297
	{ "GAMEROOM.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1298
	{ "GAMEROOM.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1299
	{ "GAMEROOM.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	// lobbymain.cpp:1300
	{ "GAMEROOM.obar.myinfo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	// lobbymain.cpp:1301
	{ "GAMEROOM.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1302
	{ "GAMEROOM.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1303
	{ "GAMEROOM.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	// lobbymain.cpp:1304
	{ "GAMEROOM.obar.option", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	// lobbymain.cpp:1305
	{ "GAMEROOM.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1306
	{ "GAMEROOM.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1308
	{ "GAMEROOM.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	// lobbymain.cpp:1309
	{ "GAMEROOM.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	// lobbymain.cpp:1310
	{ "GAMEROOM.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	// lobbymain.cpp:1311
	{ "GAMEROOM.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	// lobbymain.cpp:1312
	{ "GAMEROOM.obar.messenger_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	// lobbymain.cpp:1313
	{ "GAMEROOM.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	// lobbymain.cpp:1314
	{ "GAMEROOM.obar.gift_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	// lobbymain.cpp:1315
	{ "GAMEROOM.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	// lobbymain.cpp:1316
	{ "GAMEROOM.treasure_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnTreasureAlarmInit },
	// lobbymain.cpp:1317
	{ "GAMEROOM.treasure_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnTreasureAlarmUp },
	// lobbymain.cpp:1320
	{ "GAMEROOM_EXT", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_Init },
	// lobbymain.cpp:1321
	{ "GAMEROOM_EXT", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExt_Finish },
	// lobbymain.cpp:1322
	{ "GAMEROOM_EXT", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExt_Destroy },
	// lobbymain.cpp:1324
	{ "GAMEROOM_EXT.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_TitleInit },
	// lobbymain.cpp:1325
	{ "GAMEROOM_EXT.room_event", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EventOwnerDraw },
	// lobbymain.cpp:1327
	{ "GAMEROOM_EXT.user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_RoomUserInit },
	// lobbymain.cpp:1328
	{ "GAMEROOM_EXT.user", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_RoomUserOwnerDraw },
	// lobbymain.cpp:1329
	{ "GAMEROOM_EXT.user", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserLBtnUp },
	// lobbymain.cpp:1330
	{ "GAMEROOM_EXT.user", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserRBtnUp },
	// lobbymain.cpp:1331
	{ "GAMEROOM_EXT.user", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserDClick },
	// lobbymain.cpp:1332
	{ "GAMEROOM_EXT.30_team1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamTab1Init },
	// lobbymain.cpp:1333
	{ "GAMEROOM_EXT.30_team2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamTab2Init },
	// lobbymain.cpp:1334
	{ "GAMEROOM_EXT.30_guild1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildTab1Init },
	// lobbymain.cpp:1335
	{ "GAMEROOM_EXT.30_guild2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildTab2Init },
	// lobbymain.cpp:1336
	{ "GAMEROOM_EXT.30_team_num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamNumInit },
	// lobbymain.cpp:1337
	{ "GAMEROOM_EXT.30_team_num", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamNumOwnerDraw },
	// lobbymain.cpp:1338
	{ "GAMEROOM_EXT.30_guild_num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildNumInit },
	// lobbymain.cpp:1339
	{ "GAMEROOM_EXT.30_guild_num", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildNumOwnerDraw },
	// lobbymain.cpp:1341
	{ "GAMEROOM_EXT.chatbg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgInit },
	// lobbymain.cpp:1342
	{ "GAMEROOM_EXT.chatbg", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgOwnerDraw },
	// lobbymain.cpp:1343
	{ "GAMEROOM_EXT.chatwndbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatWndBtnInit },
	// lobbymain.cpp:1344
	{ "GAMEROOM_EXT.chatwndbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnChatWndBtnUp },
	// lobbymain.cpp:1345
	{ "GAMEROOM_EXT.emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnEmoticonInit },
	// lobbymain.cpp:1346
	{ "GAMEROOM_EXT.emoticon", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnEmoticonBtnUp },
	// lobbymain.cpp:1347
	{ "GAMEROOM_EXT.language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnLanguageInit },
	// lobbymain.cpp:1348
	{ "GAMEROOM_EXT.chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatInputInit },
	// lobbymain.cpp:1349
	{ "GAMEROOM_EXT.chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatInputEnterKey },
	// lobbymain.cpp:1350
	{ "GAMEROOM_EXT.chattarget", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetInit },
	// lobbymain.cpp:1351
	{ "GAMEROOM_EXT.chattarget", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetBtnDown },
	// lobbymain.cpp:1352
	{ "GAMEROOM_EXT.chattarget", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatTargetEnterKey },
	// lobbymain.cpp:1353
	{ "GAMEROOM_EXT.chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_ChatViewInit },
	// lobbymain.cpp:1354
	{ "GAMEROOM_EXT.report", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnReportBtnUp },
	// lobbymain.cpp:1356
	{ "GAMEROOM_EXT.map_background", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapBgInit },
	// lobbymain.cpp:1357
	{ "GAMEROOM_EXT.map", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapInit },
	// lobbymain.cpp:1358
	{ "GAMEROOM_EXT.map", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapBtnUp },
	// lobbymain.cpp:1359
	{ "GAMEROOM_EXT.mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapPrevInit },
	// lobbymain.cpp:1360
	{ "GAMEROOM_EXT.mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapNextInit },
	// lobbymain.cpp:1361
	{ "GAMEROOM_EXT.mapprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapPrevBtnUp },
	// lobbymain.cpp:1362
	{ "GAMEROOM_EXT.mapnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapNextBtnUp },
	// lobbymain.cpp:1363
	{ "GAMEROOM_EXT.mapcomboex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapHoleComboInit },
	// lobbymain.cpp:1364
	{ "GAMEROOM_EXT.mapcomboex", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MapHoleComboDown },
	// lobbymain.cpp:1366
	{ "GAMEROOM_EXT.overgauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapGaugeInit },
	// lobbymain.cpp:1367
	{ "GAMEROOM_EXT.maplock", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MapLockInit },
	// lobbymain.cpp:1369
	{ "GAMEROOM_EXT.start_tip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_StartTipInit },
	// lobbymain.cpp:1370
	{ "GAMEROOM_EXT.ready_tip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ReadyTipInit },
	// lobbymain.cpp:1371
	{ "GAMEROOM_EXT.start", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_StartInit },
	// lobbymain.cpp:1372
	{ "GAMEROOM_EXT.start", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_StartBtnUp },
	// lobbymain.cpp:1374
	{ "GAMEROOM_EXT.ready", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ReadyInit },
	// lobbymain.cpp:1375
	{ "GAMEROOM_EXT.ready", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ReadyBtnUp },
	// lobbymain.cpp:1377
	{ "GAMEROOM_EXT.change_team", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ChangeTeamBtnInit },
	// lobbymain.cpp:1378
	{ "GAMEROOM_EXT.change_team", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ChangeTeamBtnUp },
	// lobbymain.cpp:1379
	{ "GAMEROOM_EXT.invite", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildInviteInit },
	// lobbymain.cpp:1380
	{ "GAMEROOM_EXT.invite", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExt_GuildInviteLBtnUp },
	// lobbymain.cpp:1381
	{ "GAMEROOM_EXT.modify", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_RoomOptionInit },
	// lobbymain.cpp:1382
	{ "GAMEROOM_EXT.modify", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomOptionBtnUp },
	// lobbymain.cpp:1384
	{ "GAMEROOM_EXT.powerbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_PowerBarInit },
	// lobbymain.cpp:1385
	{ "GAMEROOM_EXT.controlbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ControlBarInit },
	// lobbymain.cpp:1386
	{ "GAMEROOM_EXT.accuracybar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AccuracyBarInit },
	// lobbymain.cpp:1387
	{ "GAMEROOM_EXT.spinbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_SpinBarInit },
	// lobbymain.cpp:1388
	{ "GAMEROOM_EXT.curvebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CurveBarInit },
	// lobbymain.cpp:1389
	{ "GAMEROOM_EXT.epower", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_PowerEditInit },
	// lobbymain.cpp:1390
	{ "GAMEROOM_EXT.econtrol", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ControlEditInit },
	// lobbymain.cpp:1391
	{ "GAMEROOM_EXT.eaccuracy", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AccuracyEditInit },
	// lobbymain.cpp:1392
	{ "GAMEROOM_EXT.espin", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_SpinEditInit },
	// lobbymain.cpp:1393
	{ "GAMEROOM_EXT.ecurve", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CurveEditInit },
	// lobbymain.cpp:1395
	{ "GAMEROOM_EXT.cur_char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharInit },
	// lobbymain.cpp:1396
	{ "GAMEROOM_EXT.cur_char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CharDown },
	// lobbymain.cpp:1397
	{ "GAMEROOM_EXT.cur_char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharOwnerDraw },
	// lobbymain.cpp:1398
	{ "GAMEROOM_EXT.cur_caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieInit },
	// lobbymain.cpp:1399
	{ "GAMEROOM_EXT.cur_caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CaddieDown },
	// lobbymain.cpp:1400
	{ "GAMEROOM_EXT.cur_caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieOwnerDraw },
	// lobbymain.cpp:1401
	{ "GAMEROOM_EXT.cur_club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubInit },
	// lobbymain.cpp:1402
	{ "GAMEROOM_EXT.cur_club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ClubDown },
	// lobbymain.cpp:1403
	{ "GAMEROOM_EXT.cur_club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubOwnerDraw },
	// lobbymain.cpp:1404
	{ "GAMEROOM_EXT.cur_aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecInit },
	// lobbymain.cpp:1405
	{ "GAMEROOM_EXT.cur_aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_AztecDown },
	// lobbymain.cpp:1406
	{ "GAMEROOM_EXT.cur_aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecOwnerDraw },
	// lobbymain.cpp:1407
	{ "GAMEROOM_EXT.cur_mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotInit },
	// lobbymain.cpp:1408
	{ "GAMEROOM_EXT.cur_mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MascotDown },
	// lobbymain.cpp:1409
	{ "GAMEROOM_EXT.cur_mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotOwnerDraw },
	// lobbymain.cpp:1410
	{ "GAMEROOM_EXT.cur_equip_item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EquipItemInit },
	// lobbymain.cpp:1411
	{ "GAMEROOM_EXT.cur_equip_item", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_EquipItemDown },
	// lobbymain.cpp:1412
	{ "GAMEROOM_EXT.cur_equip_item", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EquipItemOwnerDraw },
	// lobbymain.cpp:1414
	{ "GAMEROOM_EXT.cur_char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharSelInit },
	// lobbymain.cpp:1415
	{ "GAMEROOM_EXT.cur_char_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CharSelDown },
	// lobbymain.cpp:1416
	{ "GAMEROOM_EXT.cur_char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CharSelOwnerDraw },
	// lobbymain.cpp:1417
	{ "GAMEROOM_EXT.cur_caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieSelInit },
	// lobbymain.cpp:1418
	{ "GAMEROOM_EXT.cur_caddie_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_CaddieSelDown },
	// lobbymain.cpp:1419
	{ "GAMEROOM_EXT.cur_caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_CaddieSelOwnerDraw },
	// lobbymain.cpp:1420
	{ "GAMEROOM_EXT.cur_club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubSelInit },
	// lobbymain.cpp:1421
	{ "GAMEROOM_EXT.cur_club_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ClubSelDown },
	// lobbymain.cpp:1422
	{ "GAMEROOM_EXT.cur_club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ClubSelOwnerDraw },
	// lobbymain.cpp:1423
	{ "GAMEROOM_EXT.cur_aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecSelInit },
	// lobbymain.cpp:1424
	{ "GAMEROOM_EXT.cur_aztec_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_AztecSelDown },
	// lobbymain.cpp:1425
	{ "GAMEROOM_EXT.cur_aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_AztecSelOwnerDraw },
	// lobbymain.cpp:1426
	{ "GAMEROOM_EXT.cur_mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotSelInit },
	// lobbymain.cpp:1427
	{ "GAMEROOM_EXT.cur_mascot_sel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_MascotSelDown },
	// lobbymain.cpp:1428
	{ "GAMEROOM_EXT.cur_mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_MascotSelOwnerDraw },
	// lobbymain.cpp:1430
	{ "GAMEROOM_EXT.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	// lobbymain.cpp:1431
	{ "GAMEROOM_EXT.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	// lobbymain.cpp:1432
	{ "GAMEROOM_EXT.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	// lobbymain.cpp:1433
	{ "GAMEROOM_EXT.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1434
	{ "GAMEROOM_EXT.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1435
	{ "GAMEROOM_EXT.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	// lobbymain.cpp:1436
	{ "GAMEROOM_EXT.ubar.front", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	// lobbymain.cpp:1437
	{ "GAMEROOM_EXT.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1438
	{ "GAMEROOM_EXT.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1439
	{ "GAMEROOM_EXT.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	// lobbymain.cpp:1440
	{ "GAMEROOM_EXT.ubar.shop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	// lobbymain.cpp:1441
	{ "GAMEROOM_EXT.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1442
	{ "GAMEROOM_EXT.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1443
	{ "GAMEROOM_EXT.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	// lobbymain.cpp:1444
	{ "GAMEROOM_EXT.ubar.myroom", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	// lobbymain.cpp:1445
	{ "GAMEROOM_EXT.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1446
	{ "GAMEROOM_EXT.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1447
	{ "GAMEROOM_EXT.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	// lobbymain.cpp:1448
	{ "GAMEROOM_EXT.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	// lobbymain.cpp:1449
	{ "GAMEROOM_EXT.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1450
	{ "GAMEROOM_EXT.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1451
	{ "GAMEROOM_EXT.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	// lobbymain.cpp:1452
	{ "GAMEROOM_EXT.ubar.gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	// lobbymain.cpp:1453
	{ "GAMEROOM_EXT.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1454
	{ "GAMEROOM_EXT.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1455
	{ "GAMEROOM_EXT.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	// lobbymain.cpp:1456
	{ "GAMEROOM_EXT.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	// lobbymain.cpp:1457
	{ "GAMEROOM_EXT.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1458
	{ "GAMEROOM_EXT.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1459
	{ "GAMEROOM_EXT.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	// lobbymain.cpp:1460
	{ "GAMEROOM_EXT.ubar.game", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	// lobbymain.cpp:1461
	{ "GAMEROOM_EXT.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1462
	{ "GAMEROOM_EXT.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1463
	{ "GAMEROOM_EXT.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	// lobbymain.cpp:1464
	{ "GAMEROOM_EXT.ubar.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	// lobbymain.cpp:1465
	{ "GAMEROOM_EXT.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1466
	{ "GAMEROOM_EXT.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1467
	{ "GAMEROOM_EXT.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	// lobbymain.cpp:1468
	{ "GAMEROOM_EXT.ubar.exit", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	// lobbymain.cpp:1469
	{ "GAMEROOM_EXT.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1470
	{ "GAMEROOM_EXT.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1471
	{ "GAMEROOM_EXT.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	// lobbymain.cpp:1472
	{ "GAMEROOM_EXT.ubar.tabbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	// lobbymain.cpp:1475
	{ "GAMEROOM_EXT.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	// lobbymain.cpp:1477
	{ "GAMEROOM_EXT.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	// lobbymain.cpp:1478
	{ "GAMEROOM_EXT.utab.magicbox", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	// lobbymain.cpp:1479
	{ "GAMEROOM_EXT.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1480
	{ "GAMEROOM_EXT.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1482
	{ "GAMEROOM_EXT.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	// lobbymain.cpp:1483
	{ "GAMEROOM_EXT.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	// lobbymain.cpp:1484
	{ "GAMEROOM_EXT.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1485
	{ "GAMEROOM_EXT.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1507
	{ "GAMEROOM_EXT.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	// lobbymain.cpp:1508
	{ "GAMEROOM_EXT.utab.scratch", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	// lobbymain.cpp:1509
	{ "GAMEROOM_EXT.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1510
	{ "GAMEROOM_EXT.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1512
	{ "GAMEROOM_EXT.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	// lobbymain.cpp:1513
	{ "GAMEROOM_EXT.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	// lobbymain.cpp:1514
	{ "GAMEROOM_EXT.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1515
	{ "GAMEROOM_EXT.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1516
	{ "GAMEROOM_EXT.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	// lobbymain.cpp:1517
	{ "GAMEROOM_EXT.utab.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	// lobbymain.cpp:1518
	{ "GAMEROOM_EXT.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1519
	{ "GAMEROOM_EXT.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1520
	{ "GAMEROOM_EXT.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	// lobbymain.cpp:1521
	{ "GAMEROOM_EXT.utab.guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	// lobbymain.cpp:1522
	{ "GAMEROOM_EXT.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1523
	{ "GAMEROOM_EXT.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1524
	{ "GAMEROOM_EXT.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	// lobbymain.cpp:1525
	{ "GAMEROOM_EXT.utab.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	// lobbymain.cpp:1526
	{ "GAMEROOM_EXT.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1527
	{ "GAMEROOM_EXT.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1530
	{ "GAMEROOM_EXT.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	// lobbymain.cpp:1531
	{ "GAMEROOM_EXT.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	// lobbymain.cpp:1532
	{ "GAMEROOM_EXT.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1533
	{ "GAMEROOM_EXT.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1544
	{ "GAMEROOM_EXT.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	// lobbymain.cpp:1545
	{ "GAMEROOM_EXT.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	// lobbymain.cpp:1546
	{ "GAMEROOM_EXT.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	// lobbymain.cpp:1547
	{ "GAMEROOM_EXT.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	// lobbymain.cpp:1548
	{ "GAMEROOM_EXT.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	// lobbymain.cpp:1549
	{ "GAMEROOM_EXT.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	// lobbymain.cpp:1550
	{ "GAMEROOM_EXT.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	// lobbymain.cpp:1551
	{ "GAMEROOM_EXT.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	// lobbymain.cpp:1552
	{ "GAMEROOM_EXT.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	// lobbymain.cpp:1553
	{ "GAMEROOM_EXT.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	// lobbymain.cpp:1554
	{ "GAMEROOM_EXT.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	// lobbymain.cpp:1555
	{ "GAMEROOM_EXT.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	// lobbymain.cpp:1556
	{ "GAMEROOM_EXT.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	// lobbymain.cpp:1557
	{ "GAMEROOM_EXT.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	// lobbymain.cpp:1558
	{ "GAMEROOM_EXT.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	// lobbymain.cpp:1559
	{ "GAMEROOM_EXT.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	// lobbymain.cpp:1560
	{ "GAMEROOM_EXT.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	// lobbymain.cpp:1561
	{ "GAMEROOM_EXT.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	// lobbymain.cpp:1562
	{ "GAMEROOM_EXT.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	// lobbymain.cpp:1563
	{ "GAMEROOM_EXT.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	// lobbymain.cpp:1564
	{ "GAMEROOM_EXT.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	// lobbymain.cpp:1565
	{ "GAMEROOM_EXT.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	// lobbymain.cpp:1566
	{ "GAMEROOM_EXT.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	// lobbymain.cpp:1567
	{ "GAMEROOM_EXT.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	// lobbymain.cpp:1568
	{ "GAMEROOM_EXT.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	// lobbymain.cpp:1569
	{ "GAMEROOM_EXT.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	// lobbymain.cpp:1570
	{ "GAMEROOM_EXT.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	// lobbymain.cpp:1571
	{ "GAMEROOM_EXT.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	// lobbymain.cpp:1572
	{ "GAMEROOM_EXT.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	// lobbymain.cpp:1573
	{ "GAMEROOM_EXT.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	// lobbymain.cpp:1574
	{ "GAMEROOM_EXT.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	// lobbymain.cpp:1575
	{ "GAMEROOM_EXT.obar.onelinemsg_req", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	// lobbymain.cpp:1576
	{ "GAMEROOM_EXT.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	// lobbymain.cpp:1577
	{ "GAMEROOM_EXT.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	// lobbymain.cpp:1578
	{ "GAMEROOM_EXT.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	// lobbymain.cpp:1579
	{ "GAMEROOM_EXT.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	// lobbymain.cpp:1580
	{ "GAMEROOM_EXT.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1581
	{ "GAMEROOM_EXT.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1582
	{ "GAMEROOM_EXT.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	// lobbymain.cpp:1583
	{ "GAMEROOM_EXT.obar.messenger", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	// lobbymain.cpp:1584
	{ "GAMEROOM_EXT.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1585
	{ "GAMEROOM_EXT.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1586
	{ "GAMEROOM_EXT.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	// lobbymain.cpp:1587
	{ "GAMEROOM_EXT.obar.myinfo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	// lobbymain.cpp:1588
	{ "GAMEROOM_EXT.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1589
	{ "GAMEROOM_EXT.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1590
	{ "GAMEROOM_EXT.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	// lobbymain.cpp:1591
	{ "GAMEROOM_EXT.obar.option", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	// lobbymain.cpp:1592
	{ "GAMEROOM_EXT.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1593
	{ "GAMEROOM_EXT.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1594
	{ "GAMEROOM_EXT.countdown", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_CountdownInit },
	// lobbymain.cpp:1596
	{ "GAMEROOM_EXT.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	// lobbymain.cpp:1597
	{ "GAMEROOM_EXT.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	// lobbymain.cpp:1598
	{ "GAMEROOM_EXT.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	// lobbymain.cpp:1599
	{ "GAMEROOM_EXT.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	// lobbymain.cpp:1600
	{ "GAMEROOM_EXT.obar.messenger_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	// lobbymain.cpp:1601
	{ "GAMEROOM_EXT.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	// lobbymain.cpp:1602
	{ "GAMEROOM_EXT.obar.gift_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	// lobbymain.cpp:1603
	{ "GAMEROOM_EXT.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	// lobbymain.cpp:1604
	{ "GAMEROOM_EXT.treasure_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnTreasureAlarmInit },
	// lobbymain.cpp:1605
	{ "GAMEROOM_EXT.treasure_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnTreasureAlarmUp },
	// lobbymain.cpp:1608
	{ "GAMEROOM_EXTRES", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_Init },
	// lobbymain.cpp:1609
	{ "GAMEROOM_EXTRES", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_Finish },
	// lobbymain.cpp:1610
	{ "GAMEROOM_EXTRES", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_Destroy },
	// lobbymain.cpp:1613
	{ "GAMEROOM_EXTRES.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_TitleInit },
	// lobbymain.cpp:1614
	{ "GAMEROOM_EXTRES.room_event", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_EventOwnerDraw },
	// lobbymain.cpp:1616
	{ "GAMEROOM_EXTRES.user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_RoomUserInit },
	// lobbymain.cpp:1617
	{ "GAMEROOM_EXTRES.user", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_RoomUserOwnerDraw },
	// lobbymain.cpp:1618
	{ "GAMEROOM_EXTRES.user", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserLBtnUp },
	// lobbymain.cpp:1619
	{ "GAMEROOM_EXTRES.user", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserRBtnUp },
	// lobbymain.cpp:1620
	{ "GAMEROOM_EXTRES.user", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_RoomUserDClick },
	// lobbymain.cpp:1621
	{ "GAMEROOM_EXTRES.30_team1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamTab1Init },
	// lobbymain.cpp:1622
	{ "GAMEROOM_EXTRES.30_team2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamTab2Init },
	// lobbymain.cpp:1623
	{ "GAMEROOM_EXTRES.30_guild1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildTab1Init },
	// lobbymain.cpp:1624
	{ "GAMEROOM_EXTRES.30_guild2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildTab2Init },
	// lobbymain.cpp:1625
	{ "GAMEROOM_EXTRES.30_team_num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamNumInit },
	// lobbymain.cpp:1626
	{ "GAMEROOM_EXTRES.30_team_num", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_TeamNumOwnerDraw },
	// lobbymain.cpp:1627
	{ "GAMEROOM_EXTRES.30_guild_num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildNumInit },
	// lobbymain.cpp:1628
	{ "GAMEROOM_EXTRES.30_guild_num", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_GuildNumOwnerDraw },
	// lobbymain.cpp:1629
	{ "GAMEROOM_EXTRES.score_gb", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_GuildScoreGaugeInit },
	// lobbymain.cpp:1631
	{ "GAMEROOM_EXTRES.chatbg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgInit },
	// lobbymain.cpp:1632
	{ "GAMEROOM_EXTRES.chatbg", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatBgOwnerDraw },
	// lobbymain.cpp:1633
	{ "GAMEROOM_EXTRES.chatwndbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatWndBtnInit },
	// lobbymain.cpp:1634
	{ "GAMEROOM_EXTRES.chatwndbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnChatWndBtnUp },
	// lobbymain.cpp:1635
	{ "GAMEROOM_EXTRES.emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnEmoticonInit },
	// lobbymain.cpp:1636
	{ "GAMEROOM_EXTRES.emoticon", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnEmoticonBtnUp },
	// lobbymain.cpp:1637
	{ "GAMEROOM_EXTRES.language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnLanguageInit },
	// lobbymain.cpp:1638
	{ "GAMEROOM_EXTRES.chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatInputInit },
	// lobbymain.cpp:1639
	{ "GAMEROOM_EXTRES.chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatInputEnterKey },
	// lobbymain.cpp:1640
	{ "GAMEROOM_EXTRES.chattarget", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetInit },
	// lobbymain.cpp:1641
	{ "GAMEROOM_EXTRES.chattarget", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChatTargetBtnDown },
	// lobbymain.cpp:1642
	{ "GAMEROOM_EXTRES.chattarget", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CLobbyMain::OnChatTargetEnterKey },
	// lobbymain.cpp:1643
	{ "GAMEROOM_EXTRES.chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnRoomList_ChatViewInit },
	// lobbymain.cpp:1644
	{ "GAMEROOM_EXTRES.report", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnReportBtnUp },
	// lobbymain.cpp:1646
	{ "GAMEROOM_EXTRES.rankframe", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_FrameInit },
	// lobbymain.cpp:1647
	{ "GAMEROOM_EXTRES.rankingtab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_RoomUserRankTabInit },
	// lobbymain.cpp:1648
	{ "GAMEROOM_EXTRES.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_RoomUserRankInit },
	// lobbymain.cpp:1649
	{ "GAMEROOM_EXTRES.ranking", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_RoomUserRankOwnerDraw },
	// lobbymain.cpp:1650
	{ "GAMEROOM_EXTRES.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_RoomUserRankLBtnUp },
	// lobbymain.cpp:1651
	{ "GAMEROOM_EXTRES.ranking", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_RoomUserRankRBtnUp },
	// lobbymain.cpp:1652
	{ "GAMEROOM_EXTRES.guildranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_RoomGuildRankInit },
	// lobbymain.cpp:1653
	{ "GAMEROOM_EXTRES.guildranking", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExtRes_RoomGuildRankOwnerDraw },
	// lobbymain.cpp:1654
	{ "GAMEROOM_EXTRES.guildranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_RoomGuildRankLBtnUp },
	// lobbymain.cpp:1655
	{ "GAMEROOM_EXTRES.guildranking", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExtRes_RoomGuildRankRBtnUp },
	// lobbymain.cpp:1656
	{ "GAMEROOM_EXTRES.score", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoom_ScoreInit },
	// lobbymain.cpp:1657
	{ "GAMEROOM_EXTRES.score", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoom_ScoreBtnUp },
	// lobbymain.cpp:1658
	{ "GAMEROOM_EXTRES.user_info", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnGameRoomExt_UserInfoInit },
	// lobbymain.cpp:1659
	{ "GAMEROOM_EXTRES.user_info", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExt_UserInfoLBtnUp },
	// lobbymain.cpp:1662
	{ "GAMEROOM_EXTRES.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	// lobbymain.cpp:1663
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	// lobbymain.cpp:1664
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	// lobbymain.cpp:1665
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnChannelRoomCloseBtnInit },
	// lobbymain.cpp:1666
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnGameRoomExt_ResCloseBtnUp },
	// lobbymain.cpp:1667
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1668
	{ "GAMEROOM_EXTRES.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1669
	{ "GAMEROOM_EXTRES.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	// lobbymain.cpp:1670
	{ "GAMEROOM_EXTRES.ubar.front", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	// lobbymain.cpp:1671
	{ "GAMEROOM_EXTRES.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1672
	{ "GAMEROOM_EXTRES.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1673
	{ "GAMEROOM_EXTRES.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	// lobbymain.cpp:1674
	{ "GAMEROOM_EXTRES.ubar.shop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	// lobbymain.cpp:1675
	{ "GAMEROOM_EXTRES.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1676
	{ "GAMEROOM_EXTRES.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1677
	{ "GAMEROOM_EXTRES.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	// lobbymain.cpp:1678
	{ "GAMEROOM_EXTRES.ubar.myroom", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	// lobbymain.cpp:1679
	{ "GAMEROOM_EXTRES.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1680
	{ "GAMEROOM_EXTRES.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1681
	{ "GAMEROOM_EXTRES.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	// lobbymain.cpp:1682
	{ "GAMEROOM_EXTRES.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	// lobbymain.cpp:1683
	{ "GAMEROOM_EXTRES.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1684
	{ "GAMEROOM_EXTRES.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1685
	{ "GAMEROOM_EXTRES.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	// lobbymain.cpp:1686
	{ "GAMEROOM_EXTRES.ubar.gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	// lobbymain.cpp:1687
	{ "GAMEROOM_EXTRES.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1688
	{ "GAMEROOM_EXTRES.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1689
	{ "GAMEROOM_EXTRES.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	// lobbymain.cpp:1690
	{ "GAMEROOM_EXTRES.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	// lobbymain.cpp:1691
	{ "GAMEROOM_EXTRES.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1692
	{ "GAMEROOM_EXTRES.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1693
	{ "GAMEROOM_EXTRES.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	// lobbymain.cpp:1694
	{ "GAMEROOM_EXTRES.ubar.game", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	// lobbymain.cpp:1695
	{ "GAMEROOM_EXTRES.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1696
	{ "GAMEROOM_EXTRES.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1697
	{ "GAMEROOM_EXTRES.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	// lobbymain.cpp:1698
	{ "GAMEROOM_EXTRES.ubar.server", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	// lobbymain.cpp:1699
	{ "GAMEROOM_EXTRES.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1700
	{ "GAMEROOM_EXTRES.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1701
	{ "GAMEROOM_EXTRES.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	// lobbymain.cpp:1702
	{ "GAMEROOM_EXTRES.ubar.exit", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	// lobbymain.cpp:1703
	{ "GAMEROOM_EXTRES.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1704
	{ "GAMEROOM_EXTRES.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1705
	{ "GAMEROOM_EXTRES.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	// lobbymain.cpp:1706
	{ "GAMEROOM_EXTRES.ubar.tabbtn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	// lobbymain.cpp:1709
	{ "GAMEROOM_EXTRES.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	// lobbymain.cpp:1711
	{ "GAMEROOM_EXTRES.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	// lobbymain.cpp:1712
	{ "GAMEROOM_EXTRES.utab.magicbox", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	// lobbymain.cpp:1713
	{ "GAMEROOM_EXTRES.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1714
	{ "GAMEROOM_EXTRES.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1716
	{ "GAMEROOM_EXTRES.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	// lobbymain.cpp:1717
	{ "GAMEROOM_EXTRES.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	// lobbymain.cpp:1718
	{ "GAMEROOM_EXTRES.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1719
	{ "GAMEROOM_EXTRES.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1741
	{ "GAMEROOM_EXTRES.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	// lobbymain.cpp:1742
	{ "GAMEROOM_EXTRES.utab.scratch", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	// lobbymain.cpp:1743
	{ "GAMEROOM_EXTRES.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1744
	{ "GAMEROOM_EXTRES.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1746
	{ "GAMEROOM_EXTRES.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	// lobbymain.cpp:1747
	{ "GAMEROOM_EXTRES.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	// lobbymain.cpp:1748
	{ "GAMEROOM_EXTRES.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1749
	{ "GAMEROOM_EXTRES.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1750
	{ "GAMEROOM_EXTRES.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	// lobbymain.cpp:1751
	{ "GAMEROOM_EXTRES.utab.ranking", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	// lobbymain.cpp:1752
	{ "GAMEROOM_EXTRES.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1753
	{ "GAMEROOM_EXTRES.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1754
	{ "GAMEROOM_EXTRES.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	// lobbymain.cpp:1755
	{ "GAMEROOM_EXTRES.utab.guild", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	// lobbymain.cpp:1756
	{ "GAMEROOM_EXTRES.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1757
	{ "GAMEROOM_EXTRES.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1758
	{ "GAMEROOM_EXTRES.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	// lobbymain.cpp:1759
	{ "GAMEROOM_EXTRES.utab.notice", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	// lobbymain.cpp:1760
	{ "GAMEROOM_EXTRES.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1761
	{ "GAMEROOM_EXTRES.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1764
	{ "GAMEROOM_EXTRES.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	// lobbymain.cpp:1765
	{ "GAMEROOM_EXTRES.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	// lobbymain.cpp:1766
	{ "GAMEROOM_EXTRES.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1767
	{ "GAMEROOM_EXTRES.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1778
	{ "GAMEROOM_EXTRES.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	// lobbymain.cpp:1779
	{ "GAMEROOM_EXTRES.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	// lobbymain.cpp:1780
	{ "GAMEROOM_EXTRES.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	// lobbymain.cpp:1781
	{ "GAMEROOM_EXTRES.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	// lobbymain.cpp:1782
	{ "GAMEROOM_EXTRES.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	// lobbymain.cpp:1783
	{ "GAMEROOM_EXTRES.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	// lobbymain.cpp:1784
	{ "GAMEROOM_EXTRES.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	// lobbymain.cpp:1785
	{ "GAMEROOM_EXTRES.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	// lobbymain.cpp:1786
	{ "GAMEROOM_EXTRES.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	// lobbymain.cpp:1787
	{ "GAMEROOM_EXTRES.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	// lobbymain.cpp:1788
	{ "GAMEROOM_EXTRES.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	// lobbymain.cpp:1789
	{ "GAMEROOM_EXTRES.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	// lobbymain.cpp:1790
	{ "GAMEROOM_EXTRES.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	// lobbymain.cpp:1791
	{ "GAMEROOM_EXTRES.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	// lobbymain.cpp:1792
	{ "GAMEROOM_EXTRES.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	// lobbymain.cpp:1793
	{ "GAMEROOM_EXTRES.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	// lobbymain.cpp:1794
	{ "GAMEROOM_EXTRES.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	// lobbymain.cpp:1795
	{ "GAMEROOM_EXTRES.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	// lobbymain.cpp:1796
	{ "GAMEROOM_EXTRES.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	// lobbymain.cpp:1797
	{ "GAMEROOM_EXTRES.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	// lobbymain.cpp:1798
	{ "GAMEROOM_EXTRES.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	// lobbymain.cpp:1799
	{ "GAMEROOM_EXTRES.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	// lobbymain.cpp:1800
	{ "GAMEROOM_EXTRES.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	// lobbymain.cpp:1801
	{ "GAMEROOM_EXTRES.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	// lobbymain.cpp:1802
	{ "GAMEROOM_EXTRES.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	// lobbymain.cpp:1803
	{ "GAMEROOM_EXTRES.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	// lobbymain.cpp:1804
	{ "GAMEROOM_EXTRES.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	// lobbymain.cpp:1805
	{ "GAMEROOM_EXTRES.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	// lobbymain.cpp:1806
	{ "GAMEROOM_EXTRES.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	// lobbymain.cpp:1807
	{ "GAMEROOM_EXTRES.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	// lobbymain.cpp:1808
	{ "GAMEROOM_EXTRES.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	// lobbymain.cpp:1809
	{ "GAMEROOM_EXTRES.obar.onelinemsg_req", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	// lobbymain.cpp:1810
	{ "GAMEROOM_EXTRES.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	// lobbymain.cpp:1811
	{ "GAMEROOM_EXTRES.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	// lobbymain.cpp:1812
	{ "GAMEROOM_EXTRES.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	// lobbymain.cpp:1813
	{ "GAMEROOM_EXTRES.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	// lobbymain.cpp:1814
	{ "GAMEROOM_EXTRES.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1815
	{ "GAMEROOM_EXTRES.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1816
	{ "GAMEROOM_EXTRES.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	// lobbymain.cpp:1817
	{ "GAMEROOM_EXTRES.obar.messenger", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	// lobbymain.cpp:1818
	{ "GAMEROOM_EXTRES.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1819
	{ "GAMEROOM_EXTRES.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1820
	{ "GAMEROOM_EXTRES.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	// lobbymain.cpp:1821
	{ "GAMEROOM_EXTRES.obar.myinfo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	// lobbymain.cpp:1822
	{ "GAMEROOM_EXTRES.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1823
	{ "GAMEROOM_EXTRES.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1824
	{ "GAMEROOM_EXTRES.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	// lobbymain.cpp:1825
	{ "GAMEROOM_EXTRES.obar.option", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	// lobbymain.cpp:1826
	{ "GAMEROOM_EXTRES.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	// lobbymain.cpp:1827
	{ "GAMEROOM_EXTRES.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	// lobbymain.cpp:1829
	{ "GAMEROOM_EXTRES.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	// lobbymain.cpp:1830
	{ "GAMEROOM_EXTRES.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	// lobbymain.cpp:1831
	{ "GAMEROOM_EXTRES.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	// lobbymain.cpp:1832
	{ "GAMEROOM_EXTRES.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	// lobbymain.cpp:1833
	{ "GAMEROOM_EXTRES.obar.messenger_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	// lobbymain.cpp:1834
	{ "GAMEROOM_EXTRES.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	// lobbymain.cpp:1835
	{ "GAMEROOM_EXTRES.obar.gift_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	// lobbymain.cpp:1836
	{ "GAMEROOM_EXTRES.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	// lobbymain.cpp:1838
	{ "GAMEROOM_EXTRES.treasure_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnTreasureAlarmInit },
	{ "GAMEROOM_EXTRES.treasure_alarm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnTreasureAlarmUp },
	// lobbymain.cpp:1841
	{ "CREATE", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_Init },
	// lobbymain.cpp:1842
	{ "CREATE", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_Finish },
	// lobbymain.cpp:1843
	{ "CREATE", FRCMD_DESTROY, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_Destroy },
	// lobbymain.cpp:1845
	{ "CREATE.pet", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreatePetInit },
	// lobbymain.cpp:1846
	{ "CREATE.pet", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreatePetOwnerDraw },
	// lobbymain.cpp:1847
	{ "CREATE.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_CharInit },
	// lobbymain.cpp:1848
	{ "CREATE.charprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_CharPrevInit },
	// lobbymain.cpp:1849
	{ "CREATE.charnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_CharNextInit },
	// lobbymain.cpp:1850
	{ "CREATE.hair", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_HairInit },
	// lobbymain.cpp:1851
	{ "CREATE.hairprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_HairPrevInit },
	// lobbymain.cpp:1852
	{ "CREATE.hairprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_HairPrevBtnUp },
	// lobbymain.cpp:1853
	{ "CREATE.hairnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_HairNextInit },
	// lobbymain.cpp:1854
	{ "CREATE.hairnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_HairNextBtnUp },
	// lobbymain.cpp:1855
	{ "CREATE.shirt", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_ShirtsInit },
	// lobbymain.cpp:1856
	{ "CREATE.shirtprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_ShirtsPrevInit },
	// lobbymain.cpp:1857
	{ "CREATE.shirtprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_ShirtsPrevBtnUp },
	// lobbymain.cpp:1858
	{ "CREATE.shirtnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_ShirtsNextInit },
	// lobbymain.cpp:1859
	{ "CREATE.shirtnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_ShirtsNextBtnUp },
	// lobbymain.cpp:1860
	{ "CREATE.create", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CLobbyMain::OnCreate_CreateBtnUp },
	// lobbymain.cpp:1861
	{ "CREATE.desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_DescInit },
	// lobbymain.cpp:1862
	{ "CREATE.warning", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::OnCreate_WarningInit },
	// lobbymain.cpp:1863
	{ "CREATE.powerbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::On_PowerBarInit },
	// lobbymain.cpp:1864
	{ "CREATE.controlbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::On_ControlBarInit },
	// lobbymain.cpp:1865
	{ "CREATE.accuratebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::On_AccuracyBarInit },
	// lobbymain.cpp:1867
	{ "CREATE.spinbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::On_SpinBarInit },
	{ "CREATE.curvebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CLobbyMain::On_CurveBarInit },
	// lobbymain.cpp:1871
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// lobbymain.cpp:1879
bool FinishUserCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomPlayer* pLhs = (const sRoomPlayer*)lhs;
	const sRoomPlayer* pRhs = (const sRoomPlayer*)rhs;

	return WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(pLhs->m_uid)].m_finishOrder < WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(pRhs->m_uid)].m_finishOrder;
}

// lobbymain.cpp:1896
bool ConnectionUserCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomPlayer* pLhs = (const sRoomPlayer*)lhs;
	const sRoomPlayer* pRhs = (const sRoomPlayer*)rhs;

	return pLhs->m_connection < pRhs->m_connection;
}

// lobbymain.cpp:1918
CLobbyMain::CLobbyMain(void) {
	WSingleton<CSharedDoc>::Instance()->m_lobbyMessage = "";

	m_loginDlg = NULL;
	m_resultReportDlg = NULL;
	m_approachGiftDlg = NULL;
	m_resultDlg = NULL;
	m_waitDlg = NULL;
	m_createCharEdit = NULL;
	m_createDescEdit = NULL;
	m_createHairEdit = NULL;
	m_createShirtsEdit = NULL;
	m_pointEventDlg = NULL;
	m_worldTourEventDlg = NULL;
	m_findEbortEventDlg = NULL;
	m_extraEventDlg = NULL;
	m_overBarButtonDown = false;
	m_subSelectionPending = false;
	m_roomListDirty = false;
	m_charViewer = NULL;
	m_caddieViewer = NULL;
	m_aztecViewer = NULL;
	m_clubViewer = NULL;
	m_mascotViewer = NULL;
	m_equipItemViewer = NULL;
	m_charSelViewer = NULL;
	m_caddieSelViewer = NULL;
	m_aztecSelViewer = NULL;
	m_clubSelViewer = NULL;
	m_mascotSelViewer = NULL;
	m_roomMasterUid = -1;
	m_waitDlgTime = -10.0f;
	m_storedText[0] = 0;
	m_chatWndOpen = true;
	m_roomSortPending = false;
	m_userSort = USERSORT_DEFAULT;
	m_userSortPending = false;
	m_titleFont = NULL;
	m_userRankTabArea = NULL;
	m_caddieWarningPending = false;
	memset(m_warningCaddies, 0, sizeof(m_warningCaddies));

	ResetControl();

	m_scrollActive = false;

	m_autoStartCountdown = 15.0f;

	m_toppagePointEventBtn = NULL;

	m_loginBitmaps[0] = NULL;
	m_loginBitmaps[1] = NULL;
	m_loginBitmaps[2] = NULL;

	m_newTabBitmap = NULL;

	m_pendingDlg = NULL;
	m_useReportEnabled = false;

	m_toppageWorldTourBtn = NULL;

	m_secondPwdDlg = NULL;

	m_toppageHalloweenBtn = NULL;
	m_halloweenEventDlg = NULL;
	m_halloweenEvent2Dlg = NULL;

	m_toppageChristmasBtn = NULL;
	m_christmasEventDlg = NULL;

	m_toppageChristmasSockBtn = NULL;
	m_christmasSockEventDlg = NULL;

	m_toppageTicketExchangeBtn = NULL;
	m_ticketExchangeDlg = NULL;

	m_toppageMissionBtn = NULL;
	m_missionEventDlg = NULL;

	m_toppageBingoBtn = NULL;
	m_bingoEventDlg = NULL;

	m_toppageEvent1Btn = NULL;

	m_toppageExtraBtn = NULL;

	m_quickResultDlg = NULL;

	m_quickResultDlg2 = NULL;
	m_valEventDlg = NULL;
	m_createNickDlg = NULL;
	m_topPageBtnIndex = 0;
	m_toppageBtnScale = 1.0f;

	WSingleton<CIconManager>::Instance()->ClearInformation();
}

// lobbymain.cpp:2087
CLobbyMain::~CLobbyMain(void) {
	std::map<unsigned long, sRoomSlot>::iterator it;
	for (it = CSharedDoc::Instance()->m_roomSlotMap.begin(); it != CSharedDoc::Instance()->m_roomSlotMap.end(); ++it) {
		if (it->second.m_pPet) { delete it->second.m_pPet; it->second.m_pPet = NULL; }
	}

	this << MsgObject(0, 2, 0, 0, 0, 0, 0);
}

// lobbymain.cpp:2097
void CLobbyMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();

	WSingleton<CMouseCursor>::Instance()->SetAutoHide(false);
}

// lobbymain.cpp:2109
void CLobbyMain::OnLoad(void) {
	CTaskMain::OnLoad();
}

// lobbymain.cpp:2118
void CLobbyMain::OnInit(void) {
	g_pFresh->GetManager()->SetExclusiveKey(true);

	CTaskMain::OnInit();

	if (m_approachGiftDlg == NULL)
		m_approachGiftTimer = 1.0f;

	WPolySoup::sCamera* camera = GOLFDOC()->m_pPolySoup->FindCamera("DefaultCamera");
	if (camera != NULL) {
		g_camera = camera->m_matrix;
		g_view->SetFOV(camera->m_fov * 0.017453292f);
	}

	m_bgCameraPaused = false;
	m_bgCameraDirection = 1.0f;
	m_bgCameraMoveTime = g_CurrentTime;

	m_cateNewRoom[0] = 0;
	m_cateNewRoom[1] = 0;
	m_cateNewRoom[2] = 0;

	for (int i = 0; i < 3; i++) {
		m_loginBitmaps[i] = g_pFresh->GetBitmap(MakeStr("login_%02d", i));
	}

	m_newTabBitmap = g_pFresh->GetBitmap("tap_new2");
}

// lobbymain.cpp:2165
void CLobbyMain::OnDestroy(void) {
	CTaskMain::OnDestroy();
}

// lobbymain.cpp:2174
// Pans the lobby's background camera sideways for 80 seconds, waits 2 seconds
// and pans back, easing in and out over 2 seconds (0.3 units per second).
void CLobbyMain::ProcessLobbyBgCamera(float dt) {
	if (CSharedDoc::Instance()->m_3DBackgroundOff)
		return;

	if (m_bgCameraPaused) {
		if (g_CurrentTime - m_bgCameraPauseTime > 2000) {
			m_bgCameraPaused = false;
			m_bgCameraDirection *= -1.0f;
			m_bgCameraMoveTime = g_CurrentTime;
		}
		return;
	}

	unsigned long elapsed = g_CurrentTime - m_bgCameraMoveTime;
	if (elapsed > 80000) {
		m_bgCameraPaused = true;
		m_bgCameraPauseTime = g_CurrentTime;
		return;
	}

	float speed = 1.0f;
	if (elapsed < 2000)
		speed = elapsed * 0.0005f;
	else if (elapsed > 78000)
		speed = 1.0f - (elapsed - 78000) * 0.0005f;

	g_camera.pivot += g_camera.xa * m_bgCameraDirection * dt * 0.3f * speed;
}

// lobbymain.cpp:2215
// Administrator hotkeys: F10 in a game room acts on the selected user,
// Ctrl+Insert in the room list on the selected room.
void CLobbyMain::ProcessAdministrator(float) {
	if (CSharedDoc::Instance()->GetPangyaUserInfo().identityBits.administrator) {
		if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") == 0
			|| stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") == 0) {
			if (g_input->GetDown("FUNC_10", true) && m_selectedUserId != 0xFFFFFFFF) {
				WSendPacket packet((enumClientPacket)0x4C);
				packet.Encode4(m_selectedUserId);
				packet.Send(SEND_TO_DEFAULT);
			}
		}

		if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "ROOMLIST") == 0) {
			if ((g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false))
				&& g_input->GetDown("INSERT", true)) {
				if (m_selectedRoomData) {
					WSendPacket packet((enumClientPacket)0x5D);
					packet.Encode2(static_cast<sRoomInfo*>(m_selectedRoomData)->m_roomNumber);
					packet.Send(SEND_TO_DEFAULT);
				}
			}
		}
	}
}

// lobbymain.cpp:2251
// Dialog keys: left/right page the notice dialogs, up/down scroll the room
// list, Page Up/Down walk the chat history (chat input) or the chat targets
// (target edit); the chat input is coloured by the chat target.
void CLobbyMain::ProcessDialogKeyboard(float) {
	if (m_noticeDlg) {
		if (g_input->GetDown("\xC1\xC2", false))   // left
			m_noticeDlg->Prev();
		else if (g_input->GetDown("\xBF\xEC", false))   // right
			m_noticeDlg->Next();
	}

	if (m_pcbangNoticeDlg) {
		if (g_input->GetDown("\xC1\xC2", false))
			m_pcbangNoticeDlg->Prev();
		else if (g_input->GetDown("\xBF\xEC", false))
			m_pcbangNoticeDlg->Next();
	}

	if (m_guildNoticeDlg) {
		if (g_input->GetDown("\xC1\xC2", false))
			m_guildNoticeDlg->Prev();
		else if (g_input->GetDown("\xBF\xEC", false))
			m_guildNoticeDlg->Next();
	}

	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "ROOMLIST") == 0 && m_roomListBox && !m_emoticonDlg) {
		if (g_input->GetDown("\xBB\xF3", false))   // up
			m_roomListBox->GetScrollBar()->ScrollUp(1);

		if (g_input->GetDown("\xC7\xCF", false))   // down
			m_roomListBox->GetScrollBar()->ScrollDown(1);
	}

	if (m_chatTargetCombo && m_chatEdit) {
		CSharedDoc* doc = CSharedDoc::Instance();

		if (g_input->GetDown("PAGE_UP", true)) {
			if (m_chatEdit->HasKeyFocus() && doc->m_chatHistory.size() > 0) {
				const char* line = m_chatEdit->GetLine(1, false);
				std::list<std::string>::iterator it = std::find(CSharedDoc::Instance()->m_chatHistory.begin(), CSharedDoc::Instance()->m_chatHistory.end(), line);
				if (it != CSharedDoc::Instance()->m_chatHistory.end()) {
					if (it != CSharedDoc::Instance()->m_chatHistory.begin()) {
						--it;
						CChatMsg::Instance()->SetChatText(it->c_str(), false);
					}
				} else {
					CChatMsg::Instance()->SetChatText(CSharedDoc::Instance()->m_chatHistory.back().c_str(), false);
				}
			} else if (m_chatTargetCombo->HasKeyFocus() && doc->m_list4CD8.size() > 0) {
				CChatMsg::Instance()->SetChatText(doc->GetPrevChatTarget(m_chatTargetCombo->GetLine(1, false)), false);
			}
		} else if (g_input->GetDown("PAGE_DOWN", true)) {
			if (m_chatEdit->HasKeyFocus() && doc->m_chatHistory.size() > 0) {
				const char* line = m_chatEdit->GetLine(1, false);
				std::list<std::string>::iterator it = std::find(CSharedDoc::Instance()->m_chatHistory.begin(), CSharedDoc::Instance()->m_chatHistory.end(), line);
				if (it != CSharedDoc::Instance()->m_chatHistory.end()) {
					++it;
					if (it != CSharedDoc::Instance()->m_chatHistory.end())
						CChatMsg::Instance()->SetChatText(it->c_str(), false);
					else
						CChatMsg::Instance()->SetChatText("", false);
				}
			} else if (m_chatTargetCombo->HasKeyFocus() && doc->m_list4CD8.size() > 0) {
				CChatMsg::Instance()->SetChatText(doc->GetNextChatTarget(m_chatTargetCombo->GetLine(1, false)), false);
			}
		}

		if (strlen(m_chatTargetCombo->GetLine(1, false)) == 0
			|| strcmp(m_chatTargetCombo->GetLine(1, false), "\xB8\xF0\xB5\xCE\xBF\xA1\xB0\xD4") == 0) {   // "to all"
			m_chatEdit->SetFontColor(0xFFFFFFFF);
		} else if (strlen(m_chatTargetCombo->GetLine(1, false)) == 0
			|| strcmp(m_chatTargetCombo->GetLine(1, false), "\xB1\xE6\xB5\xE5\xBF\xA1\xB0\xD4") == 0) {   // "to the guild"
			m_chatEdit->SetFontColor(0xFF5BFFB0);
		} else {
			m_chatEdit->SetFontColor(0xFFCF67FF);
		}
	}
}

// lobbymain.cpp:2346
// Runs the room's pets and, in the game room, sends the local player's pet
// direction and motion changes to the others.
void CLobbyMain::ProcessLobbyPetSync(float dt) {
	std::map<unsigned long, sRoomSlot>::const_iterator it;
	for (it = CSharedDoc::Instance()->m_roomSlotMap.begin(); it != CSharedDoc::Instance()->m_roomSlotMap.end(); ++it) {
		if (it->second.m_pPet == NULL)
			continue;

		it->second.m_pPet->Process(dt, false);

		if (it->first == CSharedDoc::Instance()->GetPangyaUserInfo().guid
			&& stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") == 0) {
			if (it->second.m_pPet->IsDirectionChanged()) {
				float direction = it->second.m_pPet->GetDirection();

				WSendPacket packet((enumClientPacket)0x63);
				packet.Encode1(0);
				packet.EncodeBuffer(&direction, 4);
				packet.Send(SEND_TO_DEFAULT);
			}

			if (it->second.m_pPet->IsNextMotion()) {
				WSendPacket packet((enumClientPacket)0x63);
				packet.Encode1(1);
				packet.EncodeStr(std::string(it->second.m_pPet->GetMotionName()));
				packet.Send(SEND_TO_DEFAULT);
			}
		}
	}
}

// lobbymain.cpp:2416
// The lobby's per-frame UI: the login dialog's idle fade, the approach gift
// and result dialogs' timeout, chat hotkeys, the IME language mark, closing
// the category lists and the selection bars, the start tip blink, the
// result screen's clock, the away state, the Event1 button bobbing and the
// treasure hunt's approach gift.
void CLobbyMain::ProcessLobbyUI(float dt) {
	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "LOGIN") == 0 && !m_scrollActive) {
		if (m_loginDlg) {
			if ((FrLoginDlg::GetState() == 0 || FrLoginDlg::GetState() == 5)
				&& timeGetTime() - g_mouse->GetLastInputTime() > 60000
				&& timeGetTime() - g_ime->GetLastInputTime() > 60000) {
				m_loginDlg->SetFadeout(true);

				WSingleton<CMouseCursor>::Instance()->SetActive(false);
			}
		} else if ((g_mouse->IsUpdated() || g_ime->IsUpdated()) && !g_bQuit) {
			m_loginDlg = CreateForm<FrLoginDlg>(g_pFresh->GetManager(), this, "login", NULL);
			m_loginDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnLoginDlgResult, 10);
			WSingleton<CMouseCursor>::Instance()->SetActive(true);
		}
	}

	if (m_approachGiftTimer <= 0.0f) {
		m_approachGiftTimer += dt;

		bool expired = false;
		if (m_approachGiftTimer > 0.0f)
			expired = true;

		if (g_input->GetDown("ESCAPE", true) == 1) {
			FrUniteResultDlg* resultDlg = (FrUniteResultDlg*)m_resultReportDlg;
			if (resultDlg == NULL)
				expired = true;
			else if (resultDlg->m_escClosable && !resultDlg->CheckChildDlgVisible())
				expired = true;
		}

		if (expired) {
			m_approachGiftTimer = 1.0f;

			if (!m_levelupDlgs.empty()) {
				for (unsigned int i = 0; i < m_levelupDlgs.size(); i++) {
					if (m_levelupDlgs[i]) {
						m_levelupDlgs[i]->Close((eFormRet)0, true);
						m_levelupDlgs[i] = NULL;
					}
				}
				m_levelupDlgs.clear();
			}

			if (m_resultReportDlg)
				m_resultReportDlg->Close(false);

			if (CUserInfo::Instance() && CUserInfo::Instance()->GetDlg())
				CUserInfo::Instance()->Close();

			if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXTRES") == 0) {
				if (CSharedDoc::Instance()->m_roomInfo.m_roomMode != 14) {
					OpenLayout("GAMEROOM_EXT");
					HandleMsg(MsgObject(0, 0x0F, 0, 0, 0, 0, 0));
					HandleMsg(MsgObject(0, 0x2F, 0, 0, 0, 0, 0));
				} else {
					SendRoomExitPacket();
				}
			}
		}
	} else if (m_approachGiftDlg) {
		m_approachGiftDlg->Close(true);
		m_approachGiftDlg = NULL;
	}

	if ((g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false)) && g_input->GetDown("EMOTICON", false)) {
		if (!CRankingInfo::Instance()->GetDlg() && !CMessengerInfo::Instance()->IsVisible())
			OnEmoticonBtnUp();
	}

	if ((g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false)) && g_input->GetDown("REPLAY", false))
		OnGameRoom_StartBtnUp();

	if (m_languageArea) {
		if (g_ime->IsAlphaNumericMode() != data_ab2830) {
			data_ab2830 = g_ime->IsAlphaNumericMode();
			m_languageArea->SetBgImg(data_ab2830 ? "chat_eng" : "chat_kor");

			if (m_onelineReqDlg)
				((FrOnelineReqDlg*)m_onelineReqDlg)->SetLanguage(data_ab2830);
		}
	}

	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "ROOMLIST") == 0) {
		if (g_input->GetButton(LEFT_BUTTON) == 2)
			m_overBarButtonDown = true;

		if ((m_cateVSList && m_cateVSList->IsVisible())
			|| (m_cateMassList && m_cateMassList->IsVisible())
			|| (m_cateBattleList && m_cateBattleList->IsVisible())) {
			if (m_overBarButtonDown && g_input->GetButton(LEFT_BUTTON) == 1) {
				m_overBarButtonDown = false;
				m_cateVSList->SetVisible(false);
			}
		}
	}

	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") == 0
		|| stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") == 0) {
		if (g_input->GetButton(LEFT_BUTTON) == 2)
			m_overBarButtonDown = true;

		if (m_subSelectionPending && m_overBarButtonDown && g_input->GetButton(LEFT_BUTTON) == 1) {
			m_overBarButtonDown = false;
			m_subSelectionPending = false;
			SendChangedUserInfo();
			InitControlsSelectedUserInfo();
		}
	}

	if (m_tipBlinking) {
		m_tipBlinkTime += dt;
		if (m_tipBlinkTime > 0.2f) {
			m_tipBlinkTime -= 0.2f;
			m_tipBlinkCount++;

			if (m_blinkTipArea)
				m_blinkTipArea->SetVisible(!m_blinkTipArea->IsVisible());

			if (m_tipBlinkCount == 3) {
				m_tipBlinking = false;
				m_tipBlinkCount = 0;
			}
		}
	}

	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXTRES") == 0) {
		unsigned long endTime = CSharedDoc::Instance()->m_gameTimeLimit + CSharedDoc::Instance()->m_gameStartTime;
		const char* text;
		if (endTime > g_CurrentTime) {
			unsigned long seconds = (endTime - g_CurrentTime) / 1000;
			text = MakeStr("%02d:%02d", seconds / 60, seconds % 60);
		} else {
			text = "00:00";
		}
		HandleMsg(MsgObject(this, 0x10, (intptr_t)text, 0, 0, 0, 0));
	}

	if (m_waitDlgTime < 0.0f
		&& (stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") == 0
			|| stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") == 0)) {
		bool awayChanged = false;
		CSharedDoc* doc = CSharedDoc::Instance();

		if (m_away) {
			if (g_mouse->IsUpdated() || g_ime->IsUpdated()) {
				awayChanged = true;
				m_away = false;
			}

			// Five minutes away: the room master is told to kick the player.
			if (timeGetTime() - g_mouse->GetLastInputTime() > 300000
				&& timeGetTime() - g_ime->GetLastInputTime() > 300000) {
				std::list<sRoomPlayer>::iterator it;
				for (it = doc->m_roomPlayerList.begin(); it != doc->m_roomPlayerList.end(); ++it) {
					if (it->m_uid == doc->GetPangyaUserInfo().guid)
						break;
				}

				if (it != doc->m_roomPlayerList.end() && (it->m_attribute & 0x18)) {
					WSendPacket packet((enumClientPacket)0x0F);
					packet.Encode1(0);
					packet.Encode2(0xFFFF);
					packet.Encode8(0);
					packet.Encode8(0);
					packet.Send(SEND_TO_DEFAULT);

					this << MsgObject(0, 1, 0, 0, 0, 0, 0);
				}
			}
		} else if (timeGetTime() - g_mouse->GetLastInputTime() > 60000
			&& timeGetTime() - g_ime->GetLastInputTime() > 60000) {
			m_away = true;
			awayChanged = true;
		}

		if (awayChanged) {
			std::list<sRoomPlayer>::iterator it;
			for (it = doc->m_roomPlayerList.begin(); it != doc->m_roomPlayerList.end(); ++it) {
				if (it->m_uid == doc->GetPangyaUserInfo().guid)
					break;
			}

			if (it == doc->m_roomPlayerList.end())
				return;

			if (it->m_attribute & 0x18) {
				WSendPacket packet((enumClientPacket)0x0A);
				packet.Encode2(0xFFFF);
				packet.Encode1(1);
				packet.Encode1(9);
				packet.Encode1(m_away != false);
				packet.Send(SEND_TO_DEFAULT);
			}

			WSendPacket packet((enumClientPacket)0x32);
			packet.Encode1(m_away != false);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	if (IsLocalContent((localContentType_t)0x18) && !IsLocalContent((localContentType_t)0x48)) {
		if (m_toppageEvent1Btn && m_toppageEvent1Btn->IsVisible()) {
			WRect rect = m_toppageEvent1Btn->GetRect();
			float top = dt * m_toppageBtnScale * 8.0f + rect.y;

			if ((top > m_event1IconRect.y + 15.0f && m_toppageBtnScale > 0.0f)
				|| (top < m_event1IconRect.y - 15.0f && m_toppageBtnScale < 0.0f))
				m_toppageBtnScale *= -1.0f;

			WPoint pt;
			pt.x = rect.x;
			pt.y = top;
			m_toppageEvent1Btn->MoveWindow(pt);
		}
	}

	if (CSharedDoc::Instance()->m_value5D08 == 2 && CSharedDoc::Instance()->m_roomType == 10 && m_approachGiftDlg == NULL) {
		unsigned char giftCount = CTHunter::Instance()->GetGiftSize();
		if (giftCount <= 8)
			m_approachGiftTimer = -9.0f;
		else if (giftCount <= 16)
			m_approachGiftTimer = -12.0f;
		else
			m_approachGiftTimer = -20.0f;

		FrTreasureGift* dlg = CreateForm<FrTreasureGift>(g_pFresh->GetManager(), this, "trgift", NULL);
		m_approachGiftDlg = dlg;
		dlg->SetApproachMode();
		m_approachGiftDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnApproachGiftDlgResult, 0x0C);
		m_approachGiftDlg->SetTimeLimit(12.0f);
		m_approachGiftDlg->EnableDrag(false);
		m_approachGiftDlg->SetFixed(true);
		CSharedDoc::Instance()->m_value5D08 = 0;
	}
}

// lobbymain.cpp:2817
// The game room's auto start: once enough players are in a non-guild room
// that allows it, counts down m_autoStartCountdown, announcing the last five
// seconds, and starts the game for the room master at zero.
void CLobbyMain::ProcessAutoStart(float dt) {
	sRoomInfo& room = CSharedDoc::Instance()->m_roomInfo;

	if (m_autoStartCountdown > 0.0f
		&& stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") == 0
		&& (m_roomOptionD || m_autoStartArmed)) {
		// TODO: the dword at CSharedDoc 0x4B4F (bit 1: 5 players are enough)
		// overlaps m_serverChannel's name in the current SharedDoc layout.
		int minPlayers = (CSharedDoc::Instance()->m_serverChannel.name[0x3D] & 2) ? 5 : 10;
		int needed = room.m_userLimit * 3 / 4;
		if (minPlayers >= needed)
			needed = minPlayers;

		if (room.m_userCount >= needed && room.m_gameType != 6 && room.m_option[1] == 0) {
			float left = m_autoStartCountdown - dt;
			m_autoStartCountdown = left;

			const char* text;
			if (left < 6.0f && left + dt > 6.0f) {
				// TODO: the text runs on past its first 32 bytes (77 in all).
				text = "(\xBE\xCB\xB8\xB2)\xC2\xFC\xBF\xA9\xC0\xCE\xBF\xF8\xC0\xCC \xC0\xFC\xBF\xF8 \xC1\xD8\xBA\xF1\xBF\xCF\xB7\xE1\xB0\xA1";
			} else if (left < 5.0f && left + dt > 5.0f) {
				text = "(\xBE\xCB\xB8\xB2)\xB0\xD4\xC0\xD3\xBD\xC3\xC0\xDB 5\xC3\xCA\xC0\xFC\xC0\xD4\xB4\xCF\xB4\xD9.";
			} else if (left < 4.0f && left + dt > 4.0f) {
				text = "(\xBE\xCB\xB8\xB2)\xB0\xD4\xC0\xD3\xBD\xC3\xC0\xDB 4\xC3\xCA\xC0\xFC\xC0\xD4\xB4\xCF\xB4\xD9.";
			} else if (left < 3.0f && left + dt > 3.0f) {
				text = "(\xBE\xCB\xB8\xB2)\xB0\xD4\xC0\xD3\xBD\xC3\xC0\xDB 3\xC3\xCA\xC0\xFC\xC0\xD4\xB4\xCF\xB4\xD9.";
			} else if (left < 2.0f && left + dt > 2.0f) {
				text = "(\xBE\xCB\xB8\xB2)\xB0\xD4\xC0\xD3\xBD\xC3\xC0\xDB 2\xC3\xCA\xC0\xFC\xC0\xD4\xB4\xCF\xB4\xD9.";
			} else if (left < 1.0f && left + dt > 1.0f) {
				m_autoStartArmed = false;
				text = "(\xBE\xCB\xB8\xB2)\xB0\xD4\xC0\xD3\xBD\xC3\xC0\xDB 1\xC3\xCA\xC0\xFC\xC0\xD4\xB4\xCF\xB4\xD9.";
			} else {
				if (left < 0.0f && m_roomMasterUid == CSharedDoc::Instance()->GetPangyaUserInfo().guid
					&& CSharedDoc::Instance()->m_roomInfo.m_gameType != 6)
					OnGameRoom_StartBtnUp();
				return;
			}

			this << MsgObject(0, 3, (intptr_t)text, 0xFFFF7878, 0, 0, 0);
			return;
		}
	}

	if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 6)
		m_autoStartCountdown = 6.0f;
	else
		m_autoStartCountdown = 15.0f;
}

// lobbymain.cpp:2856
void CLobbyMain::OnProcess(float dt) {
	CTaskMain::OnProcess(dt);

	ProcessLobbyBgCamera(dt);
	ProcessLobbyPetSync(dt);

	if (IsLocalContent((localContentType_t)0x0A))
		ProcessAdministrator(dt);

	ProcessDialogKeyboard(dt);
	ProcessLobbyUI(dt);
	ProcessAutoStart(dt);
	ResetControlsRoomListSort(dt);

	if (IsLocalContent((localContentType_t)0x4C) && g_pFresh->IsCurrentLayout("TOPPAGE"))
		AnimateWebEventButton(dt);

	if (IsLocalContent((localContentType_t)0x57))
		RefreshAllUccClothes();
}

// lobbymain.cpp:2889
// Cycles the Christmas event button through its "xmas_main_i<n>" frames
// (1, 2, 3, 2), one every 0.3 seconds.
void CLobbyMain::AnimateWebEventButton(float dt) {
	static int s_frame;
	static int s_frameIndex;
	static float s_frameTime;

	s_frameTime += dt;
	if (s_frameTime > 0.3f) {
		int frames[4] = { 1, 2, 3, 2 };

		s_frameTime = 0.0f;
		if (++s_frameIndex > 3)
			s_frameIndex = 0;
		s_frame = frames[s_frameIndex];

		char number[4] = { 0, };
		std::string image("xmas_main_i");
		image += itoa(s_frame, number, 10);

		if (m_toppageChristmasBtn) {
			m_toppageChristmasBtn->SetButtonImg(image.c_str(), FrButton::NORMAL);
			m_toppageChristmasBtn->SetButtonImg(image.c_str(), FrButton::OVER);
		}
	}
}

// lobbymain.cpp:2917
void CLobbyMain::OnDisplay(void) {
}

// lobbymain.cpp:2921
void CLobbyMain::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	// TODO(boot): 0x5B: lobbymain.cpp:2945, a room slot exhibition motion/direction (CSharedDoc 0x12B8 map of sRoomSlot, CExhibition::SetMotion/SetDirection)
	// TODO(boot): 0x59: lobbymain.cpp:2968, delete the room slot exhibitions and clear the map, re-enable the pet areas
	// TODO(boot): 0x5A: lobbymain.cpp:2981, set a room slot (sRoomSlot from param1/param2) and ShowPet
	// TODO(boot): 0x4F: lobbymain.cpp:2999, ask to remove the friend m_removeFriendUid ("notify_yesno", OnRemoveFriendDlgResult)

	case 0x44:
		// lobbymain.cpp:3017
		OpenEventPrizeForm((unsigned char)msg.m_param1);
		break;

	case 0: {
		// lobbymain.cpp:3021, open a layout
		if (g_pFresh) {
			const char* name = (const char*)msg.m_param1;

			if (!g_pFresh->IsCurrentLayout(name)) {
				CloseAllDialogs();
				ResetControl();

				if (stricmp(name, "TOPPAGE") == 0) {
					CIconManager::Instance()->ClearInformation();
					AfxGetTask()->SetWhisper(false);
				} else {
					AfxGetTask()->SetWhisper(true);
				}

				g_pFresh->OpenLayout(name, this, false);
				WSingleton<CMouseCursor>::Instance()->Toggle(true);
				CheckTikiReport(name);
			}
		}
		break;
	}

	case 0x03:
		// lobbymain.cpp:3088, a chat line: kept for the chat view outside
		// the game room layouts
		if (strcmpi(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM")
			&& strcmpi(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT")
			&& strcmpi(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXTRES")) {
			sChatLine line;
			line.m_text = (const char*)msg.m_param1;
			line.m_color = msg.m_param2;
			CSharedDoc::Instance()->m_chatLineList.push_back(line);
		}
		if (m_chatView)
			m_chatView->AddLine((const char*)msg.m_param1, msg.m_param2, false);
		break;

	// TODO(boot): 0x60: lobbymain.cpp:3104
	case 0x0A: {
		// lobbymain.cpp:3138, add a lobby user (hidden administrators
		// only show to themselves or to administrators)
		sBriefUserInfo* info = (sBriefUserInfo*)msg.m_param1;
		CSharedDoc* doc = CSharedDoc::Instance();
		if (info->uid == doc->m_userInfo.guid && doc->m_userInfo.IsIdentity(4)) {
			if (info->lobbyFlags & 4)
				break;
		}
		if (info->uid != doc->m_userInfo.guid && !doc->m_userInfo.identityBits.administrator
			&& (info->identity & 0x14)) {
			if (!info->invisible)
				break;
		}
		if (m_userListBox) {
			m_userListBox->AddItem(info);
			RoomList_SortUserList(m_userSort);
		}
		if (m_userListDlg) {
			m_userListDlg->AddUser((void*)msg.m_param1);
			m_userListDlg->SortUserList();
		}
		break;
	}

	case 0x0C: {
		// lobbymain.cpp:3174, remove a lobby user
		sBriefUserInfo* info = (sBriefUserInfo*)msg.m_param1;
		CSharedDoc* doc = CSharedDoc::Instance();
		if (info->uid == doc->m_userInfo.guid && doc->m_userInfo.IsIdentity(4)) {
			if (info->lobbyFlags & 4)
				break;
		}
		if (m_userListBox)
			m_userListBox->DelItem(info, msg.m_param2);
		if (m_userListDlg)
			m_userListDlg->DelUser((void*)msg.m_param1, msg.m_param2);
		if (info->uid == m_selectedUserId) {
			m_selectedUserId = -1;
			m_removeFriendUid = -1;
			if (m_userActionBtn)
				m_userActionBtn->Enable(false);
		}
		break;
	}

	case 0x0B:
		// lobbymain.cpp:3204, clear the lobby user list
		if (m_userListBox)
			m_userListBox->ClearItem();
		m_selectedUserId = -1;
		m_removeFriendUid = -1;
		break;

	case 0x04: {
		// lobbymain.cpp:3213, add a room to the room list (filtered by the
		// current game type, else by the current category)
		if (m_roomListBox) {
			sRoomInfo* room = (sRoomInfo*)msg.m_param1;
			if (data_aa9eac[3] != -1) {
				if (room->m_gameType == data_aa9eac[3])
					m_roomListBox->AddItem(room);
			} else if (GetCateByGameType(room->m_gameType) == data_aa9eac[2] || data_aa9eac[2] == ROOMCATE_ALL) {
				m_roomListBox->AddItem(room);
			}
		}
		break;
	}

	case 0x05: {
		// lobbymain.cpp:3228, remove a room from the room list, keeping
		// at least 6 lines
		if (m_roomListBox) {
			sRoomInfo* room = (sRoomInfo*)msg.m_param1;
			if (data_aa9eac[3] != -1) {
				if (room->m_gameType == data_aa9eac[3]) {
					m_roomListBox->DelItem(room, msg.m_param2);
					for (int i = m_roomListBox->GetCurrentItemSize(true); i < 6; i++)
						m_roomListBox->AddItem(NULL);
				}
			} else if (GetCateByGameType(room->m_gameType) == data_aa9eac[2] || data_aa9eac[2] == ROOMCATE_ALL) {
				m_roomListBox->DelItem(room, msg.m_param2);
				for (int i = m_roomListBox->GetCurrentItemSize(true); i < 6; i++)
					m_roomListBox->AddItem(NULL);
			}
		}
		break;
	}

	case 0x06:
		// lobbymain.cpp:3251, clear the room list
		if (m_roomListBox)
			m_roomListBox->ClearItem();
		break;

	case 0x0D: {
		// lobbymain.cpp:3257, pad the room list to 6 lines (or drop empty
		// lines past 6), then sort it
		if (m_roomListBox) {
			int size = m_roomListBox->GetCurrentItemSize(true);
			if (size <= 6) {
				for (; size < 6; size++)
					m_roomListBox->AddItem(NULL);
			} else {
				int extra = size - 6;
				FrListBox::ITEM_LIST::iterator it = m_roomListBox->m_itemList.begin();
				while (it != m_roomListBox->m_itemList.end() && extra > 0) {
					FrListBox::ITEM_LIST::iterator cur = it++;
					if ((*cur)->pData == NULL) {
						m_roomListBox->_DelItem(cur);
						extra--;
					}
				}
			}
			RoomList_SortRoomList(m_roomSort, 0);
		}
		break;
	}

	case 0x07: {
		// lobbymain.cpp:3293, a player entered the game room
		if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 5 || CSharedDoc::Instance()->m_roomInfo.m_gameType == 6) {
			if (CSharedDoc::Instance()->m_gameOver) {
				sSlotInfo* slot = (sSlotInfo*)msg.m_param1;
				if (slot->m_attribute & 3)
					m_slotList2.push_back(slot);
				else
					m_slotList1.push_back(slot);
				RemakeTeam(false);
			} else {
				RemakeTeam(true);
			}
		} else if (m_roomUserListBox) {
			m_roomUserListBox->AddItem((void*)msg.m_param1);
			m_roomUserListBox->SortItem(ConnectionUserCompare);
		}

		if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 4 || CSharedDoc::Instance()->m_roomInfo.m_gameType == 10) {
			// lobbymain.cpp:3322, drop one empty line past 24
			if (m_roomUserListBox && m_roomUserListBox->GetCurrentItemSize(true) > 24) {
				FrListBox::ITEM_LIST::iterator it;
				for (it = m_roomUserListBox->m_itemList.begin(); it != m_roomUserListBox->m_itemList.end(); ++it) {
					if ((*it)->pData == NULL) {
						m_roomUserListBox->_DelItem(it);
						break;
					}
				}
				m_roomUserListBox->SortItem(ConnectionUserCompare);
			}
			break;
		}
		if (IsMassGame(CSharedDoc::Instance()->m_roomInfo.m_gameType))
			break;

		// lobbymain.cpp:3341, the player's slot (pet area, character)
		sSlotInfo* player = (sSlotInfo*)msg.m_param1;
		sRoomSlot slot;
		// TODO(boot): sRoomSlot 0x0 = m_petArea[player->m_connection - 1]
		// and the angel wing bits at 0x28C (player->m_flags57) need the
		// members named in SharedDoc.h
		slot.m_pPet = NULL;
		slot.m_characterInfo = *(sCharacterInfo*)msg.m_param2;
		CSharedDoc::Instance()->m_roomSlotMap[player->m_uid] = slot;
		if (IsLocalContent((localContentType_t)0x54)) {
			if (!(player->m_flags151.flag & 1))
				ShowPet(player->m_uid);
		} else {
			ShowPet(player->m_uid);
		}

		// lobbymain.cpp:3365, drop one empty line past 4
		if (m_roomUserListBox && m_roomUserListBox->GetCurrentItemSize(true) > 4) {
			FrListBox::ITEM_LIST::iterator it;
			for (it = m_roomUserListBox->m_itemList.begin(); it != m_roomUserListBox->m_itemList.end(); ++it) {
				if ((*it)->pData == NULL) {
					m_roomUserListBox->_DelItem(it);
					break;
				}
			}
			m_roomUserListBox->SortItem(ConnectionUserCompare);
		}
		break;
	}

	case 0x08: {
		// lobbymain.cpp:3396, a player left the game room
		unsigned long uid = msg.m_param2;
		if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 5 || CSharedDoc::Instance()->m_roomInfo.m_gameType == 6) {
			if (CSharedDoc::Instance()->m_gameOver) {
				std::list<sSlotInfo*>& slots = msg.m_param3 ? m_slotList2 : m_slotList1;
				std::list<sSlotInfo*>::iterator it;
				for (it = slots.begin(); it != slots.end(); ++it) {
					if ((*it)->m_uid == uid) {
						slots.erase(it);
						break;
					}
				}
				RemakeTeam(false);
			} else {
				RemakeTeam(true);
			}
		} else if (m_roomUserListBox && msg.m_param1) {
			m_roomUserListBox->DelItem((void*)msg.m_param1);
		}

		if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 4 || CSharedDoc::Instance()->m_roomInfo.m_gameType == 10) {
			// lobbymain.cpp:3440, keep 24 lines
			if (m_roomUserListBox && m_roomUserListBox->GetCurrentItemSize(true) < 24)
				m_roomUserListBox->AddItem(NULL);
		} else if (!IsMassGame(CSharedDoc::Instance()->m_roomInfo.m_gameType)) {
			HidePet(uid);
			std::map<unsigned long, sRoomSlot>::iterator it = CSharedDoc::Instance()->m_roomSlotMap.find(uid);
			if (it != CSharedDoc::Instance()->m_roomSlotMap.end()) {
				// TODO(boot): lobbymain.cpp:3453, the slot's pet area
				// (sRoomSlot 0x0) SetMouseEvent(false)
				CSharedDoc::Instance()->m_roomSlotMap.erase(it);
			}
			// TODO(boot): lobbymain.cpp:3460, re-place the remaining pets
			// (sRoomSlot 0x0 = m_petArea[connection - 1], SetCenter on the
			// area's centre, AttachAngelWingFx for bit 1 of 0x28C)
			if (m_roomUserListBox && m_roomUserListBox->GetCurrentItemSize(true) < 4)
				m_roomUserListBox->AddItem(NULL);
		}

		if (uid == m_selectedUserId) {
			m_selectedUserId = -1;
			m_removeFriendUid = -1;
		}
		break;
	}

	case 0x09: {
		// lobbymain.cpp:3495, a room player's angel wing bits changed
		sRoomPlayer* player = (sRoomPlayer*)msg.m_param1;
		// TODO(boot): lobbymain.cpp:3502, copy bits 0-1 of m_angelFlag into
		// the room slot's 0x28C (not named in sRoomSlot yet)
		if (m_roomUserListBox) {
			FrListBox::ITEM_LIST::iterator it;
			for (it = m_roomUserListBox->m_itemList.begin(); it != m_roomUserListBox->m_itemList.end(); ++it) {
				sRoomPlayer* item = (sRoomPlayer*)(*it)->pData;
				if (item && player->m_uid == item->m_uid) {
					item->m_angelFlag = (item->m_angelFlag & ~3) | (player->m_angelFlag & 3);
					break;
				}
			}
		}
		break;
	}

	case 0x0E:
		// lobbymain.cpp:3532, rebuild the game room's user list
		if (CSharedDoc::Instance()->m_roomInfo.m_userCount > 1) {
			m_tipBlinking = true;
			m_tipBlinkTime = -10.0f;
			m_blinkTipArea = m_readyTipArea;
		} else {
			m_blinkTipArea = m_startTipArea;
		}
		MakeRoomUserList();
		this << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		break;

	// TODO(boot): 0x0F: lobbymain.cpp:3554, find the room master (identity bit 2, else the 0x18 attribute in CSharedDoc::m_roomPlayerList2 (protected) then m_roomPlayerList) into m_roomMasterUid; master: m_blinkTipArea = m_startTipArea, clear m_roomOptionA/B; m_mapBtn->Enable(roomMode != 14 && master), SetStartBtn, SetRoomControls, m_roomControlWnd->Enable(master && gameType != 6)

	// TODO(boot): 0x71: lobbymain.cpp:3628
	// TODO(boot): 0x10: lobbymain.cpp:3635
	// TODO(boot): 0x11: lobbymain.cpp:3711

	case 0x12: {
		// lobbymain.cpp:3756, reload the hole selection
		ReLoadHoleItem();
		if (m_holeCombo) {
			int index = msg.m_param1;
			if (CSharedDoc::Instance()->m_roomInfo.m_holeCount == 18) {
				if (index > 0)
					index = 1;
			}
			m_holeCombo->SelectItem(index);
		}
		break;
	}

	case 0x16:
		// lobbymain.cpp:3771
		m_storedFlag = msg.m_param1 != 0;
		break;

	case 0x17:
		// lobbymain.cpp:3775
		*(int*)msg.m_param1 = m_storedFlag;
		break;

	case 0x18: {
		// lobbymain.cpp:3780, start the game
		std::list<sRoomPlayer>::iterator it;
		for (it = CSharedDoc::Instance()->m_roomPlayerList.begin(); it != CSharedDoc::Instance()->m_roomPlayerList.end(); ++it)
			it->m_attribute &= ~4;
		CSharedDoc::Instance()->m_gameOver = false;
		WSingleton<CMouseCursor>::Instance()->m_mode = CMouseCursor::SCREEN_GAME;
		WSingleton<CMouseCursor>::Instance()->SetActive(false);
		CTaskManager::Instance()->ChangeTask("CGolfTask", "CGolfDoc", false);
		break;
	}

	case 0x1E:
		// lobbymain.cpp:3792, login result state (CLobbyTask)
		if (m_loginDlg) {
			FrLoginDlg::SetState((eLoginState)msg.m_param1);
			break;
		}
		if (msg.m_param1 == 0xD)
			CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"CREATE", 0, 0, 0);
		break;

	case 0x1F:
		// lobbymain.cpp:3806, game server login state
		if (m_serverDlg)
			m_serverDlg->SetState((gsLoginState_t)msg.m_param1);
		break;

	// TODO(boot): 0x42: lobbymain.cpp:3815, skinned theme (GetCurrentSkinnedTheme, FrDesktop::SetWallPaper, top page button images)
	// TODO(boot): 0x30: lobbymain.cpp:3850, invite dialog (IsLocalContent 0x54, "invite" form, SetInvaiteInfo, OnInviteDlgResult)

	case 0x31: {
		// lobbymain.cpp:3880, the banish vote (not for the room master nor
		// the player it is about)
		unsigned long guid = CSharedDoc::Instance()->GetPangyaUserInfo().guid;
		if (guid != m_roomMasterUid && guid != (unsigned long)msg.m_param2) {
			FrForm* form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "banish_vote", NULL);
			form->SetMessage((const char*)msg.m_param1, false);
			form->SetTimeLimit(5.0f);
			form->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnVoteDlgResult, 1);
		}
		break;
	}

	case 0x23: {
		// lobbymain.cpp:3892, a topmost notice
		FrForm* form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		form->SetMessage((const char*)msg.m_param1, false);
		form->SetTimeLimit(5.0f);
		form->Open((FRESH_RESULT_CALLBACK)0, 3);
		form->SetTopmost(true);
		form->FindNextTopFocus(false);
		break;
	}

	case 0x25: {
		// lobbymain.cpp:3909, a notice that quits
		FrForm* form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		form->SetMessage((const char*)msg.m_param1, false);
		form->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnNotifyQuitDlgResult, 3);
		break;
	}

	case 0x27:
		// lobbymain.cpp:3916
		strcpy(m_storedText, (const char*)msg.m_param1);
		break;

	case 0x28:
		// lobbymain.cpp:3920
		*(char**)msg.m_param1 = m_storedText;
		break;

	// TODO(boot): 0x29: lobbymain.cpp:3926
	// TODO(boot): 0x46: lobbymain.cpp:4017, team change (btn_teamchange_*, RemakeTeam)
	// TODO(boot): 0x49: lobbymain.cpp:4066, character parts (BuildMyPartTidList, SetLevelBar)
	// TODO(boot): 0x4A, 0x4B, 0x4C, 0x4E: lobbymain.cpp:4115-4157, copy the local player records into CSharedDoc (0x1DF8, 0x1E11, 0x634/0x1524, 0x1E23) when param1 is the local id, then SetLevelBar
	case 0x2B:
		// lobbymain.cpp:4160, lock the room controls (param1 == 1)
		m_roomOptionC = msg.m_param1 == 1;
		if (m_startBtn)
			m_startBtn->Enable(!m_roomOptionC);
		if (m_channelRoomCloseBtn)
			m_channelRoomCloseBtn->Enable(!m_roomOptionC);
		m_tipBlinking = !m_roomOptionC;
		if (m_blinkTipArea)
			m_blinkTipArea->SetVisible(!m_roomOptionC);
		break;

	case 0x2C:
		// lobbymain.cpp:4179, one part of the owned item lists arrived; all
		// five (bits 0-4) in: sort them
		CSharedDoc::Instance()->m_lobbyStateValue |= msg.m_param1;
		if (CSharedDoc::Instance()->m_lobbyStateValue == 0x1F)
			CSharedDoc::Instance()->SortAllMyItemList();
		break;

	// TODO(boot): 0x2A: lobbymain.cpp:4187, SortRank() (CLobbyMain::SortRank 0x7e7cc0 not declared yet)
	// TODO(boot): 0x2D: lobbymain.cpp:4192, game results (CSharedDoc 0x455C/0x49E0, SortRank, "uniteresult" FrUniteResultDlg into m_resultReportDlg, OnResultDlgResult)

	case 0x2E:
		// lobbymain.cpp:4278
		m_chatWndOpen = false;
		break;

	case 0x32:
		// lobbymain.cpp:4282, rebuild the lobby user dialog
		if (m_userListDlg)
			m_userListDlg->MakeUserList(CSharedDoc::Instance()->m_userMap);
		break;

	case 0x33:
		// lobbymain.cpp:4288, room enter answer (1: the room does not exist)
		if (msg.m_param1 == 1) {
			this << MsgObject(this, 0x23, (intptr_t)"\xC7\xF6\xC0\xE7 \xC1\xB8\xC0\xE7\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xB9\xE6\xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
			break;
		}
		EnableRoomEnterControls(false);
		if (m_dtRoomDlg == NULL && m_roomListBox && m_roomListBox->GetSelected()) {
			FrDtRoomDlg* dlg = CreateForm<FrDtRoomDlg>(g_pFresh->GetManager(), this, "detailroominfo", NULL);
			m_dtRoomDlg = dlg;
			dlg->SetRoomInfo(*(sRoomInfo*)m_roomListBox->GetSelected()->pData);
			m_dtRoomDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnDtRoomDlgResult, 1);
		}
		break;

	case 0x34:
		// lobbymain.cpp:4313, the equipped consumables
		memcpy(CSharedDoc::Instance()->m_equipItem, (const void*)msg.m_param1, sizeof(CSharedDoc::Instance()->m_equipItem));
		break;

	case 0x35:
		// lobbymain.cpp:4329, posted with "TOPPAGE" once the channel is
		// entered from the server list
		if (IsLocalContent((localContentType_t)0x10))
			OpenParanNickChange();
		else
			OpenCaddieWarningForm();
		break;

	case 0x37:
		// lobbymain.cpp:4365, whisper to a player: select them as the
		// chat target and focus the chat input
		if (CSharedDoc::Instance()->AddWhisperPartner(m_chatTargetCombo, (const char*)msg.m_param1))
			m_chatTargetCombo->SetLine(1, (const char*)msg.m_param1, 0, false, 0);
		if (CUserInfo::Instance()->IsVisible())
			CUserInfo::Instance()->Close();
		if (m_userListDlg)
			m_userListDlg->Close(true);
		if (m_chatEdit)
			m_chatEdit->SetKeyFocus(true);
		break;

	case 0x38:
		// lobbymain.cpp:4377, add a whisper partner
		CSharedDoc::Instance()->AddWhisperPartner(m_chatTargetCombo, (const char*)msg.m_param1);
		break;

	case 0x47:
		// lobbymain.cpp:4381, refill the chat target list
		CSharedDoc::Instance()->InitRecentWhisperList(m_chatTargetCombo);
		CSharedDoc::Instance()->InitReservedChatList(m_chatTargetCombo, (eReservedChatPartner)0);
		break;

	// TODO(boot): 0x3A: lobbymain.cpp:4387, walks CSharedDoc::m_channelList when the byte at CSharedDoc 0x4AFD has bit 1 (the loop does nothing)
	// TODO(boot): 0x2F: lobbymain.cpp:4404
	// TODO(boot): 0x3B: lobbymain.cpp:4454
	// TODO(boot): 0x3D: lobbymain.cpp:4599, set or clear bit 2 of the attribute of the room player found by param1 (param2 == 1)

	case 0x3E:
		// lobbymain.cpp:4609
		OpenCreateInfo(msg.m_param1, (const char*)msg.m_param2);
		break;

	case 0x3F:
		// lobbymain.cpp:4615, hand out the chat input line
		*(const char**)msg.m_param1 = m_chatEdit ? m_chatEdit->GetLine(1, false) : NULL;
		break;

	case 0x40:
		// lobbymain.cpp:4620, nickname change result (0: done)
		if (IsLocalContent((localContentType_t)0x10) && m_paranNickChangeDlg) {
			if (msg.m_param1 == 0)
				m_paranNickChangeDlg->CloseDlg();
			else
				m_paranNickChangeDlg->EnableControls(true);
		}
		if (m_optionDlg) {
			if (msg.m_param1 == 0)
				m_optionDlg->CloseDlg();
			else
				m_optionDlg->EnableChangeNickControls(true);
		}
		break;

	// TODO(boot): the other cases of the original fall to the default for
	// now: 0x51-0x53, 0x56, 0x58, 0x5C, 0x5E, 0x5F, 0x61, 0x6D-0x70
	// (lobbymain.cpp:4646-5069), 0xB3 (4784), 0x1F7 (4808, GameExtResQuit),
	// 0x12D (4264, "levelupitem" FrLevelupItemForm), 0x201 and the ids
	// 0x21D-0x251 (4895-5260).

	case 0x41:
	case 0x6B:
	case 0xB2:
	case 0xB4:
	case 0x211:
	case 0x21B:
		// ignored by the original (no HandleMsgCommon)
		PG_BOOT_TODOF(("lobbymain.cpp:4620 empty case %#x (of 0x41, 0x6B, 0xB2, 0xB4, 0x211, 0x21B)", (unsigned)(msg.m_msg)));
		break;

#ifdef PG_BOOT_TRACE // trace cases for the TODO(boot) ids above (CMake build only)
		goto pg_boot_todo_default_1; // a case falling through to the default
	case 0x5B:
		PG_BOOT_TODO("msg 0x5B: lobbymain.cpp:2945, a room slot exhibition motion/direction (CSharedDoc 0x12B8 map of sRoomSlot, CExhibition::SetMotion/SetDirection)");
		goto pg_boot_todo_default_1;
	case 0x59:
		PG_BOOT_TODO("msg 0x59: lobbymain.cpp:2968, delete the room slot exhibitions and clear the map, re-enable the pet areas");
		goto pg_boot_todo_default_1;
	case 0x5A:
		PG_BOOT_TODO("msg 0x5A: lobbymain.cpp:2981, set a room slot (sRoomSlot from param1/param2) and ShowPet");
		goto pg_boot_todo_default_1;
	case 0x4F:
		PG_BOOT_TODO("msg 0x4F: lobbymain.cpp:2999, ask to remove the friend m_removeFriendUid (\"notify_yesno\", OnRemoveFriendDlgResult)");
		goto pg_boot_todo_default_1;
	case 0x60:
		PG_BOOT_TODO("msg 0x60: lobbymain.cpp:3104");
		goto pg_boot_todo_default_1;
	case 0xF:
		PG_BOOT_TODO("msg 0x0F: lobbymain.cpp:3554, find the room master (identity bit 2, else the 0x18 attribute in CSharedDoc::m_roomPlayerList2 (protected) then m_roomPlayerList) into m_roomMasterUid; master: m_blinkTipArea = m_startTipArea, clear m_roomOptionA/B; m_mapBtn->Enable(roomMode != 14 && master), SetStartBtn, SetRoomControls, m_roomControlWnd->Enable(master && gameType != 6)");
		goto pg_boot_todo_default_1;
	case 0x71:
		PG_BOOT_TODO("msg 0x71: lobbymain.cpp:3628");
		goto pg_boot_todo_default_1;
	case 0x10:
		PG_BOOT_TODO("msg 0x10: lobbymain.cpp:3635");
		goto pg_boot_todo_default_1;
	case 0x11:
		PG_BOOT_TODO("msg 0x11: lobbymain.cpp:3711");
		goto pg_boot_todo_default_1;
	case 0x42:
		PG_BOOT_TODO("msg 0x42: lobbymain.cpp:3815, skinned theme (GetCurrentSkinnedTheme, FrDesktop::SetWallPaper, top page button images)");
		goto pg_boot_todo_default_1;
	case 0x30:
		PG_BOOT_TODO("msg 0x30: lobbymain.cpp:3850, invite dialog (IsLocalContent 0x54, \"invite\" form, SetInvaiteInfo, OnInviteDlgResult)");
		goto pg_boot_todo_default_1;
	case 0x29:
		PG_BOOT_TODO("msg 0x29: lobbymain.cpp:3926");
		goto pg_boot_todo_default_1;
	case 0x46:
		PG_BOOT_TODO("msg 0x46: lobbymain.cpp:4017, team change (btn_teamchange_*, RemakeTeam)");
		goto pg_boot_todo_default_1;
	case 0x49:
		PG_BOOT_TODO("msg 0x49: lobbymain.cpp:4066, character parts (BuildMyPartTidList, SetLevelBar)");
		goto pg_boot_todo_default_1;
	case 0x4A:
	case 0x4B:
	case 0x4C:
	case 0x4E:
		PG_BOOT_TODO("msg 0x4A, 0x4B, 0x4C, 0x4E: lobbymain.cpp:4115-4157, copy the local player records into CSharedDoc (0x1DF8, 0x1E11, 0x634/0x1524, 0x1E23) when param1 is the local id, then SetLevelBar");
		goto pg_boot_todo_default_1;
	case 0x2A:
		PG_BOOT_TODO("msg 0x2A: lobbymain.cpp:4187, SortRank() (CLobbyMain::SortRank 0x7e7cc0 not declared yet)");
		goto pg_boot_todo_default_1;
	case 0x2D:
		PG_BOOT_TODO("msg 0x2D: lobbymain.cpp:4192, game results (CSharedDoc 0x455C/0x49E0, SortRank, \"uniteresult\" FrUniteResultDlg into m_resultReportDlg, OnResultDlgResult)");
		goto pg_boot_todo_default_1;
	case 0x3A:
		PG_BOOT_TODO("msg 0x3A: lobbymain.cpp:4387, walks CSharedDoc::m_channelList when the byte at CSharedDoc 0x4AFD has bit 1 (the loop does nothing)");
		goto pg_boot_todo_default_1;
	case 0x2F:
		PG_BOOT_TODO("msg 0x2F: lobbymain.cpp:4404");
		goto pg_boot_todo_default_1;
	case 0x3B:
		PG_BOOT_TODO("msg 0x3B: lobbymain.cpp:4454");
		goto pg_boot_todo_default_1;
	case 0x3D:
		PG_BOOT_TODO("msg 0x3D: lobbymain.cpp:4599, set or clear bit 2 of the attribute of the room player found by param1 (param2 == 1)");
		goto pg_boot_todo_default_1;
	pg_boot_todo_default_1:
#endif
	default:
		// lobbymain.cpp:5265
		HandleMsgCommon(msg);
		break;
	}
}

// lobbymain.cpp:5360
void CLobbyMain::OpenPCBangNoticeForm(void) {
	if ((m_pcbangNoticeDlg == 0 && CLoginInfo::Instance()->IsPcBang())
		|| CSharedDoc::Instance()->m_noticeShown) {
		OnUnderBar_NoticeUp();
		if (m_toppageNoticeBtn && m_noticeDlg) {
			m_noticeDlg->SetIconRect(m_toppageNoticeBtn->GetRect());
		}
	}
}

// lobbymain.cpp:5382
bool CLobbyMain::NeedsChangeNick(void) {
	if (CLoginInfo::Instance()->IsWebLogin()
		&& CSharedDoc::Instance()->GetPangyaUserInfo().nameByEdit == 1
		&& strcmp(CSharedDoc::Instance()->GetPangyaUserInfo().nickname, CSharedDoc::Instance()->m_loginId) == 0) {
		return true;
	}

	return false;
}

// lobbymain.cpp:5394
void CLobbyMain::OpenParanNickChange(void) {
	if (NeedsChangeNick() == true) {
		m_paranNickChangeDlg = CreateForm<FrParanNickDlg>(g_pFresh->GetManager(), this, "changenick_paran", NULL);
		m_paranNickChangeDlg->SetMessage("\xc6\xce\xbe\xdf\xbf\xa1\xbc\xad \xbb\xe7\xbf\xeb\xc7\xcf\xbd\xc7 \xb4\xeb\xc8\xad\xb8\xed\xc0\xbb \xc0\xd4\xb7\xc2\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", false);
		m_paranNickChangeDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnParanNickChangeResult, 0);
	} else {
		OpenCaddieWarningForm();
	}
}

// lobbymain.cpp:5409
void CLobbyMain::OpenCaddieWarningForm(void) {
	if (m_caddieWarningPending && CSharedDoc::Instance()->m_loginDlgCreated) {
		m_caddieWarningPending = false;

		for (int i = 0; i < 12; i++) {
			if (m_warningCaddies[i].id != 0) {
				sCaddieInfo info = m_warningCaddies[i];
				memset(&m_warningCaddies[i], 0, sizeof(sCaddieInfo));

				if (info.id != 0) {
					m_caddieWarningDlg = CreateForm<FrCaddieWarningDlg>(g_pFresh->GetManager(), this, "caddie_warning", NULL);
					static_cast<FrCaddieWarningDlg*>(m_caddieWarningDlg)->SetCaddieInfo(info);
					m_caddieWarningDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnCaddieWarningDlgResult, 0x40);
				}
				break;
			}
		}
	} else {
		OpenNoteForm();
	}
}

// lobbymain.cpp:5442
void CLobbyMain::OpenNoteForm(void) {
	if (CSharedDoc::Instance()->m_loginDlgCreated && CSharedDoc::Instance()->m_noteList.size() > 0) {
		if (m_mailBoxDlg == 0) {
			m_mailBoxDlg = CreateForm<FrMailBoxDlg>(g_pFresh->GetManager(), this, "mailbox", NULL);
			m_mailBoxDlg->EnableDrag(false);
			m_mailBoxDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnMailBoxDlgResult, 0);
		}
	}
}

// lobbymain.cpp:5461
void CLobbyMain::OpenEventPrizeForm(unsigned char event) {
	if (m_eventDlg[event] == 0) {
		m_eventDlg[event] = CreateForm<FrEventPrizeDlg>(g_pFresh->GetManager(), this, "notice_popup", NULL);
		static_cast<FrEventPrizeDlg*>(m_eventDlg[event])->SetEvent(event);
		m_eventDlg[event]->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnEventDlgResult, 3);
	}
}

// lobbymain.cpp:5476
void CLobbyMain::OnToppage_Init(int) {
	CTaskMain::OnInit();

	CSharedDoc::Instance()->ClearUserInfoTimeMap((unsigned long)-1);
	memset(CSharedDoc::Instance()->m_roomPlayers, 0, sizeof(CSharedDoc::Instance()->m_roomPlayers));

	SetRandomBGM();
	TurnOn3DBackground();

	if (IsLocalContent((localContentType_t)0x6A)) {
		m_topPageBtnIndex = CLoginInfo::Instance()->IsPcBang() ? 2 : 1;
	}
}

// lobbymain.cpp:5503
void CLobbyMain::OnToppage_Finish(void) {
	if (CSharedDoc::Instance()->m_paranNickChanged) {
		if (IsLocalContent((localContentType_t)0x10)) {
			CSharedDoc::Instance()->m_paranNickChanged = false;
			OpenEventPrizeForm(5);

			if (CSharedDoc::Instance()->m_flag50F6 == true) {
				this << MsgObject(0, 0x3E, 0x1A000006, (intptr_t)
			"[\xC7\xC7\xC7\xC9 \xBC\xB1\xB9\xB0 \xB8\xDE\xC0\xCF]\n"
			"\n"
			"\xBE\xC8\xB3\xE7~ \xC0\xFC \xB4\xE7\xBD\xC5\xC0\xBB \xBF\xA9\xB1\xE2 \xB5\xA5\xB8\xAE\xB0\xED \xBF\xC2 \xBD\xC3\xB0\xA3\xC0\xC7 \xBF\xE4\xC1\xA4 \xC7\xC7\xC7\xC9\xC0\xCC\xB6\xF3\xB0\xED \xC7\xD5\xB4\xCF\xB4\xD9. \xB9\xE6\xB1\xDD \xBF\xB9\xBB\xDB \xC4\xB3\xB8\xAF\xC5\xCD\xB8\xA6 \xBC\xB1\xC5\xC3\xC7\xDF\xB3\xD7\xBF\xE4. \xB0\xE8\xBC\xD3 \xC0\xDF \xBA\xB8\xBB\xEC\xC6\xEC\xC1\xD6\xBC\xBC\xBF\xE4.\n"
			"\n"
			"\xBE\xC6~ \xB1\xD7\xB8\xAE\xB0\xED \xC0\xCC\xB0\xC5 \xB9\xDE\xC0\xB8\xBC\xBC\xBF\xE4. \xB4\xE7\xBD\xC5\xC0\xCC \xC6\xCE\xBE\xDF\xBF\xA1 \xBA\xB8\xB4\xD9 \xBD\xB1\xB0\xD4 \xC0\xFB\xC0\xC0\xC7\xCF\xB1\xE2 \xC0\xA7\xC7\xD8\xBC\xAD \xB5\xE5\xB8\xAE\xB4\xC2 \xC7\xE0\xBF\xEE\xC0\xC7 \xB8\xF1\xB0\xC9\xC0\xCC\xC0\xD4\xB4\xCF\xB4\xD9.\n"
			"\xC0\xCC \xC7\xE0\xBF\xEE\xC0\xC7 \xB8\xF1\xB0\xC9\xC0\xCC\xB4\xC2 \xB4\xE7\xBD\xC5\xC0\xCC \xB8\xB8\xB5\xE7 \xC4\xB3\xB8\xAF\xC5\xCD\xC0\xC7 \\c0xffff0000\\c\xC1\xA4\xC8\xAE\xB5\xB5\\c0xff000000\\c\xB8\xA6 \xB3\xF4\xC7\xF4\xC1\xD6\xB0\xED \\c0xffff0000\\c\xC6\xCE\xBE\xDF\xC0\xD3\xC6\xD1\xC6\xAE\xC1\xB8\\c0xff000000\\c\xC0\xBB \xB3\xD0\xC7\xF4\xC1\xD6\xB4\xC2 \xBF\xAA\xC7\xD2\xC0\xBB \xC7\xD1\xB4\xE4\xB4\xCF\xB4\xD9.\n"
			"\xC7\xCF\xB3\xAA \xC1\xD6\xC0\xC7\xC7\xD2 \xB0\xCD\xC0\xBA\xBF\xE4~ \xB4\xE7\xBD\xC5\xC0\xC7 \xBD\xC7\xB7\xC2\xC0\xCC \xBE\xEE\xB4\xC0 \xC1\xA4\xB5\xB5 \xBE\xC8\xC1\xA4\xB5\xC7\xB4\xC2 \xB7\xB9\xBA\xA7, \xC1\xEF \\c0xffff0000\\c\xC1\xD6\xB4\xCF\xBE\xEE" "E\\c0xff000000\\c\xB0\xA1 \xB5\xC7\xB8\xE9 \xC8\xBF\xB0\xFA\xB0\xA1 \xBE\xF8\xBE\xEE\xC1\xF6\xB4\xCF\xB1\xEE\xBF\xE4~ \xBE\xCB\xBE\xC6\xB5\xCE\xBD\xC3\xB0\xED\xBF\xE4.\n"
			"\n"
			"\xB1\xD7\xB8\xAE\xB0\xED \xB4\xEB\xC0\xFC\xB8\xF0\xB5\xE5\xBF\xA1 \xB0\xA1\xB1\xE2 \xC0\xFC\xBF\xA1 \xC7\xD1\xB9\xF8 \xC8\xA5\xC0\xDA \xBF\xAC\xBD\xC0\xC7\xCF\xB0\xED \xBD\xCD\xB4\xD9\xB8\xE9 \xC6\xA9\xC5\xE4\xB8\xAE\xBE\xF3 \xB8\xF0\xB5\xE5\xBF\xA1 \xB5\xE9\xBE\xEE\xB0\xA1\xBA\xB8\xBC\xBC\xBF\xE4.\n"
			"\xC2\xF7\xB1\xD9\xC2\xF7\xB1\xD9 \xB8\xF0\xB5\xCE \xB9\xE8\xBF\xEC\xBD\xC3\xB8\xE9 \xBC\xB1\xB9\xB0\xB5\xB5 \xC1\xD8\xB4\xD9\xB0\xED \xC7\xCF\xB3\xD7\xBF\xE4. ^^\n"
			"\n"
			"\xB1\xD7\xB7\xB3 \xB4\xE3\xBF\xA1 \xB6\xC7 \xBA\xC1\xBF\xE4. \xBE\xC8\xB3\xE7", 0, 0, 0);
				CSharedDoc::Instance()->m_flag50F6 = false;
			}
		} else {
			CSharedDoc::Instance()->m_paranNickChanged = false;
			OpenEventPrizeForm(2);
		}
	}

	if (CSharedDoc::Instance()->m_tutorialFinish) {
		this << MsgObject(0, 0xAA, 1, 0, 0, 0, 0);
	}
}

// lobbymain.cpp:5544
void CLobbyMain::OnToppage_Destroy(void) {
}

// lobbymain.cpp:5548
void CLobbyMain::OnToppage_GameInit(intptr_t param) {
	m_toppageGameBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageGameBtn) {
		m_toppageGameBtn->SetPushSound("ui_desktop_icon_click");
		m_toppageGameBtn->EnableHover(true, 0.5f);
	}
}

// lobbymain.cpp:5558
void CLobbyMain::OnToppage_GameUp(void) {
	OnUnderBar_GameUp();
}

// lobbymain.cpp:5563
void CLobbyMain::OnToppage_FamilyInit(intptr_t param) {
	m_toppageFamilyBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageFamilyBtn) {
		m_toppageFamilyBtn->SetPushSound("ui_desktop_icon_click");
		m_toppageFamilyBtn->EnableHover(true, 0.5f);
	}
}

// lobbymain.cpp:5606
void CLobbyMain::OnToppage_PangysStudyInit(intptr_t param) {
	m_toppagePangysStudyBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppagePangysStudyBtn) {
		m_toppagePangysStudyBtn->SetPushSound("ui_desktop_icon_click");
		m_toppagePangysStudyBtn->EnableHover(true, 0.5f);
	}
}

// lobbymain.cpp:5615
void CLobbyMain::OnToppage_PangysStudyUp(void) {
	CloseAllDialogs();

	CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
	CTaskManager::Instance()->PostMsg(0, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
}

// lobbymain.cpp:5623
void CLobbyMain::OnToppage_NoticeInit(intptr_t param) {
	m_toppageNoticeBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageNoticeBtn) {
		m_toppageNoticeBtn->SetPushSound("ui_desktop_icon_click");
		m_toppageNoticeBtn->EnableHover(true, 0.5f);
	}
}

// lobbymain.cpp:5633
void CLobbyMain::OnToppage_NoticeUp(void) {
	OnUnderBar_NoticeUp();

	if (m_toppageNoticeBtn && m_noticeDlg) {
		m_noticeDlg->SetIconRect(m_toppageNoticeBtn->GetRect());
	}
}

// lobbymain.cpp:5641
void CLobbyMain::OnToppage_PcbangInit(intptr_t param) {
	m_toppagePcbangBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppagePcbangBtn) {
		m_toppagePcbangBtn->SetVisible(CLoginInfo::Instance()->IsPcBang() ? true : false);
		m_toppagePcbangBtn->Enable(CLoginInfo::Instance()->IsPcBang() ? true : false);

		if (CLoginInfo::Instance()->IsPcBang()) {
			CIconManager::Instance()->AddTopIconFromOutSide();
		}

		cTopEventIcon((eTopIconID)0).Prepare(m_toppagePcbangBtn);
	}
}

// lobbymain.cpp:5662
void CLobbyMain::OnToppage_PcbangUp(void) {
	CloseAllDialogs();
	m_pcbangNoticeDlg = CreateForm<FrNoticeDlg>(g_pFresh->GetManager(), this, "notice_popup", NULL);
	m_pcbangNoticeDlg->LoadInitFile("pcbang.ini");
	m_pcbangNoticeDlg->SetIconRect(m_toppagePcbangBtn->GetRect());
	m_pcbangNoticeDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnPCBangDlgResult, 0x11);
}

// lobbymain.cpp:5681
void CLobbyMain::OnToppage_RentalExpiredAlramInit(intptr_t param) {
	FrButton* btn = DYNAMIC_CAST(FrButton, param);
	if (btn) {
		if (!S5::RENTALPARTS::IsExistExpiredRentalItem(CSharedDoc::Instance()->m_myPartsList)) {
			btn->SetVisible(false);
			btn->Enable(false);
		}
	}
}

// lobbymain.cpp:5727
void CLobbyMain::OnToppage_Event1Init(intptr_t param) {
	m_toppageEvent1Btn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageEvent1Btn) {
		m_toppageEvent1Btn->SetPushSound("ui_desktop_icon_click");
		m_toppageEvent1Btn->EnableHover(true, 0.5f);
		m_toppageEvent1Btn->SetVisible(false);

		if (IsLocalContent((localContentType_t)0x18)) {
			m_toppageEvent1Btn->SetVisible(true);
		}

		m_event1IconRect = m_toppageEvent1Btn->GetRect();
	}
}

// lobbymain.cpp:5745
void CLobbyMain::OnToppage_Event1Up(void) {
	if (IsLocalContent((localContentType_t)0x18)) {
		CloseAllDialogs();

		if (IsLocalContent((localContentType_t)0x48)) {
			m_holesEventDlg = CreateForm<ntHolesEventDlg>(g_pFresh->GetManager(), this, "holes_EntranceExam", NULL);
		} else {
			m_holesEventDlg = CreateForm<ntHolesEventDlg>(g_pFresh->GetManager(), this, "holes_event", NULL);
		}

		m_holesEventDlg = CreateForm<ntHolesEventDlg>(g_pFresh->GetManager(), this, "holes_event", NULL);
		m_holesEventDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnHolesEventDlgResult, 0x11);
		m_holesEventDlg->SetIconRect(m_toppageEvent1Btn->GetRect());
	}
}

// lobbymain.cpp:5762
void CLobbyMain::OnToppage_NewInit(intptr_t param) {
	m_newArea = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:5767
// Draws the "new" tab image next to the Pangya's study button.
void CLobbyMain::OnToppage_NewOwnerDraw(int) {
	if (m_toppagePangysStudyBtn) {
		if (m_newTabBitmap) {
			WRect rect = m_toppagePangysStudyBtn->GetRect();
			g_pFresh->GetManager()->GetGDI()->DrawTexture(m_newTabBitmap, WRect(rect.x + 58.0f, rect.y + 12.0f, m_newTabBitmap->Width(), m_newTabBitmap->Height()), 0xFFFFFFFF, 0);
		}
	}
}

// lobbymain.cpp:5849
bool CLobbyMain::OnValEventDlgResult(int, FrForm*) {
	m_valEventDlg = 0;
	return true;
}

// lobbymain.cpp:5861
// Shows the point event button where the content allows it: in the next top
// page slot, next to the PC bang button, or at x 160 in a PC bang.
void CLobbyMain::OnToppage_PointEventInit(intptr_t param) {
	m_toppagePointEventBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppagePointEventBtn) {
		if (IsLocalContent((localContentType_t)0x15)) {
			m_toppagePointEventBtn->SetPushSound("ui_desktop_icon_click");
			m_toppagePointEventBtn->EnableHover(true, 0.5f);
			m_toppagePointEventBtn->SetVisible(true);
			m_toppagePointEventBtn->SetPushDelay(0.0f);

			if (IsLocalContent((localContentType_t)0x6A)) {
				WRect rect = m_toppagePointEventBtn->GetRect();
				rect.x = GetTopPageBtnRect();
				m_toppagePointEventBtn->SetRect(rect);
			} else if (!CLoginInfo::Instance()->IsPcBang()) {
				WRect rect = m_toppagePointEventBtn->GetRect();
				rect.x = m_toppagePcbangBtn->GetRect().x;
				rect.y = m_toppagePcbangBtn->GetRect().y - 2.0f;
				m_toppagePointEventBtn->SetRect(rect);
			} else {
				WRect rect = m_toppagePointEventBtn->GetRect();
				rect.x = 160.0f;
				rect.y -= 2.0f;
				m_toppagePointEventBtn->SetRect(rect);
			}
		} else {
			m_toppagePointEventBtn->SetVisible(false);
		}
	}
}

// lobbymain.cpp:5929
void CLobbyMain::OnToppage_PointEventDown(void) {
	if (IsLocalContent((localContentType_t)0x15)) {
		CloseAllDialogs();
		WSendPacket packet((enumClientPacket)0x95);
		packet.Send(SEND_TO_DEFAULT);

		CSharedDoc::Instance()->m_pointEventState = 2;
		CSharedDoc::Instance()->m_pointEventFlag = false;
	}
}

// lobbymain.cpp:5956
void CLobbyMain::OnToppage_ValEventInit(intptr_t param) {
	m_toppageValEventBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageValEventBtn) {
		m_toppageValEventBtn->SetVisible(IsLocalContent((localContentType_t)0x78));
		if (m_toppageValEventBtn->IsVisible()) {
			m_toppageValEventBtn->SetPushDelay(0.1f);
			cTopEventIcon((eTopIconID)8).Prepare(m_toppageValEventBtn);
		}
	}
}

// lobbymain.cpp:5985
// Shows the world tour event button where the content allows it, after the
// point event or PC bang button.
void CLobbyMain::OnToppage_WorldTourEventInit(intptr_t param) {
	m_toppageWorldTourBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageWorldTourBtn) {
		if (IsLocalContent((localContentType_t)0x1E)) {
			m_toppageWorldTourBtn->SetPushSound("ui_desktop_icon_click");
			m_toppageWorldTourBtn->EnableHover(true, 0.5f);
			m_toppageWorldTourBtn->SetVisible(true);
			m_toppageWorldTourBtn->SetPushDelay(0.0f);

			bool isPcBang = CLoginInfo::Instance()->IsPcBang() != 0;
			bool hasPointEvent = IsLocalContent((localContentType_t)0x15);

			WRect pcbangRect = m_toppagePcbangBtn->GetRect();
			WRect pointRect = m_toppagePointEventBtn->GetRect();
			WRect rect = m_toppageWorldTourBtn->GetRect();

			rect.y += 2.0f;

			if (IsLocalContent((localContentType_t)0x6A)) {
				WRect slotRect = m_toppageWorldTourBtn->GetRect();
				slotRect.x = GetTopPageBtnRect();
				m_toppageWorldTourBtn->SetRect(slotRect);
			} else if (isPcBang) {
				if (hasPointEvent) {
					rect.x = pointRect.x + pointRect.w;
					m_toppageWorldTourBtn->SetRect(rect);
				} else {
					rect.x = pcbangRect.x + pcbangRect.w;
					m_toppageWorldTourBtn->SetRect(rect);
				}
			} else if (hasPointEvent) {
				rect.x = pointRect.x + pointRect.w;
				m_toppageWorldTourBtn->SetRect(rect);
			}
		} else {
			m_toppageWorldTourBtn->SetVisible(false);
			m_toppageWorldTourBtn->Enable(false);
		}
	}
}

// lobbymain.cpp:6046
// Opens the world tour event dialog on its map flags (or refreshes them).
void CLobbyMain::OnToppage_WorldTourEventDown(void) {
	if (IsLocalContent((localContentType_t)0x1E)) {
		if (!m_toppageWorldTourBtn)
			return;

		CloseAllDialogs();

		CWorldTourEvent* pEvent = (CWorldTourEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x1E);
		if (!pEvent)
			return;

		if (m_worldTourEventDlg) {
			m_worldTourEventDlg->SetMapFlag(pEvent->GetMapFlag());
		} else {
			m_worldTourEventDlg = CreateForm<FrWorldTourEventDlg>(g_pFresh->GetManager(), this, "world_tour_event_dlg", NULL);
			if (m_worldTourEventDlg) {
				m_worldTourEventDlg->SetMapFlag(pEvent->GetMapFlag());
				m_worldTourEventDlg->SetIconRect(m_toppageWorldTourBtn->GetRect());
				m_worldTourEventDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnWorldTourEventDlgResult, 0x11);
			}
		}
	}
}

// lobbymain.cpp:6081
// Shows the find-Ebort event button after the PC bang, point event and world
// tour buttons that are shown.
void CLobbyMain::OnToppage_FindEbortEventInit(intptr_t param) {
	m_toppageFindEbortBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageFindEbortBtn) {
		if (IsLocalContent((localContentType_t)0x20)) {
			m_toppageFindEbortBtn->SetVisible(true);
			m_toppageFindEbortBtn->Enable(true);
			m_toppageFindEbortBtn->SetPushDelay(0.0f);

			bool isPcBang = CLoginInfo::Instance()->IsPcBang() != 0;
			bool hasPointEvent = IsLocalContent((localContentType_t)0x15);
			bool hasWorldTour = IsLocalContent((localContentType_t)0x1E);

			WRect rect = m_toppageFindEbortBtn->GetRect();

			if (isPcBang)
				rect.x += m_toppagePcbangBtn->GetRect().w + m_toppagePcbangBtn->GetRect().x;

			if (hasPointEvent)
				rect.x += m_toppagePointEventBtn->GetRect().w + m_toppagePointEventBtn->GetRect().x;

			if (hasWorldTour)
				rect.x += m_toppageWorldTourBtn->GetRect().w + m_toppageWorldTourBtn->GetRect().x;

			rect.y -= 5.0f;
			m_toppageFindEbortBtn->SetRect(rect);
		} else {
			m_toppageFindEbortBtn->SetVisible(false);
			m_toppageFindEbortBtn->Enable(false);
		}
	}
}

// lobbymain.cpp:6121
void CLobbyMain::OnToppage_FindEbortEventDown(void) {
	if (IsLocalContent((localContentType_t)0x20)) {
		if (m_toppageFindEbortBtn) {
			CloseAllDialogs();

			if (!m_findEbortEventDlg) {
				m_findEbortEventDlg = CreateForm<FrFindEbortEventDlg>(g_pFresh->GetManager(), this, "find_ebort_event_dlg", NULL);
				if (m_findEbortEventDlg) {
					m_findEbortEventDlg->SetIconRect(m_toppageFindEbortBtn->GetRect());
					m_findEbortEventDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnFindEbortEventDlgResult, 0x11);
				}
			}
		}
	}
}

// lobbymain.cpp:6144
void CLobbyMain::OnToppage_Halloween2007Init(intptr_t param) {
	m_toppageHalloweenBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageHalloweenBtn) {
		if (IsLocalContent((localContentType_t)0x21)) {
			CHalloweenEvent* pEvent;
			if (CContentsDoc::Instance()->GetContainer((localContentType_t)0x21, pEvent)) {
				m_toppageHalloweenBtn->SetVisible(true);
				m_toppageHalloweenBtn->Enable(true);
			} else {
				m_toppageHalloweenBtn->SetVisible(false);
				m_toppageHalloweenBtn->Enable(false);
			}

			cTopEventIcon((eTopIconID)9).Prepare(m_toppageHalloweenBtn);
		} else {
			m_toppageHalloweenBtn->SetVisible(false);
			m_toppageHalloweenBtn->Enable(false);
		}
	}
}

// lobbymain.cpp:6178
void CLobbyMain::OnToppage_Halloween2007EvenDown(void) {
	if (IsLocalContent((localContentType_t)0x21)) {
		if (m_toppageHalloweenBtn) {
			CloseAllDialogs();

			if (!m_halloweenEventDlg) {
				m_halloweenEventDlg = CreateForm<FrHalloweenEventDlg>(g_pFresh->GetManager(), this, "halloween2007_event_dlg", NULL);
				if (m_halloweenEventDlg) {
					m_halloweenEventDlg->SetIconRect(m_toppageHalloweenBtn->GetRect());
					m_halloweenEventDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnHalloweenEventDlgResult, 0x11);
				}
			}
		}
	}
}

// lobbymain.cpp:6201
void CLobbyMain::OnToppage_Christmas2007EventInit(intptr_t param) {
	m_toppageChristmasBtn = DYNAMIC_CAST(FrButton, param);
	if (IsLocalContent((localContentType_t)0x4C)) {
		CChristmasEvent* pEvent;
		if (CContentsDoc::Instance()->GetContainer((localContentType_t)0x4C, pEvent) && pEvent->GetFlag()) {
			m_toppageChristmasBtn->SetVisible(true);
			m_toppageChristmasBtn->Enable(true);

			cTopEventIcon((eTopIconID)7).Prepare(m_toppageChristmasBtn);
			return;
		}
	}

	m_toppageChristmasBtn->SetVisible(false);
	m_toppageChristmasBtn->Enable(false);
}

// lobbymain.cpp:6230
void CLobbyMain::OnToppage_Christmas2007EventDown(void) {
	if (IsLocalContent((localContentType_t)0x4C)) {
		if (m_toppageChristmasBtn) {
			CloseAllDialogs();

			if (!m_christmasEventDlg) {
				m_christmasEventDlg = CreateForm<FrChristmasEventDlg>(g_pFresh->GetManager(), this, "christmas2007_event_dlg", NULL);
				if (m_christmasEventDlg) {
					m_christmasEventDlg->SetIconRect(m_toppageChristmasBtn->GetRect());
					m_christmasEventDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnChristmasEventDlgResult, 0x11);
				}
			}
		}
	}
}

// lobbymain.cpp:6306
// Shows the ticket exchange button while the ticket exchange is open.
void CLobbyMain::OnToppage_TicketExchangeInit(intptr_t param) {
	m_toppageTicketExchangeBtn = DYNAMIC_CAST(FrButton, param);
	if (IsLocalContent((localContentType_t)0x7A)) {
		CTicketExchange* pExchange = (CTicketExchange*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x7A);
		if (pExchange) {
			if (pExchange->GetFlag()) {
				m_toppageTicketExchangeBtn->SetVisible(true);
				m_toppageTicketExchangeBtn->Enable(true);

				cTopEventIcon((eTopIconID)10).Prepare(m_toppageTicketExchangeBtn);

				m_toppageTicketExchangeBtn->SetPushSound("ui_desktop_icon_click");
				m_toppageTicketExchangeBtn->EnableHover(true, 0.5f);
				m_toppageTicketExchangeBtn->SetVisible(true);
				m_toppageTicketExchangeBtn->SetPushDelay(0.0f);

				if (IsLocalContent((localContentType_t)0x6A)) {
					WRect rect = m_toppageTicketExchangeBtn->GetRect();
					rect.x = GetTopPageBtnRect();
					m_toppageTicketExchangeBtn->SetRect(rect);
				}
				return;
			}
		}
	}

	m_toppageTicketExchangeBtn->SetVisible(false);
	m_toppageTicketExchangeBtn->Enable(false);
}

// lobbymain.cpp:6349
void CLobbyMain::OnToppage_TicketExchangeDown(void) {
	if (IsLocalContent((localContentType_t)0x7A)) {
		if (m_toppageTicketExchangeBtn) {
			CloseAllDialogs();

			if (!m_ticketExchangeDlg) {
				m_ticketExchangeDlg = CreateForm<FrTicketExchangeDlg>(g_pFresh->GetManager(), this, "ticket_exchage_dlg", NULL);
				if (m_ticketExchangeDlg) {
					m_ticketExchangeDlg->SetIconRect(m_ticketExchangeDlg->GetRect());
					m_ticketExchangeDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnTicketExchangeDlgResult, 0x11);
				}
			}
		}
	}
}

// lobbymain.cpp:6374
void CLobbyMain::OnToppage_MissionEventInit(intptr_t param) {
	m_toppageMissionBtn = DYNAMIC_CAST(FrButton, param);
	cTopEventIcon((eTopIconID)13).Prepare(m_toppageMissionBtn);

	if (!IsLocalContent((localContentType_t)0x83)) {
		m_toppageMissionBtn->Enable(false);
		m_toppageMissionBtn->SetVisible(false);
	}
}

// lobbymain.cpp:6389
void CLobbyMain::OnToppage_MissionEventDown(void) {
	if (!IsLocalContent((localContentType_t)0x83)) {
		return;
	}

	WSendPacket packet((enumClientPacket)0xE3);
	packet.Send(SEND_TO_DEFAULT);

	this << MsgObject(this, 1, 0, 0, 0, 0, 0);
}

// lobbymain.cpp:6402
void CLobbyMain::OnToppage_BingoEventInit(intptr_t param) {
	m_toppageBingoBtn = DYNAMIC_CAST(FrButton, param);
	cTopEventIcon((eTopIconID)12).Prepare(m_toppageBingoBtn);

	if (!IsLocalContent((localContentType_t)0x85)) {
		m_toppageBingoBtn->Enable(false);
		m_toppageBingoBtn->SetVisible(false);
	}
}

// lobbymain.cpp:6417
void CLobbyMain::OnToppage_BingoEventDown(void) {
	if (!IsLocalContent((localContentType_t)0x85)) {
		return;
	}

	WSendPacket packet((enumClientPacket)0xE5);
	packet.Encode4(0);
	packet.Send(SEND_TO_DEFAULT);

	this << MsgObject(this, 1, 0, 0, 0, 0, 0);
}

// lobbymain.cpp:5973
void CLobbyMain::OnToppage_ValEventDown(void) {
	if (IsLocalContent((localContentType_t)0x78) == true) {
		WSendPacket packet((enumClientPacket)0xDC);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// lobbymain.cpp:6431
// Skins the birthday event button (Kooh's, Hana's or Arin's) from its bitmap
// box, or hides it when no birthday event runs.
void CLobbyMain::OnToppage_KoohBirthdayEventInit(intptr_t param) {
	m_toppageKoohBirthdayBtn = DYNAMIC_CAST(FrButton, param);

	std::string boxName("");
	if (IsLocalContent((localContentType_t)0x8E)) {
		boxName = "KoohBirthdayEvent";
	}

	if (IsLocalContent((localContentType_t)0x94)) {
		boxName = "HanaBirthdayEvent";
	} else if (IsLocalContent((localContentType_t)0x9B)) {
		boxName = "ArinBirthdayEvent";
	}

	if (boxName != "") {
		CGDBitmapBox* pBox = (CGDBitmapBox*)WSingleton<CGameDataDB>::Instance()->GetGameData(eGAMEDATADB_BITMAPBOX, boxName.c_str());
		m_toppageKoohBirthdayBtn->SetButtonImg(pBox->GetFileName("LobbyIconNormal"), FrButton::NORMAL);
		m_toppageKoohBirthdayBtn->SetButtonImg(pBox->GetFileName("LobbyIconOver"), FrButton::OVER);
		m_toppageKoohBirthdayBtn->SetButtonImg(pBox->GetFileName("LobbyIconBlink"), FrButton::BLINK);

		cTopEventIcon((eTopIconID)6).Prepare(m_toppageKoohBirthdayBtn);
	} else {
		m_toppageKoohBirthdayBtn->Enable(false);
		m_toppageKoohBirthdayBtn->SetVisible(false);
	}
}

// lobbymain.cpp:6469
void CLobbyMain::OnToppage_KoohBirthdayEventDown(void) {
	WSendPacket packet(0xEF);
	packet.Send(SEND_TO_DEFAULT);
	this << MsgObject(this, 1, 0, 0, 0, 0, 0);
}

// lobbymain.cpp:6477
void CLobbyMain::OnToppage_TreasureIslandInit(intptr_t param) {
	m_toppageTreasureBtn = DYNAMIC_CAST(FrButton, param);
	cTopEventIcon((eTopIconID)11).Prepare(m_toppageTreasureBtn);

	if (!IsLocalContent((localContentType_t)0x8C)) {
		m_toppageTreasureBtn->Enable(false);
		m_toppageTreasureBtn->SetVisible(false);
	}
}

// lobbymain.cpp:6492
void CLobbyMain::OnToppage_TreasureIslandLBDown(void) {
	if (IsLocalContent((localContentType_t)0x8C)) {
		ShellExecuteA(NULL, NULL, "http://qa.pangya.gametree.co.kr/events/bomul/PangyaBomul.aspx", NULL, NULL, SW_SHOWMAXIMIZED);
	}
}

// lobbymain.cpp:6647
void CLobbyMain::OnToppage_ServerInit(intptr_t param) {
	m_toppageServerBtn = DYNAMIC_CAST(FrButton, param);
	if (m_toppageServerBtn) {
		m_toppageServerBtn->SetPushSound("ui_desktop_icon_click");
		m_toppageServerBtn->EnableHover(true, 0.5f);
	}
}

// lobbymain.cpp:6657
void CLobbyMain::OnToppage_ServerUp(void) {
	CloseAllDialogs();
	OnUnderBar_ServerUp();
}

// lobbymain.cpp:6664
void CLobbyMain::CloseAllDialogs(void) {
	if (m_optionDlg)
		m_optionDlg->Close(true);

	if (m_pcbangNoticeDlg)
		m_pcbangNoticeDlg->Close(true);

	if (CMessengerInfo::Instance()->IsVisible())
		CMessengerInfo::Instance()->Toggle();

	if (CUserInfo::Instance())
		CUserInfo::Instance()->Close();

	if (m_serverDlg)
		m_serverDlg->Close(false);

	if (m_pointEventDlg)
		m_pointEventDlg->Close(true);

	if (m_createNickDlg)
		m_createNickDlg->CloseDlg(0);

	CIconToolTip::GetInstance()->ClearToolTip();

	CTaskMain::CloseAllDialogs();
}

// lobbymain.cpp:6705
void CLobbyMain::OnServer_Init(int) {
	CTaskMain::OnInit();
	SetRandomBGM();
	TurnOn3DBackground();
}

// lobbymain.cpp:6713
void CLobbyMain::OnServer_Finish(void) {
	if (m_serverDlg == 0) {
		m_serverDlg = CreateForm<FrServerDlg>(g_pFresh->GetManager(), this, "server", NULL);

		m_serverDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnServerDlgResult, 0x10);
	}
}

// lobbymain.cpp:6763
bool CLobbyMain::OnNewRecordDlgResult(int, FrForm*) {
	m_newRecordDlg = 0;
	return true;
}

// lobbymain.cpp:6775
void CLobbyMain::OnLogin_Init(int) {
	CTaskMain::OnInit();

	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)) {
		CMessengerInfo::Instance()->Close(true);
		WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
	}

	CSharedDoc::Instance()->ClearAll();
	BuddyMGR->ClearBuddy();

	SetRandomBGM();
	TurnOn3DBackground();
}

// lobbymain.cpp:6792
void CLobbyMain::OnLogin_Finish(void) {
	g_mouse->ResetInputTime();
	g_ime->ResetInputTime();
	WSingleton<CMouseCursor>::Instance()->SetActive(true);

	if (!WSingleton<CProjectG>::Instance()->CheckUIHack()) {
		m_scrollActive = true;

		FrForm* form = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		// "A modified file was found. Remove the file or reinstall PangYa, then run it again."
		form->SetMessage("\xBA\xAF\xC1\xB6\xB5\xC8 \xC6\xC4\xC0\xCF\xC0\xCC \xB9\xDF\xB0\xDF\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9. \xC7\xD8\xB4\xE7 \xC6\xC4\xC0\xCF\xC0\xBB \xC1\xA6\xB0\xC5 \xC7\xCF\xB0\xC5\xB3\xAA \xC6\xCE\xBE\xDF\xB8\xA6 \xC0\xE7\xBC\xB3\xC4\xA1 \xC8\xC4 \xB4\xD9\xBD\xC3 \xBD\xC7\xC7\xE0\xC7\xD8\xC1\xD6\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", false);
		form->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnUIHackDlgResult, 0);
		return;
	}

	m_loginDlg = CreateForm<FrLoginDlg>(g_pFresh->GetManager(), this, "login", NULL);
	m_loginDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnLoginDlgResult, 10);

	WSingleton<CCardManager>::Instance()->InitCardBuffDlgPosition();
	CItemBuff::ResetBuffList();
}

// lobbymain.cpp:6815
void CLobbyMain::OnBlank_Init(int) {
	CTaskMain::OnInit();
}

// lobbymain.cpp:6820
void CLobbyMain::OnBlank_Finish(void) {
	unsigned char forceNickname = CSharedDoc::Instance()->m_forceNicknameChange;
	if (forceNickname == 1 || forceNickname == 2) {
		GetActor("Lobby") << MsgObject(0, 0x249, 1, 0, 0, 0, 0);
	}
}

// lobbymain.cpp:6833
void CLobbyMain::OnSecond_Pwd_Init(int) {
	CTaskMain::OnInit();
}

// lobbymain.cpp:6838
void CLobbyMain::OnSecond_Pwd_Finish(void) {
}

// lobbymain.cpp:6852
bool CLobbyMain::OnMakeRoomDlgResult(int ret, FrForm*) {
	if (ret == FrOK) {
		if (m_makeRoomDlg) {
			if (LatestVersion() && !m_makeRoomDlg->MakeRoom(false)) {
				return false;
			}
		}
	} else {
		EnableRoomEnterControls(true);
	}

	m_makeRoomDlg = 0;
	return true;
}

// lobbymain.cpp:6873
bool CLobbyMain::OnRemoveFriendDlgResult(int ret, FrForm*) {
	if (ret == 1) {
		BuddyMGR->GetBuddy(m_removeFriendUid);
		m_removeFriendUid = (unsigned long)-1;
	}
	return true;
}

// lobbymain.cpp:6888
bool CLobbyMain::OnEventDlgResult(int, FrForm* form) {
	m_eventDlg[static_cast<FrEventPrizeDlg*>(form)->GetEvent()] = 0;
	return true;
}

// lobbymain.cpp:6899
bool CLobbyMain::OnMailBoxDlgResult(int, FrForm*) {
	m_mailBoxDlg = 0;
	return true;
}

// lobbymain.cpp:6911
bool CLobbyMain::OnScoreDlgResult(int, FrForm*) {
	m_scoreDlg = 0;
	return true;
}

// lobbymain.cpp:6918
bool CLobbyMain::OnResultDlg_Report(int, FrForm*) {
	if (!IsLocalContent((localContentType_t)0x14)) {
		return false;
	}
	m_resultReportDlg = 0;
	return true;
}

// lobbymain.cpp:6927
bool CLobbyMain::OnApproachGiftDlgResult(int, FrForm*) {
	m_approachGiftDlg = 0;

	WSendPacket packet((enumClientPacket)0xC3);
	packet.Encode4(CSharedDoc::Instance()->GetPangyaUserInfo().guid);
	packet.Send(SEND_TO_DEFAULT);

	return true;
}

// lobbymain.cpp:6962
bool CLobbyMain::OnGuildNoticeDlgResult(int, FrForm*) {
	m_guildNoticeDlg = 0; if (m_guildNewsReader) { delete m_guildNewsReader; m_guildNewsReader = 0; }

	OpenPCBangNoticeForm();
	CSharedDoc::Instance()->m_loginDlgCreated = false;
	return true;
}

// lobbymain.cpp:7124
bool CLobbyMain::OnUserListDlgResult(int, FrForm*) {
	m_userListDlg = 0;
	return true;
}

// lobbymain.cpp:7130
bool CLobbyMain::OnNotifyQuitDlgResult(int, FrForm*) {
	g_bQuit = true;
	return true;
}

// lobbymain.cpp:7136
bool CLobbyMain::OnLoginDlgResult(int ret, FrForm*) {
	if (ret == FrOK) {
		if (m_loginDlg) {
			m_loginDlg->Login();
			m_loginDlg->SetCloseResult(FrNONE);
			return false;
		}
	} else {
		if (ret == FrCANCEL) {
			g_bQuit = true;
		}

		m_loginDlg = 0;
	}

	return true;
}

// lobbymain.cpp:7156
bool CLobbyMain::OnDtRoomDlgResult(int ret, FrForm*) {
	if (ret != 1) {
		EnableRoomEnterControls(true);
	}
	m_dtRoomDlg = 0;
	return true;
}

// lobbymain.cpp:7165
bool CLobbyMain::OnVoteDlgResult(int ret, FrForm*) {
	WSendPacket packet(0x27);

	if (ret == 1)
		packet.Encode1(0);
	else
		packet.Encode1(1);

	packet.Send(SEND_TO_DEFAULT);
	return true;
}

// lobbymain.cpp:7290
bool CLobbyMain::OnItemEquipResult(int ret, FrForm*) {
	if (ret == 1 || ret == 2) {
		if (m_itemEquipDlg) {
			memcpy(m_equipItem, m_itemEquipDlg->GetMyItemSlot(), sizeof(m_equipItem));
		}
		m_itemEquipDlg = NULL;
		HandleMsg(MsgObject(0, 1, 0, 0, 0, 0, 0));

		WSendPacket packet(0x20);
		packet.Encode1(2);
		packet.EncodeBuffer(m_equipItem, sizeof(m_equipItem));
		packet.Send(SEND_TO_DEFAULT);
	}
	return true;
}

// lobbymain.cpp:7353
bool CLobbyMain::OnPCBangDlgResult(int, FrForm*) {
	m_pcbangNoticeDlg = 0;

	if (CSharedDoc::Instance()->m_noticeShown) {
		OnToppage_NoticeUp();
	}
	return true;
}

// lobbymain.cpp:7364
bool CLobbyMain::OnCreateInfoResult(int, FrForm*) {
	return true;
}

// lobbymain.cpp:7402
bool CLobbyMain::OnReportDlgResult(int, FrForm*) {
	m_reportDlg = 0;
	return true;
}

// lobbymain.cpp:7408
bool CLobbyMain::OnParanNickChangeResult(int, FrForm*) {
	m_paranNickChangeDlg = 0;

	if (WSingleton<CSharedDoc>::Instance()->m_paranNickChanged) {
		WSingleton<CSharedDoc>::Instance()->m_paranNickChanged = false;
		OpenEventPrizeForm(2);
	}

	OpenCaddieWarningForm();

	return true;
}

// lobbymain.cpp:7424
bool CLobbyMain::OnCaddieWarningDlgResult(int, FrForm*) {
	m_caddieWarningDlg = 0;
	OpenNoteForm();
	return true;
}

// lobbymain.cpp:7431
bool CLobbyMain::OnUIHackDlgResult(int, FrForm*) {
	g_bQuit = true;
	return true;
}

// lobbymain.cpp:7438
bool CLobbyMain::OnMapSelectDlgResult(int, FrForm*) {
	m_mapSelectDlg = 0;
	return true;
}

// lobbymain.cpp:7445
bool CLobbyMain::OnHolesEventDlgResult(int, FrForm*) {
	m_holesEventDlg = 0;
	return true;
}

// lobbymain.cpp:7451
bool CLobbyMain::OnPointEventDlgResult(int, FrForm*) {
	m_pointEventDlg = 0;
	return true;
}

// lobbymain.cpp:7458
bool CLobbyMain::OnWorldTourEventDlgResult(int, FrForm*) {
	m_worldTourEventDlg = 0;
	return true;
}

// lobbymain.cpp:7465
bool CLobbyMain::OnFindEbortEventDlgResult(int, FrForm*) {
	m_findEbortEventDlg = 0;
	return true;
}

// lobbymain.cpp:7472
bool CLobbyMain::OnHalloweenEventDlgResult(int, FrForm*) {
	m_halloweenEventDlg = 0;
	return true;
}

// lobbymain.cpp:7479
bool CLobbyMain::OnChristmasEventDlgResult(int, FrForm*) {
	m_christmasEventDlg = 0;
	return true;
}

// lobbymain.cpp:7487
bool CLobbyMain::OnChristmasSockEventDlgResult(int, FrForm*) {
	m_christmasSockEventDlg = 0;
	return true;
}

// lobbymain.cpp:7495
bool CLobbyMain::OnTicketExchangeDlgResult(int, FrForm*) {
	m_ticketExchangeDlg = 0;
	return true;
}

// lobbymain.cpp:7528
bool CLobbyMain::OnLevelupFormResult(int, FrForm*) {
	if (!m_levelupDlgs.empty()) {

		m_levelupDlgs[m_levelupDlgs.size() - 1] = 0;
		m_levelupDlgs.erase(m_levelupDlgs.end() - 1);
	}

	return true;
}

// lobbymain.cpp:7539
bool CLobbyMain::OnFirstTutorialDlgResult(int, FrForm*) {
	return true;
}

// lobbymain.cpp:7577
bool CLobbyMain::OnQuickResultDlgResult(int, FrForm*) {
	m_quickResultDlg = 0;
	return true;
}

// lobbymain.cpp:7584
void CLobbyMain::OpenLayout(const char* layoutName) {
	this << MsgObject(0, 0, (intptr_t)layoutName, 0, 0, 0, 0);
}

// lobbymain.cpp:7589
void CLobbyMain::ResetControl(void) throw() {
	m_toppageGameBtn = NULL;
	m_toppageFamilyBtn = NULL;
	m_toppagePangysStudyBtn = NULL;
	m_toppageMsgBtn = NULL;
	m_barButton[13] = NULL;
	m_toppageNoticeBtn = NULL;
	m_toppageServerBtn = NULL;

	m_matchingDlg = NULL;
	m_userListDlg = NULL;
	m_dtRoomDlg = NULL;
	m_scoreDlg = NULL;
	m_passwordDlg = NULL;

	m_optionDlg = NULL;
	m_emoticonDlg = NULL;
	m_reportDlg = NULL;
	m_serverDlg = NULL;
	m_guildNoticeDlg = NULL;
	m_itemEquipDlg = NULL;

	m_unusedDlg = NULL;
	m_mailBoxDlg = NULL;
	m_pcbangNoticeDlg = NULL;
	m_resultReportDlg = NULL;
	m_newRecordDlg = NULL;
	m_paranNickChangeDlg = NULL;
	m_caddieWarningDlg = NULL;
	m_roomExtResQuitDlg = NULL;
	m_roomUseReportDlg = NULL;
	m_channelRoomCloseBtn = NULL;
	m_makeRoomBtn = NULL;

	m_guildInfoBtn = NULL;



	m_userListBox = NULL;
	m_roomListBox = NULL;
	m_selectedRoomData = NULL;
	m_languageArea = NULL;
	m_chatEdit = NULL;
	m_chatView = NULL;
	m_chatTargetCombo = NULL;
	m_emoticonBtn = NULL;
	m_roomTitleEdit = NULL;
	m_chatBg = NULL;


	m_mapBgViewer = NULL;
	m_roomOptionBtn = NULL;
	m_roomUserListBox = NULL;
	m_userRankListBox = NULL;
	m_guildRankListBox = NULL;
	m_mapBtn = NULL;
	m_mapPrevBtn = NULL;
	m_mapNextBtn = NULL;
	m_holeCombo = NULL;
	m_mapGauge = NULL;
	m_mapLockArea = NULL;
	m_backGroundViewer = NULL;
	m_startBtn = NULL;
	m_readyBtn = NULL;
	m_roomSettingWnd = NULL;
	m_scoreBtn = NULL;
	m_guildInviteBtn = NULL;
	m_changeTeamBtn = NULL;
	m_teamNumArea = NULL;
	m_teamTab1Area = NULL;
	m_teamTab2Area = NULL;
	m_guildTab1Area = NULL;
	m_guildTab2Area = NULL;

	m_cateAllBtn = NULL;
	m_cateVSBtn = NULL;
	m_cateMassBtn = NULL;
	m_cateBattleBtn = NULL;
	m_cateChatBtn = NULL;
	m_cateVSList = NULL;
	m_cateMassList = NULL;
	m_cateBattleList = NULL;

	m_roomControlWnd = NULL;

	m_userActionBtn = NULL;
	m_extReservedWnd = NULL;

	m_popupDlg = NULL;

	m_mapSelectDlg = NULL;

	m_onelineReqDlg = NULL;
	m_mapExtraWnd = NULL;
	m_extReservedWnd2 = NULL;
	m_extFrame = NULL;
	m_charControlWnd = NULL;
	m_roomImage = NULL;
	m_roomUserImage[0] = NULL;
	m_roomUserImage[1] = NULL;
	m_roomUserMarkImage[0] = NULL;
	m_roomUserMarkImage[1] = NULL;
	m_guildScoreGauge = NULL;
	m_roomUserSelImage = NULL;
	m_roomUserReserved2 = NULL;
	m_roomUserReserved3 = NULL;
	m_roomListImage = NULL;
	m_eventImage = NULL;

	m_tipBlinking = false;
	m_tipBlinkCount = 0;
	m_tipBlinkTime = 0.0f;

	memset(m_extRoomUserImage, 0, sizeof(m_extRoomUserImage));
	memset(m_roomUserReserved, 0, sizeof(m_roomUserReserved));
	memset(m_petArea, 0, sizeof(m_petArea));
	memset(m_eventDlg, 0, sizeof(m_eventDlg));
	memset(&m_powerBar, 0, sizeof(FrGaugeBar*) * 5);
	memset(&m_powerStatic, 0, sizeof(FrStatic*) * 5);

	m_guildNewsReader = NULL;
	m_roomSortResetTime = 0;
	m_userSortResetTime = 0;
	m_blinkTipArea = NULL;
	m_startTipArea = NULL;
	m_readyTipArea = NULL;
	m_countdownArea = NULL;

	m_toppageEvent1Btn = NULL;
}

// lobbymain.cpp:7817
void CLobbyMain::HidePet(unsigned long uid) {
	CSharedDoc *doc = CSharedDoc::Instance();
	std::map<unsigned long, sRoomSlot>::iterator it = doc->m_roomSlotMap.find(uid);

	if (it != doc->m_roomSlotMap.end()) {
		if (it->second.m_pPet) {
			delete it->second.m_pPet;
			it->second.m_pPet = NULL;
		}
	}
}

// lobbymain.cpp:7825
void CLobbyMain::EnableRoomEnterControls(bool enable) {
	if (m_roomListBox)
		m_roomListBox->Enable(enable);

	if (m_makeRoomBtn && !(CSharedDoc::Instance()->m_currentChannel.flags & 0x80))
		m_makeRoomBtn->Enable(enable);

	if (m_guildInfoBtn) {
		m_guildInfoBtn->Enable(enable);
	}

	if (enable) {
		if (m_dtRoomDlg)
			static_cast<FrDtRoomDlg*>(m_dtRoomDlg)->EnableJoinBtn(true);
	}

	if ((CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0xE) == 0xE) {
		if (m_guildInfoBtn)
			m_guildInfoBtn->Enable(false);
	}
}

// lobbymain.cpp:9059
void CLobbyMain::OpenCreateInfo(unsigned long id, const char* text) {
	std::string message;
	if (text) {
		message = text;
	}

	if (g_pFresh->GetManager()->GetDesktop()->FindChildByName("information_large")) {
		return;
	}

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "information_large", 0);

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(id);

	FrArea* pPortrait = DYNAMIC_CAST(FrArea, pForm->FindChildByName("portrait"));
	if (pItem && pPortrait) {
		pPortrait->SetBgImg(pItem->m_icon);
	}

	FrEdit* pName = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("name"));
	FrArea* pLevel = DYNAMIC_CAST(FrArea, pForm->FindChildByName("level"));
	if (pItem) {
		if (pName && pLevel) {
			if (pItem->m_level & 0x80) {
				pLevel->SetBgImg(MakeStr("level_%03d", (pItem->m_level & 0x7F) + 1));
				pName->AddText(MakeStr("\xC0\xCC\xB8\xA7: %s\n\n\xB7\xB9\xBA\xA7:              \xC0\xCC\xC7\xCF", pItem->m_name), false, true); // CP949: "name: %s, level: ... and below"
			} else {
				pLevel->SetBgImg(MakeStr("level_%03d", (pItem->m_level & 0x7F) + 1));
				pName->AddText(MakeStr("\xC0\xCC\xB8\xA7: %s\n\n\xB7\xB9\xBA\xA7:              \xC0\xCC\xBB\xF3", pItem->m_name), false, true); // CP949: "name: %s, level: ... and above"
			}
		}

		if (text == NULL) {
			IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(pItem->m_typeId);
			if (pDesc) {
				if (message[0] != 0) {
					message += "\n\n";
				}

				message += "\\c0xffff0000\\c";
				message += pDesc->m_text;
			}
		}
	}

	pForm->SetMessage(message.c_str(), false);
	pForm->Open(text ? (FRESH_RESULT_CALLBACK)&CLobbyMain::OnCreateInfoResult : 0, 3);
}

// lobbymain.cpp:9273
bool CLobbyMain::LatestVersion(void) {
	if (CSharedDoc::Instance()->m_latestVersion == "645.00") {
		this << MsgObject(this, 0x23, (intptr_t)"\xC3\xD6\xBD\xC5 \xB9\xF6\xC1\xAF\xC0\xB8\xB7\xCE \xBE\xF7\xB5\xA5\xC0\xCC\xC6\xAE\xB0\xA1 \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		return false;
	}

	return true;
}

// lobbymain.cpp:9308
bool RoomStateCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	if (pLhs->m_isPublic == 0 && pRhs->m_isPublic == 1) {
		return false;
	}
	if (pLhs->m_isPublic == 1 && pRhs->m_isPublic == 0) {
		return true;
	}
	if (pLhs->m_intrusionAccess == 0 && pRhs->m_intrusionAccess == 1) {
		return false;
	}
	if (pLhs->m_intrusionAccess == 1 && pRhs->m_intrusionAccess == 0) {
		return true;
	}
	if (pLhs->m_intrusionAccess == 1 && pRhs->m_intrusionAccess == 1) {
		if (pLhs->m_userCount == pLhs->m_userLimit && pRhs->m_userCount < pRhs->m_userLimit) {
			return false;
		}
		if (pLhs->m_userCount < pLhs->m_userLimit && pRhs->m_userCount == pRhs->m_userLimit) {
			return true;
		}
		if (pLhs->m_option[0] == 1 && pRhs->m_option[0] == 0) {
			return false;
		}
		if (pLhs->m_option[0] == 0 && pRhs->m_option[0] == 1) {
			return true;
		}
		if (pLhs->m_state == 0 && pRhs->m_state == 1) {
			return false;
		}
		if (pLhs->m_state == 1 && pRhs->m_state == 0) {
			return true;
		}
	}

	return false;
}

// lobbymain.cpp:9374
bool RoomStateRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	if (pRhs->m_isPublic == 0 && pLhs->m_isPublic == 1) {
		return false;
	}
	if (pRhs->m_isPublic == 1 && pLhs->m_isPublic == 0) {
		return true;
	}
	if (pRhs->m_intrusionAccess == 0 && pLhs->m_intrusionAccess == 1) {
		return false;
	}
	if (pRhs->m_intrusionAccess == 1 && pLhs->m_intrusionAccess == 0) {
		return true;
	}
	if (pRhs->m_intrusionAccess == 1 && pLhs->m_intrusionAccess == 1) {
		if (pRhs->m_userCount == pRhs->m_userLimit && pLhs->m_userCount < pLhs->m_userLimit) {
			return false;
		}
		if (pRhs->m_userCount < pRhs->m_userLimit && pLhs->m_userCount == pLhs->m_userLimit) {
			return true;
		}
		if (pRhs->m_option[0] == 1 && pLhs->m_option[0] == 0) {
			return false;
		}
		if (pRhs->m_option[0] == 0 && pLhs->m_option[0] == 1) {
			return true;
		}
		if (pRhs->m_state == 0 && pLhs->m_state == 1) {
			return false;
		}
		if (pRhs->m_state == 1 && pLhs->m_state == 0) {
			return true;
		}
	}

	return false;
}

// lobbymain.cpp:9440
bool RoomNoCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_roomNumber < pRhs->m_roomNumber;
}

// lobbymain.cpp:9472
bool RoomNoRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_roomNumber > pRhs->m_roomNumber;
}

// lobbymain.cpp:9504
bool RoomHoleCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_holeCount < pRhs->m_holeCount;
}

// lobbymain.cpp:9537
bool RoomHoleRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_holeCount > pRhs->m_holeCount;
}

// lobbymain.cpp:9570
bool RoomCourseCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_course < pRhs->m_course;
}

// lobbymain.cpp:9602
bool RoomCourseRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	// Lounge-sized rooms first.
	if (pLhs->m_userLimit != 100 && pRhs->m_userLimit == 100) {
		return false;
	}
	if (pLhs->m_userLimit == 100 && pRhs->m_userLimit != 100) {
		return true;
	}

	return pLhs->m_course > pRhs->m_course;
}

// lobbymain.cpp:9634
bool RoomTitleCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	return lstrcmpiA(pLhs->m_title, pRhs->m_title) < 0;
}

// lobbymain.cpp:9651
bool RoomTitleRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	return lstrcmpiA(pLhs->m_title, pRhs->m_title) > 0;
}

// lobbymain.cpp:9668
bool RoomGameTypeCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	if (pLhs->m_gameType == pRhs->m_gameType) {
		return pLhs->m_trophy > pRhs->m_trophy;
	}

	return pLhs->m_gameType < pRhs->m_gameType;
}

// lobbymain.cpp:9688
bool RoomGameTypeRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	if (pLhs->m_gameType == pRhs->m_gameType) {
		return pLhs->m_trophy > pRhs->m_trophy;
	}

	return pLhs->m_gameType > pRhs->m_gameType;
}

// lobbymain.cpp:9708
bool RoomUserCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	if (pLhs->m_userLimit > pRhs->m_userLimit) {
		return false;
	}
	if (pLhs->m_userLimit < pRhs->m_userLimit) {
		return true;
	}

	return pLhs->m_userCount <= pRhs->m_userCount ? true : false;
}

// lobbymain.cpp:9734
bool RoomUserRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sRoomInfo* pLhs = (const sRoomInfo*)lhs;
	const sRoomInfo* pRhs = (const sRoomInfo*)rhs;

	if (pLhs->m_userLimit > pRhs->m_userLimit) {
		return true;
	}
	if (pLhs->m_userLimit < pRhs->m_userLimit) {
		return false;
	}

	return pLhs->m_userCount <= pRhs->m_userCount ? true : false;
}

// lobbymain.cpp:9782
bool UserGenderCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->charType < pRhs->charType;
}

// lobbymain.cpp:9799
bool UserGenderRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->charType > pRhs->charType;
}

// lobbymain.cpp:9816
bool UserLevelCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->level < pRhs->level;
}

// lobbymain.cpp:9833
bool UserLevelRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->level > pRhs->level;
}

// lobbymain.cpp:9886
bool UserGuildCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->guildUID < pRhs->guildUID;
}

// lobbymain.cpp:9903
bool UserGuildRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return pLhs->guildUID > pRhs->guildUID;
}

// lobbymain.cpp:9920
bool UserNicknameCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return lstrcmpiA(pLhs->nick, pRhs->nick) < 0;
}

// lobbymain.cpp:9937
bool UserNicknameRevCompare(const void* lhs, const void* rhs) {
	if (!lhs) {
		return false;
	}
	if (!rhs) {
		return true;
	}

	const sBriefUserInfo* pLhs = (const sBriefUserInfo*)lhs;
	const sBriefUserInfo* pRhs = (const sBriefUserInfo*)rhs;

	return lstrcmpiA(pLhs->nick, pRhs->nick) > 0;
}

// lobbymain.cpp:9954
void CLobbyMain::RoomList_LoadRoomImages(void) {
	m_roomListBoxBmp[0][0] = g_pFresh->RegisterBitmap("roomlistbox_n_01");
	m_roomListBoxBmp[1][0] = g_pFresh->RegisterBitmap("roomlistbox_n_02");
	m_roomListBoxBmp[2][0] = g_pFresh->RegisterBitmap("roomlistbox_n_03");
	m_roomListBoxBmp[0][1] = g_pFresh->RegisterBitmap("roomlistbox_o_01");
	m_roomListBoxBmp[1][1] = g_pFresh->RegisterBitmap("roomlistbox_o_02");
	m_roomListBoxBmp[2][1] = g_pFresh->RegisterBitmap("roomlistbox_o_03");
	m_roomListBoxBmp[0][2] = g_pFresh->RegisterBitmap("roomlistbox_d_01");
	m_roomListBoxBmp[1][2] = g_pFresh->RegisterBitmap("roomlistbox_d_02");
	m_roomListBoxBmp[2][2] = g_pFresh->RegisterBitmap("roomlistbox_d_03");
	m_roomListBoxBmp[0][3] = g_pFresh->RegisterBitmap("roomlistbox_e_01");
	m_roomListBoxBmp[1][3] = g_pFresh->RegisterBitmap("roomlistbox_e_02");
	m_roomListBoxBmp[2][3] = g_pFresh->RegisterBitmap("roomlistbox_e_03");

	m_roomSortNoBmp[0] = g_pFresh->RegisterBitmap("room_sort_no_n");
	m_roomSortNoBmp[1] = g_pFresh->RegisterBitmap("room_sort_no_o");
	m_roomSortNoBmp[2] = g_pFresh->RegisterBitmap("room_sort_no_d");
	m_roomSortCateBmp[0] = g_pFresh->RegisterBitmap("room_sort_cate_n_down");
	m_roomSortCateBmp[1] = g_pFresh->RegisterBitmap("room_sort_cate_o");
	m_roomSortCateBmp[2] = g_pFresh->RegisterBitmap("room_sort_cate_d");
	m_roomSortHoleBmp[0] = g_pFresh->RegisterBitmap("room_sort_hole_n");
	m_roomSortHoleBmp[1] = g_pFresh->RegisterBitmap("room_sort_hole_o");
	m_roomSortHoleBmp[2] = g_pFresh->RegisterBitmap("room_sort_hole_d");
	m_roomSortCourseBmp[0] = g_pFresh->RegisterBitmap("room_sort_course_n");
	m_roomSortCourseBmp[1] = g_pFresh->RegisterBitmap("room_sort_course_o");
	m_roomSortCourseBmp[2] = g_pFresh->RegisterBitmap("room_sort_course_d");
	m_roomSortStateBmp[0] = g_pFresh->RegisterBitmap("room_sort_state_up_n");
	m_roomSortStateBmp[1] = g_pFresh->RegisterBitmap("room_sort_state_up_o");
	m_roomSortStateBmp[2] = g_pFresh->RegisterBitmap("room_sort_state_up_d");
	m_roomSortStateBmp[3] = g_pFresh->RegisterBitmap("room_sort_state_dn_n");
	m_roomSortStateBmp[4] = g_pFresh->RegisterBitmap("room_sort_state_dn_o");
	m_roomSortStateBmp[5] = g_pFresh->RegisterBitmap("room_sort_state_dn_d");

	m_genderBmp[0][0] = g_pFresh->RegisterBitmap("i_male");
	m_genderBmp[0][1] = g_pFresh->RegisterBitmap("i_female");
	m_genderBmp[1][0] = g_pFresh->RegisterBitmap("i_male_02");
	m_genderBmp[1][1] = g_pFresh->RegisterBitmap("i_female_02");
	m_genderBmp[2][0] = g_pFresh->RegisterBitmap("i_male_03");
	m_genderBmp[2][1] = g_pFresh->RegisterBitmap("i_female_03");

	m_genderMannerBmp[0] = g_pFresh->RegisterBitmap("i_male_manner");
	m_genderMannerBmp[1] = g_pFresh->RegisterBitmap("i_female_manner");

	m_genderAngelBmp[0] = g_pFresh->RegisterBitmap("i_male_angel");
	m_genderAngelBmp[1] = g_pFresh->RegisterBitmap("i_female_angel");

	m_userSortGenderBmp[0] = g_pFresh->RegisterBitmap("user_sort_gender_normal");
	m_userSortGenderBmp[1] = g_pFresh->RegisterBitmap("user_sort_gender_over");
	m_userSortGenderBmp[2] = g_pFresh->RegisterBitmap("user_sort_gender_click");
	m_userSortLevelBmp[0] = g_pFresh->RegisterBitmap("user_sort_level_normal");
	m_userSortLevelBmp[1] = g_pFresh->RegisterBitmap("user_sort_level_over");
	m_userSortLevelBmp[2] = g_pFresh->RegisterBitmap("user_sort_level_click");

	m_userSortGuildBmp[0] = g_pFresh->RegisterBitmap("user_sort_guild_normal");
	m_userSortGuildBmp[1] = g_pFresh->RegisterBitmap("user_sort_guild_over");
	m_userSortGuildBmp[2] = g_pFresh->RegisterBitmap("user_sort_guild_click");
	m_userSortNickBmp[0] = g_pFresh->RegisterBitmap("user_sort_nick_normal");
	m_userSortNickBmp[1] = g_pFresh->RegisterBitmap("user_sort_nick_over");
	m_userSortNickBmp[2] = g_pFresh->RegisterBitmap("user_sort_nick_click");

	m_userSortArrowBmp[0] = g_pFresh->RegisterBitmap("user_sort_up");
	m_userSortArrowBmp[1] = g_pFresh->RegisterBitmap("user_sort_down");

	m_roomImage = g_pFresh->RegisterBitmap("ingame");
}

// lobbymain.cpp:10044
void CLobbyMain::OnRoomList_Init(int param) {
	CTaskMain::OnInit();

	WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType = 0x10;

	WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode = 0x10;

	RoomList_LoadRoomImages();

	_ISkinnedTheme* theme = S5::THEME::GetCurrentSkinnedTheme();
	g_pFresh->GetDesktop()->SetWallPaper(theme->GetRoomListBackGroundImgName(), true);

	SetTimeVariableBGM();
	TurnOff3DBackground();

	std::map<unsigned long, sRoomSlot>::iterator it;
	for (it = CSharedDoc::Instance()->m_roomSlotMap.begin(); it != CSharedDoc::Instance()->m_roomSlotMap.end(); ++it) {
		if (it->second.m_pPet) { delete it->second.m_pPet; it->second.m_pPet = NULL; }
	}
	CSharedDoc::Instance()->m_roomSlotMap.clear();

	if (IsLocalContent((localContentType_t)0x57)) {
		WSingleton<NetResourceManager>::Instance()->SetClearGarbage();
	}

	CSharedDoc* doc = WSingleton<CSharedDoc>::Instance();
	unsigned long games = doc->m_myStatGamesPlayed;
	if (games < 1) games = 1;
	if ((float)doc->m_myStatNoMannerGames / (float)games * 100.0f >= 3.0f) {
		CPartTidList& partTidList = doc->GetMyPartTidList();
		sCharacterInfo& info = WSingleton<CSharedDoc>::Instance()->m_characterMap[WSingleton<CSharedDoc>::Instance()->m_myCharacterId];

		for (int i = 0; i < partTidList.m_numTotalParts; i++) {
			switch (info.partTypeIds[i]) {
			case 0x8016800:
			case 0x8058800:
			case 0x8098800:
			case 0x80DC800:
			case 0x8118800:
			case 0x8160800:
			case 0x8190800:
			case 0x81E2800:
			case 0x8214800:
			case 0x8254800:
				info.partTypeIds[i] = 0;
				info.partIds[i] = 0;
			}

			switch (partTidList.m_partTids[i]) {
			case 0x8016800:
			case 0x8058800:
			case 0x8098800:
			case 0x80DC800:
			case 0x8118800:
			case 0x8160800:
			case 0x8190800:
			case 0x81E2800:
			case 0x8214800:
			case 0x8254800:
				partTidList.m_partTids[i] = 0;
			}
		}
	}

	m_roomSort = (roomSort_t)6;
	m_roomSortPending = false;
}

// lobbymain.cpp:10101
void CLobbyMain::OnRoomList_Finish(void) {
	m_titleArea->SetBgImg("title_roomlist");

	if (WSingleton<CSharedDoc>::Instance()->m_tutorialFinish) {
		this << MsgObject(0, 0xAA, 1, 0, 0, 0, 0);
	}
}

// lobbymain.cpp:10110
void CLobbyMain::OnRoomList_Destroy(void) {
}

// lobbymain.cpp:10114
void CLobbyMain::OnRoomList_RoomListInit(intptr_t param) {
	m_roomListBox = DYNAMIC_CAST(FrListBox, param);
	if (m_roomListBox) {
		m_roomListBox->UseDummy(true);
		m_roomListBox->UseRightButton(true);

		if (data_aa9eac[3] != -1) {
			std::list<sRoomInfo>::iterator it;
			for (it = WSingleton<CSharedDoc>::Instance()->m_roomList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_roomList.end(); ++it) {
				if (it->m_gameType == data_aa9eac[3]) {
					m_roomListBox->AddItem(&*it);
				}
			}
		} else {
			std::list<sRoomInfo>::iterator it;
			for (it = WSingleton<CSharedDoc>::Instance()->m_roomList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_roomList.end(); ++it) {
				if (GetCateByGameType(it->m_gameType) == data_aa9eac[2] || data_aa9eac[2] == ROOMCATE_ALL) {
					m_roomListBox->AddItem(&*it);
				}
			}
		}

		this << MsgObject(0, 0x0D, 0, 0, 0, 0, 0);
	}
}

// lobbymain.cpp:10210
const char* RoomListGetGameCategory(unsigned char gameType) {
	switch (gameType) {
	case 0:
	case 1:
		return "\xB4\xEB\xC0\xFC";
	case 4:
	case 5:
	case 6:
		return "\xB4\xEB\xC8\xB8";
	case 7:
	case 9:
	case 10:
		return "\xB9\xE8\xC6\xB2";
	case 2:
		return "\xB4\xEB\xC8\xAD";
	}
	return 0;
}

// lobbymain.cpp:10235
unsigned long DecideRoomColor(sRoomInfo* info) {
	if (info->m_intrusionAccess && info->m_isPublic) {
		if (info->m_roomMode != 14) {
			if (!info->m_state) {
				return 0xFFFFFFFF;
			}
			return WSingleton<CIntrusion>::Instance()->GetActiveColor();
		}
		return 0;
	}
	return info->m_roomMode != 14 ? 0xB2FFFFFF : 0;
}

// lobbymain.cpp:10627
void CLobbyMain::OnRoomList_RoomListLDown(void) {
	FrListItem* pItem = m_roomListBox->GetItemUnderCursor();

	if (pItem) {
		if (m_roomListBox) {
			m_roomListBox->SelectItem(pItem, true);
		}
		m_selectedRoomData = pItem->pData;
	} else {
		if (m_roomListBox) {
			m_roomListBox->SelectItem(0, true);
		}
		m_selectedRoomData = 0;
	}
}

// lobbymain.cpp:10933
void CLobbyMain::OnRoomList_RoomSortNoInit(intptr_t param) {
	m_roomSortNoViewer = DYNAMIC_CAST(FrViewer, param);
	m_roomSortDescending[0] = false;
}

// lobbymain.cpp:10939
void CLobbyMain::OnRoomList_RoomSortNoDown(void) {
	if (m_roomSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_roomSort == 0) {
			m_roomSortPending ^= 1;
		}

		m_roomSortDescending[0] = false;
		m_roomSortDescending[1] = false;
		m_roomSortDescending[2] = false;
		m_roomSortDescending[3] = false;
		if (m_cateVSList) {
			m_cateVSList->SetVisible(false);
		}
		if (m_cateMassList) {
			m_cateMassList->SetVisible(false);
		}
		if (m_cateBattleList) {
			m_cateBattleList->SetVisible(false);
		}

		m_roomSortDescending[0] = true;
		m_roomSortResetTime = 0.3f;
		if (m_roomListBox) {
			m_roomSort = (roomSort_t)0;
			if (m_roomSortPending) {
				m_roomListBox->SortItem(RoomNoRevCompare);
			} else {
				m_roomListBox->SortItem(RoomNoCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:10955
void CLobbyMain::OnRoomList_RoomSortNoOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_roomSortNoViewer;

		if (pViewer) {
			int state = 0;

			if (m_roomSortDescending[0]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_roomSortNoBmp[state], pViewer->GetRect(), 0xFFFFFFFF, 0);
		}
	}
}

// lobbymain.cpp:10982
void CLobbyMain::OnRoomList_RoomSortCateAll_BtnInit(intptr_t param) {
	m_cateAllBtn = DYNAMIC_CAST(FrButton, param);
	m_cateAllBtn->SetStatus(data_aa9eac[2] == ROOMCATE_ALL ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:10988
void CLobbyMain::OnRoomList_RoomSortCateAll_BtnDown(void) {
	RoomList_FilterRoomList(ROOMSORT_CATEGORY, ROOMCATE_ALL);
	RoomList_SetCateTabBtn();
	data_aa9eac[3] = -1;
}

// lobbymain.cpp:10996
void CLobbyMain::OnRoomList_RoomSortCateVS_BtnInit(intptr_t param) {
	m_cateVSBtn = DYNAMIC_CAST(FrButton, param);
	m_cateVSBtn->SetStatus(data_aa9eac[2] == ROOMCATE_VS ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:11002
void CLobbyMain::OnRoomList_RoomSortCateVS_BtnDown(void) {
	RoomList_FilterRoomList(ROOMSORT_CATEGORY, ROOMCATE_VS);
	RoomList_SetCateTabBtn();
	m_cateNewRoom[0] = 0;
	data_aa9eac[3] = -1;
}

// lobbymain.cpp:11011
void CLobbyMain::OnRoomList_RoomSortCateMass_BtnInit(intptr_t param) {
	m_cateMassBtn = DYNAMIC_CAST(FrButton, param);
	m_cateMassBtn->SetStatus(data_aa9eac[2] == ROOMCATE_MASS ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:11017
void CLobbyMain::OnRoomList_RoomSortCateMass_BtnDown(void) {
	RoomList_FilterRoomList(ROOMSORT_CATEGORY, ROOMCATE_MASS);
	RoomList_SetCateTabBtn();
	m_cateNewRoom[1] = 0;
	data_aa9eac[3] = -1;
}

// lobbymain.cpp:11026
void CLobbyMain::OnRoomList_RoomSortCateBattle_BtnInit(intptr_t param) {
	m_cateBattleBtn = DYNAMIC_CAST(FrButton, param);
	m_cateBattleBtn->SetStatus(data_aa9eac[2] == ROOMCATE_BATTLE ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:11032
void CLobbyMain::OnRoomList_RoomSortCateBattle_BtnDown(void) {
	RoomList_FilterRoomList(ROOMSORT_CATEGORY, ROOMCATE_BATTLE);
	RoomList_SetCateTabBtn();
	m_cateNewRoom[2] = 0;
	data_aa9eac[3] = -1;
}

// lobbymain.cpp:11041
void CLobbyMain::OnRoomList_RoomSortCateChat_BtnInit(intptr_t param) {
	m_cateChatBtn = DYNAMIC_CAST(FrButton, param);
	m_cateChatBtn->SetStatus(data_aa9eac[2] == ROOMCATE_CHAT ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:11047
void CLobbyMain::OnRoomList_RoomSortCateChat_BtnDown(void) {
	RoomList_FilterRoomList(ROOMSORT_CATEGORY, ROOMCATE_CHAT);
	RoomList_SetCateTabBtn();
	data_aa9eac[3] = -1;
}

// lobbymain.cpp:11155
void CLobbyMain::OnRoomList_RoomSortHoleInit(intptr_t param) {
	m_roomSortHoleViewer = DYNAMIC_CAST(FrViewer, param);
	m_roomSortDescending[1] = false;
}

// lobbymain.cpp:11161
void CLobbyMain::OnRoomList_RoomSortHoleDown(void) {
	if (m_roomSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_roomSort == 4) {
			m_roomSortPending ^= 1;
		}

		m_roomSortDescending[0] = false;
		m_roomSortDescending[1] = false;
		m_roomSortDescending[2] = false;
		m_roomSortDescending[3] = false;
		if (m_cateVSList) {
			m_cateVSList->SetVisible(false);
		}
		if (m_cateMassList) {
			m_cateMassList->SetVisible(false);
		}
		if (m_cateBattleList) {
			m_cateBattleList->SetVisible(false);
		}

		m_roomSortDescending[1] = true;
		m_roomSortResetTime = 0.3f;
		if (m_roomListBox) {
			m_roomSort = (roomSort_t)4;
			if (m_roomSortPending) {
				m_roomListBox->SortItem(RoomHoleRevCompare);
			} else {
				m_roomListBox->SortItem(RoomHoleCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:11177
void CLobbyMain::OnRoomList_RoomSortHoleOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_roomSortHoleViewer;

		if (pViewer) {
			int state = 0;

			if (m_roomSortDescending[1]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_roomSortHoleBmp[state], pViewer->GetRect(), 0xFFFFFFFF, 0);
		}
	}
}

// lobbymain.cpp:11204
void CLobbyMain::OnRoomList_RoomSortCourseInit(intptr_t param) {
	m_roomSortCourseViewer = DYNAMIC_CAST(FrViewer, param);
	m_roomSortDescending[2] = false;
}

// lobbymain.cpp:11210
void CLobbyMain::OnRoomList_RoomSortCourseDown(void) {
	if (m_roomSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_roomSort == 5) {
			m_roomSortPending ^= 1;
		}

		m_roomSortDescending[0] = false;
		m_roomSortDescending[1] = false;
		m_roomSortDescending[2] = false;
		m_roomSortDescending[3] = false;
		if (m_cateVSList) {
			m_cateVSList->SetVisible(false);
		}
		if (m_cateMassList) {
			m_cateMassList->SetVisible(false);
		}
		if (m_cateBattleList) {
			m_cateBattleList->SetVisible(false);
		}

		m_roomSortDescending[2] = true;
		m_roomSortResetTime = 0.3f;
		if (m_roomListBox) {
			m_roomSort = (roomSort_t)5;
			if (m_roomSortPending) {
				m_roomListBox->SortItem(RoomCourseRevCompare);
			} else {
				m_roomListBox->SortItem(RoomCourseCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:11226
void CLobbyMain::OnRoomList_RoomSortCourseOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_roomSortCourseViewer;

		if (pViewer) {
			int state = 0;

			if (m_roomSortDescending[2]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_roomSortCourseBmp[state], pViewer->GetRect(), 0xFFFFFFFF, 0);
		}
	}
}

// lobbymain.cpp:11253
void CLobbyMain::OnRoomList_RoomSortStateInit(intptr_t param) {
	m_roomSortStateViewer = DYNAMIC_CAST(FrViewer, param);
	m_roomSortDescending[3] = false;
}

// lobbymain.cpp:11259
void CLobbyMain::OnRoomList_RoomSortStateDown(void) {
	if (m_roomSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_roomSort == 6) {
			m_roomSortPending ^= 1;
		}

		m_roomSortDescending[0] = false;
		m_roomSortDescending[1] = false;
		m_roomSortDescending[2] = false;
		m_roomSortDescending[3] = false;
		if (m_cateVSList) {
			m_cateVSList->SetVisible(false);
		}
		if (m_cateMassList) {
			m_cateMassList->SetVisible(false);
		}
		if (m_cateBattleList) {
			m_cateBattleList->SetVisible(false);
		}

		m_roomSortDescending[3] = true;
		m_roomSortResetTime = 0.3f;
		if (m_roomListBox) {
			m_roomSort = (roomSort_t)6;
			if (m_roomSortPending) {
				m_roomListBox->SortItem(RoomStateRevCompare);
			} else {
				m_roomListBox->SortItem(RoomStateCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:11275
void CLobbyMain::OnRoomList_RoomSortStateOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_roomSortStateViewer;

		if (pViewer) {
			int state = 0;

			if (m_roomSortDescending[3]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_roomSortStateBmp[state + m_roomSortPending * 3], pViewer->GetRect(), 0xFFFFFFFF, 0);
		}
	}
}

// lobbymain.cpp:11300
void CLobbyMain::OnRoomList_RoomSortCateVS_ListInit(intptr_t param) {
	m_cateVSList = DYNAMIC_CAST(FrListBox, param);
	if (m_cateVSList) {
		for (int i = 0; i < 2; i++) {
			m_cateVSList->AddItem(&g_strGameType[i]);
		}

		m_cateVSList->SetVisible(false);
		m_cateVSList->EnableScrollBar(false);
	}
}

// lobbymain.cpp:11370
void CLobbyMain::OnRoomList_RoomSortCateMass_ListInit(intptr_t param) {
	m_cateMassList = DYNAMIC_CAST(FrListBox, param);
	if (m_cateMassList) {
		for (int i = 2; i < 5; i++) {
			m_cateMassList->AddItem(&g_strGameType[i]);
		}

		m_cateMassList->SetVisible(false);
		m_cateMassList->EnableScrollBar(false);
	}
}

// lobbymain.cpp:11441
void CLobbyMain::OnRoomList_RoomSortCateBattle_ListInit(intptr_t param) {
	m_cateBattleList = DYNAMIC_CAST(FrListBox, param);
	if (m_cateBattleList) {
		for (int i = 5; i < 7; i++) {
			m_cateBattleList->AddItem(&g_strGameType[i]);
		}

		m_cateBattleList->SetVisible(false);
		m_cateBattleList->EnableScrollBar(false);
	}
}

// lobbymain.cpp:11523
void CLobbyMain::OnRoomList_MakeRoomInit(intptr_t param) {
	m_makeRoomBtn = DYNAMIC_CAST(FrButton, param);
	if (m_makeRoomBtn) {
		_ISkinnedTheme* theme = S5::THEME::GetCurrentSkinnedTheme();
		THEME_BUTTONS buttons;
		theme->GetMakeRoomButtonImgName(buttons);

		m_makeRoomBtn->SetButtonImg(buttons.szPush, FrButton::PRESSED);
		m_makeRoomBtn->SetButtonImg(buttons.szNormal, FrButton::NORMAL);
		m_makeRoomBtn->SetButtonImg(buttons.szOver, FrButton::OVER);
	}
}

// lobbymain.cpp:11601
void CLobbyMain::OnRoomList_GuildInfoInit(intptr_t param) {
	m_guildInfoBtn = DYNAMIC_CAST(FrButton, param);
	if (m_guildInfoBtn) {
		_ISkinnedTheme* theme = S5::THEME::GetCurrentSkinnedTheme();

		if (theme) {
			THEME_BUTTONS buttons;
			theme->GetQuickStartButtonImgName(buttons);

			m_guildInfoBtn->SetButtonImg(buttons.szPush, FrButton::PRESSED);
			m_guildInfoBtn->SetButtonImg(buttons.szNormal, FrButton::NORMAL);
			m_guildInfoBtn->SetButtonImg(buttons.szOver, FrButton::OVER);
			m_guildInfoBtn->SetPushDelay(0.1f);
		}
	}
}

// lobbymain.cpp:11820
void CLobbyMain::OnRoomList_UserListInit(intptr_t param) {
	m_userListBox = DYNAMIC_CAST(FrListBox, param);
	if (!m_userListBox) {
		return;
	}

	m_userListBox->UseRightButton(true);

	for (std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.begin();
		it != CSharedDoc::Instance()->m_userMap.end(); ++it) {
		CSharedDoc* doc = CSharedDoc::Instance();

		// An administrator sees everyone; others do not see hidden administrators.
		if (!doc->m_userInfo.identityBits.administrator) {
			sBriefUserInfo user = it->second;
			if (user.identityBits.administrator) {
				sBriefUserInfo admin = it->second;
				if (admin.invisible) {
					continue;
				}
			}
		}

		m_userListBox->AddItem(&it->second);
	}
}

// lobbymain.cpp:11854
void CLobbyMain::OnRoomList_UserListLBtnUp(void) {
	FrListItem* item = m_userListBox->GetItemUnderCursor();
	if (item) {
		sBriefUserInfo* info = (sBriefUserInfo*)item->pData;

		if (info) {
			m_selectedUserId = info->uid;
			m_removeFriendUid = info->id;

			if (m_userActionBtn) {
				m_userActionBtn->Enable(true);
			}
		}

		item->selected = true;
	}
}

// lobbymain.cpp:12203
void CLobbyMain::OnRoomList_UserSortGenderInit(intptr_t param) {
	m_userSortGenderViewer = DYNAMIC_CAST(FrViewer, param);
	m_userSortDescending[0] = false;
}

// lobbymain.cpp:12209
void CLobbyMain::OnRoomList_UserSortGenderUp(void) {
	if (m_userSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_userSort == 0) {
			m_userSortPending ^= 1;
		} else {
			m_userSortPending = false;
		}

		m_userSortDescending[0] = true;
		m_userSortDescending[1] = false;
		m_userSortDescending[2] = false;
		m_userSortDescending[3] = false;
		m_userSortResetTime = 0.3f;
		if (m_userListBox) {
			m_userSort = (userSort_t)0;
			if (m_userSortPending) {
				m_userListBox->SortItem(UserGenderRevCompare);
			} else {
				m_userListBox->SortItem(UserGenderCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:12231
void CLobbyMain::OnRoomList_UserSortGenderOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_userSortGenderViewer;

		if (pViewer) {
			int state = 0;

			if (m_userSortDescending[0]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			WRect rect = pViewer->GetRect();
			pGDI->DrawTexture(m_userSortGenderBmp[state], rect, 0xFFFFFFFF, 0);

			if (m_userSort == 0) {
				WRect arrow(rect.x + rect.w, rect.y + 5.0f, 12.0f, 12.0f);
				pGDI->DrawTexture(m_userSortArrowBmp[m_userSortPending == 0], arrow, 0xFFFFFFFF, 0);
			}
		}
	}
}

// lobbymain.cpp:12263
void CLobbyMain::OnRoomList_UserSortLevelInit(intptr_t param) {
	m_userSortLevelViewer = DYNAMIC_CAST(FrViewer, param);
	m_userSortDescending[1] = false;
}

// lobbymain.cpp:12269
void CLobbyMain::OnRoomList_UserSortLevelUp(void) {
	if (m_userSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_userSort == 1) {
			m_userSortPending ^= 1;
		} else {
			m_userSortPending = false;
		}

		m_userSortDescending[0] = false;
		m_userSortDescending[1] = true;
		m_userSortDescending[2] = false;
		m_userSortDescending[3] = false;
		m_userSortResetTime = 0.3f;
		if (m_userListBox) {
			m_userSort = (userSort_t)1;
			if (m_userSortPending) {
				m_userListBox->SortItem(UserLevelRevCompare);
			} else {
				m_userListBox->SortItem(UserLevelCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:12290
void CLobbyMain::OnRoomList_UserSortLevelOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_userSortLevelViewer;

		if (pViewer) {
			int state = 0;

			if (m_userSortDescending[1]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			WRect rect = pViewer->GetRect();
			pGDI->DrawTexture(m_userSortLevelBmp[state], rect, 0xFFFFFFFF, 0);

			if (m_userSort == 1) {
				WRect arrow(rect.x + rect.w, rect.y + 5.0f, 12.0f, 12.0f);
				pGDI->DrawTexture(m_userSortArrowBmp[m_userSortPending == 0], arrow, 0xFFFFFFFF, 0);
			}
		}
	}
}

// lobbymain.cpp:12369
void CLobbyMain::OnRoomList_UserSortGuildInit(intptr_t param) {
	m_userSortGuildViewer = DYNAMIC_CAST(FrViewer, param);
	m_userSortDescending[2] = false;
}

// lobbymain.cpp:12375
void CLobbyMain::OnRoomList_UserSortGuildUp(void) {
	if (m_userSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_userSort == 2) {
			m_userSortPending ^= 1;
		} else {
			m_userSortPending = false;
		}

		m_userSortDescending[0] = false;
		m_userSortDescending[1] = false;
		m_userSortDescending[2] = true;
		m_userSortDescending[3] = false;
		m_userSortResetTime = 0.3f;
		if (m_userListBox) {
			m_userSort = (userSort_t)2;
			if (m_userSortPending) {
				m_userListBox->SortItem(UserGuildRevCompare);
			} else {
				m_userListBox->SortItem(UserGuildCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:12396
void CLobbyMain::OnRoomList_UserSortGuildOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_userSortGuildViewer;

		if (pViewer) {
			int state = 0;

			if (m_userSortDescending[2]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			WRect rect = pViewer->GetRect();
			pGDI->DrawTexture(m_userSortGuildBmp[state], rect, 0xFFFFFFFF, 0);

			if (m_userSort == 2) {
				WRect arrow(rect.x + rect.w, rect.y + 5.0f, 12.0f, 12.0f);
				pGDI->DrawTexture(m_userSortArrowBmp[m_userSortPending == 0], arrow, 0xFFFFFFFF, 0);
			}
		}
	}
}

// lobbymain.cpp:12428
void CLobbyMain::OnRoomList_UserSortNickInit(intptr_t param) {
	m_userSortNickViewer = DYNAMIC_CAST(FrViewer, param);
	m_userSortDescending[3] = false;
}

// lobbymain.cpp:12434
void CLobbyMain::OnRoomList_UserSortNickUp(void) {
	if (m_userSortResetTime > 0.0f) {
		return;
	}

	{
		if (m_userSort == 3) {
			m_userSortPending ^= 1;
		} else {
			m_userSortPending = false;
		}

		m_userSortDescending[0] = false;
		m_userSortDescending[1] = false;
		m_userSortDescending[2] = false;
		m_userSortDescending[3] = true;
		m_userSortResetTime = 0.3f;
		if (m_userListBox) {
			m_userSort = (userSort_t)3;
			if (m_userSortPending) {
				m_userListBox->SortItem(UserNicknameRevCompare);
			} else {
				m_userListBox->SortItem(UserNicknameCompare);
			}
		}
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// lobbymain.cpp:12455
void CLobbyMain::OnRoomList_UserSortNickOwnerDraw(int) {
	FrWndManager* pManager = g_pFresh->GetManager();
	FrGraphicInterface* pGDI = pManager->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_userSortNickViewer;

		if (pViewer) {
			int state = 0;

			if (m_userSortDescending[3]) {
				state = 2;
			} else {
				WRect rect = pViewer->GetRect();

				if (rect.IsInRect(pManager->GetMousePos())) {
					state = 1;
				}
			}

			WRect rect = pViewer->GetRect();
			pGDI->DrawTexture(m_userSortNickBmp[state], rect, 0xFFFFFFFF, 0);

			if (m_userSort == 3) {
				WRect arrow(rect.x + rect.w, rect.y + 5.0f, 12.0f, 12.0f);
				pGDI->DrawTexture(m_userSortArrowBmp[m_userSortPending == 0], arrow, 0xFFFFFFFF, 0);
			}
		}
	}
}

// Reconstrução estática: popula a lista de jogadores da sala usando os
// registros mantidos por CSharedDoc. Mantém as linhas vazias que a interface
// espera (4 para salas normais, 24 para salas de grande capacidade).
void CLobbyMain::MakeRoomUserList(void) {
    if (m_roomUserListBox == NULL) {
        return;
    }

    CSharedDoc* doc = CSharedDoc::Instance();
    if (doc == NULL) {
        return;
    }

    m_roomUserListBox->ClearItem();

    std::list<sRoomPlayer>::iterator it;
    for (it = doc->m_roomPlayerList.begin(); it != doc->m_roomPlayerList.end(); ++it) {
        m_roomUserListBox->AddItem(&*it);
    }

    m_roomUserListBox->SortItem(ConnectionUserCompare);

    int targetCount = (doc->m_roomInfo.m_gameType == 4 ||
        doc->m_roomInfo.m_gameType == 10) ? 24 : 4;

    while (m_roomUserListBox->GetCurrentItemSize(true) < targetCount) {
        m_roomUserListBox->AddItem(NULL);
    }
}

// lobbymain.cpp:12522
void CLobbyMain::RoomList_SortRoomList(roomSort_t sort, int) {
	if (m_roomListBox == NULL) {
		return;
	}

	m_roomSort = sort;

	switch (sort) {
	case ROOMSORT_NO:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomNoRevCompare);
		} else {
			m_roomListBox->SortItem(RoomNoCompare);
		}
		break;
	case ROOMSORT_TITLE:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomTitleRevCompare);
		} else {
			m_roomListBox->SortItem(RoomTitleCompare);
		}
		break;
	case ROOMSORT_HOLE:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomHoleRevCompare);
		} else {
			m_roomListBox->SortItem(RoomHoleCompare);
		}
		break;
	case ROOMSORT_GAMETYPE:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomGameTypeRevCompare);
		} else {
			m_roomListBox->SortItem(RoomGameTypeCompare);
		}
		// no break in the original: falls into the course sort
	case ROOMSORT_COURSE:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomCourseRevCompare);
		} else {
			m_roomListBox->SortItem(RoomCourseCompare);
		}
		break;
	case ROOMSORT_STATE:
		if (m_roomSortPending) {
			m_roomListBox->SortItem(RoomStateRevCompare);
		} else {
			m_roomListBox->SortItem(RoomStateCompare);
		}
		break;
	}
}

// Reconstrução estática: aplica a ordenação ativa da lista de usuários do lobby.
// Os comparadores e a sequência (gênero, nível, guilda, apelido) já existem no arquivo.
void CLobbyMain::RoomList_SortUserList(userSort_t sort) {
    if (m_userListBox == NULL) {
        return;
    }

    m_userSort = sort;
    bool reverse = (m_userSortPending != false);

    switch ((int)sort) {
    case 0:
        m_userListBox->SortItem(reverse ? UserGenderRevCompare : UserGenderCompare);
        break;
    case 1:
        m_userListBox->SortItem(reverse ? UserLevelRevCompare : UserLevelCompare);
        break;
    case 2:
        m_userListBox->SortItem(reverse ? UserGuildRevCompare : UserGuildCompare);
        break;
    case 3:
    default:
        m_userListBox->SortItem(reverse ? UserNicknameRevCompare : UserNicknameCompare);
        break;
    }
}

// Reconstrução estática: filtra a lista de salas pelo tipo/categoria.
// A lista de origem é CSharedDoc::m_roomList; os eventos 0x04/0x05 mantêm
// a mesma regra de filtragem para salas que chegam/saem enquanto a tela está aberta.
void CLobbyMain::RoomList_FilterRoomList(roomSort_t sort, int cate) {
    if (sort == ROOMSORT_CATEGORY) {
        data_aa9eac[2] = cate;
        data_aa9eac[3] = -1;
    }

    if (m_roomListBox == NULL) {
        return;
    }

    m_roomListBox->ClearItem();

    CSharedDoc* doc = CSharedDoc::Instance();
    if (doc != NULL) {
        std::list<sRoomInfo>::iterator it;
        for (it = doc->m_roomList.begin(); it != doc->m_roomList.end(); ++it) {
            bool includeRoom = false;

            if (data_aa9eac[3] != -1) {
                includeRoom = (it->m_gameType == data_aa9eac[3]);
            } else {
                includeRoom = (data_aa9eac[2] == ROOMCATE_ALL ||
                    GetCateByGameType(it->m_gameType) == data_aa9eac[2]);
            }

            if (includeRoom) {
                m_roomListBox->AddItem(&*it);
            }
        }
    }

    if (sort != ROOMSORT_CATEGORY) {
        RoomList_SortRoomList(sort, 0);
    } else {
        RoomList_SortRoomList(m_roomSort, 0);
    }

    // HandleMsg(0x0D) aplica o preenchimento mínimo de seis linhas e
    // ordena a lista de forma idêntica ao fluxo normal de atualização.
    this << MsgObject(0, 0x0D, 0, 0, 0, 0, 0);
}

// lobbymain.cpp:12572
void CLobbyMain::RoomList_SetCateTabBtn(void) {
	// data_aa9eac[2]: the room list's current category (ROOMCATE_*)
	m_cateVSBtn->SetStatus(data_aa9eac[2] == ROOMCATE_VS ? FrButton::PRESSED : FrButton::NORMAL);
	m_cateMassBtn->SetStatus(data_aa9eac[2] == ROOMCATE_MASS ? FrButton::PRESSED : FrButton::NORMAL);
	m_cateBattleBtn->SetStatus(data_aa9eac[2] == ROOMCATE_BATTLE ? FrButton::PRESSED : FrButton::NORMAL);
	m_cateChatBtn->SetStatus(data_aa9eac[2] == ROOMCATE_CHAT ? FrButton::PRESSED : FrButton::NORMAL);
	m_cateAllBtn->SetStatus(data_aa9eac[2] == ROOMCATE_ALL ? FrButton::PRESSED : FrButton::NORMAL);
}

// lobbymain.cpp:12682
// Clears the sort orders once their reset timers run out and, in the room
// list, opens a category tab's list after the tab is held for 0.8 seconds;
// releasing the button picks from the open list.
void CLobbyMain::ResetControlsRoomListSort(float dt) {
	if (m_roomSortResetTime > 0.0f) {
		m_roomSortResetTime -= dt;
		if (m_roomSortResetTime <= 0.0f) {
			m_roomSortDescending[0] = false;
			m_roomSortDescending[1] = false;
			m_roomSortDescending[2] = false;
			m_roomSortDescending[3] = false;
		}
	}

	if (m_userSortResetTime > 0.0f) {
		m_userSortResetTime -= dt;
		if (m_userSortResetTime <= 0.0f) {
			m_userSortDescending[0] = false;
			m_userSortDescending[1] = false;
			m_userSortDescending[2] = false;
			m_userSortDescending[3] = false;
		}
	}

	if (stricmp(g_pFresh->GetManager()->GetLayoutID(), "ROOMLIST") != 0)
		return;

	if (g_input->GetButton(LEFT_BUTTON) == 3) {
		WVector mouse = g_input->GetMousePoint();
		WVector down = g_input->GetButtonDownPos(LEFT_BUTTON);
		FrButton* tabs[3] = { m_cateVSBtn, m_cateMassBtn, m_cateBattleBtn };
		FrListBox* lists[3] = { m_cateVSList, m_cateMassList, m_cateBattleList };

		for (int i = 0; i < 3; i++) {
			WRect tabRect = tabs[i]->GetRect();
			WRect listRect = lists[i]->GetRect();
			WPoint downPt(down.x, down.y);
			WPoint mousePt(mouse.x, mouse.y);

			if (tabs[i]->GetStatus() == 2 && tabRect.IsInRect(downPt)
				&& (tabRect.IsInRect(mousePt) || listRect.IsInRect(mousePt))) {
				m_cateNewRoom[i] += dt;
				if (m_cateNewRoom[i] > 0.8f)
					lists[i]->SetVisible(true);
			} else {
				m_cateNewRoom[i] = 0.0f;
				lists[i]->SetVisible(false);
			}
		}
	} else {
		if (m_cateVSList->IsVisible())
			OnRoomList_RoomSortCateVS_ListLUp();
		if (m_cateMassList->IsVisible())
			OnRoomList_RoomSortCateMass_ListLUp();
		if (m_cateBattleList->IsVisible())
			OnRoomList_RoomSortCateBattle_ListLUp();

		m_cateNewRoom[0] = 0.0f;
		m_cateNewRoom[1] = 0.0f;
		m_cateNewRoom[2] = 0.0f;
	}
}

// lobbymain.cpp:12791
void CLobbyMain::OnChatBgInit(intptr_t param) {
	m_chatBg = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:12796
void CLobbyMain::OnChatBgOwnerDraw(int) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		pGDI->Box(m_chatBg->GetRect(), 0x58000000, 0, 0.0f);
	}
}

// lobbymain.cpp:12805
void CLobbyMain::OnChatWndBtnInit(intptr_t param) {
	m_chatWndBtn = DYNAMIC_CAST(FrButton, param);
	m_chatWndBtn->SetPushDelay(0.0f);

	bool up = COption::Instance()->gGetChatWndUp() != 0;

	ToggleChatBtn(up);
}

// lobbymain.cpp:12815
void CLobbyMain::OnChatWndBtnUp(void) {
	COption::Instance()->gSetChatWndUp(COption::Instance()->gGetChatWndUp() == 0);
	ToggleChatBtn(true);
}

// lobbymain.cpp:12823
void CLobbyMain::ToggleChatBtn(bool resize) {
	if (COption::Instance()->gGetChatWndUp()) {
		m_chatWndBtn->SetButtonImg("chatwnd_down", FrButton::NORMAL);
		if (resize) {
			WRect rect = m_chatBg->GetRect();
			rect.y -= 200.0f;
			rect.h += 200.0f;
			m_chatBg->SetRect(rect);

			rect = m_chatView->GetRect();
			rect.y -= 200.0f;
			rect.h += 200.0f;
			m_chatView->SetRect(rect);

			if (m_chatView->GetScrollBar()) {
				m_chatView->GetScrollBar()->ScrollToBottom();
			}
		}
	} else {
		m_chatWndBtn->SetButtonImg("chatwnd_up", FrButton::NORMAL);
		if (resize) {
			WRect rect = m_chatBg->GetRect();
			rect.y += 200.0f;
			rect.h -= 200.0f;
			m_chatBg->SetRect(rect);

			rect = m_chatView->GetRect();
			rect.y += 200.0f;
			rect.h -= 200.0f;
			m_chatView->SetRect(rect);

			if (m_chatView->GetScrollBar()) {
				m_chatView->GetScrollBar()->ScrollToBottom();
			}
		}
	}
}

// lobbymain.cpp:12864
void CLobbyMain::ReLoadHoleItem(void) {
	if (m_holeCombo) {
		m_holeCombo->ClearAllListItem();
		m_holeCombo->AddTexItem("hole_front");
		if (CSharedDoc::Instance()->m_roomInfo.m_holeCount < 18) {
			m_holeCombo->AddTexItem("hole_back");
			m_holeCombo->AddTexItem("hole_random");
		}
		m_holeCombo->AddTexItem("hole_shuffle");
	}
}

// lobbymain.cpp:12881
void CLobbyMain::OnRoomList_ChatViewInit(intptr_t param) {
	m_chatView = DYNAMIC_CAST(FrEdit, param);
	m_chatView->HidePrivacy(true);
	CSharedDoc::Instance()->m_chatLineList.clear();
}

// lobbymain.cpp:12889
void CLobbyMain::OnLanguageInit(intptr_t param) {
	m_languageArea = DYNAMIC_CAST(FrArea, param);
	if (m_languageArea) {
		if (g_ime->IsAlphaNumericMode()) {
			m_languageArea->SetBgImg("chat_eng");
		} else {
			m_languageArea->SetBgImg("chat_kor");
		}
	}
}

// lobbymain.cpp:12908
void CLobbyMain::OnChatInputInit(intptr_t param) {
	m_chatEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_chatEdit) {
		m_chatEdit->SetKeyFocus(true);
	}
}

// lobbymain.cpp:12953
void CLobbyMain::OnChatTargetInit(intptr_t param) {
	m_chatTargetCombo = DYNAMIC_CAST(FrComboBox, param);
	m_chatTargetCombo->HidePrivacy(true);
	CSharedDoc::Instance()->InitReservedChatList(m_chatTargetCombo,
		(eReservedChatPartner)(CSharedDoc::Instance()->m_roomInfo.m_gameType == 5 || CSharedDoc::Instance()->m_roomInfo.m_gameType == 6 ? 3 : 2));
}

// lobbymain.cpp:12960
void CLobbyMain::OnChatTargetBtnDown(int) {
	m_chatEdit->SetKeyFocus(false);
}

// lobbymain.cpp:12965
bool CLobbyMain::OnChatTargetEnterKey(int) {
	m_chatEdit->SetKeyFocus(true);
	return false;
}

// lobbymain.cpp:12971
void CLobbyMain::OnEmoticonInit(intptr_t param) {
	m_emoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// lobbymain.cpp:12997
void CLobbyMain::OnChannelRoomCloseBtnInit(intptr_t param) {
	m_channelRoomCloseBtn = DYNAMIC_CAST(FrButton, param);
}

// lobbymain.cpp:13007
void CLobbyMain::OnGameRoom_Init(int) {
	CTaskMain::OnInit();

	m_away = false;
	m_roomOptionA = false;
	m_roomOptionC = false;
	m_roomOptionD = false;
	m_autoStartArmed = false;

	if (CSharedDoc::Instance()->GetPangyaUserInfo().identity == 2) {
		m_roomOptionA = true;
		m_roomOptionC = true;
	}

	m_roomOptionB = false;
	m_selectedUserId = 0xFFFFFFFF;
	m_removeFriendUid = 0xFFFFFFFF;
	m_blinkTipArea = NULL;
	m_startTipArea = NULL;
	m_readyTipArea = NULL;

	g_mouse->ResetInputTime();
	g_ime->ResetInputTime();

	if (CSharedDoc::Instance()->m_roomInfo.m_roomMode == 14) {
		g_pFresh->GetDesktop()->SetWallPaper("chaos_background.jpg", true);
		(!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xBD\xBA\xC6\xF3\xBC\xC8 \xBC\xC5\xC7\xC3 \xB8\xF0\xB5\xE5\xBF\xA1\xBC\xAD\xB4\xC2, \n\\c0xffff0000\\c\xB0\xD4\xC0\xD3 \xC1\xDF \xC0\xD4\xC0\xE5\\c0xff000000\\c \xB9\xD7 \\c0xffff0000\\c\xC6\xBC\xC5\xB0\xB8\xAE\xC6\xF7\xC6\xAE\\c0xff000000\\c \xBB\xE7\xBF\xEB\xC0\xCC \xBA\xD2\xB0\xA1\xB4\xC9 \xC7\xD5\xB4\xCF\xB4\xD9.",
			0, 0, 0, 0));
	} else {
		g_pFresh->GetDesktop()->SetWallPaper("gameroom_background.jpg", true);
	}

	SetTimeVariableBGM();
}

// lobbymain.cpp:13082
void CLobbyMain::OnGameRoom_Destroy(void) {
	EnableUnderBar(true);
}

// lobbymain.cpp:13087
void CLobbyMain::OnGameRoom_BackGroundInit(intptr_t param) {
	m_backGroundViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_backGroundViewer) {
		m_backGroundViewer->SetMouseEvent(false);
	}
}

// lobbymain.cpp:13094
void CLobbyMain::OnGameRoom_Pet0Init(intptr_t param) {
	m_petArea[0] = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:13101
void CLobbyMain::OnGameRoom_Pet1Init(intptr_t param) {
	m_petArea[1] = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:13108
void CLobbyMain::OnGameRoom_Pet2Init(intptr_t param) {
	m_petArea[2] = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:13115
void CLobbyMain::OnGameRoom_Pet3Init(intptr_t param) {
	m_petArea[3] = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:13122
void CLobbyMain::Banish(void) {
	if (m_selectedUserId != (unsigned long)-1) {
		WSendPacket packet((enumClientPacket)0x26);
		packet.Encode4(m_removeFriendUid);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// Reconstrução estática: vincula o controle da lista de jogadores da sala.
void CLobbyMain::OnGameRoom_RoomUserInit(int param) {
    m_roomUserListBox = DYNAMIC_CAST(FrListBox, param);
    if (m_roomUserListBox == NULL) {
        return;
    }

    m_roomUserListBox->UseDummy(true);
    m_roomUserListBox->UseRightButton(true);
    MakeRoomUserList();
}

// lobbymain.cpp:13737
void CLobbyMain::OnGameRoom_RoomUserRBtnUp(void) {
	OnGameRoom_RoomUserLBtnUp();

	if (WSingleton<CUserInfo>::Instance()) {
		CSharedDoc* doc = CSharedDoc::Instance();
		std::map<unsigned long, sBriefUserInfo>::iterator it = doc->m_userMap.find(m_selectedUserId);
		if (it != doc->m_userMap.end()) {
			it->second.roomNumber = doc->m_roomInfo.m_roomNumber;
		}
		WSingleton<CUserInfo>::Instance()->SetInfo(it->second.id, m_selectedUserId, true, true, false, false, it->second.nick);
	}
}

// lobbymain.cpp:13762
void CLobbyMain::OnGameRoom_RoomUserDClick(void) {
	if (GalleryMode()) {
		if (MyGuid(true) != m_selectedUserId) {
			WSendPacket packet((enumClientPacket)0x3F);
			packet.Encode4(m_removeFriendUid);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
}

// lobbymain.cpp:13775
void CLobbyMain::OnGameRoom_TitleInit(intptr_t param) {
	m_roomTitleEdit = DYNAMIC_CAST(FrEdit, param);
	HandleMsg(MsgObject(this, 0x10, 0, 0, 0, 0, 0));

	WSingleton<CSharedDoc>::Instance()->SortAllMyItemList();

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userLimit >= 100) {
		m_roomTitleEdit->SetFontColor(0xFF47A5DD);
	} else {
		m_roomTitleEdit->SetFontColor(0xFFFFFFFF);
	}
}

// lobbymain.cpp:13788
void CLobbyMain::OnGameRoom_EventOwnerDraw(int) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userLimit >= 100) {
		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		if (pGDI) {
			if (m_eventImage == 0) {
				m_eventImage = g_pFresh->GetBitmap("room_event02");
			}

			if (m_eventImage) {
				pGDI->DrawTexture(m_eventImage, WRect(8.0f, 45.0f, 43.0f, 32.0f), 0xFFFFFFFF, 0);
			}
		}
	}
}

// lobbymain.cpp:13804
void CLobbyMain::OnGameRoom_MapBgInit(intptr_t param) {
	m_mapBgViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_mapBgViewer) {
		m_mapBgViewer->SetMouseEvent(false);
	}
}

// lobbymain.cpp:13880
void CLobbyMain::InitControlsSelectedUserInfo(void) {
	m_subSelection = 0;
	m_subSelectionTab = 0;

	if (m_charSelViewer) m_charSelViewer->SetVisible(false);
	if (m_caddieSelViewer) m_caddieSelViewer->SetVisible(false);
	if (m_aztecSelViewer) m_aztecSelViewer->SetVisible(false);
	if (m_clubSelViewer) m_clubSelViewer->SetVisible(false);
	if (m_mascotSelViewer) m_mascotSelViewer->SetVisible(false);
}

// lobbymain.cpp:13957
void CLobbyMain::GameExtResQuit(void) {
	if (!IsLocalContent((localContentType_t)0x14)) {
		m_useReportEnabled = false;
	}

	if (m_resultReportDlg == 0) {
		if (m_useReportEnabled && CSharedDoc::Instance()->m_roomInfo.m_roomMode != 14) {
			OpenUseReportForm();
		} else {
			OpenGameExtResQuitForm();
		}
	}
}

// lobbymain.cpp:14014
void CLobbyMain::OpenUseReportForm(void) {
	if (m_roomUseReportDlg == 0) {
		m_roomUseReportDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", NULL);
		m_roomUseReportDlg->SetMessage("\xC6\xBC\xC5\xB0 \xB8\xAE\xC6\xF7\xC6\xAE \xBF\xEB\xC1\xF6\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		m_roomUseReportDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnRoomUseReportDlgResult, 1);
	}
}

// lobbymain.cpp:14027
void CLobbyMain::OpenGameExtResQuitForm(void) {
	if (m_roomExtResQuitDlg == 0) {
		m_roomExtResQuitDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", NULL);

		m_roomExtResQuitDlg->SetMessage("\xB0\xD4\xC0\xD3\xC0\xCC \\c0xffff0000\\c\xBF\xCF\xC0\xFC\xC8\xF7 \xC1\xBE\xB7\xE1\\c0xff000000\\c\xB5\xC7\xB1\xE2 \xC0\xFC\xBF\xA1 \xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB8\xE9, \\c0xffff0000\\c\xB0\xE6\xC7\xE8\xC4\xA1 \xC8\xB9\xB5\xE6\xB0\xFA \xB0\xA2\xC1\xBE \xB1\xE2\xB7\xCF\xC0\xCC \xB9\xAB\xC8\xBF\xC3\xB3\xB8\xAE\\c0xff000000\\c\xB5\xC7\xC1\xF6\xB8\xB8 \\c0xffff0000\\c\xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xC0\xB2\xC0\xBA \xBF\xC3\xB6\xF3\xB0\xA1\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9.\\c0xff000000\\c\n\n\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xB7\xCE\xBA\xF1\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);

		m_roomExtResQuitDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnRoomExtResQuitDlgResult, 1);
	}
}

// lobbymain.cpp:14041
void CLobbyMain::OnGameRoom_CharInit(intptr_t param) {
	m_charViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:14046
void CLobbyMain::OnGameRoom_CharDown(void) {
	WRect rect = m_charSelViewer->GetRect();
	WPoint pt(rect.x, rect.y - 5.0f);
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)1, 1);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();

		barRect.x = rect.x - barRect.w * 0.5f + 0.5f;
		if (barRect.x + barRect.w > g_view->GetWidth()) {
			barRect.x = g_view->GetWidth() - barRect.w;
		}
		m_subEquipBar->SetRect(barRect);
	}
}

// lobbymain.cpp:14108
void CLobbyMain::OnGameRoom_CaddieInit(intptr_t param) {
	m_caddieViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:14113
void CLobbyMain::OnGameRoom_CaddieDown(void) {
	WRect rect = m_caddieSelViewer->GetRect();
	WPoint pt(rect.x, rect.y - 5.0f);
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)7, 1);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();

		barRect.x = rect.x - barRect.w * 0.5f + 0.5f;
		if (barRect.x + barRect.w > g_view->GetWidth()) {
			barRect.x = g_view->GetWidth() - barRect.w;
		}
		m_subEquipBar->SetRect(barRect);
	}
}

// lobbymain.cpp:14178
void CLobbyMain::OnGameRoom_ClubInit(intptr_t param) {
	m_clubViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:14183
void CLobbyMain::OnGameRoom_ClubDown(void) {
	WRect rect = m_clubSelViewer->GetRect();
	WPoint pt(rect.x, rect.y - 5.0f);
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)4, 1);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();

		barRect.x = rect.x - barRect.w * 0.5f + 0.5f;
		if (barRect.x + barRect.w > g_view->GetWidth()) {
			barRect.x = g_view->GetWidth() - barRect.w;
		}
		m_subEquipBar->SetRect(barRect);
	}
}

// lobbymain.cpp:14242
void CLobbyMain::OnGameRoom_AztecInit(intptr_t param) {
	m_aztecViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:14247
void CLobbyMain::OnGameRoom_AztecDown(void) {
	WRect rect = m_aztecSelViewer->GetRect();
	WPoint pt(rect.x, rect.y - 5.0f);
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)5, 1);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();

		barRect.x = rect.x - barRect.w * 0.5f + 0.5f;
		if (barRect.x + barRect.w > g_view->GetWidth()) {
			barRect.x = g_view->GetWidth() - barRect.w;
		}
		m_subEquipBar->SetRect(barRect);
	}
}

// lobbymain.cpp:14347
void CLobbyMain::OnGameRoom_MascotInit(intptr_t param) {
	m_mascotViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:14352
void CLobbyMain::OnGameRoom_MascotDown(void) {
	WRect rect = m_mascotSelViewer->GetRect();
	WPoint pt(rect.x, rect.y - 5.0f);
	OpenSubEquipBar(pt, (PANGYA_ITEM_GROUP)16, 1);

	if (m_subEquipBar) {
		WRect barRect = m_subEquipBar->GetRect();

		barRect.x = rect.x - barRect.w * 0.5f + 0.5f;
		if (barRect.x + barRect.w > g_view->GetWidth()) {
			barRect.x = g_view->GetWidth() - barRect.w;
		}
		m_subEquipBar->SetRect(barRect);
	}
}

// lobbymain.cpp:14405
void CLobbyMain::OnGameRoom_EquipItemInit(intptr_t param) {
	m_equipItemViewer = DYNAMIC_CAST(FrViewer, param);
}

// lobbymain.cpp:11540
void CLobbyMain::OnRoomList_MakeRoomUp(void) {
	CloseAllDialogs();

	if (LatestVersion()) {
		if (CSharedDoc::Instance()->IsControlServerService(2)) {
			HandleMsg(MsgObject(this, 0x272, 2, 0, 0, 0, 0));
			return;
		}

		if (m_serverDlg == 0 && CSharedDoc::Instance()->m_currentChannel.id == 0xFF) {
			WSendPacket packet((enumClientPacket)0x43);
			packet.Send(SEND_TO_DEFAULT);

			m_serverDlg = CreateForm<FrServerDlg>(g_pFresh->GetManager(), this, "server", 0);
			m_serverDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnServerDlgResult, WPoint(m_serverDlg->GetRect().x, 50.0f), 0x10);
			m_serverDlg->SetIconRect(m_barButton[8]->GetRect());
		} else {
			EnableRoomEnterControls(true);

			m_makeRoomDlg = CreateForm<FrMakeRoomDlg>(g_pFresh->GetManager(), this, "makeroom", 0);
			m_makeRoomDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnMakeRoomDlgResult, 3);
		}
	}
}

// lobbymain.cpp:14410
void CLobbyMain::OnGameRoom_EquipItemDown(void) {
	InitControlsSelectedUserInfo();

	if (!m_itemEquipDlg) {
		m_itemEquipDlg = CreateForm<FrEquipDlg>(g_pFresh->GetManager(), this, "equip", NULL);

		CSharedDoc* doc = CSharedDoc::Instance();
		if ((doc->m_userInfo.identity >> 1) & 1)
			m_itemEquipDlg->Init(doc->m_roomPlayers[0].m_equipItem, true);
		else
			m_itemEquipDlg->Init(doc->m_equipItem, true);

		m_itemEquipDlg->Open((FRESH_RESULT_CALLBACK)&CLobbyMain::OnItemEquipResult, 3);
	}
}

// lobbymain.cpp:14450
void CLobbyMain::OnGameRoom_CharSelInit(intptr_t param) {
	m_charSelViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_charSelViewer) {
		m_charSelViewer->SetVisible(false);
	}
}

// lobbymain.cpp:14460
void CLobbyMain::OnGameRoom_CharSelDown(void) {
	InitControlsSelectedUserInfo();
}

// lobbymain.cpp:14544
void CLobbyMain::OnGameRoom_CaddieSelInit(intptr_t param) {
	m_caddieSelViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_caddieSelViewer) {
		m_caddieSelViewer->SetVisible(false);
	}
}

// lobbymain.cpp:14554
void CLobbyMain::OnGameRoom_CaddieSelDown(void) {
	InitControlsSelectedUserInfo();
}

// lobbymain.cpp:14674
void CLobbyMain::OnGameRoom_ClubSelInit(intptr_t param) {
	m_clubSelViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_clubSelViewer) {
		m_clubSelViewer->SetVisible(false);
	}
}

// lobbymain.cpp:14684
void CLobbyMain::OnGameRoom_ClubSelDown(void) {
	InitControlsSelectedUserInfo();
}

// lobbymain.cpp:14784
void CLobbyMain::OnGameRoom_AztecSelInit(intptr_t param) {
	m_aztecSelViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_aztecSelViewer) {
		m_aztecSelViewer->SetVisible(false);
	}
}

// lobbymain.cpp:14794
void CLobbyMain::OnGameRoom_AztecSelDown(void) {
	InitControlsSelectedUserInfo();
}

// lobbymain.cpp:14894
void CLobbyMain::OnGameRoom_MascotSelInit(intptr_t param) {
	m_mascotSelViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_mascotSelViewer) {
		m_mascotSelViewer->SetVisible(false);
	}
}

// lobbymain.cpp:14904
void CLobbyMain::OnGameRoom_MascotSelDown(void) {
	InitControlsSelectedUserInfo();
}

// lobbymain.cpp:15013
void CLobbyMain::OnGameRoom_StartTipInit(intptr_t param) {
	m_startTipArea = DYNAMIC_CAST(FrArea, param);
	if (m_startTipArea) {
		m_startTipArea->SetVisible(false);
	}
}

// lobbymain.cpp:15022
void CLobbyMain::OnGameRoom_ReadyTipInit(intptr_t param) {
	m_readyTipArea = DYNAMIC_CAST(FrArea, param);
	if (m_readyTipArea) {
		m_readyTipArea->SetVisible(false);
	}
}

// lobbymain.cpp:15031
void CLobbyMain::OnGameRoom_StartInit(intptr_t param) {
	m_startBtn = DYNAMIC_CAST(FrButton, param);
}

// lobbymain.cpp:15167
void CLobbyMain::OnGameRoom_ReadyInit(intptr_t param) {
	m_readyBtn = DYNAMIC_CAST(FrButton, param);
	if (m_readyBtn) {
		m_readyBtn->SetVisible(false);
		m_readyBtn->Enable(false);
	}
}

// lobbymain.cpp:15262
void CLobbyMain::OnGameRoom_RoomOptionInit(intptr_t param) {
	m_roomOptionBtn = DYNAMIC_CAST(FrButton, param);
}

// lobbymain.cpp:15340
void CLobbyMain::OnGameRoom_MapPrevInit(intptr_t param) {
	m_mapPrevBtn = DYNAMIC_CAST(FrButton, param);
	if (m_mapPrevBtn) {
		m_mapPrevBtn->Enable(false);
		m_mapPrevBtn->SetVisible(false);
		m_mapPrevBtn->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:15351
void CLobbyMain::OnGameRoom_MapNextInit(intptr_t param) {
	m_mapNextBtn = DYNAMIC_CAST(FrButton, param);
	if (m_mapNextBtn) {
		m_mapNextBtn->Enable(false);
		m_mapNextBtn->SetVisible(false);
		m_mapNextBtn->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:15484
void CLobbyMain::OnGameRoom_MapLockInit(intptr_t param) {
	m_mapLockArea = DYNAMIC_CAST(FrArea, param);
	if (m_mapLockArea) {
		m_mapLockArea->SetVisible(false);
	}
}

// lobbymain.cpp:15494
void CLobbyMain::OnGameRoom_UserListInit(int) {
}

// lobbymain.cpp:15519
void CLobbyMain::OnGameRoom_ChangeTeamBtnInit(intptr_t param) {
	m_changeTeamBtn = DYNAMIC_CAST(FrButton, param);
	if (m_changeTeamBtn) {
		m_changeTeamBtn->SetPushDelay(0.5f);
	}
}

// lobbymain.cpp:15547
void CLobbyMain::OnGameRoom_PowerBarInit(intptr_t param) {
	m_powerBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_powerBar->SetRange(0, 50, 0);
}

// lobbymain.cpp:15553
void CLobbyMain::OnGameRoom_ControlBarInit(intptr_t param) {
	m_controlBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_controlBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:15559
void CLobbyMain::OnGameRoom_AccuracyBarInit(intptr_t param) {
	m_accuracyBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_accuracyBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:15565
void CLobbyMain::OnGameRoom_SpinBarInit(intptr_t param) {
	m_spinBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_spinBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:15571
void CLobbyMain::OnGameRoom_CurveBarInit(intptr_t param) {
	m_curveBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_curveBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:15577
void CLobbyMain::OnGameRoom_PowerEditInit(intptr_t param) {
	m_powerStatic = DYNAMIC_CAST(FrStatic, param);
}

// lobbymain.cpp:15582
void CLobbyMain::OnGameRoom_ControlEditInit(intptr_t param) {
	m_controlStatic = DYNAMIC_CAST(FrStatic, param);
}

// lobbymain.cpp:15587
void CLobbyMain::OnGameRoom_AccuracyEditInit(intptr_t param) {
	m_accuracyStatic = DYNAMIC_CAST(FrStatic, param);
}

// lobbymain.cpp:15592
void CLobbyMain::OnGameRoom_SpinEditInit(intptr_t param) {
	m_spinStatic = DYNAMIC_CAST(FrStatic, param);
}

// lobbymain.cpp:15597
void CLobbyMain::OnGameRoom_CurveEditInit(intptr_t param) {
	m_curveStatic = DYNAMIC_CAST(FrStatic, param);
}

// lobbymain.cpp:15602
void CLobbyMain::OnGameRoom_ScoreInit(intptr_t param) {
	m_scoreBtn = DYNAMIC_CAST(FrButton, param);
	if (m_scoreBtn) {
		m_scoreBtn->Enable(false);
	}
}

// lobbymain.cpp:15645
void CLobbyMain::OnGameRoomExt_Init(int param) {
	OnGameRoom_Init(param);
}

// lobbymain.cpp:15704
void CLobbyMain::OnGameRoomExt_Destroy(void) {
	EnableUnderBar(true);
}

// lobbymain.cpp:15995
void CLobbyMain::OnGameRoomExt_CountdownInit(intptr_t param) {
	m_countdownArea = DYNAMIC_CAST(FrArea, param);
	if (m_countdownArea) {
		m_countdownArea->SetVisible(false);
	}
}

// lobbymain.cpp:16064
void CLobbyMain::OnGameRoomExt_FrameInit(intptr_t param) {
	m_extFrame = DYNAMIC_CAST(FrFrame, param);
}

// lobbymain.cpp:16069
void CLobbyMain::OnGameRoomExt_GuildTab1Init(intptr_t param) {
	m_guildTab1Area = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:16074
void CLobbyMain::OnGameRoomExt_GuildTab2Init(intptr_t param) {
	m_guildTab2Area = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:16079
void CLobbyMain::OnGameRoomExt_TeamTab1Init(intptr_t param) {
	m_teamTab1Area = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:16084
void CLobbyMain::OnGameRoomExt_TeamTab2Init(intptr_t param) {
	m_teamTab2Area = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:16089
void CLobbyMain::OnGameRoomExt_TeamNumInit(intptr_t param) {
	m_teamNumArea = DYNAMIC_CAST(FrArea, param);
}

// lobbymain.cpp:16127
void CLobbyMain::OnGameRoomExtRes_GuildScoreGaugeInit(intptr_t param) {
	m_guildScoreGauge = DYNAMIC_CAST(FrGaugeBar, param);
	m_guildScoreGauge->SetVisible(false);
}

// lobbymain.cpp:16155
void CLobbyMain::OnGameRoomExt_GuildNumInit(intptr_t param) {
	m_guildNumArea = DYNAMIC_CAST(FrArea, param);
	if (CSharedDoc::Instance()->m_roomInfo.m_gameType != 6) {
		m_guildNumArea->SetVisible(false);
	}
}

// lobbymain.cpp:16231
void CLobbyMain::OnGameRoomExt_GuildInviteInit(intptr_t param) {
	m_guildInviteBtn = DYNAMIC_CAST(FrButton, param);
}

// lobbymain.cpp:16333
void CLobbyMain::OnGameRoomExtRes_RoomUserRankTabInit(intptr_t param) {
	m_userRankTabArea = DYNAMIC_CAST(FrArea, param);
	if (m_userRankTabArea) {
		if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 10) {
			m_userRankTabArea->SetBgImg("approach_tab");
		}
	}
}

// lobbymain.cpp:16552
void CLobbyMain::OnGameRoomExtRes_RoomUserRankLBtnUp(void) {
	FrListItem* item = m_userRankListBox->GetItemUnderCursor();

	if (!item) {
		return;
	}

	sGameResultInfo* info = (sGameResultInfo*)item->pData;
	if (!info) {
		return;
	}

	m_selectedUserId = info->m_uid;
	m_removeFriendUid = info->m_id;

	if (m_scoreDlg) {
		m_scoreDlg->SetPlayer(m_selectedUserId);
	} else {
		if (m_scoreBtn) {
			m_scoreBtn->Enable(true);
		}
		if (m_userActionBtn) {
			m_userActionBtn->Enable(true);
		}
	}
}

// lobbymain.cpp:16671
void CLobbyMain::OnGameRoomExtRes_RoomGuildRankLBtnUp(void) {
	FrListItem* item = m_guildRankListBox->GetItemUnderCursor();

	if (!item) {
		return;
	}

	sGuildRankEntry* entry = (sGuildRankEntry*)item->pData;
	if (!entry) {
		return;
	}

	if (m_scoreDlg) {
		m_scoreDlg->SetGuildPlayer(entry->m_uid);
	} else {
		if (m_scoreBtn) {
			m_scoreBtn->Enable(true);
		}
	}
}

// lobbymain.cpp:16820
void CLobbyMain::OnGameRoomExt_UserInfoInit(intptr_t param) {
	m_userActionBtn = DYNAMIC_CAST(FrButton, param);
	if (m_userActionBtn) {
		m_userActionBtn->Enable(false);
	}
}

// lobbymain.cpp:16912
void CLobbyMain::OnGameRoomExtRes_Finish(void) {
	m_titleArea->SetBgImg("title_mass");

	EnableUnderBar(false);
	m_barButton[15]->Enable(true);

	OpenNewRecordForm();
}

// lobbymain.cpp:16922
void CLobbyMain::OnGameRoomExtRes_Destroy(void) {
	EnableUnderBar(true);
}

// lobbymain.cpp:16932
void CLobbyMain::OnCreate_Init(int) {
	CTaskMain::OnInit();
}

// lobbymain.cpp:16937
void CLobbyMain::OnCreate_Finish(void) {
}

// lobbymain.cpp:16942
void CLobbyMain::OnCreate_Destroy(void) {
	if (m_createNickDlg != 0) {
		m_createNickDlg->CloseDlg(1);
		m_createNickDlg = 0;
	}
}

// lobbymain.cpp:16984
void CLobbyMain::OnCreatePetOwnerDraw(void) {
	if (m_bongdariShop[2]) { // 0x122, the same byte CTaskMain names as the third Bongdari shop flag
		return;
	}

	CSharedDoc* doc = WSingleton<CSharedDoc>::Instance();
	int guid = MyGuid(false);
	std::map<unsigned long, sRoomSlot>::iterator it = doc->m_roomSlotMap.find(guid);
	if (it == doc->m_roomSlotMap.end()) {
		return;
	}

	sRoomSlot& slot = doc->m_roomSlotMap[guid];

	if (slot.m_pPet) {
		WSingleton<CSharedDoc>::Instance()->m_roomSlotMap[MyGuid(false)].m_pPet->Display(0.0f, 0.0f);
	}
}

// lobbymain.cpp:16999
void CLobbyMain::OnCreate_CharInit(intptr_t param) {
	m_createCharEdit = DYNAMIC_CAST(FrEdit, param);
}

// lobbymain.cpp:17038
void CLobbyMain::OnCreate_CharPrevInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17050
void CLobbyMain::OnCreate_CharNextInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17088
void CLobbyMain::OnCreate_HairInit(intptr_t param) {
	m_createHairColor = 0;
	m_createHairEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_createHairEdit) {
		m_createHairEdit->SetLine(1, "\xb8\xd3\xb8\xae\xbb\xf6" "1", 0, false, 0); // "hair color 1" (CP949)
	}
}

// lobbymain.cpp:17096
void CLobbyMain::OnCreate_HairPrevInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17125
void CLobbyMain::OnCreate_HairNextInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17154
void CLobbyMain::OnCreate_ShirtsInit(intptr_t param) {
	m_createShirtsEdit = DYNAMIC_CAST(FrEdit, param);
	m_createShirtsColor = 0;
	if (m_createShirtsEdit) {
		m_createShirtsEdit->SetLine(1, "\xbc\xc5\xc3\xf7\xbb\xf6" "1", 0, false, 0); // "shirts color 1" (CP949)
	}
}

// lobbymain.cpp:17163
void CLobbyMain::OnCreate_ShirtsPrevInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17184
void CLobbyMain::OnCreate_ShirtsNextInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (pButton) {
		pButton->SetPushDelay(0.0f);
	}
}

// lobbymain.cpp:17205
void CLobbyMain::OnCreate_CreateBtnUp(void) {
	sCreateCharacter data;
	data.m_typeId = m_createCharType | 0x4000000;
	data.m_colors = (m_createHairColor & 0xF) | (m_createShirtsColor << 4);
	WSendPacket packet((enumClientPacket)0x08);
	packet.EncodeBuffer(&data, sizeof(data));
	packet.Send(SEND_TO_LOGIN);
	WSingleton<CSharedDoc>::Instance()->m_paranNickChanged = true;
	MsgObject msg(NULL, 1, 0, 0, 0, 0, 0);
	HandleMsg(msg);
}

// lobbymain.cpp:17225
void CLobbyMain::OnCreate_DescInit(intptr_t param) {
	m_createDescEdit = DYNAMIC_CAST(FrEdit, param);
}

// lobbymain.cpp:17230
void CLobbyMain::OnCreate_WarningInit(intptr_t param) {
	FrEdit* pEdit = DYNAMIC_CAST(FrEdit, param);

	pEdit->AddText("\\c0xffff0000\\c* \xc7\xd1 \xb9\xf8 \xbc\xb1\xc5\xc3\xc7\xd1 \xc4\xb3\xb8\xaf\xc5\xcd\xb4\xc2 \xb3\xaa\xc1\xdf\xbf\xa1 \xb9\xd9\xb2\xdc \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.\\c0xff000000\\c", false, true);
	pEdit->AddText("  \xbd\xc5\xc1\xdf\xc8\xf7 \xbc\xb1\xc5\xc3\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", false, true);
}

// lobbymain.cpp:17238
void CLobbyMain::On_PowerBarInit(intptr_t param) {
	m_powerBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_powerBar->SetRange(0, 50, 0);
}

// lobbymain.cpp:17244
void CLobbyMain::On_ControlBarInit(intptr_t param) {
	m_controlBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_controlBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:17250
void CLobbyMain::On_AccuracyBarInit(intptr_t param) {
	m_accuracyBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_accuracyBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:17256
void CLobbyMain::On_SpinBarInit(intptr_t param) {
	m_spinBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_spinBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:17262
void CLobbyMain::On_CurveBarInit(intptr_t param) {
	m_curveBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_curveBar->SetRange(0, 30, 0);
}

// lobbymain.cpp:17290
bool CLobbyMain::OnMissionEventResult(int, FrForm*) {
	if (!IsLocalContent((localContentType_t)0x83)) {
		return false;
	}
	m_missionEventDlg = 0;
	return true;
}

// lobbymain.cpp:17303
bool CLobbyMain::OnBingoEventResult(int, FrForm*) {
	if (!IsLocalContent((localContentType_t)0x85)) {
		return false;
	}
	m_bingoEventDlg = 0;
	return true;
}

// lobbymain.cpp:17315
bool CLobbyMain::OnKoohBirthdayEventDlg(int, FrForm*) {
	if (!IsLocalContent((localContentType_t)0x8E)) {
		return false;
	}
	m_toppageKoohBirthdayBtn = 0;
	return true;
}

// lobbymain.cpp:17329
void CLobbyMain::JoinOtherServerRoom(void) {
}

// lobbymain.cpp:17405
// Applies up to ten downloaded UCC clothes to the avatars of both slot lists
// and of the game room's user list.
void CLobbyMain::RefreshAllUccClothes(void) {
	if (CUserInfo::Instance()->GetDlg())
		return;

	sUccRefreshItem item;

	for (int i = 0; i < 10; i++) {
		if (!UccManager()->GetRefreshItem(item))
			return;

		std::list<sSlotInfo*>::iterator it;
		for (it = m_slotList2.begin(); it != m_slotList2.end(); ++it)
			_RefreshUccClothes(*it, item.m_typeId, item.m_uccKey);

		for (it = m_slotList1.begin(); it != m_slotList1.end(); ++it)
			_RefreshUccClothes(*it, item.m_typeId, item.m_uccKey);

		if (m_roomUserListBox) {
			for (int j = 0; j < m_roomUserListBox->GetCurrentItemSize(false); j++) {
				FrListItem* listItem = m_roomUserListBox->GetItem(j);
				if (listItem)
					_RefreshUccClothes((sSlotInfo*)listItem->pData, item.m_typeId, item.m_uccKey);
			}
		}
	}
}

// lobbymain.cpp:17445
void CLobbyMain::OnToppage_UCCShopBtnUp(void) {
	CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
	CTaskManager::Instance()->PostMsg(0, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
	CTaskManager::Instance()->PostMsg(0, "Shop", 0xC6, 3, 0, 0, 0);
}

// lobbymain.cpp:17452
void CLobbyMain::OnToppage_CardShopBtnUp(void) {
	CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
	CTaskManager::Instance()->PostMsg(0, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
	CTaskManager::Instance()->PostMsg(0, "Shop", 0xC6, 4, 0, 0, 0);
}

// lobbymain.cpp:17459
float CLobbyMain::GetTopPageBtnRect(void) {
	if (IsLocalContent((localContentType_t)0x6A)) {
		if (CLoginInfo::Instance()->IsPcBang()) {
			return m_topPageBtnIndex++ * 80.0f;
		}
		return m_topPageBtnIndex++ * 80.0f;
	}
	return 0.0f;
}

// lobbymain.cpp:17484
// Decides whether the game result screen offers the use report form.
void CLobbyMain::CheckTikiReport(const char* layout) {
	if (IsLocalContent((localContentType_t)0x14)) {
		if (strcmp(layout, "GAMEROOM_EXTRES") == 0) {
			CSharedDoc* doc = CSharedDoc::Instance();

			if (doc->GetPangyaUserInfo().identity & 0x0E) {
				m_useReportEnabled = false;
			} else {
				std::list<sItemInfo>::iterator it;
				for (it = doc->m_itemInfoList.begin(); it != CSharedDoc::Instance()->m_itemInfoList.end(); ++it) {
					sItemInfo item = *it;

					if (item.m_typeId == 0x1A000041) {
						m_useReportEnabled = true;
						break;
					}

					m_useReportEnabled = false;
				}
			}

			if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 6)
				m_useReportEnabled = false;

			if (CSharedDoc::Instance()->m_roomInfo.m_userLimit > 30)
				m_useReportEnabled = false;

			if (m_resultReportDlg != NULL)
				m_useReportEnabled = false;

			if (CSharedDoc::Instance()->m_currentChannel.flags & 0x800)
				m_useReportEnabled = false;

			if (CSharedDoc::Instance()->m_myLevel <= 5)
				m_useReportEnabled = false;
		}
	}
}

// lobbymain.cpp:18052
void CLobbyMain::OnToppage_TopIcon1Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(0, param);
}

// lobbymain.cpp:18056
void CLobbyMain::OnToppage_TopIcon1Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(0);
}

// lobbymain.cpp:18062
void CLobbyMain::OnToppage_TopIcon2Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(1, param);
}

// lobbymain.cpp:18066
void CLobbyMain::OnToppage_TopIcon2Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(1);
}

// lobbymain.cpp:18072
void CLobbyMain::OnToppage_TopIcon3Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(2, param);
}

// lobbymain.cpp:18076
void CLobbyMain::OnToppage_TopIcon3Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(2);
}

// lobbymain.cpp:18082
void CLobbyMain::OnToppage_TopIcon4Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(3, param);
}

// lobbymain.cpp:18087
void CLobbyMain::OnToppage_TopIcon4Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(3);
}

// lobbymain.cpp:18093
void CLobbyMain::OnToppage_TopIcon5Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(4, param);
}

// lobbymain.cpp:18098
void CLobbyMain::OnToppage_TopIcon5Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(4);
}

// lobbymain.cpp:18104
void CLobbyMain::OnToppage_TopIcon6Init(int param) {
	CIconManager::Instance()->InitalizeTopIcon(5, param);
}

// lobbymain.cpp:18109
void CLobbyMain::OnToppage_TopIcon6Up(void) {
	CloseAllDialogs();
	CIconManager::Instance()->OnButtonDown(5);
}
