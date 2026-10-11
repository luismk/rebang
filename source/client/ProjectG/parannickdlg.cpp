// Original: d:\build\custom\temp\code\client\projectg\parannickdlg.cpp
#include <string.h>
#include <string>

#include "ParanNickDlg.h"
#include "ObjectFactory.h"
#include "fredit.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "binstr.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"

// parannickdlg.cpp:18 (run-time type and factory registration of FrParanNickDlg)
const WRTTI FrParanNickDlg::m_RTTI("FrParanNickDlg", &FrForm::m_RTTI);

IObject* FrParanNickDlgMakeInstance(void) {
	return new FrParanNickDlg;
}

struct __sFrParanNickDlg {
	__sFrParanNickDlg(void) {
		ObjectFactory().AddObjectFunctor(FrParanNickDlgMakeInstance, "FrParanNickDlg");
	}
};
static __sFrParanNickDlg s_registerFrParanNickDlg;

// parannickdlg.cpp:20
const sFRESH_MSGMAP* FrParanNickDlg::GetMessageMap(void) const {
	return &FrParanNickDlg::_MsgMap;
}

sFRESH_MSGMAP FrParanNickDlg::_MsgMap = { &FrForm::_MsgMap, &FrParanNickDlg::_MsgEntries[0] };

sFRESH_ENTRY FrParanNickDlg::_MsgEntries[] = {
	// parannickdlg.cpp:22
	{ "nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrParanNickDlg::OnNickInit },
	// parannickdlg.cpp:23
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrParanNickDlg::OnOkBtnUp },

	// parannickdlg.cpp:25
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// parannickdlg.cpp:28
FrParanNickDlg::FrParanNickDlg(void) {
}

// parannickdlg.cpp:32
void FrParanNickDlg::CloseDlg(void) {
	FrForm::OnOK();
}

// parannickdlg.cpp:37
void FrParanNickDlg::OnNickInit(intptr_t param) {
	m_pNick = DYNAMIC_CAST(FrEdit, param);
}

// parannickdlg.cpp:42
void FrParanNickDlg::EnableControls(bool bEnable) {
	m_pNick->Enable(bEnable);
}

// parannickdlg.cpp:47
void FrParanNickDlg::OnOkBtnUp(void) {
	char nick[22];
	strcpy(nick, m_pNick->GetLine(1, false));
	const char* reason = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringNick(nick);

	if (reason) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

		if (pForm) {
			pForm->SetMessage(reason, false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		}
		return;
	}

	const char* text = m_pNick->GetLine(1, false);
	cTokenV token;
	token.Init(text, strlen(text));

	int count = token.GetTokenNum(0, " '", 2) - 1;

	if (count > 0) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

		if (pForm) {
			pForm->SetMessage("[']\xB4\xC2 \xB4\xEB\xC8\xAD\xB8\xED\xBF\xA1 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		}
		return;
	}

	WSendPacket packet((enumClientPacket)0x8e);

	{
		std::string nickName = m_pNick->GetLine(1, false);
		packet.EncodeStr(nickName);
	}
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 1, 0, 0, 0, 0, 0));

	m_pNick->Enable(false);
}
