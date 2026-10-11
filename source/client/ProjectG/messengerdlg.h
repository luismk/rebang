// Original: d:\build\custom\temp\code\client\projectg\messengerdlg.h
#ifndef PG_CLIENT_PROJECTG_MESSENGERDLG_H
#define PG_CLIENT_PROJECTG_MESSENGERDLG_H

#include <list>
#include <map>
#include <string>
#include <string.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "frbutton.h"
#include "frform.h"
#include "frarea.h"
#include "frlistbox.h"
#include "frcontextmenuctrl.h"
#include "../client/Wangreal/include/wtypes.h"
#include "buddymanager.h"

class FrAddFriendDlg;
class FrGuildNoticeDlg;
class FrMemoDlg;
class FrNoteDlg;
class FrMessengerChatDlg;
class FrMessengerDlg;
class NewsReader;
struct sUserInfoTime;

// One line of a messenger chat (the history a chat slot keeps and replays).
// messengerdlg.h:29-37; the copy constructor, operator= and destructor are
// implicit.
struct sMsnChat {
	std::string nick;                    // 0x00
	std::string text;                    // 0x1C
	int reserved;                        // 0x38 (only zeroed by the constructor so far)

	// messengerdlg.h:31
	sMsnChat(void) {
		nick = "";
		text = "";

		reserved = 0;
	}
};
CHECK_SIZE(sMsnChat, 0x3C);
BEGIN_LAYOUT_CHECK(sMsnChat)
	CHECK_FIELD(sMsnChat, nick, 0x00);
	CHECK_FIELD(sMsnChat, text, 0x1C);
	CHECK_FIELD(sMsnChat, reserved, 0x38);
END_LAYOUT_CHECK

// One buddy's chat (CMessengerInfo::m_chatDlgList): its window when open, the
// history replayed when it reopens, its saved position and the unsent input.
// messengerdlg.h:63-71; the copy constructor and destructor are implicit.
struct sChatSlot {
	FrMessengerChatDlg* pChatDlg;        // 0x00, 0 while the window is closed
	unsigned long uid;                   // 0x04, the buddy's uid
	bool bReceived;                      // 0x08, created by a received chat (RecvChat); a non-destroying Close keeps it
	std::list<sMsnChat> chatList;        // 0x0C
	WRect rcChatDlg;                     // 0x18, the window's rect saved by CloseWindow
	std::string tempChat;                // 0x28, the unsent input line (SaveTempChat)

	// messengerdlg.h:63
	sChatSlot(void) {
		pChatDlg = 0; uid = 0; bReceived = false; tempChat = "";
		memset(&rcChatDlg, 0, sizeof(rcChatDlg));
		chatList.clear();
	}
};
CHECK_SIZE(sChatSlot, 0x44);
BEGIN_LAYOUT_CHECK(sChatSlot)
	CHECK_FIELD(sChatSlot, pChatDlg, 0x00);
	CHECK_FIELD(sChatSlot, uid, 0x04);
	CHECK_FIELD(sChatSlot, bReceived, 0x08);
	CHECK_FIELD(sChatSlot, chatList, 0x0C);
	CHECK_FIELD(sChatSlot, rcChatDlg, 0x18);
	CHECK_FIELD(sChatSlot, tempChat, 0x28);
END_LAYOUT_CHECK

namespace MSN {
	// Messenger presence. 3 and 4 are the own-status menu entries ("busy",
	// "online"); 0-2 and 5 are what FrMessengerDlg::GetBuddyStatus derives
	// from a buddy's flags (0x20/0x40 with 0x02, 0x10, none).
	enum eStatusType {
		STATUS_ACTIVE = 0,               // flags 0x20 and 0x02
		STATUS_PLAYING = 1,              // flags 0x40 and 0x02
		STATUS_AWAY = 2,                 // flag 0x10
		STATUS_BUSY = 3,                 // flag 0x80, own menu entry
		STATUS_ONLINE = 4,               // own menu entry
		STATUS_OFFLINE = 5
	};
}

// Progress of the messenger server login, CMessengerInfo::m_serverListRequest
// (CTask::OnPacketCommon 0x2D: 2 once the login is accepted, 3 once the
// position and the 0x14 request are sent; 4 is tested as "logged in").
enum eConnectStatus {
	CONNECT_STATUS_ACCEPTED = 2,
	CONNECT_STATUS_POSITION_SENT = 3,
	CONNECT_STATUS_COMPLETE = 4
};

// Messenger (buddy list) dialog; its size (0x380) is the `new` of
// FrMessengerDlgMakeInstance. Only the members this project's code uses so
// far are declared.
class FrMessengerDlg : public FrForm {
	friend class CMessengerInfo;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // messengerdlg.h:105

	FrMessengerDlg(void);

	virtual bool Close(bool bDestroy);
	void SetAddFriendNick(const char* nick, unsigned long uid);
	void SetTargetNick(const char* nick);
	void OpenAddFriendNickDlg(void);
	void UpdateFriendList(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnLButtonDown(const WPoint& pt);
	void RestoreOther(unsigned long);
	void OnMessenger_FriendLBtnDown(void);
	void OnMessenger_MyInfoInit(int param);
	void OnMessenger_StateBtnInit(int param);
	bool OnBlockFriendDlgResult(int ret, FrForm* pForm);
	bool OnBlockCancelFriendDlgResult(int ret, FrForm* pForm);
	void OnMessenger_FriendListDBtnDown(void);
	void OnMessenger_GuildListDBtnDown(void);
	void OnMessenger_StateMenuLBtnDown(void);
	void OnInfoFriend(void);
	void OnAcceptFriend(void);
	void OnNoteToFriend(void);
	void OnMessenger_GuildLBtnDown(void);
	void OnChatToFriend(void);
	bool OnGuildNoticeDlgResult(int ret, FrForm* pForm);
	MSN::eStatusType GetBuddyStatus(sFriend* pFriend);
	void SetCount(void);

	void OnMessenger_StateLBtnDown(void);
	bool OnAddFriendDlgResult(int ret, FrForm* pForm);
	bool OnOptionDlgResult(int ret, FrForm* pForm);
	void OnMessenger_FindLBtnDown(void);
	void OnMessenger_CloseLBtnDown(void);
	bool OnRemoveFriendDlgResult(int ret, FrForm* pForm);
	void OnRemoveFriend(void);
	void OnBlockFriend(void);
	void OnBlockCancelFriend(void);
	void OnMessenger_FriendBtnInit(int param);
	bool OnMemoDlgResult(int ret, FrForm* pForm);
	void OnMessenger_GuildBtnInit(int param);
	void OnMessenger_RightMenuInit(int param);
	void OnMessenger_StateMenuInit(int param);
	void OnMessenger_GuildNoticeBtnInit(int param);
	void OnAliasToFriend(void);
	bool OnNoteDlgResult(int ret, FrForm* pForm);
	void OnMessenger_FriendListLBtnDown(void);
	void OnMessenger_GuildListLBtnDown(void);
	void OnInviteFriendRealMyRoom(bool bForce);
	void OnMessenger_FriendListInit(int param);
	void OnMessenger_GuildListInit(int param);
	void OnMessenger_GuildNoticeLBtnDown(void);
	void OnMessenger_GuildListRBtnDown(void);

	FrListBox* m_pFriendList;            // 0x110
	FrListBox* m_pGuildList;             // 0x114
	FrContextMenuCtrl* m_pRightMenu;     // 0x118
	FrContextMenuCtrl* m_pStateMenu;     // 0x11C
	const Bitmap* m_pStatusIcon[5];      // 0x120: play, dive, block, busy icons; the fifth is 0
	const Bitmap* m_pSexIcon[6];         // 0x134: male, female, then the _02 and _03 pairs
	const Bitmap* m_pDownBtn;            // 0x14C, "down_btn_n" (OnMessenger_FriendListInit)
	const Bitmap* m_pUpBtn;              // 0x150, "up_btn_n"
	unsigned long m_selectUID;           // 0x154, uid of the double-clicked buddy
	FrButton* m_pTabBtn[2];              // 0x158 (friend, guild)
	FrButton* m_pGuildNoticeBtn;         // 0x160 (OnMessenger_GuildNoticeBtnInit)
	FrButton* m_pStateBtn;               // 0x164 (OnMessenger_StateBtnInit), friend tab only
	int m_reserved168;                   // 0x168, only zeroed by the constructor so far
	FrArea* m_pMyInfo;                   // 0x16C (OnMessenger_MyInfoInit)
	int m_list0Count;                    // 0x170, buddies with sFriend::listFlags bit 0 (SetCount)
	int m_list0OnCount;                  // 0x174, of those, logged on with flags bit 2
	int m_list1Count;                    // 0x178, buddies with sFriend::listFlags bit 1
	int m_list1OnCount;                  // 0x17C, of those, logged on
	FrNoteDlg* m_pNoteDlg;               // 0x180 (OnNoteDlgResult)
	FrAddFriendDlg* m_pAddFriendDlg;     // 0x184
	FrForm* m_pOptionDlg;                // 0x188
	PADDING(0x18C, 0x190);
	FrMemoDlg* m_pMemoDlg;               // 0x190, the memo sent with a friend request (OnMemoDlgResult)
	sFriend m_groupRow[3];               // 0x194: the list's header rows, logged on (0), logged off (1) and a third (2); status holds the fold state
	std::list<sFriend*> m_onlineFriends;     // 0x33C, UpdateFriendList: listFlags bit 0, logged on (flags 2 and 4)
	std::list<sFriend*> m_offlineFriends;    // 0x348, UpdateFriendList: the other listFlags bit 0 buddies
	std::list<sFriend*> m_sortList0;         // 0x354, cleared by UpdateFriendList; role unconfirmed
	std::list<sFriend*> m_sortList1;         // 0x360, cleared by UpdateFriendList; role unconfirmed
	std::list<sFriend*> m_sortList2;         // 0x36C, cleared by UpdateFriendList; role unconfirmed
	bool m_inviteEnabled;                // 0x378, copied from CMessengerInfo by Toggle; without it the right menu disables "invite" (IsLocalContent(0x54))
	PADDING(0x379, 0x380);

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(FrMessengerDlg, 0x380);
BEGIN_LAYOUT_CHECK(FrMessengerDlg)
	CHECK_FIELD(FrMessengerDlg, m_pFriendList, 0x110);
	CHECK_FIELD(FrMessengerDlg, m_pGuildList, 0x114);
	CHECK_FIELD(FrMessengerDlg, m_pRightMenu, 0x118);
	CHECK_FIELD(FrMessengerDlg, m_pStateMenu, 0x11C);
	CHECK_FIELD(FrMessengerDlg, m_pStatusIcon, 0x120);
	CHECK_FIELD(FrMessengerDlg, m_pSexIcon, 0x134);
	CHECK_FIELD(FrMessengerDlg, m_pDownBtn, 0x14C);
	CHECK_FIELD(FrMessengerDlg, m_pUpBtn, 0x150);
	CHECK_FIELD(FrMessengerDlg, m_selectUID, 0x154);
	CHECK_FIELD(FrMessengerDlg, m_pTabBtn, 0x158);
	CHECK_FIELD(FrMessengerDlg, m_pGuildNoticeBtn, 0x160);
	CHECK_FIELD(FrMessengerDlg, m_pStateBtn, 0x164);
	CHECK_FIELD(FrMessengerDlg, m_pMyInfo, 0x16C);
	CHECK_FIELD(FrMessengerDlg, m_list0Count, 0x170);
	CHECK_FIELD(FrMessengerDlg, m_list0OnCount, 0x174);
	CHECK_FIELD(FrMessengerDlg, m_list1Count, 0x178);
	CHECK_FIELD(FrMessengerDlg, m_list1OnCount, 0x17C);
	CHECK_FIELD(FrMessengerDlg, m_pAddFriendDlg, 0x184);
	CHECK_FIELD(FrMessengerDlg, m_pOptionDlg, 0x188);
	CHECK_FIELD(FrMessengerDlg, m_pMemoDlg, 0x190);
	CHECK_FIELD(FrMessengerDlg, m_pNoteDlg, 0x180);
	CHECK_FIELD(FrMessengerDlg, m_groupRow, 0x194);
	CHECK_FIELD(FrMessengerDlg, m_reserved168, 0x168);
	CHECK_FIELD(FrMessengerDlg, m_onlineFriends, 0x33C);
	CHECK_FIELD(FrMessengerDlg, m_offlineFriends, 0x348);
	CHECK_FIELD(FrMessengerDlg, m_inviteEnabled, 0x378);
END_LAYOUT_CHECK

#pragma pack(push, 1)

// One server of a server list (0x5C bytes: list<sGameServerInfo> nodes are
// 0x64). Its original header is not known yet; it is declared here, next to
// its only user so far.
struct sGameServerInfo {
	// Inline in the original (ServerDlg constructs its members with it).
	sGameServerInfo(void) {
		name[0] = 0;
		m_address[0] = 0;
	}

	char name[0x28];                     // 0x00
	unsigned long id;                    // 0x28
	int m_maxUsers;                      // 0x2C
	int m_curUsers;                      // 0x30
	char m_address[0x12];                // 0x34
	unsigned long m_port;                // 0x46
	unsigned long flags;                 // 0x4A
	int m_badgeCount;                    // 0x4E (ServerDlg draws it as a number when > 0)
	unsigned long m_flags2;              // 0x52 (ServerListCompare tests bit 0x40 of its low byte)
	UNKNOWN(0x56, 0x5A);
	unsigned short m_iconKey;            // 0x5A (key of FrServerDlg::m_textMap: the name of the server's icon)
};
CHECK_SIZE(sGameServerInfo, 0x5C);
BEGIN_LAYOUT_CHECK(sGameServerInfo)
	CHECK_FIELD(sGameServerInfo, name, 0x00);
	CHECK_FIELD(sGameServerInfo, id, 0x28);
	CHECK_FIELD(sGameServerInfo, flags, 0x4A);
	CHECK_FIELD(sGameServerInfo, m_maxUsers, 0x2C);
	CHECK_FIELD(sGameServerInfo, m_curUsers, 0x30);
	CHECK_FIELD(sGameServerInfo, m_address, 0x34);
	CHECK_FIELD(sGameServerInfo, m_port, 0x46);
	CHECK_FIELD(sGameServerInfo, m_badgeCount, 0x4E);
	CHECK_FIELD(sGameServerInfo, m_flags2, 0x52);
	CHECK_FIELD(sGameServerInfo, m_iconKey, 0x5A);
END_LAYOUT_CHECK

// One messenger server of the list the login server sends (0x5D bytes: the
// list's _Buynode copies 0x17 dwords and a byte into a 0x68 node).
struct MSNServerInfo : public sGameServerInfo {
	bool m_tried;                        // 0x5C, GetConnectServerInfo skips it
};
CHECK_SIZE(MSNServerInfo, 0x5D);
BEGIN_LAYOUT_CHECK(MSNServerInfo)
	CHECK_FIELD(MSNServerInfo, m_tried, 0x5C);
END_LAYOUT_CHECK

#pragma pack(pop)

// Messenger (buddy chat) state. Only the members this project's code uses so
// far are declared.
class CMessengerInfo : public WSingleton<CMessengerInfo>, public FrCmdTarget {
	friend class FrMessengerDlg;
	friend class FrMessengerChatDlg;
	friend class CMouseCursor;    // Display hit-tests m_rcMessengerDlg
	friend class CTaskMain;       // HandleMsgCommon places the bubble by m_rcAlarmBtn
	friend class FrServerDlg;     // ConnectToServer clears m_waitingForServerList

public:
	CMessengerInfo(void);

	// messengerdlg.h:306
	FrMessengerDlg* GetMessengerDlg(void) {
		return m_pMessengerDlg;
	}
	void AlarmBtnUp(unsigned long);
	bool IsGameFocus(void);              // messengerdlg.h:369
	void SetGameFocus(bool focus);
	bool IsVisible(void);
	void Refresh(void);
	void Open(bool);
	void Notify1(const char* msg);
	void Notify2(const char* msg);
	void Notify4(bool);
	void Toggle(void);
	void InvalidateCurrentServer(void);
	bool Close(bool bDestroy);
	void Process(float dt);
	void SetOfflineUserNick(const char* nick, unsigned long uid);
	void SetTargetNick(const char* nick);
	void SetMyStatus(MSN::eStatusType status);
	void CloseWindow(void);
	void RepositionPopupDlg(FrMessengerChatDlg* pDlg, int index);

	// messengerdlg.h:367
	MSN::eStatusType GetMyStatus(void) { return m_myStatus; }

	// messengerdlg.h:371
	void SetComplete(eConnectStatus status) { m_serverListRequest = status; }

	// messengerdlg.h:375
	bool IsWaitingForServerList(void) { return m_waitingForServerList; }
	void SetServerListWaitingState(bool waiting) { m_waitingForServerList = waiting; }   // messengerdlg.h:376

	// messengerdlg.h:379
	void SetCurrentServerIndex(int index) { m_currentServerIndex = index; }

protected:
	void SaveCurrentFocus(void);
	void ApplySavedFocus(void);
	bool OnMessengerDlgResult(int ret, FrForm* pForm);
	bool OnMessengerChatDlgResult(int ret, FrForm* pForm);

	std::list<sChatSlot> m_chatDlgList;   // 0x2C

	WRect m_rcMessengerDlg;              // 0x38
	WRect m_rcAlarmBtn;                  // 0x48, (663, 0) 38x32; only the constructor so far (named after its button size)
	std::map<unsigned long, sUserInfoTime> m_userInfoMap;   // 0x58

public:
	std::list<MSNServerInfo> m_serverList;   // 0x64, cleared by MSNUnit::ShutDown
	FrMessengerDlg* m_pMessengerDlg;     // 0x70
	float m_notifyElapsed;               // 0x74, reset by Notify2, advanced by Process while m_notifyActive
	float m_reconnectElapsed;            // 0x78, Process reconnects the messenger after 60 s
	bool m_notifyActive;                 // 0x7C, set by Notify2, cleared by Process
	bool m_gameFocus;                    // 0x7D
	bool m_autoAway;                     // 0x7E, set by Process after 120 s without mouse or IME input
	int m_serverListRequest;             // 0x80, reset by MSNUnit::Init
	bool m_waitingForServerList;         // 0x84
	int m_currentServerIndex;            // 0x88
	MSN::eStatusType m_myStatus;         // 0x8C
	int m_popupCount;                    // 0x90, counted by RepositionPopupDlg
	bool m_groupState[3];                // 0x94, fold state of the header rows: logged on (0), logged off (1), the third row (2); toggled by a click on a row
	PADDING(0x97, 0x98);
	std::list<unsigned long> m_focusOrder;   // 0x98, uids of the windows to refocus (-1: the messenger), ApplySavedFocus
	NewsReader* m_pGuildNews;            // 0xA4, deleted when the guild notice closes
	FrGuildNoticeDlg* m_pGuildNoticeDlg; // 0xA8
	PADDING(0xAC, 0xB4);
	bool m_inviteEnabled;                // 0xB4, cleared when IsLocalContent(0x54); Toggle copies it to FrMessengerDlg::m_inviteEnabled

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CMessengerInfo)
	CHECK_FIELD(CMessengerInfo, m_chatDlgList, 0x2C);
	CHECK_FIELD(CMessengerInfo, m_rcMessengerDlg, 0x38);
	CHECK_FIELD(CMessengerInfo, m_pMessengerDlg, 0x70);
	CHECK_FIELD(CMessengerInfo, m_gameFocus, 0x7D);
	CHECK_FIELD(CMessengerInfo, m_serverList, 0x64);
	CHECK_FIELD(CMessengerInfo, m_serverListRequest, 0x80);
	CHECK_FIELD(CMessengerInfo, m_waitingForServerList, 0x84);
	CHECK_FIELD(CMessengerInfo, m_currentServerIndex, 0x88);
	CHECK_FIELD(CMessengerInfo, m_myStatus, 0x8C);
	CHECK_FIELD(CMessengerInfo, m_rcAlarmBtn, 0x48);
	CHECK_FIELD(CMessengerInfo, m_userInfoMap, 0x58);
	CHECK_FIELD(CMessengerInfo, m_notifyElapsed, 0x74);
	CHECK_FIELD(CMessengerInfo, m_reconnectElapsed, 0x78);
	CHECK_FIELD(CMessengerInfo, m_notifyActive, 0x7C);
	CHECK_FIELD(CMessengerInfo, m_autoAway, 0x7E);
	CHECK_FIELD(CMessengerInfo, m_popupCount, 0x90);
	CHECK_FIELD(CMessengerInfo, m_groupState, 0x94);
	CHECK_FIELD(CMessengerInfo, m_inviteEnabled, 0xB4);
	CHECK_FIELD(CMessengerInfo, m_focusOrder, 0x98);
	CHECK_FIELD(CMessengerInfo, m_pGuildNews, 0xA4);
	CHECK_FIELD(CMessengerInfo, m_pGuildNoticeDlg, 0xA8);
END_LAYOUT_CHECK

CMessengerInfo *MESSENGER(void);     // messengerdlg.h:435

#endif // PG_CLIENT_PROJECTG_MESSENGERDLG_H
