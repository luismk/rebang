// Original: d:\build\custom\temp\code\client\projectg\christmassockeventdlg.cpp
#include "ChristmasSockEventDlg.h"
#include "ObjectFactory.h"
#include "frviewer.h"

// christmassockeventdlg.cpp:8 (run-time type and factory registration of FrChristmasSockEventDescDlg)
const WRTTI FrChristmasSockEventDescDlg::m_RTTI("FrChristmasSockEventDescDlg", &FrForm::m_RTTI);

IObject* FrChristmasSockEventDescDlgMakeInstance(void) {
	return new FrChristmasSockEventDescDlg;
}

struct __sFrChristmasSockEventDescDlg {
	__sFrChristmasSockEventDescDlg(void) {
		ObjectFactory().AddObjectFunctor(FrChristmasSockEventDescDlgMakeInstance, "FrChristmasSockEventDescDlg");
	}
};
static __sFrChristmasSockEventDescDlg s_registerFrChristmasSockEventDescDlg;

// christmassockeventdlg.cpp:10
const sFRESH_MSGMAP* FrChristmasSockEventDescDlg::GetMessageMap(void) const {
	return &FrChristmasSockEventDescDlg::_MsgMap;
}

sFRESH_MSGMAP FrChristmasSockEventDescDlg::_MsgMap = { &FrForm::_MsgMap, &FrChristmasSockEventDescDlg::_MsgEntries[0] };

sFRESH_ENTRY FrChristmasSockEventDescDlg::_MsgEntries[] = {
	// christmassockeventdlg.cpp:12
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasSockEventDescDlg::OnInitDescView },

	// christmassockeventdlg.cpp:14
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// christmassockeventdlg.cpp:17
FrChristmasSockEventDescDlg::FrChristmasSockEventDescDlg(void) {
	m_pDescView = 0;
}

// christmassockeventdlg.cpp:22
FrChristmasSockEventDescDlg::~FrChristmasSockEventDescDlg(void) {
}

// christmassockeventdlg.cpp:26
void FrChristmasSockEventDescDlg::OnInitDescView(intptr_t param) {
	m_pDescView = DYNAMIC_CAST(FrViewer, param);

	if (m_pDescView) {

		m_pDescView->ShowScrollBar(false);
		m_pDescView->Open("2010_Christmas_Event_Popup.jpg");
	}
}
