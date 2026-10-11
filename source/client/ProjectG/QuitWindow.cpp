// Original: d:\build\custom\temp\code\client\projectg\quitwindow.cpp
#include "NetworkSystem.h"
#include "ClientSetting.h"
#include "QuitWindow.h"
#include "mousecursor.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "OptionDlg.h"
#include "avatar_task.h"
#include "GolfDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "frbutton.h"
#include "frdesktop.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "woverlay.h"
#include "wview.h"

extern bool g_bQuit;

// clientsetting.h:415
int COption::gGetTerrainTooltip(void) {
	return m_option.m_terrainTooltip;
}

// clientsetting.h:443
void COption::SetQuitWindowed(bool windowed) {
	m_quitWindowed = windowed;
}

// quitwindow.cpp:24 (run-time type and factory registration of FrQuitForm)
const WRTTI FrQuitForm::m_RTTI("FrQuitForm", &FrForm::m_RTTI);

IObject* FrQuitFormMakeInstance(void) {
	return new FrQuitForm;
}

struct __sFrQuitForm {
	__sFrQuitForm(void) {
		ObjectFactory().AddObjectFunctor(FrQuitFormMakeInstance, "FrQuitForm");
	}
};
static __sFrQuitForm s_registerFrQuitForm;

// quitwindow.cpp:26
const sFRESH_MSGMAP* FrQuitForm::GetMessageMap(void) const {
	return &FrQuitForm::_MsgMap;
}

sFRESH_MSGMAP FrQuitForm::_MsgMap = { &FrForm::_MsgMap, &FrQuitForm::_MsgEntries[0] };

sFRESH_ENTRY FrQuitForm::_MsgEntries[] = {
	{ "option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuitForm::OnOptionBtnInit },
	{ "option", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuitForm::OnOptionBtnUp },
	{ "next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuitForm::OnNextBtnInit },
	{ "next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuitForm::OnNextBtnUp },
	{ "tolobby", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuitForm::OnToLobbyBtnInit },
	{ "tolobby", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuitForm::OnToLobbyBtnUp },
	{ "exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuitForm::OnExitBtnInit },
	{ "exit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuitForm::OnExitBtnUp },
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuitForm::OnCloseBtnUp },

	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// quitwindow.cpp:43
FrQuitForm::FrQuitForm(void) : m_pOptionBtn(0), m_pNextBtn(0), m_pToLobbyBtn(0), m_pExitBtn(0), m_descState(0), m_elapsed(0) {
	CGolfDoc* pGolfDoc = GOLFDOC();
	if (pGolfDoc && pGolfDoc->GameMode() == 0x200) {
		if (COption::Instance()->gGetTerrainTooltip()) {
			CMouseCursor::Instance()->SetTerrainToolTip(false);
		}
	}
}

// quitwindow.cpp:53
void FrQuitForm::OnDraw(void) {
	FrDesktop* pDesktop = g_pFresh->GetDesktop();

	unsigned char alpha = (unsigned char)(m_elapsed * 64.0f);
	WOverlay::DrawBox(g_view, WRect(pDesktop->GetRect()), 0, (alpha << 24) | 0x404040, 0.0f);
}

// quitwindow.cpp:115
void FrQuitForm::OnOptionBtnInit(intptr_t param) {
	m_pOptionBtn = DYNAMIC_CAST(FrButton, param);

	if (GOLFDOC()->m_gameState < 15) {
		m_pOptionBtn->SetVisible(false);
	}
}

// quitwindow.cpp:123
void FrQuitForm::OnNextBtnInit(intptr_t param) {
	m_pNextBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pNextBtn) {
		CSharedDoc* pDoc = CSharedDoc::Instance();
		if (pDoc->m_roomType == 11 || GOLFDOC()->m_gameState >= 8 || CQuitWindow::Instance()->HasOptionWindow()) {
			m_pNextBtn->SetVisible(false);
		}
	}
}

// quitwindow.cpp:133
void FrQuitForm::OnToLobbyBtnInit(intptr_t param) {
	m_pToLobbyBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pToLobbyBtn) {
		if ((__rtti_obj = (!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask())) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI)) {
			m_pToLobbyBtn->SetButtonImg("i_exit", FrButton::NORMAL);
		} else {
			m_pToLobbyBtn->SetButtonImg("i53", FrButton::NORMAL);
		}

		if (OfflinePlay()) {
			m_pToLobbyBtn->Enable(false);
		}
	}
}

// quitwindow.cpp:149
void FrQuitForm::OnExitBtnInit(intptr_t param) {
	m_pExitBtn = DYNAMIC_CAST(FrButton, param);
}

// quitwindow.cpp:154
void FrQuitForm::OnOptionBtnUp(void) {
	if (m_pOptionBtn) {
		m_pOptionBtn->Enable(false);
	}

	if (CQuitWindow::Instance()) {
		CQuitWindow::Instance()->CreateOptionWindow();
	}
}

// quitwindow.cpp:163
void FrQuitForm::OnNextBtnUp(void) {
	Close(false);

	IActor* pActor = (!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask())->GetActor("Tutorial");
	if (pActor) {
		pActor << MsgObject(0, 0x14B, 0, 0, 0, 0, 0);
	}
}

// quitwindow.cpp:175
void FrQuitForm::OnToLobbyBtnUp(void) {
	if (!WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		g_bQuit = true;
		return;
	}

	if (CSharedDoc::Instance()->m_roomType == 2) {
		OnToLobbyConfirmResult(1, 0);
		return;
	}

	FrForm* pForm;
	if (CSharedDoc::Instance()->m_gameMode == 1) {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		pForm->SetMessage("\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xB4\xEB\xB1\xE2\xBD\xC7\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	} else if (CSharedDoc::Instance()->m_gameMode == 5) {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		pForm->SetMessage("\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xB4\xEB\xB1\xE2\xBD\xC7\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?       (\xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xC0\xB2\xC0\xBA \xB1\xE2\xB7\xCF\xB5\xC7\xC1\xF6 \xBE\xCA\xC1\xF6\xB8\xB8, \xBB\xE7\xBF\xEB\xB5\xC8 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xB5\xB9\xB7\xC1 \xB9\xDE\xC1\xF6 \xB8\xF8\xC7\xD5\xB4\xCF\xB4\xD9.)", false);
	} else if (GalleryMode()) {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		pForm->SetMessage("\xB0\xB6\xB7\xAF\xB8\xAE\xB8\xA6 \xB3\xA1\xB3\xBB\xB0\xED \xB7\xCE\xBA\xF1\xB7\xCE \xB3\xAA\xB0\xA9\xB4\xCF\xB4\xD9.", false);
	} else if (CSharedDoc::Instance()->m_roomType == 11 || CSharedDoc::Instance()->m_roomType == 12) {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		pForm->SetMessage("\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xB7\xCE\xBA\xF1\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE? ", false);
	} else if ((CSharedDoc::Instance()->m_roomType == 0 && GOLFDOC()->GetCurrentPlayerNum() == 1) || CSharedDoc::Instance()->m_roomType == 10) {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		pForm->SetMessage("\xB0\xD4\xC0\xD3\xC0\xBB \xB1\xD7\xB8\xB8\xC7\xCF\xB0\xED \xB7\xCE\xBA\xF1\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	} else {
		pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "tolobby", 0);
		pForm->SetMessage(
			"\xB0\xD4\xC0\xD3\xC0\xCC \\c0xffff0000\\c\xBF\xCF\xC0\xFC\xC8\xF7 \xC1\xBE\xB7\xE1\\c0xff000000\\c\xB5\xC7\xB1\xE2 \xC0\xFC\xBF\xA1 \xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB8\xE9, \\c0xffff0000\\c\xB0\xE6\xC7\xE8\xC4\xA1 \xC8\xB9\xB5\xE6\xB0\xFA \xB0\xA2\xC1\xBE \xB1\xE2\xB7\xCF\xC0\xCC \xB9\xAB\xC8\xBF\xC3\xB3\xB8\xAE\\c0xff000000\\c\xB5\xCB\xB4\xCF\xB4\xD9.\n\n "
			"\t\t\t\t\t\t\t\xB6\xC7\xC7\xD1 \xB8\xB9\xC0\xBA \xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xB7\xCE \xC0\xCE\xC7\xD8 \xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xC0\xB2\xC0\xCC \xB4\xD9\xC0\xBD\xB0\xFA \xB0\xB0\xC0\xBA \xBC\xF6\xC4\xA1\xB0\xA1 \xB5\xC7\xB8\xE9 \\c0xffff0000\\c\xBC\xBA\xBA\xB0 \xC7\xA5\xBD\xC3 \xBE\xC6\xC0\xCC\xC4\xDC\\c0xff000000\\c\xC0\xCC \xC5\xB9\xC7\xD8\xC1\xFC\xB0\xFA \xB5\xBF\xBD\xC3\xBF\xA1 "
			"\t\t\t\t\t\t\t\\c0xffff0000\\c\xBB\xE7\xBF\xEB \xB1\xE2\xB4\xC9\xBF\xA1 \xB4\xEB\xC7\xD1 \xC1\xA6\xBE\xE0\\c0xff000000\\c\xC0\xCC \xBB\xFD\xB1\xE9\xB4\xCF\xB4\xD9.\n\n\n\n\n\n\n\n"
			"\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xB7\xCE\xBA\xF1\xB7\xCE \xB3\xAA\xB0\xA1\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	}

	pForm->Open((FRESH_RESULT_CALLBACK)&FrQuitForm::OnToLobbyConfirmResult, 1);
}

// quitwindow.cpp:240
void FrQuitForm::OnExitBtnUp(void) {
	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		FrForm* pForm;
		if (OnlinePlay()) {
			if (GalleryMode()) {
				pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
				pForm->SetMessage("\xB0\xB6\xB7\xAF\xB8\xAE\xB8\xA6 \xB3\xA1\xB3\xBB\xB0\xED \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
			} else if (CSharedDoc::Instance()->m_roomType == 2) {
				pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
				pForm->SetMessage("\xB4\xEB\xC8\xAD\xB9\xE6\xBF\xA1\xBC\xAD \xB3\xAA\xB0\xA1\xB0\xED \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
			} else {
				pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "tolobby", 0);
				pForm->SetMessage(
					"\xB0\xD4\xC0\xD3\xC0\xCC \\c0xffff0000\\c\xBF\xCF\xC0\xFC\xC8\xF7 \xC1\xBE\xB7\xE1\\c0xff000000\\c\xB5\xC7\xB1\xE2 \xC0\xFC\xBF\xA1 \xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB8\xE9, \\c0xffff0000\\c\xB0\xE6\xC7\xE8\xC4\xA1 \xC8\xB9\xB5\xE6\xB0\xFA \xB0\xA2\xC1\xBE \xB1\xE2\xB7\xCF\xC0\xCC \xB9\xAB\xC8\xBF\xC3\xB3\xB8\xAE\\c0xff000000\\c\xB5\xCB\xB4\xCF\xB4\xD9.\n\n "
					"\t\t\t\t\t\t\t\xB6\xC7\xC7\xD1 \xB8\xB9\xC0\xBA \xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xB7\xCE \xC0\xCE\xC7\xD8 \xB0\xAD\xC1\xA6\xC1\xBE\xB7\xE1\xC0\xB2\xC0\xCC \xB4\xD9\xC0\xBD\xB0\xFA \xB0\xB0\xC0\xBA \xBC\xF6\xC4\xA1\xB0\xA1 \xB5\xC7\xB8\xE9 \\c0xffff0000\\c\xBC\xBA\xBA\xB0 \xC7\xA5\xBD\xC3 \xBE\xC6\xC0\xCC\xC4\xDC\\c0xff000000\\c\xC0\xCC \xC5\xB9\xC7\xD8\xC1\xFC\xB0\xFA \xB5\xBF\xBD\xC3\xBF\xA1 "
					"\t\t\t\t\t\t\t\\c0xffff0000\\c\xBB\xE7\xBF\xEB \xB1\xE2\xB4\xC9\xBF\xA1 \xB4\xEB\xC7\xD1 \xC1\xA6\xBE\xE0\\c0xff000000\\c\xC0\xCC \xBB\xFD\xB1\xE9\xB4\xCF\xB4\xD9.\n\n\n\n\n\n\n\n"
					"\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
			}
		} else {
			pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
			pForm->SetMessage("\xB0\xD4\xC0\xD3\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		}

		pForm->Open((FRESH_RESULT_CALLBACK)&FrQuitForm::OnExitConfirmResult, 1);
	} else {
		g_bQuit = true;
	}
}

// quitwindow.cpp:286
void FrQuitForm::OnCloseBtnUp(void) {
	Close(FrNONE, true);
}

// quitwindow.cpp:392
bool FrQuitForm::OnExitConfirmResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		if (OnlinePlay()) {
			WSendPacket packet((enumClientPacket)0xF);
			packet.Encode1(1);
			packet.Encode2(0xFFFF);

			packet.Encode8(0);
			packet.Encode8(0);

			packet.Send(SEND_TO_DEFAULT);
		}

		Close(FrNONE, true);
		g_bQuit = true;
	}

	return true;
}

// quitwindow.cpp:415
void FrQuitForm::EnableOptionBtn(bool enable) {
	if (m_pOptionBtn) {
		m_pOptionBtn->Enable(enable);
	}
}

// quitwindow.cpp:423
CQuitWindow::CQuitWindow(void) : m_pQuitForm(0), m_pOptionWindow(0), m_cursorActive(true) {
}

// quitwindow.cpp:427
CQuitWindow::~CQuitWindow(void) {
}

// quitwindow.cpp:431
void CQuitWindow::Process(void) {
	if (!(GOLFDOC()->GameMode() & 0x2030) && !g_pFresh->GetManager()->HasEscKeyWindow()) {
		if (!m_pQuitForm) {
			m_pQuitForm = CreateForm<FrQuitForm>(g_pFresh->GetManager(), this, "quitdlg", 0);
			m_pQuitForm->Open((FRESH_RESULT_CALLBACK)&CQuitWindow::OnQuitFormResult, 1);
			m_pQuitForm->MoveCursor("tolobby");
			COption::Instance()->SetQuitWindowed(true);

			if (GOLFDOC()->m_gameState < 15) {
				m_cursorActive = CMouseCursor::Instance()->IsActive();
				CMouseCursor::Instance()->SetActive(true);
			}
		}
	}
}

// quitwindow.cpp:457
bool CQuitWindow::IsActive(void) {
	return m_pQuitForm ? true : false;
}

// quitwindow.cpp:462
bool CQuitWindow::OnQuitFormResult(int ret, FrForm* pForm) {
	COption::Instance()->SetQuitWindowed(false);

	CGolfDoc* pGolfDoc = GOLFDOC();
	if (pGolfDoc && pGolfDoc->GameMode() == 0x200) {
		if (COption::Instance()->gGetTerrainTooltip()) {
			CMouseCursor::Instance()->SetTerrainToolTip(true);
		}
	}

	if (GOLFDOC()->m_gameState < 15) {
		CMouseCursor::Instance()->SetActive(m_cursorActive);
	}

	m_pQuitForm = 0;
	return true;
}

// quitwindow.cpp:481
void CQuitWindow::CreateOptionWindow(void) {
	if (m_pQuitForm) {
		m_pQuitForm->EnableOptionBtn(false);
	}

	if (!m_pOptionWindow) {
		CMouseCursor::Instance()->m_uiEnable[0] = false;

		m_pOptionWindow = CreateForm<FrOptionDlg>(g_pFresh->GetManager(), this, "option", 0);
		m_pOptionWindow->Open((FRESH_RESULT_CALLBACK)&CQuitWindow::OnOptionResult, 0x11);
		m_pOptionWindow->SetApplyScrSize(true);
		m_pOptionWindow->SetCloseResult(FrCANCEL);

		if (m_pQuitForm) {
			m_pOptionWindow->SetRect(m_pQuitForm->GetOptionBtnRect());
		} else {
			m_pOptionWindow->SetRect(WRect(g_view->GetWidth() - 64.0f, 181.0f, 64.0f, 24.0f));
		}
	}
}

// quitwindow.cpp:502
bool CQuitWindow::OnOptionResult(int ret, FrForm* pForm) {
	m_pOptionWindow = 0;
	if (m_pQuitForm) {
		m_pQuitForm->EnableOptionBtn(true);
	}

	CMouseCursor::Instance()->m_uiEnable[0] = true;

	return true;
}
