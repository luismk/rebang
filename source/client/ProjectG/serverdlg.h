// Original: d:\build\custom\temp\code\client\projectg\serverdlg.h
#ifndef PG_CLIENT_PROJECTG_SERVERDLG_H
#define PG_CLIENT_PROJECTG_SERVERDLG_H

#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/ProjectG/messengerdlg.h"

class Bitmap;
class FrAutoConnect;
class FrButton;
class FrListBox;
class FrLogoutDlg;
class FrStatic;
class WTitleFont;
struct FrListItem;
struct sChannelInfo;

// Login progress of the server dialog (FrServerDlg::m_state; PDB name, values
// not recovered: SetState clears m_bxxx on 4).
enum gsLoginState_t {
	gsLoginState_None = 0
};

// Four-piece progress bar of the server dialog (serverdlg.cpp:81-140): left
// cap, stretched middle, right cap and the fill drawn inside the frame.
// Layout from the code (size 0x74, no vfptr).
class cProgressBar {
	LAYOUT_CHECK_FRIEND;
public:
	struct sProgressImg {
		WRect rect;                 // 0x0
		const Bitmap* pBitmap;      // 0x10
	};

	cProgressBar(void);
	~cProgressBar(void);

	void Initialize(const WRect& rect, float marginX, float marginY);
	void SetImg(const char* left, const char* middle, const char* right, const char* fill);
	void SetRange(int min, int max);
	void SetPos(int pos);
	void Display(void);

private:
	sProgressImg m_img[4];          // 0x0: left, middle, right, fill
	WRect m_rect;                   // 0x50
	int m_min;                      // 0x60
	int m_max;                      // 0x64
	int m_pos;                      // 0x68
	float m_marginX;                // 0x6c
	float m_marginY;                // 0x70
};

CHECK_SIZE(cProgressBar::sProgressImg, 0x14);
CHECK_SIZE(cProgressBar, 0x74);
BEGIN_LAYOUT_CHECK(cProgressBar)
	CHECK_FIELD(cProgressBar, m_img, 0x0);
	CHECK_FIELD(cProgressBar, m_rect, 0x50);
	CHECK_FIELD(cProgressBar, m_min, 0x60);
	CHECK_FIELD(cProgressBar, m_max, 0x64);
	CHECK_FIELD(cProgressBar, m_pos, 0x68);
	CHECK_FIELD(cProgressBar, m_marginX, 0x6c);
	CHECK_FIELD(cProgressBar, m_marginY, 0x70);
END_LAYOUT_CHECK

// Server and channel selection dialog. Size 0x540 is the `new` of
// FrServerDlgMakeInstance; members named from the code that uses them.
class FrServerDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // serverdlg.h:28

	FrServerDlg(void);
	virtual ~FrServerDlg(void);

	void SetState(gsLoginState_t state);
	bool OnResultAutoRecon(int ret, FrForm* pForm);
	void FinishAutoLogin(void);
	void SetLoginMessage(const char* msg);
	void SetLoginMessageAlert(const char* msg);
	virtual void OnCancel(void);
	void BeginAutoLogin(void);
	bool CloseByEnterChannel(bool bDestroy);
	void FailedToConnectServer(void);
	void AddLoginReplyCount(int count);
	void OnNotify(const char* msg, unsigned long flag);
	virtual void OnProc(const float dt);
	virtual bool OnInit(void);
	void ConnectToServer(unsigned long serverId);
	void BuildServerList(void);
	void BuildChannelList(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void ConnectToServer(void);
	bool OnLoginDlgResult(int ret, FrForm* pForm);
	void TurnOnChannels(void);
	void TurnOffChannels(void);
	bool OnNotifyResult(int ret, FrForm* pForm);
	bool OnWaitNotifyResult(int ret, FrForm* pForm);
	bool OnQuitDlgResult(int ret, FrForm* pForm);
	void OpenQuitDlg(void);
	void OnWaitNotify(const char* msg, unsigned long flag);

	void OnServerListInit(intptr_t param);
	void OnServerListOwnerDraw(int param);
	void OnServerListBtnDown(void);
	void OnChannelListInit(intptr_t param);
	void OnChannelListOwnerDraw(intptr_t param);
	void OnChannelListBtnDown(void);
	void OnConnectMessageInit(intptr_t param);
	void OnConnectCancleInit(intptr_t param);
	void OnConnectCancleUp(void);
	void OnMessageInit(intptr_t param);
	void OnCurServerInit(intptr_t param);
	void OnCloseBtnInit(intptr_t param);
	void OnCloseLBtnUp(void);

private:
	void ClearServerList(void);
	bool IsNotDrawServerList(FrListItem* pItem);
	bool IsDisableEnterChannel(unsigned long flags, unsigned char level);
	void DrawServerButton(const WPoint& pos, const sGameServerInfo* pServer, bool bOver);
	void DrawChannelButton(const WPoint& pos, const sChannelInfo* pChannel, bool bOver);

	int m_bConnectById;                   // 0x110 (ConnectToServer(serverId) sets it)
	FrListBox* m_pServerList;             // 0x114 ("serverlist")
	FrListBox* m_pChannelList;            // 0x118 ("channellist")
	FrStatic* m_pMessage;                 // 0x11c ("message")
	FrStatic* m_pConnectMessage;          // 0x120 ("connectmessage")
	FrStatic* m_pCurServer;               // 0x124 ("curserver")
	FrButton* m_pCancel;                  // 0x128 ("cancel")
	FrForm* m_pLoginDlg;                  // 0x12c (OnLoginDlgResult clears it)
	gsLoginState_t m_state;               // 0x130
	float m_loginWaitTime;                // 0x134 (OnProc accumulates dt; AddLoginReplyCount resets it)
	float m_connectDelay;                 // 0x138 (OnProc counts it down, then ConnectToServer)
	bool m_bUnused;                       // 0x13c (only the constructor clears it)
	bool m_bReturnToLobby;                // 0x13d (CloseByEnterChannel: send the lobby task to the room list)
	bool m_bAlert;                        // 0x13e (SetLoginMessageAlert sets it, SetLoginMessage clears it)
	bool m_bAutoLoginEnabled;             // 0x13f (SetState clears it on state 4)
	float m_alertBlinkTime;               // 0x140 (OnProc blinks the alert message every 0.5 s)
	sGameServerInfo m_selectedServer;     // 0x144 (ConnectToServer copies it into CSharedDoc)
	const Bitmap* m_pServerIcon[4];       // 0x1a0 (ITEMS: icon_expevent, icon_pangevent, icon_angelwing, icon_community)
	const Bitmap* m_pConnectionIcon;      // 0x1b0 (ITEMS: icon_connection)
	const Bitmap* m_pNewMark;             // 0x1b4 (ITEMS: mark_new)
	tagWTITLEFONT m_titleFontInfo;        // 0x1b8 (the constructor creates m_pTitleFont from it)
	WTitleFont* m_pTitleFont;             // 0x1d8 (released through g_resrcmng by the destructor)
	FrForm* m_pNotifyDlg;                 // 0x1dc (OnNotifyResult clears it)
	FrForm* m_pWaitNotifyDlg;             // 0x1e0 (OnWaitNotifyResult clears it)
	FrLogoutDlg* m_pQuitDlg;              // 0x1e4 (OnQuitDlgResult clears it)
	cProgressBar* m_pProgressBar;         // 0x1e8 (OnConnectCancleUp resets it)
	FrAutoConnect* m_pAutoConnect;        // 0x1ec (OnResultAutoRecon clears it)
	int m_loginReplyCount;                // 0x1f0 (AddLoginReplyCount: 0..21, 21 = done)
	FrButton* m_pConnectCancle;           // 0x1f4 ("connect_cancle")
	sGameServerInfo m_serverList[9];      // 0x1f8 (ClearServerList zeroes it)
	std::map<unsigned int, std::string> m_textMap;   // 0x534 (cleared by the destructor)
};
BEGIN_LAYOUT_CHECK(FrServerDlg)
	CHECK_FIELD(FrServerDlg, m_bConnectById, 0x110);
	CHECK_FIELD(FrServerDlg, m_pServerList, 0x114);
	CHECK_FIELD(FrServerDlg, m_pChannelList, 0x118);
	CHECK_FIELD(FrServerDlg, m_pMessage, 0x11c);
	CHECK_FIELD(FrServerDlg, m_pConnectMessage, 0x120);
	CHECK_FIELD(FrServerDlg, m_pCurServer, 0x124);
	CHECK_FIELD(FrServerDlg, m_pCancel, 0x128);
	CHECK_FIELD(FrServerDlg, m_pLoginDlg, 0x12c);
	CHECK_FIELD(FrServerDlg, m_state, 0x130);
	CHECK_FIELD(FrServerDlg, m_loginWaitTime, 0x134);
	CHECK_FIELD(FrServerDlg, m_connectDelay, 0x138);
	CHECK_FIELD(FrServerDlg, m_bReturnToLobby, 0x13d);
	CHECK_FIELD(FrServerDlg, m_bAlert, 0x13e);
	CHECK_FIELD(FrServerDlg, m_bAutoLoginEnabled, 0x13f);
	CHECK_FIELD(FrServerDlg, m_alertBlinkTime, 0x140);
	CHECK_FIELD(FrServerDlg, m_selectedServer, 0x144);
	CHECK_FIELD(FrServerDlg, m_pServerIcon, 0x1a0);
	CHECK_FIELD(FrServerDlg, m_pConnectionIcon, 0x1b0);
	CHECK_FIELD(FrServerDlg, m_pNewMark, 0x1b4);
	CHECK_FIELD(FrServerDlg, m_bUnused, 0x13c);
	CHECK_FIELD(FrServerDlg, m_titleFontInfo, 0x1b8);
	CHECK_FIELD(FrServerDlg, m_pTitleFont, 0x1d8);
	CHECK_FIELD(FrServerDlg, m_pNotifyDlg, 0x1dc);
	CHECK_FIELD(FrServerDlg, m_pWaitNotifyDlg, 0x1e0);
	CHECK_FIELD(FrServerDlg, m_pQuitDlg, 0x1e4);
	CHECK_FIELD(FrServerDlg, m_pProgressBar, 0x1e8);
	CHECK_FIELD(FrServerDlg, m_pAutoConnect, 0x1ec);
	CHECK_FIELD(FrServerDlg, m_loginReplyCount, 0x1f0);
	CHECK_FIELD(FrServerDlg, m_pConnectCancle, 0x1f4);
	CHECK_FIELD(FrServerDlg, m_serverList, 0x1f8);
	CHECK_FIELD(FrServerDlg, m_textMap, 0x534);
END_LAYOUT_CHECK
CHECK_SIZE(FrServerDlg, 0x540);

#endif // PG_CLIENT_PROJECTG_SERVERDLG_H
