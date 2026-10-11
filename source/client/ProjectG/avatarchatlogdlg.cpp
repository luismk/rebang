// Original: d:\build\custom\temp\code\client\projectg\avatarchatlogdlg.cpp
#include "AvatarChatLogDlg.h"
#include "ObjectFactory.h"
#include "ChatMsg.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "frbutton.h"
#include "fredit.h"
#include "frscrollbar.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "ReportDlg.h"

// avatarchatlogdlg.cpp:10 (run-time type and factory registration of FrAvatarChatLogDlg)
const WRTTI FrAvatarChatLogDlg::m_RTTI("FrAvatarChatLogDlg", &FrForm::m_RTTI);

IObject* FrAvatarChatLogDlgMakeInstance(void) {
	return new FrAvatarChatLogDlg;
}

struct __sFrAvatarChatLogDlg {
	__sFrAvatarChatLogDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAvatarChatLogDlgMakeInstance, "FrAvatarChatLogDlg");
	}
};
static __sFrAvatarChatLogDlg s_registerFrAvatarChatLogDlg;

// avatarchatlogdlg.cpp:12
const sFRESH_MSGMAP* FrAvatarChatLogDlg::GetMessageMap(void) const {
	return &FrAvatarChatLogDlg::_MsgMap;
}

sFRESH_MSGMAP FrAvatarChatLogDlg::_MsgMap = { &FrForm::_MsgMap, &FrAvatarChatLogDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAvatarChatLogDlg::_MsgEntries[] = {
	// avatarchatlogdlg.cpp:14
	{ "chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarChatLogDlg::OnChatViewInit },
	// avatarchatlogdlg.cpp:15
	{ "hidechat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarChatLogDlg::OnHideChatInit },
	// avatarchatlogdlg.cpp:16
	{ "showchat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarChatLogDlg::OnShowChatInit },

	// avatarchatlogdlg.cpp:18
	{ "hidechat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarChatLogDlg::OnBtnHideChatUp },
	// avatarchatlogdlg.cpp:19
	{ "showchat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarChatLogDlg::OnBtnShowChatUp },
	// avatarchatlogdlg.cpp:20
	{ "savechat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarChatLogDlg::OnBtnSaveChatUp },
	// avatarchatlogdlg.cpp:21
	{ "accuse", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarChatLogDlg::OnBtnAccuseUp },
	// avatarchatlogdlg.cpp:22
	{ "closedlg", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarChatLogDlg::OnBtnCloseDlgUp },

	// avatarchatlogdlg.cpp:24
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// avatarchatlogdlg.cpp:33
FrAvatarChatLogDlg::FrAvatarChatLogDlg(void) {
	m_pUnusedWnd1 = 0;
	m_pShowChatBtn = 0;
	m_pHideChatBtn = 0;
	m_pUnusedWnd2 = 0;
	m_pUnusedWnd3 = 0;
	m_pChatView = 0;
}

// avatarchatlogdlg.cpp:37
FrAvatarChatLogDlg::~FrAvatarChatLogDlg(void) {
}

// avatarchatlogdlg.cpp:41
void FrAvatarChatLogDlg::OnBtnShowChatUp(void) {
	if (m_pHideChatBtn) {
		m_pHideChatBtn->SetVisible(true);
	}
	if (m_pShowChatBtn) {
		m_pShowChatBtn->SetVisible(false);
	}

	GOLFDOC()->ShowChat(true);

	// "Turns the in-game chat window on."
	WSingleton<CChatMsg>::Instance()->AddChatMsg("\xB0\xD4\xC0\xD3\xB3\xBB \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xBB \xC8\xB0\xBC\xBA\xC8\xAD\xC7\xD5\xB4\xCF\xB4\xD9.", 0xFFFF7878, true, false);
}

// avatarchatlogdlg.cpp:53
void FrAvatarChatLogDlg::OnBtnHideChatUp(void) {
	if (m_pHideChatBtn) {
		m_pHideChatBtn->SetVisible(false);
	}
	if (m_pShowChatBtn) {
		m_pShowChatBtn->SetVisible(true);
	}

	GOLFDOC()->ShowChat(false);

	// "Hides the in-game chat window."
	WSingleton<CChatMsg>::Instance()->AddChatMsg("\xB0\xD4\xC0\xD3\xB3\xBB \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xBB \xBC\xFB\xB1\xE9\xB4\xCF\xB4\xD9.", 0xFFFF7878, true, false);
}

// avatarchatlogdlg.cpp:66
void FrAvatarChatLogDlg::OnBtnSaveChatUp(void) {
	int index = WSingleton<CSharedDoc>::Instance()->m_chatManager.SaveChatHistory(GOLFDOC()->m_chatHistoryIndex, 1);
	GOLFDOC()->m_chatHistoryIndex = index;
}

// avatarchatlogdlg.cpp:72
bool FrAvatarChatLogDlg::OnReportDlgResult(int ret, FrForm* pForm) {
	m_pReportDlg = 0;
	return true;
}

// avatarchatlogdlg.cpp:78
void FrAvatarChatLogDlg::OnBtnAccuseUp(void) {
	m_pReportDlg = CreateForm<FrReportDlg>(g_pFresh->GetManager(), this, "report", 0);
	if (m_pReportDlg) {



		std::string msg;
		// "1. Earlier contents of the chat window are recorded on the server too."
		msg = "1. \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xC7 \xC1\xF6\xB3\xAD \xB3\xBB\xBF\xEB\xB5\xB5 \xB8\xF0\xB5\xCE \xBC\xAD\xB9\xF6\xBF\xA1 \xB1\xE2\xB7\xCF\xB5\xCB\xB4\xCF\xB4\xD9.\n";
		// "2. Manipulated, false or repeated reports are punished."
		msg += "2. \xC1\xB6\xC0\xDB, \xC7\xE3\xC0\xA7, \xB9\xDD\xBA\xB9\xBD\xC5\xB0\xED\xB4\xC2 \xC3\xB3\xB9\xFA\xC0\xBB \xB9\xDE\xBD\xC0\xB4\xCF\xB4\xD9.\n";
		// "3. Reports are handled within 48 hours."
		msg += "3. \xBD\xC5\xB0\xED\xC7\xCF\xBD\xC5 \xB3\xBB\xBF\xEB\xC0\xBA 48\xBD\xC3\xB0\xA3 \xC0\xCC\xB3\xBB\xBF\xA1 \xC3\xB3\xB8\xAE\xB5\xCB\xB4\xCF\xB4\xD9.";

		m_pReportDlg->SetMessage(msg.c_str(), false);
		m_pReportDlg->Open((FRESH_RESULT_CALLBACK)&FrAvatarChatLogDlg::OnReportDlgResult, 3);
	}
}

// avatarchatlogdlg.cpp:94
void FrAvatarChatLogDlg::OnBtnCloseDlgUp(void) {
	Close(FrOK, true);
}

// avatarchatlogdlg.cpp:99
void FrAvatarChatLogDlg::OnHideChatInit(intptr_t param) {
	m_pHideChatBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pHideChatBtn) {


		if (GOLFDOC()->IsShowChat()) {

			m_pHideChatBtn->SetVisible(true);
		}
		else {

			m_pHideChatBtn->SetVisible(false);
		}
	}
}

// avatarchatlogdlg.cpp:115
void FrAvatarChatLogDlg::OnShowChatInit(intptr_t param) {
	m_pShowChatBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pShowChatBtn) {


		if (GOLFDOC()->IsShowChat()) {

			m_pShowChatBtn->SetVisible(false);
		}
		else {

			m_pShowChatBtn->SetVisible(true);
		}
	}
}

// avatarchatlogdlg.cpp:131
void FrAvatarChatLogDlg::OnChatViewInit(intptr_t param) {
	m_pChatView = DYNAMIC_CAST(FrEdit, param);
	if (m_pChatView) {


		m_pChatView->m_nFlags.Enable(0x1000);

		std::list<sChatLine>::iterator it;
		for (it = CSharedDoc::Instance()->m_chatLineList.begin(); it != CSharedDoc::Instance()->m_chatLineList.end(); ++it) {
			m_pChatView->AddLine((*it).m_text.c_str(), WSingleton<CChatMsg>::Instance()->ConvertToUIColor((*it).m_color), false);
		}
	}
}

// avatarchatlogdlg.cpp:146
void FrAvatarChatLogDlg::AddChatMsg(const char* msg, int color) {
	if (m_pChatView->GetLineNum() > 999) {
		// Drop the oldest line.
		m_pChatView->DeleteFirstLine();
		m_pChatView->GetScrollBar()->DelItem();
	}

	m_pChatView->AddLine(msg, color, false);
}
