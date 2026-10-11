// Original: d:\build\custom\temp\code\client\projectg\serverdlg.cpp
#include "../client/ProjectG/NetworkSystem.h"
#include "../client/ProjectG/Packet.h"
#include "../client/ProjectG/logininfo.h"
#include "../client/ProjectG/SoundManager.h"
#include "../client/ProjectG/PangGiftDlg.h"
#include "../client/ProjectG/commonutil.h"
#include "../client/ProjectG/binstr.h"
#include "../client/ProjectG/ServerDlg.h"

#include "../client/ProjectG/SharedDoc.h"
#include "../client/ProjectG/LogoutDlg.h"
#include "../client/ProjectG/LobbyTask.h"
#include "../client/ProjectG/TaskManager.h"
#include "../client/Fresh/include/fresh.h"
#include "../client/Fresh/include/frbutton.h"
#include "../client/Fresh/include/frform.h"
#include "../client/Fresh/include/frdesktop.h"
#include "../client/Fresh/include/frlistbox.h"
#include "../client/Fresh/include/frstatic.h"
#include "../client/Fresh/include/frscrollbar.h"
#include "../client/Fresh/include/frgraphicinterface.h"
#include "../client/Fresh/include/frwndmanager.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/ProjectG/actor.h"
#include "../client/ProjectG/s5/SkinnedTheme.h"
#include "../client/ProjectG/s5/Utilities.h"

// encryption.h:61 (the original's file-local copy of encryption.h's
// XorWithKey; including encryption.h itself shifts this unit's static
// initializer numbering).
static unsigned int XorWithKey(char *data, unsigned int size) {
	const char *key = "{782AE110-2EEF-4c61-B030-A53F17634F7D}";
	unsigned int i = 0;
	unsigned int j;

	if (size < 0x26) {
		for (j = 0; j < 0x26; j++) {
			data[i++] ^= key[j];
			if (i >= size)
				i = 0;
		}
		return 0x26;
	}

	for (j = 0; j < size; j++) {
		data[j] ^= key[i++];
		if (i >= 0x26)
			i = 0;
	}
	return size;
}

// The server the player is connected to: CSharedDoc keeps it as an
// sGameServerInfo from m_serverName on (ConnectToServer copies it there).
static sGameServerInfo* ConnectedServer(void) {
	return (sGameServerInfo*)WSingleton<CSharedDoc>::Instance()->m_serverName;
}

// serverdlg.cpp:81
cProgressBar::cProgressBar(void)
	: m_min(0), m_max(0), m_pos(0), m_marginX(0), m_marginY(0) {
}

// serverdlg.cpp:82
cProgressBar::~cProgressBar(void) {
}

// serverdlg.cpp:84
void cProgressBar::Initialize(const WRect& rect, float marginX, float marginY) {
	m_rect = rect;
	m_marginX = marginX;
	m_marginY = marginY;
}

// serverdlg.cpp:90
void cProgressBar::SetImg(const char* left, const char* middle, const char* right, const char* fill) {
	m_img[0].pBitmap = g_pFresh->GetBitmap(left);
	m_img[1].pBitmap = g_pFresh->GetBitmap(middle);
	m_img[2].pBitmap = g_pFresh->GetBitmap(right);
	m_img[3].pBitmap = g_pFresh->GetBitmap(fill);

	for (int i = 0; i < 4; i++) {
		if (m_img[i].pBitmap)
			m_img[i].rect = WRect(m_rect.x, m_rect.y, m_img[i].pBitmap->Width(), m_img[i].pBitmap->Height());
	}

	m_img[2].rect.x = m_rect.x + m_rect.w - m_img[2].rect.w;
	m_img[1].rect.x = m_rect.x + m_img[0].rect.w;
	m_img[1].rect.w = m_rect.w - m_img[0].rect.w - m_img[2].rect.w;

	m_img[3].rect.x = m_rect.x + m_marginX;
	m_img[3].rect.y = m_rect.y + m_marginY;
	m_img[3].rect.w = m_rect.w - m_marginX * 2;
	m_img[3].rect.h = m_img[1].rect.h - m_marginY * 2;
}

// serverdlg.cpp:114
void cProgressBar::SetRange(int min, int max) {
	m_min = min;
	m_max = max;
}

// serverdlg.cpp:120
void cProgressBar::SetPos(int pos) {
	if (pos < m_min) m_pos = m_min; else m_pos = pos;
	if (pos > m_max) m_pos = m_max; else m_pos = pos;
}

// serverdlg.cpp:125
void cProgressBar::Display(void) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi) {
		int range = m_max - m_min;
		if (range) {
			WRect rect = m_img[3].rect;
			rect.w = rect.w * m_pos / range;
			if (m_img[3].pBitmap)
				gdi->DrawTexture(m_img[3].pBitmap, rect, 0xffffffff, 0);
		}
		if (m_img[0].pBitmap) gdi->DrawTexture(m_img[0].pBitmap, m_img[0].rect, 0xffffffff, 0);
		if (m_img[1].pBitmap) gdi->DrawTexture(m_img[1].pBitmap, m_img[1].rect, 0xffffffff, 0);
		if (m_img[2].pBitmap) gdi->DrawTexture(m_img[2].pBitmap, m_img[2].rect, 0xffffffff, 0);
	}
}

// Automatic reconnection form: a progress bar fills while the client waits,
// then retries the login after a random delay. Declared in serverdlg.cpp
// (its GetRTTI is serverdlg.cpp:146). Size 0x128 is the `new` of
// FrAutoConnectMakeInstance.
class FrAutoConnect : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // serverdlg.cpp:146

	FrAutoConnect(void);
	virtual ~FrAutoConnect(void);

	void Retry(void);
	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDrawArea(void);
	void OnInitStaticMsg(intptr_t param);
	void OnInitBt1(intptr_t param);
	void OnInitBt2(intptr_t param);

private:
	cProgressBar* m_pProgressBar;         // 0x110 (deleted by the destructor)
	FrStatic* m_pStaticMsg;               // 0x114 ("proc_msg")
	int m_state;                          // 0x118 (1 waiting, 0 retry scheduled, 2 sent; Retry sets 0)
	float m_elapsed;                      // 0x11c (OnProc accumulates dt)
	float m_retryTime;                    // 0x120 (OnProc: elapsed time of the next attempt)
	UNKNOWN(0x124, 0x128);
};
BEGIN_LAYOUT_CHECK(FrAutoConnect)
	CHECK_FIELD(FrAutoConnect, m_pProgressBar, 0x110);
	CHECK_FIELD(FrAutoConnect, m_pStaticMsg, 0x114);
	CHECK_FIELD(FrAutoConnect, m_state, 0x118);
	CHECK_FIELD(FrAutoConnect, m_elapsed, 0x11c);
	CHECK_FIELD(FrAutoConnect, m_retryTime, 0x120);
END_LAYOUT_CHECK
CHECK_SIZE(FrAutoConnect, 0x128);

// serverdlg.cpp:157
FrAutoConnect::FrAutoConnect(void)
	: m_pProgressBar(0) {
	m_pProgressBar = new cProgressBar;
	m_retryTime = 0;
	m_elapsed = 0;
	m_state = 1;
}

// serverdlg.cpp:162
FrAutoConnect::~FrAutoConnect(void) {
	if (m_pProgressBar) {
		delete m_pProgressBar;
		m_pProgressBar = 0;
	}
}

// serverdlg.cpp:167
void FrAutoConnect::Retry(void) {
	m_state = 0;
}

// serverdlg.cpp:172
bool FrAutoConnect::OnInit(void) {
	FrForm::OnInit();

	m_pProgressBar->Initialize(WRect(m_rect.x + 30, m_rect.y + 60, m_rect.w - 60, 30), 3, 3);
	m_pProgressBar->SetImg("gauge_left_frm", "gauge_middle_frm", "gauge_right_frm", "gauge_bar_frm");
	m_pProgressBar->SetRange(0, 1000);

	return true;
}

// serverdlg.cpp:183
void FrAutoConnect::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_pStaticMsg)
		m_pStaticMsg->SetCaption(MakeStr("������ �����ϰ� �ֽ��ϴ�. %c", 0x20));

	if (m_state == 2)
		return;

	m_elapsed += dt;
	if (m_elapsed > 10.0f) {
		Close((eFormRet)1, true);

		FrWnd* pForm = g_pFresh->GetManager()->GetDesktop()->FindChildForm("server");
		if (pForm && pForm->IsKindOf(&FrServerDlg::m_RTTI)) {
			FrServerDlg* pServerDlg = (FrServerDlg*)pForm;
			pServerDlg->OnNotify("������ ������ �� �����ϴ�.", 2);
			pServerDlg->FailedToConnectServer();
		}
		return;
	}

	m_pProgressBar->SetPos((int)(m_elapsed * 100.0f));

	if (m_state == 1) {
		if (m_elapsed > 1.0f) {
			m_elapsed = 1.0f;
			m_state = 0;
			m_retryTime = (float)(rand() % 1000 + 100) * 0.001f;
		}
		return;
	}

	if (m_elapsed > m_retryTime) {
		m_state = 2;
		m_retryTime = (float)(rand() % 1000 + 100) * 0.001f + m_elapsed;

		std::string id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
		std::string server = WSingleton<CSharedDoc>::Instance()->m_loginServerAddress;

		unsigned long key = 0x77DEF61B;
		XorWithKey((char *)&key, sizeof(key));

		WSendPacket send((enumClientPacket)2);
		send.EncodeStr(id);
		send.Encode4(WSingleton<CSharedDoc>::Instance()->m_myUID);
		send.Encode4(CLoginInfo::Instance()->AuthUid());
		send.Encode2(0x6696);
		send.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetLoginAuthKey());
		send.EncodeStr(std::string("645.400"));
		send.Encode4(key);
		send.Encode4(CLoginInfo::Instance()->IsPcBang());
		send.Send(SEND_TO_DEFAULT);
	}
}

// serverdlg.cpp:275
void FrAutoConnect::OnDrawArea(void) {
	if (!g_pFresh->GetManager()->GetGDI())
		return;


	m_pProgressBar->Display();
}

// serverdlg.cpp:283
void FrAutoConnect::OnInitStaticMsg(intptr_t param) {
	m_pStaticMsg = DYNAMIC_CAST(FrStatic, param);
}

// serverdlg.cpp:287
void FrAutoConnect::OnInitBt1(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	pButton->SetVisible(false);
}

// serverdlg.cpp:292
void FrAutoConnect::OnInitBt2(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	pButton->SetVisible(false);
}

// serverdlg.cpp:298 (run-time type and factory registration of FrAutoConnect)
const WRTTI FrAutoConnect::m_RTTI("FrAutoConnect", &FrForm::m_RTTI);

IObject* FrAutoConnectMakeInstance(void) {
	return new FrAutoConnect;
}

struct __sFrAutoConnect {
	__sFrAutoConnect(void) {
		ObjectFactory().AddObjectFunctor(FrAutoConnectMakeInstance, "FrAutoConnect");
	}
};
static __sFrAutoConnect s_registerFrAutoConnect;

// serverdlg.cpp:299
const sFRESH_MSGMAP* FrAutoConnect::GetMessageMap(void) const {
	return &FrAutoConnect::_MsgMap;
}

sFRESH_MSGMAP FrAutoConnect::_MsgMap = { &FrForm::_MsgMap, &FrAutoConnect::_MsgEntries[0] };

sFRESH_ENTRY FrAutoConnect::_MsgEntries[] = {
	// serverdlg.cpp:301
	{ "drawingArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrAutoConnect::OnDrawArea },
	// serverdlg.cpp:302
	{ "proc_msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAutoConnect::OnInitStaticMsg },
	// serverdlg.cpp:303
	{ "yes", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAutoConnect::OnInitBt1 },
	// serverdlg.cpp:304
	{ "no", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAutoConnect::OnInitBt2 },

	// serverdlg.cpp:306
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// serverdlg.cpp:310 (run-time type and factory registration of FrServerDlg)
const WRTTI FrServerDlg::m_RTTI("FrServerDlg", &FrForm::m_RTTI);

IObject* FrServerDlgMakeInstance(void) {
	return new FrServerDlg;
}

struct __sFrServerDlg {
	__sFrServerDlg(void) {
		ObjectFactory().AddObjectFunctor(FrServerDlgMakeInstance, "FrServerDlg");
	}
};
static __sFrServerDlg s_registerFrServerDlg;

// serverdlg.cpp:312
const sFRESH_MSGMAP* FrServerDlg::GetMessageMap(void) const {
	return &FrServerDlg::_MsgMap;
}

sFRESH_MSGMAP FrServerDlg::_MsgMap = { &FrForm::_MsgMap, &FrServerDlg::_MsgEntries[0] };

sFRESH_ENTRY FrServerDlg::_MsgEntries[] = {
	// serverdlg.cpp:314
	{ "serverlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnServerListInit },
	// serverdlg.cpp:315
	{ "serverlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnServerListOwnerDraw },
	// serverdlg.cpp:316
	{ "serverlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrServerDlg::OnServerListBtnDown },
	// serverdlg.cpp:317
	{ "channellist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnChannelListInit },
	// serverdlg.cpp:318
	{ "channellist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnChannelListOwnerDraw },
	// serverdlg.cpp:319
	{ "channellist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrServerDlg::OnChannelListBtnDown },
	// serverdlg.cpp:320
	{ "connectmessage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnConnectMessageInit },
	// serverdlg.cpp:321
	{ "connect_cancle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnConnectCancleInit },
	// serverdlg.cpp:322
	{ "connect_cancle", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrServerDlg::OnConnectCancleUp },

	// serverdlg.cpp:324
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnMessageInit },
	// serverdlg.cpp:325
	{ "curserver", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnCurServerInit },
	// serverdlg.cpp:326
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrServerDlg::OnCloseBtnInit },
	// serverdlg.cpp:327
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrServerDlg::OnCloseLBtnUp },

	// serverdlg.cpp:329
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// serverdlg.cpp:366
FrServerDlg::FrServerDlg(void) {
	m_pServerList = 0;
	m_pChannelList = 0;
	m_loginWaitTime = 0;
	m_connectDelay = 0;
	m_state = gsLoginState_None;
	m_pMessage = 0;
	m_pCurServer = 0;
	m_pCancel = 0;
	m_bUnused = false;
	m_pNotifyDlg = 0;
	m_pWaitNotifyDlg = 0;
	m_pLoginDlg = 0;
	m_bAlert = false;
	m_alertBlinkTime = 0;
	m_bAutoLoginEnabled = false;

	memset(m_pServerIcon, 0, sizeof(m_pServerIcon));
	m_pConnectionIcon = 0;

	m_pTitleFont = g_resrcmng->GetTitleFont();
	char* fontFile = "[font_angel.jpg";

	m_titleFontInfo.filename = &fontFile;
	m_titleFontInfo.flag = 0;
	m_titleFontInfo.fonth = 0xB;
	m_titleFontInfo.fontw = 6;
	m_titleFontInfo.numPages = 1;
	m_titleFontInfo.texw = 0x80;
	m_titleFontInfo.texh = 0x20;
	m_titleFontInfo.pCharSet = "1234567890";
	m_pTitleFont->Create(&m_titleFontInfo);

	m_bReturnToLobby = (WSingleton<CSharedDoc>::Instance()->m_currentChannel.id == 0xFF);
	m_loginReplyCount = 0;
	m_pAutoConnect = 0;
	m_pProgressBar = 0;
	m_pConnectCancle = 0;
	memset(m_serverList, 0, sizeof(m_serverList));
}

// serverdlg.cpp:410
FrServerDlg::~FrServerDlg(void) {
	ClearServerList();

	if (m_pProgressBar) {
		delete m_pProgressBar;
		m_pProgressBar = 0;
	}

	m_textMap.clear();




	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// serverdlg.cpp:425
void FrServerDlg::ClearServerList(void) {
	m_pServerList->ClearItem();
	memset(m_serverList, 0, sizeof(m_serverList));
}

// serverdlg.cpp:431
void FrServerDlg::SetState(gsLoginState_t state) {
	m_state = state;
	if (state == 4) {
		m_bAutoLoginEnabled = false;
	}
}

// serverdlg.cpp:440
void FrServerDlg::OnCancel(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.id == 0xff) {

		SetLoginMessageAlert("\xc3\xa4\xb3\xce\xc0\xbb \xbc\xb1\xc5\xc3\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
		OpenQuitDlg();
		return;
	}


	FrForm::Close(true);
}

// serverdlg.cpp:452
bool FrServerDlg::OnInit(void) {
	m_textMap[0] = std::string("list_bongdal");
	m_textMap[1] = std::string("list_pipin");
	m_textMap[2] = std::string("list_titangboo");
	m_textMap[3] = std::string("list_dolphiny");
	m_textMap[4] = std::string("list_lolo");
	m_textMap[5] = std::string("list_quma");
	m_textMap[6] = std::string("list_tiki");
	m_textMap[7] = std::string("list_cadie");
	m_textMap[8] = std::string("list_caren");
	m_textMap[9] = std::string("");
	m_textMap[10] = std::string("");
	m_textMap[11] = std::string("");
	m_textMap[120] = std::string("");
	m_textMap[128] = std::string("");

	m_loginWaitTime = 0;

	BuildServerList();
	BuildChannelList();
	EnableDrag(false);

	if (IsLocalContent((localContentType_t)8)) {
		bool bAny = false;
		std::string text;
		std::string remain;

		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().timeBlock > 0) {
			int total = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().timeBlock;
			int days = total / 24;
			int hours = total % 24;
			if (days > 0)
				remain += MakeStr("%d\xc0\xcf ", days);

			if (hours > 0)
				remain += MakeStr("%d\xbd\xc3\xb0\xa3 ", hours);

			if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().flagBlock & 4) {
				text = MakeStr("\\c0xffff0000\\c\xb0\xb3\xc0\xce \xbb\xf3\xc1\xa1 \xba\xed\xb7\xb0");
				bAny = true;
			}

			if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().flagBlock & 8) {
				if (bAny)
					text += MakeStr(", ");
				else
					text = MakeStr("\\c0xffff0000\\c");

				text += MakeStr("\xc3\xa4\xc6\xc3 \xba\xed\xb7\xb0");
				bAny = true;
			}

			if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().flagBlock & 0x10) {
				if (bAny)
					text += MakeStr(", ");
				else
					text = MakeStr("\\c0xffff0000\\c");

				text += MakeStr("\xc0\xfc\xb1\xa4\xc6\xc7 \xba\xed\xb7\xb0");
				bAny = true;
			}

			if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().flagBlock & 0x80) {
				if (bAny)
					text += MakeStr(", ");
				else
					text = MakeStr("\\c0xffff0000\\c");

				text += MakeStr("\xc6\xed\xc1\xf6\xbe\xb2\xb1\xe2 \xba\xed\xb7\xb0");
				bAny = true;
			}

			text += MakeStr("\\c0xff000000\\c \xbb\xf3\xc5\xc2\xc0\xd4\xb4\xcf\xb4\xd9. \xba\xed\xb7\xb0 \xc7\xd8\xc1\xa6\xbd\xc3\xb0\xa3\xb1\xee\xc1\xf6 %s\xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.\n\n", remain.c_str());
			text += MakeStr("\xb0\xed\xb0\xb4\xbc\xbe\xc5\xcd\xb7\xce \xb9\xae\xc0\xc7\xc7\xcf\xbf\xa9 \xc1\xd6\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.");

			if (bAny)
				AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x23, (intptr_t)text.c_str(), 0, 0, 0, 0));
		}
	}

	return true;
}

// serverdlg.cpp:593
void FrServerDlg::OnProc(const float dt) {

	if (m_bAlert) {
		m_alertBlinkTime += dt;

		if (m_alertBlinkTime > 0.5f) {
			m_pMessage->SetVisible(!m_pMessage->IsVisible());
			m_alertBlinkTime = 0;
		}
	}

	switch (m_state) {
	case 0:
	case 4:

		m_connectDelay -= dt;
		if (m_connectDelay > 0.0f) {
			m_connectDelay = 0;
		}

		break;
	case 1:









		m_connectDelay -= dt;
		if (m_connectDelay < 0.0f) {
			ConnectToServer();
			break;
		}
		m_bAutoLoginEnabled = true;


		break;
	case 2:

		if (WSingleton<WNetworkSystem>::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_DEFAULT)) {

			if (!WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {

				m_state = (gsLoginState_t)4; m_bAutoLoginEnabled = false;
			}
		}

		break;
	case 3:
		m_state = (gsLoginState_t)4; m_bAutoLoginEnabled = false;
		break;
	case 5:





		m_state = (gsLoginState_t)3;

		break;
	case 14:













		SetLoginMessage("\xc0\xd4\xc0\xe5 \xc0\xda\xb0\xdd\xc0\xbb \xb9\xde\xc0\xba \xc0\xaf\xc0\xfa\xb8\xb8 \xc1\xa2\xbc\xd3\xc7\xd2 \xbc\xf6 \xc0\xd6\xb4\xc2 \xbc\xad\xb9\xf6\xc0\xd4\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 15:



		SetLoginMessage("\xc0\xd4\xc0\xe5 \xc1\xa6\xc7\xd1 \xbd\xc3\xb0\xa3\xc0\xcc \xc1\xf6\xb3\xaa\xbc\xad \xbc\xad\xb9\xf6\xc1\xa2\xbc\xd3\xc0\xbb \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 16:



		SetLoginMessage("\xb4\xeb\xc8\xb8 \xc2\xfc\xb0\xa1\xc0\xda\xb0\xa1 \xbe\xc6\xb4\xcf\xb6\xf3\xbc\xad \xbc\xad\xb9\xf6\xc1\xa2\xbc\xd3\xc0\xbb \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 20:



		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x20, 0, 0, 0, 0, 0);

		break;
	case 17:



		SetLoginMessage("PangYa is not available from your area.");
		FailedToConnectServer();

		break;
	case 9:



		SetLoginMessage("\xbb\xe7\xbf\xeb\xb1\xdd\xc1\xf6\xb5\xc8 \xbe\xc6\xc0\xcc\xb5\xf0\xc0\xd4\xb4\xcf\xb4\xd9. \xb0\xed\xb0\xb4\xc1\xf6\xbf\xf8\xbc\xbe\xc5\xcd\xb7\xce \xb9\xae\xc0\xc7\xba\xce\xc5\xb9\xb5\xe5\xb8\xb3\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 18:



		SetLoginMessage("\xb0\xe8\xc1\xa4 \xc0\xcc\xc0\xfc\xc0\xcc \xb5\xc7\xbe\xee \xc1\xa2\xbc\xd3\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 8:



		SetLoginMessage("\xbf\xb5\xb1\xb8\xc0\xfb\xc0\xb8\xb7\xce \xbb\xe7\xbf\xeb\xb1\xdd\xc1\xf6\xb5\xc8 \xbe\xc6\xc0\xcc\xb5\xf0\xc0\xd4\xb4\xcf\xb4\xd9. \xb0\xed\xb0\xb4\xc1\xf6\xbf\xf8\xbc\xbe\xc5\xcd\xb7\xce \xb9\xae\xc0\xc7\xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.");
		FailedToConnectServer();

		break;
	case 19:



		SetLoginMessage("\xc7\xf6\xc0\xe7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xcf\xb1\xe2 \xbe\xee\xb7\xc1\xbf\xee \xbb\xf3\xc5\xc2\xc0\xd4\xb4\xcf\xb4\xd9. \xc0\xe1\xbd\xc3 \xc8\xc4\xbf\xa1 \xb4\xd9\xbd\xc3 \xc1\xa2\xbc\xd3\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
		FailedToConnectServer();
		break;
	case 12:
	case 13:
		FailedToConnectServer();
		break;
	}





	if (m_pConnectMessage->IsVisible()) {

		m_loginWaitTime += dt;
		if (m_loginWaitTime > 10.0f) {


			m_pConnectCancle->Enable(true);
		}
	}
















}

// serverdlg.cpp:752
void FrServerDlg::AddLoginReplyCount(int count) {

	if (m_loginReplyCount == 0) {


		m_loginWaitTime = 0; m_pConnectMessage->SetVisible(true);

		if (m_pConnectCancle) {
			m_pConnectCancle->SetVisible(true);
		}
	}

	if (count == 21) {



		m_loginWaitTime = 0; m_loginReplyCount = 21; m_pConnectMessage->SetVisible(false);

		if (m_pConnectCancle) {

			m_pConnectCancle->SetVisible(false);
			m_pConnectCancle->Enable(false);
		}
	} else {




		m_loginReplyCount += 2;
		if (m_loginReplyCount > 21) {
			m_loginReplyCount = 20;
		}

		int pos = m_loginReplyCount * 100 / 21;
		m_pConnectMessage->SetCaption("\xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa4\xba\xb8\xb8\xa6 \xbf\xe4\xc3\xbb\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
		m_pProgressBar->SetPos(pos);
	}

}

// serverdlg.cpp:788
void FrServerDlg::BeginAutoLogin(void) {
	if (m_pAutoConnect) {

		m_pAutoConnect->Retry();
	} else {




		m_pAutoConnect = CreateForm<FrAutoConnect>(g_pFresh->GetManager(), this, "GHOST_LOADING", 0);
		m_pAutoConnect->EnableDrag(false);
		m_pAutoConnect->Open((FRESH_RESULT_CALLBACK)&FrServerDlg::OnResultAutoRecon, 0);
	}
}

// serverdlg.cpp:804
bool FrServerDlg::OnResultAutoRecon(int ret, FrForm* pForm) {




	m_pAutoConnect = 0;
	return true;
}

// serverdlg.cpp:814
void FrServerDlg::FinishAutoLogin(void) {
	if (m_pAutoConnect) {
		m_pAutoConnect->Close(true);
	}
}

// serverdlg.cpp:822
bool FrServerDlg::CloseByEnterChannel(bool bDestroy) {
	if (m_bReturnToLobby && !g_pFresh->IsCurrentLayout("SERVERLIST")) {

		if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI))) {

			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
		}

		CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
	}


	return FrForm::Close(bDestroy);
}

// serverdlg.cpp:837
// Selects the server `serverId` of the list as the one to connect to (the
// automatic reconnection): the list is walked from its last item; a server
// the player is already connected to is skipped.
void FrServerDlg::ConnectToServer(unsigned long serverId) {

	m_bConnectById = 1;
	int count = m_pServerList->GetCurrentItemSize(false);

	sGameServerInfo* pServer = 0;
	for (int i = 1; i <= count; i++) {

		FrListItem* pItem = m_pServerList->GetItem(count - i);
		if (!pItem)
			continue;


		pServer = (sGameServerInfo*)pItem->pData;
		if (!pServer || !pServer->name[0])
			continue;


		if (ConnectedServer()->id == 0) {
			memcpy(ConnectedServer(), &m_selectedServer, sizeof(sGameServerInfo));

		} else if (ConnectedServer()->id == pServer->id) {
			continue;
		}




		if (pServer->id == serverId)
			break;
	}


	memcpy(&m_selectedServer, pServer, sizeof(sGameServerInfo));
}

// serverdlg.cpp:334
bool ServerListCompare(const void* a, const void* b) {
	const sGameServerInfo* pA = (const sGameServerInfo*)a;
	const sGameServerInfo* pB = (const sGameServerInfo*)b;

	if ((pA->flags & 0x80) || (pB->flags & 0x80))
		return false;

	if ((pA->m_flags2 & 0x40) || (pB->flags & 0x40))
		return false;

	return pA->m_curUsers > pB->m_curUsers;
}

// serverdlg.cpp:869
void FrServerDlg::ConnectToServer(void) {
	m_pServerList->Enable(false);
	m_pChannelList->Enable(false);

	WSingleton<CSharedDoc>::Instance()->m_gameServerAddress = m_selectedServer.m_address;
	WSingleton<CSharedDoc>::Instance()->m_gameServerPort = m_selectedServer.m_port;
	WSingleton<CSharedDoc>::Instance()->m_value84 = m_selectedServer.id;

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
		WSingleton<WNetworkSystem>::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_DEFAULT);

	WSingleton<WNetworkSystem>::Instance()->SendMessage(WNetworkSystem::NET_UNIT_DEFAULT, NetworkUnit::EVENT_NEXT);

	WSingleton<CMessengerInfo>::Instance()->m_waitingForServerList = false;

	m_bUnused = false;
	m_loginWaitTime = 0;
	if (m_pConnectMessage)
		m_pConnectMessage->SetVisible(false);

	memcpy(WSingleton<CSharedDoc>::Instance()->m_serverName, &m_selectedServer, sizeof(sGameServerInfo));

	m_state = (gsLoginState_t)2;
}

// serverdlg.cpp:911
void FrServerDlg::BuildServerList(void) {
	ClearServerList();

	unsigned int count = 0;
	std::list<sGameServerInfo>::iterator it;
	// The servers flagged 0x80 (and not 0x10) come first.
	for (it = WSingleton<CSharedDoc>::Instance()->m_gameServerList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameServerList.end(); ++it) {
		if ((it->flags & 0x80) && !(it->flags & 0x10)) {
			m_pServerList->AddItem(&*it);
			count++;
		}
	}

	for (it = WSingleton<CSharedDoc>::Instance()->m_gameServerList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameServerList.end(); ++it) {
		if ((it->flags & 0x80) && !(it->flags & 0x10))
			continue;

		if ((it->flags & 0x10) && !((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity >> 10) & 1) && IsLocalContent((localContentType_t)10)) {
			if (!((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity >> 2) & 1))
				continue;

			strcat(it->name, " [\xb8\xc1\xc5\xe4]");
		}

		m_pServerList->AddItem(&*it);
		count++;
	}

	if (count < 9) {
		for (unsigned int i = 0; i < 9 - count; i++) {
			m_pServerList->AddItem(&m_serverList[i]);
		}
	}

	m_pServerList->SortItem(ServerListCompare);
	m_pServerList->GetScrollBar()->ScrollToFirst();
}

// serverdlg.cpp:1012
bool FrServerDlg::OnLoginDlgResult(int ret, FrForm* pForm) {
	m_pLoginDlg = 0;
	return true;
}

// serverdlg.cpp:1018
void FrServerDlg::OnServerListInit(intptr_t param) {
	m_pServerList = DYNAMIC_CAST(FrListBox, param);

	if (m_pServerList) {


		m_pServerList->SetPushSound(0);

		m_pServerIcon[0] = g_pFresh->GetManager()->GetBitmap("ITEMS", "icon_expevent");
		m_pServerIcon[1] = g_pFresh->GetManager()->GetBitmap("ITEMS", "icon_pangevent");
		m_pServerIcon[2] = g_pFresh->GetManager()->GetBitmap("ITEMS", "icon_angelwing");

		m_pServerIcon[3] = g_pFresh->GetManager()->GetBitmap("ITEMS", "icon_community");


		m_pConnectionIcon = g_pFresh->GetManager()->GetBitmap("ITEMS", "icon_connection");
	}
}

// serverdlg.cpp:1036
void FrServerDlg::OnServerListBtnDown(void) {
	if (!m_pServerList)
		return;

	FrListItem* pItem = m_pServerList->GetItemUnderCursor();
	if (!pItem)
		return;

	sGameServerInfo* pServer = (sGameServerInfo*)pItem->pData;
	if (!pServer || !pServer->name[0])
		return;

	if (ConnectedServer()->id == pServer->id)
		return;

	if (m_connectDelay > 0)
		return;

	if (!(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0x8014)) {
		if (pServer->flags & 0x40) {
			if (WSingleton<CSharedDoc>::Instance()->m_myLevel > 5) {
				SetLoginMessageAlert("��Ű ������ ������ �� �ֽ��ϴ�.");
				return;
			}

			if (WSingleton<CSharedDoc>::Instance()->m_myLevel == 0) {
				SetLoginMessageAlert("��Ű" "F ������ ������ �� �����ϴ�.");
				return;
			}
		}

		if ((pServer->flags & 4) && WSingleton<CSharedDoc>::Instance()->m_myLevel < 6) {
			SetLoginMessageAlert("���� �����̻� ������ �� �ֽ��ϴ�.");
			return;
		}

		if ((pServer->flags & 8) && WSingleton<CSharedDoc>::Instance()->m_value4D70 == 0) {
			SetLoginMessageAlert(MakeStr("��%d�� �̻� ������ �� �ִ� ���μ����Դϴ�.", 19));
			return;
		}
	}

	int maxUsers = pServer->m_maxUsers;
	int curUsers = pServer->m_curUsers;
	if (curUsers >= maxUsers - 200 && (curUsers >= maxUsers - 150 || rand() % 3 != 0)
		&& !(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0x14)) {
		SetLoginMessageAlert("������ ���� �ο��� ��� á���ϴ�.");
		m_connectDelay = 1.0f;
	} else {
		m_pAutoConnect = 0;
		m_selectedServer = *pServer;
		m_connectDelay = 0;

		WSingleton<CSharedDoc>::Instance()->ClearCurrentChannel(true);
		m_state = (gsLoginState_t)1;
		m_pChannelList->ClearItem();
		m_pCancel->SetVisible(false);
		m_pMessage->SetVisible(false);
		m_bAlert = false;

		g_audio->PlaySfx("ui_desktop_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	WSingleton<CSharedDoc>::Instance()->m_giftList2.clear();
	WSingleton<CSharedDoc>::Instance()->m_mailList.clear();
	m_loginReplyCount = 0;
	if (m_pProgressBar)
		m_pProgressBar->SetPos(10);

	S5::UTIL::SetServerProperty(pServer->flags);
}

// serverdlg.cpp:1158
bool FrServerDlg::IsNotDrawServerList(FrListItem* pItem) {
	if (!m_pServerList) return true;
	if (!pItem) return true;
	return !pItem->pData;
}

// serverdlg.cpp:1166
void FrServerDlg::DrawServerButton(const WPoint& pos, const sGameServerInfo* pServer, bool bOver) {
	bool bDisable = m_connectDelay > 0;

	THEME_BUTTONS buttons;
	S5::THEME::GetServerSelectButton(pServer->flags, buttons);

	std::string imgName;
	if (bDisable) {
		imgName = buttons.szPush;
	} else {
		imgName = bOver ? buttons.szOver : buttons.szNormal;
	}

	const Bitmap* pBitmap = g_pFresh->GetBitmap(imgName.c_str());

	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	gdi->DrawTexture(pBitmap, WRect(pos.x, pos.y, pBitmap->Width(), pBitmap->Height()), 0xffffffff, 0);

	if (strcmp(pServer->name, "") != 0) {
		std::map<unsigned int, std::string>::iterator it = m_textMap.find(pServer->m_iconKey);
		if (it == m_textMap.end())
			return;

		std::string& iconName = it->second;
		if (iconName.length()) {
			const Bitmap* pIcon = g_pFresh->GetBitmap(iconName.c_str());
			gdi->DrawTexture(pIcon, WRect(pos, pIcon->Width(), pIcon->Height()), 0xffffffff, 0);
		}

		gdi->SetTextStyle(1);

		char name[0x20] = { 0 };
		if (strlen(pServer->name) > 0x10) {
			mbscut(name, pServer->name, 0x10);
			strcat(name, "...");
		} else {
			strncpy(name, pServer->name, 0x1f);
		}

		gdi->PrintText(WPoint(pos.x + 51.0f, pos.y + 10.0f), 0, name, 0);
		gdi->SetTextStyle(0);
	}
}

// serverdlg.cpp:1237
void FrServerDlg::OnServerListOwnerDraw(int param) {
	if (!m_pServerList)
		return;

	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	sGameServerInfo* pServer = (sGameServerInfo*)pItem->pData;
	if (!pServer)
		return;

	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	DrawServerButton(pItem->pos, pServer, pItem->underCursor);

	if (strcmp(pServer->name, "") == 0)
		return;

	gdi->SetTextStyle(1);

	if (ConnectedServer()->id == pServer->id)
		gdi->DrawTexture(m_pConnectionIcon, WRect(pItem->pos.x + 9.0f, pItem->pos.y + 3.0f, m_pConnectionIcon->Width(), m_pConnectionIcon->Height()), 0xffffffff, 0);

	if (pServer->m_badgeCount > 0) {
		float x = m_rect.x;
		float y = m_rect.y;
		gdi->DrawTexture(m_pServerIcon[2], WRect(x + 420.0f, y + 83.0f, m_pServerIcon[2]->Width(), m_pServerIcon[2]->Height()), 0xffffffff, 0);

		const char* text = MakeStr("%d", pServer->m_badgeCount);
		m_pTitleFont->GetTextWidth(g_view, text);
		m_pTitleFont->Print(g_view, x + 440.0f, y + 90.0f, text, 0, 0xffffffff, 0);
	}

	float offset = 143.0f;
	if (pServer->m_flags2 & 0x40) {
		gdi->DrawTexture(m_pServerIcon[3], WRect(pItem->pos.x + 143.0f, pItem->pos.y + 8.0f, m_pServerIcon[3]->Width(), m_pServerIcon[3]->Height()), 0xffffffff, 0);
		offset = 128.0f;
	}

	if (pServer->m_flags2 & 0x2) {
		gdi->DrawTexture(m_pServerIcon[1], WRect(offset + pItem->pos.x, pItem->pos.y + 8.0f, m_pServerIcon[1]->Width(), m_pServerIcon[1]->Height()), 0xffffffff, 0);
		offset -= 15.0f;
	}

	if (pServer->m_flags2 & 0x10) {
		gdi->DrawTexture(m_pServerIcon[0], WRect(offset + pItem->pos.x, pItem->pos.y + 8.0f, m_pServerIcon[0]->Width(), m_pServerIcon[0]->Height()), 0xffffffff, 0);
		offset -= 15.0f;
	}

	if (pServer->m_flags2 & 0x4) {
		gdi->DrawTexture(m_pServerIcon[0], WRect(offset + pItem->pos.x, pItem->pos.y + 8.0f, m_pServerIcon[0]->Width(), m_pServerIcon[0]->Height()), 0xa0ffffff, 0);
	}

	if (pServer->m_flags2 & 0x8) {
		gdi->DrawTexture(m_pServerIcon[2], WRect(pItem->pos.x + 158.0f, pItem->pos.y - 5.0f + 8.0f, m_pServerIcon[2]->Width(), m_pServerIcon[2]->Height()), 0xffffffff, 0);
	}

	if (m_pConnectMessage->IsVisible() && m_pProgressBar)
		m_pProgressBar->Display();

	gdi->SetTextStyle(0);
}

// serverdlg.cpp:1342
void FrServerDlg::BuildChannelList(void) {
	if (m_pWaitNotifyDlg)
		m_pWaitNotifyDlg->Close(true);

	if (m_pNotifyDlg)
		m_pNotifyDlg->Close(true);

	m_bAutoLoginEnabled = false; m_pChannelList->ClearItem();

	std::list<sChannelInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_channelList.begin();
	if (it != WSingleton<CSharedDoc>::Instance()->m_channelList.end()) {
		for (; it != WSingleton<CSharedDoc>::Instance()->m_channelList.end(); ++it) {
			m_pChannelList->AddItem(&*it);
		}

		TurnOnChannels();
		m_pChannelList->GetScrollBar()->ScrollToFirst();
		return;
	}

	m_pMessage->SetVisible(true); m_pCurServer->SetVisible(false); m_pChannelList->SetVisible(false); m_pServerList->Enable(true);
}

// serverdlg.cpp:1368
void FrServerDlg::OnChannelListInit(intptr_t param) {
	m_pChannelList = DYNAMIC_CAST(FrListBox, param);
	if (m_pChannelList) {


		m_pNewMark = g_pFresh->GetManager()->GetBitmap("ITEMS", "mark_new");
	}
}

// serverdlg.cpp:1377
void FrServerDlg::OnChannelListBtnDown(void) {
	FrListItem* pItem = m_pChannelList->GetItemUnderCursor();
	if (!pItem)
		return;

	const sChannelInfo* pChannel = (const sChannelInfo*)pItem->pData;
	if (!pChannel)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.id == pChannel->id) {
		Close(true);
		return;
	}

	if (!(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0x14)) {
		if (ConnectedServer()->flags & 0x20) {
			// A server with the level limits of the channels.
			unsigned long games = WSingleton<CSharedDoc>::Instance()->m_myStatGamesPlayed;
			if (games < 1)
				games = 1;

			float ratio = (float)WSingleton<CSharedDoc>::Instance()->m_myStatNoMannerGames / (float)games;
			if (ratio >= 0.4f) {
				SetLoginMessageAlert("\xb0\xad\xc1\xa6\xc1\xbe\xb7\xe1\xb7\xfc\xc0\xcc 40%\xc0\xcc\xbb\xf3\xc0\xce \xc0\xaf\xc0\xfa\xb4\xc2 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x800) && WSingleton<CSharedDoc>::Instance()->m_myLevel > 10) {
				SetLoginMessageAlert("\xb7\xe7\xc5\xb0/\xba\xf1\xb1\xe2\xb3\xca\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if (pChannel->flags & 0x1000) {
				if (WSingleton<CSharedDoc>::Instance()->m_myLevel < 11 || WSingleton<CSharedDoc>::Instance()->m_myLevel > 20) {
					SetLoginMessageAlert("\xc1\xd6\xb4\xcf\xbe\xee/\xbd\xc3\xb4\xcf\xbe\xee\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
					return;
				}
			}

			if ((pChannel->flags & 0x2000) && WSingleton<CSharedDoc>::Instance()->m_myLevel < 21) {
				SetLoginMessageAlert("\xbe\xc6\xb8\xb6\xc3\xdf\xbe\xee\xc0\xcc\xbb\xf3 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}
		} else {
			if ((pChannel->flags & 0x800) && WSingleton<CSharedDoc>::Instance()->m_myLevel > 5) {
				SetLoginMessageAlert("\xb7\xe7\xc5\xb0\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x1000) && (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6 || WSingleton<CSharedDoc>::Instance()->m_myLevel > 15)) {
				SetLoginMessageAlert("\xba\xf1\xb1\xe2\xb3\xca/\xc1\xd6\xb4\xcf\xbe\xee\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x2000) && (WSingleton<CSharedDoc>::Instance()->m_myLevel < 11 || WSingleton<CSharedDoc>::Instance()->m_myLevel > 20)) {
				SetLoginMessageAlert("\xc1\xd6\xb4\xcf\xbe\xee/\xbd\xc3\xb4\xcf\xbe\xee\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x80) && WSingleton<CSharedDoc>::Instance()->m_myLevel < 6) {
				SetLoginMessageAlert("\xba\xf1\xb1\xe2\xb3\xca \xc0\xcc\xbb\xf3 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x200) && WSingleton<CSharedDoc>::Instance()->m_myLevel > 15) {
				SetLoginMessageAlert("\xc1\xd6\xb4\xcf\xbe\xee \xc0\xcc\xc7\xcf\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}

			if ((pChannel->flags & 0x400) && WSingleton<CSharedDoc>::Instance()->m_myLevel < 11) {
				SetLoginMessageAlert("\xc1\xd6\xb4\xcf\xbe\xee \xc0\xcc\xbb\xf3\xb8\xb8 \xc0\xd4\xc0\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
				return;
			}
		}
	}

	m_pMessage->SetVisible(false);

	WSingleton<CSharedDoc>::Instance()->m_selectedChannel = *pChannel;

	if (WSingleton<CSharedDoc>::Instance()->m_lobbyStateFlag) {
		WSendPacket packet(0x83);
		packet.Encode1(pChannel->id);
		packet.Send(SEND_TO_DEFAULT);
	} else {
		WSendPacket packet(4);
		packet.Encode1(pChannel->id);
		packet.Send(SEND_TO_DEFAULT);
	}

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && WSingleton<CMessengerInfo>::Instance()->m_serverListRequest == 4) {
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber = 0xFFFF;
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType = 0xFF;

		const sChannelInfo* pSelected = &WSingleton<CSharedDoc>::Instance()->m_selectedChannel;
		sUserPosition position;
		position.m_userValue = WSingleton<CSharedDoc>::Instance()->m_value84;
		position.m_channelId = pSelected->id;
		wsprintfA(position.m_channelName, pSelected->name);
		position.m_roomNumber = 0xFFFF;
		position.m_value2 = (unsigned long)-1;

		WSendPacket msnPacket(0x23);
		msnPacket.EncodeBuffer(&position, sizeof(sUserPosition));
		msnPacket.Send(SEND_TO_MSN);
	}

	m_pCancel->SetVisible(false);
}

// serverdlg.cpp:1519
bool FrServerDlg::IsDisableEnterChannel(unsigned long flags, unsigned char level) {









	if ((flags & 0x800 && WSingleton<CSharedDoc>::Instance()->m_myLevel > 5) || (flags & 0x1000 && (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6 || WSingleton<CSharedDoc>::Instance()->m_myLevel > 15)) || (flags & 0x2000 && (WSingleton<CSharedDoc>::Instance()->m_myLevel < 11 || WSingleton<CSharedDoc>::Instance()->m_myLevel > 20)) || (flags & 0x200 && WSingleton<CSharedDoc>::Instance()->m_myLevel > 15) || (flags & 0x400 && WSingleton<CSharedDoc>::Instance()->m_myLevel < 11)) {

		return true;
	}

	return false;
}

// serverdlg.cpp:1537
void FrServerDlg::DrawChannelButton(const WPoint& pos, const sChannelInfo* pChannel, bool bOver) {

	bool bDisable = IsDisableEnterChannel(pChannel->flags, WSingleton<CSharedDoc>::Instance()->m_myLevel);










	THEME_BUTTONS buttons;
	S5::THEME::GetChannelSelectButton(S5::UTIL::GetServerProperty(), buttons);

	std::string imgName;
	if (bDisable) {
		imgName = buttons.szPush;
	} else {

		imgName = bOver ? buttons.szOver : buttons.szNormal;
	}


	const Bitmap* pBitmap = g_pFresh->GetBitmap(imgName.c_str());


	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	gdi->DrawTexture(pBitmap, WRect(pos.x, pos.y, pBitmap->Width(), pBitmap->Height()), 0xffffffff, 0);


	if (!stricmp(pChannel->name, "\xb7\xe7\xc5\xb0") || !stricmp(pChannel->name, "\xb7\xe7\xc5\xb0\xc3\xa4\xb3\xce"))
		gdi->DrawTexture(m_pNewMark, WRect(pos.x, pos.y, m_pNewMark->Width(), m_pNewMark->Height()), 0xffffffff, 0);


	gdi->SetTextStyle(0);
	gdi->PrintText(WPoint(pos.x + 76.0f, pos.y + 15.0f), 0, pChannel->name, 0);
}

// serverdlg.cpp:1575
void FrServerDlg::OnChannelListOwnerDraw(intptr_t param) {


	FrListItem* pItem = (FrListItem*)param;
	if (pItem) {
		const sChannelInfo* pChannel = (const sChannelInfo*)pItem->pData;
		if (pChannel) {


			FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

			DrawChannelButton(pItem->pos, pChannel, pItem->underCursor);


			float x = pItem->pos.x + 76.0f;
			float y = pItem->pos.y + 31.0f;






			int step = pChannel->maxUsers / 18;

			if (step == 0)
				step = 1;

			int bars = min(18, (pChannel->curUsers + step - 1) / step);

			WOverlay::DrawBox(g_view, WRect(x, y, 110.0f, 6.0f), 0, 0xff535353, 0);

			for (int i = 0; i < bars; i++) {






				WOverlay::DrawBox(g_view, WRect(x + i * 6 + 1.0f, y + 1.0f, 6.0f, 4.0f), 0, pChannel->curUsers < pChannel->maxUsers ? 0xfffeaf24 : 0xffff0000, 0);
			}



			if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.id == pChannel->id && !stricmp(WSingleton<CSharedDoc>::Instance()->m_currentChannel.name, pChannel->name))
				gdi->DrawTexture(m_pConnectionIcon, WRect(pItem->pos.x + 9.0f, pItem->pos.y + 3.0f, m_pConnectionIcon->Width(), m_pConnectionIcon->Height()), 0xffffffff, 0);
		}
	}
}

// serverdlg.cpp:1620
void FrServerDlg::OnConnectMessageInit(intptr_t param) {
	m_pConnectMessage = DYNAMIC_CAST(FrStatic, param);
	if (m_pConnectMessage) {


		m_pConnectMessage->SetVisible(false);
		if (!m_pProgressBar) {

			WRect rect = m_pConnectMessage->GetRect();
			WRect barRect(rect.x, rect.y - 25, 180, 30);



			m_pProgressBar = new cProgressBar;
			m_pProgressBar->Initialize(barRect, 3, 3);
			m_pProgressBar->SetImg("gauge_left_frm", "gauge_middle_frm", "gauge_right_frm", "gauge_bar_frm");
			m_pProgressBar->SetRange(0, 100);
			m_pProgressBar->SetPos(10);

			rect.x += 25;
			m_pConnectMessage->SetRect(rect);
		}
	}
}

// serverdlg.cpp:1646
void FrServerDlg::OnConnectCancleInit(intptr_t param) {
	m_pConnectCancle = DYNAMIC_CAST(FrButton, param);


	m_pConnectCancle->Enable(false);
	m_pConnectCancle->SetVisible(false);
}

// serverdlg.cpp:1656
void FrServerDlg::OnConnectCancleUp(void) {
	m_pConnectCancle->Enable(false);
	m_pConnectCancle->SetVisible(false);


	if (m_pConnectMessage->IsVisible()) {
		m_pConnectMessage->SetVisible(false);
	}

	if (m_pProgressBar) {
		m_pProgressBar->SetPos(0);
	}

	FailedToConnectServer();
}

// serverdlg.cpp:1671
void FrServerDlg::OnMessageInit(intptr_t param) {
	m_pMessage = DYNAMIC_CAST(FrStatic, param);
}

// serverdlg.cpp:1678
void FrServerDlg::OnCurServerInit(intptr_t param) {
	m_pCurServer = DYNAMIC_CAST(FrStatic, param);
}

// serverdlg.cpp:1685
void FrServerDlg::OnCloseBtnInit(intptr_t param) {
	m_pCancel = DYNAMIC_CAST(FrButton, param);
	if (g_pFresh->IsCurrentLayout("SERVERLIST")) {

		m_pCancel->SetVisible(false);
	}
}

// serverdlg.cpp:1694
void FrServerDlg::OnCloseLBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.id == 0xff) {

		SetLoginMessageAlert("\xc3\xa4\xb3\xce\xc0\xbb \xbc\xb1\xc5\xc3\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
		OpenQuitDlg();
		return;
	}


	FrForm::Close(true);
}

// serverdlg.cpp:1706
void FrServerDlg::SetLoginMessage(const char* msg) {
	m_pMessage->SetCaption(msg);
	m_pMessage->SetVisible(true);
	m_pMessage->SetTextColor(0xff000000, 0xffffffff);
	m_bAlert = false;
}

// serverdlg.cpp:1714
void FrServerDlg::SetLoginMessageAlert(const char* msg) {
	m_pMessage->SetCaption(msg);
	m_pMessage->SetVisible(true);
	m_pMessage->SetTextColor(0xffff0000, 0xffffffff);
	m_bAlert = true;
}

// serverdlg.cpp:1722
void FrServerDlg::TurnOnChannels(void) {
	m_pCurServer->SetCaption(WSingleton<CSharedDoc>::Instance()->m_serverName);
	m_pMessage->SetVisible(false);
	m_pCurServer->SetVisible(true);
	m_pChannelList->SetVisible(true);
	m_pChannelList->Enable(true);
	m_pServerList->Enable(true);
}

// serverdlg.cpp:1732
void FrServerDlg::TurnOffChannels(void) {
	m_pMessage->SetVisible(true);
	m_pCurServer->SetVisible(false);
	m_pChannelList->SetVisible(false);
	m_pServerList->Enable(true);
}

// serverdlg.cpp:1740
void FrServerDlg::FailedToConnectServer(void) {
	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
		WSingleton<WNetworkSystem>::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_DEFAULT);

	// CSharedDoc keeps the connected server as an sGameServerInfo at 0x4B05
	// (see ConnectToServer); its id, at 0x4B2D, is cleared.
	ConnectedServer()->id = 0;

	m_bAutoLoginEnabled = false;

	m_state = (gsLoginState_t)0;
	m_pCurServer->SetCaption(WSingleton<CSharedDoc>::Instance()->m_serverName);
	m_pConnectMessage->SetVisible(true);
	m_pCurServer->SetVisible(true);
	m_pChannelList->SetVisible(false);

	m_pServerList->Enable(true);
}

// serverdlg.cpp:1767
void FrServerDlg::OnNotify(const char* msg, unsigned long flag) {
	if (m_pNotifyDlg) {
		m_pNotifyDlg->Close(true);
	}

	m_pNotifyDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
	m_pNotifyDlg->SetMessage(msg, false);
	m_pNotifyDlg->Open((FRESH_RESULT_CALLBACK)&FrServerDlg::OnNotifyResult, flag);
}

// serverdlg.cpp:1777
bool FrServerDlg::OnNotifyResult(int ret, FrForm* pForm) {
	m_pNotifyDlg = 0;
	return true;
}

// serverdlg.cpp:1793
bool FrServerDlg::OnWaitNotifyResult(int ret, FrForm* pForm) {
	m_pWaitNotifyDlg = 0;
	return true;
}

// serverdlg.cpp:1783
void FrServerDlg::OnWaitNotify(const char* msg, unsigned long flag) {
	if (m_pWaitNotifyDlg) {
		m_pWaitNotifyDlg->Close(true);
	}

	m_pWaitNotifyDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "wait", 0);
	m_pWaitNotifyDlg->SetMessage(msg, false);
	m_pWaitNotifyDlg->Open((FRESH_RESULT_CALLBACK)&FrServerDlg::OnWaitNotifyResult, flag);
}

// serverdlg.cpp:1805
bool FrServerDlg::OnQuitDlgResult(int ret, FrForm* pForm) {
	m_pQuitDlg = 0;
	return true;
}

// serverdlg.cpp:1798
void FrServerDlg::OpenQuitDlg(void) {
	m_pQuitDlg = CreateForm<FrLogoutDlg>(g_pFresh->GetManager(), this, "quit", 0);
	m_pQuitDlg->DisableServerSelect();
	m_pQuitDlg->Open((FRESH_RESULT_CALLBACK)&FrServerDlg::OnQuitDlgResult, 1);
}
