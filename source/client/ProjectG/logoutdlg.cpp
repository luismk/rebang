// Original: d:\build\custom\temp\code\client\projectg\logoutdlg.cpp
#include "LogoutDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "fresh.h"
#include "TaskManager.h"
#include "actor.h"
#include "logininfo.h"
#include "SharedDoc.h"
#include "../../shared/Localize.h"

extern bool g_bQuit;

// logoutdlg.cpp:21 (run-time type and factory registration of FrLogoutDlg)
const WRTTI FrLogoutDlg::m_RTTI("FrLogoutDlg", &FrForm::m_RTTI);

IObject* FrLogoutDlgMakeInstance(void) {
	return new FrLogoutDlg;
}

struct __sFrLogoutDlg {
	__sFrLogoutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrLogoutDlgMakeInstance, "FrLogoutDlg");
	}
};
static __sFrLogoutDlg s_registerFrLogoutDlg;

// logoutdlg.cpp:23
const sFRESH_MSGMAP* FrLogoutDlg::GetMessageMap(void) const {
	return &FrLogoutDlg::_MsgMap;
}

sFRESH_MSGMAP FrLogoutDlg::_MsgMap = { &FrForm::_MsgMap, &FrLogoutDlg::_MsgEntries[0] };

sFRESH_ENTRY FrLogoutDlg::_MsgEntries[] = {
	// logoutdlg.cpp:25
	{ "server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLogoutDlg::OnServerBtnInit },
	// logoutdlg.cpp:26
	{ "server", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLogoutDlg::OnServerBtnUp },
	// logoutdlg.cpp:27
	{ "logout", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLogoutDlg::OnLogoutBtnInit },
	// logoutdlg.cpp:28
	{ "logout", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLogoutDlg::OnLogoutBtnUp },
	// logoutdlg.cpp:29
	{ "exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLogoutDlg::OnExitBtnInit },
	// logoutdlg.cpp:30
	{ "exit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLogoutDlg::OnExitBtnUp },

	// logoutdlg.cpp:32
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// logoutdlg.cpp:38
FrLogoutDlg::FrLogoutDlg(void) : m_pServerBtn(0), m_pLogoutBtn(0), m_pExitBtn(0), m_pHoverBtn(0) {
}

// Minimal safe implementation of the logout button handler. Send a
// logout message to the main actor and close the dialog.
void FrLogoutDlg::OnLogoutBtnUp(void) {
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEB, 0, 0, 0, 0, 0));
	Close(FrOK, true);
}

// logoutdlg.cpp:42
void FrLogoutDlg::DisableServerSelect(void) {
	m_pServerBtn->Enable(false);
}

// logoutdlg.cpp:47
void FrLogoutDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_pServerBtn && m_pServerBtn->GetStatus() == FrButton::OVER) {

		if (m_pHoverBtn != m_pServerBtn) {


			m_pHoverBtn = m_pServerBtn; SetDesc("\xC1\xA2\xBC\xD3\xC7\xD2 \xBC\xAD\xB9\xF6\xB8\xA6 \xBC\xB1\xC5\xC3\xC7\xD5\xB4\xCF\xB4\xD9.");
		}
	}
	if (m_pLogoutBtn && m_pLogoutBtn->GetStatus() == FrButton::OVER) {

		if (m_pHoverBtn != m_pLogoutBtn) {

			m_pHoverBtn = m_pLogoutBtn;
			SetDesc("\xB7\xCE\xB1\xD7\xBE\xC6\xBF\xF4 \xC7\xD5\xB4\xCF\xB4\xD9.");
		}
	}
	else if (m_pExitBtn && m_pExitBtn->GetStatus() == FrButton::OVER) {

		if (m_pHoverBtn != m_pExitBtn) {


			m_pHoverBtn = m_pExitBtn; SetDesc("\xC6\xCE\xBE\xDF \xC7\xC1\xB7\xCE\xB1\xD7\xB7\xA5\xC0\xBB \xB3\xA1\xB3\xBB\xB0\xED \xC0\xA9\xB5\xB5\xBF\xEC\xB7\xCE \xB3\xAA\xB0\xA9\xB4\xCF\xB4\xD9.");
		}
	}
}

// logoutdlg.cpp:77
void FrLogoutDlg::OnServerBtnInit(intptr_t param) {
	m_pServerBtn = DYNAMIC_CAST(FrButton, param);
	if (!m_pServerBtn) {
		return;
	}




	if (g_pFresh->IsCurrentLayout("SERVERLIST") || OfflinePlay() || g_pFresh->IsCurrentLayout("CREATE") || g_pFresh->IsCurrentLayout("AGREEMENT") || g_pFresh->IsCurrentLayout("BLANK")) {

		m_pServerBtn->Enable(false);
	}

	if (CLoginInfo::Instance()->IsWebLogin()) {


		WPoint pt = m_pServerBtn->GetRect().TopLeft();


		pt.x += 38.0f;

		m_pServerBtn->MoveWindow(pt);
	}
}

// logoutdlg.cpp:103
void FrLogoutDlg::OnServerBtnUp(void) {
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEA, 0, 0, 0, 0, 0));

	Close(FrOK, true);
}

// logoutdlg.cpp:110
void FrLogoutDlg::OnLogoutBtnInit(intptr_t param) {
	m_pLogoutBtn = DYNAMIC_CAST(FrButton, param);
	if (!m_pLogoutBtn) {
		return;
	}

	if (OfflinePlay()) {
		m_pLogoutBtn->Enable(false);
	}
	if (CLoginInfo::Instance()->IsWebLogin()) {
		m_pLogoutBtn->SetVisible(false);
	}
}

// logoutdlg.cpp:180
void FrLogoutDlg::OnExitBtnInit(intptr_t param) {
	m_pExitBtn = DYNAMIC_CAST(FrButton, param);

	if (IsLocalContent((localContentType_t)0x10)
		&& CLoginInfo::Instance()->IsWebLogin()) {


		WPoint pt = m_pExitBtn->GetRect().TopLeft();


		pt.x -= 38.0f;

		m_pExitBtn->MoveWindow(pt);
	}
}

// logoutdlg.cpp:198
void FrLogoutDlg::OnExitBtnUp(void) {
	g_bQuit = true;
}
