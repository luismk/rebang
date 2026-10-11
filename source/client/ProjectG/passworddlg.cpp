// Original: d:\build\custom\temp\code\client\projectg\passworddlg.cpp
#include "PasswordDlg.h"
#include "ObjectFactory.h"
#include "fredit.h"

// passworddlg.cpp:20 (run-time type and factory registration of FrPasswordDlg)
const WRTTI FrPasswordDlg::m_RTTI("FrPasswordDlg", &FrForm::m_RTTI);

IObject* FrPasswordDlgMakeInstance(void) {
	return new FrPasswordDlg;
}

struct __sFrPasswordDlg {
	__sFrPasswordDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPasswordDlgMakeInstance, "FrPasswordDlg");
	}
};
static __sFrPasswordDlg s_registerFrPasswordDlg;

// passworddlg.cpp:22
const sFRESH_MSGMAP* FrPasswordDlg::GetMessageMap(void) const {
	return &FrPasswordDlg::_MsgMap;
}

sFRESH_MSGMAP FrPasswordDlg::_MsgMap = { &FrForm::_MsgMap, &FrPasswordDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPasswordDlg::_MsgEntries[] = {
	// passworddlg.cpp:24
	{ "password", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordDlg::OnPasswordInit },
	// passworddlg.cpp:25
	{ "password", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrPasswordDlg::OnPasswordEnterKey },
	// passworddlg.cpp:26
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordDlg::OnOkBtnUp },

	// passworddlg.cpp:28
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// passworddlg.cpp:32
FrPasswordDlg::FrPasswordDlg(void) {
	m_pPassword = 0;
}

// passworddlg.cpp:36
void FrPasswordDlg::InitContent(void) {
	// "Please enter the last 7 digits of your resident registration number
	// to prevent misuse by others."
	SetMessage("\xC5\xB8\xC0\xCE\xC0\xC7 \xBE\xC7\xBF\xEB\xC0\xBB \xB9\xE6\xC1\xF6\xC7\xCF\xB1\xE2 \xC0\xA7\xC7\xCF\xBF\xA9 \xC1\xD6\xB9\xCE\xB9\xF8\xC8\xA3 \xB5\xDA\xC0\xC7 7\xC0\xDA\xB8\xAE\xB8\xA6 \xC0\xD4\xB7\xC2\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", false);

	m_pPassword->SetCharLimit(7, false);
}

// passworddlg.cpp:59
void FrPasswordDlg::OnPasswordInit(intptr_t param) {
	m_pPassword = DYNAMIC_CAST(FrEdit, param);
	m_pPassword->SetKeyFocus(true);
}

// passworddlg.cpp:69
bool FrPasswordDlg::OnPasswordEnterKey(int param) {
	if (m_pPassword && !m_pPassword->GetStyle().GetFlag(0x2)) {
		OnFreshOkay();
	}
	return false;
}

// passworddlg.cpp:77
void FrPasswordDlg::OnOkBtnUp(void) {
	OnFreshOkay();
}
