// Original: d:\build\custom\temp\code\client\projectg\kbdmanualdlg.cpp
#include "KbdManualDlg.h"
#include "ObjectFactory.h"

// kbdmanualdlg.cpp:15 (run-time type and factory registration of FrKbdManualDlg)
const WRTTI FrKbdManualDlg::m_RTTI("FrKbdManualDlg", &FrForm::m_RTTI);

IObject* FrKbdManualDlgMakeInstance(void) {
	return new FrKbdManualDlg;
}

struct __sFrKbdManualDlg {
	__sFrKbdManualDlg(void) {
		ObjectFactory().AddObjectFunctor(FrKbdManualDlgMakeInstance, "FrKbdManualDlg");
	}
};
static __sFrKbdManualDlg s_registerFrKbdManualDlg;

// kbdmanualdlg.cpp:17
const sFRESH_MSGMAP* FrKbdManualDlg::GetMessageMap(void) const {
	return &FrKbdManualDlg::_MsgMap;
}

sFRESH_MSGMAP FrKbdManualDlg::_MsgMap = { &FrForm::_MsgMap, &FrKbdManualDlg::_MsgEntries[0] };

sFRESH_ENTRY FrKbdManualDlg::_MsgEntries[] = {
	// kbdmanualdlg.cpp:19
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrKbdManualDlg::OnCloseBtnUp },

	// kbdmanualdlg.cpp:21
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// kbdmanualdlg.cpp:25
void FrKbdManualDlg::OnCloseBtnUp(void) {
	OnFreshOkay();
}
