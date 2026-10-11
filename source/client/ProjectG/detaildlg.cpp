// Original: d:\build\custom\temp\code\client\projectg\detaildlg.cpp
#include "DetailDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "frarea.h"
#include "frviewer.h"

// detaildlg.cpp:15 (run-time type and factory registration of NtDetailDlg)
const WRTTI NtDetailDlg::m_RTTI("NtDetailDlg", &FrForm::m_RTTI);

IObject* NtDetailDlgMakeInstance(void) {
	return new NtDetailDlg;
}

struct __sNtDetailDlg {
	__sNtDetailDlg(void) {
		ObjectFactory().AddObjectFunctor(NtDetailDlgMakeInstance, "NtDetailDlg");
	}
};
static __sNtDetailDlg s_registerNtDetailDlg;

// detaildlg.cpp:17
const sFRESH_MSGMAP* NtDetailDlg::GetMessageMap(void) const {
	return &NtDetailDlg::_MsgMap;
}

sFRESH_MSGMAP NtDetailDlg::_MsgMap = { &FrForm::_MsgMap, &NtDetailDlg::_MsgEntries[0] };

sFRESH_ENTRY NtDetailDlg::_MsgEntries[] = {
	// detaildlg.cpp:19
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtDetailDlg::OnCloseBtnInit },
	// detaildlg.cpp:20
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtDetailDlg::OnCloseBtnUp },
	// detaildlg.cpp:21
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtDetailDlg::OnCaptionInit },
	// detaildlg.cpp:22
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtDetailDlg::OnViewInit },

	// detaildlg.cpp:24
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// detaildlg.cpp:28
NtDetailDlg::NtDetailDlg(void) {
}

// detaildlg.cpp:33
NtDetailDlg::~NtDetailDlg(void) {
}

// detaildlg.cpp:38
void NtDetailDlg::OnCloseBtnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// detaildlg.cpp:44
void NtDetailDlg::OnCloseBtnUp(void) {
	OnFreshOkay();
}

// detaildlg.cpp:50
void NtDetailDlg::OnCaptionInit(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// detaildlg.cpp:58
void NtDetailDlg::OnViewInit(intptr_t param) {
	m_pView = DYNAMIC_CAST(FrViewer, param);

	if (m_pView) {
		m_pView->Open("notice_popup08.jpg");
		m_pView->SetKeyFocus(true);
	}
}
