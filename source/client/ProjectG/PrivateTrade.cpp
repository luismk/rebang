// Original: d:\build\custom\temp\code\client\projectg\privatetrade.cpp
#include "PrivateTrade.h"

#include <string>

#include "padding.h"
#include "ChatMsg.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "commonutil.h"
#include "formbar.h"
#include "frcmdtarget.h"
#include "frdesktop.h"
#include "frform.h"
#include "fresh.h"
#include "../client/Wangreal/include/wview.h"
#include "PrivateTradeDlg.h"
#include "avatar_avatar.h"
#include "SharedDoc.h"

extern unsigned long g_CurrentTime;

enum _ePrivateTradeState {
	_ePrivateTradeState_None = 0,
	_ePrivateTradeState_Wait = 2,    // a request or accept wait bar is open
	_ePrivateTradeState_Accepted = 6 // the trade window closed with OK
};

// The private-trade state machine owned by CPrivateTrade (size 0x34, the
// `new` of CPrivateTrade's constructor).
class CPrivateTradeImpl : public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	friend class CPrivateTrade;
private:
	CPrivateTradeImpl(void);
	virtual ~CPrivateTradeImpl(void);
	void SetTraderNick(const std::string&);
	void SetTraderGuid(unsigned long);
	const std::string& GetTraderNick(void) const;
	const unsigned long GetTraderGuid(void) const;
	void SetState(_ePrivateTradeState);
	_ePrivateTradeState GetState(void) const;
	void Reset(void);
	void Init(void);
	void Process(float);
	void Display(void);
	void CloseWnd(void);
	void OpenRequestWaitWnd(const std::string& nick);
	void OpenAcceptWaitWnd(const std::string& nick);
	FrFormBar* OpenFormBar(void);
	bool OnResultWait(int ret, FrForm* pForm);
	bool OnResultTrade(int ret, FrForm* pForm);
	void OnOpenTradeWnd(void);
	void ChatMsg(const char* msg);

	_ePrivateTradeState m_state;     // 0x4
	float m_elapsed;                 // 0x8, 1.0f in the constructor
	FrPrivateTrade* m_tradeForm;     // 0xc, closed by CloseWnd
	FrFormBar* m_waitForm;           // 0x10, closed by CloseWnd
	unsigned long m_traderGuid;      // 0x14
	std::string m_traderNick;        // 0x18
};
BEGIN_LAYOUT_CHECK(CPrivateTradeImpl)
	CHECK_FIELD(CPrivateTradeImpl, m_state, 0x4);
	CHECK_FIELD(CPrivateTradeImpl, m_elapsed, 0x8);
	CHECK_FIELD(CPrivateTradeImpl, m_tradeForm, 0xc);
	CHECK_FIELD(CPrivateTradeImpl, m_waitForm, 0x10);
	CHECK_FIELD(CPrivateTradeImpl, m_traderGuid, 0x14);
	CHECK_FIELD(CPrivateTradeImpl, m_traderNick, 0x18);
END_LAYOUT_CHECK
CHECK_SIZE(CPrivateTradeImpl, 0x34);

// privatetrade.cpp:30
void CPrivateTradeImpl::SetTraderNick(const std::string& nick) { m_traderNick = nick; }
// privatetrade.cpp:31
void CPrivateTradeImpl::SetTraderGuid(unsigned long guid) { m_traderGuid = guid; }
// privatetrade.cpp:32
const std::string& CPrivateTradeImpl::GetTraderNick(void) const { return m_traderNick; }
// privatetrade.cpp:33
const unsigned long CPrivateTradeImpl::GetTraderGuid(void) const { return m_traderGuid; }

// privatetrade.cpp:47
void CPrivateTradeImpl::SetState(_ePrivateTradeState state) { m_state = state; }
// privatetrade.cpp:48
_ePrivateTradeState CPrivateTradeImpl::GetState(void) const { return m_state; }

// privatetrade.cpp:66
CPrivateTradeImpl::CPrivateTradeImpl(void) : m_state(_ePrivateTradeState_None), m_elapsed(1.0f), m_tradeForm(NULL), m_waitForm(NULL), m_traderGuid(0) {
}

// privatetrade.cpp:71
CPrivateTradeImpl::~CPrivateTradeImpl(void) {
}

// privatetrade.cpp:76
void CPrivateTradeImpl::Reset(void) {
	m_tradeForm = NULL;
	m_waitForm = NULL;
	m_state = _ePrivateTradeState_None;
	m_elapsed = 0.0f;
	m_traderGuid = 0;
	m_traderNick = "";
}

// privatetrade.cpp:89
void CPrivateTradeImpl::Init(void) {
	m_state = _ePrivateTradeState_None;
}

// privatetrade.cpp:97
void CPrivateTradeImpl::Process(float) {
}

// privatetrade.cpp:105
void CPrivateTradeImpl::Display(void) {
}

// privatetrade.cpp:115
void CPrivateTradeImpl::OnOpenTradeWnd(void) {
	CloseWnd();

	ntAvatar* pAvatar = WSingleton<ntAvatarManager>::Instance()->FindAvatarByGuid(m_traderGuid);
	if (pAvatar) {
		SetState((_ePrivateTradeState)3);

		if (m_tradeForm == NULL) {
			unsigned long typeId = pAvatar->GetCharacterInfo().typeId;
			m_tradeForm = CreateForm<FrPrivateTrade>(g_pFresh->GetManager(), this, "FrPrivateTrade", NULL);
			m_tradeForm->SetCompanion(m_traderNick, m_traderGuid, typeId);
			m_tradeForm->Open((FRESH_RESULT_CALLBACK)&CPrivateTradeImpl::OnResultTrade, 1);
		}
	} else {
		ChatMsg("\xB0\xC5\xB7\xA1 \xBB\xF3\xB4\xEB\xB0\xA1 \xB4\xEB\xC8\xAD\xB9\xE6\xBF\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \xB0\xC5\xB7\xA1\xB0\xA1 \xC3\xEB\xBC\xD2 \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
	}
}

// privatetrade.cpp:142
void CPrivateTradeImpl::OpenRequestWaitWnd(const std::string& nick) {
	if (m_waitForm == NULL) {
		SetState(_ePrivateTradeState_Wait);
		m_waitForm = OpenFormBar();
		m_waitForm->SetAutoCloseTimer(10.0f);
		m_waitForm->EnableButton(true);
		m_waitForm->SetDesc(MakeStr("%s\xB4\xD4\xBF\xA1\xB0\xD4 \xB0\xC5\xB7\xA1 \xBF\xE4\xC3\xBB\xC0\xCC \xBF\xD4\xBD\xC0\xB4\xCF\xB4\xD9.", nick.c_str()));
	}
}

// privatetrade.cpp:162
void CPrivateTradeImpl::OpenAcceptWaitWnd(const std::string& nick) {
	if (m_waitForm == NULL) {
		SetState(_ePrivateTradeState_Wait);
		m_waitForm = OpenFormBar();
		m_waitForm->SetAutoCloseTimer(12.0f);
		m_waitForm->SetDesc(MakeStr("%s\xB4\xD4\xBF\xA1\xB0\xD4 \xB0\xC5\xB7\xA1\xB8\xA6 \xBF\xE4\xC3\xBB\xC1\xDF \xC0\xD4\xB4\xCF\xB4\xD9.", nick.c_str()));
	}
}

// privatetrade.cpp:181
bool CPrivateTradeImpl::OnResultTrade(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_tradeForm = NULL;
		SetState(_ePrivateTradeState_Accepted);
	} else {
		if (ret == FrCANCEL) {
			WSendPacket packet((enumClientPacket)0xDB);
			packet.Encode1(2);
			packet.Encode4(m_traderGuid);
			packet.EncodeStr(m_traderNick);
			packet.Send(SEND_TO_DEFAULT);

			ChatMsg("\xB0\xC5\xB7\xA1\xB0\xA1 \xC3\xEB\xBC\xD2 \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
		}

		if (ret != FrNONE) {
			Reset();
		}
	}

	return true;
}

// privatetrade.cpp:216
bool CPrivateTradeImpl::OnResultWait(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_waitForm = NULL;

		WSendPacket packet((enumClientPacket)0xDB);
		packet.Encode1(4);
		packet.Encode4(m_traderGuid);
		packet.EncodeStr(m_traderNick);
		packet.Send(SEND_TO_DEFAULT);

		OnOpenTradeWnd();
	} else {
		if (ret == FrNO) {
			WSendPacket packet((enumClientPacket)0xDB);
			packet.Encode1(1);
			packet.Encode4(m_traderGuid);
			packet.EncodeStr(m_traderNick);
			packet.Send(SEND_TO_DEFAULT);
		}

		if (ret != FrNONE) {
			Reset();
		}
	}

	return true;
}

// privatetrade.cpp:248
void CPrivateTradeImpl::ChatMsg(const char* msg) {
	WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string(msg), 0xFFFF8000, false, false);
}

// privatetrade.cpp:264
FrFormBar* CPrivateTradeImpl::OpenFormBar(void) {
	FrFormBar* pBar = CreateForm<FrFormBar>(g_pFresh->GetManager(), this, "FORM_BAR", NULL);
	pBar->SetRange(0, 1000);

	WRect rc = pBar->GetRect();
	pBar->SetRect(WRect(g_view->GetWidth() - rc.w - 30.0f, 80.0f, rc.w, rc.h));

	rc.x = rc.y = 0.0f;
	pBar->Attach(rc, "course_icon_o");
	pBar->Open((FRESH_RESULT_CALLBACK)&CPrivateTradeImpl::OnResultWait, 1);

	WPoint pt = pBar->GetRelativeBarPos();
	pt.y = 30.0f;
	pBar->MoveBar(pt);

	return pBar;
}

// privatetrade.cpp:287
void CPrivateTradeImpl::CloseWnd(void) {
	if (m_tradeForm) {
		m_tradeForm->Close(FrNONE, true);
		m_tradeForm = NULL;
	}

	if (m_waitForm) {
		m_waitForm->Close(FrNONE, true);
		m_waitForm = NULL;
	}
}

// privatetrade.cpp:304
const WRTTI CPrivateTrade::m_RTTI("CPrivateTrade", &IActor::m_RTTI);

IObject* CPrivateTradeMakeInstance(void) {
	return new CPrivateTrade;
}

struct __impCPrivateTrade {
	__impCPrivateTrade(void) {
		ObjectFactory().AddObjectFunctor(CPrivateTradeMakeInstance, "CPrivateTrade");
		ActorFactory().AddActorClass("CPrivateTrade");
	}
};
static __impCPrivateTrade s_registerCPrivateTrade;

// privatetrade.cpp:308
CPrivateTrade::CPrivateTrade(void) : m_impl(NULL) {
	m_impl = new CPrivateTradeImpl;
}

// privatetrade.cpp:313
CPrivateTrade::~CPrivateTrade(void) {
	delete m_impl;
}

// privatetrade.cpp:318
void CPrivateTrade::OnLoad(void) {
}

// privatetrade.cpp:323
void CPrivateTrade::OnInit(void) {
	m_impl->Init();
	m_impl->SetState(_ePrivateTradeState_None);
}

// privatetrade.cpp:329
void CPrivateTrade::OnReload(void) {
	m_impl->SetState(_ePrivateTradeState_None);
}

// privatetrade.cpp:334
void CPrivateTrade::OnProcess(float) {
}

// privatetrade.cpp:339
void CPrivateTrade::OnDisplay(void) {
}

// privatetrade.cpp:344
void CPrivateTrade::OnDestroy(void) {
	m_impl->CloseWnd();
}

// privatetrade.cpp:349
void CPrivateTrade::HandleMsg(const MsgObject& msg) {
	if ((unsigned long)msg.m_reserved > g_CurrentTime) {

		StoreDelayedMsg(msg);
		return;
	}

	if (msg.m_msg != 0x247)
		return;

	switch (msg.m_param1) {
	case 0: {
		unsigned long guid = msg.m_param2;
		const char* nick = (const char*)msg.m_param3;

		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid != guid && m_impl->m_state == _ePrivateTradeState_None) {
			m_impl->m_state = (_ePrivateTradeState)2;

			WSendPacket packet((enumClientPacket)0xDB);
			packet.Encode1(0);
			packet.Encode4(guid);
			packet.EncodeStr(std::string(nick));
			packet.Send(SEND_TO_DEFAULT);

			m_impl->OpenAcceptWaitWnd(std::string(nick));
			m_impl->m_traderGuid = guid;
			m_impl->m_traderNick = std::string(nick);
		}
		break;
	}
	case 1: {
		m_impl->m_state = (_ePrivateTradeState)3;

		const void* items = (const void*)msg.m_param2;

		WSendPacket packet((enumClientPacket)0xDB);
		packet.Encode1(5);
		packet.Encode4(m_impl->m_traderGuid);
		packet.EncodeStr(m_impl->m_traderNick);
		packet.EncodeBuffer(items, 0x368);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 2: {
		m_impl->m_state = (_ePrivateTradeState)4;

		const void* items = (const void*)msg.m_param2;

		WSendPacket packet((enumClientPacket)0xDB);
		packet.Encode1(6);
		packet.Encode4(m_impl->m_traderGuid);
		packet.EncodeStr(m_impl->m_traderNick);
		packet.EncodeBuffer(items, 0x368);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 3: {
		m_impl->m_state = (_ePrivateTradeState)5;

		const void* myItems = (const void*)msg.m_param2;
		const void* traderItems = (const void*)msg.m_param3;

		WSendPacket packet((enumClientPacket)0xDB);
		packet.Encode1(7);
		packet.Encode4(m_impl->m_traderGuid);
		packet.EncodeStr(m_impl->m_traderNick);
		packet.EncodeBuffer(myItems, 0x368);
		packet.EncodeBuffer(traderItems, 0x368);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	}
}

// privatetrade.cpp:449
void CPrivateTrade::OnPacket(WReceivedPacket* pPacket) {
	switch (pPacket->Decode1()) {
	case 0: {
		unsigned long guid = pPacket->Decode4();
		std::string nick = pPacket->DecodeStr();

		if (m_impl->m_state != _ePrivateTradeState_None) {
			WSendPacket packet((enumClientPacket)0xDB);
			packet.Encode1(3);
			packet.Encode4(guid);
			packet.EncodeStr(nick);
			packet.Send(SEND_TO_DEFAULT);
		} else {
			m_impl->m_state = _ePrivateTradeState_Wait;
			m_impl->m_traderGuid = guid;
			m_impl->m_traderNick = nick;

			m_impl->OpenRequestWaitWnd(nick);
		}
		break;
	}
	case 2:
		if (m_impl->m_traderGuid) {
			m_impl->ChatMsg("\xB0\xC5\xB7\xA1\xB0\xA1 \xC3\xEB\xBC\xD2 \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
		}
		m_impl->CloseWnd();
		m_impl->Reset();
		break;
	case 1:
		m_impl->ChatMsg("\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xCC \xB0\xC5\xB7\xA1\xB8\xA6 \xB0\xC5\xC0\xFD \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.");
		m_impl->CloseWnd();
		m_impl->Reset();
		break;
	case 3:
		m_impl->ChatMsg("\xBB\xF3\xB4\xEB\xB9\xE6\xC0\xBA \xC7\xF6\xC0\xE7 \xB4\xD9\xB8\xA5 \xBB\xE7\xB6\xF7\xB0\xFA \xB0\xC5\xB7\xA1\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.");
		m_impl->CloseWnd();
		m_impl->Reset();
		break;
	case 4:
		m_impl->OnOpenTradeWnd();
		break;
	case 5: {
		pPacket->Decode4();
		std::string nick = pPacket->DecodeStr();

		_sPrivateTradeItem items;
		pPacket->DecodeBuffer(&items, sizeof(items));

		FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
		if (pTrade) {
			pTrade->UpdateFrTradeItem(items);
		}
		break;
	}
	case 6: {
		pPacket->Decode4();
		std::string nick = pPacket->DecodeStr();

		_sPrivateTradeItem items;
		pPacket->DecodeBuffer(&items, sizeof(items));

		FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
		if (pTrade) {
			pTrade->FinalFrWait(items);
		}
		break;
	}
	case 7: {
		pPacket->Decode4();
		std::string nick = pPacket->DecodeStr();

		_sPrivateTradeItem items;
		pPacket->DecodeBuffer(&items, sizeof(items));

		FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
		if (pTrade) {
			pTrade->FinalFrOK(items);
		}
		break;
	}
	case 9:
		pPacket->Decode1();

		if (m_impl->m_traderGuid) {
			m_impl->ChatMsg("\xC7\xF6\xC0\xE7 \xB0\xC5\xB7\xA1\xB8\xA6 \xB4\xF5\xC0\xCC\xBB\xF3 \xC1\xF8\xC7\xE0 \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
		}
		m_impl->CloseWnd();
		m_impl->Reset();
		break;
	}
}
