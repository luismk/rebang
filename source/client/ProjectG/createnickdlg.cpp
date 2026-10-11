// Original: d:\build\custom\temp\code\client\projectg\createnickdlg.cpp
#include <string.h>
#include <string>
#include <windows.h>
#include <mmsystem.h>

#include "CreateNickDlg.h"
#include "ObjectFactory.h"
#include "EmoticonDlg.h"
#include "LogoutDlg.h"
#include "frbutton.h"
#include "fredit.h"
#include "Localize.h"
#include "fresh.h"
#include "binstr.h"
#include "commonutil.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"

// createnickdlg.cpp:19 (run-time type and factory registration of FrCreateNickDlg)
const WRTTI FrCreateNickDlg::m_RTTI("FrCreateNickDlg", &FrForm::m_RTTI);

IObject* FrCreateNickDlgMakeInstance(void) {
	return new FrCreateNickDlg;
}

struct __sFrCreateNickDlg {
	__sFrCreateNickDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCreateNickDlgMakeInstance, "FrCreateNickDlg");
	}
};
static __sFrCreateNickDlg s_registerFrCreateNickDlg;

// createnickdlg.cpp:21
const sFRESH_MSGMAP* FrCreateNickDlg::GetMessageMap(void) const {
	return &FrCreateNickDlg::_MsgMap;
}

sFRESH_MSGMAP FrCreateNickDlg::_MsgMap = { &FrForm::_MsgMap, &FrCreateNickDlg::_MsgEntries[0] };

sFRESH_ENTRY FrCreateNickDlg::_MsgEntries[] = {
	// createnickdlg.cpp:23
	{ "editnick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateNickDlg::OnEditNickInit },
	// createnickdlg.cpp:24
	{ "editnick", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrCreateNickDlg::OnEditNickEnterKey },
	// createnickdlg.cpp:25
	{ "editnick", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCreateNickDlg::OnEditNickDown },
	// createnickdlg.cpp:26
	{ "dispnick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateNickDlg::OnDispNickInit },
	// createnickdlg.cpp:27
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateNickDlg::OnMessasgeInit },
	// createnickdlg.cpp:28
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateNickDlg::OnOKBtnUp },
	// createnickdlg.cpp:29
	{ "check", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateNickDlg::OnCheckBtnUp },
	// createnickdlg.cpp:30
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateNickDlg::OnCancelBtnUp },
	// createnickdlg.cpp:31
	{ "emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateNickDlg::OnEmoticonInit },
	// createnickdlg.cpp:32
	{ "emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateNickDlg::OnEmoticonUp },

	// createnickdlg.cpp:34
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// createnickdlg.cpp:37
void NotifyErrorMsg(FrCreateNickDlg* pDlg, const char* msg) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), pDlg, "notify", 0);
	if (pForm) {
		pForm->SetMessage(msg, false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
	}
}

// createnickdlg.cpp:52
FrCreateNickDlg::FrCreateNickDlg(void)
	: m_pEditNick(0), m_pDispNick(0), m_pEmoticonBtn(0), m_pEmoticonDlg(0), m_bChecked(false) {
	m_nick[0] = '\0';
}

// createnickdlg.cpp:57
void FrCreateNickDlg::CloseDlg(int ret) {
	if (ret >= 0 && ret <= 1) {
		FrForm::OnOK();
	} else {
		FrForm::OnCancel();
	}
}

// createnickdlg.cpp:68
void FrCreateNickDlg::OnEmoticonInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// createnickdlg.cpp:76
void FrCreateNickDlg::OnEmoticonUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", this);
		if (m_pEmoticonDlg) {
			WPoint pt(0, 0);
			const WRect& rect = m_pEmoticonBtn->GetRect();
			pt.x = rect.x - 200.0f;
			pt.y = rect.y + 24.0f;

			m_pEmoticonDlg->SetFixed(true);
			m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrCreateNickDlg::OnEmoticonResult, pt, 0);
		}
	}
}

// createnickdlg.cpp:94
bool FrCreateNickDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == 1) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pId = DYNAMIC_CAST(FrEdit, FindChildByName("id"));
				if (m_pEditNick) {
					const char* front = m_pEditNick->GetEditText_Front();
					const char* comp = m_pEditNick->GetEditText_Comp();
					const char* end = m_pEditNick->GetEditText_End();
					m_pEditNick->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					m_pEditNick->SetKeyFocus(true);
				}
			}
		}
	} else {
		if (m_pEditNick) {
			m_pEditNick->SetKeyFocus(true);
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// createnickdlg.cpp:126
void FrCreateNickDlg::OnEditNickInit(intptr_t param) {
	m_pEditNick = DYNAMIC_CAST(FrEdit, param);
}

// createnickdlg.cpp:131
void FrCreateNickDlg::OnEditNickDown(void) {
}

// createnickdlg.cpp:136
bool FrCreateNickDlg::OnEditNickEnterKey(int param) {
	OnCheckBtnUp();
	return false;
}

// createnickdlg.cpp:142
void FrCreateNickDlg::OnDispNickInit(intptr_t param) {
	m_pDispNick = DYNAMIC_CAST(FrEdit, param);

	if (m_pDispNick) {
		m_pDispNick->m_nFlags.Enable(0x1000);
	}
}

// createnickdlg.cpp:151
void FrCreateNickDlg::OnMessasgeInit(intptr_t param) {
	FrEdit* pMessage = DYNAMIC_CAST(FrEdit, param);

	pMessage->AddText("\\c0xffff0000\\c* \xc7\xd1 \xb9\xf8 \xbc\xb3\xc1\xa4\xc7\xd1 \xb4\xeb\xc8\xad\xb8\xed\xc0\xba \xba\xaf\xb0\xe6\xc0\xcc \xbe\xee\xb7\xc6\xbd\xc0\xb4\xcf\xb4\xd9. \\c0xff000000\\c", false, true);
}

// createnickdlg.cpp:158
void FrCreateNickDlg::OnOKBtnUp(void) {
	if (strcmpi(m_nick, m_pEditNick->GetLine(1, false))) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		if (pForm) {
			pForm->SetMessage("\xb4\xeb\xc8\xad\xb8\xed\xc0\xcc \xc0\xaf\xc8\xbf\xc7\xd1\xc1\xf6 \xc8\xae\xc0\xce\xc7\xcf\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.", false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		}
		return;
	}

	if (!CheckNickByClient()) {
		return;
	}

	if (IsLocalContent((localContentType_t)0x81)) {
		if (!GetCheckFlag()) {
			NotifyErrorMsg(this, "\xb4\xd0\xb3\xd7\xc0\xd3 \xc1\xdf\xba\xb9\xc3\xbc\xc5\xa9\xb8\xa6 \xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4!");
			return;
		}
	}

	WSendPacket packet(6);
	packet.EncodeStr(std::string(m_pEditNick->GetLine(1, false)));
	packet.Send((eSendTo)3);

	AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
}

// createnickdlg.cpp:190
void FrCreateNickDlg::OnCheckBtnUp(void) {
	if (CheckNickByClient()) {
		WSendPacket packet(7);
		packet.EncodeStr(std::string(m_pEditNick->GetLine(1, false)));
		packet.Send((eSendTo)3);

		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
	}
}

// createnickdlg.cpp:202
void FrCreateNickDlg::OnCancelBtnUp(void) {
	FrLogoutDlg* pDlg = CreateForm<FrLogoutDlg>(g_pFresh->GetManager(), this, "quit", 0);
	pDlg->Open((FRESH_RESULT_CALLBACK)&FrCreateNickDlg::OnQuitDlgResult, 1);
}

// createnickdlg.cpp:208
bool FrCreateNickDlg::OnQuitDlgResult(int ret, FrForm* pForm) {
	CloseDlg(ret);
	return true;
}

// createnickdlg.cpp:215
bool FrCreateNickDlg::CheckNickByClient(void) {
	if (!m_pEditNick) {
		return false;
	}

	int len = strlen(m_pEditNick->GetLine(1, false));
	if (len <= 0 || len > 16) {
		NotifyErrorMsg(this, "\xb4\xeb\xc8\xad\xb8\xed \xb1\xe6\xc0\xcc\xb0\xa1 \xc0\xaf\xc8\xbf\xc7\xd1\xc1\xf6 \xc8\xae\xc0\xce\xc7\xcf\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.");
		return false;
	}

	char nick[24];
	strcpy(nick, m_pEditNick->GetLine(1, false));
	const char* reason = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringNick(nick);
	if (reason) {
		NotifyErrorMsg(this, reason);
		return false;
	}

	const char* text = m_pEditNick->GetLine(1, false);
	cTokenV token;
	token.Init(text, strlen(text));

	int count = token.GetTokenNum(0, " '", 2) - 1;
	if (count > 0) {
		NotifyErrorMsg(this, "[']\xb4\xc2 \xb4\xeb\xc8\xad\xb8\xed\xbf\xa1 \xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		return false;
	}

	if (!strcmpi(text, WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id)) {
		NotifyErrorMsg(this, "\xb4\xeb\xc8\xad\xb8\xed\xc0\xbb \xbe\xc6\xc0\xcc\xb5\xf0\xbf\xcd \xb4\xd9\xb8\xa3\xb0\xd4 \xbc\xb3\xc1\xa4\xc7\xd8 \xc1\xd6\xbd\xca\xbd\xc3\xbf\xc0.");
		return false;
	}

	return true;
}

// createnickdlg.cpp:257
void FrCreateNickDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);

	static DWORD s_lastSyncTime = timeGetTime();
	static DWORD s_curTime;

	s_curTime = timeGetTime();
	if (s_curTime - s_lastSyncTime < 200) {
		return;
	}

	s_lastSyncTime = s_curTime;
	if (strcmpi(m_pEditNick->GetLine(1, false), m_pDispNick->GetLine(1, false))) {
		m_pDispNick->SetLine(1, m_pEditNick->GetLine(1, false), 0, false, 0);
	}
}

// createnickdlg.cpp:278
void FrCreateNickDlg::ReceiveCheckCode(int code) {
	if (!IsLocalContent((localContentType_t)0x81)) {
		return;
	}

	bool bOk;
	switch (code) {
	case 0:
	case 8:
		bOk = true;
		break;
	default:
		bOk = false;
		break;
	}

	SetCheckFlag(bOk);
	if (!bOk) {
		NotifyErrorMsg(this, "\xb4\xd0\xb3\xd7\xc0\xd3 \xc1\xdf\xba\xb9\xc3\xbc\xc5\xa9\xb8\xa6 \xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4!");
	}
}

// createnickdlg.cpp:306
void FrCreateNickDlg::SetNick(const char* nick) {
	strcpy(m_nick, nick);

	if (m_pDispNick) {
		m_pDispNick->SetLine(1, nick, 0, false, 0);
	}
	if (m_pEditNick) {
		m_pEditNick->SetLine(1, nick, 0, false, 0);
	}
}
