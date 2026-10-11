// Original: d:\build\custom\temp\code\client\projectg\helpdlg.cpp
#include "HelpDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frviewer.h"
#include "frscrollbar.h"
#include "../client/Wangreal/include/wview.h"

#include <math.h>

// helpdlg.cpp:18 (run-time type and factory registration of FrHelpDlg)
const WRTTI FrHelpDlg::m_RTTI("FrHelpDlg", &FrForm::m_RTTI);

IObject* FrHelpDlgMakeInstance(void) {
	return new FrHelpDlg;
}

struct __sFrHelpDlg {
	__sFrHelpDlg(void) {
		ObjectFactory().AddObjectFunctor(FrHelpDlgMakeInstance, "FrHelpDlg");
	}
};
static __sFrHelpDlg s_registerFrHelpDlg;

// helpdlg.cpp:20
const sFRESH_MSGMAP* FrHelpDlg::GetMessageMap(void) const {
	return &FrHelpDlg::_MsgMap;
}

sFRESH_MSGMAP FrHelpDlg::_MsgMap = { &FrForm::_MsgMap, &FrHelpDlg::_MsgEntries[0] };

sFRESH_ENTRY FrHelpDlg::_MsgEntries[] = {
	// helpdlg.cpp:22
	{ "title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHelpDlg::OnTitleInit },
	// helpdlg.cpp:23
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHelpDlg::OnViewInit },
	// helpdlg.cpp:24
	{ "view", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrHelpDlg::OnViewBtnUp },

	// helpdlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// helpdlg.cpp:29
bool FrHelpDlg::OnInit(void) {
	if (m_pView == 0)
		return FrForm::OnInit();

	if (m_helpType == 0) {
		m_pView->Open("cha_up_help.jpg");

		if (m_pTitle)
			m_pTitle->SetBgImg("cha_up_help");
	}
	else if (m_helpType == 2) {
		m_pView->Open("quest_help.jpg");

		if (m_pTitle)
			m_pTitle->SetBgImg("quest_help");
	}
	else {
		m_pView->Open("club_up_help.jpg");

		if (m_pTitle)
			m_pTitle->SetBgImg("club_up");
	}

	WRect rect = GetRect();

	WRect viewRect = m_pView->GetRect();
	viewRect.w = m_pView->GetSrcWidth() + 5.0f;
	m_pView->SetRect(viewRect);

	if (m_pView->GetSrcWidth() + 30.0f > rect.w) {
		// keep the cancel button at its distance from the right edge
		FrWnd* pCancel = FindChildByName("cancel");

		WRect cancelRect;
		if (pCancel) {

			cancelRect = pCancel->GetRect();
			cancelRect.x = m_pView->GetSrcWidth() + 30.0f - (rect.w - cancelRect.x);
			pCancel->SetRect(cancelRect);
		}

		rect.w = m_pView->GetSrcWidth() + 30.0f;
		rect.x = (float)floor((g_view->GetWidth() - rect.w) * 0.5f);
	}

	rect.h = m_pView->GetSrcHeight() + 80.0f;
	rect.y = (float)floor((g_view->GetHeight() - rect.h) * 0.5f);

	SetRect(rect);

	return FrForm::OnInit();
}

// helpdlg.cpp:87
void FrHelpDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrArea, param);
}

// helpdlg.cpp:92
void FrHelpDlg::OnViewInit(intptr_t param) {
	m_pView = DYNAMIC_CAST(FrViewer, param);
	if (m_pView) {


		m_pView->GetScrollBar()->SetVisible(false);
	}
}

// helpdlg.cpp:102
void FrHelpDlg::OnViewBtnUp(void) {
	Close(true);
}
